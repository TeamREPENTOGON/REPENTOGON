
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
      "zhl", "function", "reference", "funArgs", "funArg", "argParam", "optNamedFunArgs", 
      "optNamedFunArg", "templateSpec", "type", "typeSpecifier", "simpleType", 
      "simpleTypeSignedness", "simpleTypeLength", "nestedName", "pointerAttribute", 
      "genericCode", "class", "inheritance", "inheritanceDecl", "depends", 
      "classBody", "classBodyElement", "classSignature", "classFunction", 
      "classField", "innerField", "fullName", "signature", "forwardDecl", 
      "typedef", "functionPtr", "fptr", "memberPtr"
    },
    std::vector<std::string>{
      "", "'\"'", "':'", "';'", "", "'cleanup'", "'static'", "'virtual'", 
      "'__declspec'", "'('", "')'", "','", "'*'", "'<'", "'>'", "'['", "']'", 
      "'{'", "'}'", "'class'", "'struct'", "'reference'", "'depends'", "'typedef'", 
      "'const'", "'&'", "'unsigned'", "'signed'", "'long'", "'int'", "'short'", 
      "'char'", "'bool'", "'float'", "'double'", "'void'", "", "'public'", 
      "'private'", "'protected'", "", "", "'eax'", "'ebx'", "'ecx'", "'edx'", 
      "'esi'", "'edi'", "'esp'", "'ebp'", "", "'xmm0'", "'xmm1'", "'xmm2'", 
      "'xmm3'", "'xmm4'", "'xmm5'", "'xmm6'", "'xmm7'", "", "'__stdcall'", 
      "'__cdecl'", "'__fastcall'", "'__thiscall'"
    },
    std::vector<std::string>{
      "", "DoubleQuote", "Colon", "Semi", "Qualifier", "Cleanup", "Static", 
      "Virtual", "Declspec", "LeftParen", "RightParen", "Comma", "Star", 
      "Lt", "Gt", "LeftRBracket", "RightRBracket", "LeftBracket", "RightBracket", 
      "Class", "Struct", "Reference", "Depends", "Typedef", "Const", "CppRef", 
      "Unsigned", "Signed", "Long", "Int", "Short", "Char", "Bool", "Float", 
      "Double", "Void", "Visibility", "Public", "Private", "Protected", 
      "Register", "GeneralPurposeRegister", "Eax", "Ebx", "Ecx", "Edx", 
      "Esi", "Edi", "Esp", "Ebp", "SSERegister", "Xmm0", "Xmm1", "Xmm2", 
      "Xmm3", "Xmm4", "Xmm5", "Xmm6", "Xmm7", "CallingConvention", "Stdcall", 
      "Cdecl", "Fastcall", "Thiscall", "Signature", "ReferenceSignature", 
      "Name", "Number", "HexNumber", "DecNumber", "GenericCode", "Whitespace", 
      "Newline", "BlockComment", "LineComment", "Any"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,75,379,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,1,0,1,0,1,0,1,0,
  	1,0,1,0,5,0,75,8,0,10,0,12,0,78,9,0,1,0,1,0,1,1,5,1,83,8,1,10,1,12,1,
  	86,9,1,1,1,3,1,89,8,1,1,1,1,1,1,1,1,1,3,1,95,8,1,1,1,1,1,1,1,1,2,1,2,
  	1,2,1,2,1,2,1,3,1,3,1,3,5,3,108,8,3,10,3,12,3,111,9,3,1,4,1,4,1,4,3,4,
  	116,8,4,1,5,1,5,1,5,1,5,1,6,1,6,1,6,5,6,125,8,6,10,6,12,6,128,9,6,1,7,
  	1,7,3,7,132,8,7,1,8,1,8,1,8,1,8,5,8,138,8,8,10,8,12,8,141,9,8,1,8,1,8,
  	1,9,1,9,3,9,147,8,9,1,9,5,9,150,8,9,10,9,12,9,153,9,9,1,10,3,10,156,8,
  	10,1,10,1,10,3,10,160,8,10,1,10,3,10,163,8,10,1,11,1,11,3,11,167,8,11,
  	1,11,4,11,170,8,11,11,11,12,11,171,1,11,3,11,175,8,11,1,11,1,11,1,11,
  	1,11,3,11,181,8,11,1,11,5,11,184,8,11,10,11,12,11,187,9,11,1,11,1,11,
  	1,11,3,11,192,8,11,1,11,3,11,195,8,11,1,12,1,12,1,13,1,13,1,14,1,14,1,
  	14,1,14,5,14,205,8,14,10,14,12,14,208,9,14,1,15,1,15,3,15,212,8,15,1,
  	15,1,15,3,15,216,8,15,3,15,218,8,15,1,16,1,16,1,17,1,17,1,17,3,17,225,
  	8,17,1,17,3,17,228,8,17,1,17,1,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,
  	1,19,3,19,240,8,19,1,19,1,19,1,20,1,20,1,20,1,20,5,20,248,8,20,10,20,
  	12,20,251,9,20,1,20,1,20,1,20,1,21,5,21,257,8,21,10,21,12,21,260,9,21,
  	1,22,1,22,1,22,1,22,1,22,1,22,3,22,268,8,22,1,23,1,23,1,23,1,24,5,24,
  	274,8,24,10,24,12,24,277,9,24,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,25,
  	1,25,1,25,1,25,5,25,290,8,25,10,25,12,25,293,9,25,1,25,1,25,1,26,5,26,
  	298,8,26,10,26,12,26,301,9,26,1,26,1,26,1,27,1,27,1,27,1,27,3,27,309,
  	8,27,1,28,1,28,1,28,1,28,3,28,315,8,28,1,29,1,29,1,29,1,29,1,30,1,30,
  	1,30,1,30,1,30,1,31,1,31,3,31,328,8,31,1,32,1,32,1,32,3,32,333,8,32,1,
  	32,4,32,336,8,32,11,32,12,32,337,1,32,1,32,1,32,1,32,3,32,344,8,32,1,
  	32,1,32,1,32,1,33,1,33,1,33,3,33,352,8,33,1,33,1,33,1,33,1,33,1,33,1,
  	33,5,33,360,8,33,10,33,12,33,363,9,33,1,33,4,33,366,8,33,11,33,12,33,
  	367,1,33,1,33,1,33,1,33,3,33,374,8,33,1,33,1,33,1,33,1,33,0,0,34,0,2,
  	4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,
  	52,54,56,58,60,62,64,66,0,3,1,0,26,27,2,0,28,28,30,30,1,0,19,20,403,0,
  	76,1,0,0,0,2,84,1,0,0,0,4,99,1,0,0,0,6,104,1,0,0,0,8,112,1,0,0,0,10,117,
  	1,0,0,0,12,121,1,0,0,0,14,129,1,0,0,0,16,133,1,0,0,0,18,144,1,0,0,0,20,
  	155,1,0,0,0,22,194,1,0,0,0,24,196,1,0,0,0,26,198,1,0,0,0,28,200,1,0,0,
  	0,30,217,1,0,0,0,32,219,1,0,0,0,34,221,1,0,0,0,36,234,1,0,0,0,38,239,
  	1,0,0,0,40,243,1,0,0,0,42,258,1,0,0,0,44,267,1,0,0,0,46,269,1,0,0,0,48,
  	275,1,0,0,0,50,285,1,0,0,0,52,299,1,0,0,0,54,304,1,0,0,0,56,314,1,0,0,
  	0,58,316,1,0,0,0,60,320,1,0,0,0,62,327,1,0,0,0,64,329,1,0,0,0,66,348,
  	1,0,0,0,68,75,3,56,28,0,69,75,3,34,17,0,70,75,3,32,16,0,71,75,3,60,30,
  	0,72,75,3,62,31,0,73,75,3,58,29,0,74,68,1,0,0,0,74,69,1,0,0,0,74,70,1,
  	0,0,0,74,71,1,0,0,0,74,72,1,0,0,0,74,73,1,0,0,0,75,78,1,0,0,0,76,74,1,
  	0,0,0,76,77,1,0,0,0,77,79,1,0,0,0,78,76,1,0,0,0,79,80,5,0,0,1,80,1,1,
  	0,0,0,81,83,5,4,0,0,82,81,1,0,0,0,83,86,1,0,0,0,84,82,1,0,0,0,84,85,1,
  	0,0,0,85,88,1,0,0,0,86,84,1,0,0,0,87,89,5,59,0,0,88,87,1,0,0,0,88,89,
  	1,0,0,0,89,90,1,0,0,0,90,91,3,18,9,0,91,92,3,28,14,0,92,94,5,9,0,0,93,
  	95,3,6,3,0,94,93,1,0,0,0,94,95,1,0,0,0,95,96,1,0,0,0,96,97,5,10,0,0,97,
  	98,5,3,0,0,98,3,1,0,0,0,99,100,5,21,0,0,100,101,3,18,9,0,101,102,5,66,
  	0,0,102,103,5,3,0,0,103,5,1,0,0,0,104,109,3,8,4,0,105,106,5,11,0,0,106,
  	108,3,8,4,0,107,105,1,0,0,0,108,111,1,0,0,0,109,107,1,0,0,0,109,110,1,
  	0,0,0,110,7,1,0,0,0,111,109,1,0,0,0,112,113,3,18,9,0,113,115,3,54,27,
  	0,114,116,3,10,5,0,115,114,1,0,0,0,115,116,1,0,0,0,116,9,1,0,0,0,117,
  	118,5,13,0,0,118,119,5,40,0,0,119,120,5,14,0,0,120,11,1,0,0,0,121,126,
  	3,14,7,0,122,123,5,11,0,0,123,125,3,14,7,0,124,122,1,0,0,0,125,128,1,
  	0,0,0,126,124,1,0,0,0,126,127,1,0,0,0,127,13,1,0,0,0,128,126,1,0,0,0,
  	129,131,3,18,9,0,130,132,3,54,27,0,131,130,1,0,0,0,131,132,1,0,0,0,132,
  	15,1,0,0,0,133,134,5,13,0,0,134,139,3,18,9,0,135,136,5,11,0,0,136,138,
  	3,18,9,0,137,135,1,0,0,0,138,141,1,0,0,0,139,137,1,0,0,0,139,140,1,0,
  	0,0,140,142,1,0,0,0,141,139,1,0,0,0,142,143,5,14,0,0,143,17,1,0,0,0,144,
  	146,3,20,10,0,145,147,3,16,8,0,146,145,1,0,0,0,146,147,1,0,0,0,147,151,
  	1,0,0,0,148,150,3,30,15,0,149,148,1,0,0,0,150,153,1,0,0,0,151,149,1,0,
  	0,0,151,152,1,0,0,0,152,19,1,0,0,0,153,151,1,0,0,0,154,156,5,24,0,0,155,
  	154,1,0,0,0,155,156,1,0,0,0,156,159,1,0,0,0,157,160,3,22,11,0,158,160,
  	3,28,14,0,159,157,1,0,0,0,159,158,1,0,0,0,160,162,1,0,0,0,161,163,5,24,
  	0,0,162,161,1,0,0,0,162,163,1,0,0,0,163,21,1,0,0,0,164,195,3,24,12,0,
  	165,167,3,24,12,0,166,165,1,0,0,0,166,167,1,0,0,0,167,169,1,0,0,0,168,
  	170,3,26,13,0,169,168,1,0,0,0,170,171,1,0,0,0,171,169,1,0,0,0,171,172,
  	1,0,0,0,172,195,1,0,0,0,173,175,3,24,12,0,174,173,1,0,0,0,174,175,1,0,
  	0,0,175,176,1,0,0,0,176,195,5,31,0,0,177,195,5,32,0,0,178,195,5,35,0,
  	0,179,181,3,24,12,0,180,179,1,0,0,0,180,181,1,0,0,0,181,185,1,0,0,0,182,
  	184,3,26,13,0,183,182,1,0,0,0,184,187,1,0,0,0,185,183,1,0,0,0,185,186,
  	1,0,0,0,186,188,1,0,0,0,187,185,1,0,0,0,188,195,5,29,0,0,189,195,5,33,
  	0,0,190,192,3,26,13,0,191,190,1,0,0,0,191,192,1,0,0,0,192,193,1,0,0,0,
  	193,195,5,34,0,0,194,164,1,0,0,0,194,166,1,0,0,0,194,174,1,0,0,0,194,
  	177,1,0,0,0,194,178,1,0,0,0,194,180,1,0,0,0,194,189,1,0,0,0,194,191,1,
  	0,0,0,195,23,1,0,0,0,196,197,7,0,0,0,197,25,1,0,0,0,198,199,7,1,0,0,199,
  	27,1,0,0,0,200,206,5,66,0,0,201,202,5,2,0,0,202,203,5,2,0,0,203,205,5,
  	66,0,0,204,201,1,0,0,0,205,208,1,0,0,0,206,204,1,0,0,0,206,207,1,0,0,
  	0,207,29,1,0,0,0,208,206,1,0,0,0,209,211,5,25,0,0,210,212,5,25,0,0,211,
  	210,1,0,0,0,211,212,1,0,0,0,212,218,1,0,0,0,213,215,5,12,0,0,214,216,
  	5,24,0,0,215,214,1,0,0,0,215,216,1,0,0,0,216,218,1,0,0,0,217,209,1,0,
  	0,0,217,213,1,0,0,0,218,31,1,0,0,0,219,220,5,70,0,0,220,33,1,0,0,0,221,
  	222,7,2,0,0,222,224,5,66,0,0,223,225,3,40,20,0,224,223,1,0,0,0,224,225,
  	1,0,0,0,225,227,1,0,0,0,226,228,3,36,18,0,227,226,1,0,0,0,227,228,1,0,
  	0,0,228,229,1,0,0,0,229,230,5,17,0,0,230,231,3,42,21,0,231,232,5,18,0,
  	0,232,233,5,3,0,0,233,35,1,0,0,0,234,235,3,38,19,0,235,236,5,11,0,0,236,
  	237,3,38,19,0,237,37,1,0,0,0,238,240,5,36,0,0,239,238,1,0,0,0,239,240,
  	1,0,0,0,240,241,1,0,0,0,241,242,3,28,14,0,242,39,1,0,0,0,243,244,5,22,
  	0,0,244,249,5,9,0,0,245,246,5,66,0,0,246,248,5,11,0,0,247,245,1,0,0,0,
  	248,251,1,0,0,0,249,247,1,0,0,0,249,250,1,0,0,0,250,252,1,0,0,0,251,249,
  	1,0,0,0,252,253,5,66,0,0,253,254,5,10,0,0,254,41,1,0,0,0,255,257,3,44,
  	22,0,256,255,1,0,0,0,257,260,1,0,0,0,258,256,1,0,0,0,258,259,1,0,0,0,
  	259,43,1,0,0,0,260,258,1,0,0,0,261,268,3,46,23,0,262,268,3,32,16,0,263,
  	268,3,50,25,0,264,268,3,60,30,0,265,268,3,62,31,0,266,268,3,58,29,0,267,
  	261,1,0,0,0,267,262,1,0,0,0,267,263,1,0,0,0,267,264,1,0,0,0,267,265,1,
  	0,0,0,267,266,1,0,0,0,268,45,1,0,0,0,269,270,5,64,0,0,270,271,3,48,24,
  	0,271,47,1,0,0,0,272,274,5,4,0,0,273,272,1,0,0,0,274,277,1,0,0,0,275,
  	273,1,0,0,0,275,276,1,0,0,0,276,278,1,0,0,0,277,275,1,0,0,0,278,279,3,
  	18,9,0,279,280,5,66,0,0,280,281,5,9,0,0,281,282,3,6,3,0,282,283,5,10,
  	0,0,283,284,5,3,0,0,284,49,1,0,0,0,285,286,3,18,9,0,286,291,3,52,26,0,
  	287,288,5,11,0,0,288,290,3,52,26,0,289,287,1,0,0,0,290,293,1,0,0,0,291,
  	289,1,0,0,0,291,292,1,0,0,0,292,294,1,0,0,0,293,291,1,0,0,0,294,295,5,
  	3,0,0,295,51,1,0,0,0,296,298,5,12,0,0,297,296,1,0,0,0,298,301,1,0,0,0,
  	299,297,1,0,0,0,299,300,1,0,0,0,300,302,1,0,0,0,301,299,1,0,0,0,302,303,
  	3,54,27,0,303,53,1,0,0,0,304,308,5,66,0,0,305,306,5,15,0,0,306,307,5,
  	67,0,0,307,309,5,16,0,0,308,305,1,0,0,0,308,309,1,0,0,0,309,55,1,0,0,
  	0,310,311,5,64,0,0,311,315,3,2,1,0,312,313,5,65,0,0,313,315,3,4,2,0,314,
  	310,1,0,0,0,314,312,1,0,0,0,315,57,1,0,0,0,316,317,7,2,0,0,317,318,5,
  	66,0,0,318,319,5,3,0,0,319,59,1,0,0,0,320,321,5,23,0,0,321,322,3,18,9,
  	0,322,323,5,66,0,0,323,324,5,3,0,0,324,61,1,0,0,0,325,328,3,64,32,0,326,
  	328,3,66,33,0,327,325,1,0,0,0,327,326,1,0,0,0,328,63,1,0,0,0,329,330,
  	3,18,9,0,330,332,5,9,0,0,331,333,5,59,0,0,332,331,1,0,0,0,332,333,1,0,
  	0,0,333,335,1,0,0,0,334,336,5,12,0,0,335,334,1,0,0,0,336,337,1,0,0,0,
  	337,335,1,0,0,0,337,338,1,0,0,0,338,339,1,0,0,0,339,340,5,66,0,0,340,
  	341,5,10,0,0,341,343,5,9,0,0,342,344,3,12,6,0,343,342,1,0,0,0,343,344,
  	1,0,0,0,344,345,1,0,0,0,345,346,5,10,0,0,346,347,5,3,0,0,347,65,1,0,0,
  	0,348,349,3,18,9,0,349,351,5,9,0,0,350,352,5,59,0,0,351,350,1,0,0,0,351,
  	352,1,0,0,0,352,353,1,0,0,0,353,354,5,66,0,0,354,355,5,2,0,0,355,361,
  	5,2,0,0,356,357,5,66,0,0,357,358,5,2,0,0,358,360,5,2,0,0,359,356,1,0,
  	0,0,360,363,1,0,0,0,361,359,1,0,0,0,361,362,1,0,0,0,362,365,1,0,0,0,363,
  	361,1,0,0,0,364,366,5,12,0,0,365,364,1,0,0,0,366,367,1,0,0,0,367,365,
  	1,0,0,0,367,368,1,0,0,0,368,369,1,0,0,0,369,370,5,66,0,0,370,371,5,10,
  	0,0,371,373,5,9,0,0,372,374,3,12,6,0,373,372,1,0,0,0,373,374,1,0,0,0,
  	374,375,1,0,0,0,375,376,5,10,0,0,376,377,5,3,0,0,377,67,1,0,0,0,45,74,
  	76,84,88,94,109,115,126,131,139,146,151,155,159,162,166,171,174,180,185,
  	191,194,206,211,215,217,224,227,239,249,258,267,275,291,299,308,314,327,
  	332,337,343,351,361,367,373
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

std::vector<ZHLParser::TypedefContext *> ZHLParser::ZhlContext::typedef_() {
  return getRuleContexts<ZHLParser::TypedefContext>();
}

ZHLParser::TypedefContext* ZHLParser::ZhlContext::typedef_(size_t i) {
  return getRuleContext<ZHLParser::TypedefContext>(i);
}

std::vector<ZHLParser::FunctionPtrContext *> ZHLParser::ZhlContext::functionPtr() {
  return getRuleContexts<ZHLParser::FunctionPtrContext>();
}

ZHLParser::FunctionPtrContext* ZHLParser::ZhlContext::functionPtr(size_t i) {
  return getRuleContext<ZHLParser::FunctionPtrContext>(i);
}

std::vector<ZHLParser::ForwardDeclContext *> ZHLParser::ZhlContext::forwardDecl() {
  return getRuleContexts<ZHLParser::ForwardDeclContext>();
}

ZHLParser::ForwardDeclContext* ZHLParser::ZhlContext::forwardDecl(size_t i) {
  return getRuleContext<ZHLParser::ForwardDeclContext>(i);
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
    setState(76);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 19) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 19)) & 2498090418438067) != 0)) {
      setState(74);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(68);
        signature();
        break;
      }

      case 2: {
        setState(69);
        class_();
        break;
      }

      case 3: {
        setState(70);
        genericCode();
        break;
      }

      case 4: {
        setState(71);
        typedef_();
        break;
      }

      case 5: {
        setState(72);
        functionPtr();
        break;
      }

      case 6: {
        setState(73);
        forwardDecl();
        break;
      }

      default:
        break;
      }
      setState(78);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(79);
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

