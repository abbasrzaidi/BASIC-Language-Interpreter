#include "arithmetic.h"
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

ConstValue::ConstValue(int n)
{
    value = n;
}

ConstValue::~ConstValue()
{
    //destructor
}

string ConstValue::format() const
{
    return to_string(value);
}

int ConstValue::getValue() const
{
    return value;
}

AdditionExpression::AdditionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

AdditionExpression::~AdditionExpression() {
    delete this->left;
    delete this->right;
}

string AdditionExpression::format() const {
    return "(" + this->left->format() + " + " + this->right->format() + ")";
}

int AdditionExpression::getValue() const
{
    //recursively evaluate addition expression
    return left->getValue() + right->getValue();
}

SubtractionExpression::SubtractionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

SubtractionExpression::~SubtractionExpression() {
    delete this->left;
    delete this->right;
}

string SubtractionExpression::format() const {
    return "(" + this->left->format() + " - " + this->right->format() + ")";
}

int SubtractionExpression::getValue() const
{
    //recursively evaluate subtraction expression
    return left->getValue() - right->getValue();
}

MultiplicationExpression::MultiplicationExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

MultiplicationExpression::~MultiplicationExpression() {
    delete this->left;
    delete this->right;
}

string MultiplicationExpression::format() const {
    return "(" + this->left->format() + " * " + this->right->format() + ")";
}

int MultiplicationExpression::getValue() const
{
    //recursively evaluate multiplication expression
    return left->getValue() * right->getValue();
}

DivisionExpression::DivisionExpression(NumericExpression* left, NumericExpression* right) : left(left), right(right) {

}

DivisionExpression::~DivisionExpression() {
    delete this->left;
    delete this->right;
}

string DivisionExpression::format() const {

    return "(" + this->left->format() + " / " + this->right->format() + ")";
}

int DivisionExpression::getValue() const
{
    //recursively evaluate division expression
    if (right->getValue() == 0)
    {
        //ERROR HANDLING: divide by 0 error
        string s = "Division by 0: " + left->format() + " = " + to_string(left->getValue()) + ", " + right->format() + " = " + to_string(right->getValue());
        throw logic_error(s);
    }

    return left->getValue() / right->getValue();
}

IntVariable::IntVariable(string n, map<string, int>& varMap) : varMap(varMap)
{
    name = n;
    value = 0;
    if (varMap.find(name) == varMap.end())
    {
        //if the integer variable being created does not already exist in the map, create an entry for it
        //initalize its value in the map to 0
        varMap.insert(make_pair(name,0));
    }
    //pass in the map in the constructor to initialize it and to avoid making copies
    
}

IntVariable::~IntVariable()
{
    
}

string IntVariable::getName() const
{
    return name;
}

string IntVariable::format() const
{
    return name;
}

void IntVariable::setValue(NumericExpression* v)
{
    value = v;
    varMap[this->name] = value->getValue();
    //access the appropriate entry in the map and set it the appropriate value
    //we don't have to check that the value exists in the map here because
    //when we were parsing, we created the intVariable object in the map
}

int IntVariable::getValue() const
{
    if (varMap.find(this->name) == varMap.end())
    {
        //if the variable does not exist in the map, return 0
        return 0;
    }

    return varMap.at(this->name);
}

ArrayVariable::ArrayVariable(string n, NumericExpression* i, map<string, map<int,int>>& arrMap) : arrMap(arrMap)
{
    name = n;
    index = i;
    if (arrMap.find(name) == arrMap.end())
    {
        //if the array variable does not exist in the map, create an entry for it 
        map <int, int> temp;
        arrMap.insert(make_pair(name, temp));
        (arrMap.at(name)).insert(make_pair(index->getValue(), 0));
        //default set the value to 0 at the specified index

    }
    else //array does exist in the map
    {
        if (arrMap.at(name).find(index->getValue()) == arrMap.at(name).end())
        {
            //if the specific index given does not exist in the map 
            (arrMap.at(this->name)).insert(make_pair(index->getValue(), 0));
            //default set the value to 0 at the specified index

        }
    }
}

ArrayVariable::~ArrayVariable()
{
    delete this->index;
}

string ArrayVariable::getName() const
{
    return name;
}

string ArrayVariable::format() const
{
    return name + "[" + index->format() + "]";
}

void ArrayVariable::setValue(NumericExpression* v) 
{
    this->value = v;
    arrMap.at(name).at(index->getValue()) = value->getValue();
    //set value for array at specified index to passed in parameter
}

int ArrayVariable::getValue() const
{
    if (arrMap.find(this->name) == arrMap.end())
    {
        //couldn't find array variable at all
        return 0;
    }
    else if ((arrMap.at(this->name).find(index->getValue()) == (arrMap.at(this->name).end())))
    {
        //found array variable, but no entry exists at specific index
        return 0;
    }
    return (arrMap.at(this->name)).at(index->getValue()); 
}

