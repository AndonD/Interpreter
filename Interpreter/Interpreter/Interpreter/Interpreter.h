#pragma once
#include "../InputReader/InputReader.h"
#include "../Variable/Variable.h"
#include "../Function/Function.h"
#include "../HashMap/HashMap.h"
#include <string>

class Interpreter
{
private:
	InputReader input;
	HashMap<std::string, Variable> variables;
	HashMap<std::string, Function> functions;

public:
	Interpreter(InputReader& _input);
	~Interpreter();

	void start();
	void validate();

	void executeLine(const std::string& line);
	void validateLine(const std::string& line);

	unsigned long evaluateExpression(std::string& expr, size_t& index);
	unsigned long evaluateParenthesisExpr(std::string& expr, size_t& index);
	unsigned long evaluateFunctionCall(std::string& expr, size_t& index, const Token& func);
	void read(const std::string& str, size_t& ind);
};

