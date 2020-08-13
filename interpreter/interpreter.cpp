#include "interpreter.h"
#include <string>
#include <cstring>
#include <sstream>
#include <cctype> //included for isalpha() and isdigit()
#include <unordered_map>
#include <stack>
#include <exception>
#include <stdexcept>

using namespace std;

Interpreter::Interpreter(std::istream& in) {
    this->parse(in);
}

Interpreter::~Interpreter() {
	for (size_t i = 0; i < commandVector.size(); i++) {
		delete commandVector[i];
	}

	for (size_t i = 0; i < expressionVector.size(); i++) {
		delete expressionVector[i];
	}
}

void Interpreter::parse(std::istream& in) {
    string line;

    while (getline(in, line)) {
        size_t line_number;
        stringstream stream(line);
        stream >> line_number;
	
        lineVector.push_back(line_number);
        //above vector will help for creating map of commands for HW4

        //creating the correct kind of command object based on given command
        string command;
        stream >> command;

        if (command == "PRINT") {
        	NumericExpression* print_value;

        	//first have to identify what the first character is 
        	//based on the first character, we have 3 cases below
        	whitespace(stream);
        	char first_char;
        	first_char = stream.peek();

        	//case 1: printing a constant
        	if (isdigit(first_char) != 0 || first_char == '-') {
        		string const_value = parse_constant(stream);
        		print_value = new ConstValue(stoi(const_value));
        	}

        	//case 2: printing a variable
        	if (isalpha(first_char) != 0) {
        		Variable* var = parse_variable(first_char, stream);
        		print_value = var;
        	}

        	//case 3: printing a numeric expression
        	if (first_char == '(') {
        		print_value = parse_numeric_expression(stream);
        	}


        	Print* print_command = new Print(print_value);
        	commandVector.push_back(print_command);
        }
        else if (command == "LET") {
        	whitespace(stream);
        	Variable* store_in = parse_variable(' ',stream);

        	//NOTE ---- THIS MIGHT CAUSE SOME ERRORS BELOW ---- 
        	//whitespace(stream);
        	NumericExpression* new_value = parse_numeric_expression(stream);

        	Let* let_command = new Let(store_in, new_value);
        	commandVector.push_back(let_command);
        }
        else if (command == "GOTO") {
        	//only argument after will be a line number
        	int lineDestination = 0;
        	stream >> lineDestination;
        	LineNumber* destLine = new LineNumber(lineDestination);
        	Goto* goto_command = new Goto(destLine);
        	commandVector.push_back(goto_command);
        }
        else if (command == "IF") {
        	BooleanExpression* bool_exp = parse_bool_exp(stream);

        	//now looking for THEN
        	whitespace(stream);

        	//IF THEN CODE might not work here bc of this so check!!!
        	char t = stream.peek();
        	while (!isValid(t)) {
        		stream.ignore(1);
        		t = stream.peek();
        	}

        	stream.ignore(3);//3 bc a then has 4 characters

        	//now looking for line number
        	whitespace(stream);
        	string line_num = parse_constant(stream);
        	LineNumber* conditional = new LineNumber(stoi(line_num));

        	//creating the if command
        	If* if_command = new If(bool_exp, conditional);
        	commandVector.push_back(if_command);
        }
        else if (command == "GOSUB") {
        	//only argument after will be a line number
        	int lineDestination = 0;
        	stream >> lineDestination;
        	LineNumber* destLine = new LineNumber(lineDestination);
        	Gosub* gosub_command = new Gosub(destLine);
        	commandVector.push_back(gosub_command);
        }
        else if (command == "RETURN") {
        	Return* return_command = new Return();
        	commandVector.push_back(return_command);
        }
        else if (command == "END") {
        	End* end_command = new End();
        	commandVector.push_back(end_command);
        }
    }
}

bool Interpreter::isValid(char c) {
	if (c == ' ')
		return false;
	else if (c == 9)
		return false;
	else if (c == '+' || c == '-' || c == '*' || c == '/')
		return false;
	else if (c == EOF)
		return false;
	else if (c == ']')
		return false;
	else if (c == ')')
		return false;
	else if (c == 'T')
		return false;
	else if (c == '>' || c == '<' || c == '=')
		return false;

	return true;
}

