#include "booleanexp.h"

using namespace std;

GreaterThan::GreaterThan(NumericExpression* left, NumericExpression* right) : left(left), right(right){
}

GreaterThan::~GreaterThan()
{
	delete this->left;
	delete this->right;
}

string GreaterThan::format() const
{
	return "[" + this->right->format() + " < " + this->left->format() + "]";
	//formatted as such in accordance with assignment write up
}

bool GreaterThan::getValue() const
{
	//evaluate greater than expression
	if (left->getValue() > right->getValue())
	{
		return true;
	}

	return false;
}

LessThan::LessThan(NumericExpression* left, NumericExpression* right) : left(left), right(right){
}

LessThan::~LessThan()
{
	delete this->left;
	delete this->right;
}

string LessThan::format() const
{
	return "[" + this->left->format() + " < " + this->right->format() +"]";
}

bool LessThan::getValue() const
{
	//evaluate less than expression
	if (left->getValue() < right->getValue())
	{
		return true;
	}

	return false;
}

EqualTo::EqualTo(NumericExpression* left, NumericExpression* right) : left(left), right(right){
}

EqualTo::~EqualTo()
{
	delete this->left;
	delete this->right;
}

string EqualTo::format() const
{
	return "[" + this->left->format() + " = " + this->right->format() + "]";
}

bool EqualTo::getValue() const
{
	//evaluate equal to expression
	if (left->getValue() == right->getValue())
	{
		return true;
	}

	return false;
}
