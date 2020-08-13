#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "arithmetic.h"
#include "linenumber.h"
#include "booleanexp.h"
#include <map>
#include <stack>
#include <vector>

class Command {
public:
	virtual ~Command() {};
    virtual std::string format() const = 0;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const = 0;
};

class Print : public Command {
public: 
	Print(NumericExpression* n);
	~Print();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;

private:
	NumericExpression* input;
};

class Let : public Command { 
public: 
	Let(Variable* v, NumericExpression* n);
	~Let();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;

private:
	Variable* valueset;
	NumericExpression* expression;
};

class Goto : public Command {
public: 
	Goto(LineNumber* l);
	~Goto();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;
    int find(std::vector<int>& lineVector, int x) const;

private:
	LineNumber* newLine;
};

class If : public Command {
public: 
	If(BooleanExpression* b, LineNumber* l);
	~If();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;
    int find(std::vector<int>& lineVector, int x) const;

private:
	BooleanExpression* condition;
	LineNumber* newLine;
};

class Gosub : public Command {
public: 
	Gosub(LineNumber* l);
	~Gosub();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;
    int find(std::vector<int>& lineVector, int x) const;

private:
	LineNumber* newLine;
};

class Return : public Command {  
public: 
	Return();
	~Return();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;
    int find(std::vector<int>& lineVector, int x) const;
};

class End : public Command {
public: 
	End();
	~End();
    virtual std::string format() const;
    virtual void evaluate(bool& end, std::vector<int>& lineVector, int& currLine, int& index, std::stack<int>& goSubLines) const;

private:
};

#endif
