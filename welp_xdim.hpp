// welp_xdim.hpp - last update : 15 / 02 / 2021
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

		template <class ... _size_pack> void resize(welp::xdim_layout layout, _size_pack&& ... sizes);
		inline std::size_t size(std::size_t index_number) const noexcept { return m_sizes[index_number]; }
		inline std::size_t size() const noexcept { return m_total_size; }
		inline welp::xdim_layout layout() const noexcept { return m_layout; }
		inline void clear() noexcept;

		inline const Ty* data() const noexcept { return m_data_ptr; }
		inline Ty* data() noexcept { return m_data_ptr; }

		inline const Ty* begin() const noexcept { return m_data_ptr; }
		inline const Ty* end() const noexcept { return m_end_ptr; }
		inline Ty* begin() noexcept { return m_data_ptr; }
		inline Ty* end() noexcept { return m_end_ptr; }
		inline const Ty* cbegin() const noexcept { return m_data_ptr; }
		inline const Ty* cend() const noexcept { return m_end_ptr; }


		xdim() noexcept = default;
		template <class ... _size_pack> xdim(welp::xdim_layout memory_layout, _size_pack&& ... sizes);
		xdim(const welp::xdim<Ty, dim, _Allocator>& rhs);
		welp::xdim<Ty, dim, _Allocator>& operator=(const welp::xdim<Ty, dim, _Allocator>& rhs);
		xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		welp::xdim<Ty, dim, _Allocator>& operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept;
		~xdim();

	private:

		Ty* m_data_ptr = nullptr;
		Ty* m_end_ptr = nullptr;
		std::size_t m_total_size = 0;
		std::size_t m_offset_coeff[dim] = { 0 };
		std::size_t m_sizes[dim] = { 0 };
		welp::xdim_layout m_layout = welp::xdim_undef;

		template <class ... _size_pack> inline void resize_left(_size_pack&& ... sizes);
		template <class ... _size_pack> inline void resize_right(_size_pack&& ... sizes);
	};

	template <class Ty, std::size_t dim> class xdim_view
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;
		template <class ... _index_pack> inline const Ty& operator()(_index_pack&& ... indices) const noexcept;
		template <class ... _index_pack> inline Ty& operator()(_index_pack&& ... indices) noexcept;

		template <class ... _size_pack> void resize(welp::xdim_layout layout, _size_pack&& ... sizes);
		inline std::size_t size(std::size_t index_number) const noexcept { return m_sizes[index_number]; }
		inline std::size_t size() const noexcept { return m_total_size; }
		inline welp::xdim_layout layout() const noexcept { return m_layout; }
		inline void clear() noexcept;

		inline void set_data(Ty* data_ptr) noexcept { m_data_ptr = data_ptr; m_end_ptr = m_data_ptr + m_total_size; };
		inline const Ty* data() const noexcept { return m_data_ptr; }
		inline Ty* data() noexcept { return m_data_ptr; }

		inline const Ty* begin() const noexcept { return m_data_ptr; }
		inline const Ty* end() const noexcept { return m_end_ptr; }
		inline Ty* begin() noexcept { return m_data_ptr; }
		inline Ty* end() noexcept { return m_end_ptr; }
		inline const Ty* cbegin() const noexcept { return m_data_ptr; }
		inline const Ty* cend() const noexcept { return m_end_ptr; }


		xdim_view() noexcept = default;
		template <class ... _size_pack> xdim_view(Ty* data_ptr, welp::xdim_layout memory_layout, _size_pack&& ... sizes);
		xdim_view(const welp::xdim_view<Ty, dim>& rhs) noexcept = default;
		welp::xdim_view<Ty, dim>& operator=(const welp::xdim_view<Ty, dim>& rhs) noexcept = default;
		xdim_view(welp::xdim_view<Ty, dim>&& rhs) noexcept = default;
		welp::xdim_view<Ty, dim>& operator=(welp::xdim_view<Ty, dim>&& rhs) noexcept = default;
		~xdim_view() = default;

	private:

		Ty* m_data_ptr = nullptr;
		Ty* m_end_ptr = nullptr;
		std::size_t m_total_size = 0;
		std::size_t m_offset_coeff[dim] = { 0 };
		std::size_t m_sizes[dim] = { 0 };
		welp::xdim_layout m_layout = welp::xdim_undef;

		template <class ... _size_pack> inline void resize_left(_size_pack&& ... sizes);
		template <class ... _size_pack> inline void resize_right(_size_pack&& ... sizes);
	};

	template <class Ty, std::size_t dim> class xdim_view_const
	{

	public:

		inline const Ty& operator[](std::size_t offset) const noexcept;
		template <class ... _index_pack> inline const Ty& operator()(_index_pack&& ... indices) const noexcept;

		template <class ... _size_pack> void resize(welp::xdim_layout layout, _size_pack&& ... sizes);
		inline std::size_t size(std::size_t index_number) const noexcept { return m_sizes[index_number]; }
		inline std::size_t size() const noexcept { return m_total_size; }
		inline welp::xdim_layout layout() const noexcept { return m_layout; }
		inline void clear() noexcept;

		inline void set_data(const Ty* data_ptr) noexcept { m_data_ptr = data_ptr; m_end_ptr = m_data_ptr + m_total_size; };
		inline const Ty* data() const noexcept { return m_data_ptr; }

		inline const Ty* begin() const noexcept { return m_data_ptr; }
		inline const Ty* end() const noexcept { return m_end_ptr; }
		inline const Ty* cbegin() const noexcept { return m_data_ptr; }
		inline const Ty* cend() const noexcept { return m_end_ptr; }


		xdim_view_const() noexcept = default;
		template <class ... _size_pack> xdim_view_const(const Ty* data_ptr, welp::xdim_layout memory_layout, _size_pack&& ... sizes);
		xdim_view_const(const welp::xdim_view_const<Ty, dim>& rhs) noexcept = default;
		welp::xdim_view_const<Ty, dim>& operator=(const welp::xdim_view_const<Ty, dim>& rhs) noexcept = default;
		xdim_view_const(welp::xdim_view_const<Ty, dim>&& rhs) noexcept = default;
		welp::xdim_view_const<Ty, dim>& operator=(welp::xdim_view_const<Ty, dim>&& rhs) noexcept = default;
		~xdim_view_const() = default;

	private:

		const Ty* m_data_ptr = nullptr;
		const Ty* m_end_ptr = nullptr;
		std::size_t m_total_size = 0;
		std::size_t m_offset_coeff[dim] = { 0 };
		std::size_t m_sizes[dim] = { 0 };
		welp::xdim_layout m_layout = welp::xdim_undef;

		template <class ... _size_pack> inline void resize_left(_size_pack&& ... sizes);
		template <class ... _size_pack> inline void resize_right(_size_pack&& ... sizes);
	};
}


