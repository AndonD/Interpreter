#include "InputReader.h"
#include "../HelpingFunctions/HelpingFunctions.h"


InputReader::InputReader(const std::string& fileName) :inputFileName(fileName), positionInFile(0),
												numberOfLine(0), endOfFile(false) {}

InputReader::~InputReader()
{
}

std::string InputReader::getNextLine()
{
	std::string line;
	std::ifstream input(inputFileName);
	if (!input.is_open())
	{
		throw std::exception("Cannot open file!\n");
	}
	else
	{
		input.seekg(positionInFile);
		std::getline(input, line);
		numberOfLine++;
		if (input.eof())			//if there is no such if-statement, input.tellg() returns garbage value
		{
			endOfFile = true;
		}
		positionInFile = (size_t)input.tellg();
	}
	input.close();
	return line;
}

void InputReader::returnToBegin()
{
	positionInFile = 0;
	numberOfLine = 0;
	endOfFile = false;
}
