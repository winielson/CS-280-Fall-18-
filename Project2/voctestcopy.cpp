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
				infile.open(argv[i + 1]);

				in = &infile;
				//cout << argv[i+1];


				while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
				{
					cout << tok;
				}


				//getNextToken(in, linenum);
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
		else if (argu.at(0) != '-') //read from the filename, otherwise read from standard input 
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

	return 0;
}

// Project2lex.h 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <cctype>
#include "tokens.h"

using std::string;
using std::istream;
using std::ostream;
using namespace std;

Token getNextToken(istream *in, int *linenum)
{
	enum LexState
	{
		BEGIN,
		INID,
		INSTRING,
		INDONE,
		INERR
	};

	LexState lexstate = BEGIN;
	string lexeme;
	char ch;
	// Token tok;

	while (in->get(ch))
	{
		if (ch == '\n')
			(*linenum)++;

		switch (lexstate)
		{
		case BEGIN:
			if (isspace(ch))
			{
				continue;
			}

			lexeme = ch;

			if (isalpha(ch))
			{
				lexstate = INID;
			}
			else if (ch == '"')
			{
				lexstate = INSTRING;
			}
			else
			{
				lexstate = INDONE;
			}

			break;

		case INID:
			cout << "IN INID\n";
			break;

		case INSTRING:
			while (ch != '"')
			{
				lexeme += ch;
				in->get(ch);
			}

			cout << "finished lexeme: " << lexeme << endl;
			//
			return Token(SCONST, lexeme, *linenum);
			lexstate = INDONE;
			break;

		case INDONE:
			return Token(DONE, lexeme, *linenum);
			break;

		case INERR:
			break;

		}
	}

	return Token();
}


ostream& operator<<(ostream& out, const Token& tok)
{
	out << tok.GetTokenType() << "(" << tok.GetLexeme() << ")";
	return out;
}

// Project2lex.h 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <cctype>
#include "tokens.h"

//using std::string;
//using std::istream;
//using std::ostream;
using namespace std;

Token getNextToken(istream *in, int *linenum)
{
	enum LexState
	{
		BEGIN,
		INID,
		INSTRING,
		INDONE,
		INERR
	};

	LexState lexstate = BEGIN;
	string lexeme;
	char ch;
	// Token tok;

	//!in->eof()
	while (in->get(ch))
	{
		if (ch == '\n')
			(*linenum)++;

		switch (lexstate)
		{
		case BEGIN:
			if (isspace(ch))
			{
				continue;
			}

			lexeme = ch;

			if (isalpha(ch))
			{
				lexstate = INID;
			}
			else if (ch == '"')
			{
				lexstate = INSTRING;
			}
			else
			{
				lexstate = INDONE;
			}

			break;

		case INID:
			cout << "IN INID\n";
			break;

		case INSTRING:
			while (!in->eof())
			{
				lexeme += ch;
				in->get(ch);
			}

			cout << "finished lexeme: " << lexeme << endl;

			return Token(SCONST, lexeme, *linenum);
			lexstate = INDONE;
			break;

		case INDONE:
			return Token(DONE, lexeme, *linenum);
			break;

		case INERR:
			break;

		default:
			return Token(DONE, lexeme, *linenum);
			break;

		}
	}

	return Token(DONE, lexeme, *linenum);
}






// Project2lex.h
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <cctype>
//#include <fstream>
#include "tokens.h"

using std::string;
using std::istream;
using std::ostream;

using namespace std;

Token getNextToken(istream *in, int *linenum)
{
enum LexState
{
BEGIN,
INID,
INSTRING,
INDONE,
INERR
};

LexState lexstate = BEGIN;
string lexeme;
char ch;
cout << "Linenum"; //<< linenum;
int num = *linenum;
// string type;

//Token tok;
//tp

cout << "IN getNextToken\n";

//!in->eof()
while (in->get(ch))
{
if (ch == '\n')
num++;
//(*linenum)++;


switch (lexstate)
{
case BEGIN:
if (isspace(ch))
{
continue;
}

//(*linenum)++;

lexeme = ch;

if (isalpha(ch))
{
lexstate = INID;
}
else if (ch == '"')
{
lexeme.erase(0,1);
lexstate = INSTRING;
}
else
{
return Token(DONE, lexeme, *linenum);
lexstate = INDONE;

}

break;

case INID:
cout << "IN INID\n";
return Token(DONE, lexeme, *linenum);
break;

case INSTRING:
//TokenType type = SCONST;

// lexeme += ch;
//in->get(ch);

while(ch != '"')
{
lexeme += ch;
in->get(ch);
cout << "Lexeme now: " << lexeme << endl;

//lexstate = INSTRING;
}

cout << "ch now: " << ch << endl;

if(ch == '"')
{
cout << "Attempting to return Token(SCONST, lexeme, *linenum)\n";
in->putback(ch);
cout << "linenum: " << num << endl;
cout << "ch now: " << ch << endl;

//lexstate = BEGIN;


return Token(SCONST, lexeme, num);
//return Token(SCONST, "Abc", *linenum);
//break;
}

/*
while (!in->eof())
{
lexeme += ch;
in->get(ch);
if(ch == '"')
{
lexstate = INDONE;
return Token(SCONST, lexeme, *linenum);
break;
}
}*/



//cout << "finished lexeme: " << lexeme << endl;
//type = "SCONST";

//return Token(type, lexeme, *linenum);

break;

		case INDONE:
			// TokenType type = DONE;
			return Token(DONE, lexeme, *linenum);
			break;

		case INERR:
			return Token(DONE, lexeme, *linenum);
			break;

		default:
			//  TokenType type = DONE;
			return Token(DONE, lexeme, *linenum);
			break;

		}
	}

	// TokenType type = DONE;
	return Token(DONE, lexeme, *linenum);
}


ostream& operator<<(ostream& out, const Token& tok)
{
	if (tok.GetTokenType() == SCONST)
	{
		out << "SCONST(" << tok.GetLexeme() << ")" << endl;
	}


	// out << "SCONST" << tok.GetTokenType()
	return out;
}

// Project2main.cpp 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/17/2018

#include <string>
#include <iostream>
#include <fstream>
//#include <istream>
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
				infile.open(argv[i + 1]);

				in = &infile;
				//cout << argv[i+1];

				cout << "OUTSIDE OF getNextToken\n";

				while ((tok = getNextToken(in, linenum)) != DONE && tok != ERR)
				{
					cout << "IN getNextToken\n";
					//cout << tok;
				}

				infile.close();

				//getNextToken(in, linenum);
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
		}/*
		 else if (argu.at(0) != '-') //read from the filename, otherwise read from standard input
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

		 infile.close();
		 }*/
	}

	return 0;
}

