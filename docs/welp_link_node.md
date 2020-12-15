# welp_link_node.hpp

Incoming ...

### Code example with std::pmr::memory_resource (requires C++17)

	#include <iostream>
	#include "welp_link_node.hpp"

	class administrator : public welp::link_node<int>
	{

	public:

		void identify_your_pets(int year)
		{
			std::cout << "- Please identify all the pets you had in " << year << "!" << std::endl;
			notify_all_observer_nodes(year);
		}
	};

	class pet_owner : public welp::link_node<int>
	{

	public:

		void on_notification_from_node(const int& year) override
		{
			std::cout << "- Ok, I will call all the pets I had in " << year << "." << std::endl;
			notify_all_observer_nodes(0);
		}
	};

	class cat : public welp::observer_node<int>
	{
	
	public:
	
		void on_notification_from_node(const int&) override
		{
			std::cout << "- Meow!" << std::endl;
		}
	};

	class dog : public welp::observer_node<int>
	{
	
	public:
	
		void on_notification_from_node(const int&) override
		{
			std::cout << "- Woof!" << std::endl;
		}
	};
	
	
	int main()
	{
		administrator Peter;
		pet_owner Gary;
		cat Garfield;
		dog Goofy;

		Peter.add_observer_node(&Gary);
		Gary.add_observer_node(&Garfield);
		Gary.add_observer_node(&Goofy);
	
		Peter.identify_your_pets(2020);
	
		std::cin.get();
		return 0;
	}
