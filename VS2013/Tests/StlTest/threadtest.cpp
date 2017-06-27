#include <thread>
#include <mutex>
#include <atomic>
#include <iostream>
#include <sstream>
#include <chrono>


using namespace std;

chrono::seconds TimeSleep(1);

class RecogLink
{
public:
	RecogLink(const RecogLink& a)
	{
		cout << "RecogLink copied" << endl;
	}

	RecogLink(const RecogLink&& a)
	{
		cout << "RecogLink moved" << endl;
	}

	RecogLink(int i)
	{
		cout << "RecogLink constructed" << endl;
		counter = 0;
		stringstream ss;
		ss << "Thread: " << i << " - ";
		id = ss.str();
	}


	void Thread2(void)
	{
		cout << id << "start" << endl;
		counter++;

		cout << id << "counter " << counter << endl;
		cout << id << "end" << endl;
	}

	static int Thread1(RecogLink* pLink)
	{
		const auto& id = pLink->id;
		cout << id << "start" << endl;
		pLink->counter++;

		this_thread::sleep_for(TimeSleep);

		cout << id << "counter " << pLink->counter << endl;

		pLink->Thread2();

		cout << id << "end" << endl;

		return 9;
	};
protected:
private:
	atomic_int counter;
	std::string id;
};

void ThreadTest(void)
{

	string s1;
	string s2;
	s1 == s2;
	s1 = "a";
	s2 = "b";
	s1 == s2;

	s2 = "a";
	s1 == s2;
	s2 = "as";
	s1 == s2;
	thread th0;

	RecogLink link(28);
	thread th1(&RecogLink::Thread1, &link);

	auto a1 = th1.joinable();
	auto a2 = th1.get_id();
	auto a3 = th1.native_handle();

	th0.swap(th1);
	auto a4 = th1.joinable();
	auto a5 = th1.get_id();
	auto a6 = th1.native_handle();

	th0.join();
}