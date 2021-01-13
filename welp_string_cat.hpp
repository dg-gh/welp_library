// welp_string_cat.hpp - last update : 12 / 01 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_STRING_CAT_HPP
#define WELP_STRING_CAT_HPP


////// INCLUDES //////

#include <string>
#include <cstring>


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_STRING_CAT_DEBUG_MODE)
#define WELP_STRING_CAT_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_STRING_CAT_DEBUG_MODE
#include <cassert>
#endif // WELP_STRING_CAT_DEBUG_MODE


////// DESCRIPTIONS //////

namespace welp
{
	template <std::size_t max_number_of_strings, class string_Ty = std::string, class _Allocator = std::allocator<const string_Ty*>> class string_cat
	{

	public:

		inline string_cat<max_number_of_strings, string_Ty, _Allocator>& operator<<(const string_Ty& str);
		template <class return_string_Ty = string_Ty> return_string_Ty concatenate() const;

		inline void reset() noexcept;
		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		inline void pop_back() noexcept;
		inline void pop_back(std::size_t instances) noexcept;
		std::size_t concatenated_string_size() const noexcept;

		string_cat() = default;
		string_cat(const welp::string_cat<max_number_of_strings, string_Ty, _Allocator>&) = default;
		welp::string_cat<max_number_of_strings, string_Ty, _Allocator>& operator=(const welp::string_cat<max_number_of_strings, string_Ty, _Allocator>&) = default;
		string_cat(welp::string_cat<max_number_of_strings, string_Ty, _Allocator>&&) = default;
		welp::string_cat<max_number_of_strings, string_Ty, _Allocator>& operator=(welp::string_cat<max_number_of_strings, string_Ty, _Allocator>&&) = default;
		~string_cat() = default;

		// if max_number_of_strings = 0
		// void reserve(std::size_t instances);
		// std::size_t size();
		// std::size_t capacity();

	private:

		const string_Ty* string_array[max_number_of_strings] = { nullptr };
		std::size_t number_of_strings = 0;
	};
}


