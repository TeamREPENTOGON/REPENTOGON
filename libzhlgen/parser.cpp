#include <array>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <variant>

#include "ZHLLexer.h"
#include "ZHLParser.h"
#include "ZHLParserBaseListener.h"
#include "ZHLParserBaseVisitor.h"

namespace fs = std::filesystem;

enum PointerKind {
    LREF,
    RREF,
    POINTER
};

struct PointerDecl {
    bool _const;
    PointerKind _kind;
};

enum Signedness {
    SIGNED,
    UNSIGNED
};

enum Length {
    LONG,
    LONGLONG,
    SHORT
};

enum BasicTypes {
    CHAR,
    INT,
    BOOL,
    VOID,
    FLOAT,
    DOUBLE
};

struct BasicType {
    std::optional<Signedness> _sign;
    std::array<std::optional<Length>, 2> _length;
    BasicTypes _type;
};

struct FunctionPtr;
struct Struct;

struct Type {
    bool _const;
    // Basic type represents the char, int, bool etc. types and their composites.
    // std::string represents a type that is composed of a single word (minus pointers) that
    // can be a typedef or the name of a class.
    // unique_ptr<Struct> represent a type defined in a structure.
    // unique_ptr<FunctionPtr> represents a pointer to function. unique_ptr used because FunctionPtr
    // contains a Type, which would create an infinite structure if it was added here without an
    // indirection.
    // Nested names are not allowed because properly processing them would require interpreting the files.
    std::variant<BasicType, FunctionPtr*, Struct*, std::string> _name;
    std::optional<std::vector<PointerDecl>> _pointerDecl;
};

struct FunctionPtr {
    Type _ret;
    std::vector<Type> _parameters;
};

enum NamespaceKind {
    GLOBAL,
    CLASS
};

enum FunctionKind {
    FUNCTION,
    METHOD
};

enum Registers {
    EAX,
    EBX,
    ECX,
    EDX,
    EDI,
    ESI,
    ESP,
    EBP,
    XMM0,
    XMM1,
    XMM2,
    XMM3,
    XMM4,
    XMM5,
    XMM6,
    XMM7,
    MAX
};

enum FunctionQualifiers {
    STATIC = 1 << 0,
    VIRTUAL = 1 << 1,
    CLEANUP = 1 << 2
};

enum CallingConventions {
    CDECL,
    STDCALL,
    FASTCALL,
    THISCALL
};

struct FunctionParam {
    Type _type;
    std::string _name;
    std::optional<Registers> _reg;
};

struct Function {
    uint32_t _qualifiers;
    FunctionKind _kind;
    std::optional<CallingConventions> _convention;
    Type _ret;
    std::string _name;
    std::vector<FunctionParam> _params;

    bool IsVirtual() const {
        return _qualifiers & VIRTUAL;
    }

    bool IsCleanup() const {
        return _qualifiers & CLEANUP || (_convention && *_convention == CDECL);
    }

    bool IsStatic() const {
        return _qualifiers & STATIC;
    }
};

struct Variable {
    Type _type;
    std::string _name;
};

struct Signature {
    std::string _sig;
    Function _function;
};

struct VariableSignature {
    std::string _sig;
    Variable _variable;
};

struct Namespace {
    // std::vector<Namespace> _children;
    std::vector<Function> _functions;
    std::vector<Variable> _fields;
    std::vector<Signature> _signatures;
};

struct Struct {
    Namespace _namespace;
    std::string _name;
    /*
     * A struct is pending if its name has been encountered but no definition has 
     * been found yet. This can happen if the name is used as the return type of 
     * a function, or used as a dependency.
     * If a struct is still pending once all files have been processed, the name is 
     * considered invalid and discarded.
     */
    bool _pending = true; 
};

typedef std::string GenericCode;

class ErrorLogger : public std::ostringstream {
public:
    struct EndToken { };
    static EndToken _end;

    template<typename T>
    ErrorLogger& operator<<(T const& t) {
        _buf << t;
        return *this;
    }

    static ErrorLogger& endl(ErrorLogger& logger) {
        logger.newline();
        return logger;
    }

    ErrorLogger& operator<<(ErrorLogger& (*v)(ErrorLogger&)) {
        return v(*this);
    }

    ErrorLogger& operator<<(EndToken const& token) {
        _errors.push_back(_buf.str());
        _buf.clear();
        return *this;
    }

    void newline() {
        _buf << std::endl;
    }
private:
    std::vector<std::string> _errors;
    std::ostringstream _buf;
};

ErrorLogger::EndToken ErrorLogger::_end;

class Parser : public ZHLParserBaseVisitor {
public:
    virtual std::any visitZhl(ZHLParser::ZhlContext* ctx) override {
        return visitChildren(ctx);
    }

