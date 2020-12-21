// welp_cyclic_resource.hpp - last update : 21 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_CYCLIC_RESOURCE_H
#define WELP_CYCLIC_RESOURCE_H


////// INCLUDES //////

#include <cstdlib>

#ifdef WELP_CYCLIC_DEBUG_MODE
#include <iostream>
#ifdef WELP_CYCLIC_INCLUDE_FSTREAM
#include <fstream>
#endif // WELP_CYCLIC_INCLUDE_FSTREAM
#endif // WELP_CYCLIC_DEBUG_MODE

#ifndef WELP_CYCLIC_RECORD_INT
#define WELP_CYCLIC_RECORD_INT unsigned int
#endif // WELP_CYCLIC_RECORD_INT


////// DESCRIPTIONS //////

namespace welp
{
	// allocate and deallocate should preferably not throw
	// allocate should preferably return a nullptr in case of allocation failure
	// can be substituted by std::allocator<char> although a non-throwing version would be preferable
	class _default_cyclic_sub_allocator
	{

	public:

		inline char* allocate(std::size_t bytes) const noexcept { return static_cast<char*>(std::malloc(bytes)); }
		inline void deallocate(char* ptr, std::size_t) const noexcept { std::free(static_cast<void*>(ptr)); }

		_default_cyclic_sub_allocator() = default;
		~_default_cyclic_sub_allocator() = default;
	};

	// memory resource
	template <std::size_t mem_align, class sub_allocator = welp::_default_cyclic_sub_allocator> class cyclic_resource
	{

	private:

		char* current_ptr = nullptr;
		char* end_ptr = nullptr;
		char* data_ptr = nullptr;
		char* data_ptr_unaligned = nullptr;

#ifdef WELP_CYCLIC_DEBUG_MODE
		WELP_CYCLIC_RECORD_INT alloc_count = 0;
		WELP_CYCLIC_RECORD_INT dealloc_count = 0;
		std::size_t alloc_byte_count = 0;
		WELP_CYCLIC_RECORD_INT cycle_count = 0;
		bool record_on = false;
#endif // WELP_CYCLIC_DEBUG_MODE

	public:

		template <class Ty> inline Ty* allocate_type(std::size_t instances) noexcept;
		template <class Ty> inline Ty* allocate_type_padded(std::size_t instances, std::size_t alloc_align) noexcept;
		inline void* allocate_byte(std::size_t bytes) noexcept;
		inline void* allocate_byte_padded(std::size_t bytes, std::size_t alloc_align) noexcept;

		template <class Ty> inline void deallocate_ptr(Ty*) noexcept;

		inline void reset_pool() noexcept;
		inline std::size_t capacity() const noexcept;
		inline bool new_pool(std::size_t bytes);
		inline void delete_pool();

		inline bool owns_resources() const noexcept { return  data_ptr_unaligned != nullptr; }

#ifdef WELP_CYCLIC_DEBUG_MODE
		void record_start() noexcept;
		void record_stop() noexcept;
		void record_reset() noexcept;

		void record_say() const;
		template <typename msg_Ty> void record_say
		(const msg_Ty& msg) const;
		template <typename msg_Ty1, typename msg_Ty2> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2) const;
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3) const;
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4) const;

#ifdef WELP_CYCLIC_INCLUDE_FSTREAM
		void record_write(const char* const filename) const;
		template <typename msg_Ty> void record_write
		(const char* const filename, const msg_Ty& msg) const;
		template <typename msg_Ty1, typename msg_Ty2> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2) const;
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3) const;
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4) const;
#endif // WELP_CYCLIC_INCLUDE_FSTREAM
#endif // WELP_CYCLIC_DEBUG_MODE

		cyclic_resource() = default;
		virtual ~cyclic_resource() { delete_pool(); }

	private:

#ifdef WELP_CYCLIC_DEBUG_MODE
		void record_say_sub() const;
#ifdef WELP_CYCLIC_INCLUDE_FSTREAM
		void record_write_sub(std::ostream& rec_write) const;
#endif // WELP_CYCLIC_INCLUDE_FSTREAM
#endif // WELP_CYCLIC_DEBUG_MODE

		cyclic_resource(const welp::cyclic_resource<mem_align, sub_allocator>&) = delete;
		cyclic_resource<mem_align, sub_allocator>& operator=(const welp::cyclic_resource<mem_align, sub_allocator>&) = delete;
		cyclic_resource(welp::cyclic_resource<mem_align, sub_allocator>&&) = delete;
		welp::cyclic_resource<mem_align, sub_allocator>& operator=(welp::cyclic_resource<mem_align, sub_allocator>&&) = delete;
	};
}



////// IMPLEMENTATIONS //////