////// IMPLEMENTATIONS //////

template <class Ty, std::size_t dim, class _Allocator>
inline const Ty& welp::xdim<Ty, dim, _Allocator>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator>
inline Ty& welp::xdim<Ty, dim, _Allocator>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline const Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * m_offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * m_offset_coeff[n];
	}
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _index_pack>
inline Ty& welp::xdim<Ty, dim, _Allocator>::operator()(_index_pack&& ... indices) noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * m_offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * m_offset_coeff[n];
	}
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _size_pack>
void welp::xdim<Ty, dim, _Allocator>::resize(welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _size_pack>
inline void welp::xdim<Ty, dim, _Allocator>::resize_left(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_size_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_data_ptr = this->allocate(_total_size);
		Ty* ptr = m_data_ptr;
		for (std::size_t n = _total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_sizes[n] = _indices[n];
		}
		m_offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			m_offset_coeff[n - 1] = m_offset_coeff[n] * _indices[n];
		}
		m_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _size_pack>
inline void welp::xdim<Ty, dim, _Allocator>::resize_right(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	clear();

	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_size_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_data_ptr = this->allocate(_total_size);
		Ty* ptr = m_data_ptr;
		for (std::size_t n = _total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		m_offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			m_sizes[n] = _indices[n];
			m_offset_coeff[n + 1] = m_offset_coeff[n] * _indices[n];
		}
		m_sizes[dim_m1] = _indices[dim_m1];
		m_layout = welp::xdim_right;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
inline void welp::xdim<Ty, dim, _Allocator>::clear() noexcept
{
	if (m_data_ptr != nullptr)
	{
		Ty* ptr = m_data_ptr + (m_total_size - 1);
		for (std::size_t n = m_total_size; n > 0; n--)
		{
			ptr->~Ty(); ptr--;
		}
		this->deallocate(m_data_ptr, m_total_size);
	}
	std::memset(this, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
	m_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim, class _Allocator> template <class ... _size_pack>
welp::xdim<Ty, dim, _Allocator>::xdim(const welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	if (rhs.m_data_ptr != nullptr)
	{
		m_data_ptr = this->allocate(rhs.m_total_size);
		Ty* ptr = m_data_ptr;
		for (std::size_t n = rhs.m_total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		m_end_ptr = m_data_ptr + rhs.m_total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_offset_coeff[n] = rhs.m_offset_coeff[n];
			m_sizes[n] = rhs.m_sizes[n];
		}
		m_total_size = rhs.m_total_size;
		ptr = m_data_ptr;
		Ty* rhs_ptr = rhs.m_data_ptr;
		for (std::size_t n = m_total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
		m_layout = rhs.m_layout;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(const welp::xdim<Ty, dim, _Allocator>& rhs)
{
	clear();

	if (rhs.m_data_ptr != nullptr)
	{
		m_data_ptr = this->allocate(rhs.m_total_size);
		Ty* ptr = m_data_ptr;
		for (std::size_t n = rhs.m_total_size; n > 0; n--)
		{
			new (ptr) Ty(); ptr++;
		}
		m_end_ptr = m_data_ptr + rhs.m_total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_offset_coeff[n] = rhs.m_offset_coeff[n];
			m_sizes[n] = rhs.m_sizes[n];
		}
		m_total_size = rhs.m_total_size;
		ptr = m_data_ptr;
		Ty* rhs_ptr = rhs.m_data_ptr;
		for (std::size_t n = m_total_size; n > 0; n--)
		{
			*ptr++ = *rhs_ptr++;
		}
		m_layout = rhs.m_layout;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::xdim(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	if (rhs.m_data_ptr != nullptr)
	{
		m_data_ptr = rhs.m_data_ptr;
		m_end_ptr = rhs.m_end_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_offset_coeff[n] = rhs.m_offset_coeff[n];
			m_sizes[n] = rhs.m_sizes[n];
		}
		m_total_size = rhs.m_total_size;
		m_layout = rhs.m_layout;
		std::memset(&rhs, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
		rhs.m_layout = welp::xdim_undef;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>& welp::xdim<Ty, dim, _Allocator>::operator=(welp::xdim<Ty, dim, _Allocator>&& rhs) noexcept
{
	clear();

	if (rhs.m_data_ptr != nullptr)
	{
		m_data_ptr = rhs.m_data_ptr;
		m_end_ptr = rhs.m_end_ptr;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_offset_coeff[n] = rhs.m_offset_coeff[n];
			m_sizes[n] = rhs.m_sizes[n];
		}
		m_total_size = rhs.m_total_size;
		m_layout = rhs.m_layout;
		std::memset(&rhs, 0, sizeof(welp::xdim<Ty, dim, _Allocator>));
		rhs.m_layout = welp::xdim_undef;
	}
}

template <class Ty, std::size_t dim, class _Allocator>
welp::xdim<Ty, dim, _Allocator>::~xdim()
{
	clear();
}


template <class Ty, std::size_t dim>
inline const Ty& welp::xdim_view<Ty, dim>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim>
inline Ty& welp::xdim_view<Ty, dim>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline const Ty& welp::xdim_view<Ty, dim>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * m_offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * m_offset_coeff[n];
	}
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline Ty& welp::xdim_view<Ty, dim>::operator()(_index_pack&& ... indices) noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * m_offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * m_offset_coeff[n];
	}
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
void welp::xdim_view<Ty, dim>::resize(welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
inline void welp::xdim_view<Ty, dim>::resize_left(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_sizes[n] = _indices[n];
		}
		m_offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			m_offset_coeff[n - 1] = m_offset_coeff[n] * _indices[n];
		}
		m_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
inline void welp::xdim_view<Ty, dim>::resize_right(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		m_offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			m_sizes[n] = _indices[n];
			m_offset_coeff[n + 1] = m_offset_coeff[n] * _indices[n];
		}
		m_sizes[dim_m1] = _indices[dim_m1];
		m_layout = welp::xdim_right;
	}
}

template <class Ty, std::size_t dim>
inline void welp::xdim_view<Ty, dim>::clear() noexcept
{
	std::memset(this, 0, sizeof(welp::xdim_view<Ty, dim>));
	m_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
welp::xdim_view<Ty, dim>::xdim_view(Ty* data_ptr, const welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	m_data_ptr = data_ptr;

	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_undef:
		break;
	}
}


template <class Ty, std::size_t dim>
inline const Ty& welp::xdim_view_const<Ty, dim>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(offset < total_size);
#endif // WELP_XDIM_DEBUG_MODE
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _index_pack>
inline const Ty& welp::xdim_view_const<Ty, dim>::operator()(_index_pack&& ... indices) const noexcept
{
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(indices))... };
#ifdef WELP_XDIM_DEBUG_MODE
	for (std::size_t n = 0; n < dim; n++) { assert(_indices[n] < sizes[n]); }
	assert(sizeof...(indices) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t offset = _indices[0] * m_offset_coeff[0];
	for (std::size_t n = 1; n < dim; n++)
	{
		offset += _indices[n] * m_offset_coeff[n];
	}
	return *(m_data_ptr + offset);
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
void welp::xdim_view_const<Ty, dim>::resize(welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_undef:
		break;
	}
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
inline void welp::xdim_view_const<Ty, dim>::resize_left(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		for (std::size_t n = 0; n < dim; n++)
		{
			m_sizes[n] = _indices[n];
		}
		m_offset_coeff[dim - 1] = 1;
		for (std::size_t n = dim - 1; n > 0; n--)
		{
			m_offset_coeff[n - 1] = m_offset_coeff[n] * _indices[n];
		}
		m_layout = welp::xdim_left;
	}
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
inline void welp::xdim_view_const<Ty, dim>::resize_right(_size_pack&& ... sizes)
{
#ifdef WELP_XDIM_DEBUG_MODE
	assert(sizeof...(sizes) == dim);
#endif // WELP_XDIM_DEBUG_MODE
	std::size_t _indices[dim] = { static_cast<std::size_t>(std::forward<_index_pack>(sizes))... };
	std::size_t _total_size = 1;
	for (std::size_t n = 0; n < dim; n++)
	{
		_total_size *= _indices[n];
	}
	if (_total_size != 0)
	{
		m_end_ptr = m_data_ptr + _total_size;
		m_total_size = _total_size;
		constexpr std::size_t dim_m1 = dim - 1;
		m_offset_coeff[0] = 1;
		for (std::size_t n = 0; n < dim_m1; n++)
		{
			m_sizes[n] = _indices[n];
			m_offset_coeff[n + 1] = m_offset_coeff[n] * _indices[n];
		}
		m_sizes[dim_m1] = _indices[dim_m1];
		m_layout = welp::xdim_right;
	}
}

template <class Ty, std::size_t dim>
inline void welp::xdim_view_const<Ty, dim>::clear() noexcept
{
	std::memset(this, 0, sizeof(welp::xdim_view_const<Ty, dim>));
	m_layout = welp::xdim_undef;
}

template <class Ty, std::size_t dim> template <class ... _size_pack>
welp::xdim_view_const<Ty, dim>::xdim_view_const(const Ty* data_ptr, const welp::xdim_layout memory_layout, _size_pack&& ... sizes)
{
	m_data_ptr = data_ptr;

	switch (memory_layout)
	{

	case welp::xdim_left:
		resize_left(std::forward<_size_pack>(sizes)...);
		break;

	case welp::xdim_right:
		resize_right(std::forward<_size_pack>(sizes)...);
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
