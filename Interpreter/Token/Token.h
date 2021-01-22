#pragma once
#include <string>

struct Token
{
public:
	enum Type { number, variable, funct, oper, open_par, close_par, open_bracket, close_bracket, print, read, assignment, error };
	
	Type type;
	unsigned long value;	//value if type is number
	std::string name;		//name if type is variable, funct
	char symbol;			//value if type is oper

	Token& operator=(const Token& other)
	{
		this->type = other.type;
		this->value = other.value;
		this->name = other.name;
		this->symbol = other.symbol;
		return *this;
	}
};