# welp_cyclic_buffer.hpp

Cyclic buffers !

Incoming ...

	#define WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#include "welp_cyclic_buffer.hpp"
#include <iostream>

welp::cyclic_buffer_sync<int> Q;
std::mutex mu;


void f() // load values from Q
{
	int x0, x1, x2, x3, x4, x5, x6, x7;
	while (!(Q.load_stream(8) >> x0 >> x1 >> x2 >> x3
		>> x4 >> x5 >> x6 >> x7).good());

	std::lock_guard<std::mutex> lock(mu);
	std::cout << x0 << ' ' << x1 << ' ' << x2 << ' ' << x3 << ' '
		<< x4 << ' ' << x5 << ' ' << x6 << ' ' << x7 << std::endl;
}

void g() // store values in Q
{
	while (!(Q.store_stream(16) << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8
		<< 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16).good());
}


int main()
{
	Q.new_buffer(100);

	std::thread th0(f);
	std::thread th1(f);
	std::thread th2(g);

	th0.join();
	th1.join();
	th2.join();

	std::cin.get();
}