    virtual std::any visitFunction(ZHLParser::FunctionContext* ctx) override {
        Function fn;

        std::vector<std::string> parts = std::any_cast<std::vector<std::string>>(visit(ctx->nestedName()));
        fn._name = MergeNameParts(parts);

        std::optional<std::string> convention;
        if (auto node = ctx->CallingConvention()) {
            convention = node->getText();
            if (convention == "__stdcall") {
                fn._convention = CallingConventions::STDCALL;
            }
            else if (convention == "__fastcall") {
                fn._convention = CallingConventions::FASTCALL;
            }
            else if (convention == "__cdecl") {
                fn._convention = CallingConventions::CDECL;
            }
            else if (convention == "__thiscall") {
                if (!_currentStruct) {
                    if (ctx->nestedName()->Name().size() == 1) {
                        _errors << "[ERROR] __thiscall convention applied to non member function " << fn._name << ErrorLogger::endl; // << ErrorLogger::_end;
                        exit(-1);
                    }
                }

                fn._convention = CallingConventions::THISCALL;
            }
        }

        for (antlr4::tree::TerminalNode* x : ctx->Qualifier()) {
            std::string text = x->getText();
            if (text == "virtual") {
                if (fn.IsVirtual()) {
                    WarnRepeatedFunctionQualifier(fn._name, text);
                } else {
                    fn._qualifiers |= FunctionQualifiers::VIRTUAL;
                }
            }
            else if (text == "static") {
                if (fn.IsStatic()) {
                    WarnRepeatedFunctionQualifier(fn._name, text);
                }
                else {
                    fn._qualifiers |= FunctionQualifiers::STATIC;
                }
            }
            else if (text == "cleanup") {
                if (fn.IsCleanup()) {
                    WarnRepeatedFunctionQualifier(fn._name, text);
                }
                else {
                    fn._qualifiers |= FunctionQualifiers::CLEANUP;
                }
            }

            fn._ret = std::any_cast<Type>(visit(ctx->type()));
        }

        if (fn.IsVirtual() && fn.IsStatic()) {
            _errors << "[ERROR] static and virtual qualifiers applied together to function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::endl << ErrorLogger::_end;
            exit(-1);
        }

        if (fn.IsStatic() && fn._convention && fn._convention == CallingConventions::THISCALL) {
            _errors << "[ERROR] __thiscall convention applied to static function " << GetCurrentFunctionQualifiedName(fn._name) << ErrorLogger::endl << ErrorLogger::_end;
            exit(-1);
        }

        fn._name = parts.back();
        if (auto args = ctx->funArgs()) {
            fn._params = std::any_cast<std::vector<FunctionParam>>(visit(ctx->funArgs()));
        }
        return 0;
    }

    virtual std::any visitReference(ZHLParser::ReferenceContext* ctx) override {
        return 0;
    }

    virtual std::any visitFunArgs(ZHLParser::FunArgsContext* ctx) override {
        std::vector<FunctionParam> params;
        typedef std::pair<bool, std::vector<unsigned int>> RegUse;
        std::array<RegUse, Registers::MAX> state = { { { false,  {} } } };

        int i = 0;
        for (ZHLParser::FunArgContext* arg : ctx->funArg()) {
            FunctionParam param = std::any_cast<FunctionParam>(visit(arg));
            if (param._reg) {
                RegUse& use = state[*param._reg];
                use.first = true;
                use.second.push_back(i);
            }
            params.push_back(param);
            ++i;
        }

        for (RegUse const& use : state) {
            if (use.first) {
                if (use.second.size() > 1) {
                    std::ostringstream err;
                    err << "[ERROR] Register ";
                }
            }
        }

        return params;
    }

    virtual std::any visitFunArg(ZHLParser::FunArgContext* ctx) override {
        FunctionParam param;
        param._type = std::any_cast<Type>(visit(ctx->type()));
        param._name = std::any_cast<std::string>(visit(ctx->fullName()));
        if (auto argParam = ctx->argParam()) {
            param._reg = std::any_cast<Registers>(visit(argParam));
        }

        return param;
    }

    virtual std::any visitArgParam(ZHLParser::ArgParamContext* ctx) override {
        std::string name = ctx->Register()->toString();

        std::map<std::string, Registers> assoc;
        assoc["eax"] = Registers::EAX;
        assoc["ebx"] = Registers::EBX;
        assoc["ecx"] = Registers::ECX;
        assoc["edx"] = Registers::EDX;
        assoc["esi"] = Registers::ESI;
        assoc["edi"] = Registers::EDI;
        assoc["esp"] = Registers::ESP;
        assoc["ebp"] = Registers::EBP;
        assoc["xmm0"] = Registers::XMM0;
        assoc["xmm1"] = Registers::XMM1;
        assoc["xmm2"] = Registers::XMM2;
        assoc["xmm3"] = Registers::XMM3;
        assoc["xmm4"] = Registers::XMM4;
        assoc["xmm5"] = Registers::XMM5;
        assoc["xmm6"] = Registers::XMM6;
        assoc["xmm7"] = Registers::XMM7;

        return assoc[name];
    }

