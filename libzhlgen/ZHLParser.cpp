
// Generated from ZHLParser.g4 by ANTLR 4.12.0


#include "ZHLParserListener.h"
#include "ZHLParserVisitor.h"

#include "ZHLParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct ZHLParserStaticData final {
  ZHLParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  ZHLParserStaticData(const ZHLParserStaticData&) = delete;
  ZHLParserStaticData(ZHLParserStaticData&&) = delete;
  ZHLParserStaticData& operator=(const ZHLParserStaticData&) = delete;
  ZHLParserStaticData& operator=(ZHLParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag zhlparserParserOnceFlag;
ZHLParserStaticData *zhlparserParserStaticData = nullptr;

void zhlparserParserInitialize() {
  assert(zhlparserParserStaticData == nullptr);
  auto staticData = std::make_unique<ZHLParserStaticData>(
    std::vector<std::string>{
      "zhl", "function", "reference", "funArgs", "funArg", "argParam", "type", 
      "genericCode", "class", "depends", "classBody", "classSignature", 
      "classFunction", "classField", "innerField", "fullName", "signature"
    },
    std::vector<std::string>{
      "", "'\"'", "':'", "';'", "'cleanup'", "'static'", "'virtual'", "'__declspec'", 
      "'('", "')'", "','", "'*'", "'<'", "'>'", "'['", "']'", "'{'", "'}'", 
      "'class'", "'struct'", "'reference'", "'depends'", "", "", "'eax'", 
      "'ebx'", "'ecx'", "'edx'", "'esi'", "'edi'", "'esp'", "'ebp'", "", 
      "'xmm0'", "'xmm1'", "'xmm2'", "'xmm3'", "'xmm4'", "'xmm5'", "'xmm6'", 
      "'xmm7'"
    },
    std::vector<std::string>{
      "", "DoubleQuote", "Colon", "Semi", "Cleanup", "Static", "Virtual", 
      "Declspec", "LeftParen", "RightParen", "Comma", "Ptr", "Lt", "Rt", 
      "LeftRBracket", "RightRBracket", "LeftBracket", "RightBracket", "Class", 
      "Struct", "Reference", "Depends", "Register", "GeneralPurposeRegister", 
      "Eax", "Ebx", "Ecx", "Edx", "Esi", "Edi", "Esp", "Ebp", "SSERegister", 
      "Xmm0", "Xmm1", "Xmm2", "Xmm3", "Xmm4", "Xmm5", "Xmm6", "Xmm7", "Qualifier", 
      "Signature", "ReferenceSignature", "Name", "GenericCode", "Number", 
      "Whitespace", "Newline", "BlockComment", "LineComment", "Any"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,51,177,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,1,0,1,0,1,0,5,0,38,8,0,10,0,12,0,41,9,0,1,0,1,
  	0,1,1,5,1,46,8,1,10,1,12,1,49,9,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,58,
  	8,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,3,1,3,1,3,5,3,71,8,3,10,3,12,3,
  	74,9,3,1,4,1,4,5,4,78,8,4,10,4,12,4,81,9,4,1,4,1,4,3,4,85,8,4,1,5,1,5,
  	1,5,1,5,1,6,1,6,5,6,93,8,6,10,6,12,6,96,9,6,1,7,1,7,1,8,1,8,1,8,3,8,103,
  	8,8,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,5,9,114,8,9,10,9,12,9,117,9,9,
  	1,9,1,9,1,9,1,10,1,10,1,10,5,10,125,8,10,10,10,12,10,128,9,10,1,11,1,
  	11,1,11,1,12,5,12,134,8,12,10,12,12,12,137,9,12,1,12,1,12,1,12,1,12,1,
  	12,1,12,1,12,1,13,1,13,1,13,1,13,5,13,150,8,13,10,13,12,13,153,9,13,1,
  	13,1,13,1,14,5,14,158,8,14,10,14,12,14,161,9,14,1,14,1,14,1,15,1,15,1,
  	15,1,15,3,15,169,8,15,1,16,1,16,1,16,1,16,3,16,175,8,16,1,16,0,0,17,0,
  	2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,0,1,1,0,18,19,178,0,39,1,
  	0,0,0,2,47,1,0,0,0,4,62,1,0,0,0,6,67,1,0,0,0,8,75,1,0,0,0,10,86,1,0,0,
  	0,12,90,1,0,0,0,14,97,1,0,0,0,16,99,1,0,0,0,18,109,1,0,0,0,20,126,1,0,
  	0,0,22,129,1,0,0,0,24,135,1,0,0,0,26,145,1,0,0,0,28,159,1,0,0,0,30,164,
  	1,0,0,0,32,174,1,0,0,0,34,38,3,32,16,0,35,38,3,16,8,0,36,38,3,14,7,0,
  	37,34,1,0,0,0,37,35,1,0,0,0,37,36,1,0,0,0,38,41,1,0,0,0,39,37,1,0,0,0,
  	39,40,1,0,0,0,40,42,1,0,0,0,41,39,1,0,0,0,42,43,5,0,0,1,43,1,1,0,0,0,
  	44,46,5,41,0,0,45,44,1,0,0,0,46,49,1,0,0,0,47,45,1,0,0,0,47,48,1,0,0,
  	0,48,50,1,0,0,0,49,47,1,0,0,0,50,51,3,12,6,0,51,52,5,44,0,0,52,53,5,2,
  	0,0,53,54,5,2,0,0,54,55,5,44,0,0,55,57,5,8,0,0,56,58,3,6,3,0,57,56,1,
  	0,0,0,57,58,1,0,0,0,58,59,1,0,0,0,59,60,5,9,0,0,60,61,5,3,0,0,61,3,1,
  	0,0,0,62,63,5,20,0,0,63,64,3,12,6,0,64,65,5,44,0,0,65,66,5,3,0,0,66,5,
  	1,0,0,0,67,72,3,8,4,0,68,69,5,10,0,0,69,71,3,8,4,0,70,68,1,0,0,0,71,74,
  	1,0,0,0,72,70,1,0,0,0,72,73,1,0,0,0,73,7,1,0,0,0,74,72,1,0,0,0,75,79,
  	5,44,0,0,76,78,5,11,0,0,77,76,1,0,0,0,78,81,1,0,0,0,79,77,1,0,0,0,79,
  	80,1,0,0,0,80,82,1,0,0,0,81,79,1,0,0,0,82,84,3,30,15,0,83,85,3,10,5,0,
  	84,83,1,0,0,0,84,85,1,0,0,0,85,9,1,0,0,0,86,87,5,12,0,0,87,88,5,22,0,
  	0,88,89,5,13,0,0,89,11,1,0,0,0,90,94,5,44,0,0,91,93,5,11,0,0,92,91,1,
  	0,0,0,93,96,1,0,0,0,94,92,1,0,0,0,94,95,1,0,0,0,95,13,1,0,0,0,96,94,1,
  	0,0,0,97,98,5,45,0,0,98,15,1,0,0,0,99,100,7,0,0,0,100,102,5,44,0,0,101,
  	103,3,18,9,0,102,101,1,0,0,0,102,103,1,0,0,0,103,104,1,0,0,0,104,105,
  	5,16,0,0,105,106,3,20,10,0,106,107,5,17,0,0,107,108,5,3,0,0,108,17,1,
  	0,0,0,109,110,5,21,0,0,110,115,5,8,0,0,111,112,5,44,0,0,112,114,5,10,
  	0,0,113,111,1,0,0,0,114,117,1,0,0,0,115,113,1,0,0,0,115,116,1,0,0,0,116,
  	118,1,0,0,0,117,115,1,0,0,0,118,119,5,44,0,0,119,120,5,9,0,0,120,19,1,
  	0,0,0,121,125,3,22,11,0,122,125,3,14,7,0,123,125,3,26,13,0,124,121,1,
  	0,0,0,124,122,1,0,0,0,124,123,1,0,0,0,125,128,1,0,0,0,126,124,1,0,0,0,
  	126,127,1,0,0,0,127,21,1,0,0,0,128,126,1,0,0,0,129,130,5,42,0,0,130,131,
  	3,24,12,0,131,23,1,0,0,0,132,134,5,41,0,0,133,132,1,0,0,0,134,137,1,0,
  	0,0,135,133,1,0,0,0,135,136,1,0,0,0,136,138,1,0,0,0,137,135,1,0,0,0,138,
  	139,3,12,6,0,139,140,5,44,0,0,140,141,5,8,0,0,141,142,3,6,3,0,142,143,
  	5,9,0,0,143,144,5,3,0,0,144,25,1,0,0,0,145,146,3,12,6,0,146,151,3,28,
  	14,0,147,148,5,10,0,0,148,150,3,28,14,0,149,147,1,0,0,0,150,153,1,0,0,
  	0,151,149,1,0,0,0,151,152,1,0,0,0,152,154,1,0,0,0,153,151,1,0,0,0,154,
  	155,5,3,0,0,155,27,1,0,0,0,156,158,5,11,0,0,157,156,1,0,0,0,158,161,1,
  	0,0,0,159,157,1,0,0,0,159,160,1,0,0,0,160,162,1,0,0,0,161,159,1,0,0,0,
  	162,163,3,30,15,0,163,29,1,0,0,0,164,168,5,44,0,0,165,166,5,14,0,0,166,
  	167,5,46,0,0,167,169,5,15,0,0,168,165,1,0,0,0,168,169,1,0,0,0,169,31,
  	1,0,0,0,170,171,5,42,0,0,171,175,3,2,1,0,172,173,5,43,0,0,173,175,3,4,
  	2,0,174,170,1,0,0,0,174,172,1,0,0,0,175,33,1,0,0,0,17,37,39,47,57,72,
  	79,84,94,102,115,124,126,135,151,159,168,174
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  zhlparserParserStaticData = staticData.release();
}

}

ZHLParser::ZHLParser(TokenStream *input) : ZHLParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ZHLParser::ZHLParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ZHLParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *zhlparserParserStaticData->atn, zhlparserParserStaticData->decisionToDFA, zhlparserParserStaticData->sharedContextCache, options);
}

