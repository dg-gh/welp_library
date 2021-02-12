// welp_xdim.hpp - last update : 12 / 02 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_XDIM_HPP
#define WELP_XDIM_HPP


////// INCLUDES //////

#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_XDIM_DEBUG_MODE)
#define WELP_XDIM_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#include <cstring>
#include <memory>

#ifdef WELP_XDIM_DEBUG_MODE
#include <cassert>
#endif // WELP_XDIM_DEBUG_MODE


////// DESCRIPTIONS //////

namespace welp
{
	enum xdim_layout
	{
		xdim_left,
		xdim_right,
		xdim_undef
	};

	template <class Ty, std::size_t dim, class _Allocator = std::allocator<Ty>> class xdim : private _Allocator
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;
		template <class ... _index_pack> inline const Ty& operator()(_index_pack&& ... indices) const noexcept;
		template <class ... _index_pack> inline Ty& operator()(_index_pack&& ... indices) noexcept;

		template <class ... _index_pack> void resize(welp::xdim_layout layout, _index_pack&& ... indices);
		inline std::size_t size(std::size_t index_number) const noexcept { return sizes[index_number]; }
		inline std::size_t size() const noexcept { return total_size; }
		inline welp::xdim_layout layout() const noexcept { return _layout; }
		inline void clear() noexcept;

		inline const Ty* data() const noexcept { return data_ptr; }
		inline Ty* data() noexcept { return data_ptr; }

		inline const Ty* begin() const noexcept { return data_ptr; }
		inline const Ty* end() const noexcept { return end_ptr; }
		inline Ty* begin() noexcept { return data_ptr; }
		inline Ty* end() noexcept { return end_ptr; }
		inline const Ty* cbegin() const noexcept { return data_ptr; }
		inline const Ty* cend() const noexcept { return end_ptr; }


		xdim() = default;
		template <class ... _index_pack> xdim(welp::xdim_layout memory_layout, _index_pack&& ... indices);
		xdim(const welp::xdim<Ty, dim, _Allocator>& rhs);
		welp::xdim<Ty, dim, _Allocator>& operator=(const welp::xdim<Ty, dim, _Allocator>& rhs);
		xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		welp::xdim<Ty, dim, _Allocator>& operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		~xdim();

	private:

		Ty* data_ptr = nullptr;
		Ty* end_ptr = nullptr;
		std::size_t total_size = 0;
		std::size_t offset_coeff[dim] = { 0 };
		std::size_t sizes[dim] = { 0 };
		welp::xdim_layout _layout = welp::xdim_undef;

