
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ZHLParserListener.h"


/**
 * This class provides an empty implementation of ZHLParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ZHLParserBaseListener : public ZHLParserListener {
public:

  virtual void enterZhl(ZHLParser::ZhlContext * /*ctx*/) override { }
  virtual void exitZhl(ZHLParser::ZhlContext * /*ctx*/) override { }

  virtual void enterFunction(ZHLParser::FunctionContext * /*ctx*/) override { }
  virtual void exitFunction(ZHLParser::FunctionContext * /*ctx*/) override { }

  virtual void enterReference(ZHLParser::ReferenceContext * /*ctx*/) override { }
  virtual void exitReference(ZHLParser::ReferenceContext * /*ctx*/) override { }

  virtual void enterFunArgs(ZHLParser::FunArgsContext * /*ctx*/) override { }
  virtual void exitFunArgs(ZHLParser::FunArgsContext * /*ctx*/) override { }

  virtual void enterFunArg(ZHLParser::FunArgContext * /*ctx*/) override { }
  virtual void exitFunArg(ZHLParser::FunArgContext * /*ctx*/) override { }

  virtual void enterArgParam(ZHLParser::ArgParamContext * /*ctx*/) override { }
  virtual void exitArgParam(ZHLParser::ArgParamContext * /*ctx*/) override { }

  virtual void enterOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext * /*ctx*/) override { }
  virtual void exitOptNamedFunArgs(ZHLParser::OptNamedFunArgsContext * /*ctx*/) override { }

  virtual void enterOptNamedFunArg(ZHLParser::OptNamedFunArgContext * /*ctx*/) override { }
  virtual void exitOptNamedFunArg(ZHLParser::OptNamedFunArgContext * /*ctx*/) override { }

  virtual void enterTemplateSpec(ZHLParser::TemplateSpecContext * /*ctx*/) override { }
  virtual void exitTemplateSpec(ZHLParser::TemplateSpecContext * /*ctx*/) override { }

  virtual void enterType(ZHLParser::TypeContext * /*ctx*/) override { }
  virtual void exitType(ZHLParser::TypeContext * /*ctx*/) override { }

  virtual void enterTypeSpecifier(ZHLParser::TypeSpecifierContext * /*ctx*/) override { }
  virtual void exitTypeSpecifier(ZHLParser::TypeSpecifierContext * /*ctx*/) override { }

  virtual void enterSimpleType(ZHLParser::SimpleTypeContext * /*ctx*/) override { }
  virtual void exitSimpleType(ZHLParser::SimpleTypeContext * /*ctx*/) override { }

  virtual void enterSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext * /*ctx*/) override { }
  virtual void exitSimpleTypeSignedness(ZHLParser::SimpleTypeSignednessContext * /*ctx*/) override { }

  virtual void enterSimpleTypeLength(ZHLParser::SimpleTypeLengthContext * /*ctx*/) override { }
  virtual void exitSimpleTypeLength(ZHLParser::SimpleTypeLengthContext * /*ctx*/) override { }

  virtual void enterNestedName(ZHLParser::NestedNameContext * /*ctx*/) override { }
  virtual void exitNestedName(ZHLParser::NestedNameContext * /*ctx*/) override { }

  virtual void enterPointerAttribute(ZHLParser::PointerAttributeContext * /*ctx*/) override { }
  virtual void exitPointerAttribute(ZHLParser::PointerAttributeContext * /*ctx*/) override { }

  virtual void enterGenericCode(ZHLParser::GenericCodeContext * /*ctx*/) override { }
  virtual void exitGenericCode(ZHLParser::GenericCodeContext * /*ctx*/) override { }

  virtual void enterClass(ZHLParser::ClassContext * /*ctx*/) override { }
  virtual void exitClass(ZHLParser::ClassContext * /*ctx*/) override { }

  virtual void enterInheritance(ZHLParser::InheritanceContext * /*ctx*/) override { }
  virtual void exitInheritance(ZHLParser::InheritanceContext * /*ctx*/) override { }

  virtual void enterInheritanceDecl(ZHLParser::InheritanceDeclContext * /*ctx*/) override { }
  virtual void exitInheritanceDecl(ZHLParser::InheritanceDeclContext * /*ctx*/) override { }

  virtual void enterDepends(ZHLParser::DependsContext * /*ctx*/) override { }
  virtual void exitDepends(ZHLParser::DependsContext * /*ctx*/) override { }

  virtual void enterClassBody(ZHLParser::ClassBodyContext * /*ctx*/) override { }
  virtual void exitClassBody(ZHLParser::ClassBodyContext * /*ctx*/) override { }

  virtual void enterClassBodyElement(ZHLParser::ClassBodyElementContext * /*ctx*/) override { }
  virtual void exitClassBodyElement(ZHLParser::ClassBodyElementContext * /*ctx*/) override { }

  virtual void enterClassSignature(ZHLParser::ClassSignatureContext * /*ctx*/) override { }
  virtual void exitClassSignature(ZHLParser::ClassSignatureContext * /*ctx*/) override { }

  virtual void enterClassFunction(ZHLParser::ClassFunctionContext * /*ctx*/) override { }
  virtual void exitClassFunction(ZHLParser::ClassFunctionContext * /*ctx*/) override { }

  virtual void enterClassField(ZHLParser::ClassFieldContext * /*ctx*/) override { }
  virtual void exitClassField(ZHLParser::ClassFieldContext * /*ctx*/) override { }

  virtual void enterInnerField(ZHLParser::InnerFieldContext * /*ctx*/) override { }
  virtual void exitInnerField(ZHLParser::InnerFieldContext * /*ctx*/) override { }

  virtual void enterFullName(ZHLParser::FullNameContext * /*ctx*/) override { }
  virtual void exitFullName(ZHLParser::FullNameContext * /*ctx*/) override { }

  virtual void enterSignature(ZHLParser::SignatureContext * /*ctx*/) override { }
  virtual void exitSignature(ZHLParser::SignatureContext * /*ctx*/) override { }

  virtual void enterForwardDecl(ZHLParser::ForwardDeclContext * /*ctx*/) override { }
  virtual void exitForwardDecl(ZHLParser::ForwardDeclContext * /*ctx*/) override { }

  virtual void enterTypedef(ZHLParser::TypedefContext * /*ctx*/) override { }
  virtual void exitTypedef(ZHLParser::TypedefContext * /*ctx*/) override { }

  virtual void enterFunctionPtr(ZHLParser::FunctionPtrContext * /*ctx*/) override { }
  virtual void exitFunctionPtr(ZHLParser::FunctionPtrContext * /*ctx*/) override { }

  virtual void enterFptr(ZHLParser::FptrContext * /*ctx*/) override { }
  virtual void exitFptr(ZHLParser::FptrContext * /*ctx*/) override { }

  virtual void enterMemberPtr(ZHLParser::MemberPtrContext * /*ctx*/) override { }
  virtual void exitMemberPtr(ZHLParser::MemberPtrContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

