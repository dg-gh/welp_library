// welp_cyclic_buffer.hpp - last update : 03 / 02 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_CYCLIC_BUFFER_HPP
#define WELP_CYCLIC_BUFFER_HPP


////// INCLUDES //////

#include <cstdlib>


#if defined(WELP_CYCLIC_BUFFER_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#define WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#endif
#ifndef WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC
#define WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC
#endif
#endif // WELP_CYCLIC_BUFFER_INCLUDE_ALL


#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#include <mutex>
#include <atomic>
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC
#ifndef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#include <atomic>
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
#endif // WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC


////// DESCRIPTIONS //////

namespace welp
{
	// allocate and deallocate should preferably not throw
	// allocate should preferably return a nullptr in case of allocation failure
	// can be substituted by std::allocator<char> although a non-throwing version would be preferable
	class default_cyclic_buffer_allocator
	{

	public:

		inline char* allocate(std::size_t bytes) const noexcept { return static_cast<char*>(std::malloc(bytes)); }
		inline void deallocate(char* ptr, std::size_t) const noexcept { std::free(static_cast<void*>(ptr)); }
	};

	template <class Ty, class _Allocator = default_cyclic_buffer_allocator> class cyclic_buffer : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline bool store_cpy(const Ty& obj);
		inline bool store_move(Ty&& obj) noexcept;
		inline bool store_ptr(Ty* obj) noexcept;
		template <class ... _Args> inline bool store_new(_Args&& ... args);

		inline bool load_cpy(Ty& obj);
		inline bool load(Ty& obj) noexcept;