    virtual std::any visitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext* ctx) override {
        return 0;
    }

    virtual std::any visitOptNamedFunArg(ZHLParser::OptNamedFunArgContext* ctx) override {
        return 0;
    }

    virtual std::any visitTemplateSpec(ZHLParser::TemplateSpecContext* ctx) override {
        return 0;
    }

    virtual std::any visitType(ZHLParser::TypeContext* ctx) override {
        return 0;
    }

    virtual std::any visitTypeSpecifier(ZHLParser::TypeSpecifierContext* ctx) override {
        return 0;
    }

    virtual std::any visitSimpleType(ZHLParser::SimpleTypeContext* ctx) override {
        return 0;
    }

    virtual std::any visitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext* ctx) override {
        return 0;
    }

    virtual std::any visitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext* ctx) override {
        return 0;
    }

    virtual std::any visitNestedName(ZHLParser::NestedNameContext* ctx) override {
        std::vector<antlr4::tree::TerminalNode*> parts = ctx->Name();
        std::vector<std::string> partsStr;
        for (auto x : parts)
            partsStr.push_back(x->toString());
        std::string type = partsStr[0];

        if (_types->find(type) == _types->end()) {
            Struct* s = new Struct;
            s->_name = type;
            (* _types)[type] = s;
        }

        return partsStr;
    }

    virtual std::any visitPointerAttribute(ZHLParser::PointerAttributeContext* ctx) override {
        return 0;
    }

    virtual std::any visitGenericCode(ZHLParser::GenericCodeContext* ctx) override {
        return 0;
    }

    virtual std::any visitClass(ZHLParser::ClassContext* ctx) override {
        return 0;
    }

    virtual std::any visitDepends(ZHLParser::DependsContext* ctx) override {
        return 0;
    }

    virtual std::any visitClassBody(ZHLParser::ClassBodyContext* ctx) override {
        return 0;
    }

    virtual std::any visitClassSignature(ZHLParser::ClassSignatureContext* ctx) override {
        return 0;
    }

    virtual std::any visitClassFunction(ZHLParser::ClassFunctionContext* ctx) override {
        return 0;
    }

    virtual std::any visitClassField(ZHLParser::ClassFieldContext* ctx) override {
        return 0;
    }

    virtual std::any visitInnerField(ZHLParser::InnerFieldContext* ctx) override {
        return 0;
    }

    virtual std::any visitFullName(ZHLParser::FullNameContext* ctx) override {
        return 0;
    }

    virtual std::any visitSignature(ZHLParser::SignatureContext* ctx) override {
        return 0;
    }

    virtual std::any visitForwardDecl(ZHLParser::ForwardDeclContext* ctx) override {
        return 0;
    }

    virtual std::any visitTypedef(ZHLParser::TypedefContext* ctx) override {
        return 0;
    }

    virtual std::any visitFunctionPtr(ZHLParser::FunctionPtrContext* ctx) override {
        return 0;
    }

    virtual std::any visitFptr(ZHLParser::FptrContext* ctx) override {
        return 0;
    }

    virtual std::any visitMemberPtr(ZHLParser::MemberPtrContext* ctx) override {
        return 0;
    }

    virtual std::any visitInheritance(ZHLParser::InheritanceContext* ctx) override {
        return 0;
    }

private:
    Namespace* _global;
    // std::vector<Struct*> _structs;
    std::string _currentFile;
    Struct* _currentStruct;
    ErrorLogger _errors;
    Function* _currentFunction = nullptr;
    std::map<std::string, Struct*>* _types;

    std::string GetCurrentFunctionQualifiedName(std::string const& name) {
        if (!_currentStruct) {
            return name;
        } else {
            return _currentStruct->_name + "::" + name;
        }
    }

    void WarnRepeatedFunctionQualifier(std::string const& name, std::string const& qualifier) {
        _errors << "[WARN] Repeated '" << qualifier << "' qualifier on function " << name << ErrorLogger::endl << ErrorLogger::_end;
    }

    std::string MergeNameParts(std::vector<std::string> const& parts) {
        std::ostringstream s;
        std::ostream_iterator<std::string> iter(s, "::");
        std::copy(parts.begin(), parts.end(), iter);
        return s.str();
    }
};

void process_file(fs::path const& path) {
    std::cout << "Processing " << path << std::endl;
    std::ifstream stream(path);

    if (!stream.is_open()) {
        return;
    }

    antlr4::ANTLRInputStream input_stream(stream);
    ZHLLexer lexer(&input_stream);
    antlr4::CommonTokenStream tokens(&lexer);
    ZHLParser parser(&tokens);

    antlr4::tree::ParseTree* tree = parser.zhl();
    Parser p2;
    p2.visit(tree);
}

int main(int argc, char** argv) {
	fs::path functions("../../libzhl/functions");
	for (fs::directory_entry const& entry : fs::directory_iterator(functions)) {
		//std::cout << "File " << entry.path() << std::endl;
		if (!entry.is_regular_file())
			continue;

		//std::cout << "Ext " << entry.path().extension() << std::endl;
		if (entry.path().extension() != std::string(".zhl"))
			continue;

        process_file(entry.path());
	}

    process_file("../../libzhlgen/IsaacRepentanceStripped.h");

    void (Parser::*foo)(int);

	return 0;
}
