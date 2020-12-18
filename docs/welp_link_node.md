# welp_link_node.hpp

welp_link_node.hpp provides two classes :

- welp::link_node<msg_Ty, label_Ty, Allocator> is a class that may receive notifications from other link nodes with msg_Ty as the first template parameter and can maintain its own list of subscribers to send notifications to.
- welp::observer_node<msg_Ty> is a class that may receive notifications from other link nodes with msg_Ty as the first template parameter. It does not maintain subsribers and cannot send notifications.

### Code example

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