ZHLParser::NestedNameContext* ZHLParser::FunctionContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
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

tree::TerminalNode* ZHLParser::FunctionContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
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
    setState(84);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(81);
      match(ZHLParser::Qualifier);
      setState(86);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(88);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(87);
      match(ZHLParser::CallingConvention);
    }
    setState(90);
    type();
    setState(91);
    nestedName();
    setState(92);
    match(ZHLParser::LeftParen);
    setState(94);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(93);
      funArgs();
    }
    setState(96);
    match(ZHLParser::RightParen);
    setState(97);
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
    setState(99);
    match(ZHLParser::Reference);
    setState(100);
    type();
    setState(101);
    match(ZHLParser::Name);
    setState(102);
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
    setState(104);
    funArg();
    setState(109);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(105);
      match(ZHLParser::Comma);
      setState(106);
      funArg();
      setState(111);
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

ZHLParser::TypeContext* ZHLParser::FunArgContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::FullNameContext* ZHLParser::FunArgContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
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
    setState(112);
    type();
    setState(113);
    fullName();
    setState(115);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(114);
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

tree::TerminalNode* ZHLParser::ArgParamContext::Gt() {
  return getToken(ZHLParser::Gt, 0);
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
    setState(117);
    match(ZHLParser::Lt);
    setState(118);
    match(ZHLParser::Register);
    setState(119);
    match(ZHLParser::Gt);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OptNamedFunArgsContext ------------------------------------------------------------------

ZHLParser::OptNamedFunArgsContext::OptNamedFunArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::OptNamedFunArgContext *> ZHLParser::OptNamedFunArgsContext::optNamedFunArg() {
  return getRuleContexts<ZHLParser::OptNamedFunArgContext>();
}

ZHLParser::OptNamedFunArgContext* ZHLParser::OptNamedFunArgsContext::optNamedFunArg(size_t i) {
  return getRuleContext<ZHLParser::OptNamedFunArgContext>(i);
}

std::vector<tree::TerminalNode *> ZHLParser::OptNamedFunArgsContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::OptNamedFunArgsContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::OptNamedFunArgsContext::getRuleIndex() const {
  return ZHLParser::RuleOptNamedFunArgs;
}

void ZHLParser::OptNamedFunArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptNamedFunArgs(this);
}