		inline std::size_t size() const noexcept;
		inline std::size_t capacity() const noexcept;
		inline std::size_t capacity_remaining() const noexcept;

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		cyclic_buffer() = default;
		cyclic_buffer(const welp::cyclic_buffer<Ty, _Allocator>&) = delete;
		welp::cyclic_buffer<Ty, _Allocator>& operator=(const welp::cyclic_buffer<Ty, _Allocator>&) = delete;
		cyclic_buffer(welp::cyclic_buffer<Ty, _Allocator>&&) = delete;
		welp::cyclic_buffer<Ty, _Allocator>& operator=(welp::cyclic_buffer<Ty, _Allocator>&&) = delete;
		~cyclic_buffer() { delete_buffer(); }

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		std::size_t _size = 0;
		std::size_t _capacity = 0;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_buffer<Ty, _Allocator>::storage_cell&) = default;
			welp::cyclic_buffer<Ty, _Allocator>::storage_cell& operator=(const welp::cyclic_buffer<Ty, _Allocator>::storage_cell&) = default;
			storage_cell(welp::cyclic_buffer<Ty, _Allocator>::storage_cell&&) = default;
			welp::cyclic_buffer<Ty, _Allocator>::storage_cell& operator=(welp::cyclic_buffer<Ty, _Allocator>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC
	template <class Ty, class _Allocator = default_cyclic_buffer_allocator,
		std::size_t padding_size = 8> class cyclic_buffer_atom : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline bool store_cpy(const Ty& obj);
		inline bool store_move(Ty&& obj) noexcept;
		inline bool store_ptr(Ty* obj) noexcept;
		template <class ... _Args> inline bool store_new(_Args&& ... args);

		inline bool load_cpy(Ty& obj);
		inline bool load(Ty& obj) noexcept;

		inline std::size_t size() const noexcept;
		inline std::size_t capacity() const noexcept;
		inline std::size_t capacity_remaining() const noexcept;

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		cyclic_buffer_atom() = default;
		cyclic_buffer_atom(const welp::cyclic_buffer_atom<Ty, _Allocator>&) = delete;
		welp::cyclic_buffer_atom<Ty, _Allocator>& operator=(const welp::cyclic_buffer_atom<Ty, _Allocator>&) = delete;
		cyclic_buffer_atom(welp::cyclic_buffer_atom<Ty, _Allocator>&&) = delete;
		welp::cyclic_buffer_atom<Ty, _Allocator>& operator=(welp::cyclic_buffer_atom<Ty, _Allocator>&&) = delete;
		~cyclic_buffer_atom() { delete_buffer(); }

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		std::size_t padding0[padding_size] = { 0 };
		std::atomic<storage_cell*> last_cell_ptr{ nullptr };
		std::size_t padding1[padding_size] = { 0 };
		std::atomic<storage_cell*> next_cell_ptr{ nullptr };
		std::size_t padding2[padding_size] = { 0 };

		std::size_t _capacity = 0;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell&) = default;
			welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell& operator=(
				const welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell&) = default;
			storage_cell(welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell&&) = default;
			welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell& operator=(
				welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
#endif // WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
	template <class Ty, class _Allocator = default_cyclic_buffer_allocator, class mutex_Ty = std::mutex> class cyclic_buffer_sync : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline bool store_cpy(const Ty& obj);
		inline bool store_move(Ty&& obj) noexcept;
		inline bool store_ptr(Ty* obj_ptr) noexcept;
		template <class ... _Args> inline bool store_new(_Args&& ... args);

		inline bool load_cpy(Ty& obj);
		inline bool load(Ty& obj) noexcept;

		inline std::size_t size() const noexcept;
		inline std::size_t capacity() const noexcept;
		inline std::size_t capacity_remaining() const noexcept;

		class slock;
		inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock store_block(std::size_t requested_capacity)
		{
			return welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock(this, requested_capacity);
		}

		class llock;
		inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock load_block(std::size_t requested_capacity)
		{
			return welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock(this, requested_capacity);
		}

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		cyclic_buffer_sync() = default;
		cyclic_buffer_sync(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>&) = delete;
		welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>& operator=(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>&) = delete;
		cyclic_buffer_sync(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>&&) = delete;
		welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>& operator=(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>&&) = delete;
		~cyclic_buffer_sync() { delete_buffer(); }

		class slock
		{

		public:

			slock() = delete;
			slock(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>* _cyclic_buffer_ptr, std::size_t requested_capacity)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
				_good = (requested_capacity <= _cyclic_buffer_ptr->_capacity - reinterpret_cast<std::atomic<std::size_t>&>(_cyclic_buffer_ptr->_size).load());
			}
			slock(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock&) = delete;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator=(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock&) = delete;
			slock(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock&&) = default;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator=(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock&&) = delete;
			~slock() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

			inline bool good() const noexcept { return _good; }

			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator<<(const Ty& obj) const
			{
				if (_good)
				{
					cyclic_buffer_ptr->last_cell_ptr->storage = obj;
					cyclic_buffer_ptr->last_cell_ptr++;
					if (cyclic_buffer_ptr->last_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->last_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size++;
				}
				return *this;
			}
			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator<<(Ty&& obj) const noexcept
			{
				if (_good)
				{
					cyclic_buffer_ptr->last_cell_ptr->storage = std::move(obj);
					cyclic_buffer_ptr->last_cell_ptr++;
					if (cyclic_buffer_ptr->last_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->last_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size++;
				}
				return *this;
			}
			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator<<(Ty* obj_ptr) const noexcept
			{
				if (_good)
				{
					cyclic_buffer_ptr->last_cell_ptr->storage_ptr = obj_ptr;
					cyclic_buffer_ptr->last_cell_ptr++;
					if (cyclic_buffer_ptr->last_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->last_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size++;
				}
				return *this;
			}
			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::slock& operator<(const Ty& obj) const
			{
				if (_good)
				{
					cyclic_buffer_ptr->last_cell_ptr->storage = obj;
					cyclic_buffer_ptr->last_cell_ptr++;
					if (cyclic_buffer_ptr->last_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->last_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size++;
				}
				return *this;
			}

		private:

			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>* const cyclic_buffer_ptr;
			bool _good = false;
		};
		class llock
		{

		public:

			llock() = delete;
			llock(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>* _cyclic_buffer_ptr, std::size_t requested_capacity)
				: cyclic_buffer_ptr(_cyclic_buffer_ptr)
			{
				cyclic_buffer_ptr->buffer_mutex.lock();
				_good = (requested_capacity <= reinterpret_cast<std::atomic<std::size_t>&>(_cyclic_buffer_ptr->_size).load());
			}
			llock(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock&) = delete;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock& operator=(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock&) = delete;
			llock(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock&&) = default;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock& operator=(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock&&) = delete;
			~llock() { cyclic_buffer_ptr->buffer_mutex.unlock(); }

			inline bool good() const noexcept { return _good; }

			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock& operator>>(Ty& obj) const noexcept
			{
				if (_good)
				{
					if (cyclic_buffer_ptr->next_cell_ptr->storage_ptr != nullptr)
					{
						const Ty* temp_storage_ptr = cyclic_buffer_ptr->next_cell_ptr->storage_ptr;
						cyclic_buffer_ptr->next_cell_ptr->storage_ptr = nullptr;
						obj = std::move(*temp_storage_ptr);
					}
					else
					{
						obj = std::move(cyclic_buffer_ptr->next_cell_ptr->storage);
					}
					cyclic_buffer_ptr->next_cell_ptr++;
					if (cyclic_buffer_ptr->next_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->next_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size--;
				}
				return *this;
			}
			inline const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::llock& operator>(Ty& obj) const
			{
				if (_good)
				{
					if (cyclic_buffer_ptr->next_cell_ptr->storage_ptr != nullptr)
					{
						const Ty* temp_storage_ptr = cyclic_buffer_ptr->next_cell_ptr->storage_ptr;
						cyclic_buffer_ptr->next_cell_ptr->storage_ptr = nullptr;
						obj = *temp_storage_ptr;
					}
					else
					{
						obj = std::move(cyclic_buffer_ptr->next_cell_ptr->storage);
					}
					cyclic_buffer_ptr->next_cell_ptr++;
					if (cyclic_buffer_ptr->next_cell_ptr == cyclic_buffer_ptr->cells_end_ptr)
					{
						cyclic_buffer_ptr->next_cell_ptr = cyclic_buffer_ptr->cells_data_ptr;
					}
					cyclic_buffer_ptr->_size--;
				}
				return *this;
			}

		private:

			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>* const cyclic_buffer_ptr;
			bool _good = false;
		};

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		std::size_t _size = 0;
		std::size_t _capacity = 0;

		mutex_Ty buffer_mutex;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(const welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&) = default;
			storage_cell(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell& operator=(welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
#endif // WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
}


////// IMPLEMENTATIONS //////

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::store_cpy(const Ty& obj)
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

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::store_move(Ty&& obj) noexcept
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

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::store_ptr(Ty* obj_ptr) noexcept
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

template <class Ty, class _Allocator> template <class ... _Args>
inline bool welp::cyclic_buffer<Ty, _Allocator>::store_new(_Args&& ... args)
{
	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage.~Ty();
		new (&(last_cell_ptr->storage)) Ty(std::forward<_Args>(args)...);
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

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::load_cpy(Ty& obj)
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr == nullptr)
		{
			obj = std::move(next_cell_ptr->storage);
		}
		else
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = *temp_storage_ptr;
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

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::load(Ty& obj) noexcept
{
	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr == nullptr)
		{
			obj = std::move(next_cell_ptr->storage);
		}
		else
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = std::move(*temp_storage_ptr);
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

template <class Ty, class _Allocator>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator>::size() const noexcept
{
	return _size;
}

template <class Ty, class _Allocator>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator>::capacity() const noexcept
{
	return _capacity;
}

template <class Ty, class _Allocator>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator>::capacity_remaining() const noexcept
{
	return _capacity - _size;
}

template <class Ty, class _Allocator>
bool welp::cyclic_buffer<Ty, _Allocator>::new_buffer(std::size_t instances)
{
	delete_buffer();

	try
	{
		cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
			this->allocate((instances + 1) * sizeof(storage_cell))));
		if (cells_data_ptr != nullptr)
		{
			_capacity = instances;
			cells_end_ptr = cells_data_ptr;
			next_cell_ptr = cells_data_ptr;
			last_cell_ptr = cells_data_ptr;
			for (std::size_t n = instances + 1; n > 0; n--)
			{
				new (cells_end_ptr) storage_cell(); cells_end_ptr++;
			}
			return true;
		}
		else
		{
			delete_buffer(); return false;
		}
	}
	catch (...)
	{
		delete_buffer(); return false;
	}
}

template <class Ty, class _Allocator>
void welp::cyclic_buffer<Ty, _Allocator>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
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
		last_cell_ptr = nullptr;
		_size = 0;
		_capacity = 0;
	}
}

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC
template <class Ty, class _Allocator, std::size_t padding_size>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::store_cpy(const Ty& obj)
{
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_last_cell_ptr;
	storage_cell* initial_last_cell_ptr = last_cell_ptr.load();
	do
	{
		final_last_cell_ptr = initial_last_cell_ptr + 1;
		if ((final_last_cell_ptr != temp_next_cell_ptr) && ((temp_next_cell_ptr != cells_data_ptr)
			|| (final_last_cell_ptr != cells_end_ptr)))
		{
			if (final_last_cell_ptr == cells_end_ptr) { final_last_cell_ptr = cells_data_ptr; }
		}
		else
		{
			return false;
		}
	} while (!last_cell_ptr.compare_exchange_strong(initial_last_cell_ptr, final_last_cell_ptr));
	initial_last_cell_ptr->storage = obj;
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::store_move(Ty&& obj) noexcept
{
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_last_cell_ptr;
	storage_cell* initial_last_cell_ptr = last_cell_ptr.load();
	do
	{
		final_last_cell_ptr = initial_last_cell_ptr + 1;
		if ((final_last_cell_ptr != temp_next_cell_ptr) && ((temp_next_cell_ptr != cells_data_ptr)
			|| (final_last_cell_ptr != cells_end_ptr)))
		{
			if (final_last_cell_ptr == cells_end_ptr) { final_last_cell_ptr = cells_data_ptr; }
		}
		else
		{
			return false;
		}
	} while (!last_cell_ptr.compare_exchange_strong(initial_last_cell_ptr, final_last_cell_ptr));
	initial_last_cell_ptr->storage = std::move(obj);
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::store_ptr(Ty* obj_ptr) noexcept
{
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_last_cell_ptr;
	storage_cell* initial_last_cell_ptr = last_cell_ptr.load();
	do
	{
		final_last_cell_ptr = initial_last_cell_ptr + 1;
		if ((final_last_cell_ptr != temp_next_cell_ptr) && ((temp_next_cell_ptr != cells_data_ptr)
			|| (final_last_cell_ptr != cells_end_ptr)))
		{
			if (final_last_cell_ptr == cells_end_ptr) { final_last_cell_ptr = cells_data_ptr; }
		}
		else
		{
			return false;
		}
	} while (!last_cell_ptr.compare_exchange_strong(initial_last_cell_ptr, final_last_cell_ptr));
	initial_last_cell_ptr->storage_ptr = obj_ptr;
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size> template <class ... _Args>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::store_new(_Args&& ... args)
{
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_last_cell_ptr;
	storage_cell* initial_last_cell_ptr = last_cell_ptr.load();
	do
	{
		final_last_cell_ptr = initial_last_cell_ptr + 1;
		if ((final_last_cell_ptr != temp_next_cell_ptr) && ((temp_next_cell_ptr != cells_data_ptr)
			|| (final_last_cell_ptr != cells_end_ptr)))
		{
			if (final_last_cell_ptr == cells_end_ptr) { final_last_cell_ptr = cells_data_ptr; }
		}
		else
		{
			return false;
		}
	} while (!last_cell_ptr.compare_exchange_strong(initial_last_cell_ptr, final_last_cell_ptr));
	initial_last_cell_ptr->storage.~Ty();
	new (&(initial_last_cell_ptr->storage)) Ty(std::forward<_Args>(args)...);
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::load_cpy(Ty& obj)
{
	storage_cell* temp_last_cell_ptr = last_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_next_cell_ptr;
	storage_cell* initial_next_cell_ptr = next_cell_ptr.load();
	do
	{
		if (temp_last_cell_ptr != initial_next_cell_ptr)
		{
			final_next_cell_ptr = initial_next_cell_ptr + 1;
			if (final_next_cell_ptr == cells_end_ptr)
			{
				final_next_cell_ptr = cells_data_ptr;
			}
		}
		else
		{
			return false;
		}
	} while (!next_cell_ptr.compare_exchange_strong(initial_next_cell_ptr, final_next_cell_ptr));

	if (initial_next_cell_ptr->storage_ptr == nullptr)
	{
		obj = std::move(initial_next_cell_ptr->storage);
	}
	else
	{
		const Ty* temp_storage_ptr = initial_next_cell_ptr->storage_ptr;
		initial_next_cell_ptr->storage_ptr = nullptr;
		obj = *temp_storage_ptr;
	}
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::load(Ty& obj) noexcept
{
	storage_cell* temp_last_cell_ptr = last_cell_ptr.load(std::memory_order_relaxed);
	storage_cell* final_next_cell_ptr;
	storage_cell* initial_next_cell_ptr = next_cell_ptr.load();
	do
	{
		if (temp_last_cell_ptr != initial_next_cell_ptr)
		{
			final_next_cell_ptr = initial_next_cell_ptr + 1;
			if (final_next_cell_ptr == cells_end_ptr)
			{
				final_next_cell_ptr = cells_data_ptr;
			}
		}
		else
		{
			return false;
		}
	} while (!next_cell_ptr.compare_exchange_strong(initial_next_cell_ptr, final_next_cell_ptr));

	if (initial_next_cell_ptr->storage_ptr == nullptr)
	{
		obj = std::move(initial_next_cell_ptr->storage);
	}
	else
	{
		const Ty* temp_storage_ptr = initial_next_cell_ptr->storage_ptr;
		initial_next_cell_ptr->storage_ptr = nullptr;
		obj = std::move(*temp_storage_ptr);
	}
	return true;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline std::size_t welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::size() const noexcept
{
	storage_cell* temp_last_cell_ptr = last_cell_ptr.load();
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load();
	if (temp_last_cell_ptr >= temp_next_cell_ptr)
	{
		return static_cast<std::size_t>(temp_last_cell_ptr - temp_next_cell_ptr);
	}
	else
	{
		return (_capacity + 1) - static_cast<std::size_t>(temp_next_cell_ptr - temp_last_cell_ptr);
	}
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline std::size_t welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::capacity() const noexcept
{
	return _capacity;
}

template <class Ty, class _Allocator, std::size_t padding_size>
inline std::size_t welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::capacity_remaining() const noexcept
{
	storage_cell* temp_last_cell_ptr = last_cell_ptr.load();
	storage_cell* temp_next_cell_ptr = next_cell_ptr.load();
	if (temp_last_cell_ptr >= temp_next_cell_ptr)
	{
		return _capacity - static_cast<std::size_t>(temp_last_cell_ptr - temp_next_cell_ptr);
	}
	else
	{
		return static_cast<std::size_t>(temp_next_cell_ptr - temp_last_cell_ptr) - 1;
	}
}

template <class Ty, class _Allocator, std::size_t padding_size>
bool welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::new_buffer(std::size_t instances)
{
	delete_buffer();

	try
	{
		cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
			this->allocate((instances + 1) * sizeof(storage_cell))));
		if (cells_data_ptr != nullptr)
		{
			_capacity = instances;
			cells_end_ptr = cells_data_ptr;
			next_cell_ptr.store(cells_data_ptr, std::memory_order_relaxed);
			last_cell_ptr.store(cells_data_ptr, std::memory_order_relaxed);
			for (std::size_t n = instances + 1; n > 0; n--)
			{
				new (cells_end_ptr) storage_cell(); cells_end_ptr++;
			}
			return true;
		}
		else
		{
			delete_buffer(); return false;
		}
	}
	catch (...)
	{
		delete_buffer(); return false;
	}
}

template <class Ty, class _Allocator, std::size_t padding_size>
void welp::cyclic_buffer_atom<Ty, _Allocator, padding_size>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
		std::size_t buffer_size = static_cast<std::size_t>(cells_end_ptr - cells_data_ptr);
		cells_end_ptr--;
		for (std::size_t n = buffer_size; n > 0; n--)
		{
			cells_end_ptr->~storage_cell(); cells_end_ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(cells_data_ptr)), buffer_size * sizeof(storage_cell));
		cells_data_ptr = nullptr;
		cells_end_ptr = nullptr;
		next_cell_ptr.store(nullptr, std::memory_order_relaxed);
		last_cell_ptr.store(nullptr, std::memory_order_relaxed);
		_capacity = 0;
	}
}
#endif // WELP_CYCLIC_BUFFER_INCLUDE_ATOMIC

#ifdef WELP_CYCLIC_BUFFER_INCLUDE_MUTEX
template <class Ty, class _Allocator, class mutex_Ty>
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::store_cpy(const Ty& obj)
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
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::store_move(Ty&& obj) noexcept
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
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::store_ptr(Ty* obj_ptr) noexcept
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

template <class Ty, class _Allocator, class mutex_Ty> template <class ... _Args>
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::store_new(_Args&& ... args)
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if ((last_cell_ptr + 1 != next_cell_ptr) && ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr)))
	{
		last_cell_ptr->storage.~Ty();
		new (&(last_cell_ptr->storage)) Ty(std::forward<_Args>(args)...);
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
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::load_cpy(Ty& obj)
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr == nullptr)
		{
			obj = std::move(next_cell_ptr->storage);
		}
		else
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = *temp_storage_ptr;
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
inline bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::load(Ty& obj) noexcept
{
	std::lock_guard<mutex_Ty> _lock(buffer_mutex);

	if (last_cell_ptr != next_cell_ptr)
	{
		if (next_cell_ptr->storage_ptr == nullptr)
		{
			obj = std::move(next_cell_ptr->storage);
		}
		else
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			obj = std::move(*temp_storage_ptr);
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
inline std::size_t welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::size() const noexcept
{
	return reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::capacity() const noexcept
{
	return _capacity;
}

template <class Ty, class _Allocator, class mutex_Ty>
inline std::size_t welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::capacity_remaining() const noexcept
{
	return _capacity - reinterpret_cast<std::atomic<std::size_t>&>(_size).load();
}

template <class Ty, class _Allocator, class mutex_Ty>
bool welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::new_buffer(std::size_t instances)
{
	delete_buffer();

	try
	{
		cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
			this->allocate((instances + 1) * sizeof(storage_cell))));
		if (cells_data_ptr != nullptr)
		{
			_capacity = instances;
			cells_end_ptr = cells_data_ptr;
			next_cell_ptr = cells_data_ptr;
			last_cell_ptr = cells_data_ptr;
			for (std::size_t n = instances + 1; n > 0; n--)
			{
				new (cells_end_ptr) storage_cell(); cells_end_ptr++;
			}
			return true;
		}
		else
		{
			delete_buffer(); return false;
		}
	}
	catch (...)
	{
		delete_buffer(); return false;
	}
}

template <class Ty, class _Allocator, class mutex_Ty>
void welp::cyclic_buffer_sync<Ty, _Allocator, mutex_Ty>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
		std::lock_guard<mutex_Ty> _lock(buffer_mutex);
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
		_size = 0;
		_capacity = 0;
	}
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
