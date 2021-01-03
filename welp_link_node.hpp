// welp_link_node.hpp - last update : 24 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_LINK_NODE_HPP
#define WELP_LINK_NODE_HPP


////// INCLUDES //////

#include <cstdlib>
#include <map>


// include all in one line with #define WELP_LINK_NODE_INCLUDE_ALL
#if defined(WELP_LINK_NODE_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_LINK_NODE_INCLUDE_MUTEX
#define WELP_LINK_NODE_INCLUDE_MUTEX
#endif
#endif // WELP_LINK_NODE_INCLUDE_ALL


#ifdef WELP_LINK_NODE_INCLUDE_MUTEX
#include <mutex>
#endif // WELP_LIN_NODE_INCLUDE_MUTEX


////// DESCRIPTIONS //////

namespace welp
{
	// the allocator must have std::pair<char* const, label_Ty>> as template parameter
	// link_node can add both an observer_node and a link_node with the method add_observer_node
	template<class msg_Ty, class label_Ty = int, class _Allocator = std::allocator<std::pair<char* const, label_Ty>>> class link_node
	{

	public:

		// getting notifications

		virtual void on_notification_from_node(const msg_Ty&) {}


		// sending notifications

		void notify_all_observer_nodes() const;
		void notify_all_observer_nodes(const msg_Ty& msg) const;
		void notify_all_observer_nodes(const msg_Ty& msg, const label_Ty& compare_label) const;
		template <class Predicate> void notify_all_observer_nodes(const msg_Ty& msg, Predicate Pr) const;


		// observer nodes management

		template <class Ty> void add_observer_node(const Ty* const target_ptr);
		template <class Ty> void add_observer_node(const Ty* const target_ptr, const label_Ty& target_label);
		template <class Ty> void remove_observer_node(const Ty* const target_ptr);
		inline void remove_all_observer_nodes() noexcept;
		void remove_all_observer_nodes(const label_Ty& target_label);
		template <class Predicate> void remove_all_observer_nodes(Predicate Pr);
		template <class Ty> bool contains_observer_node(const Ty* const target_ptr) const;
		inline std::size_t observer_node_count() const noexcept;
		std::size_t observer_node_count(const label_Ty& target_label) const;
		template <class Predicate> std::size_t observer_node_count(Predicate Pr) const;
		template <class Ty> label_Ty observer_node_label(const Ty* const target_ptr) const noexcept;


		link_node() = default;
		link_node(const welp::link_node<msg_Ty, label_Ty, _Allocator>&) = default;
		welp::link_node<msg_Ty, label_Ty, _Allocator>& operator=(const welp::link_node<msg_Ty, label_Ty, _Allocator>&) = default;
		link_node(welp::link_node<msg_Ty, label_Ty, _Allocator>&&) = default;
		welp::link_node<msg_Ty, label_Ty, _Allocator>& operator=(welp::link_node<msg_Ty, label_Ty, _Allocator>&&) = default;
		virtual ~link_node() = default;

	private:

		std::map<char*, label_Ty, std::less<char*>, _Allocator> _welp_link_node_map;
	};

#ifdef WELP_LINK_NODE_INCLUDE_MUTEX
	// the allocator must have std::pair<char* const, label_Ty>> as template parameter
	// link_node_sync can add both an observer_node and a link_node_sync with the method add_observer_node
	template<class msg_Ty, class label_Ty = int, class _Allocator = std::allocator<std::pair<char* const, label_Ty>>> class link_node_sync
	{

	public:

		// getting notifications

		virtual void on_notification_from_node(const msg_Ty&) {}


		// sending notifications

		void notify_all_observer_nodes() const;
		void notify_all_observer_nodes(const msg_Ty& msg) const;
		void notify_all_observer_nodes(const msg_Ty& msg, const label_Ty& compare_label) const;
		template <class Predicate> void notify_all_observer_nodes(const msg_Ty& msg, Predicate Pr) const;


		// observer nodes management

		template <class Ty> void add_observer_node(const Ty* const target_ptr);
		template <class Ty> void add_observer_node(const Ty* const target_ptr, const label_Ty& target_label);
		template <class Ty> void remove_observer_node(const Ty* const target_ptr);
		inline void remove_all_observer_nodes() noexcept;
		void remove_all_observer_nodes(const label_Ty& target_label);
		template <class Predicate> void remove_all_observer_nodes(Predicate Pr);
		template <class Ty> bool contains_observer_node(const Ty* const target_ptr) const;
		inline std::size_t observer_node_count() const noexcept;
		std::size_t observer_node_count(const label_Ty& target_label) const;
		template <class Predicate> std::size_t observer_node_count(Predicate Pr) const;
		template <class Ty> label_Ty observer_node_label(const Ty* const target_ptr) const noexcept;


		link_node_sync() = default;
		link_node_sync(const welp::link_node_sync<msg_Ty, label_Ty, _Allocator>&) = default;
		welp::link_node_sync<msg_Ty, label_Ty, _Allocator>& operator=(const welp::link_node_sync<msg_Ty, label_Ty, _Allocator>&) = default;
		link_node_sync(welp::link_node_sync<msg_Ty, label_Ty, _Allocator>&&) = default;
		welp::link_node_sync<msg_Ty, label_Ty, _Allocator>& operator=(welp::link_node_sync<msg_Ty, label_Ty, _Allocator>&&) = default;
		virtual ~link_node_sync() = default;

	private:

		std::map<char*, label_Ty, std::less<char*>, _Allocator> _welp_link_node_map;
		std::mutex _welp_link_node_mutex;
	};
#endif // WELP_LIN_NODE_INCLUDE_MUTEX

