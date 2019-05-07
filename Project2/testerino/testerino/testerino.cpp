// testerino.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int main(int argc, _TCHAR* argv[])
{
	int var;
	int& vr = var;
	var = 3;
	cout << vr;

	return 0;
}

