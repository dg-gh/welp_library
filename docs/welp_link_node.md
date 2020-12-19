# welp_link_node.hpp

welp_link_node.hpp provides three classes :

- welp::link_node<msg_Ty, label_Ty, Allocator> is a class that may receive notifications from other link nodes with msg_Ty as the first template parameter and can maintain its own list of subscribers to send notifications to.
- welp::observer_node<msg_Ty> is a class that may receive notifications from other link nodes with msg_Ty as the first template parameter. The only difference with link_node is that it does not maintain subsribers and cannot send notifications.
- welp::link_node_sync<msg_Ty, label_Ty, Allocator> is a class that is identical to welp::link_node<msg_Ty, label_Ty, Allocator> except that it has a mutex to secure sensitive methods.

# Member functions of welp::link_node<msg_Ty, label_Ty, Allocator> L

Every link node maintains a list of subscribers that can be link nodes or observer nodes, or derived classes of any of these two. Template parameter msg_Ty is the type of message sent when a link node sends a message to an other node (a link node or an observer node). Template parameter label_Ty is a label assigned to every subscriber of a link node. Every link node maintains its list of subcriber in a map where the pointer to a subscriber is the key and the label is the value associated to the key.

### Getting notification

As L can both get and receive notification from other nodes, it maintains its own list of subscribers and has a virtual method void on_notification_from_node(msg) that gets called when an other node sends a notification to L with msg as the message.

### Notifying subscribers

Template parameter Allocator is an allocator of the form of std::allocator<std::pair<char* const, label_Ty>>.

	L.notify_all_observer_nodes()

Sends a notification to every subscriber with msg_ty() (result of the default construction) as the message.

	L.notify_all_observer_nodes(msg)

Sends a notification to every subscriber with msg as the message.

	L.notify_all_observer_nodes(msg, target_label)

Sends a notification to every subscriber with label equal to target_label and with msg as the message.

	L.notify_all_observer_nodes(msg, Pr)

Sends a notification to every subscriber with labels that satisfy predicate Pr and with msg as the message.

### Adding subscribers

Subcribers can be any class that inherits publicly from link nodes or observer nodes with the same message type.

	L.add_observer_node(&other_L);
	
Adds subscriber other_L to L with label label_Ty.
	
	L.add_observer_node(&other_L, target_label);
	
Adds subscriber other_L to L with labels equal to target_label.

### Removing subscribers

	L.remove_observer_node(&other_L);
	
Removes other_L from the set of subscribers of L.

	L.remove_all_observer_nodes();
	
Removes all subscribers of L.

	L.remove_all_observer_nodes(target_label);
	
Removes all subscribers of L with label equal to targel_label.

	L.remove_all_observer_nodes(Pr);
	
Removes all subscribers of L with labels that satisfy predicate Pr.

### Checking subscribers

	L.contains_observer_node(&other_L);

Returns true if other_L is a subscriber of L.

	L.observer_node_count();

Returns the number of subscribers of L;

	L.observer_node_count(target_label);

Returns the number of subscribers of L with label equals to target_label;

	L.observer_node_count(Pr);

Returns the number of subscribers of L with labels that satisfy predicate Pr;

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
