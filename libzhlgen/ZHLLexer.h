
// Generated from ZHLLexer.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  ZHLLexer : public antlr4::Lexer {
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

