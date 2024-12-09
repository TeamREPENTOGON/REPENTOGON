
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ZHLParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ZHLParser.
 */
class  ZHLParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ZHLParser.
   */
    virtual std::any visitZhl(ZHLParser::ZhlContext *context) = 0;

    virtual std::any visitExternalFunc(ZHLParser::ExternalFuncContext *context) = 0;

    virtual std::any visitTypeInfo(ZHLParser::TypeInfoContext *context) = 0;

    virtual std::any visitTypeInfoDef(ZHLParser::TypeInfoDefContext *context) = 0;

    virtual std::any visitFunction(ZHLParser::FunctionContext *context) = 0;

    virtual std::any visitReference(ZHLParser::ReferenceContext *context) = 0;

    virtual std::any visitFunArgs(ZHLParser::FunArgsContext *context) = 0;

    virtual std::any visitFunArg(ZHLParser::FunArgContext *context) = 0;

    virtual std::any visitArgParam(ZHLParser::ArgParamContext *context) = 0;

    virtual std::any visitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext *context) = 0;

    virtual std::any visitOptNamedFunArg(ZHLParser::OptNamedFunArgContext *context) = 0;

    virtual std::any visitTemplateSpec(ZHLParser::TemplateSpecContext *context) = 0;

    virtual std::any visitType(ZHLParser::TypeContext *context) = 0;

    virtual std::any visitTypeSpecifier(ZHLParser::TypeSpecifierContext *context) = 0;

    virtual std::any visitSimpleType(ZHLParser::SimpleTypeContext *context) = 0;

    virtual std::any visitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext *context) = 0;

    virtual std::any visitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext *context) = 0;

    virtual std::any visitNestedName(ZHLParser::NestedNameContext *context) = 0;

    virtual std::any visitPointerAttribute(ZHLParser::PointerAttributeContext *context) = 0;

    virtual std::any visitGenericCode(ZHLParser::GenericCodeContext *context) = 0;

    virtual std::any visitClass(ZHLParser::ClassContext *context) = 0;

    virtual std::any visitInheritance(ZHLParser::InheritanceContext *context) = 0;

    virtual std::any visitInheritanceDecl(ZHLParser::InheritanceDeclContext *context) = 0;

    virtual std::any visitDepends(ZHLParser::DependsContext *context) = 0;

    virtual std::any visitClassBody(ZHLParser::ClassBodyContext *context) = 0;

    virtual std::any visitVtable(ZHLParser::VtableContext *context) = 0;

    virtual std::any visitVtableEntry(ZHLParser::VtableEntryContext *context) = 0;

    virtual std::any visitVtableSignature(ZHLParser::VtableSignatureContext *context) = 0;

    virtual std::any visitClassSignature(ZHLParser::ClassSignatureContext *context) = 0;

    virtual std::any visitClassFunction(ZHLParser::ClassFunctionContext *context) = 0;

    virtual std::any visitFunctionName(ZHLParser::FunctionNameContext *context) = 0;

    virtual std::any visitClassField(ZHLParser::ClassFieldContext *context) = 0;

    virtual std::any visitInnerField(ZHLParser::InnerFieldContext *context) = 0;

    virtual std::any visitInnerFieldPtr(ZHLParser::InnerFieldPtrContext *context) = 0;

    virtual std::any visitFullName(ZHLParser::FullNameContext *context) = 0;

    virtual std::any visitSignature(ZHLParser::SignatureContext *context) = 0;

    virtual std::any visitForwardDecl(ZHLParser::ForwardDeclContext *context) = 0;

    virtual std::any visitTypedef(ZHLParser::TypedefContext *context) = 0;

    virtual std::any visitFunctionPtr(ZHLParser::FunctionPtrContext *context) = 0;

    virtual std::any visitFptr(ZHLParser::FptrContext *context) = 0;

    virtual std::any visitMemberPtr(ZHLParser::MemberPtrContext *context) = 0;


};

