// Project0.cpp 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 9/18/2018

#include <iostream>
#include <algorithm>
using namespace std;

//First letter L; convert every uppercase letter to lowercase; print result on line by itself; If a character is not an uppercase letter, then it should be unchanged.
void pUtoL(string& a) //All function parameters reference to string "arg"
{
	for (int i = 0; i < a.length(); i++) //Iterates through string starting from first element and stops when the length of the string is met. [NOT i <= a.length because array starts at 0 and length starts counting from 1]
	{
		a[i] = tolower(a[i]); //Elements are converted to lowercase one by one
	}
}

//First letter U; convert every lowercase letter to uppercase; print result on line by itself; If a character is not a lowercase letter, then it should be unchanged. 
void pLtoU(string& a)
{
	for (int i = 0; i < a.length(); i++)
	{
		a[i] = toupper(a[i]); //Elements are converted to uppercase one by one
	}
}

//First letter r; print the letters in reverse order
void pRev(string& a) 
{
	reverse(a.begin(), a.end()); //From algorithm library; reverses the elements in the range (first element, last element)
}

//First letter R; print the letters in reverse order while ALSO converting any lowercase letters to uppercase. 
void pRevUp(string& a) 
{
	reverse(a.begin(), a.end()); //From algorithm library; reverses the elements in the range (first element, last element)
	for (int i = 0; i < a.length(); i++) //Same for loop used in pLtoU function
	{
		a[i] = toupper(a[i]);
	}
}

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i) //Loops through all command line arguments passed to the program
	{
		string arg = argv[i]; //Initialize argument into string arg
		
		if(arg[0] == 'S' || arg.empty() == 1){} //Nothing is printed when S is first letter or if empty
        else if(arg[0] == 'U')
        {
            arg.erase(0,1); //Discards first character
            pLtoU(arg); //Converts all lower case letters in string into upper case
            cout << arg << endl;
        }
        else if(arg[0] == 'L')
        {
            arg.erase(0,1);
            pUtoL(arg); //Converts all upper case letters in string into lower case
            cout << arg << endl;
        }
        else if(arg[0] == 'r')
        {
            arg.erase(0,1);
            pRev(arg); //Prints the string in reverse order
            cout << arg << endl;
        }
        else if(arg[0] == 'R')
        {
            arg.erase(0,1);
            pRevUp(arg); //Prints the string in reverse order and converts lower case letters to upper case
            cout << arg << endl;
        }
        else
        {
            cout << "FAIL" << endl; //Printed when none if the beginning letter does not have an associated function
        }
	}
	return 0;
}