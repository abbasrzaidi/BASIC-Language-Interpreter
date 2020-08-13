#include "command.h"

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

Print::Print(NumericExpression* n)
{
	input = n;
}

Print::~Print()
{
	delete this->input;
}

string Print::format() const
{
	return " PRINT " + input->format();
}

void Print::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	cout << input->getValue() << endl;
	index++;
	if (index < (int) lineVector.size() && index >= 0)//check to make sure index is in range
	{
		currLine = lineVector[index];
	}
	else
	{
		end = false;
	}
}

Let::Let(Variable* v, NumericExpression* n)
{
	valueset = v;
	expression = n;
}

Let::~Let()
{
	delete this->valueset;
	delete this->expression;
}

string Let::format() const
{
	return " LET " + this->valueset->format() + " " + this->expression->format();
}

void Let::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	valueset->setValue(expression);
	index++;
	if (index < (int) lineVector.size() && index >= 0)//checking that index is in range
	{
		currLine = lineVector[index]; //sets currLine which will be used in interpreter.cpp to approrpriate line number 
	}
	else
	{
		end = false;
	}
}

Goto::Goto(LineNumber* l)
{
	newLine = l;
}

Goto::~Goto()
{
	delete this->newLine;
}

string Goto::format() const
{
	return " GOTO " + this->newLine->format();
}

void Goto::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	index = this->find(lineVector,newLine->getValue());

	//ERROR HANDLING: if line jumping to does not exist
	if (index == -1)
	{
		string s = "GOTO to non-existent line " + to_string(newLine->getValue());
		throw logic_error(s);
	}

	currLine = lineVector[index];

}

int Goto::find(vector<int>& lineVector, int x) const //helper function to find appropriate index of line being jumped to 
{
	for (unsigned int i = 0; i < lineVector.size(); i++)
	{
		if (lineVector[i] == x)
		{
			return i;
		}
	}

	return -1;
}

If::If(BooleanExpression* b, LineNumber* l)
{
	condition = b;
	newLine = l;
}

If::~If()
{
	delete this->condition;
	delete this->newLine;
}

string If::format() const
{
	return " IF " + this->condition->format() + " THEN " + this->newLine->format();
}

void If::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	if (condition->getValue())//if the boolexpression is true
	{
		index = this->find(lineVector, newLine->getValue());

		//ERROR HANDLING: if line jumping to does not exist 
		//note: we only want to check this error if the boolean expression is true 
		if (index == -1)
		{
			string s = "IF jump to non-existent line " + to_string(newLine->getValue());
			throw logic_error(s);
		}
		currLine = lineVector[index];
	}
	else //if the boolean expression is false
	{
		index++;
		if (index < (int) lineVector.size() && index >= 0) //checks that index is in acceptable range 
		{
			currLine = lineVector[index];
		}
		else
		{
			end = false;
		}
	}
}

int If::find(vector<int>& lineVector, int x) const //helper function to find appropriate index of line being jumped to 
{
	for (unsigned int i = 0; i < lineVector.size(); i++)
	{
		if (lineVector[i] == x)
		{
			return i;
		}
	}

	return -1;
}

Gosub::Gosub(LineNumber* l)
{
	newLine = l;
}

Gosub::~Gosub()
{
	delete this->newLine;
}

string Gosub::format() const
{
	return " GOSUB " + this->newLine->format();
}

void Gosub::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	if (index + 1 < (int) lineVector.size()) //make sure that there is another line in the lineVector (not going out of bounds)
	{
		goSubLines.push(lineVector[index+1]);
	}

	index = this->find(lineVector, newLine->getValue());

	//ERROR HANDLING: jumping to non existent line
	if (index == -1)
	{
		string s = "GOSUB to non-existent line " + to_string(newLine->getValue());
		throw logic_error(s);
	}
	currLine = lineVector[index];
}

int Gosub::find(vector<int>& lineVector, int x) const //helper function to find appropriate index of line being jumped to 
{
	for (unsigned int i = 0; i < lineVector.size(); i++)
	{
		if (lineVector[i] == x)
		{
			return i;
		}
	}

	return -1;
}

Return::Return()
{
	//empty constructor
}

Return::~Return()
{
	//destructor
}

string Return::format() const
{
	return " RETURN ";
}

void Return::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	if (goSubLines.empty())
	{
		//ERROR HANDLING: no matching GOSUB call for RETURN call
		string s = "No matching GOSUB for RETURN";
		throw logic_error(s);
	}
	else
	{
		index = this->find(lineVector, goSubLines.top());
		goSubLines.pop();//we want to remove the line number from the stack since we are already jumping to it 
		currLine = lineVector[index];
	}
}

int Return::find(vector<int>& lineVector, int x) const
{
	for (unsigned int i = 0; i < lineVector.size(); i++)
	{
		if (lineVector[i] == x)
		{
			return i;
		}
	}

	return 0;
}

End::End()
{
	//empty constructor
}

End::~End()
{
	//destructor
}

string End::format() const
{
	return " END ";
}

void End::evaluate(bool& end, vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const
{
	//all we want to do here is set the boolean to false which will end the while loop that handles execution 
	end = false;
}

