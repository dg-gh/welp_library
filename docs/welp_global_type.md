# welp_global_type.hpp

Incoming ...

	#include <iostream>
	#include <string>
	#include "welp_global_type.hpp"
 	   
	int main()
	{
		welp::global_type<std::string> S1 = welp::make_global_type<std::string>("Hello world!");
		std::cout << "S1 : " << S1 << std::endl;
		welp::global_type<std::string> S2 = welp::make_global_type<std::string>("How are you?");
		std::cout << "S2 : " << S2 << std::endl;
		S1.resize(5);
		std::cout << "S1 resized to 5, so S2 : " << S2 << std::endl;
		
		std::cin.get();
		return 0;
	}
