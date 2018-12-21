#ifndef DATABASEN_HPP
#define DATABASEN_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstdlib>
using std::fstream;
using std::string;
using std::cout;
using std::endl;
using std::ios;


#include "database.hpp"

#define re reinterpret_cast<char * >
#define INF_MAXID 0x3f3f3f3f
const int MAX_SZ_ = 50;

template<class T, class U>
class database_n
{
private:
	mystring<30> name;
	int num; // num kind of *

	struct BLOCK
	{
		T id[MAX_SZ_ + 1];
		T maxid = INF_MAXID;
		database<U, U> info[MAX_SZ_ + 1];

		int pre = 0, ne = 0;
		int sz = 0;
	};
public:
	string getName() { return name; }
	void setName(string _name) { name = _name; }
	void initialize(string _name)
	{
		setName(_name);
		fstream fi;
		fi.open(_name + ".bin", fstream::in);
		if (!fi)
		{
			fi.open(_name + ".bin", fstream::out | fstream::trunc | fstream::binary);
			int sz = 1;
			fi.write(re(&sz), sizeof(sz));
			sz = 1;
			fi.write(re(&sz), sizeof(sz));
			num = 0;
			fi.write(re(&num), sizeof(sz));
			BLOCK bl;
			fi.write(re(&bl), sizeof(BLOCK));
			fi.close();
		}
	}

	void Add(T _id, U _info)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::out | fstream::binary);
		int sz = 1;
		fi.read(re(&sz), sizeof(sz));
		int now = 1;
		fi.read(re(&now), sizeof(now));
		fi.read(re(&num), sizeof(num));

		BLOCK bnow;
		while (now)
		{
			fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(bnow));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;
			}

			for (int i=0;i<=bnow.sz;++i)
				if (_id == bnow.id[i])
				{
					bnow.info[i].Add(_info, _info);
					return;
				}

			for (int i = 0; i <= bnow.sz; ++i)
				if (_id < bnow.id[i] || i == bnow.sz)
				{
					for (int k = bnow.sz; k > i; --k)
					{
						bnow.id[k] = bnow.id[k - 1];
						bnow.info[k] = bnow.info[k - 1];
					}
					bnow.id[i] = _id;

					++num;
					fi.seekp(sizeof(int) * 2, ios::beg);
					fi.write(re(&num), sizeof(int));
					string _num = std::to_string(num);
					bnow.info[i].initialize(_name + _num);
					bnow.info[i].Add(_info, _info);

					if (i == 0 && bnow.pre) //update pre block's maxnumber
					{
						BLOCK bpre;
						fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.read(re(&bpre), sizeof(bpre));
						bpre.maxid = _id;
						fi.seekp(sizeof(int) * 3 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.write(re(&bpre), sizeof(bpre));
					}

					++bnow.sz;
					if (bnow.sz >= MAX_SZ_) //if this block is oversized
					{
						BLOCK bne;
						int hsz = bnow.sz / 2 + 1;
						for (int j = hsz; j < bnow.sz; ++j)
						{
							bne.id[j - hsz] = bnow.id[j];
							bne.info[j - hsz] = bnow.info[j];
						}
						++sz;

						bne.sz = bnow.sz - hsz;
						bnow.sz = hsz;
						bne.maxid = bnow.maxid;
						bnow.maxid = bne.id[0];
						bne.pre = now;
						bne.ne = bnow.ne;
						bnow.ne = sz;

						if (bne.ne)
						{
							BLOCK bnene;
							fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(bne.ne - 1), ios::beg);
							fi.read(re(&bnene), sizeof(bnene));
							bnene.pre = sz;
							fi.seekp(sizeof(int) * 3 + sizeof(BLOCK)*(bne.ne - 1), ios::beg);
							fi.write(re(&bnene), sizeof(bnene));
						}

						fi.seekp(sizeof(int) * 3 + sizeof(BLOCK)*(sz - 1), ios::beg);
						fi.write(re(&bne), sizeof(bne));
						fi.seekp(0, ios::beg);
						fi.write(re(&sz), sizeof(sz));
					}
					fi.seekp(sizeof(int) * 3 + sizeof(BLOCK)*(now - 1), ios::beg);
					fi.write(re(&bnow), sizeof(bnow));
					fi.close();
					return;
				}
		}
		fi.close();
		return;
	}

	void Del(T _id, U _info)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::out | fstream::binary);
		int sz = 1, now = 1;
		fi.read(re(&sz), sizeof(sz));
		fi.read(re(&now), sizeof(now));
		fi.read(re(&num), sizeof(num));

		BLOCK bnow;
		while (now)
		{
			fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(BLOCK));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;
			}
			for (int i=0;i<bnow.sz;++i)
				if (_id == bnow.id[i])
				{
					bnow.info[i].Del(_info);
					if (i == 0 && bnow.pre)
					{
						BLOCK bpre;
						fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.read(re(&bpre), sizeof(BLOCK));
						bpre.maxid = bnow.id[0];
						fi.seekp(sizeof(int) * 3 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.write(re(&bpre), sizeof(BLOCK));
					}
					fi.close();
					return;
				}
		}
		fi.close();
		return;
	}

	void Modify(T _id, U _info, U _info_n)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::out | fstream::binary);
		int sz = 1;
		fi.read(re(&sz), sizeof(sz));
		int now = 1;
		fi.read(re(&now), sizeof(now));
		fi.read(re(&num), sizeof(num));

		BLOCK bnow;
		while (now)
		{
			fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(BLOCK));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;				
			}
			
			for (int i = 0; i < bnow.sz; ++i)
				if (_id == bnow.id[i])
				{
					bnow.info[i].Del(_info);
					bnow.info[i].Add(_info, _info);
					break;
				}
			fi.close();
			return;
		}
		fi.close();
		return;		
	}

	typename database<U,U>::iterator Find(T _id)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::binary);
		int _sz;
		fi.read(re(&_sz), sizeof(int));
		int now;
		fi.read(re(&now), sizeof(int));
		fi.read(re(&num), sizeof(int));

		BLOCK bnow;
		auto ret = bnow.info[0].it_null();
		while (now)
		{
			fi.seekg(sizeof(int) * 3 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(BLOCK));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;
			}

			for (int i=0;i<bnow.sz;++i)
				if (_id == bnow.id[i])
				{
					ret = bnow.info[i].begin();
					fi.close();
					return ret;
				}
			fi.close();
			return ret;
		}
		fi.close();
		return ret;
	}
};


#endif // !DATABASEN_HPP
