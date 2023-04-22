
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLParser : public antlr4::Parser {
public:
  enum {
    DoubleQuote = 1, Colon = 2, Semi = 3, Cleanup = 4, Static = 5, Virtual = 6, 
    Declspec = 7, LeftParen = 8, RightParen = 9, Comma = 10, Ptr = 11, Lt = 12, 
    Rt = 13, LeftRBracket = 14, RightRBracket = 15, LeftBracket = 16, RightBracket = 17, 
    Class = 18, Struct = 19, Reference = 20, Depends = 21, Register = 22, 
    GeneralPurposeRegister = 23, Eax = 24, Ebx = 25, Ecx = 26, Edx = 27, 
    Esi = 28, Edi = 29, Esp = 30, Ebp = 31, SSERegister = 32, Xmm0 = 33, 
    Xmm1 = 34, Xmm2 = 35, Xmm3 = 36, Xmm4 = 37, Xmm5 = 38, Xmm6 = 39, Xmm7 = 40, 
    Qualifier = 41, Signature = 42, ReferenceSignature = 43, Name = 44, 
    GenericCode = 45, Number = 46, Whitespace = 47, Newline = 48, BlockComment = 49, 
    LineComment = 50, Any = 51
  };

  enum {
    RuleZhl = 0, RuleFunction = 1, RuleReference = 2, RuleFunArgs = 3, RuleFunArg = 4, 
    RuleArgParam = 5, RuleType = 6, RuleGenericCode = 7, RuleClass = 8, 
    RuleDepends = 9, RuleClassBody = 10, RuleClassSignature = 11, RuleClassFunction = 12, 
    RuleClassField = 13, RuleInnerField = 14, RuleFullName = 15, RuleSignature = 16
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
  class TypeContext;
  class GenericCodeContext;
  class ClassContext;
  class DependsContext;
  class ClassBodyContext;
  class ClassSignatureContext;
  class ClassFunctionContext;
  class ClassFieldContext;
  class InnerFieldContext;
  class FullNameContext;
  class SignatureContext; 

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
    std::vector<antlr4::tree::TerminalNode *> Name();
    antlr4::tree::TerminalNode* Name(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Colon();
    antlr4::tree::TerminalNode* Colon(size_t i);
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
    antlr4::tree::TerminalNode *Name();
    FullNameContext *fullName();
    std::vector<antlr4::tree::TerminalNode *> Ptr();
    antlr4::tree::TerminalNode* Ptr(size_t i);
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
    antlr4::tree::TerminalNode *Rt();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgParamContext* argParam();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Name();
    std::vector<antlr4::tree::TerminalNode *> Ptr();
    antlr4::tree::TerminalNode* Ptr(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeContext* type();

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

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassContext* class_();

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
    std::vector<antlr4::tree::TerminalNode *> Ptr();
    antlr4::tree::TerminalNode* Ptr(size_t i);

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


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

