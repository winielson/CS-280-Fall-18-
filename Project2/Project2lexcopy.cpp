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
		INIDENT,
		ININT,
		INSTRING,
		INCOM,
		INASSIGN,
		//INEQ,
		INNEQ,
		INLT,
		// INLEQ,
		INGT,
		// INGEQ,
		INLOGICAND,
		INLOGICOR,
		/*INLPAREN,
		INRPAREN,
		INSC,*/
		INDONE,
		INERR

	};

	LexState lexstate = BEGIN;
	string lexeme;
	char ch;
	// int i = 0;

	//Token Counters

	while (in->get(ch))
	{
		if (ch == '\n' && ch != '\r'){
			cout << "am 'newline' so linenum++\n";
			(*linenum)++;
		}
		switch (lexstate)
		{
		case BEGIN:
			if (isspace(ch))
				continue;

			lexeme = ch;
			//cout <<endl<<  "CH: " << ch << endl;
			if (isalpha(ch))
			{
				//cout << "entering INIDENT\n";
				lexstate = INIDENT;
			}
			else if (isdigit(ch))
			{
				lexstate = ININT;
			}
			else if (ch == '"')
			{
				lexstate = INSTRING;
			}
			else if (ch == '#')
			{
				lexstate = INCOM;
			}
			else if (ch == '+')
			{
				return Token(PLUS, lexeme, *linenum);
			}
			else if (ch == '-')
			{
				return Token(MINUS, lexeme, *linenum);
			}
			else if (ch == '*')
			{
				return Token(STAR, lexeme, *linenum);
			}
			else if (ch == '/')
			{
				return Token(SLASH, lexeme, *linenum);
			}
			else if (ch == '=')
			{
				lexstate = INASSIGN; //INCLUDE EQ
			}
			else if (ch == '!')
			{
				lexstate = INNEQ; //INCLUDE EQ
			}
			else if (ch == '<')
			{
				lexstate = INLT; //INCLUDE LEQ
			}
			else if (ch == '>')
			{
				lexstate = INGT; //INCLUDE GEQ
			}
			else if (ch == '&')
			{
				lexstate = INLOGICAND;
			}
			else if (ch == '|')
			{
				lexstate = INLOGICOR;
			}
			else if (ch == '(')
			{
				return Token(LPAREN, lexeme, *linenum);
			}
			else if (ch == ')')
			{
				return Token(RPAREN, lexeme, *linenum);

			}
			else if (ch == ';')
			{
				//cout << "SC ch: " << ch << endl;
				return Token(SC, lexeme, *linenum);
			}
			else if (ch == '.') //if number is decimal, but makes it so you cannot start a newline with a '.'
			{
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			/*else
			{
			return Token(DONE, lexeme, *linenum);
			//lexstate = INDONE;
			}*/
			break;

		case INIDENT:
			//KEYWORDS
			// cout << "IN INIDENT\n";
			if (lexeme == "hello6")
			{
				cout << "OUST INIDENT OPERATORS" << endl;
			}
			if (lexeme == "print" || lexeme == "if" || lexeme == "then" || lexeme == "true" || lexeme == "false")
			{
				if (lexeme == "print")
				{
					in->putback(ch);
					return Token(PRINT, lexeme, *linenum);
				}
				else if (lexeme == "if")
				{
					in->putback(ch);
					return Token(IF, lexeme, *linenum);
				}
				else if (lexeme == "then")
				{
					in->putback(ch);
					return Token(THEN, lexeme, *linenum);
				}
				else if (lexeme == "true")
				{
					in->putback(ch);
					return Token(TRUE, lexeme, *linenum);
				}
				else if (lexeme == "false")
				{
					// cout << "IN FALSE ch: " << ch << endl;
					in->putback(ch);
					return Token(FALSE, lexeme, *linenum);
				}
			}
			//if operators follow an ident, putback in stream and end IDENT instead of including operators to IDENT
			//else if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '!' || ch == '<' || ch == '>' || ch == '&' || ch == '|' || ch == '(' || ch == ')' || ch == ';' || ch == '"' || ch == ' ')
			else if (!isdigit(ch) && !isalpha(ch))
			{
				if (lexeme == "hello6")
				{
					cout << "in INIDENT OPERATORS" << endl;
					cout << "ch: '" << ch << "'" << endl;
				}
				//cout << "PUTTING BACK: '" << ch << "' because it is connected to an IDENT\n";

				// in->putback(ch);
				return Token(IDENT, lexeme, *linenum);
			}/*
			 else if (ch == '\n') //IDENT ends when space or newline is seen
			 {
			 //cout << "Lexeme at the end: " << lexeme << "." << endl;
			 //in->putback(ch);
			 if (lexeme == "hello6")
			 {
			 cout <<"in INIDENT NEWLINE" << endl;
			 }
			 return Token(IDENT, lexeme, *linenum);
			 }*/

			/*
			else if (isspace(ch)) //IDENT ends when space or newline is seen
			{
			//cout << "Lexeme at the end: " << lexeme << endl;
			in->putback(ch);
			return Token(IDENT, lexeme, *linenum);
			}
			else if (ch == '\n') //IDENT ends when space or newline is seen
			{
			//cout << "Lexeme at the end: " << lexeme << endl;
			//in->putback(ch);
			return Token(IDENT, lexeme, *linenum);
			}*/
			lexeme += ch;

			break;

		case ININT:
			if (isalpha(ch)) //if int is connected to letter
			{
				lexeme += ch;
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			else if (!isdigit(ch))
			{
				//cout << "Lexeme at the end: " << lexeme << endl;
				in->putback(ch); //puts nondigit back into in istream
				return Token(ICONST, lexeme, *linenum);
			}

			lexeme += ch;
			//return Token(ICONST, lexeme, *linenum);
			break;

		case INSTRING:

			if (ch == '"')
			{
				//cout << "Lexeme at the end: " << lexeme << endl;
				lexeme.erase(0, 1);
				return Token(SCONST, lexeme, *linenum);
			}

			lexeme += ch;
			// cout << "LINENUM CTR: "<< *linenum << endl;

			if (ch == '\n')
			{
				//cout << " IN STRING ERROR\n LEXEME: " << lexeme << endl;
				(*linenum)--; //ch is already read in while loop so decrease by one
				return Token(ERR, lexeme, *linenum);
			}
			break;

		case INCOM:

			if (ch == '\n') //When comment line ends, bring back to BEGIN
			{
				lexstate = BEGIN;
			}
			break;

		case INASSIGN:

			//i++;
			//cout << i << ": ch in INASSIGN: " << ch << endl;
			//lexeme += ch;
			//cout << i << ": lexeme in INASSIGN: " << lexeme << endl; 
			lexeme += ch; //peeks next char necessary to check if assign or eq
			if (lexeme == "= ")
			{
				in->putback(ch);
				//cout << "ASSIGN" << endl;
				return Token(ASSIGN, lexeme, *linenum);
			}
			else if (lexeme == "==")
			{
				//cout << "EQ" << endl;
				return Token(EQ, lexeme, *linenum);
			}
			else
			{
				in->putback(ch);
				return Token(ASSIGN, lexeme, *linenum);
			}

			/*
			//FOR ERR5
			else if(lexeme != "=\n")
			{
			in->putback(ch);
			return Token(ASSIGN, lexeme, *linenum);
			}
			else
			{
			in->putback(ch);
			return Token(ERR, lexeme, *linenum);
			}*/
			break;

		case INNEQ:
			//cout << i << ": ch in INASSIGN: " << ch << endl;
			//lexeme += ch;
			//cout << i << ": lexeme in INASSIGN: " << lexeme << endl; 
			lexeme += ch; //peeks next char necessary to check if neq    
			if (lexeme == "!=")
			{
				//in->putback(ch);
				//cout << "ASSIGN" << endl;
				return Token(NEQ, lexeme, *linenum);
			}
			else if (lexeme == "!\n")
			{
				//cout << "In INNEQ ERR. ch: " << ch <<endl;
				//lexeme -= ch;
				lexeme.pop_back(); //puts newline back so output is correct
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			else
			{
				//cout << "In INNEQ ERR. ch: " << ch <<endl;
				//lexeme -= ch;
				//(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}

			break;

		case INLT:

			lexeme += ch;  //peeks next char necessary to check if lt or leq
			//cout << "LEXEME IN INLT: " << lexeme <<endl;
			if (lexeme == "<=")
			{
				//cout << "EQ" << endl;
				return Token(LEQ, lexeme, *linenum);
			}
			else if (lexeme == "< ")
			{
				in->putback(ch);
				//cout << "ASSIGN" << endl;
				return Token(LT, lexeme, *linenum);
			}
			else
			{
				in->putback(ch);
				return Token(LT, lexeme, *linenum);
			}
			break;

		case INGT:

			lexeme += ch; //peeks next char necessary to check if gt or geq
			//cout << "LEXEME IN INLT: " << lexeme <<endl;
			if (lexeme == ">=")
			{
				//cout << "EQ" << endl;
				return Token(GEQ, lexeme, *linenum);
			}
			else if (lexeme == "> ")
			{
				in->putback(ch);
				//cout << "ASSIGN" << endl;
				return Token(GT, lexeme, *linenum);
			}
			else
			{
				in->putback(ch);
				return Token(GT, lexeme, *linenum);
			}
			break;

		case INLOGICAND:
			//cout << i << ": ch in INASSIGN: " << ch << endl;
			//lexeme += ch;
			//cout << i << ": lexeme in INASSIGN: " << lexeme << endl;
			//cout  << ": lexeme in INLOGICAND BEFORE: " << lexeme << endl; 
			lexeme += ch;
			//cout  << ": lexeme in INLOGICAND AFTER: " << lexeme << endl; 
			if (lexeme == "&&")
			{
				//in->putback(ch);

				//cout << "RETURNING LOGICAND" << endl;
				return Token(LOGICAND, lexeme, *linenum);
			}
			else if (lexeme == "&\n")
			{
				//cout << "In INNEQ ERR. ch: " << ch <<endl;
				//lexeme -= ch;
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			else
			{
				//cout << "---fdasfsdafasdf\n";
				in->putback(ch);
				//(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			break;

		case INLOGICOR:
			//in->get(ch);
			//cout << "lexeme in logicor b4: '" << lexeme << "'" << endl;
			lexeme += ch;
			//cout << "lexeme in logicor after: '" << lexeme << "'" << endl;
			if (lexeme == "||")
			{
				return Token(LOGICOR, lexeme, *linenum);
			}
			else if (lexeme == "|\n")
			{
				//cout << "In INNEQ ERR. ch: " << ch <<endl;
				//lexeme -= ch;
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			else
			{
				//cout << "---fdasfsdafasdf\n";
				in->putback(ch);
				//(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}
			break;

		case INDONE:
			return Token(DONE, lexeme, *linenum);
			break;

		case INERR:
			return Token(ERR, lexeme, *linenum);
			break;

		}
	}

	return Token(ERR, lexeme, *linenum);
}

//Overloaded operators for "-v" use
ostream& operator<<(ostream& out, const Token& tok)
{
	if (tok.GetTokenType() == IDENT)
	{
		out << "IDENT(" << tok.GetLexeme() << ")";
	}
	else if (tok.GetTokenType() == PRINT)
	{
		out << "PRINT";
	}
	else if (tok.GetTokenType() == IF)
	{
		out << "IF";
	}
	else if (tok.GetTokenType() == THEN)
	{
		out << "THEN";
	}
	else if (tok.GetTokenType() == TRUE)
	{
		out << "TRUE";
	}
	else if (tok.GetTokenType() == FALSE)
	{
		out << "FALSE";
	}
	else if (tok.GetTokenType() == ICONST)
	{
		out << "ICONST(" << tok.GetLexeme() << ")";
	}
	else if (tok.GetTokenType() == SCONST)
	{
		out << "SCONST(" << tok.GetLexeme() << ")";
	}
	else if (tok.GetTokenType() == PLUS)
	{
		out << "PLUS";
	}
	else if (tok.GetTokenType() == MINUS)
	{
		out << "MINUS";
	}
	else if (tok.GetTokenType() == STAR)
	{
		out << "STAR";
	}
	else if (tok.GetTokenType() == SLASH)
	{
		out << "SLASH";
	}
	else if (tok.GetTokenType() == ASSIGN)
	{
		out << "ASSIGN";
	}
	else if (tok.GetTokenType() == EQ)
	{
		out << "EQ";
	}
	else if (tok.GetTokenType() == NEQ)
	{
		out << "NEQ";
	}
	else if (tok.GetTokenType() == LT)
	{
		out << "LT";
	}
	else if (tok.GetTokenType() == LEQ)
	{
		out << "LEQ";
	}
	else if (tok.GetTokenType() == GT)
	{
		out << "GT";
	}
	else if (tok.GetTokenType() == GEQ)
	{
		out << "GEQ";
	}
	else if (tok.GetTokenType() == LOGICAND)
	{
		out << "LOGICAND";
	}
	else if (tok.GetTokenType() == LOGICOR)
	{
		out << "LOGICOR";
	}
	else if (tok.GetTokenType() == LPAREN)
	{
		out << "LPAREN";
	}
	else if (tok.GetTokenType() == RPAREN)
	{
		out << "RPAREN";
	}
	else if (tok.GetTokenType() == SC)
	{
		out << "SC";
	}
	else if (tok.GetTokenType() == ERR)
	{
		out << "Error on line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")";
	}

	/*
	else if (tok.GetTokenType() == PLUS, MINUS...
	{
	out <<
	}*/

	return out;
}