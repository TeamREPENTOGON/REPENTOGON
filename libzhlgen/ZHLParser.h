
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLParser : public antlr4::Parser {
public:
  enum {
    DoubleQuote = 1, Colon = 2, Semi = 3, Qualifier = 4, Cleanup = 5, Static = 6, 
    Virtual = 7, Declspec = 8, LeftParen = 9, RightParen = 10, Comma = 11, 
    Star = 12, Lt = 13, Gt = 14, LeftRBracket = 15, RightRBracket = 16, 
    LeftBracket = 17, RightBracket = 18, Class = 19, Struct = 20, Reference = 21, 
    Depends = 22, Typedef = 23, Const = 24, CppRef = 25, Unsigned = 26, 
    Signed = 27, Long = 28, Int = 29, Short = 30, Char = 31, Bool = 32, 
    Float = 33, Double = 34, Void = 35, Visibility = 36, Public = 37, Private = 38, 
    Protected = 39, Register = 40, GeneralPurposeRegister = 41, Eax = 42, 
    Ebx = 43, Ecx = 44, Edx = 45, Esi = 46, Edi = 47, Esp = 48, Ebp = 49, 
    SSERegister = 50, Xmm0 = 51, Xmm1 = 52, Xmm2 = 53, Xmm3 = 54, Xmm4 = 55, 
    Xmm5 = 56, Xmm6 = 57, Xmm7 = 58, CallingConvention = 59, Stdcall = 60, 
    Cdecl = 61, Fastcall = 62, Thiscall = 63, Signature = 64, ReferenceSignature = 65, 
    Name = 66, Number = 67, HexNumber = 68, DecNumber = 69, GenericCode = 70, 
    Whitespace = 71, Newline = 72, BlockComment = 73, LineComment = 74, 
    Any = 75
  };

  enum {
    RuleZhl = 0, RuleFunction = 1, RuleReference = 2, RuleFunArgs = 3, RuleFunArg = 4, 
    RuleArgParam = 5, RuleOptNamedFunArgs = 6, RuleOptNamedFunArg = 7, RuleTemplateSpec = 8, 
    RuleType = 9, RuleTypeSpecifier = 10, RuleSimpleType = 11, RuleSimpleTypeSignedness = 12, 
    RuleSimpleTypeLength = 13, RuleNestedName = 14, RulePointerAttribute = 15, 
    RuleGenericCode = 16, RuleClass = 17, RuleInheritance = 18, RuleInheritanceDecl = 19, 
    RuleDepends = 20, RuleClassBody = 21, RuleClassSignature = 22, RuleClassFunction = 23, 
    RuleClassField = 24, RuleInnerField = 25, RuleFullName = 26, RuleSignature = 27, 
    RuleForwardDecl = 28, RuleTypedef = 29, RuleFunctionPtr = 30, RuleFptr = 31, 
    RuleMemberPtr = 32
  };

  explicit ZHLParser(antlr4::TokenStream *input);

  ZHLParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~ZHLParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ZhlContext;
  class FunctionContext;
  class ReferenceContext;
  class FunArgsContext;
  class FunArgContext;
  class ArgParamContext;
  class OptNamedFunArgsContext;
  class OptNamedFunArgContext;
  class TemplateSpecContext;
  class TypeContext;
  class TypeSpecifierContext;
  class SimpleTypeContext;
  class SimpleTypeSignednessContext;
  class SimpleTypeLengthContext;
  class NestedNameContext;
  class PointerAttributeContext;
  class GenericCodeContext;
  class ClassContext;
  class InheritanceContext;
  class InheritanceDeclContext;
  class DependsContext;
  class ClassBodyContext;
  class ClassSignatureContext;
  class ClassFunctionContext;
  class ClassFieldContext;
  class InnerFieldContext;
  class FullNameContext;
  class SignatureContext;
  class ForwardDeclContext;
  class TypedefContext;
  class FunctionPtrContext;
  class FptrContext;
  class MemberPtrContext; 

  class  ZhlContext : public antlr4::ParserRuleContext {
  public:
    ZhlContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<SignatureContext *> signature();
    SignatureContext* signature(size_t i);
    std::vector<ClassContext *> class_();
    ClassContext* class_(size_t i);
    std::vector<GenericCodeContext *> genericCode();
    GenericCodeContext* genericCode(size_t i);
    std::vector<TypedefContext *> typedef_();
    TypedefContext* typedef_(size_t i);
    std::vector<FunctionPtrContext *> functionPtr();
    FunctionPtrContext* functionPtr(size_t i);
    std::vector<ForwardDeclContext *> forwardDecl();
    ForwardDeclContext* forwardDecl(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ZhlContext* zhl();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    NestedNameContext *nestedName();
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    antlr4::tree::TerminalNode *Semi();
    std::vector<antlr4::tree::TerminalNode *> Qualifier();
    antlr4::tree::TerminalNode* Qualifier(size_t i);
    antlr4::tree::TerminalNode *CallingConvention();
    FunArgsContext *funArgs();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  ReferenceContext : public antlr4::ParserRuleContext {
  public:
    ReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Reference();
    TypeContext *type();
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *Semi();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReferenceContext* reference();

  class  FunArgsContext : public antlr4::ParserRuleContext {
  public:
    FunArgsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FunArgContext *> funArg();
    FunArgContext* funArg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunArgsContext* funArgs();

  class  FunArgContext : public antlr4::ParserRuleContext {
  public:
    FunArgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    FullNameContext *fullName();
    ArgParamContext *argParam();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunArgContext* funArg();

  class  ArgParamContext : public antlr4::ParserRuleContext {
  public:
    ArgParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Lt();
    antlr4::tree::TerminalNode *Register();
    antlr4::tree::TerminalNode *Gt();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgParamContext* argParam();

  class  OptNamedFunArgsContext : public antlr4::ParserRuleContext {
  public:
    OptNamedFunArgsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<OptNamedFunArgContext *> optNamedFunArg();
    OptNamedFunArgContext* optNamedFunArg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptNamedFunArgsContext* optNamedFunArgs();

  class  OptNamedFunArgContext : public antlr4::ParserRuleContext {
  public:
    OptNamedFunArgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    FullNameContext *fullName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptNamedFunArgContext* optNamedFunArg();

  class  TemplateSpecContext : public antlr4::ParserRuleContext {
  public:
    TemplateSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Lt();
    std::vector<TypeContext *> type();
    TypeContext* type(size_t i);
    antlr4::tree::TerminalNode *Gt();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TemplateSpecContext* templateSpec();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeSpecifierContext *typeSpecifier();
    TemplateSpecContext *templateSpec();
    std::vector<PointerAttributeContext *> pointerAttribute();
    PointerAttributeContext* pointerAttribute(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();

  class  TypeSpecifierContext : public antlr4::ParserRuleContext {
  public:
    TypeSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleTypeContext *simpleType();
    NestedNameContext *nestedName();
    std::vector<antlr4::tree::TerminalNode *> Const();
    antlr4::tree::TerminalNode* Const(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeSpecifierContext* typeSpecifier();

  class  SimpleTypeContext : public antlr4::ParserRuleContext {
  public:
    SimpleTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleTypeSignednessContext *simpleTypeSignedness();
    std::vector<SimpleTypeLengthContext *> simpleTypeLength();
    SimpleTypeLengthContext* simpleTypeLength(size_t i);
    antlr4::tree::TerminalNode *Char();
    antlr4::tree::TerminalNode *Bool();
    antlr4::tree::TerminalNode *Void();
    antlr4::tree::TerminalNode *Int();
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *Double();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleTypeContext* simpleType();

  class  SimpleTypeSignednessContext : public antlr4::ParserRuleContext {
  public:
    SimpleTypeSignednessContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Signed();
    antlr4::tree::TerminalNode *Unsigned();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleTypeSignednessContext* simpleTypeSignedness();

  class  SimpleTypeLengthContext : public antlr4::ParserRuleContext {
  public:
    SimpleTypeLengthContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Long();
    antlr4::tree::TerminalNode *Short();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleTypeLengthContext* simpleTypeLength();

  class  NestedNameContext : public antlr4::ParserRuleContext {
  public:
    NestedNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Name();
    antlr4::tree::TerminalNode* Name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedNameContext* nestedName();

  class  PointerAttributeContext : public antlr4::ParserRuleContext {
  public:
    PointerAttributeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> CppRef();
    antlr4::tree::TerminalNode* CppRef(size_t i);
    antlr4::tree::TerminalNode *Star();
    antlr4::tree::TerminalNode *Const();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PointerAttributeContext* pointerAttribute();

  class  GenericCodeContext : public antlr4::ParserRuleContext {
  public:
    GenericCodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *GenericCode();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GenericCodeContext* genericCode();

  class  ClassContext : public antlr4::ParserRuleContext {
  public:
    ClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *LeftBracket();
    ClassBodyContext *classBody();
    antlr4::tree::TerminalNode *RightBracket();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *Class();
    antlr4::tree::TerminalNode *Struct();
    DependsContext *depends();
    InheritanceContext *inheritance();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassContext* class_();

  class  InheritanceContext : public antlr4::ParserRuleContext {
  public:
    InheritanceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InheritanceDeclContext *> inheritanceDecl();
    InheritanceDeclContext* inheritanceDecl(size_t i);
    antlr4::tree::TerminalNode *Comma();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InheritanceContext* inheritance();

  class  InheritanceDeclContext : public antlr4::ParserRuleContext {
  public:
    InheritanceDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NestedNameContext *nestedName();
    antlr4::tree::TerminalNode *Visibility();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InheritanceDeclContext* inheritanceDecl();

  class  DependsContext : public antlr4::ParserRuleContext {
  public:
    DependsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Depends();
    antlr4::tree::TerminalNode *LeftParen();
    std::vector<antlr4::tree::TerminalNode *> Name();
    antlr4::tree::TerminalNode* Name(size_t i);
    antlr4::tree::TerminalNode *RightParen();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DependsContext* depends();

  class  ClassBodyContext : public antlr4::ParserRuleContext {
  public:
    ClassBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ClassSignatureContext *> classSignature();
    ClassSignatureContext* classSignature(size_t i);
    std::vector<GenericCodeContext *> genericCode();
    GenericCodeContext* genericCode(size_t i);
    std::vector<ClassFieldContext *> classField();
    ClassFieldContext* classField(size_t i);
    std::vector<TypedefContext *> typedef_();
    TypedefContext* typedef_(size_t i);
    std::vector<FunctionPtrContext *> functionPtr();
    FunctionPtrContext* functionPtr(size_t i);
    std::vector<ForwardDeclContext *> forwardDecl();
    ForwardDeclContext* forwardDecl(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassBodyContext* classBody();

  class  ClassSignatureContext : public antlr4::ParserRuleContext {
  public:
    ClassSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Signature();
    ClassFunctionContext *classFunction();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassSignatureContext* classSignature();

  class  ClassFunctionContext : public antlr4::ParserRuleContext {
  public:
    ClassFunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *LeftParen();
    FunArgsContext *funArgs();
    antlr4::tree::TerminalNode *RightParen();
    antlr4::tree::TerminalNode *Semi();
    std::vector<antlr4::tree::TerminalNode *> Qualifier();
    antlr4::tree::TerminalNode* Qualifier(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassFunctionContext* classFunction();

  class  ClassFieldContext : public antlr4::ParserRuleContext {
  public:
    ClassFieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    std::vector<InnerFieldContext *> innerField();
    InnerFieldContext* innerField(size_t i);
    antlr4::tree::TerminalNode *Semi();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassFieldContext* classField();

  class  InnerFieldContext : public antlr4::ParserRuleContext {
  public:
    InnerFieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FullNameContext *fullName();
    std::vector<antlr4::tree::TerminalNode *> Star();
    antlr4::tree::TerminalNode* Star(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InnerFieldContext* innerField();

  class  FullNameContext : public antlr4::ParserRuleContext {
  public:
    FullNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *LeftRBracket();
    antlr4::tree::TerminalNode *Number();
    antlr4::tree::TerminalNode *RightRBracket();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FullNameContext* fullName();

  class  SignatureContext : public antlr4::ParserRuleContext {
  public:
    SignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Signature();
    FunctionContext *function();
    antlr4::tree::TerminalNode *ReferenceSignature();
    ReferenceContext *reference();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignatureContext* signature();

  class  ForwardDeclContext : public antlr4::ParserRuleContext {
  public:
    ForwardDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *Struct();
    antlr4::tree::TerminalNode *Class();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ForwardDeclContext* forwardDecl();

  class  TypedefContext : public antlr4::ParserRuleContext {
  public:
    TypedefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Typedef();
    TypeContext *type();
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *Semi();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypedefContext* typedef_();

  class  FunctionPtrContext : public antlr4::ParserRuleContext {
  public:
    FunctionPtrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FptrContext *fptr();
    MemberPtrContext *memberPtr();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionPtrContext* functionPtr();

  class  FptrContext : public antlr4::ParserRuleContext {
  public:
    FptrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    std::vector<antlr4::tree::TerminalNode *> LeftParen();
    antlr4::tree::TerminalNode* LeftParen(size_t i);
    antlr4::tree::TerminalNode *Name();
    std::vector<antlr4::tree::TerminalNode *> RightParen();
    antlr4::tree::TerminalNode* RightParen(size_t i);
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *CallingConvention();
    std::vector<antlr4::tree::TerminalNode *> Star();
    antlr4::tree::TerminalNode* Star(size_t i);
    OptNamedFunArgsContext *optNamedFunArgs();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FptrContext* fptr();

  class  MemberPtrContext : public antlr4::ParserRuleContext {
  public:
    MemberPtrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    std::vector<antlr4::tree::TerminalNode *> LeftParen();
    antlr4::tree::TerminalNode* LeftParen(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Name();
    antlr4::tree::TerminalNode* Name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RightParen();
    antlr4::tree::TerminalNode* RightParen(size_t i);
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *CallingConvention();
    std::vector<antlr4::tree::TerminalNode *> Star();
    antlr4::tree::TerminalNode* Star(size_t i);
    OptNamedFunArgsContext *optNamedFunArgs();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MemberPtrContext* memberPtr();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