// ALLOCATE
template <std::size_t mem_align, class sub_allocator>
template <class Ty> inline Ty* welp::cyclic_resource<mem_align, sub_allocator>::allocate_type(std::size_t instances) noexcept
{
	constexpr std::size_t mem_align_m1 = mem_align - 1;
	instances *= sizeof(Ty);
	instances += ((mem_align - (instances & mem_align_m1)) & mem_align_m1);
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on)
	{
		alloc_count++;
		alloc_byte_count += instances;
	}
#endif // WELP_CYCLIC_DEBUG_MODE
	current_ptr += ((mem_align - (reinterpret_cast<std::size_t>(current_ptr) & mem_align_m1)) & mem_align_m1);
	if (current_ptr + instances < end_ptr)
	{
		char* temp_ptr = current_ptr;
		current_ptr += instances;
		return static_cast<Ty*>(static_cast<void*>(temp_ptr));
	}
	else
	{
#ifdef WELP_CYCLIC_DEBUG_MODE
		if (record_on) { cycle_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
		current_ptr = data_ptr + instances;
		return static_cast<Ty*>(static_cast<void*>(data_ptr));
	}
}


// ALLOCATE ALIGNED
template <std::size_t mem_align, class sub_allocator>
template <class Ty> inline Ty* welp::cyclic_resource<mem_align, sub_allocator>::allocate_type_padded(std::size_t instances, std::size_t alloc_align) noexcept
{
	std::size_t alloc_align_m1 = alloc_align - 1;
	instances *= sizeof(Ty);
	instances += ((alloc_align - (instances & alloc_align_m1)) & alloc_align_m1);
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on)
	{
		alloc_count++;
		alloc_byte_count += instances;
	}
#endif // WELP_CYCLIC_DEBUG_MODE
	current_ptr += ((alloc_align - (reinterpret_cast<std::size_t>(current_ptr) & alloc_align_m1)) & alloc_align_m1);
	if (current_ptr + instances < end_ptr)
	{
		char* temp_ptr = current_ptr;
		current_ptr += instances;
		return static_cast<Ty*>(static_cast<void*>(temp_ptr));
	}
	else
	{
#ifdef WELP_CYCLIC_DEBUG_MODE
		if (record_on) { cycle_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
		current_ptr = data_ptr + instances;
		return static_cast<Ty*>(static_cast<void*>(data_ptr));
	}
}


// ALLOCATE
template <std::size_t mem_align, class sub_allocator>
inline void* welp::cyclic_resource<mem_align, sub_allocator>::allocate_byte(std::size_t bytes) noexcept
{
	constexpr std::size_t mem_align_m1 = mem_align - 1;
	bytes += ((mem_align - (bytes & mem_align_m1)) & mem_align_m1);
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on)
	{
		alloc_count++;
		alloc_byte_count += bytes;
	}
#endif // WELP_CYCLIC_DEBUG_MODE
	current_ptr += ((mem_align - (reinterpret_cast<std::size_t>(current_ptr) & mem_align_m1)) & mem_align_m1);
	if (current_ptr + bytes < end_ptr)
	{
		char* temp_ptr = current_ptr;
		current_ptr += bytes;
		return static_cast<void*>(temp_ptr);
	}
	else
	{
#ifdef WELP_CYCLIC_DEBUG_MODE
		if (record_on) { cycle_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
		current_ptr = data_ptr + bytes;
		return static_cast<void*>(data_ptr);
	}
}


// ALLOCATE ALIGNED
template <std::size_t mem_align, class sub_allocator>
inline void* welp::cyclic_resource<mem_align, sub_allocator>::allocate_byte_padded(std::size_t bytes, std::size_t alloc_align) noexcept
{
	std::size_t alloc_align_m1 = alloc_align - 1;
	bytes += ((alloc_align - (bytes & alloc_align_m1)) & alloc_align_m1);
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on)
	{
		alloc_count++;
		alloc_byte_count += bytes;
	}
#endif // WELP_CYCLIC_DEBUG_MODE
	current_ptr += ((alloc_align - (reinterpret_cast<std::size_t>(current_ptr) & alloc_align_m1)) & alloc_align_m1);
	if (current_ptr + bytes < end_ptr)
	{
		char* temp_ptr = current_ptr;
		current_ptr += bytes;
		return static_cast<void*>(temp_ptr);
	}
	else
	{
#ifdef WELP_CYCLIC_DEBUG_MODE
		if (record_on) { cycle_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
		current_ptr = data_ptr + bytes;
		return static_cast<void*>(data_ptr);
	}
}


// DEALLOCATE
template <std::size_t mem_align, class sub_allocator>
template <class Ty> inline void welp::cyclic_resource<mem_align, sub_allocator>::deallocate_ptr(Ty*) noexcept
{
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on) { dealloc_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
}


