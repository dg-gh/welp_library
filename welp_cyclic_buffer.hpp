// welp_cyclic_buffer.hpp - last update : 16 / 01 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_CYCLIC_BUFFER_HPP
#define WELP_CYCLIC_BUFFER_HPP


////// INCLUDES //////

#include <cstddef>
#include <memory>


////// DESCRIPTIONS //////

namespace welp
{
	template <class Ty, class _Allocator = std::allocator<char>> class cyclic_buffer : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline welp::cyclic_buffer<Ty, _Allocator>& operator<<(const Ty& obj);
		inline welp::cyclic_buffer<Ty, _Allocator>& operator<<(Ty&& obj) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator>& operator<<(Ty* obj_ptr) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator>& operator<(const Ty& obj);

		inline welp::cyclic_buffer<Ty, _Allocator>& operator>>(Ty& obj) noexcept;
		inline welp::cyclic_buffer<Ty, _Allocator>& operator>(Ty& obj);
		
		const Ty& pop();
		const Ty& get();

		inline bool not_full() const noexcept;
		inline bool not_empty() const noexcept;
		inline bool bad_store() noexcept;
		inline bool bad_load() noexcept;
		inline std::size_t capacity() const noexcept;

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		void set_bad_object(const Ty& bad_obj);
		void set_bad_object(Ty&& bad_obj) noexcept;

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		Ty bad_object = Ty();
		
		bool _bad_store = false;
		bool _bad_load = false;

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

	template <class Ty, class _Allocator = std::allocator<char>> class cyclic_const_buffer : private _Allocator
	{

	private:

		class storage_cell;

	public:

		inline welp::cyclic_const_buffer<Ty, _Allocator>& operator<<(const Ty& obj);
		inline welp::cyclic_const_buffer<Ty, _Allocator>& operator<<(const Ty* obj_ptr) noexcept;
		inline welp::cyclic_const_buffer<Ty, _Allocator>& operator<(const Ty& obj);

		inline welp::cyclic_const_buffer<Ty, _Allocator>& operator>>(Ty& obj) noexcept;
		inline welp::cyclic_const_buffer<Ty, _Allocator>& operator>(Ty& obj);

		const Ty& pop();
		const Ty& get();

		inline bool not_full() const noexcept;
		inline bool not_empty() const noexcept;
		inline bool bad_store() noexcept;
		inline bool bad_load() noexcept;
		inline std::size_t capacity() const noexcept;

		bool new_buffer(std::size_t instances);
		void delete_buffer() noexcept;

		void set_bad_object(const Ty& bad_obj);
		void set_bad_object(Ty&& bad_obj) noexcept;

	private:

		storage_cell* cells_data_ptr = nullptr;
		storage_cell* cells_end_ptr = nullptr;

		storage_cell* last_cell_ptr = nullptr;
		storage_cell* next_cell_ptr = nullptr;

		Ty bad_object = Ty();

		bool _bad_store = false;
		bool _bad_load = false;

		class storage_cell
		{

		public:

			Ty storage = Ty();
			Ty* storage_ptr = nullptr;

			storage_cell() = default;
			storage_cell(const welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell&) = default;
			welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell& operator=(const welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell&) = default;
			storage_cell(welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell&&) = default;
			welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell& operator=(welp::cyclic_const_buffer<Ty, _Allocator>::storage_cell&&) = default;
			~storage_cell() = default;
		};
	};
}


////// IMPLEMENTATIONS //////

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator<<(const Ty& obj)
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator<<(Ty&& obj) noexcept
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage = std::move(obj);
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator<<(Ty* obj_ptr) noexcept
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage_ptr = obj_ptr;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator<(const Ty& obj)
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator>>(Ty& obj) noexcept
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
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;
		return *this;
	}
	else
	{
		_bad_load = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_buffer<Ty, _Allocator>& welp::cyclic_buffer<Ty, _Allocator>::operator>(Ty& obj)
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
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;
		return *this;
	}
	else
	{
		_bad_load = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
const Ty& welp::cyclic_buffer<Ty, _Allocator>::pop()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;

		if (temp_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
			temp_cell_ptr->storage_ptr = nullptr;
			return *temp_storage_ptr;
		}
		else
		{
			return temp_cell_ptr->storage;
		}
	}
	else
	{
		_bad_load = true;
		return bad_object;
	}
}

