
// Generated from ZHLParser.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"
#include "ZHLParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ZHLParser.
 */
class  ZHLParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ZHLParser.
   */
    virtual std::any visitZhl(ZHLParser::ZhlContext *context) = 0;

    virtual std::any visitFunction(ZHLParser::FunctionContext *context) = 0;

    virtual std::any visitReference(ZHLParser::ReferenceContext *context) = 0;

    virtual std::any visitFunArgs(ZHLParser::FunArgsContext *context) = 0;

    virtual std::any visitFunArg(ZHLParser::FunArgContext *context) = 0;

    virtual std::any visitArgParam(ZHLParser::ArgParamContext *context) = 0;

    virtual std::any visitType(ZHLParser::TypeContext *context) = 0;

    virtual std::any visitGenericCode(ZHLParser::GenericCodeContext *context) = 0;

    virtual std::any visitClass(ZHLParser::ClassContext *context) = 0;

    virtual std::any visitDepends(ZHLParser::DependsContext *context) = 0;

    virtual std::any visitClassBody(ZHLParser::ClassBodyContext *context) = 0;

    virtual std::any visitClassSignature(ZHLParser::ClassSignatureContext *context) = 0;

    virtual std::any visitClassFunction(ZHLParser::ClassFunctionContext *context) = 0;

    virtual std::any visitClassField(ZHLParser::ClassFieldContext *context) = 0;

    virtual std::any visitInnerField(ZHLParser::InnerFieldContext *context) = 0;

    virtual std::any visitFullName(ZHLParser::FullNameContext *context) = 0;

    virtual std::any visitSignature(ZHLParser::SignatureContext *context) = 0;


};

