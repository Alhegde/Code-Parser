/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Alisha Hegde, Syracuse University,                 //
//              alhegde@syr.edu                                    //
/////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		//std::vector<std::string> doubleChar = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
		std::set<std::string> doubleChar = { "<<" , ">>" , "::", "++", "--", "==" , "+=", "-=", "*=", "/=" };
		std::vector<char> singleChar = { '<', '>','[',']','(',')','{' , '}', ':', '=', '+', '-', '*', '\n' };
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		void showComments(bool show);
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		bool isSingleChar(char currChar1)   //checks if the character is single char
		{
			//std::string current = currChar[0];
			for (size_t i = 0; i < singleChar.size(); i++)
			{
				if (currChar1 == singleChar[i])
				{
					return true;
				}
			}
			return false;
		}
		void setstaticvariables()   //reset the value of the toker object
		{
			_pState = _pEatWhitespace;
			_pIn = nullptr;
			currChar = NULL;
			token = "";
		}
		

	

	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pSpecialSingleChars;
		static ConsumeState* _pSpecialCharPairs;
		static ConsumeState* _pEatString;
		//static bool first;

	};
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pSpecialSingleChars;
ConsumeState* ConsumeState::_pSpecialCharPairs;
ConsumeState* ConsumeState::_pEatString;
//bool ConsumeState:: first = true;

void testLog(const std::string& msg);
bool showComment = true;

void ConsumeState::showComments(bool show)  //permission to display comments
{
	showComment = show;
}
ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
	{
		return nullptr;
	}
	int chNext = _pIn->peek();
	char cCurr = currChar;
	char cNext = chNext;
	char cPrev = prevChar;
	std::string dChars;
	dChars = currChar;
	dChars += cNext;
	if (chNext == EOF)
	{
		_pIn->clear();
		// if peek() reads end of file character, EOF, then eofbit is set and
		// _pIn->good() will return false.  clear() restores state to good
	}
	if (currChar == '"')
	{
		testLog("state: EatString");
		return _pEatString;
	}
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}

	 if (currChar == '/' && chNext == '*')
	{
	testLog("state: eatCComment");
	return _pEatCComment;
	}
   
	if (currChar == '"' && chNext == '\'')
	{
		testLog("state eatstring");
		return _pEatString;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::find(doubleChar.begin(), doubleChar.end(), dChars) != doubleChar.end()) //finds a special character in the vector
	{
		// std::cout << "double";
		testLog("state: eatSpecialCharPairs");
		return _pSpecialCharPairs;
	}
	if (isSingleChar(currChar))
	{
		testLog("state: SpecialSingleChar");
		return _pSpecialSingleChars;
	}
	if (std::isalnum(currChar))
	{
		// std::cout << "alphanum";
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}

	if (ispunct(currChar))
	{
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}

	if (!_pIn->good())
	{
		//std::cout << "\n  end of stream with currChar = " << currChar << "\n\n";
		return _pEatWhitespace;
	}
	throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			
			currChar = _pIn->get();
			
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			if (!_pIn->good())  // end of stream
				return;
			if(showComment)     
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '\n');
	}
};

class EatSring :public ConsumeState
{
public:
	virtual void eatChars()
	{
		
			token.clear();
			currChar = _pIn->get();
			//std::cout << currChar;
			//bool flag = true;
			do {
				token += currChar;
				currChar = _pIn->get();
				if (currChar == '"')
					break;
			} while (currChar != '"');
			currChar = _pIn->get();

		}
};
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C comment";
		do {
			if (!_pIn->good())  // end of stream
				return;
			if (showComment)
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		if (showComment)
		token += currChar;
		if (showComment)
		token += _pIn->peek();
		_pIn->get();
		currChar = _pIn->get();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar) && !(isSingleChar(currChar)) && currChar != '"');
	}
};

class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

class SpecialCharPairs : public ConsumeState
{
public:
	virtual void eatChars()
	{
		//int i;
		token.clear();
		token = currChar;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();
	}

};
class SpecialSingleChars : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n eating single chars";
		if (!_pIn->good())
			return;
		if (isSingleChar(currChar))
		{
			token.clear();
			token = currChar;
		}
		currChar = _pIn->get();
	}
};




ConsumeState::ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pSpecialCharPairs = new SpecialCharPairs();
		_pSpecialSingleChars = new SpecialSingleChars();
		_pEatString = new EatSring();
		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}
void Toker::showComments(bool showComment)
{
	pConsumer->showComments(showComment);
}
void Toker::des() // resets the toker object
{
	pone->setstaticvariables();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n";
	return 0;
}
#endif
