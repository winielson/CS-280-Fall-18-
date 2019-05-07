// Project2main.cpp 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <fstream>
#include <istream>
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
    ifstream infile; //Declares input file
    Token tok;

	for (int i = 1; i < argc; i++)
	{
        string argu = argv[i];

		if (argu.at(0) == '-') //Checks if first char of string is a dash (flag)
		{
			if (argu == "-v")
			{
				//call getNextToken print every token when it is seen
                int* linenum = 0;
                //ifstream infile; //Declares input file
			    infile.open(argv[i+1]);
                
                in = &infile;
                //cout << argv[i+1];
                
                /*
                while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
                {
                    
                }*/
                
                
                getNextToken(in, linenum);
                //cout << "TT: "<< Token.GetTokenType() << endl; //GetLexeme() GetLinenum()
                return 0;
			}
			else if (argu == "-sum")
			{
				//call getNextToken print summary information 
			}
			else if (argu == "-allids")
			{
				//call getNextToken print list of the lexemes for all identifiers in alphabetical order
			}
			else
			{
				cout << "INVALID FLAG " << argu << endl;
                return -1;
			}
		}
		else if(argu.at(0) != '-') //read from the filename, otherwise read from standard input 
		{
            int fileCount;
			  
			
			infile.open(argv[i]); //Opens file 
            fileCount++;

			//ERROR CASES
            if (fileCount > 1)
            {
                cout << "TOO MANY FILE NAMES\n";
                return -1;
                
            }
            else if(infile.fail())
            {
                cout << "UNABLE TO OPEN " << argv[i] << endl;
                return -1;
            }
            else if (infile.peek() == std::ifstream::traits_type::eof() && i == argc-1) //If file has no contents (NOTE: i == argc-1 for TOO MANY FILES case)
            {
               // cout << "NO PHRASES\n";
                return -1;
            }
		}      
	}

    return 0;
}

