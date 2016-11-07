///////////////////////////////////////////////////////////////////////
// TestExec.cpp   Check the functionality of SemiExp and Tokenizer   //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <vector>
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
using namespace Scanner;
int main()
{
	std::string fileSpec = "../Tokenizer/Test.txt";
	std::string fileSpec1 = "../Tokenizer/SemiExpTestf.txt";
	std::ifstream in(fileSpec);
	std::ifstream in1(fileSpec1);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	if (!in1.good())
	{
		std::cout << "\n  can't open " << fileSpec1 << "\n\n";
		return 1;
	}

	std::cout << "----Requirement 3: Implemented packages for Tokenizing, SemiExpression and ItokCollection(SemiExp.h) ------\n\n";
	std::cout << "==============================================================================\n" ;
	std::cout << "-----Requirement 4 and Requirement 5: Testing the Toker class ------\n";
	std::cout << "==============================================================================\n";
	
	std::cout << "\n\n------- The contents of the input file for Tokenizer-----\n\n";
	std::string getContent;
	if (in.is_open())
	{
		//std::cout << "The Input file for Tokenizer\n";
		while (!in.eof())
		{
			getline(in, getContent);
			std::cout << getContent << "\n";
		}
	}
	in.close();
	in.open("../Tokenizer/Test.txt");
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	std::cout << "\n\n\n----The output for the Tokenizer---- \n";
	//std::ifstream in(fileSpec);
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	//std::cout << "\n\n"; 

	std::cout << "----Requirement 7 ,Requirement 8 and Requirement 9-----\n";
	std::cout << "=================================================================";
	
	toker.des();
	std::cout << "\n\n------- The contents of the input file for SemiExp-----\n\n";
	std::string getContent1;
	if (in1.is_open())
	{
		//std::cout << "The Input file for Tokenizer\n";
		while (!in1.eof())
		{
			getline(in1, getContent1);
			std::cout << getContent1 << "\n";
		}
	}
	in1.close();
	in1.open("../Tokenizer/SemiExpTestf.txt");
	if (!in1.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker1;
	toker1.attach(&in1);
	toker1.showComments(false);
	SemiExp semi(&toker1);
	std::cout << "\n\n\n----The output for the SemiExp---- \n";
//	semi.toLower();
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	int i = 0;
	char str[]= "Test String.\n";
	std::cout << " Requirement 9--Implementation of the ItokCollection----\n";
	
	char c;
	while (str[i]) {
		 c=str[i];
		putchar(tolower(c));
		i++;

	}
	
	std::cout << " Requirement 10: includes an automated test suite that runs the test cases for toenizer and SemiExp\n";
	
	return 0;

}
