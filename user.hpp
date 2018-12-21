#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using std::string;

#include "mystring.hpp"

class user
{
private:
	mystring <30> id, passwd;
	mystring <20> name;
	int rt = 0;
public:
	user()
	{
		id = 0;
		passwd = 0;
		name = 0;
		rt = 0;
	}
	user(int _t)
	{
		id = 0;
		passwd = 0;
		name = 0;
		rt = 0;
	}

	mystring <30> get_id() { return id; }
	mystring <30> get_passwd() { return passwd; };
	mystring <30> get_name() { return name; }
	int get_rt() { return rt; }

	void set_id(mystring <30> _id) { id = _id; }
	void set_passwd(mystring <30> _passwd) { passwd = _passwd; }
	void set_name(mystring <20> _name) { name = _name; }
	void set_rt(int _rt) { rt = _rt; }

	operator bool() const
	{
		return (rt != 0);
	}
	bool haslogin(mystring <30> _passwd)
	{
		return (passwd == _passwd);
	}
};

#endif // !USER_HPP