ZHLParser::~ZHLParser() {
  delete _interpreter;
}

const atn::ATN& ZHLParser::getATN() const {
  return *zhlparserParserStaticData->atn;
}

std::string ZHLParser::getGrammarFileName() const {
  return "ZHLParser.g4";
}

const std::vector<std::string>& ZHLParser::getRuleNames() const {
  return zhlparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& ZHLParser::getVocabulary() const {
  return zhlparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ZHLParser::getSerializedATN() const {
  return zhlparserParserStaticData->serializedATN;
}


//----------------- ZhlContext ------------------------------------------------------------------

ZHLParser::ZhlContext::ZhlContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ZhlContext::EOF() {
  return getToken(ZHLParser::EOF, 0);
}

std::vector<ZHLParser::SignatureContext *> ZHLParser::ZhlContext::signature() {
  return getRuleContexts<ZHLParser::SignatureContext>();
}

ZHLParser::SignatureContext* ZHLParser::ZhlContext::signature(size_t i) {
  return getRuleContext<ZHLParser::SignatureContext>(i);
}

std::vector<ZHLParser::ClassContext *> ZHLParser::ZhlContext::class_() {
  return getRuleContexts<ZHLParser::ClassContext>();
}

ZHLParser::ClassContext* ZHLParser::ZhlContext::class_(size_t i) {
  return getRuleContext<ZHLParser::ClassContext>(i);
}

std::vector<ZHLParser::GenericCodeContext *> ZHLParser::ZhlContext::genericCode() {
  return getRuleContexts<ZHLParser::GenericCodeContext>();
}

ZHLParser::GenericCodeContext* ZHLParser::ZhlContext::genericCode(size_t i) {
  return getRuleContext<ZHLParser::GenericCodeContext>(i);
}


size_t ZHLParser::ZhlContext::getRuleIndex() const {
  return ZHLParser::RuleZhl;
}

void ZHLParser::ZhlContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterZhl(this);
}

void ZHLParser::ZhlContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitZhl(this);
}


