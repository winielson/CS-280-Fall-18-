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
		INICONST,
		INSCONST,
		INCOM,
		INASSIGN,
		INNEQ,
		INLT,
		INGT,
		INLOGICAND,
		INLOGICOR,
	};

	LexState lexstate = BEGIN;
	string lexeme;
	char ch;

	while (in->get(ch))
	{
		switch (lexstate)
		{
		case BEGIN:
			//Separated isspace(ch) to fix newline problem took 10hrs to debug
			if (ch == ' ')
			{
				continue;
			}
			else if (ch == '\n')
			{
				(*linenum)++;
				continue;
			}

			lexeme = ch;

			if (isalpha(ch))
			{
				lexstate = INIDENT; //Includes Keywords
			}
			else if (isdigit(ch))
			{
				lexstate = INICONST;
			}
			else if (ch == '"')
			{
				lexstate = INSCONST;
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
				lexstate = INNEQ;
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
				return Token(SC, lexeme, *linenum);
			}
			else if (ch == '.') //ERR: if number is decimal, but makes it so you cannot start a newline with a '.'
			{
				(*linenum)--;
				return Token(ERR, lexeme, *linenum);
			}

			break;

		case INIDENT:
			if (isdigit(ch) || isalpha(ch)) //IDENT is letter followed by a series of words or letters
			{
				lexeme += ch;
			}
			//KEYWORDS
			else if (lexeme == "print")
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
				in->putback(ch);
				return Token(FALSE, lexeme, *linenum);
			}
			else if (!isdigit(ch) && !isalpha(ch) && ch != '\n' && ch != ' ') //if operators follow an ident, putback in stream and end IDENT instead of including operators to IDENT
			{
				in->putback(ch);
				return Token(IDENT, lexeme, *linenum);
			}
			else if (isspace(ch)) //IDENT ends when space or newline is seen
			{
				in->putback(ch);
				return Token(IDENT, lexeme, *linenum);
			}

			break;

		case INICONST:
			if (isalpha(ch)) //ERR: if letter is connected to int
			{
				lexeme += ch;
				(*linenum)--; //
				return Token(ERR, lexeme, *linenum);
			}
			else if (!isdigit(ch)) //puts operators and spaces
			{
				in->putback(ch);
				return Token(ICONST, lexeme, *linenum);
			}
			lexeme += ch;

			break;

		case INSCONST:

			if (ch == '"')
			{
				lexeme.erase(0, 1); //Erases first parenthesis
				return Token(SCONST, lexeme, *linenum);
			}

			lexeme += ch;

			if (ch == '\n') //ERR: cannot have newline within SCONST
			{
				return Token(ERR, lexeme, *linenum);
			}

			break;

		case INCOM:

			if (ch == '\n') //When comment line ends, bring back to BEGIN
			{
				(*linenum)++;
				lexstate = BEGIN;
			}
			break;

		case INASSIGN:
			lexeme += ch; //peeks next ch
			if (lexeme == "= ")
			{
				in->putback(ch);
				return Token(ASSIGN, lexeme, *linenum);
			}
			else if (lexeme == "==")
			{
				return Token(EQ, lexeme, *linenum);
			}
			else //ex: "=j","=\n". j or \n gets putback
			{
				in->putback(ch);
				return Token(ASSIGN, lexeme, *linenum);
			}

			break;

		case INNEQ:
			lexeme += ch; //peeks next ch
			if (lexeme == "!=")
			{
				return Token(NEQ, lexeme, *linenum);
			}
			else if (lexeme == "!\n") //ERR: removes newline error from peeked lexeme back
			{
				lexeme.pop_back();
				return Token(ERR, lexeme, *linenum);
			}
			else //ERR: ! has to be followed by =
			{
				in->putback(ch);
				return Token(ERR, lexeme, *linenum);
			}
			break;

		case INLT:
			lexeme += ch;
			if (lexeme == "<=")
			{
				return Token(LEQ, lexeme, *linenum);
			}
			else if (lexeme == "< ")
			{
				in->putback(ch);
				return Token(LT, lexeme, *linenum);
			}
			else
			{
				in->putback(ch);
				return Token(LT, lexeme, *linenum);
			}
			break;

		case INGT:
			lexeme += ch;
			if (lexeme == ">=")
			{
				return Token(GEQ, lexeme, *linenum);
			}
			else if (lexeme == "> ")
			{
				in->putback(ch);
				return Token(GT, lexeme, *linenum);
			}
			else
			{
				in->putback(ch);
				return Token(GT, lexeme, *linenum);
			}
			break;

		case INLOGICAND:
			lexeme += ch;
			if (lexeme == "&&")
			{
				return Token(LOGICAND, lexeme, *linenum);
			}
			else if (lexeme == "&\n") //ERR: removes newline error from peeked lexeme back
			{
				lexeme.pop_back();
				return Token(ERR, lexeme, *linenum);
			}
			else //ERR: & has to be followed by &
			{
				in->putback(ch);
				return Token(ERR, lexeme, *linenum);
			}
			break;

		case INLOGICOR:
			lexeme += ch;
			if (lexeme == "||")
			{
				return Token(LOGICOR, lexeme, *linenum);
			}
			else if (lexeme == "|\n") //ERR: removes newline error from peeked lexeme back
			{
				lexeme.pop_back();
				return Token(ERR, lexeme, *linenum);
			}
			else //ERR: | has to be followed by |
			{
				in->putback(ch);
				return Token(ERR, lexeme, *linenum);
			}
			break;
		}
	}

	(*linenum)--; //eliminates off by one error since newline is added at end
	return Token(DONE, lexeme, *linenum);
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

	return out;
}