#include "Interpreter.h"
#include "../HelpingFunctions/HelpingFunctions.h"
#include <functional>


Interpreter::Interpreter(InputReader& _input) :input(_input)
{
	size_t size = 128;
	std::function<size_t(const std::string&)> hashFunc = [size](const std::string &str)->size_t
	{
		size_t sum = 0;
		for (char c : str)
		{
			sum += c;
		}
		return sum % size;
	};

	variables = HashMap<std::string, Variable>(size, hashFunc);
	functions = HashMap<std::string, Function>(size, hashFunc);
}

Interpreter::~Interpreter()
{
}

void Interpreter::start()
{
	validate();
	while (!input.isEndOfFile())
	{
		executeLine(input.getNextLine());
	}
}
void Interpreter::validate()
{
	while (!input.isEndOfFile())
	{
		validateLine(input.getNextLine());
	}
	input.returnToBegin();
}

void Interpreter::executeLine(const std::string& line)
{
	if (line.empty()) { return; }
	size_t ind = 0;
	Token first = getNextToken(line, ind);

	if (first.type == Token::funct)
	{
		Token openBracket = getNextToken(line, ind);
		Token functionVar = getNextToken(line, ind);
		Token closeBracket = getNextToken(line, ind);
		Token assignment = getNextToken(line, ind);

		std::string body = line.substr(ind);
		functions[first.name] = Function(functionVar.name, body);
	}
	else if (first.type == Token::read)
	{
		clearWhiteSpaces(line, ind);
		read(line, ind);
	}
	else if (first.type == Token::print)
	{
		clearWhiteSpaces(line, ind);
		std::string expr = line;
		unsigned long result = evaluateExpression(expr, ind);
		std::cout << result << std::endl;
	}
	else if (first.type == Token::variable)
	{
		Token next = getNextToken(line, ind);
		
		clearWhiteSpaces(line, ind);
		std::string expr = line;
		unsigned long value = evaluateExpression(expr, ind);
		variables[first.name] = Variable(value);
	}
	else
	{
		syntaxErrorMessage(input);
		return;
	}
}

void Interpreter::validateLine(const std::string& line)
{
	if (line.empty()) { return; }

	size_t ind = 0;
	int countParenthesis = 0;
	Token first = getNextToken(line, ind);

	if (first.type == Token::funct)
	{
		Token openBracket = getNextToken(line, ind);
		if (openBracket.type != Token::open_bracket) { syntaxErrorMessage(input); }

		Token functionVar = getNextToken(line, ind);
		if (functionVar.type != Token::variable) { syntaxErrorMessage(input); }

		Token closeBracket = getNextToken(line, ind);
		if (closeBracket.type != Token::close_bracket) { syntaxErrorMessage(input); }

		Token assignment = getNextToken(line, ind);
		if (assignment.type != Token::assignment) { syntaxErrorMessage(input); }

		if (!isValidExpression(line, ind, countParenthesis)) { syntaxErrorMessage(input); }
	}

	else if (first.type == Token::read)
	{
		Token variableToRead = getNextToken(line, ind);
		if (variableToRead.type != Token::variable) { syntaxErrorMessage(input); }
		if (ind != line.length()) { syntaxErrorMessage(input); }
	}

	else if (first.type == Token::print)
	{
		clearWhiteSpaces(line, ind);
		if (!isValidExpression(line, ind, countParenthesis)) { syntaxErrorMessage(input); }
	}

	else if (first.type == Token::variable)
	{
		Token assignment = getNextToken(line, ind);
		if (assignment.type != Token::assignment) { syntaxErrorMessage(input); }

		clearWhiteSpaces(line, ind);
		if (!isValidExpression(line, ind, countParenthesis)) { syntaxErrorMessage(input); }
	}

	else
	{
		syntaxErrorMessage(input);
	}
}

