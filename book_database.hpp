#ifndef BOOKDATABASE_HPP
#define BOOKDATABASE_HPP

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
using std::string;
using std::fstream;
using std::vector;
using std::sort;
using std::cout;
using std::endl;

#include "database.hpp"
#include "database_n.hpp"
#include "mystring.hpp"
#include "book.hpp"

class book_database
{
private:
	database<mystring<20>, book> bookdata;
	database_n<mystring<40>, mystring<20>> bnamedata;
	database_n<mystring<40>, mystring<20>> bauthdata;
	database_n<mystring<40>, mystring<20>> bkeywdata;
public:
	void init()
	{
		bookdata.initialize("book");
		bnamedata.initialize("book_name");
		bauthdata.initialize("book_auth");
		bkeywdata.initialize("book_keyw");
	}
	//book
	book find(mystring<20> _isbn)
	{
		return bookdata.Find(_isbn);
	}
	bool exist(mystring<20> _isbn)
	{
		return bookdata.canFind(_isbn);
	}
	void modify(mystring<20> _isbn, book _book)
	{
		bookdata.Modify(_isbn, _book);
	}
	void add(mystring<20> _isbn, book _book)
	{
		bookdata.Add(_isbn, _book);
	}
	void del(mystring<20> _isbn)
	{
		bookdata.Del(_isbn);
	}
	void showall()
	{
		auto _it = bookdata.begin();
		for (; _it; ++_it) (*_it).out();
		return;
	}

	//name
	void Del_name(mystring<40> _name, mystring<20> _isbn)
	{
		bnamedata.Del(_name, _isbn);
	}
	void Add_name(mystring<40> _name, mystring<20> _isbn)
	{
		bnamedata.Add(_name, _isbn);
	}
	void Modify_name(mystring<40> _name, mystring<20> _isbn, mystring<20> _isbn_n)
	{
		bnamedata.Modify(_name, _isbn, _isbn_n);
	}
	void showbyname(mystring<40> _name)
	{
		vector <mystring<20>> _isbn;
		auto _it = bnamedata.Find(_name);
		for (; *_it; ++_it) _isbn.push_back(*_it);
		sort(_isbn.begin(), _isbn.end());
		book b_tmp;
		for (auto it = _isbn.begin(); it != _isbn.end(); ++it)
		{
			b_tmp = bookdata.Find(*it);
			b_tmp.out();
		}
		return;
	}

	//author
	void Del_author(mystring<40> _auth, mystring<20> _isbn)
	{
		bauthdata.Del(_auth, _isbn);
	}
	void Add_author(mystring<40> _auth, mystring<20> _isbn)
	{
		bauthdata.Add(_auth, _isbn);
	}
	void Modify_author(mystring<40> _auth, mystring<20> _isbn, mystring<20> _isbn_n)
	{
		bauthdata.Modify(_auth, _isbn, _isbn_n);
	}
	void showbyauthor(mystring<40> _auth)
	{
		vector <mystring<20>> _isbn;
		auto _it = bauthdata.Find(_auth);
		for (; *_it; ++_it) _isbn.push_back(*_it);
		sort(_isbn.begin(), _isbn.end());
		book b_tmp;
		for (auto it = _isbn.begin(); it != _isbn.end(); ++it)
		{
			b_tmp = bookdata.Find(*it);
			b_tmp.out();
		}
	}

	//keyword
	void Del_keyword(mystring<40> _keyw, mystring<20> _isbn)
	{
		bkeywdata.Del(_keyw, _isbn);
	}
	void Add_keyword(mystring<40> _keyw, mystring<20> _isbn)
	{
		bkeywdata.Add(_keyw, _isbn);
	}
	void Modify_keyword(mystring<40> _keyw, mystring<20> _isbn, mystring<20> _isbn_n)
	{
		bkeywdata.Modify(_keyw, _isbn, _isbn_n);
	}
	void showbykeyword(mystring<40> _keyw)
	{
		vector <mystring<20>> _isbn;
		auto _it = bkeywdata.Find(_keyw);
		for (; *_it; ++_it) _isbn.push_back(*_it);
		sort(_isbn.begin(), _isbn.end());
		book b_tmp;
		for (auto it = _isbn.begin(); it != _isbn.end(); ++it)
		{
			b_tmp = bookdata.Find(*it);
			b_tmp.out();
		}

	}
};

#endif // !BOOKDATABASE_HPP
