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
puts("�����ڿ��Խ��У�0.�˳�ϵͳ 1.��¼ 2.ע�� 3.ע�� 4.�޸����� 5.�����鱾��Ϣ 6.�����鱾");
puts("7.�����û� 8.ѡ��ָ���� 9.�ϲ��鱾��Ϣ 10.��ʾȫ���鱾��Ϣ 11.���� 12.��ѯ�Լ��Ĳ�����¼");
puts("13.��ʾ���� 14.��ȡ���񱨸� 15.��ȡԱ����¼ 16.��ȡ��־�ļ�");
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
		puts("��¼...");
		printf("�û�����");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("���� ��");
		inputPassword(_passwd, 30);
		string _exc = "su " + _id + " " + _passwd;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("��¼�ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("��¼ʧ�ܣ�");
		}
	}

	void REGIST()
	{
		puts("ע��...");
		printf("�û�����");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("���� ��");
		inputPassword(_passwd, 30);
		printf("�ǳƣ�");
		string _name;
		getline(cin, _name);
		string _exc = "register " + _id + " " + _passwd + " " + _name;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("ע��ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("ע��ʧ�ܣ�");
		}
	}

	void LOGOUT()
	{
		string _exc = "logout";
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("�ѵǳ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("���������");
		}
	}

	void PASSWD()
	{
		puts("�޸�����...");
		printf("�û�����");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("������(�������������)��");
		inputPassword(_passwd, 30);
		string  _passwd_n = "";
		printf("�����룺");
		inputPassword(_passwd_n, 30);
		string _exc = "passwd " + _id + " " + _passwd + " " + _passwd_n;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("�޸ĳɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("�޸�ʧ�ܣ�");
		}
	}

	void SHOW()
	{
		puts("����ѡ������������ʾ����鱾��1.ISBN 2.����(name) 3.����(author) 4.�ؼ���(keyword)");
		printf("������һ������ѡ�������еĲ���: ");
		string _str;
		getline(cin, _str);
		while (!scanner.isNumber(_str))
		{
			printf("������һ������ѡ�������еĲ���: ");
			getline(cin, _str);
		}
		int _op = stoi(_str);
		puts("�밴Ҫ��������������벻�ô��κ�˫���ţ���ƥ���ͼ����ISBN�����������");
		_str = "";
		string _exc = "";
		if (_op == 1)
		{
			printf("ISBN��");
			getline(cin, _str);
			_exc = "";
			_exc = "show -ISBN=" + _str;
		}
		else if (_op == 2)
		{
			printf("����(name)��");
			getline(cin, _str);
			_exc = "";
			_exc = "show -name=\"" + _str + "\"";
		}
		else if (_op == 3)
		{
			printf("����(author)��");
			getline(cin, _str);
			_exc = "";
			_exc = "show -author=\"" + _str + "\"";
			//cout << _exc << endl;
			//cout << _str << endl;
		}
		else if (_op == 4)
		{
			printf("�ؼ���(keyword)(��֧�ֵ��ؼ���)��");
			getline(cin, _str);
			_exc = "";
			_exc = "show -keyword=\"" + _str + "\"";
		}
		else
		{
			puts("�������룡");
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
			puts("������ʾ��");
		}
		return;
		//	show �\ISBN = [ISBN] �\name = [name] �\author = [author] �\keyword = [keyword]
	}

	void BUY()
	{
		puts("����ͼ��...");
		string null_str = "";
		printf("ISBN��");
		string _isbn;
		getline(cin, _isbn);
		if (_isbn == null_str)
		{
			puts("�������");
			return;
		}
		printf("������");
		string _quan;
		getline(cin, _quan);
		if (!scanner.isNumber(_quan))
		{
			puts("��������");
			return;
		}
		string _exc = "buy " + _isbn + " " + _quan;
		double _price = 0;
		bool SUCESS = EXECUTOR.buy(_isbn, stoi(_quan), _price);
		if (SUCESS)
		{
			LOG_add_done_fin(_exc, _price);
			puts("����ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("����ʧ�ܣ�");
		}
	}

	void USERADD()
	{
		printf("�û�����");
		string _id;
		getline(cin, _id);
		string _passwd = "";
		printf("���� ��");
		inputPassword(_passwd, 30);
		printf("Ȩ�� ��");
		string _rt;
		getline(cin, _rt);
		printf("�ǳƣ�");
		string _name;
		getline(cin, _name);
		string _exc = "useradd " + _id + " " + _passwd + " " + _rt + " " + _name;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("��ӳɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("���ʧ�ܣ�");
		}

	}

	void SELECT()
	{
		puts("ѡ��ͼ��...");
		printf("������ͼ��� ISBN��");
		string _isbn;
		getline(cin, _isbn);
		string _exc = "select " + _isbn;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("ѡ��ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("ѡ��ʧ�ܣ�");
		}
	}

	void MODIFY()
	{
		cout << "�������޸�ISBNΪ " << EXECUTOR.cur_b_isbn() << " ��ͼ����Ϣ..." << endl;
		puts("������Ϣ���粻�����밴�س������벻Ҫ��˫���ţ���");
		string nullstr = "";
		string _exc = "modify ";
		printf("ISBN��");
		string _isbn = "";
		getline(cin, _isbn);
		if (!(_isbn == nullstr)) _exc = _exc + "-ISBN=" + _isbn;
		printf("����(name)��");
		string _name = "";
		getline(cin, _name);
		if (!(_name == nullstr)) _exc = _exc + "-name=\"" + _name + "\"";
		printf("����(author)��");
		string _auth = "";
		getline(cin, _auth);
		if (!(_auth == nullstr)) _exc = _exc + "-author=\"" + _auth + "\"";
		puts("�ؼ���(keyword)(��ͬ�ؼ��ʼ�����|����)��");
		string _keyword = "";
		getline(cin, _keyword);
		if (!(_keyword == nullstr)) _exc = _exc + "-keyword=\"" + _keyword + "\"";
		printf("�۸�(price)��");
		string _price = "";
		getline(cin, _price);
		if (!(_price == nullstr)) _exc = _exc + "-price=" + _price;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done(_exc);
			puts("���³ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("����ʧ�ܣ�");
		}		
		
		//modify �\ISBN = [ISBN] �\name = [name] �\author = [author] �\keyword = [keyword] �\price = [price]
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
			puts("������ʾ��");
		}
	}

	void IMPORT()
	{
		puts("����...");
		printf("������");
		string _quan;
		getline(cin, _quan);
		if (!scanner.isNumber(_quan))
		{
			puts("��������");
			return;
		}
		printf("�ܼۣ�");
		string _price;
		getline(cin, _price);
		if (!scanner.isNumber(_price))
		{
			puts("��������");
			return;
		}
		string _exc = "import " + _quan + " " + _price;
		bool SUCESS = EXECUTOR.execute(_exc);
		if (SUCESS)
		{
			LOG_add_done_fin(_exc, -stod(_price));
			puts("����ɹ���");
		}
		else
		{
			LOG_add_invalid(_exc);
			puts("����ʧ�ܣ�");
		}

	}
	void REPORT_MYSELF()
	{
		LOGDB.report_myself(EXECUTOR.cur_u_id());
	}

	void SHOW_FINANCE()
	{
		puts("�鿴����...");
		printf("��������Ҫ�鿴�Ĵ���(����鿴���������)��");
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
			puts("������ʾ��");
		}
	}

	void REPORT_FINANCE()
	{
		bool SUCESS = EXECUTOR.report_finance();
		if (SUCESS)
		{
			puts("�����ɲ��񱨱�.");
		}
		else
		{
			puts("������ʾ��");
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
		default: puts("���������"); break;
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
		puts("��ӭ����ͼ�����ϵͳ��");
		while (true)
		{
			puts("");
			int _curu_rt = EXECUTOR.cur_u_rt();
			if (_curu_rt == 0)
			{
				puts("�����ڿ��Խ��У�0.�˳�ϵͳ 1.��¼ 2.ע��");
				printf("������һ������ѡ�������еĲ���: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("������һ������ѡ�������еĲ���: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("ȷ���˳�ϵͳ�� y/n : ");
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
					puts("���������");
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
				puts("�����ڿ��Խ��У�0.�˳�ϵͳ 1.��¼ 2.ע�� 3.ע�� 4.�޸����� 5.�����鱾��Ϣ 6.�����鱾");
				printf("������һ������ѡ�������еĲ���: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("������һ������ѡ�������еĲ���: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("ȷ���˳�ϵͳ�� y/n : ");
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
					puts("���������");
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
				puts("�����ڿ��Խ��У�0.�˳�ϵͳ 1.��¼ 2.ע�� 3.ע�� 4.�޸����� 5.�����鱾��Ϣ 6.�����鱾");
				puts("7.�����û� 8.ѡ��ָ���� 9.�ϲ��鱾��Ϣ 10.��ʾȫ���鱾��Ϣ 11.���� 12.��ѯ�Լ��Ĳ�����¼");
				printf("������һ������ѡ�������еĲ���: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("������һ������ѡ�������еĲ���: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("ȷ���˳�ϵͳ�� y/n : ");
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
					puts("���������");
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
				puts("�����ڿ��Խ��У�0.�˳�ϵͳ 1.��¼ 2.ע�� 3.ע�� 4.�޸����� 5.�����鱾��Ϣ 6.�����鱾");
				puts("7.�����û� 8.ѡ��ָ���� 9.�ϲ��鱾��Ϣ 10.��ʾȫ���鱾��Ϣ 11.���� 12.��ѯ�Լ��Ĳ�����¼");
				puts("13.��ʾ���� 14.��ȡ���񱨸� 15.��ȡԱ����¼ 16.��ȡ��־�ļ�");
				printf("������һ������ѡ�������еĲ���: ");
				string _str;
				getline(cin, _str);
				while (!scanner.isNumber(_str))
				{
					printf("������һ������ѡ�������еĲ���: ");
					getline(cin, _str);
				}
				int _op = stoi(_str);
				if (_op == 0)
				{
					printf("ȷ���˳�ϵͳ�� y/n : ");
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
					puts("���������");
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