template <class Ty, class _Allocator>
const Ty& welp::cyclic_buffer<Ty, _Allocator>::get()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			return *next_cell_ptr;
		}
		else
		{
			return next_cell_ptr->storage;
		}
	}
	else
	{
		_bad_load = true;
		return bad_object;
	}
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::not_full() const noexcept
{
	return (next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr);
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::not_empty() const noexcept
{
	return last_cell_ptr != next_cell_ptr;
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::bad_store() noexcept
{
	bool temp = _bad_store;
	_bad_store = false;
	return temp;
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_buffer<Ty, _Allocator>::bad_load() noexcept
{
	bool temp = _bad_load;
	_bad_load = false;
	return temp;
}

template <class Ty, class _Allocator>
inline std::size_t welp::cyclic_buffer<Ty, _Allocator>::capacity() const noexcept
{
	if (cells_data_ptr != nullptr)
	{
		return static_cast<std::size_t>(cells_end_ptr - cells_data_ptr) - 1;
	}
	else
	{
		return 0;
	}
}

template <class Ty, class _Allocator>
bool welp::cyclic_buffer<Ty, _Allocator>::new_buffer(std::size_t instances)
{
	delete_buffer();

	cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
		this->allocate((instances + 1) * sizeof(storage_cell))));
	if (cells_data_ptr != nullptr)
	{
		cells_end_ptr = cells_data_ptr + (instances + 1);
		next_cell_ptr = cells_data_ptr;
		last_cell_ptr = cells_data_ptr;
		storage_cell* ptr = cells_data_ptr;
		for (std::size_t n = instances; n > 0; n--)
		{
			new (ptr) storage_cell(); ptr++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator>
void welp::cyclic_buffer<Ty, _Allocator>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
		std::size_t buffer_size = static_cast<std::size_t>(cells_end_ptr - cells_data_ptr);
		storage_cell* ptr = cells_end_ptr - 1;
		for (std::size_t n = buffer_size; n > 0; n--)
		{
			ptr->~storage_cell(); ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(cells_data_ptr)), buffer_size * sizeof(storage_cell));
		cells_data_ptr = nullptr;
		cells_end_ptr = nullptr;
		next_cell_ptr = nullptr;
		cells_end_ptr = nullptr;
		_bad_store = false;
		_bad_load = false;
	}
}

template <class Ty, class _Allocator>
void welp::cyclic_buffer<Ty, _Allocator>::set_bad_object(const Ty& bad_obj)
{
	bad_object = bad_obj;
}

template <class Ty, class _Allocator>
void welp::cyclic_buffer<Ty, _Allocator>::set_bad_object(Ty&& bad_obj) noexcept
{
	bad_object = std::move(bad_obj);
}

template <class Ty, class _Allocator>
inline welp::cyclic_const_buffer<Ty, _Allocator>& welp::cyclic_const_buffer<Ty, _Allocator>::operator<<(const Ty& obj)
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_const_buffer<Ty, _Allocator>& welp::cyclic_const_buffer<Ty, _Allocator>::operator<<(const Ty* obj_ptr) noexcept
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage_ptr = obj_ptr;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_const_buffer<Ty, _Allocator>& welp::cyclic_const_buffer<Ty, _Allocator>::operator<(const Ty& obj)
{
	if ((next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr))
	{
		last_cell_ptr->storage = obj;
		last_cell_ptr++;
		if (last_cell_ptr == cells_end_ptr) { last_cell_ptr = cells_data_ptr; }
		return *this;
	}
	else
	{
		_bad_store = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_const_buffer<Ty, _Allocator>& welp::cyclic_const_buffer<Ty, _Allocator>::operator>>(Ty& obj) noexcept
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
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;
		return *this;
	}
	else
	{
		_bad_load = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
inline welp::cyclic_const_buffer<Ty, _Allocator>& welp::cyclic_const_buffer<Ty, _Allocator>::operator>(Ty& obj)
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
		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;
		return *this;
	}
	else
	{
		_bad_load = true;
		return *this;
	}
}

template <class Ty, class _Allocator>
const Ty& welp::cyclic_const_buffer<Ty, _Allocator>::pop()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		if (next_cell_ptr == cells_end_ptr)
		{
			next_cell_ptr = cells_data_ptr;
		}
		next_cell_ptr++;

		if (temp_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = temp_cell_ptr->storage_ptr;
			temp_cell_ptr->storage_ptr = nullptr;
			return *temp_storage_ptr;
		}
		else
		{
			return temp_cell_ptr->storage;
		}
	}
	else
	{
		_bad_load = true;
		return bad_object;
	}
}

