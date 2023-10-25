
// Generated from ZHLLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLLexer : public antlr4::Lexer {
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
    Visibility = 46, Public = 47, Private = 48, Protected = 49, Register = 50, 
    GeneralPurposeRegister = 51, Eax = 52, Ebx = 53, Ecx = 54, Edx = 55, 
    Esi = 56, Edi = 57, Esp = 58, Ebp = 59, SSERegister = 60, Xmm0 = 61, 
    Xmm1 = 62, Xmm2 = 63, Xmm3 = 64, Xmm4 = 65, Xmm5 = 66, Xmm6 = 67, Xmm7 = 68, 
    CallingConvention = 69, Stdcall = 70, Cdecl = 71, Fastcall = 72, Thiscall = 73, 
    X8664 = 74, X8664_Output = 75, Signature = 76, ReferenceSignature = 77, 
    Operator = 78, OpSymbol = 79, Name = 80, Number = 81, HexNumber = 82, 
    DecNumber = 83, GenericCode = 84, Whitespace = 85, Newline = 86, BlockComment = 87, 
    LineComment = 88, Any = 89
  };

  explicit ZHLLexer(antlr4::CharStream *input);

  ~ZHLLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