void ZHLParser::OptNamedFunArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptNamedFunArgs(this);
}


std::any ZHLParser::OptNamedFunArgsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitOptNamedFunArgs(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::optNamedFunArgs() {
  OptNamedFunArgsContext *_localctx = _tracker.createInstance<OptNamedFunArgsContext>(_ctx, getState());
  enterRule(_localctx, 12, ZHLParser::RuleOptNamedFunArgs);
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
    setState(121);
    optNamedFunArg();
    setState(126);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(122);
      match(ZHLParser::Comma);
      setState(123);
      optNamedFunArg();
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

//----------------- OptNamedFunArgContext ------------------------------------------------------------------

ZHLParser::OptNamedFunArgContext::OptNamedFunArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::OptNamedFunArgContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

ZHLParser::FullNameContext* ZHLParser::OptNamedFunArgContext::fullName() {
  return getRuleContext<ZHLParser::FullNameContext>(0);
}


size_t ZHLParser::OptNamedFunArgContext::getRuleIndex() const {
  return ZHLParser::RuleOptNamedFunArg;
}

void ZHLParser::OptNamedFunArgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOptNamedFunArg(this);
}

void ZHLParser::OptNamedFunArgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOptNamedFunArg(this);
}


std::any ZHLParser::OptNamedFunArgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitOptNamedFunArg(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::OptNamedFunArgContext* ZHLParser::optNamedFunArg() {
  OptNamedFunArgContext *_localctx = _tracker.createInstance<OptNamedFunArgContext>(_ctx, getState());
  enterRule(_localctx, 14, ZHLParser::RuleOptNamedFunArg);
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
    setState(129);
    type();
    setState(131);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Name) {
      setState(130);
      fullName();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TemplateSpecContext ------------------------------------------------------------------

ZHLParser::TemplateSpecContext::TemplateSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Lt() {
  return getToken(ZHLParser::Lt, 0);
}

std::vector<ZHLParser::TypeContext *> ZHLParser::TemplateSpecContext::type() {
  return getRuleContexts<ZHLParser::TypeContext>();
}

ZHLParser::TypeContext* ZHLParser::TemplateSpecContext::type(size_t i) {
  return getRuleContext<ZHLParser::TypeContext>(i);
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Gt() {
  return getToken(ZHLParser::Gt, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::TemplateSpecContext::Comma() {
  return getTokens(ZHLParser::Comma);
}

tree::TerminalNode* ZHLParser::TemplateSpecContext::Comma(size_t i) {
  return getToken(ZHLParser::Comma, i);
}


size_t ZHLParser::TemplateSpecContext::getRuleIndex() const {
  return ZHLParser::RuleTemplateSpec;
}

void ZHLParser::TemplateSpecContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTemplateSpec(this);
}

void ZHLParser::TemplateSpecContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTemplateSpec(this);
}


std::any ZHLParser::TemplateSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTemplateSpec(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TemplateSpecContext* ZHLParser::templateSpec() {
  TemplateSpecContext *_localctx = _tracker.createInstance<TemplateSpecContext>(_ctx, getState());
  enterRule(_localctx, 16, ZHLParser::RuleTemplateSpec);
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
    setState(133);
    match(ZHLParser::Lt);
    setState(134);
    type();
    setState(139);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(135);
      match(ZHLParser::Comma);
      setState(136);
      type();
      setState(141);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(142);
    match(ZHLParser::Gt);
   
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

ZHLParser::TypeSpecifierContext* ZHLParser::TypeContext::typeSpecifier() {
  return getRuleContext<ZHLParser::TypeSpecifierContext>(0);
}

ZHLParser::TemplateSpecContext* ZHLParser::TypeContext::templateSpec() {
  return getRuleContext<ZHLParser::TemplateSpecContext>(0);
}

std::vector<ZHLParser::PointerAttributeContext *> ZHLParser::TypeContext::pointerAttribute() {
  return getRuleContexts<ZHLParser::PointerAttributeContext>();
}

ZHLParser::PointerAttributeContext* ZHLParser::TypeContext::pointerAttribute(size_t i) {
  return getRuleContext<ZHLParser::PointerAttributeContext>(i);
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
  enterRule(_localctx, 18, ZHLParser::RuleType);
  size_t _la = 0;

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
    setState(144);
    typeSpecifier();
    setState(146);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(145);
      templateSpec();
    }
    setState(151);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(148);
        pointerAttribute(); 
      }
      setState(153);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeSpecifierContext ------------------------------------------------------------------

ZHLParser::TypeSpecifierContext::TypeSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::SimpleTypeContext* ZHLParser::TypeSpecifierContext::simpleType() {
  return getRuleContext<ZHLParser::SimpleTypeContext>(0);
}

ZHLParser::NestedNameContext* ZHLParser::TypeSpecifierContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::TypeSpecifierContext::Const() {
  return getTokens(ZHLParser::Const);
}

tree::TerminalNode* ZHLParser::TypeSpecifierContext::Const(size_t i) {
  return getToken(ZHLParser::Const, i);
}


size_t ZHLParser::TypeSpecifierContext::getRuleIndex() const {
  return ZHLParser::RuleTypeSpecifier;
}

void ZHLParser::TypeSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSpecifier(this);
}

void ZHLParser::TypeSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSpecifier(this);
}


std::any ZHLParser::TypeSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypeSpecifier(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypeSpecifierContext* ZHLParser::typeSpecifier() {
  TypeSpecifierContext *_localctx = _tracker.createInstance<TypeSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 20, ZHLParser::RuleTypeSpecifier);
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
    setState(155);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(154);
      match(ZHLParser::Const);
    }
    setState(159);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Unsigned:
      case ZHLParser::Signed:
      case ZHLParser::Long:
      case ZHLParser::Int:
      case ZHLParser::Short:
      case ZHLParser::Char:
      case ZHLParser::Bool:
      case ZHLParser::Float:
      case ZHLParser::Double:
      case ZHLParser::Void: {
        setState(157);
        simpleType();
        break;
      }

      case ZHLParser::Name: {
        setState(158);
        nestedName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(162);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(161);
      match(ZHLParser::Const);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeContext ------------------------------------------------------------------

ZHLParser::SimpleTypeContext::SimpleTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::SimpleTypeSignednessContext* ZHLParser::SimpleTypeContext::simpleTypeSignedness() {
  return getRuleContext<ZHLParser::SimpleTypeSignednessContext>(0);
}

std::vector<ZHLParser::SimpleTypeLengthContext *> ZHLParser::SimpleTypeContext::simpleTypeLength() {
  return getRuleContexts<ZHLParser::SimpleTypeLengthContext>();
}

ZHLParser::SimpleTypeLengthContext* ZHLParser::SimpleTypeContext::simpleTypeLength(size_t i) {
  return getRuleContext<ZHLParser::SimpleTypeLengthContext>(i);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Char() {
  return getToken(ZHLParser::Char, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Bool() {
  return getToken(ZHLParser::Bool, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Void() {
  return getToken(ZHLParser::Void, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Int() {
  return getToken(ZHLParser::Int, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Float() {
  return getToken(ZHLParser::Float, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeContext::Double() {
  return getToken(ZHLParser::Double, 0);
}


size_t ZHLParser::SimpleTypeContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleType;
}

void ZHLParser::SimpleTypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleType(this);
}

void ZHLParser::SimpleTypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleType(this);
}


std::any ZHLParser::SimpleTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleType(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeContext* ZHLParser::simpleType() {
  SimpleTypeContext *_localctx = _tracker.createInstance<SimpleTypeContext>(_ctx, getState());
  enterRule(_localctx, 22, ZHLParser::RuleSimpleType);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(194);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(164);
      simpleTypeSignedness();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(166);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(165);
        simpleTypeSignedness();
      }
      setState(169); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(168);
        simpleTypeLength();
        setState(171); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == ZHLParser::Long

      || _la == ZHLParser::Short);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(174);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(173);
        simpleTypeSignedness();
      }
      setState(176);
      match(ZHLParser::Char);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(177);
      match(ZHLParser::Bool);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(178);
      match(ZHLParser::Void);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(180);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(179);
        simpleTypeSignedness();
      }
      setState(185);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(182);
        simpleTypeLength();
        setState(187);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(188);
      match(ZHLParser::Int);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(189);
      match(ZHLParser::Float);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(191);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(190);
        simpleTypeLength();
      }
      setState(193);
      match(ZHLParser::Double);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeSignednessContext ------------------------------------------------------------------

ZHLParser::SimpleTypeSignednessContext::SimpleTypeSignednessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SimpleTypeSignednessContext::Signed() {
  return getToken(ZHLParser::Signed, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeSignednessContext::Unsigned() {
  return getToken(ZHLParser::Unsigned, 0);
}


size_t ZHLParser::SimpleTypeSignednessContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleTypeSignedness;
}

void ZHLParser::SimpleTypeSignednessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleTypeSignedness(this);
}

void ZHLParser::SimpleTypeSignednessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleTypeSignedness(this);
}


std::any ZHLParser::SimpleTypeSignednessContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleTypeSignedness(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeSignednessContext* ZHLParser::simpleTypeSignedness() {
  SimpleTypeSignednessContext *_localctx = _tracker.createInstance<SimpleTypeSignednessContext>(_ctx, getState());
  enterRule(_localctx, 24, ZHLParser::RuleSimpleTypeSignedness);
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
    setState(196);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Unsigned

    || _la == ZHLParser::Signed)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SimpleTypeLengthContext ------------------------------------------------------------------

ZHLParser::SimpleTypeLengthContext::SimpleTypeLengthContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::SimpleTypeLengthContext::Long() {
  return getToken(ZHLParser::Long, 0);
}

tree::TerminalNode* ZHLParser::SimpleTypeLengthContext::Short() {
  return getToken(ZHLParser::Short, 0);
}


size_t ZHLParser::SimpleTypeLengthContext::getRuleIndex() const {
  return ZHLParser::RuleSimpleTypeLength;
}

void ZHLParser::SimpleTypeLengthContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSimpleTypeLength(this);
}

void ZHLParser::SimpleTypeLengthContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSimpleTypeLength(this);
}


std::any ZHLParser::SimpleTypeLengthContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitSimpleTypeLength(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::SimpleTypeLengthContext* ZHLParser::simpleTypeLength() {
  SimpleTypeLengthContext *_localctx = _tracker.createInstance<SimpleTypeLengthContext>(_ctx, getState());
  enterRule(_localctx, 26, ZHLParser::RuleSimpleTypeLength);
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
    setState(198);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Long

    || _la == ZHLParser::Short)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NestedNameContext ------------------------------------------------------------------

ZHLParser::NestedNameContext::NestedNameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ZHLParser::NestedNameContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::NestedNameContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

std::vector<tree::TerminalNode *> ZHLParser::NestedNameContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::NestedNameContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}


size_t ZHLParser::NestedNameContext::getRuleIndex() const {
  return ZHLParser::RuleNestedName;
}

void ZHLParser::NestedNameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNestedName(this);
}

void ZHLParser::NestedNameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNestedName(this);
}


std::any ZHLParser::NestedNameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitNestedName(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::NestedNameContext* ZHLParser::nestedName() {
  NestedNameContext *_localctx = _tracker.createInstance<NestedNameContext>(_ctx, getState());
  enterRule(_localctx, 28, ZHLParser::RuleNestedName);
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
    setState(200);
    match(ZHLParser::Name);
    setState(206);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Colon) {
      setState(201);
      match(ZHLParser::Colon);
      setState(202);
      match(ZHLParser::Colon);
      setState(203);
      match(ZHLParser::Name);
      setState(208);
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

//----------------- PointerAttributeContext ------------------------------------------------------------------

ZHLParser::PointerAttributeContext::PointerAttributeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> ZHLParser::PointerAttributeContext::CppRef() {
  return getTokens(ZHLParser::CppRef);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::CppRef(size_t i) {
  return getToken(ZHLParser::CppRef, i);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::Star() {
  return getToken(ZHLParser::Star, 0);
}

tree::TerminalNode* ZHLParser::PointerAttributeContext::Const() {
  return getToken(ZHLParser::Const, 0);
}


size_t ZHLParser::PointerAttributeContext::getRuleIndex() const {
  return ZHLParser::RulePointerAttribute;
}

void ZHLParser::PointerAttributeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPointerAttribute(this);
}

void ZHLParser::PointerAttributeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPointerAttribute(this);
}


std::any ZHLParser::PointerAttributeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitPointerAttribute(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::PointerAttributeContext* ZHLParser::pointerAttribute() {
  PointerAttributeContext *_localctx = _tracker.createInstance<PointerAttributeContext>(_ctx, getState());
  enterRule(_localctx, 30, ZHLParser::RulePointerAttribute);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(217);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::CppRef: {
        enterOuterAlt(_localctx, 1);
        setState(209);
        match(ZHLParser::CppRef);
        setState(211);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(210);
          match(ZHLParser::CppRef);
          break;
        }

        default:
          break;
        }
        break;
      }

      case ZHLParser::Star: {
        enterOuterAlt(_localctx, 2);
        setState(213);
        match(ZHLParser::Star);
        setState(215);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ZHLParser::Const) {
          setState(214);
          match(ZHLParser::Const);
        }
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
  enterRule(_localctx, 32, ZHLParser::RuleGenericCode);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(219);
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

ZHLParser::InheritanceContext* ZHLParser::ClassContext::inheritance() {
  return getRuleContext<ZHLParser::InheritanceContext>(0);
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
  enterRule(_localctx, 34, ZHLParser::RuleClass);
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
    setState(221);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(222);
    match(ZHLParser::Name);
    setState(224);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Depends) {
      setState(223);
      depends();
    }
    setState(227);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility

    || _la == ZHLParser::Name) {
      setState(226);
      inheritance();
    }
    setState(229);
    match(ZHLParser::LeftBracket);
    setState(230);
    classBody();
    setState(231);
    match(ZHLParser::RightBracket);
    setState(232);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InheritanceContext ------------------------------------------------------------------

ZHLParser::InheritanceContext::InheritanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ZHLParser::InheritanceDeclContext *> ZHLParser::InheritanceContext::inheritanceDecl() {
  return getRuleContexts<ZHLParser::InheritanceDeclContext>();
}

ZHLParser::InheritanceDeclContext* ZHLParser::InheritanceContext::inheritanceDecl(size_t i) {
  return getRuleContext<ZHLParser::InheritanceDeclContext>(i);
}

tree::TerminalNode* ZHLParser::InheritanceContext::Comma() {
  return getToken(ZHLParser::Comma, 0);
}


size_t ZHLParser::InheritanceContext::getRuleIndex() const {
  return ZHLParser::RuleInheritance;
}

void ZHLParser::InheritanceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInheritance(this);
}

void ZHLParser::InheritanceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInheritance(this);
}


std::any ZHLParser::InheritanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInheritance(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InheritanceContext* ZHLParser::inheritance() {
  InheritanceContext *_localctx = _tracker.createInstance<InheritanceContext>(_ctx, getState());
  enterRule(_localctx, 36, ZHLParser::RuleInheritance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    inheritanceDecl();

    setState(235);
    match(ZHLParser::Comma);
    setState(236);
    inheritanceDecl();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InheritanceDeclContext ------------------------------------------------------------------

ZHLParser::InheritanceDeclContext::InheritanceDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::NestedNameContext* ZHLParser::InheritanceDeclContext::nestedName() {
  return getRuleContext<ZHLParser::NestedNameContext>(0);
}

tree::TerminalNode* ZHLParser::InheritanceDeclContext::Visibility() {
  return getToken(ZHLParser::Visibility, 0);
}


size_t ZHLParser::InheritanceDeclContext::getRuleIndex() const {
  return ZHLParser::RuleInheritanceDecl;
}

void ZHLParser::InheritanceDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInheritanceDecl(this);
}

void ZHLParser::InheritanceDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInheritanceDecl(this);
}


std::any ZHLParser::InheritanceDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitInheritanceDecl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::InheritanceDeclContext* ZHLParser::inheritanceDecl() {
  InheritanceDeclContext *_localctx = _tracker.createInstance<InheritanceDeclContext>(_ctx, getState());
  enterRule(_localctx, 38, ZHLParser::RuleInheritanceDecl);
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
    setState(239);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility) {
      setState(238);
      match(ZHLParser::Visibility);
    }
    setState(241);
    nestedName();
   
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
  enterRule(_localctx, 40, ZHLParser::RuleDepends);

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
    setState(243);
    match(ZHLParser::Depends);
    setState(244);
    match(ZHLParser::LeftParen);
    setState(249);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(245);
        match(ZHLParser::Name);
        setState(246);
        match(ZHLParser::Comma); 
      }
      setState(251);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    }
    setState(252);
    match(ZHLParser::Name);
    setState(253);
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

std::vector<ZHLParser::ClassBodyElementContext *> ZHLParser::ClassBodyContext::classBodyElement() {
  return getRuleContexts<ZHLParser::ClassBodyElementContext>();
}

ZHLParser::ClassBodyElementContext* ZHLParser::ClassBodyContext::classBodyElement(size_t i) {
  return getRuleContext<ZHLParser::ClassBodyElementContext>(i);
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
  enterRule(_localctx, 42, ZHLParser::RuleClassBody);
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
    setState(258);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 19) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 19)) & 2427721674260403) != 0)) {
      setState(255);
      classBodyElement();
      setState(260);
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

//----------------- ClassBodyElementContext ------------------------------------------------------------------

ZHLParser::ClassBodyElementContext::ClassBodyElementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::ClassSignatureContext* ZHLParser::ClassBodyElementContext::classSignature() {
  return getRuleContext<ZHLParser::ClassSignatureContext>(0);
}

ZHLParser::GenericCodeContext* ZHLParser::ClassBodyElementContext::genericCode() {
  return getRuleContext<ZHLParser::GenericCodeContext>(0);
}

ZHLParser::ClassFieldContext* ZHLParser::ClassBodyElementContext::classField() {
  return getRuleContext<ZHLParser::ClassFieldContext>(0);
}

ZHLParser::TypedefContext* ZHLParser::ClassBodyElementContext::typedef_() {
  return getRuleContext<ZHLParser::TypedefContext>(0);
}

ZHLParser::FunctionPtrContext* ZHLParser::ClassBodyElementContext::functionPtr() {
  return getRuleContext<ZHLParser::FunctionPtrContext>(0);
}

ZHLParser::ForwardDeclContext* ZHLParser::ClassBodyElementContext::forwardDecl() {
  return getRuleContext<ZHLParser::ForwardDeclContext>(0);
}


size_t ZHLParser::ClassBodyElementContext::getRuleIndex() const {
  return ZHLParser::RuleClassBodyElement;
}

void ZHLParser::ClassBodyElementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterClassBodyElement(this);
}

