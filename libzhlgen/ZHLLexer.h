
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
    Vtable = 40, Skip = 41, Override = 42, Visibility = 43, Public = 44, 
    Private = 45, Protected = 46, Register = 47, GeneralPurposeRegister = 48, 
    Eax = 49, Ebx = 50, Ecx = 51, Edx = 52, Esi = 53, Edi = 54, Esp = 55, 
    Ebp = 56, SSERegister = 57, Xmm0 = 58, Xmm1 = 59, Xmm2 = 60, Xmm3 = 61, 
    Xmm4 = 62, Xmm5 = 63, Xmm6 = 64, Xmm7 = 65, CallingConvention = 66, 
    Stdcall = 67, Cdecl = 68, Fastcall = 69, Thiscall = 70, Signature = 71, 
    ReferenceSignature = 72, Name = 73, Number = 74, HexNumber = 75, DecNumber = 76, 
    GenericCode = 77, Whitespace = 78, Newline = 79, BlockComment = 80, 
    LineComment = 81, Any = 82
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

