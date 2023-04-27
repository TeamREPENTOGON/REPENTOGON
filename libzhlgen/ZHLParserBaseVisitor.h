
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ZHLParserVisitor.h"


/**
 * This class provides an empty implementation of ZHLParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ZHLParserBaseVisitor : public ZHLParserVisitor {
public:

  virtual std::any visitZhl(ZHLParser::ZhlContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(ZHLParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReference(ZHLParser::ReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunArgs(ZHLParser::FunArgsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunArg(ZHLParser::FunArgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgParam(ZHLParser::ArgParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOptNamedFunArg(ZHLParser::OptNamedFunArgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTemplateSpec(ZHLParser::TemplateSpecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitType(ZHLParser::TypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeSpecifier(ZHLParser::TypeSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleType(ZHLParser::SimpleTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNestedName(ZHLParser::NestedNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPointerAttribute(ZHLParser::PointerAttributeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitGenericCode(ZHLParser::GenericCodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClass(ZHLParser::ClassContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInheritance(ZHLParser::InheritanceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInheritanceDecl(ZHLParser::InheritanceDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDepends(ZHLParser::DependsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassBody(ZHLParser::ClassBodyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassBodyElement(ZHLParser::ClassBodyElementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassSignature(ZHLParser::ClassSignatureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassFunction(ZHLParser::ClassFunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitClassField(ZHLParser::ClassFieldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInnerField(ZHLParser::InnerFieldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFullName(ZHLParser::FullNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignature(ZHLParser::SignatureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitForwardDecl(ZHLParser::ForwardDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypedef(ZHLParser::TypedefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionPtr(ZHLParser::FunctionPtrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFptr(ZHLParser::FptrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMemberPtr(ZHLParser::MemberPtrContext *ctx) override {
    return visitChildren(ctx);
  }


};

