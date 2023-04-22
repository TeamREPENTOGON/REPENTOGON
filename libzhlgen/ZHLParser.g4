parser grammar ZHLParser;

options {
    tokenVocab = ZHLLexer;
}

zhl:
    (signature | class | genericCode)* EOF;
    
function:
    Qualifier* type Name Colon Colon Name LeftParen funArgs? RightParen Semi;
    
reference:
    Reference type Name Semi;
    
funArgs:
    funArg (Comma funArg)*;
    
funArg:
    Name Ptr* fullName argParam?;
    
argParam:
    Lt Register Rt;
    
type:
    Name Ptr*;
    
genericCode: 
    GenericCode; // LeftBracket LeftBracket Any* RightBracket RightBracket;
    
class: 
    (Class | Struct) Name depends? LeftBracket classBody RightBracket Semi;
    
depends:
    Depends LeftParen (Name Comma)* Name RightParen;

classBody: 
    (classSignature | genericCode | classField)*;

classSignature:
    Signature classFunction;

classFunction:
    Qualifier* type Name LeftParen funArgs RightParen Semi;
    
classField:
    type innerField (Comma innerField)* Semi;
    
innerField:
    Ptr* fullName;
    
fullName:
    Name (LeftRBracket Number RightRBracket)?;
    
signature:
    ((Signature function) | (ReferenceSignature reference));