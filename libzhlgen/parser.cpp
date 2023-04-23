#include <filesystem>
#include <iostream>

#include "ZHLLexer.h"
#include "ZHLParser.h"
#include "ZHLParserBaseListener.h"
#include "ZHLParserBaseVisitor.h"

namespace fs = std::filesystem;

struct Namespace {
    std::list<Namespace> children;
};

class Parser : public ZHLParserBaseVisitor {
public:
    virtual std::any visitZhl(ZHLParser::ZhlContext* ctx) override {
        return 0;
    }

    virtual std::any visitTesta(ZHLParser::TestaContext* ctx) override {
        return 0;
    }

    virtual std::any visitTestb(ZHLParser::TestbContext* ctx) override {
        return 0;
    }

    virtual std::any visitFunction(ZHLParser::FunctionContext* ctx) override {
        return 0;
    }

    virtual std::any visitReference(ZHLParser::ReferenceContext* ctx) override {
        return 0;
    }

    virtual std::any visitFunArgs(ZHLParser::FunArgsContext* ctx) override {
        return 0;
    }

    virtual std::any visitFunArg(ZHLParser::FunArgContext* ctx) override {
        return 0;
    }

    virtual std::any visitArgParam(ZHLParser::ArgParamContext* ctx) override {
        return 0;
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
        return 0;
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

	return 0;
}
