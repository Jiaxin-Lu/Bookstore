#ifndef EXC_HPP
#define EXC_HPP

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
using std::string;
using std::fstream;
using std::cout;
using std::endl;

#include "tokenscanner.hpp"
#include "database.hpp"
#include "database_n.hpp"
#include "user_database.hpp"
#include "book_database.hpp"
#include "finance_database.hpp"
#include "log_database.hpp"
#include "mystring.hpp"
#include "user.hpp"
#include "book.hpp"
#include "logsave.hpp"

#define Error { printf("Invalid\n"); return false; }
#define INF_TIME 1e9
class exc
{
private:
	user cur_u;
	book cur_b;
	bool end_state = false;
	user_database USERDB;
	book_database BOOKDB;
	finance_database FINANCEDB;

public:
	void init()
	{
		cur_u.set_id("UNLOG");
		USERDB.init();
		BOOKDB.init();
		FINANCEDB.init();
	}
	int cur_u_rt() { return cur_u.get_rt(); }
	mystring<30> cur_u_id() { return cur_u.get_id(); }
	mystring<20> cur_b_isbn() { return cur_b.get_ISBN(); }
	bool load(string _name);
	bool execute(string _log);
	bool report_finance()
	{
		if (cur_u.get_rt() != 7) Error;
		FINANCEDB.report();
		return true;
	}
	bool su(mystring<30> _id, mystring<30> _pwd)
	{
		user utmp;
		//puts("startfind");
		//cout << "su. _id is " << _id << "_pwd is " << _pwd << endl; //???
		utmp = USERDB.find(_id);
		//puts("startcheck");
		if (!utmp.get_rt()) Error;
		mystring<30> __pwd = 0;
		if (_pwd == __pwd && cur_u.get_rt() > utmp.get_rt()) { cur_u = utmp; return true; }
		if (_pwd == __pwd) Error;
		if (!(utmp.get_passwd() == _pwd)) Error;
		cur_u = utmp;
		//puts("endcheck");
		return true;
	}
	bool logout()
	{
		if (cur_u.get_rt() == 0) Error;
		cur_u.set_rt(0);
		cur_u.set_id("UNLOG");
		//puts("logout done."); //???
		return true;
	}
	bool useradd(mystring<30> _id, mystring<30> _pwd, int _rt, mystring<20> _name)
	{
		//cout << "useradd. _id is " << _id << "_pwd is " << _pwd << "_rt is " << _rt << "_name is " << _name << endl; //???
		if (_rt >= cur_u.get_rt()) Error; //>= ??? >
		if (_rt != 1 && _rt != 3) Error;
		if (USERDB.exist(_id)) Error;
		USERDB.Adduser(_id, _pwd, _rt, _name);
		return true;
		//puts("useradd done."); //???
	}
	bool regist(mystring<30> _id, mystring<30> _pwd, mystring<20> _name)
	{
		//cout << "register. _id is " << _id << "_pwd is " << _pwd << "_rt is " << 1 << "_name is " << _name << endl; //???
		USERDB.Adduser(_id, _pwd, 1, _name);
		return true;
		//puts("register done."); //???
	}
	bool del(mystring<30> _id)
	{
		//cout << "delete. _id is " << _id << endl; //???
		if (cur_u.get_rt() != 7) Error;
		if (!USERDB.exist(_id)) Error;
		USERDB.del(_id);
		//puts("delete done."); //???
		return true;
	}
	bool passwd(mystring<30> _id, mystring<30> _pwd_n, mystring<30> _pwd=0)
	{
		//cout << "passwd. _id is " << _id << "_pwd_n is " << _pwd_n << "_pwd is " << _pwd << endl; //???
		mystring<30> str_null = 0;
		if (_pwd == str_null && cur_u.get_rt() != 7) Error;
		user utmp;
		utmp = USERDB.find(_id);
		if (utmp.get_rt() == 0) Error;
		if ((!(utmp.get_passwd() == _pwd)) && (!(_pwd == str_null))) Error;
		utmp.set_passwd(_pwd_n);
		//cout << "new passwd: " << utmp.get_passwd() << endl;//???
		USERDB.modify(_id, utmp);
		//puts("password done."); //???
		return true;
	}
	bool select(mystring<20> _isbn)
	{
		//cout << "select. _isbn is " << _isbn << endl; //???
		if (cur_u.get_rt() < 3) Error;
		book btmp;
		if (BOOKDB.exist(_isbn))
		{
			cur_b = BOOKDB.find(_isbn);
			return true;
		}
		else
		{
			btmp.set_ISBN(_isbn);
			cur_b = btmp;
			BOOKDB.add(_isbn, btmp);
			return true;
		}
		//puts("select done.");
		return true;
	}
	bool imports(int _quantity, double _price)
	{
		//cout << "imports. _quantity is " << _quantity << " _price is " << _price << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		cur_b.set_quantity(cur_b.get_quantity() + _quantity);
		BOOKDB.modify(cur_b.get_ISBN(), cur_b);
		FINANCEDB.Out(_price);
		return true;
		//puts("imports done."); //???
	}
	bool buy(mystring<20> _isbn, int _quantity, double &_price)
	{
		//cout << "buy. _isbn is " << _isbn << " _quantity is " << _quantity << endl; //???
		if (cur_u.get_rt() < 1) Error;
		if (!BOOKDB.exist(_isbn)) Error;
		book btmp = BOOKDB.find(_isbn);
		if (btmp.get_quantity() < _quantity) Error;
		btmp.set_quantity(btmp.get_quantity() - _quantity);
		BOOKDB.modify(_isbn, btmp);
		_price = _quantity*btmp.get_price();
		FINANCEDB.In(_quantity*btmp.get_price());
		return true;
		//puts("buy done.");
	}
	bool Modify_isbn(mystring<20> _isbn)
	{
		//cout << "M_isbn. _isbn is " << _isbn << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		if (BOOKDB.exist(_isbn)) Error;
		BOOKDB.del(cur_b.get_ISBN());
		if (cur_b.get_name()) BOOKDB.Modify_name(cur_b.get_name(), cur_b.get_ISBN(), _isbn);
		if (cur_b.get_author()) BOOKDB.Modify_author(cur_b.get_author(), cur_b.get_ISBN(), _isbn);
		if (cur_b.get_keyword())
		{
			string str;
			mystring<40> _keyw;
			mystring<40> cur_keyw = cur_b.get_keyword();
			int len = cur_keyw.get_len();
			int p = 0;
			while (p < len)
			{
				str = "";
				while (cur_keyw[p] != '|' && p < len) str += cur_keyw[p], ++p;
				_keyw = str;
				BOOKDB.Modify_keyword(_keyw, cur_b.get_ISBN(), _isbn);
				++p;
			}
		}
		cur_b.set_ISBN(_isbn);
		BOOKDB.add(_isbn, cur_b);
		//puts("M_isbn done.");//???
		return true;
	}
	bool Modify_name(mystring<40> _name)
	{
		//cout << "M_name. _name is " << _name << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		if (cur_b.get_name()) BOOKDB.Del_name(cur_b.get_name(), cur_b.get_ISBN());
		BOOKDB.Add_name(_name, cur_b.get_ISBN());
		cur_b.set_name(_name);
		BOOKDB.modify(cur_b.get_ISBN(), cur_b);
		//puts("M_name done.");//???
		return true;
	}
	bool Modify_author(mystring<40> _auth)
	{
		//cout << "M_auth. _auth is " << _auth << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		if (cur_b.get_author()) BOOKDB.Del_author(cur_b.get_author(), cur_b.get_ISBN());
		BOOKDB.Add_author(_auth, cur_b.get_ISBN());
		cur_b.set_author(_auth);
		BOOKDB.modify(cur_b.get_ISBN(), cur_b);
		//puts("M_auth done.");//???
		return true;
	}
	bool Modify_keyword(mystring<40> _keyw)
	{
		//cout << "M_keyw. _keyw is " << _keyw << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		string str;
		mystring<40> t_keyw;
		int len = 0, p = 0;
		if (cur_b.get_keyword())
		{
			mystring<40> cur_keyw = cur_b.get_keyword();
			len = cur_keyw.get_len();
			p = 0;
			while (p < len)
			{
				str = "";
				while (cur_keyw[p] != '|' && p < len) str += cur_keyw[p], ++p;
				t_keyw = str;
				BOOKDB.Del_keyword(t_keyw, cur_b.get_ISBN());
				++p;
			}
		}
		len = _keyw.get_len();
		p = 0;
		while (p < len)
		{
			str = "";
			while (_keyw[p] != '|' && p < len) str += _keyw[p], ++p;
			t_keyw = str;
			//cout << "in Modiry_keyword we add a keyword " << t_keyw << endl;//???
			BOOKDB.Add_keyword(t_keyw, cur_b.get_ISBN());
			++p;
		}
		cur_b.set_keyword(_keyw);
		BOOKDB.modify(cur_b.get_ISBN(), cur_b);
		//puts("M_keyw done.");//???
		return true;
	}
	bool Modify_price(double _price)
	{
		//cout << "M_price. _price is " << _price << endl; //???
		if (cur_u.get_rt() < 3) Error;
		if (!cur_b.get_ISBN()) Error;
		cur_b.set_price(_price);
		BOOKDB.modify(cur_b.get_ISBN(), cur_b);
		//puts("M_price done.");//???
		return true;
	}

