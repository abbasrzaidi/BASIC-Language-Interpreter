#ifndef BOOLEANEXP_HPP
#define BOOLEANEXP_HPP

#include "arithmetic.h" //to use NumericExpression*
#include <string>

class BooleanExpression{
public:
	virtual ~BooleanExpression() {}
	virtual std::string format() const = 0;
	virtual bool getValue() const = 0;
};

class Comparisons : public BooleanExpression{
public:
	virtual ~Comparisons() {}
	virtual std::string format() const = 0;
	virtual bool getValue() const = 0;
};

class GreaterThan : public Comparisons{
public:
	GreaterThan(NumericExpression* left, NumericExpression* right);
	~GreaterThan();
	virtual std::string format() const;
	virtual bool getValue() const;
private:
	NumericExpression* left;
	NumericExpression* right;
};

class LessThan : public Comparisons{
public:
	LessThan(NumericExpression* left, NumericExpression* right);
	~LessThan();
	virtual std::string format() const;
	virtual bool getValue() const;
private:
	NumericExpression* left;
	NumericExpression* right;
};

class EqualTo : public Comparisons{
public:
	EqualTo(NumericExpression* left, NumericExpression* right);
	~EqualTo();
	virtual std::string format() const;
	virtual bool getValue() const;
private:
	NumericExpression* left;
	NumericExpression* right;
};

#endif