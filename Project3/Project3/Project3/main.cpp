//*
* main.cpp
* /

#include "tokens.h"
#include "parse.h"
#include "parsetree.h"
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

int main(int argc, char *argv[])
{
	ifstream file;
	istream *in = &cin;
	int linenum = 0;

	// CHECK ARGUMENTS, OPEN FILES
	for (int i = 1; i<argc; i++) {
		string arg(argv[i]);
		if (in != &cin) {
			cerr << "TOO MANY FILENAMES" << endl;
			return 0;
		}
		else {
			file.open(arg);
			if (file.is_open() == false) {
				cerr << "COULD NOT OPEN " << arg << endl;
				return 0;
			}
			in = &file;
		}
	}

	ParseTree *prog = Prog(in, &linenum);

	if (prog == 0)
		return 0; // quit on error

	//OUTPUT SECTION
	cout << "LEAF COUNT: " << prog->LeafCount() << "\n"; //
	cout << "STRING COUNT: " << prog->SConstCount() << "\n";
	if (prog->IDENTct != 0){
		prog->printIDENT();
	}

	return 0;
}