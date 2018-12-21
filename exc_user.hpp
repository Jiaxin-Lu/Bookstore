#ifndef EXC_USER_HPP
#define EXC_USER_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
using std::fstream;
using std::cin;
using std::getline;


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
#include "exc.hpp"

#include <conio.h>

namespace CMEFORUSER
{
	exc EXECUTOR;
	TokenScanner scanner;
	log_database LOGDB;
	int log_sz = 0;

/*
puts("您现在可以进行：0.退出系统 1.登录 2.注册 3.注销 4.修改密码 5.搜索书本信息 6.购买书本");
puts("7.增加用户 8.选择指定书 9.合并书本信息 10.显示全部书本信息 11.进书 12.查询自己的操作记录");
puts("13.显示财务 14.获取财务报告 15.获取员工记录 16.获取日志文件");
*/
	void LOG_add_done(string __exc)
	{
		++log_sz;
		mystring<200> _exc = __exc;
		logsave TMPLOG;
		TMPLOG.set_condition(DONE);
		TMPLOG.set_log(_exc);
		TMPLOG.set_logid(log_sz);
		TMPLOG.set_userid(EXECUTOR.cur_u_id());
		TMPLOG.set_financeflag(false);
		LOGDB.add(log_sz, TMPLOG);
		TMPLOG = 0;
		TMPLOG.set_logid(log_sz);
		LOGDB.modify(0, TMPLOG);
		return;
	}
	void LOG_add_done_fin(string __exc, double _price)
	{
		++log_sz;
		mystring<200> _exc = __exc;
		logsave TMPLOG;
		TMPLOG.set_condition(DONE);
		TMPLOG.set_log(_exc);
		TMPLOG.set_logid(log_sz);
		TMPLOG.set_userid(EXECUTOR.cur_u_id());
		TMPLOG.set_financeflag(true);
		TMPLOG.set_finance(_price);
		LOGDB.add(log_sz, TMPLOG);

		TMPLOG = 0;
		TMPLOG.set_logid(log_sz);
		LOGDB.modify(0, TMPLOG);
		return;
	}
	void LOG_add_invalid(string __exc)
	{
		++log_sz;
		mystring<200> _exc = __exc;
		logsave TMPLOG;
		TMPLOG.set_condition(INVALID);
		TMPLOG.set_log(_exc);
		TMPLOG.set_logid(log_sz);
		TMPLOG.set_userid(EXECUTOR.cur_u_id());
		TMPLOG.set_financeflag(false);
		LOGDB.add(log_sz, TMPLOG);

		TMPLOG = 0;
		TMPLOG.set_logid(log_sz);
		LOGDB.modify(0, TMPLOG);
		return;
	}
	void inputPassword(string &str, int sz)
	{
		char c;
		int count = 0;
		char *password = new char[sz + 1];
		c = getch();
		while (c != '\r')
		{
			if (c == 8)//back
			{
				if (count == 0)
				{
					c = getch();
					continue;
				}
				putchar('\b'); //back
				putchar(' '); //blankspace
				putchar('\b'); //back
				count--;
			}
			if (count == sz)
			{
				c = getch();
				continue;
			} //oversize
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
			{
				putchar('*');
				password[count] = c;
				count++;
			}
			c = getch();
		}
		password[count] = '\0';
		str = password;
		delete[] password;
		cout << endl;
	}
	void SU()
	{
		puts("登录...");
		printf("用户名：");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("密码 ：");
		inputPassword(_passwd, 30);
		string _exc = "su " + _id + " " + _passwd;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("登录成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("登录失败！");
		}
	}

	void REGIST()
	{
		puts("注册...");
		printf("用户名：");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("密码 ：");
		inputPassword(_passwd, 30);
		printf("昵称：");
		string _name;
		getline(cin, _name);
		string _exc = "register " + _id + " " + _passwd + " " + _name;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("注册成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("注册失败！");
		}
	}

	void LOGOUT()
	{
		string _exc = "logout";
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("已登出！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("错误操作！");
		}
	}

	void PASSWD()
	{
		puts("修改密码...");
		printf("用户名：");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("旧密码(若不输入请空置)：");
		inputPassword(_passwd, 30);
		string  _passwd_n = "";
		printf("新密码：");
		inputPassword(_passwd_n, 30);
		string _exc = "passwd " + _id + " " + _passwd + " " + _passwd_n;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("修改成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("修改失败！");
		}
	}

