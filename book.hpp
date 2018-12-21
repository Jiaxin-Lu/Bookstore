#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using std::string;

#include "mystring.hpp"

class book
{
private:
	mystring <20> ISBN;
	mystring <40> name, author, keyword;
	double price = 0;
	int quantity = 0;

public:
	book()
	{
		ISBN = 0;
		name = 0;
		author = 0;
		keyword = 0;
		price = 0;
		quantity = 0;
	}
	book(int _t)
	{
		ISBN = 0;
		name = 0;
		author = 0;
		keyword = 0;
		price = 0;
		quantity = 0;
	}
	mystring <20> get_ISBN() { return ISBN; }
	mystring <40> get_name() { return name; }
	mystring <40> get_author() { return author; }
	mystring <40> get_keyword() { return keyword; }
	double get_price() { return price; }
	int get_quantity() { return quantity; }

	void set_ISBN(mystring <20> isbn) { ISBN = isbn; }
	void set_name(mystring <40> nm) { name = nm; }
	void set_author(mystring <40> aut) { author = aut; }
	void set_keyword(mystring <40> kyw) { keyword = kyw; }
	void set_price(double prc) { price = prc; }
	void set_quantity(int quan) { quantity = quan; }

	operator bool() const
	{
		return bool(ISBN);
	}

	void out()
	{
		if (!ISBN) return;
		ISBN.out(); std::cout << "\t";
		name.out(); std::cout << "\t";
		author.out(); std::cout << "\t";
		keyword.out(); std::cout << "\t";
		printf("%.2lf\t", price);
		printf("%d±¾\n", quantity);
	}
};

#endif // !BOOK_HPP
