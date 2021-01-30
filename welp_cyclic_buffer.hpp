// welp_cyclic_buffer.hpp - last update : 27 / 01 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_CYCLIC_BUFFER_HPP
#define WELP_CYCLIC_BUFFER_HPP


////// INCLUDES //////

#include <cstddef>
#include <memory>


#if defined(WELP_CYCLIC_BUFFER_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#define WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#endif
#endif // WELP_CYCLIC_BUFFER_INCLUDE_ALL


#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#ifndef WELP_CYCLIC_BUFFER_DEFAULT_MUTEX
#define WELP_CYCLIC_BUFFER_DEFAULT_MUTEX std::mutex
#endif
#else // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#ifndef WELP_CYCLIC_BUFFER_DEFAULT_MUTEX
#define WELP_CYCLIC_BUFFER_DEFAULT_MUTEX int
#endif
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX


////// DESCRIPTIONS //////

namespace welp
{
	template <class Ty, class _Allocator = std::allocator<char>, class mutex_Ty = WELP_CYCLIC_BUFFER_DEFAULT_MUTEX> class cyclic_buffer : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline bool store_cpy(const Ty& obj);
		inline bool store_move(Ty&& obj) noexcept;
		inline bool store_ptr(Ty* obj_ptr) noexcept;

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		inline bool store_cpy_sync(const Ty& obj);
		inline bool store_move_sync(Ty&& obj) noexcept;
		inline bool store_ptr_sync(Ty* obj_ptr) noexcept;
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator<<(const Ty& obj);
		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator<<(Ty&& obj) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator<<(Ty* obj_ptr) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator<(const Ty& obj);

		inline bool load_cpy(Ty& obj);
		inline bool load(Ty& obj) noexcept;

		const Ty& pop();
		const Ty& get();

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		inline bool load_cpy_sync(Ty& obj);
		inline bool load_sync(Ty& obj) noexcept;

		const Ty& pop_sync();
		const Ty& get_sync();
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator>>(Ty& obj) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator>(Ty& obj);


		inline bool not_full() const noexcept;
		inline bool not_empty() const noexcept;
		inline bool stream_store_completed() noexcept;
		inline bool stream_load_completed() noexcept;

		inline std::size_t size() const noexcept;
		inline std::size_t capacity() const noexcept;
		inline std::size_t capacity_remaining() const noexcept;

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		void set_bad_object(const Ty& bad_obj);
		void set_bad_object(Ty&& bad_obj) noexcept;

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		class lock;
		welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock acquire_exclusivity()
		{
			return welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock(&buffer_mutex);
		}
		void notify_one_for_capacity();
		void notify_one_for_size();

		class lock_with_size;
		welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size spin_for_size_and_acquire_exclusivity(std::size_t requested_size)
		{
			if (requested_size > _capacity) { requested_size = _capacity; }
			while (reinterpret_cast<std::atomic<std::size_t>&>(_size).load() < requested_size) {}
			return welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size(this);
		}

		class lock_with_capacity;
		welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity spin_for_capacity_and_acquire_exclusivity(std::size_t requested_capacity)
		{
			if (requested_capacity > _capacity) { requested_capacity = _capacity; }
			while (_capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load() < requested_capacity) {}
			return welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity(this);
		}
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