string Interpreter::parse_constant(stringstream& ss) {
	whitespace(ss);
	char c = ss.peek();
	char temp;
	string value = "";

	if (c == '-') { //special case bc first character in a constant can be a '-'
		ss >> temp;
		value += temp;
		c = ss.peek();
	}

	if (c == ')') {
		ss.ignore(1);
		whitespace(ss);
		c = ss.peek();
	}

	while (isValid(c)) {
		whitespace(ss);
		ss >> temp;
		value += temp;
		c = ss.peek();
	}

	return value;
} 

Variable* Interpreter::parse_variable(char first, stringstream& ss) {
	Variable* var;

	//bool to keep track of whether or not variable type is array
	bool arrayVar = false;

	//retrivieng the name of the variable
	string varName = "";

	char temp = ss.peek();
	char add = ' ';
	while (temp != EOF && temp != ']' && temp != ')' && temp!='+' && temp!='-' && temp!='*' && temp!='/' && temp!=9 && (temp == '[' || isalpha(temp)!=0) && temp != ' ') {	
		if (temp == '[') {
			arrayVar = true;
			break;
		}
		ss >> add;
		varName += add;
		//whitespace(ss);// ----- REMOVED THIS TO PASS A TEST CASE ----- 
		temp = ss.peek();
	}

	//we create different variable objects if the variable is an array or a regular intVariable
	//if the variable is a regular intVariable
	if (!arrayVar) {
		var = new IntVariable(varName, varMap);
	}
	else if (arrayVar) { //variable is an array 
		//we know that the next character is a '[' so we want to ignore it
		ss.ignore(1);

		whitespace(ss);
		char index_char = ss.peek();

		//case1: array index is a constant
		if (isdigit(index_char) != 0 || index_char == '-') {
			string index_value = parse_constant(ss);
			NumericExpression* index = new ConstValue(stoi(index_value));
			var = new ArrayVariable(varName,index, arrMap);
		}

		//case2: array index is a variable
		if (isalpha(index_char) != 0) {
			Variable* varWithinVar = parse_variable(' ',ss);
			var = new ArrayVariable(varName, varWithinVar, arrMap);
		}

		//case3: array index is a numerical expression
		if (index_char == '(') {
			NumericExpression* numExpWithinVar = parse_numeric_expression(ss);
			var = new ArrayVariable(varName,numExpWithinVar, arrMap);
		}
	}

	return var;
}

NumericExpression* Interpreter::parse_numeric_expression(stringstream& ss) {
	NumericExpression* numExp;
	NumericExpression* left;
	NumericExpression* right;

	//we know first character here is a '(' so we can ignore it
	char t = ss.peek();
	while ((t == ']' || t ==')') && t != EOF) {
		ss.ignore(1);
		t = ss.peek();
	}

	char first_char = ss.peek();
	if (first_char == '('||first_char == ']') {
		ss.ignore(1);
	} 

	whitespace(ss);

	char first_in_numExp = ss.peek();

	//case 1: value is a number 
	if (isdigit(first_in_numExp) != 0 || first_in_numExp == '-') {
		int const_value_in_exp = stoi(parse_constant(ss));
		left = new ConstValue(const_value_in_exp);
	}

	//case 2: value is a variable
	if (isalpha(first_in_numExp)) {
		left = parse_variable(' ',ss); 
	}

	//case 3: value is another numeric expression
	if (first_in_numExp == '(') {
		left = parse_numeric_expression(ss);
	}

	//we want to skip any whitespace now so we can read the operator
	whitespace(ss);

	//reading in after LHS (we are expecting an operator here)
	char second_in_numExp = ' ';
	second_in_numExp = ss.peek();

	while (second_in_numExp == ')' || second_in_numExp == ']') {
		ss.ignore(1);
		whitespace(ss);
		second_in_numExp = ss.peek();
	}

	ss >> second_in_numExp;

	//now skip any whitespace until the RHS
	whitespace(ss);

	//reading in the RHS
	char third_in_numExp = ss.peek();

	//there are 3 cases with the RHS

	//case 1: RHS is a constant
	if (isdigit(third_in_numExp) != 0 || third_in_numExp == '-') {
		int const_value_in_exp = stoi(parse_constant(ss));
		right = new ConstValue(const_value_in_exp);
	}

	//case2: RHS is a variable
	if (isalpha(third_in_numExp) != 0) {
		right = parse_variable(' ', ss);
	}

	//case3: RHS is another numeric expression
	if (third_in_numExp == '(') {
		right = parse_numeric_expression(ss);
	}

	//now that we have RHS, LHS, and operator, we can create
	//the appropriate arithmetic expression type and return it
	if (second_in_numExp == '+') {
		numExp = new AdditionExpression(left,right);
	}
	else if (second_in_numExp == '-') {
		numExp = new SubtractionExpression(left,right);
	}
	else if (second_in_numExp == '*') {
		numExp = new MultiplicationExpression(left,right);
	}
	else if (second_in_numExp == '/') {
		numExp = new DivisionExpression(left,right);
	}
	else { //if it gets here, there is no operation being done here, just return the constant
		numExp = left;
	}

	return numExp;
} 

