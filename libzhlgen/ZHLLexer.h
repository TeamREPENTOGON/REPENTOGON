
// Generated from ZHLLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLLexer : public antlr4::Lexer {
public:
  enum {
    DoubleQuote = 1, Colon = 2, Semi = 3, Qualifier = 4, Cleanup = 5, Static = 6, 
    Virtual = 7, Declspec = 8, LeftParen = 9, RightParen = 10, Comma = 11, 
    Star = 12, Lt = 13, Gt = 14, LeftRBracket = 15, RightRBracket = 16, 
    LeftBracket = 17, RightBracket = 18, Class = 19, Struct = 20, Reference = 21, 
    Depends = 22, Typedef = 23, Const = 24, CppRef = 25, Unsigned = 26, 
    Signed = 27, Long = 28, Int = 29, Short = 30, Char = 31, Bool = 32, 
    Float = 33, Double = 34, Void = 35, Visibility = 36, Public = 37, Private = 38, 
    Protected = 39, Test1 = 40, Test2 = 41, Test3 = 42, Register = 43, GeneralPurposeRegister = 44, 
    Eax = 45, Ebx = 46, Ecx = 47, Edx = 48, Esi = 49, Edi = 50, Esp = 51, 
    Ebp = 52, SSERegister = 53, Xmm0 = 54, Xmm1 = 55, Xmm2 = 56, Xmm3 = 57, 
    Xmm4 = 58, Xmm5 = 59, Xmm6 = 60, Xmm7 = 61, CallingConvention = 62, 
    Stdcall = 63, Cdecl = 64, Fastcall = 65, Thiscall = 66, Signature = 67, 
    ReferenceSignature = 68, Name = 69, Number = 70, HexNumber = 71, DecNumber = 72, 
    GenericCode = 73, Whitespace = 74, Newline = 75, BlockComment = 76, 
    LineComment = 77, Any = 78
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

