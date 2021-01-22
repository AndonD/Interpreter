#pragma once
#include "../Token/Token.h"
#include <iostream>
#include <fstream>
#include <string>

class InputReader
{
private:
	std::string inputFileName;
	size_t positionInFile;
	size_t numberOfLine;
	bool endOfFile;

public:
	InputReader(const std::string& fileName);
	~InputReader();

public:
	std::string getNextLine();
	bool isEndOfFile() const { return endOfFile; }
	size_t getNumberOfLine() const { return numberOfLine; }
	void returnToBegin();
};

