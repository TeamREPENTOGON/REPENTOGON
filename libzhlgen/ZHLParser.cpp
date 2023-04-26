
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
      "classBody", "classSignature", "classFunction", "classField", "innerField", 
      "fullName", "signature", "forwardDecl", "typedef", "functionPtr", 
      "fptr", "memberPtr"
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
  	4,1,75,374,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,1,0,1,0,1,0,1,0,1,0,1,0,5,
  	0,73,8,0,10,0,12,0,76,9,0,1,0,1,0,1,1,5,1,81,8,1,10,1,12,1,84,9,1,1,1,
  	3,1,87,8,1,1,1,1,1,1,1,1,1,3,1,93,8,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,
  	1,3,1,3,1,3,5,3,106,8,3,10,3,12,3,109,9,3,1,4,1,4,1,4,3,4,114,8,4,1,5,
  	1,5,1,5,1,5,1,6,1,6,1,6,5,6,123,8,6,10,6,12,6,126,9,6,1,7,1,7,3,7,130,
  	8,7,1,8,1,8,1,8,1,8,5,8,136,8,8,10,8,12,8,139,9,8,1,8,1,8,1,9,1,9,3,9,
  	145,8,9,1,9,5,9,148,8,9,10,9,12,9,151,9,9,1,10,3,10,154,8,10,1,10,1,10,
  	3,10,158,8,10,1,10,3,10,161,8,10,1,11,1,11,3,11,165,8,11,1,11,4,11,168,
  	8,11,11,11,12,11,169,1,11,3,11,173,8,11,1,11,1,11,1,11,1,11,3,11,179,
  	8,11,1,11,5,11,182,8,11,10,11,12,11,185,9,11,1,11,1,11,1,11,3,11,190,
  	8,11,1,11,3,11,193,8,11,1,12,1,12,1,13,1,13,1,14,1,14,1,14,1,14,5,14,
  	203,8,14,10,14,12,14,206,9,14,1,15,1,15,3,15,210,8,15,1,15,1,15,3,15,
  	214,8,15,3,15,216,8,15,1,16,1,16,1,17,1,17,1,17,3,17,223,8,17,1,17,3,
  	17,226,8,17,1,17,1,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,1,19,3,19,238,
  	8,19,1,19,1,19,1,20,1,20,1,20,1,20,5,20,246,8,20,10,20,12,20,249,9,20,
  	1,20,1,20,1,20,1,21,1,21,1,21,1,21,1,21,1,21,5,21,260,8,21,10,21,12,21,
  	263,9,21,1,22,1,22,1,22,1,23,5,23,269,8,23,10,23,12,23,272,9,23,1,23,
  	1,23,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,5,24,285,8,24,10,24,
  	12,24,288,9,24,1,24,1,24,1,25,5,25,293,8,25,10,25,12,25,296,9,25,1,25,
  	1,25,1,26,1,26,1,26,1,26,3,26,304,8,26,1,27,1,27,1,27,1,27,3,27,310,8,
  	27,1,28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,29,1,30,1,30,3,30,323,8,
  	30,1,31,1,31,1,31,3,31,328,8,31,1,31,4,31,331,8,31,11,31,12,31,332,1,
  	31,1,31,1,31,1,31,3,31,339,8,31,1,31,1,31,1,31,1,32,1,32,1,32,3,32,347,
  	8,32,1,32,1,32,1,32,1,32,1,32,1,32,5,32,355,8,32,10,32,12,32,358,9,32,
  	1,32,4,32,361,8,32,11,32,12,32,362,1,32,1,32,1,32,1,32,3,32,369,8,32,
  	1,32,1,32,1,32,1,32,0,0,33,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
  	32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,0,3,1,0,26,27,2,0,
  	28,28,30,30,1,0,19,20,399,0,74,1,0,0,0,2,82,1,0,0,0,4,97,1,0,0,0,6,102,
  	1,0,0,0,8,110,1,0,0,0,10,115,1,0,0,0,12,119,1,0,0,0,14,127,1,0,0,0,16,
  	131,1,0,0,0,18,142,1,0,0,0,20,153,1,0,0,0,22,192,1,0,0,0,24,194,1,0,0,
  	0,26,196,1,0,0,0,28,198,1,0,0,0,30,215,1,0,0,0,32,217,1,0,0,0,34,219,
  	1,0,0,0,36,232,1,0,0,0,38,237,1,0,0,0,40,241,1,0,0,0,42,261,1,0,0,0,44,
  	264,1,0,0,0,46,270,1,0,0,0,48,280,1,0,0,0,50,294,1,0,0,0,52,299,1,0,0,
  	0,54,309,1,0,0,0,56,311,1,0,0,0,58,315,1,0,0,0,60,322,1,0,0,0,62,324,
  	1,0,0,0,64,343,1,0,0,0,66,73,3,54,27,0,67,73,3,34,17,0,68,73,3,32,16,
  	0,69,73,3,58,29,0,70,73,3,60,30,0,71,73,3,56,28,0,72,66,1,0,0,0,72,67,
  	1,0,0,0,72,68,1,0,0,0,72,69,1,0,0,0,72,70,1,0,0,0,72,71,1,0,0,0,73,76,
  	1,0,0,0,74,72,1,0,0,0,74,75,1,0,0,0,75,77,1,0,0,0,76,74,1,0,0,0,77,78,
  	5,0,0,1,78,1,1,0,0,0,79,81,5,4,0,0,80,79,1,0,0,0,81,84,1,0,0,0,82,80,
  	1,0,0,0,82,83,1,0,0,0,83,86,1,0,0,0,84,82,1,0,0,0,85,87,5,59,0,0,86,85,
  	1,0,0,0,86,87,1,0,0,0,87,88,1,0,0,0,88,89,3,18,9,0,89,90,3,28,14,0,90,
  	92,5,9,0,0,91,93,3,6,3,0,92,91,1,0,0,0,92,93,1,0,0,0,93,94,1,0,0,0,94,
  	95,5,10,0,0,95,96,5,3,0,0,96,3,1,0,0,0,97,98,5,21,0,0,98,99,3,18,9,0,
  	99,100,5,66,0,0,100,101,5,3,0,0,101,5,1,0,0,0,102,107,3,8,4,0,103,104,
  	5,11,0,0,104,106,3,8,4,0,105,103,1,0,0,0,106,109,1,0,0,0,107,105,1,0,
  	0,0,107,108,1,0,0,0,108,7,1,0,0,0,109,107,1,0,0,0,110,111,3,18,9,0,111,
  	113,3,52,26,0,112,114,3,10,5,0,113,112,1,0,0,0,113,114,1,0,0,0,114,9,
  	1,0,0,0,115,116,5,13,0,0,116,117,5,40,0,0,117,118,5,14,0,0,118,11,1,0,
  	0,0,119,124,3,14,7,0,120,121,5,11,0,0,121,123,3,14,7,0,122,120,1,0,0,
  	0,123,126,1,0,0,0,124,122,1,0,0,0,124,125,1,0,0,0,125,13,1,0,0,0,126,
  	124,1,0,0,0,127,129,3,18,9,0,128,130,3,52,26,0,129,128,1,0,0,0,129,130,
  	1,0,0,0,130,15,1,0,0,0,131,132,5,13,0,0,132,137,3,18,9,0,133,134,5,11,
  	0,0,134,136,3,18,9,0,135,133,1,0,0,0,136,139,1,0,0,0,137,135,1,0,0,0,
  	137,138,1,0,0,0,138,140,1,0,0,0,139,137,1,0,0,0,140,141,5,14,0,0,141,
  	17,1,0,0,0,142,144,3,20,10,0,143,145,3,16,8,0,144,143,1,0,0,0,144,145,
  	1,0,0,0,145,149,1,0,0,0,146,148,3,30,15,0,147,146,1,0,0,0,148,151,1,0,
  	0,0,149,147,1,0,0,0,149,150,1,0,0,0,150,19,1,0,0,0,151,149,1,0,0,0,152,
  	154,5,24,0,0,153,152,1,0,0,0,153,154,1,0,0,0,154,157,1,0,0,0,155,158,
  	3,22,11,0,156,158,3,28,14,0,157,155,1,0,0,0,157,156,1,0,0,0,158,160,1,
  	0,0,0,159,161,5,24,0,0,160,159,1,0,0,0,160,161,1,0,0,0,161,21,1,0,0,0,
  	162,193,3,24,12,0,163,165,3,24,12,0,164,163,1,0,0,0,164,165,1,0,0,0,165,
  	167,1,0,0,0,166,168,3,26,13,0,167,166,1,0,0,0,168,169,1,0,0,0,169,167,
  	1,0,0,0,169,170,1,0,0,0,170,193,1,0,0,0,171,173,3,24,12,0,172,171,1,0,
  	0,0,172,173,1,0,0,0,173,174,1,0,0,0,174,193,5,31,0,0,175,193,5,32,0,0,
  	176,193,5,35,0,0,177,179,3,24,12,0,178,177,1,0,0,0,178,179,1,0,0,0,179,
  	183,1,0,0,0,180,182,3,26,13,0,181,180,1,0,0,0,182,185,1,0,0,0,183,181,
  	1,0,0,0,183,184,1,0,0,0,184,186,1,0,0,0,185,183,1,0,0,0,186,193,5,29,
  	0,0,187,193,5,33,0,0,188,190,3,26,13,0,189,188,1,0,0,0,189,190,1,0,0,
  	0,190,191,1,0,0,0,191,193,5,34,0,0,192,162,1,0,0,0,192,164,1,0,0,0,192,
  	172,1,0,0,0,192,175,1,0,0,0,192,176,1,0,0,0,192,178,1,0,0,0,192,187,1,
  	0,0,0,192,189,1,0,0,0,193,23,1,0,0,0,194,195,7,0,0,0,195,25,1,0,0,0,196,
  	197,7,1,0,0,197,27,1,0,0,0,198,204,5,66,0,0,199,200,5,2,0,0,200,201,5,
  	2,0,0,201,203,5,66,0,0,202,199,1,0,0,0,203,206,1,0,0,0,204,202,1,0,0,
  	0,204,205,1,0,0,0,205,29,1,0,0,0,206,204,1,0,0,0,207,209,5,25,0,0,208,
  	210,5,25,0,0,209,208,1,0,0,0,209,210,1,0,0,0,210,216,1,0,0,0,211,213,
  	5,12,0,0,212,214,5,24,0,0,213,212,1,0,0,0,213,214,1,0,0,0,214,216,1,0,
  	0,0,215,207,1,0,0,0,215,211,1,0,0,0,216,31,1,0,0,0,217,218,5,70,0,0,218,
  	33,1,0,0,0,219,220,7,2,0,0,220,222,5,66,0,0,221,223,3,40,20,0,222,221,
  	1,0,0,0,222,223,1,0,0,0,223,225,1,0,0,0,224,226,3,36,18,0,225,224,1,0,
  	0,0,225,226,1,0,0,0,226,227,1,0,0,0,227,228,5,17,0,0,228,229,3,42,21,
  	0,229,230,5,18,0,0,230,231,5,3,0,0,231,35,1,0,0,0,232,233,3,38,19,0,233,
  	234,5,11,0,0,234,235,3,38,19,0,235,37,1,0,0,0,236,238,5,36,0,0,237,236,
  	1,0,0,0,237,238,1,0,0,0,238,239,1,0,0,0,239,240,3,28,14,0,240,39,1,0,
  	0,0,241,242,5,22,0,0,242,247,5,9,0,0,243,244,5,66,0,0,244,246,5,11,0,
  	0,245,243,1,0,0,0,246,249,1,0,0,0,247,245,1,0,0,0,247,248,1,0,0,0,248,
  	250,1,0,0,0,249,247,1,0,0,0,250,251,5,66,0,0,251,252,5,10,0,0,252,41,
  	1,0,0,0,253,260,3,44,22,0,254,260,3,32,16,0,255,260,3,48,24,0,256,260,
  	3,58,29,0,257,260,3,60,30,0,258,260,3,56,28,0,259,253,1,0,0,0,259,254,
  	1,0,0,0,259,255,1,0,0,0,259,256,1,0,0,0,259,257,1,0,0,0,259,258,1,0,0,
  	0,260,263,1,0,0,0,261,259,1,0,0,0,261,262,1,0,0,0,262,43,1,0,0,0,263,
  	261,1,0,0,0,264,265,5,64,0,0,265,266,3,46,23,0,266,45,1,0,0,0,267,269,
  	5,4,0,0,268,267,1,0,0,0,269,272,1,0,0,0,270,268,1,0,0,0,270,271,1,0,0,
  	0,271,273,1,0,0,0,272,270,1,0,0,0,273,274,3,18,9,0,274,275,5,66,0,0,275,
  	276,5,9,0,0,276,277,3,6,3,0,277,278,5,10,0,0,278,279,5,3,0,0,279,47,1,
  	0,0,0,280,281,3,18,9,0,281,286,3,50,25,0,282,283,5,11,0,0,283,285,3,50,
  	25,0,284,282,1,0,0,0,285,288,1,0,0,0,286,284,1,0,0,0,286,287,1,0,0,0,
  	287,289,1,0,0,0,288,286,1,0,0,0,289,290,5,3,0,0,290,49,1,0,0,0,291,293,
  	5,12,0,0,292,291,1,0,0,0,293,296,1,0,0,0,294,292,1,0,0,0,294,295,1,0,
  	0,0,295,297,1,0,0,0,296,294,1,0,0,0,297,298,3,52,26,0,298,51,1,0,0,0,
  	299,303,5,66,0,0,300,301,5,15,0,0,301,302,5,67,0,0,302,304,5,16,0,0,303,
  	300,1,0,0,0,303,304,1,0,0,0,304,53,1,0,0,0,305,306,5,64,0,0,306,310,3,
  	2,1,0,307,308,5,65,0,0,308,310,3,4,2,0,309,305,1,0,0,0,309,307,1,0,0,
  	0,310,55,1,0,0,0,311,312,7,2,0,0,312,313,5,66,0,0,313,314,5,3,0,0,314,
  	57,1,0,0,0,315,316,5,23,0,0,316,317,3,18,9,0,317,318,5,66,0,0,318,319,
  	5,3,0,0,319,59,1,0,0,0,320,323,3,62,31,0,321,323,3,64,32,0,322,320,1,
  	0,0,0,322,321,1,0,0,0,323,61,1,0,0,0,324,325,3,18,9,0,325,327,5,9,0,0,
  	326,328,5,59,0,0,327,326,1,0,0,0,327,328,1,0,0,0,328,330,1,0,0,0,329,
  	331,5,12,0,0,330,329,1,0,0,0,331,332,1,0,0,0,332,330,1,0,0,0,332,333,
  	1,0,0,0,333,334,1,0,0,0,334,335,5,66,0,0,335,336,5,10,0,0,336,338,5,9,
  	0,0,337,339,3,12,6,0,338,337,1,0,0,0,338,339,1,0,0,0,339,340,1,0,0,0,
  	340,341,5,10,0,0,341,342,5,3,0,0,342,63,1,0,0,0,343,344,3,18,9,0,344,
  	346,5,9,0,0,345,347,5,59,0,0,346,345,1,0,0,0,346,347,1,0,0,0,347,348,
  	1,0,0,0,348,349,5,66,0,0,349,350,5,2,0,0,350,356,5,2,0,0,351,352,5,66,
  	0,0,352,353,5,2,0,0,353,355,5,2,0,0,354,351,1,0,0,0,355,358,1,0,0,0,356,
  	354,1,0,0,0,356,357,1,0,0,0,357,360,1,0,0,0,358,356,1,0,0,0,359,361,5,
  	12,0,0,360,359,1,0,0,0,361,362,1,0,0,0,362,360,1,0,0,0,362,363,1,0,0,
  	0,363,364,1,0,0,0,364,365,5,66,0,0,365,366,5,10,0,0,366,368,5,9,0,0,367,
  	369,3,12,6,0,368,367,1,0,0,0,368,369,1,0,0,0,369,370,1,0,0,0,370,371,
  	5,10,0,0,371,372,5,3,0,0,372,65,1,0,0,0,45,72,74,82,86,92,107,113,124,
  	129,137,144,149,153,157,160,164,169,172,178,183,189,192,204,209,213,215,
  	222,225,237,247,259,261,270,286,294,303,309,322,327,332,338,346,356,362,
  	368
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
    setState(74);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 19) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 19)) & 2498090418438067) != 0)) {
      setState(72);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(66);
        signature();
        break;
      }

      case 2: {
        setState(67);
        class_();
        break;
      }

      case 3: {
        setState(68);
        genericCode();
        break;
      }

      case 4: {
        setState(69);
        typedef_();
        break;
      }

      case 5: {
        setState(70);
        functionPtr();
        break;
      }

      case 6: {
        setState(71);
        forwardDecl();
        break;
      }

      default:
        break;
      }
      setState(76);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(77);
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
    setState(82);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(79);
      match(ZHLParser::Qualifier);
      setState(84);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(86);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(85);
      match(ZHLParser::CallingConvention);
    }
    setState(88);
    type();
    setState(89);
    nestedName();
    setState(90);
    match(ZHLParser::LeftParen);
    setState(92);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(91);
      funArgs();
    }
    setState(94);
    match(ZHLParser::RightParen);
    setState(95);
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
    setState(97);
    match(ZHLParser::Reference);
    setState(98);
    type();
    setState(99);
    match(ZHLParser::Name);
    setState(100);
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
    setState(102);
    funArg();
    setState(107);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(103);
      match(ZHLParser::Comma);
      setState(104);
      funArg();
      setState(109);
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
    setState(110);
    type();
    setState(111);
    fullName();
    setState(113);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(112);
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
    setState(115);
    match(ZHLParser::Lt);
    setState(116);
    match(ZHLParser::Register);
    setState(117);
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
    setState(119);
    optNamedFunArg();
    setState(124);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(120);
      match(ZHLParser::Comma);
      setState(121);
      optNamedFunArg();
      setState(126);
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
    setState(127);
    type();
    setState(129);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Name) {
      setState(128);
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
    setState(131);
    match(ZHLParser::Lt);
    setState(132);
    type();
    setState(137);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(133);
      match(ZHLParser::Comma);
      setState(134);
      type();
      setState(139);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(140);
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
    setState(142);
    typeSpecifier();
    setState(144);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Lt) {
      setState(143);
      templateSpec();
    }
    setState(149);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(146);
        pointerAttribute(); 
      }
      setState(151);
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
    setState(153);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(152);
      match(ZHLParser::Const);
    }
    setState(157);
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
        setState(155);
        simpleType();
        break;
      }

      case ZHLParser::Name: {
        setState(156);
        nestedName();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(160);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Const) {
      setState(159);
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
    setState(192);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(162);
      simpleTypeSignedness();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(164);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(163);
        simpleTypeSignedness();
      }
      setState(167); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(166);
        simpleTypeLength();
        setState(169); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == ZHLParser::Long

      || _la == ZHLParser::Short);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(172);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(171);
        simpleTypeSignedness();
      }
      setState(174);
      match(ZHLParser::Char);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(175);
      match(ZHLParser::Bool);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(176);
      match(ZHLParser::Void);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(178);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Unsigned

      || _la == ZHLParser::Signed) {
        setState(177);
        simpleTypeSignedness();
      }
      setState(183);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(180);
        simpleTypeLength();
        setState(185);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(186);
      match(ZHLParser::Int);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(187);
      match(ZHLParser::Float);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(189);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == ZHLParser::Long

      || _la == ZHLParser::Short) {
        setState(188);
        simpleTypeLength();
      }
      setState(191);
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
    setState(194);
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
    setState(196);
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
    setState(198);
    match(ZHLParser::Name);
    setState(204);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Colon) {
      setState(199);
      match(ZHLParser::Colon);
      setState(200);
      match(ZHLParser::Colon);
      setState(201);
      match(ZHLParser::Name);
      setState(206);
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
    setState(215);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::CppRef: {
        enterOuterAlt(_localctx, 1);
        setState(207);
        match(ZHLParser::CppRef);
        setState(209);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 23, _ctx)) {
        case 1: {
          setState(208);
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
        setState(211);
        match(ZHLParser::Star);
        setState(213);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == ZHLParser::Const) {
          setState(212);
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
    setState(217);
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
    setState(219);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(220);
    match(ZHLParser::Name);
    setState(222);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Depends) {
      setState(221);
      depends();
    }
    setState(225);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility

    || _la == ZHLParser::Name) {
      setState(224);
      inheritance();
    }
    setState(227);
    match(ZHLParser::LeftBracket);
    setState(228);
    classBody();
    setState(229);
    match(ZHLParser::RightBracket);
    setState(230);
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
    setState(232);
    inheritanceDecl();

    setState(233);
    match(ZHLParser::Comma);
    setState(234);
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
    setState(237);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::Visibility) {
      setState(236);
      match(ZHLParser::Visibility);
    }
    setState(239);
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
    setState(241);
    match(ZHLParser::Depends);
    setState(242);
    match(ZHLParser::LeftParen);
    setState(247);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(243);
        match(ZHLParser::Name);
        setState(244);
        match(ZHLParser::Comma); 
      }
      setState(249);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    }
    setState(250);
    match(ZHLParser::Name);
    setState(251);
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

