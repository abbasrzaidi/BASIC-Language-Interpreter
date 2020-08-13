#include <iostream>
#include <fstream>
#include "interpreter/interpreter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide an input file." << endl;
        return 1;
    }

    ifstream input(argv[1]);
    Interpreter interpreter(input);
    //interpreter.write(cout);//this function prints everything out from HW2 --- parsing 
    interpreter.interpret(cout);//this function will print everything for HW4 --- evaluating 
    return 0;
}
