parser grammar ZHLParser;

options {
    tokenVocab = ZHLLexer;
}

zhl:
    (signature | class | genericCode | typedef | functionPtr | forwardDecl | typeInfo | externalFunc)* EOF;

externalFunc:
    External LeftParen Name Comma Name RightParen function;

typeInfo:
    Type Name LeftBracket typeInfoDef+ RightBracket Semi;

typeInfoDef:
    Size Number Semi |
    Synonym type Semi;
    
function:
    Qualifier* CallingConvention? type nestedName LeftParen funArgs? RightParen Semi;
    
reference:
    Reference type Name Semi;
    
funArgs:
    funArg (Comma funArg)*;
    
funArg:
    type fullName argParam?;
    
argParam:
    Lt Register Gt;
    
optNamedFunArgs: 
    optNamedFunArg (Comma optNamedFunArg)*;
    
optNamedFunArg:
    type fullName?;
    
templateSpec:
    Lt type (Comma type)* Gt;
    
type:
    typeSpecifier templateSpec? pointerAttribute*;
    
typeSpecifier:
    Const? (simpleType | nestedName) Const?;
    
simpleType:
    simpleTypeSignedness
    | simpleTypeSignedness? simpleTypeLength+
    | simpleTypeSignedness? Char
    | Bool
    | Void
    | simpleTypeSignedness? simpleTypeLength* Int
    | Float
    | simpleTypeLength? Double
    ;
    
simpleTypeSignedness:
    Signed | Unsigned;
    
simpleTypeLength:
    Long | Short;
    
nestedName:
    Name (Colon Colon Name)*;
    
pointerAttribute:
    CppRef CppRef? |
    Star Const?;
    
genericCode: 
    GenericCode; // LeftBracket LeftBracket Any* RightBracket RightBracket;
    
class: 
    (Class | Struct) Name depends? (Colon inheritance)? LeftBracket classBody RightBracket Semi;
    
inheritance:
    inheritanceDecl (Comma inheritanceDecl)*;

inheritanceDecl:
    Visibility? nestedName;
    
depends:
    Depends LeftParen (Name Comma)* Name RightParen;

classBody: 
    (vtable | classSignature | genericCode | classField | functionPtr | forwardDecl)*;

vtable:
    Vtable LeftBracket vtableEntry+ RightBracket Semi;

vtableEntry:
    vtableSignature | Skip;

vtableSignature:
    Override? classSignature;

classSignature:
    Signature classFunction;

classFunction:
    Qualifier* type Name LeftParen funArgs? RightParen Semi;
    
classField:
    type innerField (Comma innerField)* Semi;
    
innerField:
    innerFieldPtr* fullName;

innerFieldPtr:
    Star Const?;
    
fullName:
    Name (LeftRBracket Number RightRBracket)?;
    
signature:
    ((Signature function) | (ReferenceSignature reference));
    
forwardDecl:
    (Struct | Class) Name Semi;
    
typedef:
    Typedef type Name Semi;
    
functionPtr:
    fptr | memberPtr;
    
fptr:
    type LeftParen CallingConvention? Star+ Name RightParen LeftParen optNamedFunArgs? RightParen Semi;
    
memberPtr:
    type LeftParen CallingConvention? Name Colon Colon (Name Colon Colon)* Star+ Name RightParen LeftParen optNamedFunArgs? RightParen Semi;