////// IMPLEMENTATIONS //////

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
inline welp::string_cat<max_number_of_strings, string_Ty, _Allocator>& welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::operator<<(const string_Ty& str)
{
#ifdef WELP_STRING_CAT_DEBUG_MODE
	assert(number_of_strings < max_number_of_strings);
#endif // WELP_STRING_CAT_DEBUG_MODE
	string_array[number_of_strings] = &str;
	number_of_strings++;
	return *this;
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
template <class return_string_Ty> return_string_Ty welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::concatenate() const
{
	const string_Ty** string_ptr = static_cast<const string_Ty**>(string_array);
	std::size_t string_size = 0;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		string_size += (*string_ptr++)->size();
	}
	string_ptr = static_cast<const string_Ty**>(string_array);
	return_string_Ty str_cat(string_size, '0');
	char* string_cat_ptr = const_cast<char*>(static_cast<const char*>(
		static_cast<const void*>(str_cat.data())));
	std::size_t size_of_char_type = sizeof(*str_cat.data());
	std::size_t temp;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		temp = (*string_ptr)->size() * size_of_char_type;
		std::memcpy(string_cat_ptr, (*string_ptr++)->data(), temp);
		string_cat_ptr += temp;
	}
	return str_cat;
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
inline void welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::reset() noexcept
{
	number_of_strings = 0;
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
inline std::size_t welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::size() const noexcept
{
	return number_of_strings;
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
constexpr std::size_t welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::capacity() const noexcept
{
	return max_number_of_strings;
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
inline void welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::pop_back() noexcept
{
	if (number_of_strings != 0) { number_of_strings--; }
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
inline void welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::pop_back(std::size_t instances) noexcept
{
	if (number_of_strings >= instances) { number_of_strings -= instances; }
}

template <std::size_t max_number_of_strings, class string_Ty, class _Allocator>
std::size_t welp::string_cat<max_number_of_strings, string_Ty, _Allocator>::concatenated_string_size() const noexcept
{
	const string_Ty** string_ptr = static_cast<const string_Ty* const*>(string_array);
	std::size_t string_size = 0;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		string_size += (*string_ptr++)->size();
	}
	return string_size;
}


namespace welp
{
	template <class string_Ty, class _Allocator> class string_cat<0, string_Ty, _Allocator> : private _Allocator
	{

	public:

		inline welp::string_cat<0, string_Ty, _Allocator>& operator<<(const string_Ty& str)
		{
			if (number_of_strings < max_number_of_strings)
			{
				*(string_ptr + number_of_strings) = &str;
				number_of_strings++;
				return *this;
			}
			else if (max_number_of_strings != 0)
			{
				reserve(number_of_strings + max_number_of_strings);
				*(string_ptr + number_of_strings) = &str;
				number_of_strings++;
				return *this;
			}
			else
			{
				reserve(2);
				*(string_ptr + number_of_strings) = &str;
				number_of_strings++;
				return *this;
			}
		}
		template <class return_string_Ty = string_Ty> return_string_Ty concatenate() const
		{
			const string_Ty** _string_ptr = string_ptr;
			std::size_t string_size = 0;
			for (std::size_t k = number_of_strings; k > 0; k--)
			{
				string_size += (*_string_ptr++)->size();
			}
			_string_ptr = string_ptr;
			return_string_Ty str_cat(string_size, '0');
			char* string_cat_ptr = const_cast<char*>(static_cast<const char*>(
				static_cast<const void*>(str_cat.data())));
			std::size_t size_of_char_type = sizeof(*str_cat.data());
			std::size_t temp;
			for (std::size_t k = number_of_strings; k > 0; k--)
			{
				temp = (*_string_ptr)->size() * size_of_char_type;
				std::memcpy(string_cat_ptr, (*_string_ptr++)->data(), temp);
				string_cat_ptr += temp;
			}
			return str_cat;
		}

		void reserve(std::size_t instances)
		{
			if (instances > max_number_of_strings)
			{
				if (string_ptr != nullptr)
				{
					_temp_string_cat temp(instances);
					std::memcpy(temp.temp_string_ptr, string_ptr, instances * sizeof(const string_Ty*));
					this->deallocate(string_ptr, max_number_of_strings);
					string_ptr = temp.temp_string_ptr;
					max_number_of_strings = temp.temp_number_of_strings;
					temp.temp_string_ptr = nullptr;
					return;
				}
				else
				{
					string_ptr = this->allocate(instances);
					max_number_of_strings = instances;
					return;
				}
			}
			else
			{
				return;
			}
		}
		void reset() noexcept
		{
			number_of_strings = 0;
		}
		void clear() noexcept
		{
			if (string_ptr != nullptr)
			{
				this->deallocate(string_ptr, max_number_of_strings);
			}
			number_of_strings = 0;
			max_number_of_strings = 0;
		}
		inline std::size_t size() const noexcept { return number_of_strings; }
		inline std::size_t capacity() const noexcept { return max_number_of_strings; }

		string_cat() = default;
		string_cat(std::size_t instances) : string_ptr(this->allocate(instances)), max_number_of_strings(instances) {}
		string_cat(const welp::string_cat<0, string_Ty, _Allocator>& rhs)
			: string_ptr(this->allocate(rhs.max_number_of_strings)),
			number_of_strings(rhs.number_of_strings),
			max_number_of_strings(rhs.max_number_of_strings)
		{
			std::memcpy(string_ptr, rhs.string_ptr, number_of_strings * sizeof(const string_Ty*));
		}
		welp::string_cat<0, string_Ty, _Allocator>& operator=(const welp::string_cat<0, string_Ty, _Allocator>& rhs)
		{
			string_ptr = this->allocate(rhs.max_number_of_strings);
			number_of_strings = rhs.number_of_strings;
			max_number_of_strings = rhs.max_number_of_strings;
			std::memcpy(string_ptr, rhs.string_ptr, number_of_strings * sizeof(const string_Ty*));
		}
		string_cat(welp::string_cat<0, string_Ty, _Allocator>&& rhs)
			: string_ptr(rhs.string_ptr),
			number_of_strings(rhs.number_of_strings),
			max_number_of_strings(rhs.number_of_strings)
		{
			rhs.string_ptr = nullptr;
			rhs.number_of_strings = 0;
			rhs.max_number_of_strings = 0;
		}
		welp::string_cat<0, string_Ty, _Allocator>& operator=(welp::string_cat<0, string_Ty, _Allocator>&& rhs)
		{
			string_ptr = rhs.string_ptr;
			number_of_strings = rhs.number_of_strings;
			max_number_of_strings = rhs.number_of_strings;

			rhs.string_ptr = nullptr;
			rhs.number_of_strings = 0;
			rhs.max_number_of_strings = 0;
		}
		~string_cat()
		{
			if (string_ptr != nullptr)
			{
				this->deallocate(string_ptr, max_number_of_strings);
			}
		}

	private:

		const string_Ty** string_ptr = nullptr;
		std::size_t number_of_strings = 0;
		std::size_t max_number_of_strings = 0;

		class _temp_string_cat : private _Allocator
		{

		public:

			const string_Ty** temp_string_ptr = nullptr;
			std::size_t temp_number_of_strings = 0;

			_temp_string_cat() = delete;
			_temp_string_cat(std::size_t instances) : temp_string_ptr(this->allocate(instances)), temp_number_of_strings(instances) {}
			_temp_string_cat(const _temp_string_cat&) = delete;
			_temp_string_cat& operator=(const _temp_string_cat&) = delete;
			_temp_string_cat(_temp_string_cat&&) = delete;
			_temp_string_cat& operator=(_temp_string_cat&&) = delete;
			~_temp_string_cat()
			{
				if (temp_string_ptr != nullptr)
				{
					this->deallocate(temp_string_ptr, temp_number_of_strings);
				}
			}
		};
	};
}


#endif // WELP_STRING_CAT_HPP


// welp_string_cat.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