		cyclic_buffer() = default;
		cyclic_buffer(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>&) = delete;
		welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator=(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>&) = delete;
		cyclic_buffer(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>&&) = delete;
		welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& operator=(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>&&) = delete;
		~cyclic_buffer() { delete_buffer(); }

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		class lock
		{

		public:

			lock() = delete;
			lock(mutex_Ty* _mutex_ptr) : mutex_ptr(_mutex_ptr) { mutex_ptr->lock(); }
			lock(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock& operator=(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock&) = delete;
			lock(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock& operator=(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock&&) = delete;
			~lock() { mutex_ptr->unlock(); }

		private:

			mutex_Ty* mutex_ptr;
		};
		class lock_with_size
		{

		public:

			inline bool good(std::size_t test_size) const noexcept { return cyclic_buffer_ptr->_size >= test_size; }

			lock_with_size() = delete;
			lock_with_size(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>* _cyclic_buffer_ptr)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
			}
			lock_with_size(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size& operator=(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size&) = delete;
			lock_with_size(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size& operator=(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_size&&) = delete;
			~lock_with_size() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

		private:

			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>* cyclic_buffer_ptr;

		};
		class lock_with_capacity
		{

		public:

			inline bool good(std::size_t test_capacity) const noexcept { return cyclic_buffer_ptr->_capacity - cyclic_buffer_ptr->_size >= test_capacity; }

			lock_with_capacity() = delete;
			lock_with_capacity(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>* _cyclic_buffer_ptr)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
			}
			lock_with_capacity(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity& operator=(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity&) = delete;
			lock_with_capacity(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity& operator=(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::lock_with_capacity&&) = delete;
			~lock_with_capacity() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

		private:

			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>* cyclic_buffer_ptr;
		};
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		std::size_t _size = 0;
		std::size_t _capacity = 0;

		Ty bad_object = Ty();

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		mutex_Ty buffer_mutex;

		std::condition_variable size_condition_variable;
		std::condition_variable capacity_condition_variable;

		bool terminate_buffer = true;
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

		bool _stream_store_completed = true;
		bool _stream_load_completed = true;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(const welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			storage_cell(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
}


////// IMPLEMENTATIONS //////

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_cpy(const Ty& obj)
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_move(Ty&& obj) noexcept
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = std::move(obj);
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_ptr(Ty* obj_ptr) noexcept
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage_ptr = obj_ptr;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_cpy_sync(const Ty& obj)
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_move_sync(Ty&& obj) noexcept
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = std::move(obj);
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::store_ptr_sync(Ty* obj_ptr) noexcept
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage_ptr = obj_ptr;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return true;
	}
	else
	{
		return false;
	}
}
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator<<(const Ty& obj)
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return *this;
	}
	else
	{
		_stream_store_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator<<(Ty&& obj) noexcept
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = std::move(obj);
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return *this;
	}
	else
	{
		_stream_store_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator<<(Ty* obj_ptr) noexcept
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage_ptr = obj_ptr;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return *this;
	}
	else
	{
		_stream_store_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator<(const Ty& obj)
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		_size++;
		return *this;
	}
	else
	{
		_stream_store_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::load_cpy(Ty& obj)
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = *temp_storage_ptr;
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::load(Ty& obj) noexcept
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = std::move(*temp_storage_ptr);
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
const Ty& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::pop()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}

		if (temp_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
			temp_cell_ptr->storage_ptr = nullptr;
			_size--;
			return *temp_storage_ptr;
		}
		else
		{
			_size--;
			return temp_cell_ptr->storage;
		}
	}
	else
	{
		_stream_load_completed = false;
		return bad_object;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
const Ty& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::get()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			return *temp_storage_ptr;
		}
		else
		{
			return next_cell_ptr->storage;
		}
	}
	else
	{
		_stream_load_completed = false;
		return bad_object;
	}
}

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::load_cpy_sync(Ty& obj)
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = *temp_storage_ptr;
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::load_sync(Ty& obj) noexcept
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = std::move(*temp_storage_ptr);
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
const Ty& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::pop_sync()
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}

		if (temp_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
			temp_cell_ptr->storage_ptr = nullptr;
			_size--;
			return *temp_storage_ptr;
		}
		else
		{
			_size--;
			return temp_cell_ptr->storage;
		}
	}
	else
	{
		_stream_load_completed = false;
		return bad_object;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
const Ty& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::get_sync()
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			return *temp_storage_ptr;
		}
		else
		{
			return next_cell_ptr->storage;
		}
	}
	else
	{
		_stream_load_completed = false;
		return bad_object;
	}
}
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator>>(Ty& obj) noexcept
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = std::move(*temp_storage_ptr);
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return *this;
	}
	else
	{
		_stream_load_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>& welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::operator>(Ty& obj)
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = *temp_storage_ptr;
		}
		else
		{
			obj = std::move(next_cell_ptr->storage);
		}
		next_cell_ptr++;
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		_size--;
		return *this;
	}
	else
	{
		_stream_load_completed = false;
		return *this;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::not_full() const noexcept
{
	return (next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr);
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::not_empty() const noexcept
{
	return last_cell_ptr != next_cell_ptr;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::stream_store_completed() noexcept
{
	bool temp = _stream_store_completed;
	_stream_store_completed = true;
	return temp;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::stream_load_completed() noexcept
{
	bool temp = _stream_load_completed;
	_stream_load_completed = true;
	return temp;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::size() const noexcept
{
#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	return reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
#else // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	return _size;
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::capacity() const noexcept
{
	if (cells_data_ptr != nullptr)
	{
		return _capacity;
	}
	else
	{
		return 0;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::capacity_remaining() const noexcept
{
#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	return _capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
#else // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	return _capacity - _size;
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
}

template <class Ty, class _Allocator, class mutex_Ty>
bool welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::new_buffer(std::size_t instances)
{
	delete_buffer();

	cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
		this->allocate((instances + 1) * sizeof(storage_cell))));
	if (cells_data_ptr != nullptr)
	{
		_capacity = instances;
		cells_end_ptr = cells_data_ptr;
		next_cell_ptr = cells_data_ptr;
		last_cell_ptr = cells_data_ptr;
#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		terminate_buffer = false;
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		for (std::size_t n = instances + 1; n > 0; n--)
		{
			new (cells_end_ptr) storage_cell(); cells_end_ptr++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
void welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		{
			std::lock_guard<mutex_Ty> _lock(buffer_mutex);
			terminate_buffer = true;
		}
		capacity_condition_variable.notify_all();
		size_condition_variable.notify_all();
		std::lock_guard<mutex_Ty> _lock(buffer_mutex);
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
		std::size_t buffer_size = static_cast<std::size_t>(cells_end_ptr - cells_data_ptr);
		cells_end_ptr--;
		for (std::size_t n = buffer_size; n > 0; n--)
		{
			cells_end_ptr->~storage_cell(); cells_end_ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(cells_data_ptr)), buffer_size * sizeof(storage_cell));
		cells_data_ptr = nullptr;
		cells_end_ptr = nullptr;
		next_cell_ptr = nullptr;
		cells_end_ptr = nullptr;
		_capacity = 0;
		_stream_store_completed = true;
		_stream_load_completed = true;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
void welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::set_bad_object(const Ty& bad_obj)
{
	bad_object = bad_obj;
}

template <class Ty, class _Allocator, class mutex_Ty>
void welp::cyclic_buffer<Ty, _Allocator, mutex_Ty>::set_bad_object(Ty&& bad_obj) noexcept
{
	bad_object = std::move(bad_obj);
}

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
namespace welp
{
	template <class Ty, class _Allocator> class cyclic_buffer<Ty, _Allocator, std::mutex> : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline bool store_cpy(const Ty& obj)
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = obj;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool store_move(Ty&& obj) noexcept
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = std::move(obj);
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool store_ptr(Ty* obj_ptr) noexcept
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage_ptr = obj_ptr;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}

		inline bool store_cpy_sync(const Ty& obj)
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = obj;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool store_move_sync(Ty&& obj) noexcept
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = std::move(obj);
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool store_ptr_sync(Ty* obj_ptr) noexcept
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage_ptr = obj_ptr;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return true;
			}
			else
			{
				return false;
			}
		}

		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator<<(const Ty& obj)
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = obj;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return *this;
			}
			else
			{
				_stream_store_completed = false;
				return *this;
			}
		}
		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator<<(Ty&& obj) noexcept
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = std::move(obj);
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return *this;
			}
			else
			{
				_stream_store_completed = false;
				return *this;
			}
		}
		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator<<(Ty* obj_ptr) noexcept
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage_ptr = obj_ptr;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return *this;
			}
			else
			{
				_stream_store_completed = false;
				return *this;
			}
		}
		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator<(const Ty& obj)
		{
			if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
			{
				last_cell_ptr->storage = obj;
				last_cell_ptr++;
				if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
				_size++;
				return *this;
			}
			else
			{
				_stream_store_completed = false;
				return *this;
			}
		}

		inline bool load_cpy(Ty& obj)
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = *temp_storage_ptr;
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool load(Ty& obj) noexcept
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = std::move(*temp_storage_ptr);
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return true;
			}
			else
			{
				return false;
			}
		}

		const Ty& pop()
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				storage_cell* temp_cell_ptr = next_cell_ptr;

				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}

				if (temp_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
					temp_cell_ptr->storage_ptr = nullptr;
					_size--;
					return *temp_storage_ptr;
				}
				else
				{
					_size--;
					return temp_cell_ptr->storage;
				}
			}
			else
			{
				_stream_load_completed = false;
				return bad_object;
			}
		}
		const Ty& get()
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					return *temp_storage_ptr;
				}
				else
				{
					return next_cell_ptr->storage;
				}
			}
			else
			{
				_stream_load_completed = false;
				return bad_object;
			}
		}

		inline bool load_cpy_sync(Ty& obj)
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = *temp_storage_ptr;
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return true;
			}
			else
			{
				return false;
			}
		}
		inline bool load_sync(Ty& obj) noexcept
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = std::move(*temp_storage_ptr);
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return true;
			}
			else
			{
				return false;
			}
		}

		const Ty& pop_sync()
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if (last_cell_ptr != next_cell_ptr)
			{
				storage_cell* temp_cell_ptr = next_cell_ptr;

				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}

				if (temp_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
					temp_cell_ptr->storage_ptr = nullptr;
					_size--;
					return *temp_storage_ptr;
				}
				else
				{
					_size--;
					return temp_cell_ptr->storage;
				}
			}
			else
			{
				_stream_load_completed = false;
				return bad_object;
			}
		}
		const Ty& get_sync()
		{
			std::lock_guard<std::mutex> _lock(buffer_mutex);

			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					return *temp_storage_ptr;
				}
				else
				{
					return next_cell_ptr->storage;
				}
			}
			else
			{
				_stream_load_completed = false;
				return bad_object;
			}
		}

		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator>>(Ty& obj) noexcept
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = std::move(*temp_storage_ptr);
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return *this;
			}
			else
			{
				_stream_load_completed = false;
				return *this;
			}
		}
		inline welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator>(Ty& obj)
		{
			if (last_cell_ptr != next_cell_ptr)
			{
				if (next_cell_ptr->storage_ptr != nullptr)
				{
					const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
					next_cell_ptr->storage_ptr = nullptr;
					obj = *temp_storage_ptr;
				}
				else
				{
					obj = std::move(next_cell_ptr->storage);
				}
				next_cell_ptr++;
				if (next_cell_ptr == cells_end_ptr)
				{
					next_cell_ptr = cells_data_ptr;
				}
				_size--;
				return *this;
			}
			else
			{
				_stream_load_completed = false;
				return *this;
			}
		}

		inline bool not_full() const noexcept
		{
			return (next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr);
		}
		inline bool not_empty() const noexcept
		{
			return last_cell_ptr != next_cell_ptr;
		}
		inline bool stream_store_completed() noexcept
		{
			bool temp = _stream_store_completed;
			_stream_store_completed = true;
			return temp;
		}
		inline bool stream_load_completed() noexcept
		{
			bool temp = _stream_load_completed;
			_stream_load_completed = true;
			return temp;
		}

		inline std::size_t size() const noexcept
		{
			return reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
		}
		inline std::size_t capacity() const noexcept
		{
			if (cells_data_ptr != nullptr)
			{
				return _capacity;
			}
			else
			{
				return 0;
			}
		}
		inline std::size_t capacity_remaining() const noexcept
		{
			return _capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
		}

		bool new_buffer(std::size_t instances)
		{
			delete_buffer();

			cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
				this->allocate((instances + 1) * sizeof(storage_cell))));
			if (cells_data_ptr != nullptr)
			{
				_capacity = instances;
				cells_end_ptr = cells_data_ptr;
				next_cell_ptr = cells_data_ptr;
				last_cell_ptr = cells_data_ptr;
				terminate_buffer = false;
				for (std::size_t n = instances + 1; n > 0; n--)
				{
					new (cells_end_ptr) storage_cell(); cells_end_ptr++;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		void delete_buffer() noexcept
		{
			if (cells_data_ptr != nullptr)
			{
				{
					std::lock_guard<std::mutex> _lock(buffer_mutex);
					terminate_buffer = true;
				}
				capacity_condition_variable.notify_all();
				size_condition_variable.notify_all();
				std::lock_guard<std::mutex> _lock(buffer_mutex);
				std::size_t buffer_size = static_cast<std::size_t>(cells_end_ptr - cells_data_ptr);
				cells_end_ptr--;
				for (std::size_t n = buffer_size; n > 0; n--)
				{
					cells_end_ptr->~storage_cell(); cells_end_ptr--;
				}
				this->deallocate(static_cast<char*>(static_cast<void*>(cells_data_ptr)), buffer_size * sizeof(storage_cell));
				cells_data_ptr = nullptr;
				cells_end_ptr = nullptr;
				next_cell_ptr = nullptr;
				cells_end_ptr = nullptr;
				_capacity = 0;
				_stream_store_completed = true;
				_stream_load_completed = true;
			}
		}

		void set_bad_object(const Ty& bad_obj)
		{
			bad_object = bad_obj;
		}
		void set_bad_object(Ty&& bad_obj) noexcept
		{
			bad_object = std::move(bad_obj);
		}

		class lock;
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock acquire_exclusivity()
		{
			return welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock(&buffer_mutex);
		}
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>& wait_for_capacity(std::size_t requested_capacity)
		{
			if (requested_capacity > _capacity) { requested_capacity = _capacity; }
			std::unique_lock<std::mutex> waiting_lock(buffer_mutex);
			capacity_condition_variable.wait(waiting_lock, [=]() {
				return (_capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load() >= requested_capacity) || terminate_buffer; });
			return *this;
		}
		void notify_one_for_capacity()
		{
			capacity_condition_variable.notify_one();
		}
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>& wait_for_size(std::size_t requested_size)
		{
			if (requested_size > _capacity) { requested_size = _capacity; }
			std::unique_lock<std::mutex> waiting_lock(buffer_mutex);
			size_condition_variable.wait(waiting_lock, [=]() {
				return (reinterpret_cast<std::atomic<std::size_t>&>(_size).load() >= requested_size) || terminate_buffer; });
			return *this;
		}
		void notify_one_for_size()
		{
			size_condition_variable.notify_one();
		}

		class lock_with_size;
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size spin_for_size_and_acquire_exclusivity(std::size_t requested_size)
		{
			if (requested_size > _capacity) { requested_size = _capacity; }
			while (reinterpret_cast<std::atomic<std::size_t>&>(_size).load() < requested_size) {}
			return welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size(this);
		}
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size wait_for_size_and_acquire_exclusivity(std::size_t requested_size)
		{
			if (requested_size > _capacity) { requested_size = _capacity; }
			{
				std::unique_lock<std::mutex> waiting_lock(buffer_mutex);
				size_condition_variable.wait(waiting_lock, [=]() {
					return (reinterpret_cast<std::atomic<std::size_t>&>(_size).load() >= requested_size) || terminate_buffer; });
			}
			return welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size(this);
		}

		class lock_with_capacity;
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity spin_for_capacity_and_acquire_exclusivity(std::size_t requested_capacity)
		{
			if (requested_capacity > _capacity) { requested_capacity = _capacity; }
			while (_capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load() < requested_capacity) {}
			return welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity(this);
		}
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity wait_for_capacity_and_acquire_exclusivity(std::size_t requested_capacity)
		{
			if (requested_capacity > _capacity) { requested_capacity = _capacity; }
			{
				std::unique_lock<std::mutex> waiting_lock(buffer_mutex);
				capacity_condition_variable.wait(waiting_lock, [=]() {
					return (_capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load() >= requested_capacity) || terminate_buffer; });
			}
			return welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity(this);
		}

		cyclic_buffer() = default;
		cyclic_buffer(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>&) = delete;
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator=(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>&) = delete;
		cyclic_buffer(welp::cyclic_buffer<Ty, _Allocator, std::mutex>&&) = delete;
		welp::cyclic_buffer<Ty, _Allocator, std::mutex>& operator=(welp::cyclic_buffer<Ty, _Allocator, std::mutex>&&) = delete;
		~cyclic_buffer() { delete_buffer(); }

		class lock
		{

		public:

			lock() = delete;
			lock(std::mutex* _mutex_ptr) : mutex_ptr(_mutex_ptr) { mutex_ptr->lock(); }
			lock(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock& operator=(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock&) = delete;
			lock(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock& operator=(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock&&) = delete;
			~lock() { mutex_ptr->unlock(); }

		private:

			std::mutex* mutex_ptr;
		};
		class lock_with_size
		{

		public:

			inline bool good(std::size_t test_size) const noexcept { return cyclic_buffer_ptr->_size >= test_size; }

			lock_with_size() = delete;
			lock_with_size(welp::cyclic_buffer<Ty, _Allocator, std::mutex>* _cyclic_buffer_ptr)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
			}
			lock_with_size(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size& operator=(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size&) = delete;
			lock_with_size(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size& operator=(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_size&&) = delete;
			~lock_with_size() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

		private:

			welp::cyclic_buffer<Ty, _Allocator, std::mutex>* cyclic_buffer_ptr;

		};
		class lock_with_capacity
		{

		public:

			inline bool good(std::size_t test_capacity) const noexcept { return cyclic_buffer_ptr->_capacity - cyclic_buffer_ptr->_size >= test_capacity; }

			lock_with_capacity() = delete;
			lock_with_capacity(welp::cyclic_buffer<Ty, _Allocator, std::mutex>* _cyclic_buffer_ptr)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
			}
			lock_with_capacity(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity&) = delete;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity& operator=(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity&) = delete;
			lock_with_capacity(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity& operator=(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::lock_with_capacity&&) = delete;
			~lock_with_capacity() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

		private:

			welp::cyclic_buffer<Ty, _Allocator, std::mutex>* cyclic_buffer_ptr;
		};

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		std::size_t _size = 0;
		std::size_t _capacity = 0;

		Ty bad_object = Ty();

		std::mutex buffer_mutex;

		std::condition_variable size_condition_variable;
		std::condition_variable capacity_condition_variable;

		bool terminate_buffer = true;
		bool _stream_store_completed = true;
		bool _stream_load_completed = true;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell&) = default;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell& operator=(const welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell&) = default;
			storage_cell(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell&&) = default;
			welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell& operator=(welp::cyclic_buffer<Ty, _Allocator, std::mutex>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
}
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX


#endif // WELP_CYCLIC_BUFFER_HPP


// welp_cyclic_buffer.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
