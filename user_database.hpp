#ifndef USERDATABASE_HPP
#define USERDATABASE_HPP

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
using std::string;
using std::fstream;
using std::cout;
using std::endl;

#include "database.hpp"
#include "database_n.hpp"
#include "mystring.hpp"
#include "user.hpp"

class user_database
{
private:
	database<mystring<30>, user> userdata;
public:
	void init()
	{
		userdata.initialize("user");
	}
	void ROOT()
	{
		init();
		user sjtuROOT;
		sjtuROOT.set_id("root");
		sjtuROOT.set_name("root");
		sjtuROOT.set_passwd("sjtu");
		sjtuROOT.set_rt(7);
		userdata.Add(sjtuROOT.get_id(), sjtuROOT);
		return;
	}
	user find(mystring<30> _id)
	{
		return userdata.Find(_id);
	}
	bool exist(mystring<30> _id)
	{
		return userdata.canFind(_id);
	}
	void Adduser(mystring<30> _id, mystring<30> _pwd, int _rt, mystring<20> _name)
	{
		user utmp;
		utmp.set_id(_id);
		utmp.set_name(_name);
		utmp.set_passwd(_pwd);
		utmp.set_rt(_rt);
		userdata.Add(_id, utmp);
		return;
	}
	void del(mystring<30> _id)
	{
		//cout << "del _id is " << _id << endl;//???
		userdata.Del(_id);
	}
	void modify(mystring<30> _id, user _user)
	{
		userdata.Modify(_id, _user);
		//cout << "new passwd again : " << _user.get_passwd() << endl;//???
		return;
	}
};

#endif // !USERDATABASE_HPP
