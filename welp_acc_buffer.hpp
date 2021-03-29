// welp_acc_buffer.hpp - last update : 03 / 02 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_ACC_BUFFER_HPP
#define WELP_ACC_BUFFER_HPP


////// INCLUDES //////

#include <cstddef>
#include <memory>


#if defined(WELP_ACC_BUFFER_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_ACC_BUFFER_INCLUDE_MUTEX
#define WELP_ACC_BUFFER_INCLUDE_MUTEX
#endif
#endif // WELP_ACC_BUFFER_INCLUDE_ALL


#ifdef WELP_ACC_BUFFER_INCLUDE_MUTEX
#include <mutex>
#endif // WELP_ACC_BUFFER_INCLUDE_MUTEX

#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_ACC_BUFFER_DEBUG_MODE)
#define WELP_ACC_BUFFER_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_ACC_BUFFER_DEBUG_MODE
#include <cassert>
#endif // WELP_ACC_BUFFER_DEBUG_MODE


////// DESCRIPTIONS //////

namespace welp
{
	template <class Ty, class _Allocator = std::allocator<char>> class acc_buffer : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline welp::acc_buffer<Ty, _Allocator>& operator<<(const Ty& obj);
		inline welp::acc_buffer<Ty, _Allocator>& operator<<(Ty&& obj) noexcept;
		inline welp::acc_buffer<Ty, _Allocator>& operator<<(Ty* const obj_ptr) noexcept;
		inline welp::acc_buffer<Ty, _Allocator>& operator<(const Ty& obj);

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;

		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		inline void pop_back();
		inline void pop_back(std::size_t instances);
		inline void reset();

		class iterator;
		inline welp::acc_buffer<Ty, _Allocator>::iterator begin() noexcept { return welp::acc_buffer<Ty, _Allocator>::iterator(data_ptr); }
		inline welp::acc_buffer<Ty, _Allocator>::iterator end() noexcept { return welp::acc_buffer<Ty, _Allocator>::iterator(current_ptr); }

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		acc_buffer() = default;
		acc_buffer(std::size_t instances);
		acc_buffer(const welp::acc_buffer<Ty, _Allocator>&);
		welp::acc_buffer<Ty, _Allocator>& operator=(const welp::acc_buffer<Ty, _Allocator>&);
		acc_buffer(welp::acc_buffer<Ty, _Allocator>&&) noexcept;
		welp::acc_buffer<Ty, _Allocator>& operator=(welp::acc_buffer<Ty, _Allocator>&&) noexcept;
		~acc_buffer();

		class iterator
		{

		public:

			using value_type = storage_cell;
			using pointer = storage_cell*; using const_pointer = const storage_cell*;
			using reference = storage_cell&; using const_reference = const storage_cell&;
			using size_type = std::size_t; using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;


			inline Ty& operator*() noexcept
			{
				if (internal_ptr->storage_ptr != nullptr) { return *(internal_ptr->storage_ptr); }
				else { return internal_ptr->storage; }
			}
			inline Ty* operator->() noexcept
			{
				if (internal_ptr->storage_ptr != nullptr) { return internal_ptr->storage_ptr; }
				else { return &(internal_ptr->storage); }
			}
			inline Ty& operator[](std::ptrdiff_t offset) noexcept
			{
				storage_cell* temp_ptr = internal_ptr + offset;
				if (temp_ptr->storage_ptr != nullptr) { return *(temp_ptr->storage_ptr); }
				else { return temp_ptr->storage; }
			}

			inline welp::acc_buffer<Ty, _Allocator>::iterator& operator+=(std::ptrdiff_t offset) noexcept { internal_ptr += offset; return *this; }
			inline welp::acc_buffer<Ty, _Allocator>::iterator& operator++() noexcept { internal_ptr++; return *this; }
			inline welp::acc_buffer<Ty, _Allocator>::iterator operator++(int) noexcept {
				welp::acc_buffer<Ty, _Allocator>::iterator temp_iterator = *this;
				internal_ptr++; return temp_iterator;
			}

