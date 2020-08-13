#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <string>
#include <map>

#include <iostream>

class NumericExpression {
public:
    virtual ~NumericExpression() {}
    virtual std::string format() const = 0;
    virtual int getValue() const = 0;
    virtual void setValue(NumericExpression* v) = 0;
};

class ConstValue : public NumericExpression{
public: 
    ConstValue(int n);
    ~ConstValue();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v) {}
private:
    int value;
};

class AdditionExpression : public NumericExpression {
public:
    AdditionExpression(NumericExpression* left, NumericExpression* right);
    ~AdditionExpression();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v) {}

private:
    NumericExpression* left;
    NumericExpression* right;
};

class SubtractionExpression : public NumericExpression {
public:
    SubtractionExpression(NumericExpression* left, NumericExpression* right);
    ~SubtractionExpression();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v) {}

private:
    NumericExpression* left;
    NumericExpression* right;
};

class MultiplicationExpression : public NumericExpression {
public:
    MultiplicationExpression(NumericExpression* left, NumericExpression* right);
    ~MultiplicationExpression();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v) {}

private:
    NumericExpression* left;
    NumericExpression* right;
};

class DivisionExpression : public NumericExpression {
public:
    DivisionExpression(NumericExpression* left, NumericExpression* right);
    ~DivisionExpression();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v) {}

private:
    NumericExpression* left;
    NumericExpression* right;
};

class Variable : public NumericExpression {
public:
    virtual ~Variable(){}
    virtual std::string format() const = 0;
    virtual int getValue() const = 0;
    virtual void setValue(NumericExpression* v) = 0;//function added for HW4
    virtual std::string getName() const = 0;

    std::string name;
    NumericExpression* value;
};

class IntVariable : public Variable{
public:
    IntVariable(std::string n, std::map<std::string, int>& varMap);
    ~IntVariable();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v);
    virtual std::string getName() const;

private:
    std::map<std::string, int>& varMap;

};

class ArrayVariable : public Variable{
public:
    ArrayVariable(std::string n, NumericExpression* i, std::map<std::string, std::map<int, int>>& arrMap);
    ~ArrayVariable();
    virtual std::string format() const;
    virtual int getValue() const;
    virtual void setValue(NumericExpression* v);
    virtual std::string getName() const;
    

private:
    NumericExpression* index;
    Variable* variableforindex;
    std::map<std::string, std::map<int,int>>& arrMap;
};

#endif