	template <class msg_Ty> class observer_node
	{

	public:

		// GETTING NOTIFICATIONS

		virtual void on_notification_from_node(const msg_Ty&) {}


		observer_node() = default;
		observer_node(const welp::observer_node<msg_Ty>&) = default;
		welp::observer_node<msg_Ty>& operator=(const welp::observer_node<msg_Ty>&) = default;
		observer_node(welp::observer_node<msg_Ty>&&) = default;
		welp::observer_node<msg_Ty>& operator=(welp::observer_node<msg_Ty>&&) = default;
		virtual ~observer_node() = default;
	};
}


////// IMPLEMENTATIONS //////

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes() const
{
	msg_Ty msg = msg_Ty();
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		iter->first->on_notification_from_node(msg);
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg) const
{
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		iter->first->on_notification_from_node(msg);
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg, const label_Ty& compare_label) const
{
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (iter->second == compare_label)
		{
			iter->first->on_notification_from_node(msg);
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg, Predicate Pr) const
{
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (Pr(iter->second))
		{
			iter->first->on_notification_from_node(msg);
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::add_observer_node(const Ty* const target_ptr)
{
	_welp_link_node_map.emplace(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))), label_Ty());
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::add_observer_node(const Ty* const target_ptr, const label_Ty& target_label)
{
	_welp_link_node_map.emplace(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))), target_label);
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::remove_observer_node(const Ty* const target_ptr)
{
	_welp_link_node_map.erase(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))));
}

