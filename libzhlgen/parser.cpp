#include <iostream>

#include "ZHLLexer.h"
#include "ZHLParserBaseListener.h"

class Listener : public ZHLParserBaseListener {
public:
	/* virtual void enterTest(ZHLParser::TestContext* ctx) override {
		std::cout << "Entering test" << std::endl;
		std::cout << ctx->getText() << std::endl;
	}

	virtual void exitTest(ZHLParser::TestContext* ctx) override {
		std::cout << "Leaving test" << std::endl;
	} */

	virtual void enterZhl(ZHLParser::ZhlContext* ctx) override {
		// std::cout << ctx->toStringTree(true) << std::endl;
	}

	virtual void enterClass(ZHLParser::ClassContext* ctx) override {
		std::cout << "Entering class " << ctx->toStringTree(true) << std::endl;
	}

	virtual void enterSignature(ZHLParser::SignatureContext* ctx) override {
		std::cout << "Entering signature " << ctx->toStringTree(true) << std::endl;
	}

	virtual void enterGenericCode(ZHLParser::GenericCodeContext* ctx) override {
		std::cout << "Entering generic code " << ctx->GenericCode()->getText() << std::endl;
	}

	virtual void enterClassField(ZHLParser::ClassFieldContext* ctx) override {
		std::cout << "Entering class field " << ctx->toStringTree(true) << std::endl;
	}

	virtual void enterInnerField(ZHLParser::InnerFieldContext* ctx) override {
		std::cout << "Entering inner field " << ctx->toStringTree(true) << std::endl;
	}

	virtual void enterClassSignature(ZHLParser::ClassSignatureContext* ctx) override {
		std::cout << "Entering class signature " << ctx->toStringTree(true) << std::endl;
	}
};

int main(int argc, char** argv) {
	std::ifstream stream("dummy.zhl");

	if (!stream.is_open()) {
		return -1;
	}

	int a[12], b;

	antlr4::ANTLRInputStream input_stream(stream);
	ZHLLexer lexer(&input_stream);
	antlr4::CommonTokenStream tokens(&lexer);
	ZHLParser parser(&tokens);

	antlr4::tree::ParseTree* tree = parser.zhl();
	Listener listener;
	antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);

	return 0;
}
