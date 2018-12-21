#ifndef TOKENSCANNER_HPP
#define TOKENSCANNER_HPP

#include <string>
using std::string;

class TokenScanner
{
private:
	string str;
	int ptr = 0;

public:
	void setInput(string strr)
	{
		str = strr;
	}

	void ignoreWhiteSpace()
	{
		for (; ptr < (int)str.length() && (str[ptr] == ' ' || str[ptr] == '\n'); ++ptr);
	}

	bool hasMoreTokens()
	{
		ignoreWhiteSpace();
		return (ptr < (int)str.length());
	}
	
	bool isNumber(string str)
	{
		bool fdot = false;
		if (str.length() == 0) return false;
		for (int i = 0; i < (int)str.length(); ++i)
		{
			if (str[i] == '.')
			{
				if (fdot == true) return false;
				else { fdot = true; continue; }
			}
			else if (str[i] < '0' || str[i] > '9') return false;
		}
		return true;
	}

	string nextToken()
	{
		string ret = "";
		ignoreWhiteSpace();
		if (str[ptr] == '-' || str[ptr] == '=')
		{
			ret += str[ptr];
			++ptr;
			return ret;
		}

		if (str[ptr] == '"')
		{
			ret += str[ptr];
			++ptr;
			for (; str[ptr] != '"'; ++ptr)
				ret += str[ptr];
			ret += str[ptr];
			++ptr;
			return ret;
		}

		for (; ptr < (int)str.length() && !(str[ptr] == ' ' || str[ptr] == '\n' || str[ptr] == '-' || str[ptr] == '='); ++ ptr) ret += str[ptr];
		return ret;
	}
	
};

#endif //! TOKENSCANNER_HPP