// welp_global_type.hpp - last update : 15 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_GLOBAL_TYPE_H
#define WELP_GLOBAL_TYPE_H


////// INCLUDES //////

#include <utility>
#include <memory>

#ifdef WELP_GLOBAL_TYPE_INCLUDE_MUTEX
#include <mutex>
#endif // WELP_GLOBAL_TYPE_INCLUDE_MUTEX


////// DESCRIPTIONS //////

namespace welp
{
	template <class base_type, unsigned int id_number = static_cast<unsigned int>(-1), class _Allocator = std::allocator<base_type>> class _global_instance;
	template <class base_type, unsigned int id_number, class _Allocator> class _global_pointer;

	template <class base_type, unsigned int id_number = static_cast<unsigned int>(-1), class _Allocator = std::allocator<base_type>>
	using global_type = welp::_global_instance<base_type, id_number, _Allocator>&;

	template <class base_type, unsigned int id_number = static_cast<unsigned int>(-1), class _Allocator = std::allocator<base_type>, class ... _Args>
	inline _global_instance<base_type, id_number, _Allocator>& make_global_type(_Args&& ... args);

#ifdef WELP_GLOBAL_TYPE_INCLUDE_MUTEX
	template <class base_type, unsigned int id_number = static_cast<unsigned int>(-1), class _Allocator = std::allocator<base_type>, class ... _Args>
	inline _global_instance<base_type, id_number, _Allocator>& make_global_type_sync(std::mutex& mu, _Args&& ... args);
#endif // WELP_GLOBAL_TYPE_INCLUDE_MUTEX

	template <class base_type, unsigned int id_number = static_cast<unsigned int>(-1), class _Allocator = std::allocator<base_type>>
	inline bool is_global_type_created() noexcept;

	template <class base_type, unsigned int id_number, class _Allocator> class _global_instance : public base_type
	{

	public:

		base_type& operator=(const base_type& rhs_base_object);
		base_type& operator=(base_type&& rhs_base_object) noexcept;

		inline const base_type* base_type_ptr() const noexcept;
		inline base_type* base_type_ptr() noexcept;
		inline const base_type& base_type_ref() const noexcept;
		inline base_type& base_type_ref() noexcept;

		constexpr unsigned int global_type_id() const noexcept;

	private:

		static _global_pointer<base_type, id_number, _Allocator> _global_instance_ptr;

		_global_instance() = delete;
		_global_instance(const _global_instance<base_type, id_number, _Allocator>&) = delete;
		welp::_global_instance<base_type, id_number, _Allocator>& operator=(const welp::_global_instance<base_type, id_number, _Allocator>&) = delete;
		_global_instance(_global_instance<base_type, id_number, _Allocator>&&) = delete;
		welp::_global_instance<base_type, id_number, _Allocator>& operator=(welp::_global_instance<base_type, id_number, _Allocator>&&) = delete;
		~_global_instance() = default;

	public:

		template <class base_type2, unsigned int id_number2, class _Allocator2, class ... _Args>
		friend inline _global_instance<base_type2, id_number2, _Allocator2>& make_global_type(_Args&& ... args);

#ifdef WELP_GLOBAL_TYPE_INCLUDE_MUTEX
		template <class base_type2, unsigned int id_number2, class _Allocator2, class ... _Args>
		friend inline _global_instance<base_type2, id_number2, _Allocator2>& make_global_type_sync(std::mutex&, _Args&& ... args);
#endif // WELP_GLOBAL_TYPE_INCLUDE_MUTEX

		template <class base_type2, unsigned int id_number2, class _Allocator2> friend inline bool is_global_type_created() noexcept;
	};

	template <class base_type, unsigned int id_number, class _Allocator> class _global_pointer : public _Allocator
	{

	public:

		template <class base_type2, unsigned int id_number2, class _Allocator2>
		friend class _global_instance;

		template <class base_type2, unsigned int id_number2, class _Allocator2, class ... _Args>
		friend inline _global_instance<base_type2, id_number2, _Allocator2>& make_global_type(_Args&& ... args);

#ifdef WELP_GLOBAL_TYPE_INCLUDE_MUTEX
		template <class base_type2, unsigned int id_number2, class _Allocator2, class ... _Args>
		friend inline _global_instance<base_type2, id_number2, _Allocator2>& make_global_type_sync(std::mutex&, _Args&& ... args);
#endif // WELP_GLOBAL_TYPE_INCLUDE_MUTEX

		template <class base_type2, unsigned int id_number2, class _Allocator2> friend inline bool is_global_type_created() noexcept;

	private:

		base_type* raw_ptr = nullptr;

		_global_pointer(base_type* const rhs_ptr);

		_global_pointer() = delete;
		_global_pointer(const _global_pointer<base_type, id_number, _Allocator>&) = default;
		_global_pointer<base_type, id_number, _Allocator> operator=(const _global_pointer<base_type, id_number, _Allocator>&) = delete;
		_global_pointer(_global_pointer<base_type, id_number, _Allocator>&&) = default;
		_global_pointer<base_type, id_number, _Allocator> operator=(_global_pointer<base_type, id_number, _Allocator>&&) = delete;
		~_global_pointer();
	};
}