	bool Show_all()
	{
		//puts("Show_all!");//???
		if (cur_u.get_rt() < 1) Error;
		BOOKDB.showall();
		//puts("Show_all done.");//???
		return true;
	}
	bool Show_finance(int _time = INF_TIME)
	{
		//cout << "Show_finance. _time is " << _time << endl;//???
		//puts("Show_finance!");//???
		if (cur_u.get_rt() != 7) Error;
		FINANCEDB.show(_time);
		//puts("Show_finance done.");//???
		return true;
	}
	bool Show_isbn(mystring<20> _isbn)
	{
		//cout << "Show_isbn _isbn is " << _isbn << endl;//???
		if (cur_u.get_rt() < 1) Error;
		if (!BOOKDB.exist(_isbn)) return false;// Error;
		book tmpb = BOOKDB.find(_isbn);
		tmpb.out();
		//puts("Show_isbn done.");//???
		return true;
	}
	bool Show_name(mystring<40> _name)
	{
		//cout << "Show_name _name is " << _name << endl;//???
		if (cur_u.get_rt() < 1) Error;
		BOOKDB.showbyname(_name);
		//puts("Show_name done.");//???
		return true;
	}
	bool Show_author(mystring<40> _auth)
	{
		//cout << "Show_author _auth is " << _auth << endl;//???
		if (cur_u.get_rt() < 1) Error;
		BOOKDB.showbyauthor(_auth);
		//puts("Show_author done.");//???
		return true;
	}
	bool Show_keyword(mystring<40> _keyw)
	{
		//cout << "Show_keyword _keyw is " << _keyw << endl;//???
		if (cur_u.get_rt() < 1) Error;
		BOOKDB.showbykeyword(_keyw);
		//puts("Show_keyword done.");//???
		return true;
	}
};

