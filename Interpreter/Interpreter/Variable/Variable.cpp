#include "Variable.h"

Variable::Variable(){}

Variable::Variable(unsigned long _value):value(_value){}

Variable::Variable(const Variable& other)
{
	this->value = other.value;
}

Variable::~Variable(){}

unsigned long Variable::getValue() const
{
	return value;
}
