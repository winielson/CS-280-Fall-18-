/*
* parsetree.h
*/

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };

class ParseTree {
protected:
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

	int LeafCount() const {
		int lc = 0;
		if (left) lc += left->LeafCount();
		if (right) lc += right->LeafCount();
		if (left == 0 && right == 0)
			lc++;
		return lc;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsString() const { return false; }
	virtual bool IsAssign() const { return false; }

	virtual string GetId() const { return ""; }
	virtual Value Eval(map<string, Value>& varmap) { cout << "base Eval\n"; return 0; }

	int IdentCount() const {
		int cnt = 0;
		if (left) cnt += left->IdentCount();
		if (right) cnt += right->IdentCount();
		if (IsIdent())
			cnt++;
		return cnt;
	}

	int StringCount() const {
		int cnt = 0;
		if (left) cnt += left->StringCount();
		if (right) cnt += right->StringCount();
		if (IsString())
			cnt++;
		return cnt;
	}

	void GetVars(map<string, int>& var) {
		if (left) left->GetVars(var);
		if (right) right->GetVars(var);
		if (IsIdent())
			var[this->GetId()]++;
	}
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}

	Value Eval(map<string, Value>& varmap){
		if (this->left != NULL){
			this->left->Eval(varmap);
		}
		if (this->right != NULL){
			this->right->Eval(varmap);
		}

		return 0;
	}
};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}

	Value Eval(map<string, Value> &varmap){
		if (this->left->Eval(varmap).isBoolType() == false){
			cout << this->linenum << ": RUNTIME ERROR if expression is not boolean typed\n";
			exit(EXIT_FAILURE);
		}

		if (this->left->Eval(varmap).getBoolean() == true){
			this->right->Eval(varmap);
		}

		return 0;
	}
};

class Assignment : public ParseTree {
public:
	Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}

	bool IsAssign() const { return true; }

	Value Eval(map<string, Value> &varmap){
		string ident;
		// int val;

		if (this->right->IsAssign()){ //checks to see if right is an assignment first so that it evals it first
			this->right->Eval(varmap); //eval child assign first

			ident = this->left->GetId();
			varmap[ident] = this->right->Eval(varmap);
		}
		else if (this->left->IsIdent()){
			ident = this->left->GetId();
			//val = this->right->Eval(varmap).getInteger();

			varmap[ident] = this->right->Eval(varmap);
			return Value(this->right->Eval(varmap));
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Left hand side must be an identifier\n";
			exit(EXIT_FAILURE);
		}



		return 0;
	}
};

class PrintStatement : public ParseTree {
	string val;
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}

	Value Eval(map<string, Value> &varmap){
		cout << this->left->Eval(varmap) << endl;
		return 0;
	}
};

class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	Value Eval(map<string, Value> &varmap){
		auto a1 = left->Eval(varmap);
		auto a2 = right->Eval(varmap);
		Value holder = new Value;
		holder = a1 + a2;
		if (holder.isError()){
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in addition\n";
			exit(EXIT_FAILURE);
		}
		else
			return a1 + a2; // uses overloaded operator+  
	}
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	Value Eval(map<string, Value> &varmap){
		auto a1 = left->Eval(varmap);
		auto a2 = right->Eval(varmap);
		Value holder = new Value;
		holder = a1 - a2;
		if (holder.isError()){
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in subtraction\n";
			exit(EXIT_FAILURE);
		}
		else
			return a1 - a2; // uses overloaded operator-   
	}
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	Value Eval(map<string, Value> &varmap){
		auto a1 = left->Eval(varmap);
		auto a2 = right->Eval(varmap);
		Value holder = new Value;
		holder = a1 * a2;

		if (holder.isError()){
			if (holder.getMessage() == "Multiplying string by a negative int"){
				cout << this->linenum << ": RUNTIME ERROR Multiplying string by a negative int\n";
				exit(EXIT_FAILURE);
			}
			else{
				cout << this->linenum << ": RUNTIME ERROR Type mismatch in multiplication\n";
				exit(EXIT_FAILURE);
			}
		}
		else
			return a1 * a2; // uses overloaded operator*  
	}
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	Value Eval(map<string, Value> &varmap){
		auto a1 = left->Eval(varmap);
		auto a2 = right->Eval(varmap);
		Value holder = new Value;

		if (a2.getInteger() == 0){ //Division by zero error
			cout << this->linenum << ": RUNTIME ERROR Division by zero\n";
			exit(EXIT_FAILURE);
		}

		holder = a1 / a2;

		if (holder.isError()){
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in division\n";
			exit(EXIT_FAILURE);
		}
		else
			return a1 / a2; // uses overloaded operator/  
	}
};