	void SHOW()
	{
		puts("您可选择按以下内容显示相关书本：1.ISBN 2.书名(name) 3.作者(author) 4.关键词(keyword)");
		printf("请输入一个数字选择您进行的操作: ");
		string _str;
		getline(cin, _str);
		while (!scanner.isNumber(_str))
		{
			printf("请输入一个数字选择您进行的操作: ");
			getline(cin, _str);
		}
		int _op = stoi(_str);
		puts("请按要求输入参数。输入不用带任何双引号，将匹配的图书以ISBN号排序输出。");
		_str = "";
		string _exc = "";
		if (_op == 1)
		{
			printf("ISBN：");
			getline(cin, _str);
			_exc = "";
			_exc = "show -ISBN=" + _str;
		}
		else if (_op == 2)
		{
			printf("名称(name)：");
			getline(cin, _str);
			_exc = "";
			_exc = "show -name=\"" + _str + "\"";
		}
		else if (_op == 3)
		{
			printf("作者(author)：");
			getline(cin, _str);
			_exc = "";
			_exc = "show -author=\"" + _str + "\"";
			//cout << _exc << endl;
			//cout << _str << endl;
		}
		else if (_op == 4)
		{
			printf("关键词(keyword)(仅支持单关键词)：");
			getline(cin, _str);
			_exc = "";
			_exc = "show -keyword=\"" + _str + "\"";
		}
		else
		{
			puts("错误输入！");
			return;
		}
		//cout << _exc << endl; //???
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("不予显示！");
		}
		return;
		//	show ‐ISBN = [ISBN] ‐name = [name] ‐author = [author] ‐keyword = [keyword]
	}

	void BUY()
	{
		puts("购买图书...");
		string null_str = "";
		printf("ISBN：");
		string _isbn;
		getline(cin, _isbn);
		if (_isbn == null_str)
		{
			puts("输入错误！");
			return;
		}
		printf("数量：");
		string _quan;
		getline(cin, _quan);
		if (!scanner.isNumber(_quan))
		{
			puts("数量错误");
			return;
		}
		string _exc = "buy " + _isbn + " " + _quan;
		double _price = 0;
		bool SUCESS = EXECUTOR.buy(_isbn, stoi(_quan), _price);
		if (SUCESS)
		{
			LOG_add_done_fin(_exc, _price);
			puts("购买成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("购买失败！");
		}
	}

	void USERADD()
	{
		printf("用户名：");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("密码 ：");
		inputPassword(_passwd, 30);
		printf("权限 ：");
		string _rt;
		getline(cin, _rt);
		printf("昵称：");
		string _name;
		getline(cin, _name);
		string _exc = "useradd " + _id + " " + _passwd + " " + _rt + " " + _name;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("添加成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("添加失败！");
		}

	}

	void SELECT()
	{
		puts("选择图书...");
		printf("请输入图书的 ISBN：");
		string _isbn;
		getline(cin, _isbn);
		string _exc = "select " + _isbn;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("选择成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("选择失败！");
		}
	}

	void MODIFY()
	{
		cout << "您正在修改ISBN为 " << EXECUTOR.cur_b_isbn() << " 的图书信息..." << endl;
		puts("更改信息（如不操作请按回车）（请不要加双引号）：");
		string nullstr = "";
		string _exc = "modify ";
		printf("ISBN：");
		string _isbn = "";
		getline(cin, _isbn);
		if (!(_isbn == nullstr)) _exc = _exc + "-ISBN=" + _isbn;
		printf("名称(name)：");
		string _name = "";
		getline(cin, _name);
		if (!(_name == nullstr)) _exc = _exc + "-name=\"" + _name + "\"";
		printf("作者(author)：");
		string _auth = "";
		getline(cin, _auth);
		if (!(_auth == nullstr)) _exc = _exc + "-author=\"" + _auth + "\"";
		puts("关键词(keyword)(不同关键词间请用|隔开)：");
		string _keyword = "";
		getline(cin, _keyword);
		if (!(_keyword == nullstr)) _exc = _exc + "-keyword=\"" + _keyword + "\"";
		printf("价格(price)：");
		string _price = "";
		getline(cin, _price);
		if (!(_price == nullstr)) _exc = _exc + "-price=" + _price;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("更新成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("更新失败！");
		}		
		
		//modify ‐ISBN = [ISBN] ‐name = [name] ‐author = [author] ‐keyword = [keyword] ‐price = [price]
	}

	void SHOW_ALL()
	{
		string _exc = "show";
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("不予显示！");
		}
	}

	void IMPORT()
	{
		puts("进书...");
		printf("数量：");
		string _quan;
		getline(cin, _quan);
		if (!scanner.isNumber(_quan))
		{
			puts("数量错误");
			return;
		}
		printf("总价：");
		string _price;
		getline(cin, _price);
		if (!scanner.isNumber(_price))
		{
			puts("数量错误");
			return;
		}
		string _exc = "import " + _quan + " " + _price;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done_fin(_exc, -stod(_price));
			puts("进书成功！");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("进书失败！");
		}

	}
	void REPORT_MYSELF()
	{
		LOGDB.report_myself(EXECUTOR.cur_u_id());
	}

	void SHOW_FINANCE()
	{
		puts("查看财务...");
		printf("请输入您要查看的次数(若想查看所有请空置)：");
		string _times;
		getline(cin, _times);
		string null_str = "";
		string _exc;
		if (_times == null_str) _exc = "show finance";
		else _exc = "show finance " + _times;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("不予显示！");
		}
	}

	void REPORT_FINANCE()
	{
		bool SUCESS = EXECUTOR.report_finance();
		if (SUCESS)
		{
			puts("已生成财务报表.");
		}
		else
		{
			puts("不予显示！");
		}
	}

	void REPORT_EMPLOYEE()
	{
		LOGDB.report_employee();
	}

	void LOGLOG()
	{
		LOGDB.LOG_LOG();
	}


	void OPERATE(int _op)
	{
		switch (_op)
		{
		case 1: SU(); break;
		case 2: REGIST(); break;
		case 3: LOGOUT(); break;
		case 4: PASSWD(); break;
		case 5: SHOW(); break;
		case 6: BUY(); break;
		case 7: USERADD(); break;
		case 8: SELECT(); break;
		case 9: MODIFY(); break;
		case 10: SHOW_ALL(); break;
		case 11: IMPORT(); break;
		case 12: REPORT_MYSELF(); break;
		case 13: SHOW_FINANCE(); break;
		case 14: REPORT_FINANCE(); break;
		case 15: REPORT_EMPLOYEE(); break;
		case 16: LOGLOG(); break;
		default: puts("错误操作！"); break;
		}
		return;
	}

	void CMD_FOR_USER()
	{
		EXECUTOR.init();
		LOGDB.init();
		logsave LOG_TMPP = LOGDB.find(0);
		log_sz = LOG_TMPP.get_logid();
		puts("WELCOME TO BOOKSTORE SYSTEM!");
		puts("欢迎来到图书管理系统！");
		while (true)
		{
			puts("");
			int _curu_rt = EXECUTOR.cur_u_rt();
			if (_curu_rt == 0)
			{
				puts("您现在可以进行：0.退出系统 1.登录 2.注册");
				printf("请输入一个数字选择您进行的操作: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("请输入一个数字选择您进行的操作: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("确认退出系统？ y/n : ");
					string _str;
					getline(cin, _str);
					if (_str[0] == 'y')
					{
						puts("");
						return;
					}
					else continue;
				}
				if (_op > 2)
				{
					puts("错误操作！");
					continue;
				}
				else
				{
					OPERATE(_op);
					continue;
				}
				continue;
			}
			else if (_curu_rt == 1)
			{
				puts("您现在可以进行：0.退出系统 1.登录 2.注册 3.注销 4.修改密码 5.搜索书本信息 6.购买书本");
				printf("请输入一个数字选择您进行的操作: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("请输入一个数字选择您进行的操作: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("确认退出系统？ y/n : ");
					string _str;
					getline(cin, _str);
					if (_str[0] == 'y')
					{
						puts("");
						return;
					}
					else continue;
				}
				if (_op > 6)
				{
					puts("错误操作！");
					continue;
				}
				else
				{
					OPERATE(_op);
					continue;
				}
				continue;
			}
			else if (_curu_rt == 3)
			{
				puts("您现在可以进行：0.退出系统 1.登录 2.注册 3.注销 4.修改密码 5.搜索书本信息 6.购买书本");
				puts("7.增加用户 8.选择指定书 9.合并书本信息 10.显示全部书本信息 11.进书 12.查询自己的操作记录");
				printf("请输入一个数字选择您进行的操作: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("请输入一个数字选择您进行的操作: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("确认退出系统？ y/n : ");
					string _str;
					getline(cin, _str);
					if (_str[0] == 'y')
					{
						puts("");
						return;
					}
					else continue;
				}
				if (_op > 12)
				{
					puts("错误操作！");
					continue;
				}
				else
				{
					OPERATE(_op);
					continue;
				}
				continue;
			}
			else if (_curu_rt == 7)
			{
				puts("您现在可以进行：0.退出系统 1.登录 2.注册 3.注销 4.修改密码 5.搜索书本信息 6.购买书本");
				puts("7.增加用户 8.选择指定书 9.合并书本信息 10.显示全部书本信息 11.进书 12.查询自己的操作记录");
				puts("13.显示财务 14.获取财务报告 15.获取员工记录 16.获取日志文件");
				printf("请输入一个数字选择您进行的操作: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("请输入一个数字选择您进行的操作: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("确认退出系统？ y/n : ");
					string _str;
					getline(cin, _str);
					if (_str[0] == 'y')
					{
						puts("");
						return;
					}
					else continue;
				}
				if (_op > 16)
				{
					puts("错误操作！");
					continue;
				}
				else
				{
					OPERATE(_op);
					continue;
				}
				continue;
			}
		}
	}
}


#endif // !EXC_USER_HPP