bool exc::load(string _name)
{
	if (cur_u.get_rt() < 7) Error;
	fstream LOADFI(_name, fstream::in);
	string _log;
	int log_tot = 0;
	while (!LOADFI.eof())
	{
		++log_tot;
		getline(LOADFI, _log);
		execute(_log);
		//puts("really executed! fuck !");
		if (end_state)
		{
			LOADFI.close();
			//puts("really return! fuck !");
			return true;
		}
	}
	LOADFI.close();
	return true;
}
bool exc::execute(string _log)
{
	try
	{
		TokenScanner scanner;
		scanner.setInput(_log);
		if (!scanner.hasMoreTokens()) Error;
		string firsttoken = scanner.nextToken();
		bool LOG_CONDITION_FLAG = false;
		//operations on user
		//load
		if (firsttoken == "load")
		{
			if (!scanner.hasMoreTokens()) Error;
			string _name = scanner.nextToken();
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = load(_name);
			return LOG_CONDITION_FLAG;
		}
		//exit
		else if (firsttoken == "exit")
		{
			end_state = true;
			return true;
		}
		//su
		else if (firsttoken == "su")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring <30> _id = str;
			str = "";
			if (scanner.hasMoreTokens()) str = scanner.nextToken();
			mystring <30> _pwd = str;
			if (scanner.hasMoreTokens()) Error;
			//puts("start su");
			LOG_CONDITION_FLAG = su(_id, _pwd);
			//puts("end su");
			return LOG_CONDITION_FLAG;
		}
		//logout
		else if (firsttoken == "logout")
		{
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = logout();
			return LOG_CONDITION_FLAG;
		}
		//useradd
		else if (firsttoken == "useradd")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<30> _id = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			mystring<30> _pwd = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			if (!scanner.isNumber(str)) Error;
			int _rt = stoi(str);
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			mystring<20> _name = str;
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = useradd(_id, _pwd, _rt, _name);
			return LOG_CONDITION_FLAG;
		}
		//register
		else if (firsttoken == "register")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<30> _id = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			mystring<30> _pwd = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			mystring<20> _name = str;
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = regist(_id, _pwd, _name);
			return LOG_CONDITION_FLAG;
			
		}
		//delete
		else if (firsttoken == "delete")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<30> _id = str;
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = del(_id);
			return LOG_CONDITION_FLAG;
		}
		//passwd
		if (firsttoken == "passwd")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<30> _id = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			mystring<30> _pwd = str;
			if (!scanner.hasMoreTokens())
			{
				LOG_CONDITION_FLAG = passwd(_id, _pwd);
				return LOG_CONDITION_FLAG;
			}
			str = scanner.nextToken();
			mystring<30> __pwd = str;
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = passwd(_id, __pwd, _pwd);
			return LOG_CONDITION_FLAG;
		}

		//operations on books
		//select
		else if (firsttoken == "select")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<20> _isbn = str;
			if (scanner.hasMoreTokens()) Error;
			LOG_CONDITION_FLAG = select(_isbn);
			return LOG_CONDITION_FLAG;
		}
		//modify
		else if (firsttoken == "modify")
		{
			if (!scanner.hasMoreTokens()) Error;
			while (scanner.hasMoreTokens())
			{
				string str = scanner.nextToken();
				if (str != "-") Error;
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				//isbn
				if (str == "ISBN")
				{
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str != "=") Error;
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					mystring<20> _isbn = str;
					Modify_isbn(_isbn);
					continue;
				}
				//name
				else if (str == "name")
				{
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					//cout << "str = ? " << str << endl;
					if (str != "=") Error;
					//puts("===");//???
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str[0] != '"' || str[str.length() - 1] != '"') Error;
					//puts("''''''''''"); //???
					str.erase(str.length() - 1, 1);	str.erase(0, 1);
					mystring<40> _name = str;
					//cout << "modify_name " << _name << endl;//???
					Modify_name(_name);
					continue;
				}
				//author
				else if (str == "author")
				{
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str != "=") Error;
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str[0] != '"' || str[str.length() - 1] != '"') Error;
					str.erase(str.length() - 1, 1);	str.erase(0, 1);
					mystring<40> _auth = str;
					Modify_author(_auth);
					continue;
				}
				//keyword
				else if (str == "keyword")
				{
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str != "=") Error;
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str[0] != '"' || str[str.length() - 1] != '"') Error;
					str.erase(str.length() - 1, 1);	str.erase(0, 1);
					mystring<40> _keyw = str;
					Modify_keyword(_keyw);
					continue;
				}
				//price
				else if (str == "price")
				{
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (str != "=") Error;
					if (!scanner.hasMoreTokens()) Error;
					str = scanner.nextToken();
					if (!scanner.isNumber(str)) Error;
					double _price = stod(str);
					Modify_price(_price);
					continue;
				}
				else Error;
			}
			return true;
		}
		//import
		else if (firsttoken == "import")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			if (!scanner.isNumber(str)) Error;
			int _quantity = stoi(str);
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			if (!scanner.isNumber(str)) Error;
			double _price = stod(str);
			LOG_CONDITION_FLAG = imports(_quantity, _price);
			return LOG_CONDITION_FLAG;
		}
		//show
		else if (firsttoken == "show")
		{
			//all
			if (!scanner.hasMoreTokens())
			{
				LOG_CONDITION_FLAG = Show_all();
				return LOG_CONDITION_FLAG;
			}
			string str = scanner.nextToken();
			//finance
			if (str == "finance")
			{
				if (!scanner.hasMoreTokens()) LOG_CONDITION_FLAG = Show_finance();
				else
				{
					str = scanner.nextToken();
					if (!scanner.isNumber(str)) Error;
					int _time = stoi(str);
					LOG_CONDITION_FLAG = Show_finance(_time);
				}
				return LOG_CONDITION_FLAG;
			}
			else if (str != "-") Error;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			//isbn
			if (str == "ISBN")
			{
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str != "=") Error;
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				mystring<20> _isbn = str;
				LOG_CONDITION_FLAG = Show_isbn(_isbn);
				return LOG_CONDITION_FLAG;
			}
			//name
			else if (str == "name")
			{
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str != "=") Error;
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str[0] != '"' || str[str.length() - 1] != '"') Error;
				str.erase(str.length() - 1, 1); str.erase(0, 1);
				mystring<40> _name = str;
				LOG_CONDITION_FLAG = Show_name(_name);
				return LOG_CONDITION_FLAG;
			}
			//author
			else if (str == "author")
			{
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str != "=") Error;
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str[0] != '"' || str[str.length() - 1] != '"') Error;
				str.erase(str.length() - 1, 1); str.erase(0, 1);
				mystring<40> _auth = str;
				LOG_CONDITION_FLAG = Show_author(_auth);
				return LOG_CONDITION_FLAG;
			}
			//keyword
			else if (str == "keyword")
			{
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str != "=") Error;
				if (!scanner.hasMoreTokens()) Error;
				str = scanner.nextToken();
				if (str[0] != '"' || str[str.length() - 1] != '"') Error;
				str.erase(str.length() - 1, 1); str.erase(0, 1);
				mystring<40> _keyw = str;
				LOG_CONDITION_FLAG = Show_keyword(_keyw);
				return LOG_CONDITION_FLAG;
			}
			else Error;
			return false;
		}
		//buy
		else if (firsttoken == "buy")
		{
			if (!scanner.hasMoreTokens()) Error;
			string str = scanner.nextToken();
			mystring<20> _isbn = str;
			if (!scanner.hasMoreTokens()) Error;
			str = scanner.nextToken();
			if (!scanner.isNumber(str)) Error;
			int _quantity = stoi(str);
			double _price = 0;
			LOG_CONDITION_FLAG = buy(_isbn, _quantity, _price);
			return LOG_CONDITION_FLAG;
		}

		else Error;
		return false;
	}
	catch (...)
	{
		Error;
	}
}

#endif // !EXC_HPP
