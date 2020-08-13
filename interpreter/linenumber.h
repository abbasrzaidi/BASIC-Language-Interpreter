#ifndef LINENUMBER_HPP
#define LINENUMBER_HPP

#include <string>

class LineNumber{
public:
	LineNumber(int n);
	~LineNumber();
	std::string format() const;
	int getValue() const;

private:
	int lineValue;
};

#endif