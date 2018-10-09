// Project1.cpp 
// Winielson Miranda
// CS 280
// Professor Gerard Ryan
// 10/10/2018

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
using namespace std;

void lower(string& a) //Converts string to lowercase
{
	for (int i = 0; i < a.length(); i++) //Iterates through string starting from first element and stops when the length of the string is met. [NOT i <= a.length because array starts at 0 and length starts counting from 1]
	{
		a[i] = tolower(a[i]); //Elements are converted to lowercase one by one
	}
}

int main(int argc, char** argv)
{
	map<string, int> pMap; //initialize phrase map
	string phrase;
	vector<string> sWord;

	//ERROR CASES
	if (argc == 1) //No arguments are passed
	{
		cout << "NO PHRASE LENGTH" << endl;
		return -1;
	}

	int N = atoi(argv[1]); //First argument is length N so this converts it into integer 
	if (N <= 0) //Phrase length cannot be negative or equal to zero
	{
		cout << "INVALID PHRASE LENGTH" << endl;
		return -1;
	}

	string mode = argv[2]; //Second argument is mode ("all" or "top")
	if (mode.empty() == 1) // No mode is given
	{
		cout << "NO MODE" << endl;
		return -1;
	}
	else if (mode != "all" && mode != "top") //Mode is not all and top
	{
		cout << "INVALID MODE" << endl;
		return -1;
	}
	else if (argv[3] == NULL) //No file is given
	{
		cout << "NO FILES GIVEN" << endl;
		return -1;
	}

	for (int i = 3; i < argc; i++) //Names of input files start at 3 element
	{
		ifstream infile; //Declares input file
		infile.open(argv[i]); //Opens file 

		if (!infile.is_open()) //File doesn't open
		{
			cerr << "BAD FILE " << argv[i] << endl;
			return -1;
		}
		else if (infile.peek() == std::ifstream::traits_type::eof()) //If file has no contents 
		{
			cout << "NO PHRASES\n";
			return -1;
		}

		if (N > 1) //Adjusts phrase length if phrase length is more than 1
		{
			while (infile >> phrase) //Splits words in file, Populates vector
			{
				lower(phrase);
				sWord.push_back(phrase); //Puts at end of phrase
			}
			for (int i = 0; i < sWord.size() - 1; i++) //Concatenates words
			{
				int j = N - 1; //Desired phrase length
				int n = i; //Made so i doesn't change
				string wword = sWord[i]; //defines wword as current point in map
				while (j != 0) //Doesn't stop until desired phrase length is met aka 'N'
				{
					wword = wword + " " + sWord[n + 1]; //Adds the next word onto the string separated by a space
					j--;
					n++;
				}
				pMap[wword]++; //Stores phrases of multiple words in map and counts frequency <phrase, frequency>
			}
		}
		else //Adjusts phrase length if phrase length is 1
		{
			while (infile >> phrase) //iterates through file and stores key and value into map
			{
				lower(phrase);//converts string to lower
				pMap[phrase]++; //Stores phrases of one word in map and counts frequency <phrase, frequency>
			}
		}
		infile.close(); //Closes File
	}

	if (mode == "all") //All Mode
	{
		int valMax = 0; //Stores largest value of map
		string keyMax; //Key associated with larges value of map
		int sameCount = 1; //Counter used to check if all values are the same

		for (map<string, int>::iterator it = pMap.begin(); it != pMap.end(); ++it) //Checks ALL PHRASES EQUALLY FREQUENT condition
		{
			if (it->second > valMax)
			{
				keyMax = it->first;
				valMax = it->second;
			}
			else if (it->second == valMax)
			{
				sameCount++; //Counter used to check if all values are the same
			}

			if (pMap.size() == sameCount) //Checks ALL PHRASES EQUALLY FREQUENT
			{
				cout << "ALL PHRASES EQUALLY FREQUENT\n";
				return 0;
			}
		}

		for (map<string, int>::iterator it = pMap.begin(); it != pMap.end(); ++it) //Outputs map
		{
			cout << it->first << " " << it->second << '\n';
		}
	}
	else if (mode == "top")
	{
		int valMax = 0;
		string keyMax;
		int sameCount = 1;

		for (map<string, int>::iterator it = pMap.begin(); it != pMap.end(); ++it) //Checks for top and ALL PHRASES EQUALLY FREQUENT conditions
		{
			if (it->second > valMax) //Stores key with highest value
			{
				keyMax = it->first;
				valMax = it->second;
			}
			else if (it->second == valMax)
			{
				sameCount++; //Counter used to check if all values are the same
			}

			if (pMap.size() == sameCount) //Checks ALL PHRASES EQUALLY FREQUENT
			{
				cout << "ALL PHRASES EQUALLY FREQUENT\n";
				return 0;
			}
		}
		cout << keyMax << " " << valMax << endl; //Outputs key with highest value
	}

	return 0;
}