template <class Ty, class _Allocator>
const Ty& welp::cyclic_const_buffer<Ty, _Allocator>::get()
{
	if (last_cell_ptr != next_cell_ptr)
	{
		storage_cell* temp_cell_ptr = next_cell_ptr;

		if (next_cell_ptr->storage_ptr != nullptr)
		{
			const Ty* temp_storage_ptr = next_cell_ptr->storage_ptr;
			next_cell_ptr->storage_ptr = nullptr;
			return *next_cell_ptr;
		}
		else
		{
			return next_cell_ptr->storage;
		}
	}
	else
	{
		_bad_load = true;
		return bad_object;
	}
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_const_buffer<Ty, _Allocator>::not_full() const noexcept
{
	return (next_cell_ptr != cells_data_ptr) || (last_cell_ptr + 1 != cells_end_ptr);
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_const_buffer<Ty, _Allocator>::not_empty() const noexcept
{
	return last_cell_ptr != next_cell_ptr;
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_const_buffer<Ty, _Allocator>::bad_store() noexcept
{
	bool temp = _bad_store;
	_bad_store = false;
	return temp;
}

template <class Ty, class _Allocator>
inline bool welp::cyclic_const_buffer<Ty, _Allocator>::bad_load() noexcept
{
	bool temp = _bad_load;
	_bad_load = false;
	return temp;
}

template <class Ty, class _Allocator>
inline std::size_t welp::cyclic_const_buffer<Ty, _Allocator>::capacity() const noexcept
{
	if (cells_data_ptr != nullptr)
	{
		return static_cast<std::size_t>(cells_end_ptr - cells_data_ptr) - 1;
	}
	else
	{
		return 0;
	}
}

template <class Ty, class _Allocator>
bool welp::cyclic_const_buffer<Ty, _Allocator>::new_buffer(std::size_t instances)
{
	delete_buffer();

	cells_data_ptr = static_cast<storage_cell*>(static_cast<void*>(
		this->allocate((instances + 1) * sizeof(storage_cell))));
	if (cells_data_ptr != nullptr)
	{
		cells_end_ptr = cells_data_ptr + (instances + 1);
		next_cell_ptr = cells_data_ptr;
		last_cell_ptr = cells_data_ptr;
		storage_cell* ptr = cells_data_ptr;
		for (std::size_t n = instances; n > 0; n--)
		{
			new (ptr) storage_cell(); ptr++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

template <class Ty, class _Allocator>
void welp::cyclic_const_buffer<Ty, _Allocator>::delete_buffer() noexcept
{
	if (cells_data_ptr != nullptr)
	{
		std::size_t buffer_size = static_cast<std::size_t>(cells_end_ptr - cells_data_ptr);
		storage_cell* ptr = cells_end_ptr - 1;
		for (std::size_t n = buffer_size; n > 0; n--)
		{
			ptr->~storage_cell(); ptr--;
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(cells_data_ptr)), buffer_size * sizeof(storage_cell));
		cells_data_ptr = nullptr;
		cells_end_ptr = nullptr;
		next_cell_ptr = nullptr;
		cells_end_ptr = nullptr;
		_bad_store = false;
		_bad_load = false;
	}
}

template <class Ty, class _Allocator>
void welp::cyclic_const_buffer<Ty, _Allocator>::set_bad_object(const Ty& bad_obj)
{
	bad_object = bad_obj;
}

template <class Ty, class _Allocator>
void welp::cyclic_const_buffer<Ty, _Allocator>::set_bad_object(Ty&& bad_obj) noexcept
{
	bad_object = std::move(bad_obj);
}


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