BooleanExpression* Interpreter::parse_bool_exp(stringstream& ss) {
	BooleanExpression* final_exp;
	NumericExpression* left;
	NumericExpression* right;
	string operator_value;
	whitespace(ss);
	char first_char = ss.peek();

	//case1: first expression is a constant value
	if (isdigit(first_char) != 0 || first_char == '-') {
		string const_value = parse_constant(ss);
		left = new ConstValue(stoi(const_value));
	}

	//case2: first expression is a variable
	if (isalpha(first_char) != 0 ) {
		left = parse_variable(' ', ss);
	}

	//case 3: first expression is a numeric expression
	if (first_char == '(') {
		left = parse_numeric_expression(ss);
	}

	//now that we have the LHS of the operation, we store the operation 
	char operation;
	whitespace(ss);
	operation = ss.peek();

	while (operation == ')' || operation == ']') {
		ss.ignore(1);
		whitespace(ss);
		ss >> operation;
	}

	//before we create the BoolExp* object, we have to read the RHS
	whitespace(ss);
	char second_char = ss.peek();
	if (second_char == '<' || second_char == '>' || second_char == '=') {
		ss.ignore(1);
		whitespace(ss);
		second_char = ss.peek();
	}

	//case1: second expression is a constant value
	if(isdigit(second_char) != 0 || second_char == '-') {
		string const_value = parse_constant(ss);
		right = new ConstValue(stoi(const_value));
	}

	//case2: second expression is a variable
	if (isalpha(second_char)) {
		right = parse_variable(' ', ss);
	}

	//case3: second expression is a numeric expression
	if (second_char == '(') {
		right = parse_numeric_expression(ss);
	}

	//now that we have stored both LHS and RHS we can create the appropriate BoolExp object

	//case1: less than
	if (operation == '<') {
		final_exp = new LessThan(left, right);
	}

	//case2: greater than
	if (operation == '>') {
		final_exp = new GreaterThan(left,right);
	}

	//case3: equal to
	if (operation == '=') {
		final_exp = new EqualTo(left, right);
	}

	return final_exp;
}

void Interpreter::write(std::ostream& out) {
	for (size_t i = 0; i < lineVector.size(); i++) {
		cout << lineVector[i];
		cout << commandVector[i]->format();
		cout << endl;
	} 
} 

void Interpreter::whitespace(stringstream& ss) {
	char c = ss.peek();
	while (c == ' ' || c == 9) {
		ss.ignore(1);
		c = ss.peek();
	}
}

void Interpreter::interpret(std::ostream& out) {
	//populating list of commands in map
	unordered_map <int, Command*> commandList;
	for (unsigned int i = 0; i < lineVector.size(); i++) {
		commandList[lineVector[i]] = commandVector[i];
	}

	//variables for handling line numbers
	int currLine = lineVector[0];//we know that we will always start at the very first line which is stored in lineVector[0]
	int index = 0;//this will hold the index in lineVector we are currently at, starting at 0
	stack<int> goSubLines; //stack for holding lines for GOSUB/RETURN commands

	bool not_at_end = true;
	while(not_at_end && index < (int) commandList.size()) {
		try {
			commandList[currLine]->evaluate(not_at_end,lineVector, currLine, index, goSubLines);
			//virtual function that will run differently based on type of command 


		} catch (logic_error& e) //error handling 
		{
			cout << "Error in line " << currLine << ": " << e.what() << "." << endl;
			not_at_end = false;
		}

	}
}