std::any ZHLParser::ZhlContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitZhl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ZhlContext* ZHLParser::zhl() {
  ZhlContext *_localctx = _tracker.createInstance<ZhlContext>(_ctx, getState());
  enterRule(_localctx, 0, ZHLParser::RuleZhl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 48378512408576) != 0)) {
      setState(37);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ZHLParser::Signature:
        case ZHLParser::ReferenceSignature: {
          setState(34);
          signature();
          break;
        }

        case ZHLParser::Class:
        case ZHLParser::Struct: {
          setState(35);
          class_();
          break;
        }

        case ZHLParser::GenericCode: {
          setState(36);
          genericCode();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(42);
    match(ZHLParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

ZHLParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::FunctionContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::FunctionContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::FunctionContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

std::vector<tree::TerminalNode *> ZHLParser::FunctionContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::FunctionContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}

tree::TerminalNode* ZHLParser::FunctionContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

tree::TerminalNode* ZHLParser::FunctionContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

tree::TerminalNode* ZHLParser::FunctionContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FunctionContext::Qualifier() {
  return getTokens(ZHLParser::Qualifier);
}

tree::TerminalNode* ZHLParser::FunctionContext::Qualifier(size_t i) {
  return getToken(ZHLParser::Qualifier, i);
}

ZHLParser::FunArgsContext* ZHLParser::FunctionContext::funArgs() {
  return getRuleContext<ZHLParser::FunArgsContext>(0);
}


size_t ZHLParser::FunctionContext::getRuleIndex() const {
  return ZHLParser::RuleFunction;
}

void ZHLParser::FunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunction(this);
}

void ZHLParser::FunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunction(this);
}


