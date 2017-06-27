#include <map>
#include <mutex>
#include <iostream>
#include <memory>
#include <chrono>



using namespace std;

class MyMutex
{
	timed_mutex m_cs;
public:
	
	template <class Rep, class Period>
	bool try_lock_for(const chrono::duration<Rep, Period>& rel_time)
	{
		cout << "MyLock try_lock_for" << endl;
		return m_cs.try_lock_for(rel_time);
	}

	void unlock()
	{
		cout << "MyLock unlock" << endl;
		return m_cs.unlock();
	}


};

class Obj
{
public:
	int m;

	Obj(int x)
		:m(x)
	{
		cout << "construct obj " << m << endl;
	}

	~Obj()
	{
		cout << "destruct obj " << m << endl;
	}
};


class LinkMap
{
public:
	MyMutex m_cs;
	map<int, shared_ptr<Obj>> m_map;
	using type_spobj = map < int, shared_ptr<Obj> >::mapped_type ;

	bool Add(int key, shared_ptr < Obj > val)
	{
		cout << "Link add" << endl;
		unique_lock<MyMutex> lock(m_cs, chrono::seconds(1));
		if (!lock)
		{
			return false;
		}
		m_map.insert(make_pair(key, val));
		return true;
	}

	bool Remove(int key)
	{
		cout << "Link Remove" << endl;

		using valt = map<int, shared_ptr<Obj>>::mapped_type;

		type_spobj val;

		auto iter = m_map.find(key);
		if (iter == m_map.cend())
		{
			cout << "key not found" << endl;
			return false;
		}		
		val = iter->second;

		unique_lock<MyMutex> lock(m_cs, chrono::seconds(1));
		if (!lock)
		{
			cout << "lock failed" << endl;
			return false;
		}
		return m_map.erase(key) ? true : false;
	}



	std::shared_ptr<Obj> ReleaseLink(int key)
	{
		unique_lock<MyMutex> lock(m_cs, chrono::seconds(1));
		if (!lock)
		{
			cout << "lock failed" << endl;
			return nullptr;
		}

		auto iter = m_map.find(key);
		if (iter == m_map.cend())
		{
			cout << "LinkMap ReleaseLink SOCKET not found" << endl;;
			return nullptr;
		}
		auto value = iter->second;
		if (m_map.erase(key) == 0)
		{
			cout << "LinkMap ReleaseLink erase return 0" << endl;
		}

		return value;
	}


	bool TransFrom(LinkMap& another, int sd)
	{
		auto link = another.ReleaseLink(sd);
		if (!link)
		{
			return false;
		}
		unique_lock<MyMutex> lock(m_cs, chrono::seconds(1));
		if (!lock)
		{
			cout << "lock failed" << endl;
			return false;
		}

		m_map.insert(make_pair(sd, link));
		return true;

	}
};


void maptest(void)
{
	LinkMap link;
	LinkMap link2;
	int nObj = 5;
	if (nObj)
	{
		auto a = make_shared<Obj>(nObj);
		link.Add(nObj, a);
	}
	link2.TransFrom(link, nObj);


	
	cout << endl << "enter to exit." << endl;
	getchar();
}