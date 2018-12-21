#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
using std::fstream;


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
#include "exc_user.hpp"

int main()
{
	fstream CMD;
	CMD.open("user.bin");
	if (!CMD)
	{
		user_database USERDB;
		USERDB.ROOT();
	}
	CMD.close();
	CMD.open("command.txt", fstream::in);
	if (CMD)
	{
		exc EXECUTOR;
		EXECUTOR.init();
		//puts("start");
		EXECUTOR.execute("su root sjtu");
		//puts("really fucking started!");
		EXECUTOR.execute("load command.txt");
		//puts("pass");
	}
	else
	{
		CMEFORUSER::CMD_FOR_USER();
		//This part is for user.
	}
	//system("pause");
	return 0;
}