std::any ZHLParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunctionContext* ZHLParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 2, ZHLParser::RuleFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(47);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(44);
      match(ZHLParser::Qualifier);
      setState(49);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(50);
    type();
    setState(51);
    match(ZHLParser::Name);
    setState(52);
    match(ZHLParser::Colon);
    setState(53);
    match(ZHLParser::Colon);
    setState(54);
    match(ZHLParser::Name);
    setState(55);
    match(ZHLParser::LeftParen);
    setState(57);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Name) {
      setState(56);
      funArgs();
    }
    setState(59);
    match(ZHLParser::RightParen);
    setState(60);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReferenceContext ------------------------------------------------------------------

ZHLParser::ReferenceContext::ReferenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ReferenceContext::Reference() {
  return getToken(ZHLParser::Reference, 0);
}

ZHLParser::TypeContext* ZHLParser::ReferenceContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::ReferenceContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ReferenceContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}


size_t ZHLParser::ReferenceContext::getRuleIndex() const {
  return ZHLParser::RuleReference;
}

void ZHLParser::ReferenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterReference(this);
}

void ZHLParser::ReferenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitReference(this);
}


std::any ZHLParser::ReferenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitReference(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ReferenceContext* ZHLParser::reference() {
  ReferenceContext *_localctx = _tracker.createInstance<ReferenceContext>(_ctx, getState());
  enterRule(_localctx, 4, ZHLParser::RuleReference);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(62);
    match(ZHLParser::Reference);
    setState(63);
    type();
    setState(64);
    match(ZHLParser::Name);
    setState(65);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunArgsContext ------------------------------------------------------------------

ZHLParser::FunArgsContext::FunArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::FunArgContext *> ZHLParser::FunArgsContext::funArg() {
  return getRuleContexts<ZHLParser::FunArgContext>();
}

ZHLParser::FunArgContext* ZHLParser::FunArgsContext::funArg(size_t i) {
  return getRuleContext<ZHLParser::FunArgContext>(i);
}

std::vector<tree::TerminalNode *> ZHLParser::FunArgsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::FunArgsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::FunArgsContext::getRuleIndex() const {
  return ZHLParser::RuleFunArgs;
}

void ZHLParser::FunArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunArgs(this);
}

void ZHLParser::FunArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunArgs(this);
}