template <class msg_Ty, class label_Ty, class _Allocator>
inline void welp::link_node<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes() noexcept
{
	_welp_link_node_map.clear();
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes(const label_Ty& target_label)
{
	auto iter = reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
	while (iter != reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end())
	{
		if (iter->second == target_label)
		{
			iter = reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
void welp::link_node<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes(Predicate Pr)
{
	auto iter = reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
	while (iter != reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end())
	{
		if (Pr(iter->second))
		{
			iter = reinterpret_cast<std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
bool welp::link_node<msg_Ty, label_Ty, _Allocator>::contains_observer_node(const Ty* const target_ptr) const
{
	if (_welp_link_node_map.count(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr)))) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
inline std::size_t welp::link_node<msg_Ty, label_Ty, _Allocator>::observer_node_count() const noexcept
{
	return _welp_link_node_map.size();
}

template <class msg_Ty, class label_Ty, class _Allocator>
std::size_t welp::link_node<msg_Ty, label_Ty, _Allocator>::observer_node_count(const label_Ty& compare_label) const
{
	std::size_t counter = 0;
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (iter->second == compare_label)
		{
			counter++;
		}
	}
	return counter;
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
std::size_t welp::link_node<msg_Ty, label_Ty, _Allocator>::observer_node_count(Predicate Pr) const
{
	std::size_t counter = 0;
	for (auto iter = reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (Pr(iter->second))
		{
			counter++;
		}
	}
	return counter;
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
inline label_Ty welp::link_node<msg_Ty, label_Ty, _Allocator>::observer_node_label(const Ty* const ptr) const noexcept
{
	char* const char_ptr = static_cast<char* const>(static_cast<void* const>(const_cast<Ty* const>(ptr)));
	if (_welp_link_node_map.count(char_ptr) != 0)
	{
		return _welp_link_node_map.at(char_ptr);
	}
	else
	{
		return label_Ty();
	}
}


#ifdef WELP_LINK_NODE_INCLUDE_MUTEX
template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes() const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	msg_Ty msg = msg_Ty();
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		iter->first->on_notification_from_node(msg);
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		iter->first->on_notification_from_node(msg);
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg, const label_Ty& compare_label) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (iter->second == compare_label)
		{
			iter->first->on_notification_from_node(msg);
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::notify_all_observer_nodes(const msg_Ty& msg, Predicate Pr) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (Pr(iter->second))
		{
			iter->first->on_notification_from_node(msg);
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::add_observer_node(const Ty* const target_ptr)
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	_welp_link_node_map.emplace(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))), label_Ty());
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::add_observer_node(const Ty* const target_ptr, const label_Ty& target_label)
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	_welp_link_node_map.emplace(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))), target_label);
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::remove_observer_node(const Ty* const target_ptr)
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	_welp_link_node_map.erase(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr))));
}

template <class msg_Ty, class label_Ty, class _Allocator>
inline void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes() noexcept
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	_welp_link_node_map.clear();
}

template <class msg_Ty, class label_Ty, class _Allocator>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes(const label_Ty& target_label)
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	auto iter = reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
	while (iter != reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end())
	{
		if (iter->second == target_label)
		{
			iter = reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
void welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::remove_all_observer_nodes(Predicate Pr)
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	auto iter = reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
	while (iter != reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end())
	{
		if (Pr(iter->second))
		{
			iter = reinterpret_cast<std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
bool welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::contains_observer_node(const Ty* const target_ptr) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	if (_welp_link_node_map.count(static_cast<char*>(static_cast<void*>(const_cast<Ty* const>(target_ptr)))) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

template <class msg_Ty, class label_Ty, class _Allocator>
inline std::size_t welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::observer_node_count() const noexcept
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	return _welp_link_node_map.size();
}

template <class msg_Ty, class label_Ty, class _Allocator>
std::size_t welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::observer_node_count(const label_Ty& compare_label) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	std::size_t counter = 0;
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (iter->second == compare_label)
		{
			counter++;
		}
	}
	return counter;
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Predicate>
std::size_t welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::observer_node_count(Predicate Pr) const
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	std::size_t counter = 0;
	for (auto iter = reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).begin();
		iter != reinterpret_cast<const std::map<welp::link_node_sync<msg_Ty, label_Ty, _Allocator>*, label_Ty>&>(_welp_link_node_map).end(); ++iter)
	{
		if (Pr(iter->second))
		{
			counter++;
		}
	}
	return counter;
}

template <class msg_Ty, class label_Ty, class _Allocator> template <class Ty>
inline label_Ty welp::link_node_sync<msg_Ty, label_Ty, _Allocator>::observer_node_label(const Ty* const ptr) const noexcept
{
	std::lock_guard<std::mutex> lock(_welp_link_node_mutex);
	char* const char_ptr = static_cast<char* const>(static_cast<void* const>(const_cast<Ty* const>(ptr)));
	if (_welp_link_node_map.count(char_ptr) != 0)
	{
		return _welp_link_node_map.at(char_ptr);
	}
	else
	{
		return label_Ty();
	}
}
#endif // WELP_LIN_NODE_INCLUDE_MUTEX


#endif // WELP_LINK_NODE_HPP


// welp_link_node.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
