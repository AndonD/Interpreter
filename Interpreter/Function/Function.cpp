#include "Function.h"


Function::Function(){}

Function::Function(std::string _variableName, std::string _body)
									  :variableName(_variableName), body(_body){}

Function::~Function(){}

Function& Function::operator=(const Function& other)
{
	if (this != &other)
	{
		this->variableName = other.variableName;
		this->body = other.body;
	}
	return *this;
}
