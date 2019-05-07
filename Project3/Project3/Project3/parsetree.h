/*
* parsetree.h
*/

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <algorithm>
#include <map>
#include "tokens.h"
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };

class ParseTree {
	int			linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }

	virtual NodeType GetType() const { return ERRTYPE; }

	virtual bool IsIDENT() const { return false; }
	virtual bool IsSConst() const { return false; }

	int LeafCount() const {

		int lc = 0;
		if (left) lc += left->LeafCount();
		if (right) lc += right->LeafCount();
		if (left == 0 && right == 0)
			lc++;

		return lc;
	}

	int IDENTCount() const {
		int ic = 0;
		if (left){
			ic += left->IDENTCount();
		}
		if (right){
			ic += right->IDENTCount();
		}
		if (left == 0 && right == 0 && IsIDENT() == true)
			ic++;

		return ic;
	}

	int SConstCount() const {
		int sc = 0;
		if (left){
			sc += left->SConstCount();
		}
		if (right){
			sc += right->SConstCount();
		}
		if (left == 0 && right == 0 && IsSConst() == true)
			sc++;

		return sc;
	}

	//Containers and methods only used for Prog
	vector<string> ivec;
	vector<string>::iterator vecIt;

	map<string, int> imap;
	map<string, int>::iterator mapIt;

	int IDENTct = 0;
	string IDENTlist;

	void mapProcess()
	{
		IDENTct = ivec.size();
		if (ivec.empty() == true)
			return;
		else{
			for (vecIt = ivec.begin(); vecIt != ivec.end(); vecIt++){ //stores IDENTS into map
				string var;
				var = *vecIt;

				mapIt = imap.find(var);
				if (mapIt != imap.end()){ //if in map already, iterates value (->second)
					mapIt->second += 1;
				}
				else {
					imap.insert(pair<string, int>(var, 1)); //if not in map already, intitializes value to 1
				}
			}

			int maxval = imap.begin()->second; //initializes first key's value to max
			for (mapIt = imap.begin(); mapIt != imap.end(); mapIt++){ //checks for key with largest value
				if (mapIt->second > maxval){
					maxval = mapIt->second;
				}
			}

			int numofmax = 0;
			for (mapIt = imap.begin(); mapIt != imap.end(); mapIt++){ //checks if multiple keys have the largest value
				if (mapIt->second == maxval){
					numofmax++;
				}
			}

			string freq;
			int maxvalct = 0;
			for (mapIt = imap.begin(); mapIt != imap.end(); mapIt++){ //fills the string of IDENTs
				if (mapIt->second == maxval){
					freq += mapIt->first;
					maxvalct++;
					if (numofmax != 1 && numofmax != maxvalct) //So ", " wont get added to last ivec string
						freq += ", ";
				}
			}
			IDENTlist = freq;
		}
	}

	void printIDENT(){
		cout << "IDENT COUNT: " << IDENTct << "\n";
		cout << IDENTlist << "\n";
	}
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}

};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
};

class Assignment : public ParseTree {
public:
	Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
};

class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class LogicAndExpr : public ParseTree {
public:
	LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class LogicOrExpr : public ParseTree {
public:
	LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class EqExpr : public ParseTree {
public:
	EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class NEqExpr : public ParseTree {
public:
	NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class LtExpr : public ParseTree {
public:
	LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class LEqExpr : public ParseTree {
public:
	LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class GtExpr : public ParseTree {
public:
	GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class GEqExpr : public ParseTree {
public:
	GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}
};

class IConst : public ParseTree {
	int val;

public:
	IConst(int l, int i) : ParseTree(l), val(i) {}
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	NodeType GetType() const { return INTTYPE; }
};

class BoolConst : public ParseTree {
	bool val;

public:
	BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

	NodeType GetType() const { return BOOLTYPE; }
};

class SConst : public ParseTree {
	bool IsSConst() const { return true; }
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const { return STRTYPE; }
};

class Ident : public ParseTree {
	bool IsIDENT() const { return true; }
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {
		id = t.GetLexeme();
	}
};

#endif /* PARSETREE_H_ */

