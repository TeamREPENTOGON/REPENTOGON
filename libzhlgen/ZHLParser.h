
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLParser : public antlr4::Parser {
public:
  enum {
    DoubleQuote = 1, Colon = 2, Semi = 3, Qualifier = 4, Cleanup = 5, Static = 6, 
    Virtual = 7, Declspec = 8, External = 9, LeftParen = 10, RightParen = 11, 
    Comma = 12, Star = 13, Lt = 14, Gt = 15, LeftRBracket = 16, RightRBracket = 17, 
    LeftBracket = 18, RightBracket = 19, Class = 20, Struct = 21, Reference = 22, 
    Depends = 23, Typedef = 24, Const = 25, CppRef = 26, Unsigned = 27, 
    Signed = 28, Long = 29, Int = 30, Short = 31, Char = 32, Bool = 33, 
    Float = 34, Double = 35, Void = 36, Type = 37, Size = 38, Synonym = 39, 
    Align = 40, Vtable = 41, Skip = 42, Pure = 43, Override = 44, Debug = 45, 
    NoHook = 46, Asm = 47, Visibility = 48, Public = 49, Private = 50, Protected = 51, 
    Register = 52, GeneralPurposeRegister = 53, Eax = 54, Ebx = 55, Ecx = 56, 
    Edx = 57, Esi = 58, Edi = 59, Esp = 60, Ebp = 61, SSERegister = 62, 
    Xmm0 = 63, Xmm1 = 64, Xmm2 = 65, Xmm3 = 66, Xmm4 = 67, Xmm5 = 68, Xmm6 = 69, 
    Xmm7 = 70, CallingConvention = 71, Stdcall = 72, Cdecl = 73, Fastcall = 74, 
    Thiscall = 75, X8664 = 76, X8664_Output = 77, ColonLessSignature = 78, 
    Signature = 79, ReferenceSignature = 80, Operator = 81, OpSymbol = 82, 
    Name = 83, Number = 84, HexNumber = 85, DecNumber = 86, GenericCode = 87, 
    Whitespace = 88, Newline = 89, BlockComment = 90, LineComment = 91, 
    Any = 92
  };

  enum {
    RuleZhl = 0, RuleExternalFunc = 1, RuleTypeInfo = 2, RuleTypeInfoDef = 3, 
    RuleAsmDef = 4, RuleFunction = 5, RuleReference = 6, RuleFunArgs = 7, 
    RuleFunArg = 8, RuleArgParam = 9, RuleOptNamedFunArgs = 10, RuleOptNamedFunArg = 11, 
    RuleTemplateSpec = 12, RuleType = 13, RuleTypeSpecifier = 14, RuleSimpleType = 15, 
    RuleSimpleTypeSignedness = 16, RuleSimpleTypeLength = 17, RuleNestedName = 18, 
    RulePointerAttribute = 19, RuleGenericCode = 20, RuleClass = 21, RuleInheritance = 22, 
    RuleInheritanceDecl = 23, RuleDepends = 24, RuleClassBody = 25, RuleVtable = 26, 
    RuleVtableEntry = 27, RuleVtableSignature = 28, RuleClassSignature = 29, 
    RuleClassFunction = 30, RuleFunctionName = 31, RuleClassField = 32, 
    RuleInnerField = 33, RuleInnerFieldPtr = 34, RuleFullName = 35, RuleSignature = 36, 
    RuleForwardDecl = 37, RuleTypedef = 38, RuleFunctionPtr = 39, RuleFptr = 40, 
    RuleMemberPtr = 41
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
  class ExternalFuncContext;
  class TypeInfoContext;
  class TypeInfoDefContext;
  class AsmDefContext;
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
  class VtableContext;
  class VtableEntryContext;
  class VtableSignatureContext;
  class ClassSignatureContext;
  class ClassFunctionContext;
  class FunctionNameContext;
  class ClassFieldContext;
  class InnerFieldContext;
  class InnerFieldPtrContext;
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
    std::vector<TypeInfoContext *> typeInfo();
    TypeInfoContext* typeInfo(size_t i);
    std::vector<ExternalFuncContext *> externalFunc();
    ExternalFuncContext* externalFunc(size_t i);
    std::vector<AsmDefContext *> asmDef();
    AsmDefContext* asmDef(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ZhlContext* zhl();

  class  ExternalFuncContext : public antlr4::ParserRuleContext {
  public:
    ExternalFuncContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *External();
    antlr4::tree::TerminalNode *LeftParen();
    std::vector<antlr4::tree::TerminalNode *> Name();
    antlr4::tree::TerminalNode* Name(size_t i);
    antlr4::tree::TerminalNode *Comma();
    antlr4::tree::TerminalNode *RightParen();
    FunctionContext *function();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternalFuncContext* externalFunc();

  class  TypeInfoContext : public antlr4::ParserRuleContext {
  public:
    TypeInfoContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Type();
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *RightBracket();
    antlr4::tree::TerminalNode *Semi();
    std::vector<TypeInfoDefContext *> typeInfoDef();
    TypeInfoDefContext* typeInfoDef(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeInfoContext* typeInfo();

  class  TypeInfoDefContext : public antlr4::ParserRuleContext {
  public:
    TypeInfoDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Size();
    antlr4::tree::TerminalNode *Number();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *Synonym();
    TypeContext *type();
    antlr4::tree::TerminalNode *Align();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeInfoDefContext* typeInfoDef();

  class  AsmDefContext : public antlr4::ParserRuleContext {
  public:
    AsmDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Asm();
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *ColonLessSignature();
    antlr4::tree::TerminalNode *Semi();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsmDefContext* asmDef();

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
    antlr4::tree::TerminalNode *Name();
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);
    std::vector<FunctionNameContext *> functionName();
    FunctionNameContext* functionName(size_t i);

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
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);
    InheritanceContext *inheritance();
    antlr4::tree::TerminalNode *Number();

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
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

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
    std::vector<VtableContext *> vtable();
    VtableContext* vtable(size_t i);
    std::vector<ClassSignatureContext *> classSignature();
    ClassSignatureContext* classSignature(size_t i);
    std::vector<GenericCodeContext *> genericCode();
    GenericCodeContext* genericCode(size_t i);
    std::vector<ClassFieldContext *> classField();
    ClassFieldContext* classField(size_t i);
    std::vector<FunctionPtrContext *> functionPtr();
    FunctionPtrContext* functionPtr(size_t i);
    std::vector<ForwardDeclContext *> forwardDecl();
    ForwardDeclContext* forwardDecl(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassBodyContext* classBody();

  class  VtableContext : public antlr4::ParserRuleContext {
  public:
    VtableContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Vtable();
    antlr4::tree::TerminalNode *LeftBracket();
    antlr4::tree::TerminalNode *RightBracket();
    antlr4::tree::TerminalNode *Semi();
    std::vector<VtableEntryContext *> vtableEntry();
    VtableEntryContext* vtableEntry(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VtableContext* vtable();

  class  VtableEntryContext : public antlr4::ParserRuleContext {
  public:
    VtableEntryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VtableSignatureContext *vtableSignature();
    antlr4::tree::TerminalNode *Skip();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *Pure();
    ClassFunctionContext *classFunction();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VtableEntryContext* vtableEntry();

  class  VtableSignatureContext : public antlr4::ParserRuleContext {
  public:
    VtableSignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassSignatureContext *classSignature();
    antlr4::tree::TerminalNode *Override();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VtableSignatureContext* vtableSignature();

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
    FunctionNameContext *functionName();
    antlr4::tree::TerminalNode *LeftParen();
    antlr4::tree::TerminalNode *RightParen();
    antlr4::tree::TerminalNode *Semi();
    std::vector<antlr4::tree::TerminalNode *> Qualifier();
    antlr4::tree::TerminalNode* Qualifier(size_t i);
    FunArgsContext *funArgs();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassFunctionContext* classFunction();

  class  FunctionNameContext : public antlr4::ParserRuleContext {
  public:
    FunctionNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Name();
    antlr4::tree::TerminalNode *Operator();
    antlr4::tree::TerminalNode *OpSymbol();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionNameContext* functionName();

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
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *Number();
    std::vector<InnerFieldPtrContext *> innerFieldPtr();
    InnerFieldPtrContext* innerFieldPtr(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InnerFieldContext* innerField();

  class  InnerFieldPtrContext : public antlr4::ParserRuleContext {
  public:
    InnerFieldPtrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Star();
    antlr4::tree::TerminalNode *Const();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InnerFieldPtrContext* innerFieldPtr();

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
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *Number();
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
    antlr4::tree::TerminalNode *Number();
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