			inline welp::acc_buffer<Ty, _Allocator>::iterator& operator-=(std::ptrdiff_t offset) noexcept { internal_ptr -= offset; return *this; }
			inline welp::acc_buffer<Ty, _Allocator>::iterator& operator--() noexcept { internal_ptr--; return *this; }
			inline welp::acc_buffer<Ty, _Allocator>::iterator operator--(int) noexcept {
				welp::acc_buffer<Ty, _Allocator>::iterator temp_iterator = *this;
				internal_ptr--; return temp_iterator;
			}

			inline welp::acc_buffer<Ty, _Allocator>::iterator operator+(std::size_t offset) const noexcept {
				return welp::acc_buffer<Ty, _Allocator>::iterator(internal_ptr + offset);
			}
			inline welp::acc_buffer<Ty, _Allocator>::iterator operator-(std::size_t offset) const noexcept {
				return welp::acc_buffer<Ty, _Allocator>::iterator(internal_ptr - offset);
			}
			inline std::ptrdiff_t operator-(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept {
				return internal_ptr - rhs.internal_ptr;
			}

			inline bool operator==(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr == rhs.internal_ptr; }
			inline bool operator!=(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr != rhs.internal_ptr; }
			inline bool operator<(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr < rhs.internal_ptr; }
			inline bool operator>(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr > rhs.internal_ptr; }
			inline bool operator<=(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr <= rhs.internal_ptr; }
			inline bool operator>=(const welp::acc_buffer<Ty, _Allocator>::iterator& rhs) const noexcept { return internal_ptr >= rhs.internal_ptr; }

			iterator() = default;
			iterator(storage_cell* ptr) : internal_ptr(ptr) {}
			iterator(const welp::acc_buffer<Ty, _Allocator>::iterator&) = default;
			welp::acc_buffer<Ty, _Allocator>::iterator& operator=(const welp::acc_buffer<Ty, _Allocator>::iterator&) = default;
			iterator(welp::acc_buffer<Ty, _Allocator>::iterator&&) = default;
			welp::acc_buffer<Ty, _Allocator>::iterator& operator=(welp::acc_buffer<Ty, _Allocator>::iterator&&) = default;
			~iterator() = default;

		private:

			storage_cell* internal_ptr;
		};

	private:

		storage_cell* current_ptr = nullptr;
		storage_cell* data_ptr = nullptr;
		std::size_t max_number_of_cells = 0;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::acc_buffer<Ty, _Allocator>::storage_cell&) = default;
			welp::acc_buffer<Ty, _Allocator>::storage_cell& operator=(const welp::acc_buffer<Ty, _Allocator>::storage_cell&) = default;
			storage_cell(welp::acc_buffer<Ty, _Allocator>::storage_cell&&) = default;
			welp::acc_buffer<Ty, _Allocator>::storage_cell& operator=(welp::acc_buffer<Ty, _Allocator>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};

#ifdef WELP_ACC_BUFFER_INCLUDE_MUTEX
	template <class Ty, class _Allocator = std::allocator<char>, class mutex_Ty = std::mutex> class acc_buffer_sync : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator<<(const Ty& obj);
		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator<<(Ty&& obj) noexcept;
		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator<<(Ty* const obj_ptr) noexcept;
		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator<(const Ty& obj);

		inline const Ty& operator[](std::size_t offset) const noexcept;
		inline Ty& operator[](std::size_t offset) noexcept;

		inline std::size_t size() const noexcept;
		constexpr std::size_t capacity() const noexcept;
		inline void pop_back();
		inline void pop_back(std::size_t instances);
		inline void reset();

		class iterator;
		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator begin() noexcept
		{
			return welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator(data_ptr);
		}
		inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator end() noexcept
		{
			return welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator(current_ptr);
		}

