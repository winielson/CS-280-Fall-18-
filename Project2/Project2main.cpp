// Project2main.cpp 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "tokens.h"
#include "Project2lex.h"

using std::string;
using std::istream;
using std::ostream;
using namespace std;

int main(int argc, char** argv)
{
	string argument;
	istream *in;
	ifstream infile;
	Token tok;

	//COMMAND LINE ARGUMENT ERROR CASES
	for (int i = 1; i < argc; i++)
	{
		string argu = argv[i];
		if (argu.at(0) == '-')
		{
			if (argu != "-v" && argu != "-sum" && argu != "-allids")
			{
				cout << "INVALID FLAG " << argu << endl;
				return -1;
			}
		}
		else if (argu.at(0) != '-') //Checks for errors in file
		{
			int fileCount = 0;

			ifstream infile; //Declares input file
			infile.open(argv[i]); //Opens file 
			fileCount++;

			//ERROR CASES
			if (fileCount > 1)
			{
				cout << "TOO MANY FILE NAMES\n";
				return -1;

			}
			else if (infile.fail())
			{
				cout << "UNABLE TO OPEN " << argv[i] << endl;
				return -1;
			}
			else if (infile.peek() == std::ifstream::traits_type::eof() && i == argc - 1) //If file has no contents (NOTE: i == argc-1 for TOO MANY FILES case)
			{
				// cout << "NO PHRASES\n";
				return -1;
			}
		}
	}

	//PROCESSING AND OUTPUT SECTION
	vector<string> args;
	vector<string>::iterator argit;
	vector<string>::iterator sum;
	vector<string>::iterator allids;

	if (argv[0] != NULL)
	{
		for (int r = 1; (r < argc); r++) //r starts at 1 not 0 bc argv[0] == progname
		{
			args.push_back(argv[r]); //Fills vector
		}
	}

	//Checks if both -sum and -allids are arguments. 
	sum = find(args.begin(), args.end(), "-sum");
	allids = find(args.begin(), args.end(), "-allids");

	if (sum != args.end() && allids != args.end()) //Multiple options loop. if both -sum and -allids are arguments, run -sum first
	{
		int L = 1;
		int* linenum = &L;

		infile.open(argv[argc - 1]); //File is always last argv
		in = &infile;

		if (!infile.is_open()) //Read from standard input
		{
			in = &cin;
		}

		int N = 0;
		int I = 0;
		int X = 0;

		string ident;
		string allident;
		vector<string> ivec;
		vector<string>::iterator it;

		while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR) //Combines -allids and -sum loop together
		{
			if (tok == IDENT)
			{
				ident = tok.GetLexeme();
				ivec.push_back(ident); //store ident in vector
				I++;
			}
			else if (tok == SCONST)
			{
				X++;
			}
			N++;
		}

		//algorithm library functions sort() and unique()
		sort(ivec.begin(), ivec.end()); //sorts alphabetically
		ivec.erase(unique(ivec.begin(), ivec.end()), ivec.end()); //erases duplicates

		int k = 0;
		for (it = ivec.begin(); it != ivec.end(); k++, it++)
		{
			allident += *it;
			if (k + 1 != ivec.size()) //So ", " wont get added to last ivec string
				allident += ", ";
		}

		cout << "IDENTIFIERS: " << allident << endl;
		cout << "Total lines: " << L << endl;
		cout << "Total tokens: " << N << endl;
		cout << "Total identifiers: " << I << endl;
		cout << "Total strings: " << X << endl;

		infile.close();
		return -1;
	}
	else if (args.size() != 0) //Single option is passed
	{
		if (args.front() == "-v")
		{
			//call getNextToken print every token when it is seen
			int L = 1;
			int* linenum = &L;

			infile.open(argv[argc - 1]); //File is always last argv
			in = &infile;

			if (!infile.is_open()) //Read from standard input
			{
				in = &cin;
			}

			while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
			{
				cout << tok << endl;
			}

			//Separated from while loop to correctly display error message
			if (tok == ERR)
			{
				cout << tok << endl;
			}

			infile.close();
		}
		else if (args.front() == "-sum")
		{
			int L = 1;
			int* linenum = &L;
			//cout << "Argv[argc-1]: " << argv[argc-1] << endl;
			infile.open(argv[argc - 1]); //File is always last argv
			in = &infile;

			if (!infile.is_open()) //Read from standard input
			{
				in = &cin;
			}

			int N = 0;
			int I = 0;
			int X = 0;

			while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
			{

				if (tok == IDENT)
				{
					I++;
				}
				else if (tok == SCONST)
				{
					X++;
				}
				N++;
			}

			cout << "Total lines: " << L << endl;
			cout << "Total tokens: " << N << endl;
			cout << "Total identifiers: " << I << endl;
			cout << "Total strings: " << X << endl;

			infile.close();
		}
		else if (args.front() == "-allids")
		{
			int L = 1;
			int* linenum = &L;

			infile.open(argv[argc - 1]); //File is always last argv
			in = &infile;

			if (!infile.is_open()) //Read from standard input
			{
				in = &cin;
			}

			string ident;
			string allident;
			vector<string> ivec;
			vector<string>::iterator it;

			while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
			{
				if (tok.GetTokenType() == IDENT)
				{
					ident = tok.GetLexeme(); //store ident in vector
					ivec.push_back(ident);
				}
			}

			sort(ivec.begin(), ivec.end()); //algorithm library function: sorts alphabetically 
			ivec.erase(unique(ivec.begin(), ivec.end()), ivec.end()); //algorithm library function: erases duplicates

			int k = 0;
			for (it = ivec.begin(); it != ivec.end(); k++, it++)
			{
				allident += *it;
				if (k + 1 != ivec.size()) //So ", " wont get added to last ivec string
					allident += ", ";
			}
			cout << "IDENTIFIERS: " << allident << endl;
			infile.close();
		}
	}

	return 0;
}