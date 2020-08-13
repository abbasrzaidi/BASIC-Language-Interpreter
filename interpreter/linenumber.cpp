#include "linenumber.h"

#include <string>

using namespace std;

LineNumber::LineNumber(int n)
{
	lineValue = n;
}

LineNumber::~LineNumber()
{
	//destructor
}

string LineNumber::format() const
{
	return "<" + to_string(lineValue) + "> ";
}

int LineNumber::getValue() const
{
	//return integer value 
	return lineValue;
}