		template <class ... _index_pack> inline void resize_left(_index_pack&& ... indices);
		template <class ... _index_pack> inline void resize_right(_index_pack&& ... indices);
	};

	template <class Ty, std::size_t dim> class xdim_ref
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;
		template <class ... _index_pack> inline const Ty& operator()(_index_pack&& ... indices) const noexcept;
		template <class ... _index_pack> inline Ty& operator()(_index_pack&& ... indices) noexcept;

		template <class ... _index_pack> void resize(welp::xdim_layout layout, _index_pack&& ... indices);
		inline std::size_t size(std::size_t index_number) const noexcept { return sizes[index_number]; }
		inline std::size_t size() const noexcept { return total_size; }
		inline welp::xdim_layout layout() const noexcept { return _layout; }
		inline void clear() noexcept;

		inline void set_data(Ty* new_data_ptr) noexcept { data_ptr = new_data_ptr; };
		inline const Ty* data() const noexcept { return data_ptr; }
		inline Ty* data() noexcept { return data_ptr; }

		inline const Ty* begin() const noexcept { return data_ptr; }
		inline const Ty* end() const noexcept { return end_ptr; }
		inline Ty* begin() noexcept { return data_ptr; }
		inline Ty* end() noexcept { return end_ptr; }
		inline const Ty* cbegin() const noexcept { return data_ptr; }
		inline const Ty* cend() const noexcept { return end_ptr; }


		xdim_ref() = default;
		template <class ... _index_pack> xdim_ref(Ty* new_data_ptr, welp::xdim_layout memory_layout, _index_pack&& ... indices);
		xdim_ref(const welp::xdim_ref<Ty, dim>& rhs) noexcept = default;
		welp::xdim_ref<Ty, dim>& operator=(const welp::xdim<Ty, dim>& rhs) noexcept = default;
		xdim_ref(welp::xdim_ref<Ty, dim>&& rhs) noexcept = default;
		welp::xdim<Ty, dim>& operator=(welp::xdim<Ty, dim>&& rhs) noexcept = default;
		~xdim_ref() = default;

	private:

		Ty* data_ptr = nullptr;
		Ty* end_ptr = nullptr;
		std::size_t total_size = 0;
		std::size_t offset_coeff[dim] = { 0 };
		std::size_t sizes[dim] = { 0 };
		welp::xdim_layout _layout = welp::xdim_undef;

		template <class ... _index_pack> inline void resize_left(_index_pack&& ... indices);
		template <class ... _index_pack> inline void resize_right(_index_pack&& ... indices);
	};

	template <class Ty, std::size_t dim> class xdim_const_ref
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		template <class ... _index_pack> inline const Ty& operator()(_index_pack&& ... indices) const noexcept;

		template <class ... _index_pack> void resize(welp::xdim_layout layout, _index_pack&& ... indices);
		inline std::size_t size(std::size_t index_number) const noexcept { return sizes[index_number]; }
		inline std::size_t size() const noexcept { return total_size; }
		inline welp::xdim_layout layout() const noexcept { return _layout; }
		inline void clear() noexcept;

		inline void set_data(const Ty* new_data_ptr) noexcept { data_ptr = new_data_ptr; };
		inline const Ty* data() const noexcept { return data_ptr; }

		inline const Ty* begin() const noexcept { return data_ptr; }
		inline const Ty* end() const noexcept { return end_ptr; }
		inline const Ty* cbegin() const noexcept { return data_ptr; }
		inline const Ty* cend() const noexcept { return end_ptr; }


		xdim_const_ref() = default;
		template <class ... _index_pack> xdim_const_ref(const Ty* new_data_ptr, welp::xdim_layout memory_layout, _index_pack&& ... indices);
		xdim_const_ref(const welp::xdim_const_ref<Ty, dim>& rhs) noexcept = default;
		welp::xdim_const_ref<Ty, dim>& operator=(const welp::xdim<Ty, dim>& rhs) noexcept = default;
		xdim_const_ref(welp::xdim_const_ref<Ty, dim>&& rhs) noexcept = default;
		welp::xdim<Ty, dim>& operator=(welp::xdim<Ty, dim>&& rhs) noexcept = default;
		~xdim_const_ref() = default;

	private:

		const Ty* data_ptr = nullptr;
		const Ty* end_ptr = nullptr;
		std::size_t total_size = 0;
		std::size_t offset_coeff[dim] = { 0 };
		std::size_t sizes[dim] = { 0 };
		welp::xdim_layout _layout = welp::xdim_undef;

		template <class ... _index_pack> inline void resize_left(_index_pack&& ... indices);
		template <class ... _index_pack> inline void resize_right(_index_pack&& ... indices);
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

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline const Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_index_pack&& ... indices) noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
void welp::xdim<Ty, dim, _Allocator>::resize(welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline void welp::xdim<Ty, dim, _Allocator>::resize_left(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		data_ptr = this->allocate(_total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = _total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			sizes[n] = _indices[n];
		}
		offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			offset_coeff[n - 1] = offset_coeff[n] * _indices[n];
		}
		_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline void welp::xdim<Ty, dim, _Allocator>::resize_right(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		data_ptr = this->allocate(_total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = _total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			sizes[n] = _indices[n];
			offset_coeff[n + 1] = offset_coeff[n] * _indices[n];
		}
		sizes[dim_m1] = _indices[dim_m1];
		_layout = welp::xdim_right;
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
	}
	std::memset(this, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
	_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
welp::xdim<Ty, dim, _Allocator>::xdim(const welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	if (rhs.data_ptr != nullptr)
	{
		data_ptr = this->allocate(rhs.total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = rhs.total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		end_ptr = data_ptr + rhs.total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		ptr = data_ptr;
		Ty* rhs_ptr = rhs.data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
		_layout = rhs._layout;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	clear();

	if (rhs.data_ptr != nullptr)
	{
		data_ptr = this->allocate(rhs.total_size);
		Ty* ptr = data_ptr;
		for (std::size_t n = rhs.total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		end_ptr = data_ptr + rhs.total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		ptr = data_ptr;
		Ty* rhs_ptr = rhs.data_ptr;
		for (std::size_t n = total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
		_layout = rhs._layout;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	if (rhs.data_ptr != nullptr)
	{
		data_ptr = rhs.data_ptr;
		end_ptr = rhs.end_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		_layout = rhs._layout;
		std::memset(&rhs, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
		rhs._layout = welp::xdim_undef;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	clear();

	if (rhs.data_ptr != nullptr)
	{
		data_ptr = rhs.data_ptr;
		end_ptr = rhs.end_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			offset_coeff[n] = rhs.offset_coeff[n];
			sizes[n] = rhs.sizes[n];
		}
		total_size = rhs.total_size;
		_layout = rhs._layout;
		std::memset(&rhs, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
		rhs._layout = welp::xdim_undef;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::~xdim()
{
	clear();
}


template <class Ty, std::size_t dim>
inline const Ty& welp::xdim_ref<Ty, dim>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim>
inline Ty& welp::xdim_ref<Ty, dim>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline const Ty& welp::xdim_ref<Ty, dim>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline Ty& welp::xdim_ref<Ty, dim>::operator()(_index_pack&& ... indices) noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
void welp::xdim_ref<Ty, dim>::resize(welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline void welp::xdim_ref<Ty, dim>::resize_left(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			sizes[n] = _indices[n];
		}
		offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			offset_coeff[n - 1] = offset_coeff[n] * _indices[n];
		}
		_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline void welp::xdim_ref<Ty, dim>::resize_right(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			sizes[n] = _indices[n];
			offset_coeff[n + 1] = offset_coeff[n] * _indices[n];
		}
		sizes[dim_m1] = _indices[dim_m1];
		_layout = welp::xdim_right;
	}
}

template <class Ty, std::size_t dim>
inline void welp::xdim_ref<Ty, dim>::clear() noexcept
{
	std::memset(this, 0, sizeof(welp::xdim_ref<Ty, dim>));
	_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
welp::xdim_ref<Ty, dim>::xdim_ref(Ty* new_data_ptr, const welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	data_ptr = new_data_ptr;

	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
}


template <class Ty, std::size_t dim>
inline const Ty& welp::xdim_const_ref<Ty, dim>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline const Ty& welp::xdim_const_ref<Ty, dim>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * offset_coeff[n];
	}
	return *(data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
void welp::xdim_const_ref<Ty, dim>::resize(welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline void welp::xdim_const_ref<Ty, dim>::resize_left(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			sizes[n] = _indices[n];
		}
		offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			offset_coeff[n - 1] = offset_coeff[n] * _indices[n];
		}
		_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline void welp::xdim_const_ref<Ty, dim>::resize_right(_index_pack&& ... indices)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		end_ptr = data_ptr + _total_size;
		total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			sizes[n] = _indices[n];
			offset_coeff[n + 1] = offset_coeff[n] * _indices[n];
		}
		sizes[dim_m1] = _indices[dim_m1];
		_layout = welp::xdim_right;
	}
}

template <class Ty, std::size_t dim>
inline void welp::xdim_const_ref<Ty, dim>::clear() noexcept
{
	std::memset(this, 0, sizeof(welp::xdim_const_ref<Ty, dim>));
	_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
welp::xdim_const_ref<Ty, dim>::xdim_const_ref(const Ty* new_data_ptr, const welp::xdim_layout memory_layout, _index_pack&& ... indices)
{
	data_ptr = new_data_ptr;

	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_index_pack>(indices)...);
		break;

	case welp::xdim_undef:
		break;
	}
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
