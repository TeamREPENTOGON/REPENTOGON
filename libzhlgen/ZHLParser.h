
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
    Float = 33, Double = 34, Void = 35, Type = 36, Size = 37, Synonym = 38, 
    Vtable = 39, Skip = 40, Override = 41, Visibility = 42, Public = 43, 
    Private = 44, Protected = 45, Register = 46, GeneralPurposeRegister = 47, 
    Eax = 48, Ebx = 49, Ecx = 50, Edx = 51, Esi = 52, Edi = 53, Esp = 54, 
    Ebp = 55, SSERegister = 56, Xmm0 = 57, Xmm1 = 58, Xmm2 = 59, Xmm3 = 60, 
    Xmm4 = 61, Xmm5 = 62, Xmm6 = 63, Xmm7 = 64, CallingConvention = 65, 
    Stdcall = 66, Cdecl = 67, Fastcall = 68, Thiscall = 69, Signature = 70, 
    ReferenceSignature = 71, Name = 72, Number = 73, HexNumber = 74, DecNumber = 75, 
    GenericCode = 76, Whitespace = 77, Newline = 78, BlockComment = 79, 
    LineComment = 80, Any = 81
  };

  enum {
    RuleZhl = 0, RuleTypeInfo = 1, RuleTypeInfoDef = 2, RuleFunction = 3, 
    RuleReference = 4, RuleFunArgs = 5, RuleFunArg = 6, RuleArgParam = 7, 
    RuleOptNamedFunArgs = 8, RuleOptNamedFunArg = 9, RuleTemplateSpec = 10, 
    RuleType = 11, RuleTypeSpecifier = 12, RuleSimpleType = 13, RuleSimpleTypeSignedness = 14, 
    RuleSimpleTypeLength = 15, RuleNestedName = 16, RulePointerAttribute = 17, 
    RuleGenericCode = 18, RuleClass = 19, RuleInheritance = 20, RuleInheritanceDecl = 21, 
    RuleDepends = 22, RuleClassBody = 23, RuleVtable = 24, RuleVtableEntry = 25, 
    RuleVtableSignature = 26, RuleClassSignature = 27, RuleClassFunction = 28, 
    RuleClassField = 29, RuleInnerField = 30, RuleInnerFieldPtr = 31, RuleFullName = 32, 
    RuleSignature = 33, RuleForwardDecl = 34, RuleTypedef = 35, RuleFunctionPtr = 36, 
    RuleFptr = 37, RuleMemberPtr = 38
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
  class TypeInfoContext;
  class TypeInfoDefContext;
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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ZhlContext* zhl();

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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeInfoDefContext* typeInfoDef();

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
    antlr4::tree::TerminalNode *Colon();
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
    antlr4::tree::TerminalNode *Name();
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

