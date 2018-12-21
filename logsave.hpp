#ifndef LOGSAVE_HPP
#define LOGSAVE_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using std::string;
using std::cout;
using std::endl;

#include "mystring.hpp"

enum LOG_CONDITION
{
	ERROR = 0, DONE, INVALID
};

class logsave
{
private:
	int log_id;
	mystring<30> user_id;
	mystring<200> _log;
	LOG_CONDITION log_condition;
	bool finance_flag = false;
	double _finance;
public:
	logsave(int _t = 0)
	{
		log_id = 0;
		user_id = 0;
		_log = 0;
		log_condition = ERROR;
		finance_flag = false;
		_finance = 0;
	}
	void set_logid(int _id) { log_id = _id; }
	void set_userid(mystring<30> _id) { user_id = _id; }
	void set_log(mystring<200> __log) { _log = __log; }
	void set_condition(LOG_CONDITION _con) { log_condition = _con; }
	void set_financeflag(bool _flag) { finance_flag = _flag; }
	void set_finance(double _fina) { _finance = _fina; }

	int get_logid() { return log_id; }
	mystring<30> get_userid() { return user_id; }
	LOG_CONDITION get_condition() { return log_condition; }
	mystring<200> get_log() { return _log; }
	bool get_financeflag() { return finance_flag; }
	double get_finance() { return _finance; }

	void out_screen()
	{
		if (log_id <= 0) return;
		cout << log_id << '\t' << user_id << '\t' << _log << '\t';
		if (log_condition == DONE) cout << "DONE";
		else if (log_condition == INVALID) cout << "INVALID";
		cout << endl;
		return;
	}

	friend ostream& operator<< (ostream & os, logsave & obj);
};

ostream& operator<< (ostream & os, logsave & obj)
{
	if (obj.log_id <= 0) return os;
	os << obj.log_id << ',' << obj.user_id << ',' << obj._log << ',';
	if (obj.log_condition == DONE) os << "DONE";
	else if (obj.log_condition == INVALID) os << "INVALID";
	return os;
}

#endif // !LOGSAVE_HPP
