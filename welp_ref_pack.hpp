// welp_ref_pack.hpp - last update : 14 / 01 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_REF_PACK_HPP
#define WELP_REF_PACK_HPP


////// INCLUDES //////

#include <cstddef>


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_REF_PACK_DEBUG_MODE)
#define WELP_REF_PACK_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_REF_PACK_DEBUG_MODE
#include <cassert>
#endif // WELP_REF_PACK_DEBUG_MODE


////// DESCRIPTIONS //////

namespace welp
{
	template <class Ty, std::size_t max_number_of_refs> class ref_pack
	{

	public:

		inline ref_pack<Ty, max_number_of_refs>& operator<<(Ty& str);

		inline void clear() noexcept;
		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		inline void pop_back() noexcept;
		inline void pop_back(std::size_t instances) noexcept;

		class iterator;
		inline welp::ref_pack<Ty, max_number_of_refs>::iterator begin() noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::iterator(static_cast<Ty**>(ptr_array));
		}
		inline welp::ref_pack<Ty, max_number_of_refs>::iterator end() noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::iterator(static_cast<Ty**>(current_ptr));
		}

		class const_iterator;
		inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator begin() const noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(ptr_array));
		}
		inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator end() const noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(current_ptr));
		}
		inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator cbegin() const noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(ptr_array));
		}
		inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator cend() const noexcept {
			return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(current_ptr));
		}

		ref_pack() = default;
		ref_pack(const welp::ref_pack<Ty, max_number_of_refs>&) = default;
		welp::ref_pack<Ty, max_number_of_refs>& operator=(const welp::ref_pack<Ty, max_number_of_refs>&) = default;
		ref_pack(welp::ref_pack<Ty, max_number_of_refs>&&) = default;
		welp::ref_pack<Ty, max_number_of_refs>& operator=(welp::ref_pack<Ty, max_number_of_refs>&&) = default;
		~ref_pack() = default;

	private:

		Ty* ptr_array[max_number_of_refs] = { nullptr };
		Ty** current_ptr = static_cast<Ty**>(ptr_array);

	public:

		class iterator
		{

		public:

			using value_type = Ty;
			using pointer = Ty*; using const_pointer = const Ty*;
			using reference = Ty&; using const_reference = const Ty&;
			using size_type = std::size_t; using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;

			inline const Ty& operator*() const noexcept { return **internal_ptr; }
			inline Ty& operator*() noexcept { return **internal_ptr; }
			inline const Ty* operator->() const noexcept { return *internal_ptr; }
			inline Ty* operator->() noexcept { return *internal_ptr; }
			inline const Ty& operator[](std::ptrdiff_t offset) const noexcept { return **(internal_ptr + offset); }
			inline Ty& operator[](std::ptrdiff_t offset) noexcept { return **(internal_ptr + offset); }

			inline welp::ref_pack<Ty, max_number_of_refs>::iterator& operator+=(std::ptrdiff_t offset) noexcept { internal_ptr += offset; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::iterator& operator++() noexcept { internal_ptr++; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::iterator operator++(int) noexcept {
				welp::ref_pack<Ty, max_number_of_refs>::iterator temp_iterator = *this;
				internal_ptr++; return temp_iterator;
			}

			inline welp::ref_pack<Ty, max_number_of_refs>::iterator& operator-=(std::ptrdiff_t offset) noexcept { internal_ptr -= offset; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::iterator& operator--() noexcept { internal_ptr--; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::iterator operator--(int) noexcept {
				welp::ref_pack<Ty, max_number_of_refs>::iterator temp_iterator = *this;
				internal_ptr--; return temp_iterator;
			}

			inline welp::ref_pack<Ty, max_number_of_refs>::iterator operator+(std::size_t offset) const noexcept {
				return welp::ref_pack<Ty, max_number_of_refs>::iterator(internal_ptr + offset);
			}
			inline welp::ref_pack<Ty, max_number_of_refs>::iterator operator-(std::size_t offset) const noexcept {
				return welp::ref_pack<Ty, max_number_of_refs>::iterator(internal_ptr - offset);
			}
			inline std::ptrdiff_t operator-(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept {
				return internal_ptr - rhs.internal_ptr;
			}

			inline bool operator==(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr == rhs.internal_ptr; }
			inline bool operator!=(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr != rhs.internal_ptr; }
			inline bool operator<(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr < rhs.internal_ptr; }
			inline bool operator>(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr > rhs.internal_ptr; }
			inline bool operator<=(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr <= rhs.internal_ptr; }
			inline bool operator>=(const welp::ref_pack<Ty, max_number_of_refs>::iterator& rhs) const noexcept { return internal_ptr >= rhs.internal_ptr; }

			iterator() = default;
			iterator(Ty** ptr) : internal_ptr(ptr) {}
			iterator(const welp::ref_pack<Ty, max_number_of_refs>::iterator&) = default;
			welp::ref_pack<Ty, max_number_of_refs>::iterator& operator=(
				const welp::ref_pack<Ty, max_number_of_refs>::iterator&) = default;
			iterator(welp::ref_pack<Ty, max_number_of_refs>::iterator&&) = default;
			welp::ref_pack<Ty, max_number_of_refs>::iterator& operator=(
				welp::ref_pack<Ty, max_number_of_refs>::iterator&&) = default;
			~iterator() = default;

		private:

			Ty** internal_ptr = nullptr;
		};

		class const_iterator
		{

		public:

			using value_type = Ty;
			using pointer = Ty*; using const_pointer = const Ty*;
			using reference = Ty&; using const_reference = const Ty&;
			using size_type = std::size_t; using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;

			inline const Ty& operator*() const noexcept { return **internal_ptr; }
			inline const Ty* operator->() const noexcept { return *internal_ptr; }
			inline const Ty& operator[](std::ptrdiff_t offset) const noexcept { return **(internal_ptr + offset); }

			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator+=(std::ptrdiff_t offset) noexcept { internal_ptr += offset; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator++() noexcept { internal_ptr++; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator operator++(int) noexcept {
				welp::ref_pack<Ty, max_number_of_refs>::const_iterator temp_iterator = *this;
				internal_ptr++; return temp_iterator;
			}

			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator-=(std::ptrdiff_t offset) noexcept { internal_ptr -= offset; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator--() noexcept { internal_ptr--; return *this; }
			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator operator--(int) noexcept {
				welp::ref_pack<Ty, max_number_of_refs>::const_iterator temp_iterator = *this;
				internal_ptr--; return temp_iterator;
			}

			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator operator+(std::size_t offset) const noexcept {
				return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(internal_ptr + offset);
			}
			inline welp::ref_pack<Ty, max_number_of_refs>::const_iterator operator-(std::size_t offset) const noexcept {
				return welp::ref_pack<Ty, max_number_of_refs>::const_iterator(internal_ptr - offset);
			}
			inline std::ptrdiff_t operator-(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept {
				return internal_ptr - rhs.internal_ptr;
			}

			inline bool operator==(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr == rhs.internal_ptr; }
			inline bool operator!=(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr != rhs.internal_ptr; }
			inline bool operator<(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr < rhs.internal_ptr; }
			inline bool operator>(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr > rhs.internal_ptr; }
			inline bool operator<=(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr <= rhs.internal_ptr; }
			inline bool operator>=(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr >= rhs.internal_ptr; }

			const_iterator() = default;
			const_iterator(const Ty* const* ptr) : internal_ptr(ptr) {}
			const_iterator(const welp::ref_pack<Ty, max_number_of_refs>::const_iterator&) = default;
			welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator=(
				const welp::ref_pack<Ty, max_number_of_refs>::const_iterator&) = default;
			const_iterator(welp::ref_pack<Ty, max_number_of_refs>::const_iterator&&) = default;
			welp::ref_pack<Ty, max_number_of_refs>::const_iterator& operator=(
				welp::ref_pack<Ty, max_number_of_refs>::const_iterator&&) = default;
			~const_iterator() = default;

		private:

			const Ty* const* internal_ptr = nullptr;
		};
	};

	template <class Ty, std::size_t max_number_of_refs> class const_ref_pack
	{

	public:

		inline const_ref_pack<Ty, max_number_of_refs>& operator<<(const Ty& str);

		inline void clear() noexcept;
		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		inline void pop_back() noexcept;
		inline void pop_back(std::size_t instances) noexcept;

		class const_iterator;
		inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator begin() const noexcept {
			return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(ptr_array));
		}
		inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator end() const noexcept {
			return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(current_ptr));
		}
		inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator cbegin() const noexcept {
			return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(ptr_array));
		}
		inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator cend() const noexcept {
			return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(static_cast<const Ty* const*>(current_ptr));
		}

		const_ref_pack() = default;
		const_ref_pack(const welp::const_ref_pack<Ty, max_number_of_refs>&) = default;
		welp::const_ref_pack<Ty, max_number_of_refs>& operator=(const welp::const_ref_pack<Ty, max_number_of_refs>&) = default;
		const_ref_pack(welp::const_ref_pack<Ty, max_number_of_refs>&&) = default;
		welp::const_ref_pack<Ty, max_number_of_refs>& operator=(welp::const_ref_pack<Ty, max_number_of_refs>&&) = default;
		~const_ref_pack() = default;

	private:

		const Ty* ptr_array[max_number_of_refs] = { nullptr };
		const Ty** current_ptr = static_cast<const Ty**>(ptr_array);

	public:

		class const_iterator
		{

		public:

			using value_type = Ty;
			using pointer = Ty*; using const_pointer = const Ty*;
			using reference = Ty&; using const_reference = const Ty&;
			using size_type = std::size_t; using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;

			inline const Ty& operator*() const noexcept { return **internal_ptr; }
			inline const Ty* operator->() const noexcept { return *internal_ptr; }
			inline const Ty& operator[](std::ptrdiff_t offset) const noexcept { return **(internal_ptr + offset); }

			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator+=(std::ptrdiff_t offset) noexcept { internal_ptr += offset; return *this; }
			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator++() noexcept { internal_ptr++; return *this; }
			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator operator++(int) noexcept {
				welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator temp_iterator = *this;
				internal_ptr++; return temp_iterator;
			}

			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator-=(std::ptrdiff_t offset) noexcept { internal_ptr -= offset; return *this; }
			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator--() noexcept { internal_ptr--; return *this; }
			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator operator--(int) noexcept {
				welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator temp_iterator = *this;
				internal_ptr--; return temp_iterator;
			}

			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator operator+(std::size_t offset) const noexcept {
				return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(internal_ptr + offset);
			}
			inline welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator operator-(std::size_t offset) const noexcept {
				return welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator(internal_ptr - offset);
			}
			inline std::ptrdiff_t operator-(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept {
				return internal_ptr - rhs.internal_ptr;
			}

			inline bool operator==(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr == rhs.internal_ptr; }
			inline bool operator!=(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr != rhs.internal_ptr; }
			inline bool operator<(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr < rhs.internal_ptr; }
			inline bool operator>(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr > rhs.internal_ptr; }
			inline bool operator<=(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr <= rhs.internal_ptr; }
			inline bool operator>=(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& rhs) const noexcept { return internal_ptr >= rhs.internal_ptr; }

			const_iterator() = default;
			const_iterator(const Ty* const* ptr) : internal_ptr(ptr) {}
			const_iterator(const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator&) = default;
			welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator=(
				const welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator&) = default;
			const_iterator(welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator&&) = default;
			welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator& operator=(
				welp::const_ref_pack<Ty, max_number_of_refs>::const_iterator&&) = default;
			~const_iterator() = default;

		private:

			const Ty* const* internal_ptr = nullptr;
		};
	};
}


////// IMPLEMENTATIONS //////

template <class Ty, std::size_t max_number_of_refs>
inline welp::ref_pack<Ty, max_number_of_refs>& welp::ref_pack<Ty, max_number_of_refs>::operator<<(Ty& rhs)
{
#ifdef WELP_REF_PACK_DEBUG_MODE
	assert(static_cast<std::size_t>(current_ptr - static_cast<Ty**>(ptr_array)) < max_number_of_refs);
#endif // WELP_REF_PACK_DEBUG_MODE
	*current_ptr++ = &rhs;
	return *this;
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::ref_pack<Ty, max_number_of_refs>::clear() noexcept
{
	current_ptr = static_cast<Ty**>(ptr_array);
}

template <class Ty, std::size_t max_number_of_refs>
inline std::size_t welp::ref_pack<Ty, max_number_of_refs>::size() const noexcept
{
	return static_cast<std::size_t>(current_ptr - static_cast<Ty* const*>(ptr_array));
}

template <class Ty, std::size_t max_number_of_refs>
constexpr std::size_t welp::ref_pack<Ty, max_number_of_refs>::capacity() const noexcept
{
	return max_number_of_refs;
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::ref_pack<Ty, max_number_of_refs>::pop_back() noexcept
{
	if (current_ptr != static_cast<Ty**>(ptr_array)) { current_ptr--; }
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::ref_pack<Ty, max_number_of_refs>::pop_back(std::size_t instances) noexcept
{
	if ((current_ptr - static_cast<Ty**>(ptr_array)) > instances)
	{
		current_ptr -= instances;
	}
	else
	{
		current_ptr = static_cast<Ty**>(ptr_array);
	}
}


template <class Ty, std::size_t max_number_of_refs>
inline welp::const_ref_pack<Ty, max_number_of_refs>& welp::const_ref_pack<Ty, max_number_of_refs>::operator<<(const Ty& rhs)
{
#ifdef WELP_REF_PACK_DEBUG_MODE
	assert(static_cast<std::size_t>(current_ptr - static_cast<const Ty**>(ptr_array)) < max_number_of_refs);
#endif // WELP_REF_PACK_DEBUG_MODE
	*current_ptr++ = &rhs;
	return *this;
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::const_ref_pack<Ty, max_number_of_refs>::clear() noexcept
{
	current_ptr = static_cast<const Ty**>(ptr_array);
}

template <class Ty, std::size_t max_number_of_refs>
inline std::size_t welp::const_ref_pack<Ty, max_number_of_refs>::size() const noexcept
{
	return static_cast<std::size_t>(current_ptr - static_cast<const Ty* const*>(ptr_array));
}

template <class Ty, std::size_t max_number_of_refs>
constexpr std::size_t welp::const_ref_pack<Ty, max_number_of_refs>::capacity() const noexcept
{
	return max_number_of_refs;
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::const_ref_pack<Ty, max_number_of_refs>::pop_back() noexcept
{
	if (current_ptr != static_cast<const Ty**>(ptr_array)) { current_ptr--; }
}

template <class Ty, std::size_t max_number_of_refs>
inline void welp::const_ref_pack<Ty, max_number_of_refs>::pop_back(std::size_t instances) noexcept
{
	if ((current_ptr - static_cast<const Ty**>(ptr_array)) > instances)
	{
		current_ptr -= instances;
	}
	else
	{
		current_ptr = static_cast<const Ty**>(ptr_array);
	}
}


#endif // WELP_REF_PACK_HPP


// welp_ref_pack.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
