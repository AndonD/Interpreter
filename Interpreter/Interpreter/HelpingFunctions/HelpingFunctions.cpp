#include "HelpingFunctions.h"

bool isUpper(const std::string& str)
{
	for (char c : str)
	{
		if (!isupper(c))
		{
			return false;
		}
	}
	return true;
}

bool isLower(const std::string& str)
{
	for (char c : str)
	{
		if (!islower(c))
		{
			return false;
		}
	}
	return true;
}

bool isOperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
	{
		return true;
	}
	return false;
}

bool isWhiteSpace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

void syntaxErrorMessage(InputReader& input)
{
	std::cout << "Syntax Error at line " << input.getNumberOfLine() << '.' << std::endl;
	throw std::exception("");
}

void clearWhiteSpaces(const std::string& str, size_t& index)
{
	for (; isWhiteSpace(str[index]); index++) {}
}

bool isValidExpression(const std::string& expr, size_t& index, int& countParenthesis)
{
	Token first = getNextToken(expr, index);

	if (first.type == Token::close_par)
	{
		countParenthesis--;
		if (index == expr.length())
		{
			return countParenthesis == 0;
		}
	}

	if (first.type == Token::open_par)
	{
		countParenthesis++;
		if (isValidExpression(expr, index, countParenthesis))
		{
			if (index == expr.length())	//if it is valid expression and there is nothing after the closing parenthesis
			{
				return true;
			}
		}
		else { return false; }	//If the expression in the parenthesis is not valid
	}
	else if ((first.type != Token::number && first.type != Token::variable && first.type != Token::funct))
	{
		return false;
	}

	if (first.type == Token::funct && !isValidFunctionExpr(expr, index)) { return false; }

	if (index == expr.length()) { return countParenthesis == 0; }

	Token second = getNextToken(expr, index);
	if (second.type == Token::close_par)
	{
		countParenthesis--;
		if (countParenthesis < 0) { return false; }
		if (index == expr.length()) { return true; }
		
		Token operatorToken = getNextToken(expr, index);
		if (operatorToken.type != Token::oper) { return false; }
	}
	else
	{
		if (index == expr.length()) { return false; }
		if (second.type != Token::oper) { return false; }
	}

	return true && isValidExpression(expr, index, countParenthesis);
}

bool isValidFunctionExpr(const std::string& expr, size_t& index)
{
	Token openBracket = getNextToken(expr, index);
	if (openBracket.type != Token::open_bracket) { return false; }

	Token parameter = getNextToken(expr, index);
	if (parameter.type != Token::number && parameter.type != Token::variable)
	{
		if (parameter.type == Token::funct && !isValidFunctionExpr(expr, index)) { return false; }
		return false;
	}

	Token closeBracket = getNextToken(expr, index);
	if (closeBracket.type != Token::close_bracket) { return false; }
	
	return true;
}

Token getNextToken(const std::string& str, size_t& index)
{
	clearWhiteSpaces(str, index);
	Token result{ Token::error, 0, "", ' ' };

	if (str[index] == '(')
	{
		result.type = Token::open_par;
		index++;
	}
	else if (str[index] == ')')
	{
		result.type = Token::close_par;
		index++;
	}
	else if (str[index] == '[')
	{
		result.type = Token::open_bracket;
		index++;
	}
	else if (str[index] == ']')
	{
		result.type = Token::close_bracket;
		index++;
	}
	else if ('0' <= str[index] && str[index] <= '9')		//If it is a number
	{
		std::string numValue;
		for (; '0' <= str[index] && str[index] <= '9'; ++index)
		{
			numValue += str[index];
		}
		result.type = Token::number;
		result.value = strToNum(numValue);
	}

	else if ('a' <= str[index] && str[index] <= 'z')	//If it is a word
	{
		std::string name;
		for (; 'a' <= str[index] && str[index] <= 'z'; ++index)
		{
			name += str[index];
		}
		if (name == "print")		//If it is the keyword "print"
		{
			result.type = Token::print;
		}
		else if (name == "read")	//If it is the keyword "read"
		{
			result.type = Token::read;
		}
		else                        //If it is a variable
		{
			result.type = Token::variable;
			result.name = name;
		}
	}

	else if ('A' <= str[index] && str[index] <= 'Z')		//If it is a function
	{
		std::string funcName;
		for (; 'A' <= str[index] && str[index] <= 'Z'; ++index)
		{
			funcName += str[index];
		}
		result.type = Token::funct;
		result.name = funcName;
	}

	else if (str[index] == '=')			//If it is operator =
	{
		result.type = Token::assignment;
		index++;
	}

	else if (isOperator(str[index]))	//If it is any other operator
	{
		result.type = Token::oper;
		result.symbol = str[index];
		index++;
	}

	return result;
}

unsigned long strToNum(const std::string& str)
{
	unsigned long result = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		result *= 10;
		result += (str[i] - '0');
	}

	return result;
}

unsigned long applyOperator(unsigned long arg1, unsigned long arg2, char op)
{
	switch (op)
	{
	case '+': return arg1 + arg2;
	case '-': return arg1 - arg2;
	case '/': return arg1 / arg2;
	case '*': return arg1 * arg2;
	case '%': return arg1 % arg2;
	default:
		break;
	}
	return 0;
}
