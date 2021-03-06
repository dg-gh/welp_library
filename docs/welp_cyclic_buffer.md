# welp_cyclic_buffer.hpp

Cyclic buffers !

Incoming ...

	#define WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	#include "welp_cyclic_buffer.hpp"
	#include <iostream>
	
	welp::cyclic_buffer_sync<int> CB;
	std::mutex mu;
	
	void f() // load values from CB
	{
		int x0, x1, x2, x3, x4, x5, x6, x7;
		while (!(CB.load_block(8) >> x0 >> x1 >> x2 >> x3
			>> x4 >> x5 >> x6 >> x7).good());

		std::lock_guard<std::mutex> lock(mu);
		std::cout << "load successful : " << x0 << ' ' << x1 << ' ' << x2 << ' ' << x3 << ' '
			<< x4 << ' ' << x5 << ' ' << x6 << ' ' << x7 << std::endl;
	}

	void g() // store values in CB
	{
		while (!(CB.store_block(16) << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8
			<< 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16).good());
		
		std::lock_guard<std::mutex> lock(mu);
		std::cout << "store successful !" << std::endl;
	}
	
	int main()
	{
		CB.new_buffer(100);
	
		std::thread th0(f);
		std::thread th1(f);
		std::thread th2(g);
	
		th0.join();
		th1.join();
		th2.join();
	
		std::cin.get();
	}
