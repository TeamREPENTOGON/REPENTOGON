
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ZHLParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ZHLParser.
 */
class  ZHLParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterZhl(ZHLParser::ZhlContext *ctx) = 0;
  virtual void exitZhl(ZHLParser::ZhlContext *ctx) = 0;

  virtual void enterFunction(ZHLParser::FunctionContext *ctx) = 0;
  virtual void exitFunction(ZHLParser::FunctionContext *ctx) = 0;

  virtual void enterReference(ZHLParser::ReferenceContext *ctx) = 0;
  virtual void exitReference(ZHLParser::ReferenceContext *ctx) = 0;

  virtual void enterFunArgs(ZHLParser::FunArgsContext *ctx) = 0;
  virtual void exitFunArgs(ZHLParser::FunArgsContext *ctx) = 0;

  virtual void enterFunArg(ZHLParser::FunArgContext *ctx) = 0;
  virtual void exitFunArg(ZHLParser::FunArgContext *ctx) = 0;

  virtual void enterArgParam(ZHLParser::ArgParamContext *ctx) = 0;
  virtual void exitArgParam(ZHLParser::ArgParamContext *ctx) = 0;

  virtual void enterOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext *ctx) = 0;
  virtual void exitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext *ctx) = 0;

  virtual void enterOptNamedFunArg(ZHLParser::OptNamedFunArgContext *ctx) = 0;
  virtual void exitOptNamedFunArg(ZHLParser::OptNamedFunArgContext *ctx) = 0;

  virtual void enterTemplateSpec(ZHLParser::TemplateSpecContext *ctx) = 0;
  virtual void exitTemplateSpec(ZHLParser::TemplateSpecContext *ctx) = 0;

  virtual void enterType(ZHLParser::TypeContext *ctx) = 0;
  virtual void exitType(ZHLParser::TypeContext *ctx) = 0;

  virtual void enterTypeSpecifier(ZHLParser::TypeSpecifierContext *ctx) = 0;
  virtual void exitTypeSpecifier(ZHLParser::TypeSpecifierContext *ctx) = 0;

  virtual void enterSimpleType(ZHLParser::SimpleTypeContext *ctx) = 0;
  virtual void exitSimpleType(ZHLParser::SimpleTypeContext *ctx) = 0;

  virtual void enterSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext *ctx) = 0;
  virtual void exitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext *ctx) = 0;

  virtual void enterSimpleTypeLength(ZHLParser::SimpleTypeLengthContext *ctx) = 0;
  virtual void exitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext *ctx) = 0;

  virtual void enterNestedName(ZHLParser::NestedNameContext *ctx) = 0;
  virtual void exitNestedName(ZHLParser::NestedNameContext *ctx) = 0;

  virtual void enterPointerAttribute(ZHLParser::PointerAttributeContext *ctx) = 0;
  virtual void exitPointerAttribute(ZHLParser::PointerAttributeContext *ctx) = 0;

  virtual void enterGenericCode(ZHLParser::GenericCodeContext *ctx) = 0;
  virtual void exitGenericCode(ZHLParser::GenericCodeContext *ctx) = 0;

  virtual void enterClass(ZHLParser::ClassContext *ctx) = 0;
  virtual void exitClass(ZHLParser::ClassContext *ctx) = 0;

  virtual void enterInheritance(ZHLParser::InheritanceContext *ctx) = 0;
  virtual void exitInheritance(ZHLParser::InheritanceContext *ctx) = 0;

  virtual void enterDepends(ZHLParser::DependsContext *ctx) = 0;
  virtual void exitDepends(ZHLParser::DependsContext *ctx) = 0;

  virtual void enterClassBody(ZHLParser::ClassBodyContext *ctx) = 0;
  virtual void exitClassBody(ZHLParser::ClassBodyContext *ctx) = 0;

  virtual void enterClassSignature(ZHLParser::ClassSignatureContext *ctx) = 0;
  virtual void exitClassSignature(ZHLParser::ClassSignatureContext *ctx) = 0;

  virtual void enterClassFunction(ZHLParser::ClassFunctionContext *ctx) = 0;
  virtual void exitClassFunction(ZHLParser::ClassFunctionContext *ctx) = 0;

  virtual void enterClassField(ZHLParser::ClassFieldContext *ctx) = 0;
  virtual void exitClassField(ZHLParser::ClassFieldContext *ctx) = 0;

  virtual void enterInnerField(ZHLParser::InnerFieldContext *ctx) = 0;
  virtual void exitInnerField(ZHLParser::InnerFieldContext *ctx) = 0;

  virtual void enterFullName(ZHLParser::FullNameContext *ctx) = 0;
  virtual void exitFullName(ZHLParser::FullNameContext *ctx) = 0;

  virtual void enterSignature(ZHLParser::SignatureContext *ctx) = 0;
  virtual void exitSignature(ZHLParser::SignatureContext *ctx) = 0;

  virtual void enterForwardDecl(ZHLParser::ForwardDeclContext *ctx) = 0;
  virtual void exitForwardDecl(ZHLParser::ForwardDeclContext *ctx) = 0;

  virtual void enterTypedef(ZHLParser::TypedefContext *ctx) = 0;
  virtual void exitTypedef(ZHLParser::TypedefContext *ctx) = 0;

  virtual void enterFunctionPtr(ZHLParser::FunctionPtrContext *ctx) = 0;
  virtual void exitFunctionPtr(ZHLParser::FunctionPtrContext *ctx) = 0;

  virtual void enterFptr(ZHLParser::FptrContext *ctx) = 0;
  virtual void exitFptr(ZHLParser::FptrContext *ctx) = 0;

  virtual void enterMemberPtr(ZHLParser::MemberPtrContext *ctx) = 0;
  virtual void exitMemberPtr(ZHLParser::MemberPtrContext *ctx) = 0;


};

