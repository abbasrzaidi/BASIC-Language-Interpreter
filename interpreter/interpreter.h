#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include "arithmetic.h"
#include "linenumber.h"
#include "command.h"
#include "booleanexp.h"
#include <vector>
#include <map>
#include <stack>

class Interpreter {
public:
    Interpreter(std::istream& in);
    ~Interpreter();
    void write(std::ostream& out);
    void interpret(std::ostream& out);//bulk of HW4 work will occur in this function
    Variable* parse_variable(char c, std::stringstream& ss);
    std::string parse_constant(std::stringstream& ss);
    NumericExpression* parse_numeric_expression(std::stringstream& ss);
    BooleanExpression* parse_bool_exp(std::stringstream& ss);
    void whitespace(std::stringstream& ss);//helper function for HW2

private:
    void parse(std::istream& in);
    std::vector <int> lineVector;
    std::vector <Command*> commandVector;
    std::vector <NumericExpression*> expressionVector;
    bool isValid(char c); //helper function for HW2
    std::map <std::string, int> varMap;
    std::map <std::string, std::map<int, int>> arrMap;
};

#endif
 