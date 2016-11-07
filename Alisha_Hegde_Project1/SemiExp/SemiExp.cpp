///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Alisha Hegde, Syracuse University                    //
//              alhegde@syr.edu                                      //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <vector>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

int SemiExp::find(std::string &str)
{
	int i = 0;
	int pos = -1;
	for (std::vector<Token>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
	{
		if (*it == str)
		{
			pos = i;
			break;
		}
		i++;

	}
	return pos;
}

	bool SemiExp::get()
	{
		if (_pToker == nullptr)
			throw(std::logic_error("no Toker reference"));
		_tokens.clear();
		while (true)
		{
			std::string token = _pToker->getTok();
			if (token == "")
				break;
			_tokens.push_back(token);
		
			if (token == "/*")
			{
				do {
					token = _pToker->getTok();
					_tokens.push_back(token);
					// return true;
				} while (token != "*/");
				return true;
			}

			if (token == "{" || token == ";" || token == "}")
				return true;

			if (token == "#")
			{
				do {
					token = _pToker->getTok();
					_tokens.push_back(token);
					// return true;
				} while (token != "\n");
				return true;
			}
			if (token == "for")
			{
				do
				{
					token = _pToker->getTok();
					_tokens.push_back(token);
					if (token == ";")
					{
						//_tokens.push_back(token);
						continue;

					}

				} while (token != ")");
				token = _pToker->getTok();
				if (token == ";")
				{
					_tokens.push_back(token);
					// continue;
				}

				return true;
			}
			if (token == "public" || token == "protected" || token == "private")
			{
				do
				{
					token = _pToker->getTok();
					_tokens.push_back(token);
				} while (token != ":");
				return true;
			}

		}

		return false;
	}

Token SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

size_t SemiExp::length()
{
  return _tokens.size();
}

void SemiExp::show()
{
  std::cout << "\n  ";
  for (auto token : _tokens)
	  if (token != "\n")
		  std::cout << token<<" ";
  
  std::cout << "\n";
}
//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

int main()
{
  Toker toker;
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  std::string fileSpec = "../Tokenizer/Test.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);
  toker.showComments(false);
  SemiExp semi(&toker);
  while(semi.get())
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
  }
  
   May have collected tokens, but reached end of stream
     before finding SemiExp terminator. 
   
  if (semi.length() > 0)
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }
  return 0;
}
#endif