class LogicAndExpr : public ParseTree {
public:
	bool bval;

	LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (!le.isBoolType() || !ri.isBoolType()){
			cout << this->linenum << ": RUNTIME ERROR one or more operands of && are not boolean typed\n";
			exit(EXIT_FAILURE);
		}

		if (le.getBoolean() && ri.getBoolean()){
			bval = true;
			return Value(bval);
		}
		else{
			bval = false;
			return Value(bval);
		}
		return 0;
	}
};

class LogicOrExpr : public ParseTree {
public:
	bool bval;

	LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (!le.isBoolType()){ //short circuiting
			cout << this->linenum << ": RUNTIME ERROR one or more operands of || are not boolean typed\n";
			exit(EXIT_FAILURE);
		}

		if (le.getBoolean() || ri.getBoolean()){
			bval = true;
			return Value(bval);
		}
		else{
			bval = false;
			return Value(bval);
		}
		return 0;
	}

};

class EqExpr : public ParseTree {
public:
	bool bval;

	EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() == this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() == this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() == this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in ==\n";
			exit(EXIT_FAILURE);
		}

		return 0;
	}

};

class NEqExpr : public ParseTree {
public:
	bool bval;

	NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() != this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() != this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() != this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in !=\n";
			exit(EXIT_FAILURE);
		}
		return 0;
	}
};

class LtExpr : public ParseTree {
public:
	bool bval;

	LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() < this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() < this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() < this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in <\n";
			exit(EXIT_FAILURE);
		}
		return 0;
	}
};

class LEqExpr : public ParseTree {
public:
	bool bval;

	LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() <= this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() <= this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() <= this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in <=\n";
			exit(EXIT_FAILURE);
		}
		return 0;
	}
};

class GtExpr : public ParseTree {
public:
	bool bval;

	GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		//cout << "b4 GT is eval\n";
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() > this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() > this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() > this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in >\n";
			exit(EXIT_FAILURE);
		}
		return 0;
	}
};

class GEqExpr : public ParseTree {
public:
	bool bval;

	GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line, l, r) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		Value le = this->left->Eval(varmap);
		Value ri = this->right->Eval(varmap);

		if (le.isIntType() && ri.isIntType()){
			if (this->left->Eval(varmap).getInteger() >= this->right->Eval(varmap).getInteger()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isStringType() && ri.isStringType()){
			if (this->left->Eval(varmap).getString() >= this->right->Eval(varmap).getString()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else if (le.isBoolType() && ri.isBoolType()){
			if (this->left->Eval(varmap).getBoolean() >= this->right->Eval(varmap).getBoolean()){
				bval = true;
				return Value(bval);
			}
			else{
				bval = false;
				return Value(bval);
			}
		}
		else{
			cout << this->linenum << ": RUNTIME ERROR Type mismatch in >=\n";
			exit(EXIT_FAILURE);
		}
		return 0;
	}
};

class IConst : public ParseTree {
	int val;

public:
	IConst(int l, int i) : ParseTree(l), val(i) {}
	IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}

	NodeType GetType() const { return INTTYPE; }

	Value Eval(map<string, Value> &varmap){
		return Value(val);
	}
};

class BoolConst : public ParseTree {
	bool val;

public:
	BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}

	NodeType GetType() const { return BOOLTYPE; }

	Value Eval(map<string, Value> &varmap){
		return Value(val);
	}
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}

	NodeType GetType() const { return STRTYPE; }
	bool IsString() const { return true; }

	Value Eval(map<string, Value> &varmap){
		return Value(val);
	}
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	bool IsIdent() const { return true; }
	string GetId() const { return id; }

	Value Eval(map<string, Value> &varmap){
		map<string, Value>::iterator it;
		it = varmap.find(id);
		if (it != varmap.end())
			return Value(it->second);
		else{
			cout << this->linenum << ": RUNTIME ERROR " << id << " was used before being set\n";
			exit(EXIT_FAILURE);
		}
		return Value();
	}
};

#endif /* PARSETREE_H_ */