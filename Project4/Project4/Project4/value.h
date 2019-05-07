/*
* value.h
*/

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <map>
#include <iostream>
using namespace std;

// object holds boolean, integer, or string, and remembers which it holds
class Value {
	bool	bval;
	int		ival;
	string	sval;
	enum VT { isBool, isInt, isString, isTypeError } type;

public:
	Value() : bval(false), ival(0), type(isTypeError) {}
	Value(bool bval) : bval(bval), ival(0), type(isBool) {}
	Value(int ival) : bval(false), ival(ival), type(isInt) {}
	Value(string sval) : bval(false), ival(0), sval(sval), type(isString) {}

	// in the case of an error, I use the value to hold the error message
	Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}

	bool isBoolType() const { return type == VT::isBool; }
	bool isIntType() const { return type == VT::isInt; }
	bool isStringType() const { return type == VT::isString; }
	bool isError() const { return type == VT::isTypeError; }
	bool hasMessage() const { return isError() && sval.size() > 0; }

	bool isTrue() const { return isBoolType() && bval; }
	bool getBoolean() const {
		if (!isBoolType())
			throw "Not boolean valued";
		return bval;
	}

	int getInteger() const {
		if (!isIntType())
			throw "Not integer valued";
		return ival;
	}

	string getString() const {
		if (!isStringType())
			throw "Not string valued";
		return sval;
	}

	string getMessage() const {
		if (!hasMessage())
			throw "No message";
		return sval;
	}

	friend ostream& operator<<(ostream& out, const Value& v) {
		if (v.type == VT::isBool) out << (v.bval ? "True" : "False");
		else if (v.type == VT::isInt) out << v.ival;
		else if (v.type == VT::isString) out << v.sval;
		else if (v.sval.size() > 0) out << "RUNTIME ERROR " << v.sval;
		else out << "TYPE ERROR";
		return out;
	}

	friend Value operator+(const Value& v, const Value& z){
		if (v.isIntType() && z.isIntType())
			return Value(v.getInteger() + z.getInteger());
		else if (v.isStringType() && z.isStringType())
			return Value(v.getString() + z.getString());
		else
			return Value("Type mismatch in addition", true);

		return 0;
	}

	friend Value operator-(const Value& v, const Value& z){
		if (v.isIntType() && z.isIntType())
			return Value(v.getInteger() - z.getInteger());
		else
			return Value("Type mismatch in subtraction", true);
		return 0;
	}

	friend Value operator*(const Value& v, const Value& z){
		if (v.isIntType() && z.isIntType())
			return Value(v.getInteger() * z.getInteger());
		else if (v.isStringType() && z.isIntType()){ //string times integer
			if (z.getInteger() > 0){
				string mult;

				for (int i = 0; i < z.getInteger(); i++){
					mult += v.getString();
				}

				return Value(mult);
			}
			else if (z.getInteger() < 0){ //cannot multiply string to negative integer
				return Value("Multiplying string by a negative int", true);
			}
			else{
				string mult = "";
				return Value(mult);
			}
		}
		else if (v.isIntType() && z.isStringType()){ //integer times string
			if (v.getInteger() > 0){
				string mult;

				for (int i = 0; i < v.getInteger(); i++){
					mult += z.getString();
				}

				return Value(mult);
			}
			else{
				string mult = "";
				return Value(mult);
			}
		}
		else if (v.isIntType() && z.isBoolType()){ //negative factor
			string val;

			if (z.getBoolean() == 1){ //opposite is returned
				val = "False";
				return Value(val);
			}
			else {
				val = "True";
				return Value(val);
			}

		}
		else
			return Value("Type mismatch in multiplication", true);

		return 0;
	}

	friend Value operator/(const Value& v, const Value& z){
		if (v.isIntType() && z.isIntType())
			return Value(v.getInteger() / z.getInteger());
		else
			return Value("Type mismatch in division", true);

		return 0;
	}

	Value operator<(const Value& v);
	Value operator<=(const Value& v);
	Value operator>(const Value& v);
	Value operator>=(const Value& v);
	Value operator==(const Value& v);
	Value operator!=(const Value& v){
		Value ans = this->operator==(v);
		ans.bval = !ans.bval;
		return ans;
	}
};

#endif /* VALUE_H_ */
