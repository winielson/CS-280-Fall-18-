// Project2lex.cpp 
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
            
        /*
        ININT,
        INPRINT,
        INIF,
        INTHEN,
        INTRUE,
        INFALSE,
        INADD,
        INSUB,
        INMULT,
        INDIV,
        INLPAREN,
        //INRPAREN,
        INSEMI,
        INEQ,
        //INEQT,
        INNOTEQ,
        INGRT,
        //INGRTEQ,
        INLSS,
       // INLSSEQ,
        INLAND,
        INLOR,
        INDONE,
        INERR*/
    };
    
    LexState lexstate = BEGIN;
    string lexeme;
    char ch;
    
    while(!in->eof())
    {
        if(ch == '\n')
            (*linenum)++;
        
        switch(lexstate)
        {
            case BEGIN:
                        if(isspace(ch))
                            continue;
                        
                        lexeme = ch;
                        
                        if(isalpha(ch))
                        {
                            lexstate = INID;
                        }
                        else if(ch == '"')
                        {
                            lexstate = INSTRING;
                        }
                       // else if(ch == ERR)
                        
                        /*else if(isdigit(ch))
                            lexstate = ININT;
                        else if(ch == '+')
                            lexstate = INADD;
                        else if(ch == '-')
                            lexstate = INSUB;
                        else if(ch == '*')
                            lexstate = INMULT;
                        else if(ch == '/')
                            lexstate = INDIV;
                        else if(ch == '(')
                            lexstate = INLPAREN;
                        else if(ch == ')')
                            lexstate = INRPAREN;
                        else if(ch == ';')
                            lexstate = INSEMI;
                        else if(ch == '=')
                            lexstate = INEQ;
                        else if(ch == '!')
                            lexstate = INNOTEQ;
                        else if(ch == '>')
                            lexstate = INGRT;
                        else if(ch == '<')
                            lexstate = INLSS;    
                        else if(ch == '&')
                            lexstate = INLAND;
                        else if(ch == '|')
                            lexstate = INLOR;
                        else if(in.eof())
                            lexstate = INDONE;
                        else
                            lexstate = INERR;*/
                        break;
           case INID: cout << "IN INID\n";
                        break;
           case INSTRING:  
                            lexeme += ch; //adds first letter
                
                            while(in->get(ch)){
                                //cout << " " << ch; 

                                if (ch == '"') //needs to be more efficient  so it doesn't check for quote every single loop
                                { 
                                    
                                    //lexstate = INDONE;
                                    lexeme.erase(0, 1); //erases quote from lexeme
                                    cout << "SCONST(" << lexeme << ")" << endl; 
                                    
                                    
                                    //return Token();
                                    //return Token(SCONST, lexeme, *linenum);
                                    //lexstate = INDONE;
                                }
                                lexeme += ch;

                            }
                        
                        //cout << "Lexeme: " << lexeme;
                
                        
                /*
                        while(ch != '"' || !isdigit(ch))
                        {
                            //in->get(ch);
                            lexeme += ch;
                            in->get(ch);
                            cout << "Lexeme = " << lexeme;
                            i++;
                        }
                 */
                //cout << "hello" << endl;
                        //lexstate = BEGIN;
                       // tok.tt = SCONST;
                        //tok.lexeme = lexeme;
                        //tok.lnum = linenum;
                
                //return tok;
                /*
            case ININT: 
                INPRINT
                 INIF INTHEN INTRUE  INFALSE  INADD INSUB  INMULT
                 */ break;
            case INDONE: 
                         cout << "IN INDONE";
                         lexeme.erase(0, 1); //erases quote from lexeme
                         cout << "SCONST(" << lexeme << ")" << endl;  break;
            case INERR: break;
        }
    }
    
       return Token(); 
}