std::vector<ZHLParser::TypedefContext *> ZHLParser::ClassBodyContext::typedef_() {
  return getRuleContexts<ZHLParser::TypedefContext>();
}

ZHLParser::TypedefContext* ZHLParser::ClassBodyContext::typedef_(size_t i) {
  return getRuleContext<ZHLParser::TypedefContext>(i);
}

std::vector<ZHLParser::FunctionPtrContext *> ZHLParser::ClassBodyContext::functionPtr() {
  return getRuleContexts<ZHLParser::FunctionPtrContext>();
}

ZHLParser::FunctionPtrContext* ZHLParser::ClassBodyContext::functionPtr(size_t i) {
  return getRuleContext<ZHLParser::FunctionPtrContext>(i);
}

std::vector<ZHLParser::ForwardDeclContext *> ZHLParser::ClassBodyContext::forwardDecl() {
  return getRuleContexts<ZHLParser::ForwardDeclContext>();
}

ZHLParser::ForwardDeclContext* ZHLParser::ClassBodyContext::forwardDecl(size_t i) {
  return getRuleContext<ZHLParser::ForwardDeclContext>(i);
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
    setState(261);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 19) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 19)) & 2427721674260403) != 0)) {
      setState(259);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
      case 1: {
        setState(253);
        classSignature();
        break;
      }

      case 2: {
        setState(254);
        genericCode();
        break;
      }

      case 3: {
        setState(255);
        classField();
        break;
      }

      case 4: {
        setState(256);
        typedef_();
        break;
      }

      case 5: {
        setState(257);
        functionPtr();
        break;
      }

      case 6: {
        setState(258);
        forwardDecl();
        break;
      }

      default:
        break;
      }
      setState(263);
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
  enterRule(_localctx, 44, ZHLParser::RuleClassSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(264);
    match(ZHLParser::Signature);
    setState(265);
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
  enterRule(_localctx, 46, ZHLParser::RuleClassFunction);
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
    setState(270);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Qualifier) {
      setState(267);
      match(ZHLParser::Qualifier);
      setState(272);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(273);
    type();
    setState(274);
    match(ZHLParser::Name);
    setState(275);
    match(ZHLParser::LeftParen);
    setState(276);
    funArgs();
    setState(277);
    match(ZHLParser::RightParen);
    setState(278);
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
  enterRule(_localctx, 48, ZHLParser::RuleClassField);
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
    setState(280);
    type();
    setState(281);
    innerField();
    setState(286);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Comma) {
      setState(282);
      match(ZHLParser::Comma);
      setState(283);
      innerField();
      setState(288);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(289);
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
  enterRule(_localctx, 50, ZHLParser::RuleInnerField);
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
    setState(294);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Star) {
      setState(291);
      match(ZHLParser::Star);
      setState(296);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(297);
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
  enterRule(_localctx, 52, ZHLParser::RuleFullName);
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
    match(ZHLParser::Name);
    setState(303);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::LeftRBracket) {
      setState(300);
      match(ZHLParser::LeftRBracket);
      setState(301);
      match(ZHLParser::Number);
      setState(302);
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
  enterRule(_localctx, 54, ZHLParser::RuleSignature);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(309);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ZHLParser::Signature: {
        setState(305);
        match(ZHLParser::Signature);
        setState(306);
        function();
        break;
      }

      case ZHLParser::ReferenceSignature: {
        setState(307);
        match(ZHLParser::ReferenceSignature);
        setState(308);
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
  enterRule(_localctx, 56, ZHLParser::RuleForwardDecl);
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
    setState(311);
    _la = _input->LA(1);
    if (!(_la == ZHLParser::Class

    || _la == ZHLParser::Struct)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(312);
    match(ZHLParser::Name);
    setState(313);
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
  enterRule(_localctx, 58, ZHLParser::RuleTypedef);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(315);
    match(ZHLParser::Typedef);
    setState(316);
    type();
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
  enterRule(_localctx, 60, ZHLParser::RuleFunctionPtr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(322);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(320);
      fptr();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(321);
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
  enterRule(_localctx, 62, ZHLParser::RuleFptr);
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
    setState(324);
    type();
    setState(325);
    match(ZHLParser::LeftParen);
    setState(327);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(326);
      match(ZHLParser::CallingConvention);
    }
    setState(330); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(329);
      match(ZHLParser::Star);
      setState(332); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(334);
    match(ZHLParser::Name);
    setState(335);
    match(ZHLParser::RightParen);
    setState(336);
    match(ZHLParser::LeftParen);
    setState(338);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(337);
      optNamedFunArgs();
    }
    setState(340);
    match(ZHLParser::RightParen);
    setState(341);
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
  enterRule(_localctx, 64, ZHLParser::RuleMemberPtr);
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
    setState(343);
    type();
    setState(344);
    match(ZHLParser::LeftParen);
    setState(346);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ZHLParser::CallingConvention) {
      setState(345);
      match(ZHLParser::CallingConvention);
    }
    setState(348);
    match(ZHLParser::Name);
    setState(349);
    match(ZHLParser::Colon);
    setState(350);
    match(ZHLParser::Colon);
    setState(356);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ZHLParser::Name) {
      setState(351);
      match(ZHLParser::Name);
      setState(352);
      match(ZHLParser::Colon);
      setState(353);
      match(ZHLParser::Colon);
      setState(358);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(360); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(359);
      match(ZHLParser::Star);
      setState(362); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == ZHLParser::Star);
    setState(364);
    match(ZHLParser::Name);
    setState(365);
    match(ZHLParser::RightParen);
    setState(366);
    match(ZHLParser::LeftParen);
    setState(368);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 24) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 24)) & 4398046515197) != 0)) {
      setState(367);
      optNamedFunArgs();
    }
    setState(370);
    match(ZHLParser::RightParen);
    setState(371);
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
