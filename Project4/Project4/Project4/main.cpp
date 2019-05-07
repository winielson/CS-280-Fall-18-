/*
* main.cpp
*/

#include "tokens.h"
#include "parse.h"
#include "value.h"
#include <fstream>
#include <map>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

int main(int argc, char *argv[])
{
	ifstream file;
	istream *in;
	int linenum = 0;

	if (argc == 1) {
		in = &cin;
	}

	else if (argc == 2) {
		file.open(argv[1]);
		if (file.is_open() == false) {
			cout << "COULD NOT OPEN " << argv[1] << endl;
			return 1;
		}
		in = &file;
	}

	else {
		cout << "TOO MANY FILENAMES" << endl;
		return 1;
	}

	ParseTree *prog = Prog(in, &linenum); //Parses args

	if (prog == 0)
		return 0;

	map<string, Value> vmap; //Identifier map

	prog->Eval(vmap); //Interprets args

	return 0;
}