std::any ZHLParser::FunArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunArgs(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunArgsContext* ZHLParser::funArgs() {
  FunArgsContext *_localctx = _tracker.createInstance<FunArgsContext>(_ctx, getState());
  enterRule(_localctx, 6, ZHLParser::RuleFunArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(67);
    funArg();
    setState(72);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(68);
      match(ZHLParser::Comma);
      setState(69);
      funArg();
      setState(74);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunArgContext ------------------------------------------------------------------

ZHLParser::FunArgContext::FunArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::FunArgContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

ZHLParser::FullNameContext* ZHLParser::FunArgContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::FunArgContext::Ptr() {
  return getTokens(ZHLParser::Ptr);
}

tree::TerminalNode* ZHLParser::FunArgContext::Ptr(size_t i) {
  return getToken(ZHLParser::Ptr, i);
}

ZHLParser::ArgParamContext* ZHLParser::FunArgContext::argParam() {
  return getRuleContext<ZHLParser::ArgParamContext>(0);
}


size_t ZHLParser::FunArgContext::getRuleIndex() const {
  return ZHLParser::RuleFunArg;
}

void ZHLParser::FunArgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunArg(this);
}

void ZHLParser::FunArgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunArg(this);
}


std::any ZHLParser::FunArgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunArg(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunArgContext* ZHLParser::funArg() {
  FunArgContext *_localctx = _tracker.createInstance<FunArgContext>(_ctx, getState());
  enterRule(_localctx, 8, ZHLParser::RuleFunArg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(75);
    match(ZHLParser::Name);
    setState(79);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Ptr) {
      setState(76);
      match(ZHLParser::Ptr);
      setState(81);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(82);
    fullName();
    setState(84);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(83);
      argParam();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgParamContext ------------------------------------------------------------------

ZHLParser::ArgParamContext::ArgParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ArgParamContext::Lt() {
  return getToken(ZHLParser::Lt, 0);
}

tree::TerminalNode* ZHLParser::ArgParamContext::Register() {
  return getToken(ZHLParser::Register, 0);
}

tree::TerminalNode* ZHLParser::ArgParamContext::Rt() {
  return getToken(ZHLParser::Rt, 0);
}


size_t ZHLParser::ArgParamContext::getRuleIndex() const {
  return ZHLParser::RuleArgParam;
}

void ZHLParser::ArgParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgParam(this);
}

void ZHLParser::ArgParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgParam(this);
}


std::any ZHLParser::ArgParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitArgParam(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ArgParamContext* ZHLParser::argParam() {
  ArgParamContext *_localctx = _tracker.createInstance<ArgParamContext>(_ctx, getState());
  enterRule(_localctx, 10, ZHLParser::RuleArgParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(86);
    match(ZHLParser::Lt);
    setState(87);
    match(ZHLParser::Register);
    setState(88);
    match(ZHLParser::Rt);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

ZHLParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TypeContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::TypeContext::Ptr() {
  return getTokens(ZHLParser::Ptr);
}

tree::TerminalNode* ZHLParser::TypeContext::Ptr(size_t i) {
  return getToken(ZHLParser::Ptr, i);
}


size_t ZHLParser::TypeContext::getRuleIndex() const {
  return ZHLParser::RuleType;
}

void ZHLParser::TypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterType(this);
}

void ZHLParser::TypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitType(this);
}


std::any ZHLParser::TypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitType(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeContext* ZHLParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 12, ZHLParser::RuleType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(90);
    match(ZHLParser::Name);
    setState(94);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(91);
        match(ZHLParser::Ptr); 
      }
      setState(96);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GenericCodeContext ------------------------------------------------------------------

ZHLParser::GenericCodeContext::GenericCodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::GenericCodeContext::GenericCode() {
  return getToken(ZHLParser::GenericCode, 0);
}


size_t ZHLParser::GenericCodeContext::getRuleIndex() const {
  return ZHLParser::RuleGenericCode;
}

void ZHLParser::GenericCodeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGenericCode(this);
}

void ZHLParser::GenericCodeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGenericCode(this);
}


std::any ZHLParser::GenericCodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitGenericCode(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::GenericCodeContext* ZHLParser::genericCode() {
  GenericCodeContext *_localctx = _tracker.createInstance<GenericCodeContext>(_ctx, getState());
  enterRule(_localctx, 14, ZHLParser::RuleGenericCode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(97);
    match(ZHLParser::GenericCode);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassContext ------------------------------------------------------------------

ZHLParser::ClassContext::ClassContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ClassContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::LeftBracket() {
  return getToken(ZHLParser::LeftBracket, 0);
}

ZHLParser::ClassBodyContext* ZHLParser::ClassContext::classBody() {
  return getRuleContext<ZHLParser::ClassBodyContext>(0);
}

tree::TerminalNode* ZHLParser::ClassContext::RightBracket() {
  return getToken(ZHLParser::RightBracket, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Class() {
  return getToken(ZHLParser::Class, 0);
}

tree::TerminalNode* ZHLParser::ClassContext::Struct() {
  return getToken(ZHLParser::Struct, 0);
}

ZHLParser::DependsContext* ZHLParser::ClassContext::depends() {
  return getRuleContext<ZHLParser::DependsContext>(0);
}


size_t ZHLParser::ClassContext::getRuleIndex() const {
  return ZHLParser::RuleClass;
}

void ZHLParser::ClassContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClass(this);
}

void ZHLParser::ClassContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClass(this);
}


std::any ZHLParser::ClassContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClass(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassContext* ZHLParser::class_() {
  ClassContext *_localctx = _tracker.createInstance<ClassContext>(_ctx, getState());
  enterRule(_localctx, 16, ZHLParser::RuleClass);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(99);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(100);
    match(ZHLParser::Name);
    setState(102);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Depends) {
      setState(101);
      depends();
    }
    setState(104);
    match(ZHLParser::LeftBracket);
    setState(105);
    classBody();
    setState(106);
    match(ZHLParser::RightBracket);
    setState(107);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DependsContext ------------------------------------------------------------------

ZHLParser::DependsContext::DependsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::DependsContext::Depends() {
  return getToken(ZHLParser::Depends, 0);
}

tree::TerminalNode* ZHLParser::DependsContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::DependsContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::DependsContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

tree::TerminalNode* ZHLParser::DependsContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::DependsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::DependsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::DependsContext::getRuleIndex() const {
  return ZHLParser::RuleDepends;
}

void ZHLParser::DependsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDepends(this);
}

void ZHLParser::DependsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDepends(this);
}


std::any ZHLParser::DependsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitDepends(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::DependsContext* ZHLParser::depends() {
  DependsContext *_localctx = _tracker.createInstance<DependsContext>(_ctx, getState());
  enterRule(_localctx, 18, ZHLParser::RuleDepends);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(109);
    match(ZHLParser::Depends);
    setState(110);
    match(ZHLParser::LeftParen);
    setState(115);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(111);
        match(ZHLParser::Name);
        setState(112);
        match(ZHLParser::Comma); 
      }
      setState(117);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
    }
    setState(118);
    match(ZHLParser::Name);
    setState(119);
    match(ZHLParser::RightParen);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassBodyContext ------------------------------------------------------------------

ZHLParser::ClassBodyContext::ClassBodyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::ClassSignatureContext *> ZHLParser::ClassBodyContext::classSignature() {
  return getRuleContexts<ZHLParser::ClassSignatureContext>();
}

ZHLParser::ClassSignatureContext* ZHLParser::ClassBodyContext::classSignature(size_t i) {
  return getRuleContext<ZHLParser::ClassSignatureContext>(i);
}

std::vector<ZHLParser::GenericCodeContext *> ZHLParser::ClassBodyContext::genericCode() {
  return getRuleContexts<ZHLParser::GenericCodeContext>();
}

ZHLParser::GenericCodeContext* ZHLParser::ClassBodyContext::genericCode(size_t i) {
  return getRuleContext<ZHLParser::GenericCodeContext>(i);
}

std::vector<ZHLParser::ClassFieldContext *> ZHLParser::ClassBodyContext::classField() {
  return getRuleContexts<ZHLParser::ClassFieldContext>();
}

ZHLParser::ClassFieldContext* ZHLParser::ClassBodyContext::classField(size_t i) {
  return getRuleContext<ZHLParser::ClassFieldContext>(i);
}


size_t ZHLParser::ClassBodyContext::getRuleIndex() const {
  return ZHLParser::RuleClassBody;
}

void ZHLParser::ClassBodyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassBody(this);
}

void ZHLParser::ClassBodyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassBody(this);
}


std::any ZHLParser::ClassBodyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassBody(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassBodyContext* ZHLParser::classBody() {
  ClassBodyContext *_localctx = _tracker.createInstance<ClassBodyContext>(_ctx, getState());
  enterRule(_localctx, 20, ZHLParser::RuleClassBody);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 57174604644352) != 0)) {
      setState(124);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ZHLParser::Signature: {
          setState(121);
          classSignature();
          break;
        }

        case ZHLParser::GenericCode: {
          setState(122);
          genericCode();
          break;
        }

        case ZHLParser::Name: {
          setState(123);
          classField();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(128);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassSignatureContext ------------------------------------------------------------------

ZHLParser::ClassSignatureContext::ClassSignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ClassSignatureContext::Signature() {
  return getToken(ZHLParser::Signature, 0);
}

ZHLParser::ClassFunctionContext* ZHLParser::ClassSignatureContext::classFunction() {
  return getRuleContext<ZHLParser::ClassFunctionContext>(0);
}


size_t ZHLParser::ClassSignatureContext::getRuleIndex() const {
  return ZHLParser::RuleClassSignature;
}

void ZHLParser::ClassSignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassSignature(this);
}

void ZHLParser::ClassSignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassSignature(this);
}


