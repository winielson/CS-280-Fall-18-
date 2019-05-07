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

	//ARGUMENT ERROR CASES
	for (int i = 1; i < argc; i++) 
	{
		string argu = argv[i];

		for (int i = 1; i < argc; i++)
		{
			string argu = argv[i];
			if (argu.at(0) == '-')
			{
				if (argu != "-v" || argu != "-sum" || argu != "-allids")
				{
					cout << "INVALID FLAG " << argu << endl;
					return -1;
				}
			}
			else if (argu.at(0) != '-') //Checks for errors in file
			{
				int fileCount;

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

		for (int r = 1; r < argc; r++) //r starts at 1 not 0 bc argv[0] == progname
		{
			args.push_back(argv[r]); //Fills vector
			//cout << endl << args.front() << endl;
			// cout << endl << 
		}

		sum = find(args.begin(), args.end(), "-sum");
		allids = find(args.begin(), args.end(), "-allids");

		if (sum != args.end() && allids != args.end()) //Checks if both -sum and -allids are arguments.
		{
			{
				//RUNS SUM FIRST ALWAYS
				int ln = 1;
				int* linenum = &ln;
				//cout << "Argv[argc-1]: " << argv[argc-1] << endl;
				infile.open(argv[argc - 1]); //File is always last argv
				in = &infile;

				if (!infile.is_open())
				{
					cout << "UNABLE TO OPEN " << argv[argc - 1] << endl;
					return -1;
				}
				int tokens = 0;
				int id = 0;
				int str = 0;

				while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
				{
					tokens++;
					if (tok == IDENT)
					{
						id++;
					}
					else if (tok == SCONST)
					{
						str++;
					}
				}

				cout << "Total lines: " << *linenum << endl;
				cout << "Total tokens: " << tokens << endl;
				cout << "Total identifiers: " << id << endl;
				cout << "Total strings: " << str << endl;

				infile.close();
			}

			{
				//RUNS ALLIDS SECOND
				int ln2 = 1;
				int* linenum2 = &ln2;

				infile.open(argv[argc - 1]); //File is always last argv
				in = &infile;

				string ident;
				vector<string> ivec;
				vector<string>::iterator it;

				while ((tok = getNextToken(in, linenum2)) != DONE && tok != ERR)
				{
					if (tok.GetTokenType() == IDENT)
					{
						ident = tok.GetLexeme(); //store ident in vector
						ivec.push_back(ident);
					}
				}

				//algorithm library functions sort() and unique()
				sort(ivec.begin(), ivec.end()); //sorts alphabetically
				ivec.erase(unique(ivec.begin(), ivec.end()), ivec.end()); //erases duplicates

				cout << "IDENTIFIERS: ";

				int k = 0;
				for (it = ivec.begin(); it != ivec.end(); k++, it++)
				{
					cout << *it;
					if (k + 1 != ivec.size()) //So ", " wont get added to last ivec string
						cout << ", ";

				}
				cout << endl;

				infile.close();
			}
		}
		else
		{
			
			if (args.front() == "-v")
			{
				//call getNextToken print every token when it is seen
				int ln = 1;
				int* linenum = &ln;

				infile.open(argv[argc - 1]); //File is always last argv
				in = &infile;

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
				int ln = 1;
				int* linenum = &ln;
				//cout << "Argv[argc-1]: " << argv[argc-1] << endl;
				infile.open(argv[argc - 1]); //File is always last argv
				in = &infile;

				if (!infile.is_open())
				{
					cout << "UNABLE TO OPEN " << argv[argc - 1] << endl;
					return -1;
				}
				int tokens = 0;
				int id = 0;
				int str = 0;

				while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
				{
					tokens++;
					if (tok == IDENT)
					{
						id++;
					}
					else if (tok == SCONST)
					{
						str++;
					}
				}

				cout << "Total lines: " << *linenum << endl;
				cout << "Total tokens: " << tokens << endl;
				cout << "Total identifiers: " << id << endl;
				cout << "Total strings: " << str << endl;

				infile.close();
			}
			else if (args.front() == "-allids")
			{
				int ln = 1;
				int* linenum = &ln;

				infile.open(argv[argc - 1]); //File is always last argv
				in = &infile;

				string ident;
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

				cout << "IDENTIFIERS: ";

				int k = 0;
				for (it = ivec.begin(); it != ivec.end(); k++, it++)
				{
					cout << *it;
					if (k + 1 != ivec.size()) //So ", " wont get added to last ivec string
						cout << ", ";

				}
				cout << endl;

				infile.close();
			}
		}

	return 0;
}