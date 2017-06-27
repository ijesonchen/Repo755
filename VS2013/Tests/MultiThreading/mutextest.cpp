#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::unique_lock
#include <atomic>
#include <chrono>

using namespace std;

std::mutex mtx;           // mutex for critical section
atomic_bool gbReady;

void print_block(int n, char c) {
	// critical section (exclusive access to std::cout signaled by lifetime of lck):
	while (!gbReady)
	{
	}
	chrono::nanoseconds ms(1);
	std::unique_lock<std::mutex> lck(mtx);
	for (int i = 0; i < n; ++i) 
	{ 
		std::cout << c;
		this_thread::sleep_for(ms);
	}
	std::cout << '\n';
}

void MutexTest(void)
{
	gbReady = false;
	std::thread th1(print_block, 50, '*');
	std::thread th2(print_block, 50, '$');
	std::thread th3(print_block, 50, 'x');
	std::thread th4(print_block, 50, 'o');

	cout << "ready." << endl;
	
	gbReady = true;

	th1.join();
	th2.join();
	th3.join();
	th4.join();

}