// RESET
template <std::size_t mem_align, class sub_allocator>
inline void welp::cyclic_resource<mem_align, sub_allocator>::reset_pool() noexcept
{
#ifdef WELP_CYCLIC_DEBUG_MODE
	if (record_on) { cycle_count++; }
#endif // WELP_CYCLIC_DEBUG_MODE
	current_ptr = data_ptr;
}


// CAPACITY
template <std::size_t mem_align, class sub_allocator>
inline std::size_t welp::cyclic_resource<mem_align, sub_allocator>::capacity() const noexcept
{
	return static_cast<std::size_t>(end_ptr - data_ptr);
}


// NEW POOL
template <std::size_t mem_align, class sub_allocator>
inline bool welp::cyclic_resource<mem_align, sub_allocator>::new_pool(std::size_t bytes)
{
	delete_pool();
	if (bytes == 0)
	{
		return false;
	}

	constexpr std::size_t mem_align_m1 = mem_align - 1;
	sub_allocator _sub_allocator;
	try
	{
		data_ptr_unaligned = _sub_allocator.allocate(bytes + mem_align_m1);
	}
	catch (...)
	{
		delete_pool(); return false;
	}
	if (data_ptr_unaligned == nullptr) { delete_pool(); return false; }
	data_ptr = data_ptr_unaligned + ((mem_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned) & mem_align_m1)) & mem_align_m1);
	current_ptr = data_ptr;
	end_ptr = data_ptr + bytes;
#ifdef WELP_CYCLIC_DEBUG_MODE
	record_reset();
	record_on = false;
#endif // WELP_CYCLIC_DEBUG_MODE
	return true;
}


// DELETE POOL
template <std::size_t mem_align, class sub_allocator>
inline void welp::cyclic_resource<mem_align, sub_allocator>::delete_pool()
{
	if (data_ptr_unaligned != nullptr)
	{
		sub_allocator _sub_allocator;
		_sub_allocator.deallocate(data_ptr_unaligned, static_cast<std::size_t>(end_ptr - data_ptr_unaligned));
		data_ptr_unaligned = nullptr;
		data_ptr = nullptr;
		end_ptr = nullptr;
	}
}


#ifdef WELP_CYCLIC_DEBUG_MODE
template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_start() noexcept { record_on = true; }
template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_stop() noexcept { record_on = false; }
template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_reset() noexcept
{
	alloc_count = 0;
	dealloc_count = 0;
	alloc_byte_count = 0;
	cycle_count = 0;
}


template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_say_sub() const
{
	std::cout << "\nCyclic pool ";
	std::cout.fill(' '); std::cout.width(2);
	std::cout << std::left << " ";

	std::cout << "> pool capacity : " << static_cast<std::size_t>(end_ptr - data_ptr)
		<< "  > allocations : " << alloc_count
		<< "  > deallocations : " << dealloc_count << std::endl;

	std::cout.fill(' '); std::cout.width(14); std::cout << std::left << " ";
	std::cout << "> bytes allocated : " << alloc_byte_count
		<< "  > cycles : " << cycle_count << std::endl;
}

template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_say() const
{
	record_say_sub();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty> void welp::cyclic_resource<mem_align, sub_allocator>::record_say(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2> void welp::cyclic_resource<mem_align, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void welp::cyclic_resource<mem_align, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void welp::cyclic_resource<mem_align, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4) const
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_CYCLIC_INCLUDE_FSTREAM
template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_write_sub(std::ostream& rec_write) const
{
	rec_write << "\nCyclic pool  > pool capacity : "
		<< static_cast<std::size_t>(end_ptr - data_ptr)
		<< "  > allocations : " << alloc_count
		<< "  > deallocations : " << dealloc_count << "\n"
		<< "             > bytes allocated : " << alloc_byte_count
		<< "  > cycles : " << cycle_count << "\n" << std::endl;
}

template <std::size_t mem_align, class sub_allocator>
void welp::cyclic_resource<mem_align, sub_allocator>::record_write(const char* const filename) const
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty> void welp::cyclic_resource<mem_align, sub_allocator>::record_write(const char* const filename, const msg_Ty& msg) const
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2> void welp::cyclic_resource<mem_align, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2) const
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void welp::cyclic_resource<mem_align, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3) const
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <std::size_t mem_align, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void welp::cyclic_resource<mem_align, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4) const
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}
#endif // WELP_CYCLIC_INCLUDE_FSTREAM
#endif // WELP_CYCLIC_DEBUG_MODE


#endif // WELP_CYCLIC_RESOURCE_H


// welp_cyclic_resource.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
