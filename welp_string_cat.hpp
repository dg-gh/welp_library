// welp_string_cat.hpp - last update : 12 / 01 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_STRING_CAT_HPP
#define WELP_STRING_CAT_HPP


////// INCLUDES //////

#include <string>
#include <cstring>


////// DESCRIPTIONS //////

namespace welp
{
	template <std::size_t max_number_of_strings, class string_Ty = std::string> class string_cat
	{

	private:

		const string_Ty* string_array[max_number_of_strings] = { nullptr };
		std::size_t number_of_strings = 0;

	public:

		string_cat<max_number_of_strings, string_Ty>& operator<<(const string_Ty& str);
		template <class return_string_Ty = string_Ty> return_string_Ty concatenate() const;

		inline void reset() noexcept;
		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		std::size_t concatenated_string_size() const noexcept;
	};
}


////// IMPLEMENTATIONS //////

template <std::size_t max_number_of_strings, class string_Ty>
welp::string_cat<max_number_of_strings, string_Ty>& welp::string_cat<max_number_of_strings, string_Ty>::operator<<(const string_Ty& str)
{
	string_array[number_of_strings] = &str;
	number_of_strings++;
	return *this;
}

template <std::size_t max_number_of_strings, class string_Ty>
template <class return_string_Ty> return_string_Ty welp::string_cat<max_number_of_strings, string_Ty>::concatenate() const
{
	const string_Ty* const* string_ptr = static_cast<const string_Ty* const*>(string_array);
	std::size_t string_size = 0;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		string_size += (*string_ptr++)->size();
	}
	string_ptr = static_cast<const string_Ty* const*>(string_array);
	return_string_Ty str_cat(string_size, static_cast<char>(0));
	char* string_cat_ptr = str_cat.data();
	std::size_t temp;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		temp = (*string_ptr)->size();
		std::memcpy(string_cat_ptr, (*string_ptr++)->data(), temp);
		string_cat_ptr += temp;
	}
	return str_cat;
}

template <std::size_t max_number_of_strings, class string_Ty>
inline void welp::string_cat<max_number_of_strings, string_Ty>::reset() noexcept
{
	number_of_strings = 0;
}

template <std::size_t max_number_of_strings, class string_Ty>
inline std::size_t welp::string_cat<max_number_of_strings, string_Ty>::size() const noexcept
{
	return number_of_strings;
}

template <std::size_t max_number_of_strings, class string_Ty>
constexpr std::size_t welp::string_cat<max_number_of_strings, string_Ty>::capacity() const noexcept
{
	return max_number_of_strings;
}

template <std::size_t max_number_of_strings, class string_Ty>
std::size_t welp::string_cat<max_number_of_strings, string_Ty>::concatenated_string_size() const noexcept
{
	const string_Ty* const* string_ptr = static_cast<const string_Ty* const*>(string_array);
	std::size_t string_size = 0;
	for (std::size_t k = number_of_strings; k > 0; k--)
	{
		string_size += (*string_ptr++)->size();
	}
	return string_size;
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
