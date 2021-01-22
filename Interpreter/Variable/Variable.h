#pragma once

class Variable
{
private:
	unsigned long value;

public:
	Variable();
	Variable(unsigned long _value);
	Variable(const Variable& other);
	~Variable();

	unsigned long getValue() const;
};
