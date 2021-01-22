#pragma once
#include "../InputReader/InputReader.h"
#include "../Token/Token.h"
#include <iostream>
#include <string>

bool isUpper(const std::string& str);
bool isLower(const std::string& str);
bool isOperator(char c);
bool isWhiteSpace(char c);

void syntaxErrorMessage(InputReader& input);

void clearWhiteSpaces(const std::string& str, size_t& index);
bool isValidExpression(const std::string& expr, size_t& index, int& countParenthesis);
bool isValidFunctionExpr(const std::string& expr, size_t& index);
Token getNextToken(const std::string& str, size_t& index);
unsigned long strToNum(const std::string& str);
unsigned long applyOperator(unsigned long arg1, unsigned long arg2, char op);