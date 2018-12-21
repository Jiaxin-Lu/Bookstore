#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
using std::fstream;
using std::string;
using std::ios;

#include "mystring.hpp"

#define re reinterpret_cast<char * >
#define INF_MAXID 0x3f3f3f3f
const int MAX_SZ = 50;
template <class T, class U>
class database
{
private:
	mystring<30> name;

	struct BLOCK
	{
		T id[MAX_SZ + 1];
		T maxid = INF_MAXID;
		U info[MAX_SZ + 1];

		int pre = 0, ne = 0;
		int sz = 0;
	};

public:
	string getName() { return name; }
	void setName(string _name) { name = _name; }
/*
* In a file:
* first tow integer one is "sz" represent how many blocks in this database
* another one is "now" represent the beginning block of this database
* then followed all the datatbase
*/

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
		//cout << "in add. _id is " << _id << endl; ???

		BLOCK bnow;
		while (now)
		{
			//puts("in while");
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(bnow));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;
			}
			
			for (int i = 0; i <= bnow.sz; ++i)
				if (_id < bnow.id[i] || i == bnow.sz)
				{
					//puts("adding");
					for (int k = bnow.sz; k > i; --k)
					{
						bnow.id[k] = bnow.id[k - 1];
						bnow.info[k] = bnow.info[k - 1];
					}
					bnow.id[i] = _id;
					bnow.info[i] = _info;
					//puts("add");
					//cout << bnow.id[i] << endl;

					if (i == 0 && bnow.pre) //update pre block's maxnumber
					{
						BLOCK bpre;
						fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.read(re(&bpre), sizeof(bpre));
						bpre.maxid = _id;
						fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.write(re(&bpre), sizeof(bpre));
						//puts("add pre");
					}

					++bnow.sz;
					//puts("add sz");
					//printf("now : %d, bnow.sz : %d\n", now, bnow.sz);
					if (bnow.sz >= MAX_SZ) //if this block is oversized
					{
						//puts("over size");
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
							fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bne.ne - 1), ios::beg);
							fi.read(re(&bnene), sizeof(bnene));
							bnene.pre = sz;
							fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bne.ne - 1), ios::beg);
							fi.write(re(&bnene), sizeof(bnene));
						}

						fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(sz - 1), ios::beg);
						fi.write(re(&bne), sizeof(bne));
						fi.seekp(0, ios::beg);
						fi.write(re(&sz), sizeof(sz));
					}
					fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
					fi.write(re(&bnow), sizeof(bnow));
					//printf("now : %d, bnow.sz : %d\n", now, bnow.sz);
					//puts("write done");
					fi.close();
					return;
				}
		}
		fi.close();
		return;
	}

	void Del(T _id)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::out | fstream::binary);
		int sz = 1;
		fi.read(re(&sz), sizeof(sz));
		int now = 1;
		fi.read(re(&now), sizeof(now));

		BLOCK bnow;
		//cout << "in dellllll. _id is " << _id << "now is " << now << endl; //???
		while (now)
		{
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(bnow));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				//puts("del next always !!!!!!!!!!!!!"); //???
				continue;
			}

			for (int i = 0; i < bnow.sz; ++i)
				if (_id == bnow.id[i])
				{
					//puts("in delete !!!!!!!!!!!!!"); //???
					for (int k = i; k < bnow.sz; ++k)
					{
						bnow.id[k] = bnow.id[k + 1];
						bnow.info[k] = bnow.info[k + 1];
					}
					--bnow.sz;
					
					if (i == 0 && bnow.pre)
					{
						BLOCK bpre;
						fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.read(re(&bpre), sizeof(bpre));
						bpre.maxid = bnow.id[0];
						fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
						fi.write(re(&bpre), sizeof(bpre));						
					}

					if (bnow.sz == 0 && sz != 1) // if this block is empty
					{
						if (bnow.pre)
						{
							BLOCK bpre;
							fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
							fi.read(re(&bpre), sizeof(bpre));
							bpre.ne = bnow.ne;
							bpre.maxid = bnow.maxid;
							fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
							fi.write(re(&bpre), sizeof(bpre));
						}
						else
						{
							int st = bnow.ne;
							fi.seekp(sizeof(int), ios::beg);
							fi.write(re(&st), sizeof(st));
						}
						if (bnow.ne)
						{
							BLOCK bne;
							fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.ne - 1), ios::beg);
							fi.read(re(&bne), sizeof(bne));
							bne.pre = bnow.pre;
							fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.ne - 1), ios::beg);
							fi.write(re(&bne), sizeof(bne));
						}

						BLOCK bchange; //cover this block entirely
						fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(sz - 1), ios::beg);
						fi.read(re(&bchange), sizeof(bchange));
						memcpy(&bnow, &bchange, sizeof(BLOCK));
						if (bnow.pre)
						{
							BLOCK bpre;
							fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
							fi.read(re(&bpre), sizeof(bpre));
							bpre.ne = now;
							fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.pre - 1), ios::beg);
							fi.write(re(&bpre), sizeof(bpre));
						}
						else
						{
							int st = now;
							fi.seekp(sizeof(int), ios::beg);
							fi.write(re(&st), sizeof(st));
						}
						if (bnow.ne)
						{
							BLOCK bne;
							fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.ne - 1), ios::beg);
							fi.read(re(&bne), sizeof(bne));
							bne.pre = now;
							fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(bnow.ne - 1), ios::beg);
							fi.write(re(&bne), sizeof(bne));
						}
						
						--sz;
						fi.seekp(0, ios::beg);
						fi.write(re(&sz), sizeof(sz));
						fi.close();
						return;
					}

					fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
					fi.write(re(&bnow), sizeof(bnow));
					fi.close();
					return;
				}
		}
		fi.close();
		return;
	}

	void Modify(T _id, U _info)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::out | fstream::binary);
		int sz;
		fi.read(re(&sz), sizeof(int));
		int now;
		fi.read(re(&now), sizeof(int));
		BLOCK bnow;
		
		while (now)
		{
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(bnow));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				continue;
			}

			for (int i = 0; i < bnow.sz; ++i)
				if (_id == bnow.id[i])
				{
					bnow.info[i] = _info;
					//puts("REALLY ! ! FUCK ! changed"); //???
					break;
				}
			fi.seekp(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.write(re(&bnow), sizeof(BLOCK));
			fi.close();
			return;
		}
		fi.close();
		return;
	}

	U Find(T _id)
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::binary);
		//puts("already open");
		//cout << "name : " << _name << endl;
		//cout << "_id : " << _id << endl;
		int sz = 1;
		fi.read(re(&sz), sizeof(int));
		int now = 1;
		fi.read(re(&now), sizeof(int));
		
		BLOCK bnow;
		U _info = U(0);
		//puts("into while");
		//std::cout << _id << std::endl;
		//cout << "now : "<< now << endl;
		//if (typeid(T) == typeid(mystring<30>())) std::cout << "id is : " << string(_id) << std::endl;
		while (now)
		{
			//std::cout << "now : " << now << std::endl;
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			fi.read(re(&bnow), sizeof(bnow));
			if (!(_id < bnow.maxid))
			{
				now = bnow.ne;
				//puts("next");
				continue;
			}

			//cout << "bnow.sz " << bnow.sz << endl;
			for (int i = 0; i < bnow.sz; ++i)
			{
				//cout << "bnow  " << bnow.id[i] << endl;
				if (_id == bnow.id[i])
				{
					//if (typeid(T) == typeid(mystring<30>()))
					//std::cout << "bnow.id[i] : " << bnow.id[i] << std::endl;
					//std::cout << "bnow.id[i] :" << string(bnow.id[i]) << std::endl;
					fi.close();
					return bnow.info[i];
				}
			}
			fi.close();
			return _info;
		}
		fi.close();
		return _info;
	}

	bool canFind(T _id)
	{
		return (Find(_id));
	}

	class iterator
	{
	private:
		int now = 1; //block number now
		int pnow = 0;//in this block's position
		int maxnow = 0; //the largest possible number for now
		mystring<30> name = 0;

	public:

		iterator()
		{
			now = 1;
			pnow = 0;
			maxnow = 0;
		}

		iterator (int _maxnow = 0, mystring<30> _name = 0)
			: now(1), pnow(0), maxnow(_maxnow), name(_name) {}

		iterator (const iterator &) = default;

		iterator &operator=(const iterator &) = default;

		iterator &operator ++()
		{
			fstream fi;
			string _name = name;
			fi.open(_name + ".bin", fstream::in | fstream::binary);
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			BLOCK bnow;
			fi.read(re(&bnow), sizeof(BLOCK));
			if (pnow == bnow.sz - 1)
				++now, pnow = 0;
			else
				++pnow;
			fi.close();
			return (*this);
		}

		iterator operator ++(int)
		{
			iterator _this = *this;
			++(*this);
			return _this;
		}

		U operator *()
		{
			fstream fi;
			string _name = name;
			fi.open(_name + ".bin", fstream::in | fstream::binary);
			fi.seekg(sizeof(int) * 2 + sizeof(BLOCK)*(now - 1), ios::beg);
			BLOCK bnow;
			fi.read(re(&bnow), sizeof(BLOCK));
			fi.close();
			return bnow.info[pnow];
		}

		U* operator ->()
		{
			return (&(operator*()));
		}

		operator bool()
		{
			return now <= maxnow;
		}
	};
	iterator begin()
	{
		fstream fi;
		string _name = name;
		fi.open(_name + ".bin", fstream::in | fstream::binary);
		int _maxnow;
		fi.read(re(&_maxnow), sizeof(int));
		fi.close();
		iterator ret(_maxnow, name);
		return ret;
	}
	iterator it_null()
	{
		iterator ret = 0;
		return ret;
	}
};

#endif // !DATABASE_HPP
