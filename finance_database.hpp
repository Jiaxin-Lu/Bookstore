#ifndef FINANCEDATABASE_HPP
#define FINANCEDATABASE_HPP

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

class finance_database
{
private:
	database<int, double> financedata;
public:
	void init()
	{
		financedata.initialize("finance");
		financedata.Add(0, 2);
		financedata.Add(1, 0);
		financedata.Add(2, 0);
	}
	void Out(double _price)
	{
		int sz = (int)financedata.Find(0);
		++sz;
		double _out = financedata.Find(2);
		financedata.Modify(2, _out + _price);
		financedata.Add(sz, -_price);
		financedata.Modify(0, sz);
		return;
	}
	void In(double _price)
	{
		int sz = (int)financedata.Find(0);
		++sz;
		double _in = financedata.Find(1);
		financedata.Modify(1, _in + _price);
		financedata.Add(sz, _price);
		financedata.Modify(0, sz);
		return;
	}
	void show(int _time)
	{
		int tot_time = (int)financedata.Find(0);
		double _in = 0, _out = 0;
		if (_time >= tot_time - 2)
		{
			_in = financedata.Find(1);
			_out = financedata.Find(2);
		}
		else
		{
			for (int i = tot_time; i >= tot_time - _time + 1; --i)
			{
				double ret = financedata.Find(i);
				if (ret > 0) _in += ret; else _out -= ret;
			}
		}
		printf("+ %.2lf - %.2lf\n", _in, _out);
		return;
	}
	void report()
	{
		int tot_time = (int)financedata.Find(0);
		fstream fi;
		fi.open("Finance_report.csv", fstream::out);
		fi << "Option_id,In,Out,Money_Remain" << endl;
		double _in = financedata.Find(1), _out = financedata.Find(2);
		double _remain = 0;
		for (int i = 3; i <= tot_time; ++i)
		{
			double ret = financedata.Find(i);
			_remain += ret;
			fi.precision(0);
			fi << i-2 << ',';
			if (ret > 0)
			{
				fi << ret << ",,";
			}
			else
			{
				fi << ',' << -ret << ',';
			}
			fi << _remain << endl;
		}
		fi << "×Ü¼Æ," << _in << ',' << _out << ',' << _in - _out << endl;
		fi.close();
		return;
	}
};

#endif // !FINANCEDATABASE_HPP
