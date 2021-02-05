// welp_xdim.hpp - last update : 05 / 02 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_XDIM_HPP
#define WELP_XDIM_HPP


////// INCLUDES //////

#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_XDIM_DEBUG_MODE)
#define WELP_XDIM_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#include <cstdlib>
#include <memory>
#include <initializer_list>

#ifdef WELP_XDIM_DEBUG_MODE
#include <cassert>
#endif // WELP_XDIM_DEBUG_MODE


////// DESCRIPTIONS //////

namespace welp
{
	template <class Ty, std::size_t dim, class _Allocator = std::allocator<Ty>> class xdim : private _Allocator
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;
		template <class ... _Args> inline const Ty& operator()(_Args&& ... args) const noexcept;
		template <class ... _Args> inline Ty& operator()(_Args&& ... args) noexcept;

		inline std::size_t size(std::size_t index) const noexcept;
		inline std::size_t size() const noexcept;

		inline const Ty* data() const noexcept;
		inline Ty* data() noexcept;

		template <class ... _Args> void resize_le(_Args&& ... args);
		template <class ... _Args> void resize_be(_Args&& ... args);
		inline void clear() noexcept;

		xdim() = default;
		xdim(const welp::xdim<Ty, dim, _Allocator>& rhs);
		welp::xdim<Ty, dim, _Allocator>& operator=(const welp::xdim<Ty, dim, _Allocator>& rhs);
		xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		welp::xdim<Ty, dim, _Allocator>& operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		~xdim();

	private:

		Ty* data_ptr = nullptr;
		std::size_t offset_coeff[dim] = { 0 };
		std::size_t sizes[dim] = { 0 };
		std::size_t total_size = 0;
	};
}


////// IMPLEMENTATIONS //////

template <class Ty, std::size_t dim, class _Allocator>
inline const Ty& welp::xdim<Ty, dim, _Allocator>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator>
inline Ty& welp::xdim<Ty, dim, _Allocator>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _Args>
inline const Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_Args&& ... args) const noexcept
{
	std::size_t indices[dim] = { static_cast<std::size_t>(args)... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(indices[n] < sizes[n]); }
	assert(sizeof...(args) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _Args>
inline Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_Args&& ... args) noexcept
{
	std::size_t indices[dim] = { static_cast<std::size_t>(args)... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(indices[n] < sizes[n]); }
	assert(sizeof...(args) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator>
inline std::size_t welp::xdim<Ty, dim, _Allocator>::size(std::size_t index) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(index < dim);
#endif // WELP_XDIM_DEBUG_MODE
	return sizes[index];
}

template <class Ty, std::size_t dim, class _Allocator>
inline std::size_t welp::xdim<Ty, dim, _Allocator>::size() const noexcept
{
	return total_size;
}

template <class Ty, std::size_t dim, class _Allocator>
inline const Ty* welp::xdim<Ty, dim, _Allocator>::data() const noexcept
{
	return data_ptr;
}

template <class Ty, std::size_t dim, class _Allocator>
inline Ty* welp::xdim<Ty, dim, _Allocator>::data() noexcept
{
	return data_ptr;
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _Args>
void welp::xdim<Ty, dim, _Allocator>::resize_le(_Args&& ... args)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(args) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::initializer_list<std::size_t> L = { static_cast<std::size_t>(args)... };
	std::copy(L.begin(), L.end(), sizes);
	total_size = 1;
	std::size_t dim_m1 = dim - 1;
	offset_coeff[0] = 1;
	for (std::size_t n = 0; n < dim_m1; n++)
	{
		total_size *= sizes[n];
		offset_coeff[n + 1] = total_size;
	}
	total_size *= sizes[dim_m1];
	if (total_size != 0)
	{
		data_ptr = this->allocate(total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
	}
	else
	{
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = 0;
			sizes[n] = 0;
		}
		total_size = 0;
	}
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _Args>
void welp::xdim<Ty, dim, _Allocator>::resize_be(_Args&& ... args)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(args) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::initializer_list<std::size_t> L = { static_cast<std::size_t>(args)... };
	std::copy(L.begin(), L.end(), sizes);
	total_size = 1;
	std::size_t dim_m1 = dim - 1;
	offset_coeff[dim_m1] = 1;
	for (std::size_t n = 0; n < dim_m1; n++)
	{
		total_size *= sizes[n];
		offset_coeff[dim_m1 - 1 - n] = total_size;
	}
	total_size *= sizes[dim_m1];
	if (total_size != 0)
	{
		data_ptr = this->allocate(total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
	}
	else
	{
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = 0;
			sizes[n] = 0;
		}
		total_size = 0;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
inline void welp::xdim<Ty, dim, _Allocator>::clear() noexcept
{
	if (data_ptr != nullptr)
	{
		Ty* ptr = data_ptr + (total_size - 1);
		for (std::size_t n = total_size; n > 0; n--)
		{
			ptr->~Ty(); ptr--;
		}
		this->deallocate(data_ptr, total_size);
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = 0;
			sizes[n] = 0;
		}
		total_size = 0;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	if (rhs.data_ptr != nullptr)
	{
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		data_ptr = this->allocate(total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		ptr = data_ptr;
		Ty* rhs_ptr = rhs.data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	clear();

	if (rhs.data_ptr != nullptr)
	{
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		data_ptr = this->allocate(total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		ptr = data_ptr;
		Ty* rhs_ptr = rhs.data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	if (rhs.data_ptr != nullptr)
	{
		data_ptr = rhs.data_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		rhs.data_ptr = nullptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			rhs.offset_coeff[n] = 0;
			rhs.sizes[n] = 0;
		}
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	clear();

	if (rhs.data_ptr != nullptr)
	{
		data_ptr = rhs.data_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		rhs.data_ptr = nullptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			rhs.offset_coeff[n] = 0;
			rhs.sizes[n] = 0;
		}
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::~xdim()
{
	clear();
}


#endif // WELP_XDIM_HPP


// welp_xdim.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
