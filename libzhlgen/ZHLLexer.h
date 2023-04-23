
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
    Protected = 39, Register = 40, GeneralPurposeRegister = 41, Eax = 42, 
    Ebx = 43, Ecx = 44, Edx = 45, Esi = 46, Edi = 47, Esp = 48, Ebp = 49, 
    SSERegister = 50, Xmm0 = 51, Xmm1 = 52, Xmm2 = 53, Xmm3 = 54, Xmm4 = 55, 
    Xmm5 = 56, Xmm6 = 57, Xmm7 = 58, CallingConvention = 59, Stdcall = 60, 
    Cdecl = 61, Fastcall = 62, Thiscall = 63, Signature = 64, ReferenceSignature = 65, 
    Name = 66, Number = 67, HexNumber = 68, DecNumber = 69, GenericCode = 70, 
    Whitespace = 71, Newline = 72, BlockComment = 73, LineComment = 74, 
    Any = 75
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

