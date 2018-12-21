#ifndef LOGDATABASE_HPP
#define LOGDATABASE_HPP

#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
using std::string;
using std::fstream;

#include "database.hpp"
#include "database_n.hpp"
#include "mystring.hpp"
#include "logsave.hpp"

const double eps = 1e-8;

class log_database
{
private:
	database<int, logsave> logdata;
	database_n<mystring<30>, int> lognamedata;
public:
	void init()
	{
		/*fstream fi;
		fi.open("log.bin", fstream::in | fstream::out | fstream::trunc);
		fi.close();*/
		logdata.initialize("log");
		lognamedata.initialize("log_name");
		logdata.Add(0, 0);
	}
	void add(int _id, logsave _data)
	{
		logdata.Add(_id, _data);
		lognamedata.Add(_data.get_userid(), _id);
	}
	logsave find(int _id)
	{
		return logdata.Find(_id);
	}
	void modify(int _id, logsave _data)
	{
		logdata.Modify(_id, _data);
	}
	void report_myself(mystring<30> _userid)
	{
		cout << "Your id is : " << _userid << " and your log histories are:" << endl;
		//puts("Your log history:");
		vector<int> _id;
		auto _it = lognamedata.Find(_userid);
		for (; *_it; ++_it) _id.push_back(*_it);
		sort(_id.begin(), _id.end());
		logsave _data;
		for (auto it = _id.begin(); it != _id.end(); ++it)
		{
			_data = logdata.Find(*it);
			_data.out_screen();
		}
		return;
	}
	void report_employee()
	{
		fstream fi;
		fi.open("employee report.csv", fstream::out);
		fi << "Log_id,User_id,Log" << endl;
		auto _it = logdata.begin();
		for (; _it; ++_it) if ((*_it).get_logid() != 0) fi << (*_it) << endl;
		fi.close();
		return;
		//unfinished
	}
	void LOG_LOG()
	{
		fstream fi;
		fi.open("LOG.log", fstream::out);
		auto _it = logdata.begin();
		for (; _it; ++_it)
		if ((*_it).get_logid() != 0)
		{
			fi << (*_it) << endl;
			if ((*_it).get_financeflag())
			{
				double _ret = (*_it).get_finance();
				if (abs(_ret - 0) > eps)
				{
					fi << "FINANCE CHANGE = ";
					if (_ret > 0)
					{
						fi << '+' <<  _ret << endl;
					}
					else
					{
						fi << '-' <<  -_ret << endl;
						//printf("- %.2lf\b", -_ret);
					}
				}
			}
		}
		fi.close();
		return;
	}
};

#endif // !LOGDATABASE_HPP
