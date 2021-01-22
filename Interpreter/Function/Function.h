#pragma once
#include <string>


class Function
{
private:
	std::string variableName;
	std::string body;
public:
	Function();
	Function(std::string _variableName, std::string _body);
	~Function();
	Function& operator=(const Function& other);
	std::string getVariableName() const { return variableName; }
	std::string getBody() const { return body; }
};