unsigned long Interpreter::evaluateExpression(std::string& expr, size_t& index)
{
	Token first = getNextToken(expr, index);

	size_t length = expr.length();
	std::string parenthesisExpr;
	if (first.type == Token::open_par)
	{
		size_t counter = 1;
		for (; index < length &&
			   ((expr[index] != '=' && !isWhiteSpace(expr[index]) && !isOperator(expr[index])) ||
			   (counter > 0)); index++)
		{
			if (expr[index] == '(')
			{
				counter++;
			}
			else if (expr[index] == ')')
			{
				counter--;
			}
			parenthesisExpr += expr[index];
		}
	}
	

	if (index == length)	//If the whole expr is just one token (it must be a number or variable or function)
	{
		size_t tempIndex = index - parenthesisExpr.length();
		if (first.type == Token::open_par)
		{
			return evaluateParenthesisExpr(expr, tempIndex);
		}
		else if (first.type == Token::number)
		{
			return first.value;
		}
		else if (first.type == Token::variable)
		{
			if (variables.hasKey(first.name))
			{
				return variables[first.name].getValue();
			}
			else
			{
				throw std::exception("Uninitialized value for this variable.");
			}
		}
		else if (first.type == Token::funct)
		{
			return evaluateFunctionCall(expr, tempIndex, first);
		}
	}

	unsigned long valueFirst;
	if (first.type == Token::open_par)
	{
		size_t tempIndex = index - parenthesisExpr.length();
		valueFirst = evaluateParenthesisExpr(expr, tempIndex);
	}
	else if (first.type == Token::number)
	{
		valueFirst = first.value;
	}
	else if (first.type == Token::variable)
	{
		if (variables.hasKey(first.name))
		{
			valueFirst = variables[first.name].getValue();
		}
		else
		{
			throw std::exception("Uninitialized value for this variable.");
		}
	}
	else if (first.type == Token::funct)
	{
		if (!functions.hasKey(first.name))
		{
			throw std::exception("Undefined function.");
		}
		valueFirst = evaluateFunctionCall(expr, index, first);
		if (index == expr.length()) { return valueFirst; }
	}

	Token oper = getNextToken(expr, index);		//oper for operator

	unsigned long result;
	if (oper.symbol == '+' || oper.symbol == '-')
	{
		expr.erase(expr.begin(), expr.begin() + index);
		index = 0;
		result = applyOperator(valueFirst, evaluateExpression(expr, index), oper.symbol);	//call the recursion for the sub-expression
		return result;
	}
	else if (oper.symbol == '*' || oper.symbol == '/' || oper.symbol == '%')
	{
		Token second = getNextToken(expr, index);
		unsigned long valueSecond;
		if (second.type == Token::open_par)
		{
			valueSecond = evaluateParenthesisExpr(expr, index);
		}
		else if (second.type == Token::number)
		{
			valueSecond = second.value;
		}
		else if (second.type == Token::variable)
		{
			if (variables.hasKey(second.name))
			{
				valueSecond = variables[second.name].getValue();
			}
			else
			{
				throw std::exception("Uninitialized value for this variable.");
			}
		}
		else if (second.type == Token::funct)
		{
			valueSecond = evaluateFunctionCall(expr, index, second);
		}

		result = applyOperator(valueFirst, valueSecond, oper.symbol);
		std::string newFirstToken = std::to_string(result);
		expr.replace(expr.begin(), expr.begin() + index, newFirstToken);

		index = 0;
		result = evaluateExpression(expr, index);
		return result;
	}
}

unsigned long Interpreter::evaluateParenthesisExpr(std::string& expr, size_t& index)
{
	std::string subExpression;
	size_t parenthesis = 1;
	for (; parenthesis != 0; index++)
	{
		if (expr[index] == '(')
		{
			parenthesis++;
		}
		else if (expr[index] == ')')
		{
			parenthesis--;
			continue;
		}
		subExpression += expr[index];
	}
	size_t beginningIndex = 0;
	return evaluateExpression(subExpression, beginningIndex);
}

unsigned long Interpreter::evaluateFunctionCall(std::string& expr, size_t& index, const Token& func)
{
	Token openBracket = getNextToken(expr, index);

	std::string parameter;
	for (; expr[index] != ']'; index++)
	{
		parameter += expr[index];
	}
	size_t tempIndex = 0;
	unsigned long parameterValue = evaluateExpression(parameter, tempIndex);

	Token closeBracket = getNextToken(expr, index);

	std::string funcBody = functions[func.name].getBody();
	
	variables[functions[func.name].getVariableName()] = parameterValue;	//put the parameter of the function in the variables database
	
	tempIndex = 0;
	unsigned long result = evaluateExpression(funcBody, tempIndex);
	variables.remove(functions[func.name].getVariableName());			//remove the parameter of the function from the variables database
	return result;
}


void Interpreter::read(const std::string& str, size_t& ind)
{	
	Token var = getNextToken(str, ind);

	unsigned long value;
	std::cin >> value;

	Variable variable(value);
	variables[var.name] = variable;
}
