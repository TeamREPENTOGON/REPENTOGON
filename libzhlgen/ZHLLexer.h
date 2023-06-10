
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
    Vtable = 40, Skip = 41, Pure = 42, Override = 43, Visibility = 44, Public = 45, 
    Private = 46, Protected = 47, Register = 48, GeneralPurposeRegister = 49, 
    Eax = 50, Ebx = 51, Ecx = 52, Edx = 53, Esi = 54, Edi = 55, Esp = 56, 
    Ebp = 57, SSERegister = 58, Xmm0 = 59, Xmm1 = 60, Xmm2 = 61, Xmm3 = 62, 
    Xmm4 = 63, Xmm5 = 64, Xmm6 = 65, Xmm7 = 66, CallingConvention = 67, 
    Stdcall = 68, Cdecl = 69, Fastcall = 70, Thiscall = 71, Signature = 72, 
    ReferenceSignature = 73, Operator = 74, OpSymbol = 75, Name = 76, Number = 77, 
    HexNumber = 78, DecNumber = 79, GenericCode = 80, Whitespace = 81, Newline = 82, 
    BlockComment = 83, LineComment = 84, Any = 85
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

