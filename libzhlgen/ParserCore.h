#pragma once

#include <array>
#include <unordered_map>

#include "CodeEmitter.h"
#include "ParserDefinitions.h"

#include "ZHLLexer.h"
#include "ZHLParser.h"
#include "ZHLParserBaseListener.h"
#include "ZHLParserBaseVisitor.h"

class Parser : public ZHLParserBaseVisitor {
public:
    Parser(Namespace* global, TypeMap* types, AsmDefMap* asmDefs,
        std::string const& filename);
    virtual std::any visitZhl(ZHLParser::ZhlContext* ctx);
    virtual std::any visitFunction(ZHLParser::FunctionContext* ctx);
    virtual std::any visitReference(ZHLParser::ReferenceContext* ctx);
    virtual std::any visitAsmDef(ZHLParser::AsmDefContext* ctx);

    /// _currentFunction assumed not null
    virtual std::any visitFunArgs(ZHLParser::FunArgsContext* ctx);
    virtual std::any visitFunArg(ZHLParser::FunArgContext* ctx);
    virtual std::any visitArgParam(ZHLParser::ArgParamContext* ctx);
    virtual std::any visitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext* ctx);
    virtual std::any visitOptNamedFunArg(ZHLParser::OptNamedFunArgContext* ctx);
    virtual std::any visitTemplateSpec(ZHLParser::TemplateSpecContext* ctx);
    virtual std::any visitType(ZHLParser::TypeContext* ctx);
    virtual std::any visitTypeSpecifier(ZHLParser::TypeSpecifierContext* ctx);
    virtual std::any visitSimpleType(ZHLParser::SimpleTypeContext* ctx);
    virtual std::any visitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext* ctx);
    virtual std::any visitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext* ctx);
    virtual std::any visitNestedName(ZHLParser::NestedNameContext* ctx);
    virtual std::any visitPointerAttribute(ZHLParser::PointerAttributeContext* ctx);
    virtual std::any visitGenericCode(ZHLParser::GenericCodeContext* ctx);
    virtual std::any visitClass(ZHLParser::ClassContext* ctx);
    virtual std::any visitDepends(ZHLParser::DependsContext* ctx);
    virtual std::any visitClassBody(ZHLParser::ClassBodyContext* ctx);
    virtual std::any visitClassSignature(ZHLParser::ClassSignatureContext* ctx);
    virtual std::any visitClassFunction(ZHLParser::ClassFunctionContext* ctx);
    virtual std::any visitClassField(ZHLParser::ClassFieldContext* ctx);
    virtual std::any visitInnerField(ZHLParser::InnerFieldContext* ctx);
    virtual std::any visitInnerFieldPtr(ZHLParser::InnerFieldPtrContext* context);
    virtual std::any visitFullName(ZHLParser::FullNameContext* ctx);
    virtual std::any visitSignature(ZHLParser::SignatureContext* ctx);
    virtual std::any visitForwardDecl(ZHLParser::ForwardDeclContext* ctx);
    virtual std::any visitTypedef(ZHLParser::TypedefContext* ctx);
    virtual std::any visitFunctionPtr(ZHLParser::FunctionPtrContext* ctx);
    virtual std::any visitFptr(ZHLParser::FptrContext* ctx);
    virtual std::any visitMemberPtr(ZHLParser::MemberPtrContext* ctx);
    virtual std::any visitInheritance(ZHLParser::InheritanceContext* ctx);
    virtual std::any visitInheritanceDecl(ZHLParser::InheritanceDeclContext* ctx);
    virtual std::any visitTypeInfo(ZHLParser::TypeInfoContext* ctx);
    virtual std::any visitTypeInfoDef(ZHLParser::TypeInfoDefContext* ctx);
    virtual std::any visitVtable(ZHLParser::VtableContext* ctx);
    virtual std::any visitVtableEntry(ZHLParser::VtableEntryContext* ctx);
    virtual std::any visitVtableSignature(ZHLParser::VtableSignatureContext* ctx);
    virtual std::any visitExternalFunc(ZHLParser::ExternalFuncContext* ctx);
    virtual std::any visitFunctionName(ZHLParser::FunctionNameContext* ctx);

    inline bool HasFatalError() const { return _hasFatalError; }

private:
    Namespace* _global;
    std::string _currentFile;
    Struct* _currentStruct = nullptr;
    Variable* _currentVariable = nullptr;
    Function* _currentFunction = nullptr;
    FunctionParam* _currentParam = nullptr;
    Signature* _currentSignature = nullptr;
    VariableSignature* _currentReference = nullptr;
    TypeMap* _types;
    AsmDefMap* _asmDefs;

    std::string GetCurrentFunctionQualifiedName(std::string const& name);
    void WarnRepeatedFunctionQualifier(std::string const& name, std::string const& qualifier);
    std::string MergeNameParts(std::vector<std::string> const& parts);
    std::string GetContext();
    void ReadQualifiers(std::vector<antlr4::tree::TerminalNode*> const& qualifiers, Function& fn);
    std::string ReadSignature(std::string const& sig);

    bool _hasFatalError = false;
};