void ZHLParser::ClassBodyElementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitClassBodyElement(this);
}


std::any ZHLParser::ClassBodyElementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitClassBodyElement(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ClassBodyElementContext* ZHLParser::classBodyElement() {
  ClassBodyElementContext *_localctx = _tracker.createInstance<ClassBodyElementContext>(_ctx, getState());
  enterRule(_localctx, 44, ZHLParser::RuleClassBodyElement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(267);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(261);
      classSignature();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(262);
      genericCode();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(263);
      classField();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(264);
      typedef_();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(265);
      functionPtr();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(266);
      forwardDecl();
      break;
    }

    default:
      break;
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
  enterRule(_localctx, 46, ZHLParser::RuleClassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(269);
    match(ZHLParser::Signature);
    setState(270);
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
  enterRule(_localctx, 48, ZHLParser::RuleClassFunction);
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
    setState(275);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(272);
      match(ZHLParser::Qualifier);
      setState(277);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(278);
    type();
    setState(279);
    match(ZHLParser::Name);
    setState(280);
    match(ZHLParser::LeftParen);
    setState(281);
    funArgs();
    setState(282);
    match(ZHLParser::RightParen);
    setState(283);
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
  enterRule(_localctx, 50, ZHLParser::RuleClassField);
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
    setState(285);
    type();
    setState(286);
    innerField();
    setState(291);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(287);
      match(ZHLParser::Comma);
      setState(288);
      innerField();
      setState(293);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(294);
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

std::vector<tree::TerminalNode *> ZHLParser::InnerFieldContext::Star() {
  return getTokens(ZHLParser::Star);
}

tree::TerminalNode* ZHLParser::InnerFieldContext::Star(size_t i) {
  return getToken(ZHLParser::Star, i);
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
  enterRule(_localctx, 52, ZHLParser::RuleInnerField);
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
    setState(299);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Star) {
      setState(296);
      match(ZHLParser::Star);
      setState(301);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(302);
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
  enterRule(_localctx, 54, ZHLParser::RuleFullName);
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
    setState(304);
    match(ZHLParser::Name);
    setState(308);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::LeftRBracket) {
      setState(305);
      match(ZHLParser::LeftRBracket);
      setState(306);
      match(ZHLParser::Number);
      setState(307);
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
  enterRule(_localctx, 56, ZHLParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(314);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Signature: {
        setState(310);
        match(ZHLParser::Signature);
        setState(311);
        function();
        break;
      }

      case ZHLParser::ReferenceSignature: {
        setState(312);
        match(ZHLParser::ReferenceSignature);
        setState(313);
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

//----------------- ForwardDeclContext ------------------------------------------------------------------

ZHLParser::ForwardDeclContext::ForwardDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Struct() {
  return getToken(ZHLParser::Struct, 0);
}

tree::TerminalNode* ZHLParser::ForwardDeclContext::Class() {
  return getToken(ZHLParser::Class, 0);
}


size_t ZHLParser::ForwardDeclContext::getRuleIndex() const {
  return ZHLParser::RuleForwardDecl;
}

void ZHLParser::ForwardDeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForwardDecl(this);
}

void ZHLParser::ForwardDeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForwardDecl(this);
}


std::any ZHLParser::ForwardDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitForwardDecl(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::ForwardDeclContext* ZHLParser::forwardDecl() {
  ForwardDeclContext *_localctx = _tracker.createInstance<ForwardDeclContext>(_ctx, getState());
  enterRule(_localctx, 58, ZHLParser::RuleForwardDecl);
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
    setState(316);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(317);
    match(ZHLParser::Name);
    setState(318);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypedefContext ------------------------------------------------------------------

ZHLParser::TypedefContext::TypedefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ZHLParser::TypedefContext::Typedef() {
  return getToken(ZHLParser::Typedef, 0);
}

ZHLParser::TypeContext* ZHLParser::TypedefContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

tree::TerminalNode* ZHLParser::TypedefContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

tree::TerminalNode* ZHLParser::TypedefContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}


size_t ZHLParser::TypedefContext::getRuleIndex() const {
  return ZHLParser::RuleTypedef;
}

void ZHLParser::TypedefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypedef(this);
}

void ZHLParser::TypedefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypedef(this);
}


std::any ZHLParser::TypedefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitTypedef(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::TypedefContext* ZHLParser::typedef_() {
  TypedefContext *_localctx = _tracker.createInstance<TypedefContext>(_ctx, getState());
  enterRule(_localctx, 60, ZHLParser::RuleTypedef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(320);
    match(ZHLParser::Typedef);
    setState(321);
    type();
    setState(322);
    match(ZHLParser::Name);
    setState(323);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionPtrContext ------------------------------------------------------------------

ZHLParser::FunctionPtrContext::FunctionPtrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::FptrContext* ZHLParser::FunctionPtrContext::fptr() {
  return getRuleContext<ZHLParser::FptrContext>(0);
}

ZHLParser::MemberPtrContext* ZHLParser::FunctionPtrContext::memberPtr() {
  return getRuleContext<ZHLParser::MemberPtrContext>(0);
}


size_t ZHLParser::FunctionPtrContext::getRuleIndex() const {
  return ZHLParser::RuleFunctionPtr;
}

void ZHLParser::FunctionPtrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionPtr(this);
}

void ZHLParser::FunctionPtrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionPtr(this);
}


std::any ZHLParser::FunctionPtrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFunctionPtr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FunctionPtrContext* ZHLParser::functionPtr() {
  FunctionPtrContext *_localctx = _tracker.createInstance<FunctionPtrContext>(_ctx, getState());
  enterRule(_localctx, 62, ZHLParser::RuleFunctionPtr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(327);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(325);
      fptr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(326);
      memberPtr();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FptrContext ------------------------------------------------------------------

ZHLParser::FptrContext::FptrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::FptrContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::LeftParen() {
  return getTokens(ZHLParser::LeftParen);
}

tree::TerminalNode* ZHLParser::FptrContext::LeftParen(size_t i) {
  return getToken(ZHLParser::LeftParen, i);
}

tree::TerminalNode* ZHLParser::FptrContext::Name() {
  return getToken(ZHLParser::Name, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::RightParen() {
  return getTokens(ZHLParser::RightParen);
}

tree::TerminalNode* ZHLParser::FptrContext::RightParen(size_t i) {
  return getToken(ZHLParser::RightParen, i);
}

tree::TerminalNode* ZHLParser::FptrContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::FptrContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::FptrContext::Star() {
  return getTokens(ZHLParser::Star);
}

tree::TerminalNode* ZHLParser::FptrContext::Star(size_t i) {
  return getToken(ZHLParser::Star, i);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::FptrContext::optNamedFunArgs() {
  return getRuleContext<ZHLParser::OptNamedFunArgsContext>(0);
}


size_t ZHLParser::FptrContext::getRuleIndex() const {
  return ZHLParser::RuleFptr;
}

void ZHLParser::FptrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFptr(this);
}

void ZHLParser::FptrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFptr(this);
}


std::any ZHLParser::FptrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitFptr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::FptrContext* ZHLParser::fptr() {
  FptrContext *_localctx = _tracker.createInstance<FptrContext>(_ctx, getState());
  enterRule(_localctx, 64, ZHLParser::RuleFptr);
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
    setState(329);
    type();
    setState(330);
    match(ZHLParser::LeftParen);
    setState(332);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(331);
      match(ZHLParser::CallingConvention);
    }
    setState(335); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(334);
      match(ZHLParser::Star);
      setState(337); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(339);
    match(ZHLParser::Name);
    setState(340);
    match(ZHLParser::RightParen);
    setState(341);
    match(ZHLParser::LeftParen);
    setState(343);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(342);
      optNamedFunArgs();
    }
    setState(345);
    match(ZHLParser::RightParen);
    setState(346);
    match(ZHLParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MemberPtrContext ------------------------------------------------------------------

ZHLParser::MemberPtrContext::MemberPtrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ZHLParser::TypeContext* ZHLParser::MemberPtrContext::type() {
  return getRuleContext<ZHLParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::LeftParen() {
  return getTokens(ZHLParser::LeftParen);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::LeftParen(size_t i) {
  return getToken(ZHLParser::LeftParen, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Name() {
  return getTokens(ZHLParser::Name);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Name(size_t i) {
  return getToken(ZHLParser::Name, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Colon() {
  return getTokens(ZHLParser::Colon);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Colon(size_t i) {
  return getToken(ZHLParser::Colon, i);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::RightParen() {
  return getTokens(ZHLParser::RightParen);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::RightParen(size_t i) {
  return getToken(ZHLParser::RightParen, i);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Semi() {
  return getToken(ZHLParser::Semi, 0);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::CallingConvention() {
  return getToken(ZHLParser::CallingConvention, 0);
}

std::vector<tree::TerminalNode *> ZHLParser::MemberPtrContext::Star() {
  return getTokens(ZHLParser::Star);
}

tree::TerminalNode* ZHLParser::MemberPtrContext::Star(size_t i) {
  return getToken(ZHLParser::Star, i);
}

ZHLParser::OptNamedFunArgsContext* ZHLParser::MemberPtrContext::optNamedFunArgs() {
  return getRuleContext<ZHLParser::OptNamedFunArgsContext>(0);
}


size_t ZHLParser::MemberPtrContext::getRuleIndex() const {
  return ZHLParser::RuleMemberPtr;
}

void ZHLParser::MemberPtrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMemberPtr(this);
}

void ZHLParser::MemberPtrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ZHLParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMemberPtr(this);
}


std::any ZHLParser::MemberPtrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ZHLParserVisitor*>(visitor))
    return parserVisitor->visitMemberPtr(this);
  else
    return visitor->visitChildren(this);
}

ZHLParser::MemberPtrContext* ZHLParser::memberPtr() {
  MemberPtrContext *_localctx = _tracker.createInstance<MemberPtrContext>(_ctx, getState());
  enterRule(_localctx, 66, ZHLParser::RuleMemberPtr);
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
    setState(348);
    type();
    setState(349);
    match(ZHLParser::LeftParen);
    setState(351);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(350);
      match(ZHLParser::CallingConvention);
    }
    setState(353);
    match(ZHLParser::Name);
    setState(354);
    match(ZHLParser::Colon);
    setState(355);
    match(ZHLParser::Colon);
    setState(361);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Name) {
      setState(356);
      match(ZHLParser::Name);
      setState(357);
      match(ZHLParser::Colon);
      setState(358);
      match(ZHLParser::Colon);
      setState(363);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(365); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(364);
      match(ZHLParser::Star);
      setState(367); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(369);
    match(ZHLParser::Name);
    setState(370);
    match(ZHLParser::RightParen);
    setState(371);
    match(ZHLParser::LeftParen);
    setState(373);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(372);
      optNamedFunArgs();
    }
    setState(375);
    match(ZHLParser::RightParen);
    setState(376);
    match(ZHLParser::Semi);
   
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
