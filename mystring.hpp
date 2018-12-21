#ifndef  MYSTRING_HPP
#define  MYSTRING_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using std::string;
using std::min;
using std::ostream;

template<int sz>
class mystring
{
private:
	char ch[sz];
	int len = 0;

public:

	int get_len() { return len; }

	mystring()
	{
		len = 0;
	}
	mystring(int c)
	{
		if (c == 0) len = 0;
		else
		{
			len = sz;
			for (int i = 0; i < sz; ++i) ch[i] = 127;
		}
	}

	char operator [] (int p)
	{
		if (p >= len) return '\0';
		else return ch[p];
	}

	mystring(string &c)
	{
		len = min((int)c.length(), sz);
		for (int i = 0; i < len; ++i) ch[i] = c[i];
	}

	mystring(char c[])
	{
		len = min((int)strlen(c), sz);
		for (int i = 0; i < len; ++i) ch[i] = c[i];
	}
	mystring(const char c[])
	{
		len = min((int)strlen(c), sz);
		for (int i = 0; i < len; ++i) ch[i] = c[i];
	}
	mystring(const string &c)
	{
		len = min((int)c.length(), sz);
		for (int i = 0; i < len; ++i) ch[i] = c[i];
	}
	template<int _sz>
	mystring(mystring<_sz> &c)
	{
		len = min(c.get_len(), sz);
		for (int i = 0; i < len; ++i) ch[i] = c[i];
	}
	
	operator string() const
	{
		string ret = "";
		for (int i = 0; i < len; ++i) ret = ret + ch[i];
		return ret;
	}
	operator bool() const
	{
		return (len > 0);
	}
	template<int _sz>
	bool operator ==(mystring<_sz> &c)
	{
		if (c.get_len() != len) return false;
		for (int i = 0; i < len; ++i)
			if (c[i] != ch[i]) return false;
		return true;
	}
	template<int _sz>
	bool operator <(mystring<_sz> &c)
	{
		int tmp_len = min(c.get_len(), len);
		for (int i = 0; i < tmp_len; ++i)
		{
			if (ch[i] < c[i]) return true;
			if (ch[i] > c[i]) return false;
		}
		if (len < c.get_len()) return true;
		return false;
	}

	void out()
	{
		for (int i = 0; i < len; ++i) std::cout << ch[i];
	}

	template<int _sz>
	friend ostream& operator<< (ostream &os, const mystring<_sz> &obj);

};

template<int _sz>
ostream& operator<< (ostream &os, const mystring<_sz> &obj)
{
	os << string(obj);
	//for (int i = 0; i < sz; ++i) os << obj.ch[i];
	return os;
}

#endif // ! MYSTRING_HPP