std::any ZHLParser::ClassSignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassSignature(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassSignatureContext* ZHLParser::classSignature() {
  ClassSignatureContext *_localctx = _tracker.createInstance<ClassSignatureContext>(_ctx, getState());
  enterRule(_localctx, 22, ZHLParser::RuleClassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(129);
    match(ZHLParser::Signature);
    setState(130);
    classFunction();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassFunctionContext ------------------------------------------------------------------

ZHLParser::ClassFunctionContext::ClassFunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::ClassFunctionContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::LeftParen() {
  return getToken(ZHLParser::LeftParen, 0);
}

ZHLParser::FunArgsContext* ZHLParser::ClassFunctionContext::funArgs() {
  return getRuleContext<ZHLParser::FunArgsContext>(0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::RightParen() {
  return getToken(ZHLParser::RightParen, 0);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::ClassFunctionContext::Qualifier() {
  return getTokens(ZHLParser::Qualifier);
}

tree::TerminalNode* ZHLParser::ClassFunctionContext::Qualifier(size_t i) {
  return getToken(ZHLParser::Qualifier, i);
}


size_t ZHLParser::ClassFunctionContext::getRuleIndex() const {
  return ZHLParser::RuleClassFunction;
}

void ZHLParser::ClassFunctionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassFunction(this);
}

void ZHLParser::ClassFunctionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassFunction(this);
}


std::any ZHLParser::ClassFunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassFunction(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassFunctionContext* ZHLParser::classFunction() {
  ClassFunctionContext *_localctx = _tracker.createInstance<ClassFunctionContext>(_ctx, getState());
  enterRule(_localctx, 24, ZHLParser::RuleClassFunction);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(135);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(132);
      match(ZHLParser::Qualifier);
      setState(137);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(138);
    type();
    setState(139);
    match(ZHLParser::Name);
    setState(140);
    match(ZHLParser::LeftParen);
    setState(141);
    funArgs();
    setState(142);
    match(ZHLParser::RightParen);
    setState(143);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ClassFieldContext ------------------------------------------------------------------

ZHLParser::ClassFieldContext::ClassFieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::ClassFieldContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<ZHLParser::InnerFieldContext *> ZHLParser::ClassFieldContext::innerField() {
  return getRuleContexts<ZHLParser::InnerFieldContext>();
}

ZHLParser::InnerFieldContext* ZHLParser::ClassFieldContext::innerField(size_t i) {
  return getRuleContext<ZHLParser::InnerFieldContext>(i);
}

tree::TerminalNode* ZHLParser::ClassFieldContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::ClassFieldContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::ClassFieldContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::ClassFieldContext::getRuleIndex() const {
  return ZHLParser::RuleClassField;
}

void ZHLParser::ClassFieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassField(this);
}

void ZHLParser::ClassFieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassField(this);
}


std::any ZHLParser::ClassFieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassField(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassFieldContext* ZHLParser::classField() {
  ClassFieldContext *_localctx = _tracker.createInstance<ClassFieldContext>(_ctx, getState());
  enterRule(_localctx, 26, ZHLParser::RuleClassField);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    type();
    setState(146);
    innerField();
    setState(151);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(147);
      match(ZHLParser::Comma);
      setState(148);
      innerField();
      setState(153);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(154);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InnerFieldContext ------------------------------------------------------------------

ZHLParser::InnerFieldContext::InnerFieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::FullNameContext* ZHLParser::InnerFieldContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::InnerFieldContext::Ptr() {
  return getTokens(ZHLParser::Ptr);
}

tree::TerminalNode* ZHLParser::InnerFieldContext::Ptr(size_t i) {
  return getToken(ZHLParser::Ptr, i);
}


size_t ZHLParser::InnerFieldContext::getRuleIndex() const {
  return ZHLParser::RuleInnerField;
}

void ZHLParser::InnerFieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInnerField(this);
}

void ZHLParser::InnerFieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInnerField(this);
}


std::any ZHLParser::InnerFieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInnerField(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InnerFieldContext* ZHLParser::innerField() {
  InnerFieldContext *_localctx = _tracker.createInstance<InnerFieldContext>(_ctx, getState());
  enterRule(_localctx, 28, ZHLParser::RuleInnerField);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(159);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Ptr) {
      setState(156);
      match(ZHLParser::Ptr);
      setState(161);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(162);
    fullName();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FullNameContext ------------------------------------------------------------------

ZHLParser::FullNameContext::FullNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::FullNameContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::LeftRBracket() {
  return getToken(ZHLParser::LeftRBracket, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::Number() {
  return getToken(ZHLParser::Number, 0);
}

tree::TerminalNode* ZHLParser::FullNameContext::RightRBracket() {
  return getToken(ZHLParser::RightRBracket, 0);
}


size_t ZHLParser::FullNameContext::getRuleIndex() const {
  return ZHLParser::RuleFullName;
}

void ZHLParser::FullNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFullName(this);
}

void ZHLParser::FullNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFullName(this);
}


std::any ZHLParser::FullNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFullName(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FullNameContext* ZHLParser::fullName() {
  FullNameContext *_localctx = _tracker.createInstance<FullNameContext>(_ctx, getState());
  enterRule(_localctx, 30, ZHLParser::RuleFullName);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    match(ZHLParser::Name);
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::LeftRBracket) {
      setState(165);
      match(ZHLParser::LeftRBracket);
      setState(166);
      match(ZHLParser::Number);
      setState(167);
      match(ZHLParser::RightRBracket);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SignatureContext ------------------------------------------------------------------

ZHLParser::SignatureContext::SignatureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SignatureContext::Signature() {
  return getToken(ZHLParser::Signature, 0);
}

ZHLParser::FunctionContext* ZHLParser::SignatureContext::function() {
  return getRuleContext<ZHLParser::FunctionContext>(0);
}

tree::TerminalNode* ZHLParser::SignatureContext::ReferenceSignature() {
  return getToken(ZHLParser::ReferenceSignature, 0);
}

ZHLParser::ReferenceContext* ZHLParser::SignatureContext::reference() {
  return getRuleContext<ZHLParser::ReferenceContext>(0);
}


size_t ZHLParser::SignatureContext::getRuleIndex() const {
  return ZHLParser::RuleSignature;
}

void ZHLParser::SignatureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSignature(this);
}

void ZHLParser::SignatureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSignature(this);
}


std::any ZHLParser::SignatureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSignature(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SignatureContext* ZHLParser::signature() {
  SignatureContext *_localctx = _tracker.createInstance<SignatureContext>(_ctx, getState());
  enterRule(_localctx, 32, ZHLParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(174);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Signature: {
        setState(170);
        match(ZHLParser::Signature);
        setState(171);
        function();
        break;
      }

      case ZHLParser::ReferenceSignature: {
        setState(172);
        match(ZHLParser::ReferenceSignature);
        setState(173);
        reference();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void ZHLParser::initialize() {
  ::antlr4::internal::call_once(zhlparserParserOnceFlag, zhlparserParserInitialize);
}
