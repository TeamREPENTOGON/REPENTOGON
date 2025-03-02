
// Generated from ZHLLexer.g4 by ANTLR 4.13.2

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
    NoHook = 46, Visibility = 47, Public = 48, Private = 49, Protected = 50, 
    Register = 51, GeneralPurposeRegister = 52, Eax = 53, Ebx = 54, Ecx = 55, 
    Edx = 56, Esi = 57, Edi = 58, Esp = 59, Ebp = 60, SSERegister = 61, 
    Xmm0 = 62, Xmm1 = 63, Xmm2 = 64, Xmm3 = 65, Xmm4 = 66, Xmm5 = 67, Xmm6 = 68, 
    Xmm7 = 69, CallingConvention = 70, Stdcall = 71, Cdecl = 72, Fastcall = 73, 
    Thiscall = 74, X8664 = 75, X8664_Output = 76, Signature = 77, ReferenceSignature = 78, 
    Operator = 79, OpSymbol = 80, Name = 81, Number = 82, HexNumber = 83, 
    DecNumber = 84, GenericCode = 85, Whitespace = 86, Newline = 87, BlockComment = 88, 
    LineComment = 89, Any = 90
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