////// IMPLEMENTATIONS //////

template <class base_type, unsigned int id_number, class _Allocator>
welp::_global_pointer<base_type, id_number, _Allocator> welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr = nullptr;

template <class base_type, unsigned int id_number, class _Allocator, class ... _Args>
inline welp::_global_instance<base_type, id_number, _Allocator>& welp::make_global_type(_Args&& ... args)
{
	if (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr)
	{
		return *static_cast<welp::_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
			welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr));
	}
	else
	{
		try
		{
			welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr
				= static_cast<_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
					welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.allocate(1)));

			if (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr)
			{
				new (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr)
					base_type(std::forward<_Args>(args)...);
			}
		}
		catch (...)
		{
			welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr = nullptr;
		}

		return *static_cast<welp::_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
			welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr));
	}
}

#ifdef WELP_GLOBAL_TYPE_INCLUDE_MUTEX
template <class base_type, unsigned int id_number, class _Allocator, class ... _Args>
inline welp::_global_instance<base_type, id_number, _Allocator>& welp::make_global_type_sync(std::mutex& mu, _Args&& ... args)
{
	if (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr)
	{
		return *static_cast<welp::_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
			welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr));
	}
	else
	{
		std::lock_guard<std::mutex> lock(mu);
		if (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr)
		{
			return *static_cast<welp::_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
				welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr));
		}
		else
		{
			try
			{
				welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr
					= static_cast<_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
						welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.allocate(1)));

				if (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr)
				{
					new (welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr)
						base_type(std::forward<_Args>(args)...);
				}
			}
			catch (...)
			{
				welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr = nullptr;
			}

			return *static_cast<welp::_global_instance<base_type, id_number, _Allocator>*>(static_cast<void*>(
				welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr));
		}
	}
}
#endif // WELP_GLOBAL_TYPE_INCLUDE_MUTEX

template <class base_type, unsigned int id_number, class _Allocator>
inline bool welp::is_global_type_created() noexcept
{
	return welp::_global_instance<base_type, id_number, _Allocator>::_global_instance_ptr.raw_ptr != nullptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
base_type& welp::_global_instance<base_type, id_number, _Allocator>::operator=(const base_type& rhs_base_object)
{
	*_global_instance_ptr.raw_ptr = rhs_base_object;
	return *_global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
base_type& welp::_global_instance<base_type, id_number, _Allocator>::operator=(base_type&& rhs_base_object) noexcept
{
	*_global_instance_ptr.raw_ptr = std::move(rhs_base_object);
	return *_global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
inline const base_type* welp::_global_instance<base_type, id_number, _Allocator>::base_type_ptr() const noexcept
{
	return _global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
inline base_type* welp::_global_instance<base_type, id_number, _Allocator>::base_type_ptr() noexcept
{
	return _global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
inline const base_type& welp::_global_instance<base_type, id_number, _Allocator>::base_type_ref() const noexcept
{
	return *_global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
inline base_type& welp::_global_instance<base_type, id_number, _Allocator>::base_type_ref() noexcept
{
	return *_global_instance_ptr.raw_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
constexpr unsigned int welp::_global_instance<base_type, id_number, _Allocator>::global_type_id() const noexcept
{
	return id_number;
}

template <class base_type, unsigned int id_number, class _Allocator>
welp::_global_pointer<base_type, id_number, _Allocator>::_global_pointer(base_type* rhs_ptr)
{
	raw_ptr = rhs_ptr;
}

template <class base_type, unsigned int id_number, class _Allocator>
welp::_global_pointer<base_type, id_number, _Allocator>::~_global_pointer()
{
	if (raw_ptr != nullptr)
	{
		raw_ptr->~base_type();
		this->deallocate(raw_ptr, sizeof(base_type));
	}
}


#endif // WELP_GLOBAL_TYPE_H


// welp_global_type.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
