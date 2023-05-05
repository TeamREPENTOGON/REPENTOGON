
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

