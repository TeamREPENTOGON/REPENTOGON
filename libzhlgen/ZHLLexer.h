
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
    Align = 40, Vtable = 41, Skip = 42, Pure = 43, Override = 44, Visibility = 45, 
    Public = 46, Private = 47, Protected = 48, Register = 49, GeneralPurposeRegister = 50, 
    Eax = 51, Ebx = 52, Ecx = 53, Edx = 54, Esi = 55, Edi = 56, Esp = 57, 
    Ebp = 58, SSERegister = 59, Xmm0 = 60, Xmm1 = 61, Xmm2 = 62, Xmm3 = 63, 
    Xmm4 = 64, Xmm5 = 65, Xmm6 = 66, Xmm7 = 67, CallingConvention = 68, 
    Stdcall = 69, Cdecl = 70, Fastcall = 71, Thiscall = 72, Signature = 73, 
    ReferenceSignature = 74, Operator = 75, OpSymbol = 76, Name = 77, Number = 78, 
    HexNumber = 79, DecNumber = 80, GenericCode = 81, Whitespace = 82, Newline = 83, 
    BlockComment = 84, LineComment = 85, Any = 86
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