		class lock;
		const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock block()
		{
			return welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock(this);
		}

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		acc_buffer_sync() = default;
		acc_buffer_sync(std::size_t instances);
		acc_buffer_sync(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&);
		welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&);
		acc_buffer_sync(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&&) noexcept;
		welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& operator=(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&&) noexcept;
		~acc_buffer_sync();

		class iterator
		{

		public:

			using value_type = storage_cell;
			using pointer = storage_cell*; using const_pointer = const storage_cell*;
			using reference = storage_cell&; using const_reference = const storage_cell&;
			using size_type = std::size_t; using difference_type = std::ptrdiff_t;
			using iterator_category = std::random_access_iterator_tag;

			inline Ty& operator*() noexcept
			{
				if (internal_ptr->storage_ptr != nullptr) { return *(internal_ptr->storage_ptr); }
				else { return internal_ptr->storage; }
			}
			inline Ty* operator->() noexcept
			{
				if (internal_ptr->storage_ptr != nullptr) { return internal_ptr->storage_ptr; }
				else { return &(internal_ptr->storage); }
			}
			inline Ty& operator[](std::ptrdiff_t offset) noexcept
			{
				storage_cell* temp_ptr = internal_ptr + offset;
				if (temp_ptr->storage_ptr != nullptr) { return *(temp_ptr->storage_ptr); }
				else { return temp_ptr->storage; }
			}

			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator+=(std::ptrdiff_t offset) noexcept { internal_ptr += offset; return *this; }
			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator++() noexcept { internal_ptr++; return *this; }
			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator operator++(int) noexcept {
				welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator temp_iterator = *this;
				internal_ptr++; return temp_iterator;
			}

			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator-=(std::ptrdiff_t offset) noexcept { internal_ptr -= offset; return *this; }
			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator--() noexcept { internal_ptr--; return *this; }
			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator operator--(int) noexcept {
				welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator temp_iterator = *this;
				internal_ptr--; return temp_iterator;
			}

			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator operator+(std::size_t offset) const noexcept {
				return welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator(internal_ptr + offset);
			}
			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator operator-(std::size_t offset) const noexcept {
				return welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator(internal_ptr - offset);
			}
			inline std::ptrdiff_t operator-(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept {
				return internal_ptr - rhs.internal_ptr;
			}

			inline bool operator==(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr == rhs.internal_ptr; }
			inline bool operator!=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr != rhs.internal_ptr; }
			inline bool operator<(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr < rhs.internal_ptr; }
			inline bool operator>(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr > rhs.internal_ptr; }
			inline bool operator<=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr <= rhs.internal_ptr; }
			inline bool operator>=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& rhs) const noexcept { return internal_ptr >= rhs.internal_ptr; }

			iterator() = default;
			iterator(storage_cell* ptr) : internal_ptr(ptr) {}
			iterator(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator&) = default;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator&) = default;
			iterator(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator&&) = default;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator& operator=(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::iterator&&) = default;
			~iterator() = default;

		private:

			storage_cell* internal_ptr;
		};

		class lock
		{

		public:

			lock() = delete;
			lock(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>* _acc_buffer_ptr) : acc_buffer_ptr(_acc_buffer_ptr)
			{
				acc_buffer_ptr->buffer_mutex.lock();
			}
			lock(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock&) = delete;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock&) = delete;
			lock(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock&&) = default;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator=(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock&&) = delete;
			~lock() { acc_buffer_ptr->buffer_mutex.unlock(); }

			inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator<<(const Ty& obj)
			{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
				assert(acc_buffer_ptr->current_ptr < acc_buffer_ptr->data_ptr + acc_buffer_ptr->max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
				acc_buffer_ptr->current_ptr->storage = obj;
				acc_buffer_ptr->current_ptr++;
				return *this;
			}
			inline const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator<<(Ty&& obj) const noexcept
			{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
				assert(acc_buffer_ptr->current_ptr < acc_buffer_ptr->data_ptr + acc_buffer_ptr->max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
				acc_buffer_ptr->current_ptr->storage = std::move(obj);
				acc_buffer_ptr->current_ptr++;
				return *this;
			}
			inline const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator<<(Ty* obj_ptr) const noexcept
			{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
				assert(acc_buffer_ptr->current_ptr < acc_buffer_ptr->data_ptr + acc_buffer_ptr->max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
				acc_buffer_ptr->current_ptr->storage_ptr = obj_ptr;
				acc_buffer_ptr->current_ptr++;
				return *this;
			}
			inline const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::lock& operator<(const Ty& obj) const
			{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
				assert(acc_buffer_ptr->current_ptr < acc_buffer_ptr->data_ptr + acc_buffer_ptr->max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
				acc_buffer_ptr->current_ptr->storage = obj;
				acc_buffer_ptr->current_ptr++;
				return *this;
			}

		private:

			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>* const acc_buffer_ptr;
		};

	private:

		storage_cell* current_ptr = nullptr;
		storage_cell* data_ptr = nullptr;
		std::size_t max_number_of_cells = 0;

		mutable mutex_Ty buffer_mutex;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			storage_cell(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
#endif // WELP_ACC_BUFFER_INCLUDE_MUTEX
}


////// IMPLEMENTATIONS //////

template <class Ty, class _Allocator>
inline welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator<<(const Ty& obj)
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	current_ptr->storage = obj;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator>
inline welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator<<(Ty&& obj) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	current_ptr->storage = std::move(obj);
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator>
inline welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator<<(Ty* const obj_ptr) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	current_ptr->storage_ptr = obj_ptr;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator>
inline welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator<(const Ty& obj)
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	current_ptr->storage = obj;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator>
inline const Ty& welp::acc_buffer<Ty, _Allocator>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(data_ptr + offset < current_ptr);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	storage_cell* temp_cell_ptr = data_ptr + offset;
	if (temp_cell_ptr->storage_ptr != nullptr)
	{
		return *(temp_cell_ptr->storage_ptr);
	}
	else
	{
		return temp_cell_ptr->storage;
	}
}

template <class Ty, class _Allocator>
inline Ty& welp::acc_buffer<Ty, _Allocator>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(data_ptr + offset < current_ptr);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	storage_cell* temp_cell_ptr = data_ptr + offset;
	if (temp_cell_ptr->storage_ptr != nullptr)
	{
		return *(temp_cell_ptr->storage_ptr);
	}
	else
	{
		return temp_cell_ptr->storage;
	}
}

template <class Ty, class _Allocator>
inline std::size_t welp::acc_buffer<Ty, _Allocator>::size() const noexcept
{
	return static_cast<std::size_t>(current_ptr - data_ptr);
}

template <class Ty, class _Allocator>
constexpr std::size_t welp::acc_buffer<Ty, _Allocator>::capacity() const noexcept
{
	return max_number_of_cells;
}

template <class Ty, class _Allocator>
inline void welp::acc_buffer<Ty, _Allocator>::pop_back()
{
	if (current_ptr != data_ptr)
	{
		current_ptr;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator>
inline void welp::acc_buffer<Ty, _Allocator>::pop_back(std::size_t instances)
{
	std::size_t n = static_cast<std::size_t>(current_ptr - data_ptr);
	for (n = (instances < n) ? instances : n; n > 0; n--)
	{
		current_ptr--;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator>
inline void welp::acc_buffer<Ty, _Allocator>::reset()
{
	for (std::size_t n = static_cast<std::size_t>(current_ptr - data_ptr);
		n > 0; n--)
	{
		current_ptr--;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator>
inline bool welp::acc_buffer<Ty, _Allocator>::new_buffer(std::size_t instances)
{
	delete_buffer();

	data_ptr = static_cast<storage_cell*>(static_cast<void*>(this->allocate(instances * sizeof(storage_cell))));
	if (data_ptr != nullptr)
	{
		storage_cell* ptr = data_ptr;
		for (; instances > 0; instances--)
		{
			new (ptr) storage_cell(); ptr++;
		}
		current_ptr = data_ptr;
		max_number_of_cells = instances;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator>
inline void welp::acc_buffer<Ty, _Allocator>::delete_buffer() noexcept
{
	if (data_ptr != nullptr)
	{
		storage_cell* ptr = data_ptr + (max_number_of_cells - 1);
		for (std::size_t n = max_number_of_cells; n > 0; n--)
		{
			ptr->~storage_cell(); ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(data_ptr)), max_number_of_cells * sizeof(storage_cell));
	}
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>::acc_buffer(std::size_t instances)
{
	new_buffer(instances);
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>::acc_buffer(const welp::acc_buffer<Ty, _Allocator>& rhs)
{
	new_buffer(rhs.capacity());

	current_ptr = data_ptr;
	storage_cell* rhs_ptr = rhs.data_ptr;
	for (std::size_t n = rhs.size(); n > 0; n--)
	{
		*current_ptr++ = *rhs_ptr++;
	}
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator=(const welp::acc_buffer<Ty, _Allocator>& rhs)
{
	delete_buffer();
	new_buffer(rhs.capacity());

	current_ptr = data_ptr;
	storage_cell* rhs_ptr = rhs.data_ptr;
	for (std::size_t n = rhs.size(); n > 0; n--)
	{
		*current_ptr++ = *rhs_ptr++;
	}

	return *this;
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>::acc_buffer(welp::acc_buffer<Ty, _Allocator>&& rhs) noexcept
	: current_ptr(rhs.current_ptr), data_ptr(rhs.data_ptr), max_number_of_cells(rhs.max_number_of_cells)
{
	rhs.current_ptr = nullptr;
	rhs.data_ptr = nullptr;
	rhs.max_number_of_cells = 0;
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>& welp::acc_buffer<Ty, _Allocator>::operator=(welp::acc_buffer<Ty, _Allocator>&& rhs) noexcept
{
	delete_buffer();

	current_ptr = rhs.current_ptr;
	data_ptr = rhs.data_ptr;
	max_number_of_cells = rhs.max_number_of_cells;

	rhs.current_ptr = nullptr;
	rhs.data_ptr = nullptr;
	rhs.max_number_of_cells = 0;

	return *this;
}

template <class Ty, class _Allocator>
welp::acc_buffer<Ty, _Allocator>::~acc_buffer()
{
	delete_buffer();
}

#ifdef WELP_ACC_BUFFER_INCLUDE_MUTEX
template <class Ty, class _Allocator, class mutex_Ty>
inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator<<(const Ty& obj)
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	current_ptr->storage = obj;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator<<(Ty&& obj) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	current_ptr->storage = std::move(obj);
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator<<(Ty* const obj_ptr) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	current_ptr->storage_ptr = obj_ptr;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator<(const Ty& obj)
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(current_ptr < data_ptr + max_number_of_cells);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	current_ptr->storage = obj;
	current_ptr++;
	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline const Ty& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator[](std::size_t offset) const noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(data_ptr + offset < current_ptr);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	storage_cell* temp_cell_ptr = data_ptr + offset;
	if (temp_cell_ptr->storage_ptr != nullptr)
	{
		return *(temp_cell_ptr->storage_ptr);
	}
	else
	{
		return temp_cell_ptr->storage;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline Ty& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator[](std::size_t offset) noexcept
{
#ifdef WELP_ACC_BUFFER_DEBUG_MODE
	assert(data_ptr + offset < current_ptr);
#endif // WELP_ACC_BUFFER_DEBUG_MODE
	std::lock_guard<std::mutex> lock(buffer_mutex);
	storage_cell* temp_cell_ptr = data_ptr + offset;
	if (temp_cell_ptr->storage_ptr != nullptr)
	{
		return *(temp_cell_ptr->storage_ptr);
	}
	else
	{
		return temp_cell_ptr->storage;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::size() const noexcept
{
	std::lock_guard<std::mutex> lock(buffer_mutex);
	return static_cast<std::size_t>(current_ptr - data_ptr);
}

template <class Ty, class _Allocator, class mutex_Ty>
constexpr std::size_t welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::capacity() const noexcept
{
	return max_number_of_cells;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline void welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::pop_back()
{
	std::lock_guard<std::mutex> lock(buffer_mutex);
	if (current_ptr != data_ptr)
	{
		current_ptr;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline void welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::pop_back(std::size_t instances)
{
	std::lock_guard<std::mutex> lock(buffer_mutex);
	std::size_t n = static_cast<std::size_t>(current_ptr - data_ptr);
	for (n = (instances < n) ? instances : n; n > 0; n--)
	{
		current_ptr--;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline void welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::reset()
{
	std::lock_guard<std::mutex> lock(buffer_mutex);
	for (std::size_t n = static_cast<std::size_t>(current_ptr - data_ptr); n > 0; n--)
	{
		current_ptr--;
		current_ptr->storage = Ty();
		current_ptr->storage_ptr = nullptr;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::new_buffer(std::size_t instances)
{
	delete_buffer();

	std::lock_guard<std::mutex> lock(buffer_mutex);
	data_ptr = static_cast<storage_cell*>(static_cast<void*>(this->allocate(instances * sizeof(storage_cell))));
	if (data_ptr != nullptr)
	{
		storage_cell* ptr = data_ptr;
		for (; instances > 0; instances--)
		{
			new (ptr) storage_cell(); ptr++;
		}
		current_ptr = data_ptr;
		max_number_of_cells = instances;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline void welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::delete_buffer() noexcept
{
	std::lock_guard<std::mutex> lock(buffer_mutex);
	if (data_ptr != nullptr)
	{
		storage_cell* ptr = data_ptr + (max_number_of_cells - 1);
		for (std::size_t n = max_number_of_cells; n > 0; n--)
		{
			ptr->~storage_cell(); ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(data_ptr)), max_number_of_cells * sizeof(storage_cell));
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::acc_buffer_sync(std::size_t instances)
{
	new_buffer(instances);
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::acc_buffer_sync(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& rhs)
{
	new_buffer(rhs.capacity());

	current_ptr = data_ptr;
	storage_cell* rhs_ptr = rhs.data_ptr;
	for (std::size_t n = rhs.size(); n > 0; n--)
	{
		*current_ptr++ = *rhs_ptr++;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator=(const welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& rhs)
{
	delete_buffer();
	new_buffer(rhs.capacity());

	current_ptr = data_ptr;
	storage_cell* rhs_ptr = rhs.data_ptr;
	for (std::size_t n = rhs.size(); n > 0; n--)
	{
		*current_ptr++ = *rhs_ptr++;
	}

	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::acc_buffer_sync(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&& rhs) noexcept
	: current_ptr(rhs.current_ptr), data_ptr(rhs.data_ptr), max_number_of_cells(rhs.max_number_of_cells)
{
	rhs.current_ptr = nullptr;
	rhs.data_ptr = nullptr;
	rhs.max_number_of_cells = 0;
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>& welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::operator=(welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>&& rhs) noexcept
{
	delete_buffer();

	current_ptr = rhs.current_ptr;
	data_ptr = rhs.data_ptr;
	max_number_of_cells = rhs.max_number_of_cells;

	rhs.current_ptr = nullptr;
	rhs.data_ptr = nullptr;
	rhs.max_number_of_cells = 0;

	return *this;
}

template <class Ty, class _Allocator, class mutex_Ty>
welp::acc_buffer_sync<Ty, _Allocator, mutex_Ty>::~acc_buffer_sync()
{
	delete_buffer();
}
#endif // WELP_ACC_BUFFER_INCLUDE_MUTEX


#endif // WELP_ACC_BUFFER_HPP


// welp_acc_buffer.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
