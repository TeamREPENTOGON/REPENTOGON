
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

