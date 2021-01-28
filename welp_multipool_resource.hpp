// welp_multipool_resource.hpp - last update : 24 / 01 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_MULTIPOOL_RESOURCE_HPP
#define WELP_MULTIPOOL_RESOURCE_HPP


////// INCLUDES //////

#include <cstdlib>
#include <cstring>


// include all in one line with #define WELP_MULTIPOOL_INCLUDE_ALL
#if defined(WELP_ALWAYS_INCLUDE_ALL) || defined(WELP_MULTIPOOL_RESOURCE_INCLUDE_ALL) || defined(WELP_MULTIPOOL_INCLUDE_ALL)

#ifndef WELP_MULTIPOOL_INCLUDE_INITLIST
#define WELP_MULTIPOOL_INCLUDE_INITLIST
#endif
#ifndef WELP_MULTIPOOL_INCLUDE_ALGORITHM
#define WELP_MULTIPOOL_INCLUDE_ALGORITHM
#endif
#ifndef WELP_MULTIPOOL_INCLUDE_MUTEX
#define WELP_MULTIPOOL_INCLUDE_MUTEX
#endif
#ifndef WELP_MULTIPOOL_INCLUDE_FSTREAM
#define WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif

#endif // WELP_ALWAYS_INCLUDE_ALL


#if defined(WELP_ALWAYS_DEBUG_MODE) || defined(WELP_MULTIPOOL_RESOURCE_DEBUG_MODE)
#ifndef WELP_MULTIPOOL_DEBUG_MODE
#define WELP_MULTIPOOL_DEBUG_MODE
#endif // WELP_MULTIPOOL_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_MULTIPOOL_DEBUG_MODE
#include <iostream>
#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
#include <fstream>
#endif //  WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
#include <initializer_list>
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST

#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
#include <algorithm>
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM

#ifdef WELP_MULTIPOOL_INCLUDE_MUTEX
#include <mutex>
#endif // WELP_MULTIPOOL_INCLUDE_MUTEX


////// OPTIONS //////

#ifndef WELP_MULTIPOOL_RECORD_INT
#define WELP_MULTIPOOL_RECORD_INT unsigned int
#endif // WELP_MULTIPOOL_RECORD_INT

// #define WELP_MULTIPOOL_NO_TEMPLATE will only enable the use of welp::quadpool


////// DESCRIPTIONS //////

// memory resources
namespace welp
{
#ifndef WELP_MULTIPOOL_NO_TEMPLATE
	// allocate and deallocate should preferably not throw
	// allocate should preferably return a nullptr in case of allocation failure
	// can be substituted by std::allocator<char> although a non-throwing version would be preferable
	class default_multipool_sub_allocator
	{

	public:

		inline char* allocate(std::size_t bytes) const noexcept { return static_cast<char*>(std::malloc(bytes)); }
		inline void deallocate(char* ptr, std::size_t) const noexcept { std::free(static_cast<void*>(ptr)); }
	};

	// memory resource single thread
	template <std::size_t max_number_of_pools, class sub_allocator = welp::default_multipool_sub_allocator> class multipool_resource
	{

	private:

		char** current_address_ptr[max_number_of_pools] = { nullptr };
		char** first_address_ptr[max_number_of_pools] = { nullptr };
		char* data_ptr[max_number_of_pools] = { nullptr };
		char* data_ptr_unaligned[max_number_of_pools] = { nullptr };

		std::size_t block_size[max_number_of_pools] = { 0 };
		std::size_t block_instances[max_number_of_pools] = { 0 };
		std::size_t number_of_pools = 0;
		std::size_t pool_align_size = 0;

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		char** top_address_ptr[max_number_of_pools] = { nullptr };
		WELP_MULTIPOOL_RECORD_INT record_allocations[max_number_of_pools] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_deallocations[max_number_of_pools] = { 0 };
		std::size_t record_max_occupancy[max_number_of_pools] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_denied_block_requests[max_number_of_pools] = { 0 };
		std::size_t record_biggest_request = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_allocations = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_deallocations = 0;
		bool record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE

	public:

		template <class Ty> inline Ty* allocate_type(std::size_t instances) noexcept;
		template <class Ty> inline Ty* allocate_type_padded(std::size_t instances, std::size_t line_size) noexcept;
		template <class Ty> inline Ty* allocate_type_in_pool(std::size_t instances, std::size_t pool_number) noexcept;
		template <class Ty> inline Ty* allocate_type_in_pool_range(std::size_t instances,
			std::size_t first_pool, std::size_t end_pool) noexcept;
		template <class Ty> inline Ty* allocate_type_padded_in_pool_range(std::size_t instances, std::size_t line_size,
			std::size_t first_pool, std::size_t end_pool) noexcept;

		inline void* allocate_byte(std::size_t bytes) noexcept;
		inline void* allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept;
		inline void* allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept;
		inline void* allocate_byte_in_pool_range(std::size_t bytes,
			std::size_t first_pool, std::size_t end_pool) noexcept;
		inline void* allocate_byte_padded_in_pool_range(std::size_t bytes, std::size_t line_size,
			std::size_t first_pool, std::size_t end_pool) noexcept;

		template <class Ty> inline bool deallocate_ptr(Ty* ptr) noexcept;
		template <class Ty> inline bool deallocate_ptr_in_pool(Ty* ptr, std::size_t pool_number) noexcept;
		template <class Ty> inline bool deallocate_ptr_in_pool_range(Ty* ptr, std::size_t first_pool, std::size_t end_pool) noexcept;

		template <class Ty> inline std::size_t blocks_remaining_type() noexcept;
		template <class Ty> inline std::size_t blocks_remaining_type(std::size_t instances) noexcept;
		inline std::size_t blocks_remaining_byte(std::size_t bytes) noexcept;
		inline std::size_t blocks_remaining_in_pool(std::size_t pool_number) noexcept;
		inline std::size_t block_size_in_pool(std::size_t pool_number) const noexcept { return block_size[pool_number]; }

		inline bool owns_resources() const noexcept { return number_of_pools != 0; }
		inline std::size_t number_of_pools_allocated() const noexcept { return number_of_pools; }
		constexpr inline std::size_t maximum_number_of_pools() const noexcept { return max_number_of_pools; }

#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
		inline void sort_pools() noexcept;
		inline void sort_pool(std::size_t pool_number) noexcept;
		inline void sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM

		inline void reset_pools() noexcept;
		inline void reset_pool(std::size_t pool_number) noexcept;
		inline void reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;

		bool new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
			const std::size_t* const input_block_instances, std::size_t pool_align);

#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
		bool new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
			std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align);
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST

		void delete_pools();

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_start() noexcept { record_on = true; };
		void record_stop() noexcept { record_on = false; };
		void record_reset() noexcept;

		void record_say();
		template <typename msg_Ty> void record_say
		(const msg_Ty& msg);
		template <typename msg_Ty1, typename msg_Ty2> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);

#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write(const char* const filename);
		template <typename msg_Ty> void record_write
		(const char* const filename, const msg_Ty& msg);
		template <typename msg_Ty1, typename msg_Ty2> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		multipool_resource() = default;
		virtual ~multipool_resource() { delete_pools(); }

	private:

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_say_sub();
#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write_sub(std::ofstream& rec_write);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		multipool_resource(const welp::multipool_resource<max_number_of_pools, sub_allocator>&) = delete;
		welp::multipool_resource<max_number_of_pools, sub_allocator>& operator=
			(const welp::multipool_resource<max_number_of_pools, sub_allocator>&) = delete;
		multipool_resource(welp::multipool_resource<max_number_of_pools, sub_allocator>&&) = delete;
		welp::multipool_resource<max_number_of_pools, sub_allocator>& operator=
			(welp::multipool_resource<max_number_of_pools, sub_allocator>&&) = delete;
	};


	// memory resource thread safe
#ifdef WELP_MULTIPOOL_INCLUDE_MUTEX
	template <std::size_t max_number_of_pools, class sub_allocator = welp::default_multipool_sub_allocator,
		class mutex_Ty = std::mutex> class multipool_resource_sync
	{

	private:

		char** current_address_ptr[max_number_of_pools] = { nullptr };
		char** first_address_ptr[max_number_of_pools] = { nullptr };
		char* data_ptr[max_number_of_pools] = { nullptr };
		char* data_ptr_unaligned[max_number_of_pools] = { nullptr };

		std::size_t block_size[max_number_of_pools] = { 0 };
		std::size_t block_instances[max_number_of_pools] = { 0 };
		std::size_t number_of_pools = 0;
		std::size_t pool_align_size = 0;

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		char** top_address_ptr[max_number_of_pools] = { nullptr };
		WELP_MULTIPOOL_RECORD_INT record_allocations[max_number_of_pools] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_deallocations[max_number_of_pools] = { 0 };
		std::size_t record_max_occupancy[max_number_of_pools] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_denied_block_requests[max_number_of_pools] = { 0 };
		std::size_t record_biggest_request = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_allocations = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_deallocations = 0;
		bool record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE

		mutex_Ty resource_mutex;

	public:

		template <class Ty> inline Ty* allocate_type(std::size_t instances) noexcept;
		template <class Ty> inline Ty* allocate_type_padded(std::size_t instances, std::size_t line_size) noexcept;
		template <class Ty> inline Ty* allocate_type_in_pool(std::size_t instances, std::size_t pool_number) noexcept;
		template <class Ty> inline Ty* allocate_type_in_pool_range(std::size_t instances,
			std::size_t first_pool, std::size_t end_pool) noexcept;
		template <class Ty> inline Ty* allocate_type_padded_in_pool_range(std::size_t instances, std::size_t line_size,
			std::size_t first_pool, std::size_t end_pool) noexcept;

		inline void* allocate_byte(std::size_t bytes) noexcept;
		inline void* allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept;
		inline void* allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept;
		inline void* allocate_byte_in_pool_range(std::size_t bytes,
			std::size_t first_pool, std::size_t end_pool) noexcept;
		inline void* allocate_byte_padded_in_pool_range(std::size_t bytes, std::size_t line_size,
			std::size_t first_pool, std::size_t end_pool) noexcept;

		template <class Ty> inline bool deallocate_ptr(Ty* ptr) noexcept;
		template <class Ty> inline bool deallocate_ptr_in_pool(Ty* ptr, std::size_t pool_number) noexcept;
		template <class Ty> inline bool deallocate_ptr_in_pool_range(Ty* ptr, std::size_t first_pool, std::size_t end_pool) noexcept;

		template <class Ty> inline std::size_t blocks_remaining_type() noexcept;
		template <class Ty> inline std::size_t blocks_remaining_type(std::size_t instances) noexcept;
		inline std::size_t blocks_remaining_byte(std::size_t bytes) noexcept;
		inline std::size_t blocks_remaining_in_pool(std::size_t pool_number) noexcept;
		inline std::size_t block_size_in_pool(std::size_t pool_number) const noexcept { return block_size[pool_number]; }

		inline bool owns_resources() const noexcept { return number_of_pools != 0; }
		inline std::size_t number_of_pools_allocated() const noexcept { return number_of_pools; }
		constexpr inline std::size_t maximum_number_of_pools() const noexcept { return max_number_of_pools; }

#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
		inline void sort_pools() noexcept;
		inline void sort_pool(std::size_t pool_number) noexcept;
		inline void sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM

		inline void reset_pools() noexcept;
		inline void reset_pool(std::size_t pool_number) noexcept;
		inline void reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;

		bool new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
			const std::size_t* const input_block_instances, std::size_t pool_align);

#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
		bool new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
			std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align);
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST

		void delete_pools();

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_start() noexcept { std::lock_guard<mutex_Ty> resource_lock(resource_mutex); record_on = true; };
		void record_stop() noexcept { std::lock_guard<mutex_Ty> resource_lock(resource_mutex); record_on = false; };
		void record_reset() noexcept;

		void record_say();
		template <typename msg_Ty> void record_say
		(const msg_Ty& msg);
		template <typename msg_Ty1, typename msg_Ty2> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_say
		(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);

#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write(const char* const filename);
		template <typename msg_Ty> void record_write
		(const char* const filename, const msg_Ty& msg);
		template <typename msg_Ty1, typename msg_Ty2> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		multipool_resource_sync() = default;
		virtual ~multipool_resource_sync() { delete_pools(); }

	private:

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_say_sub();
#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write_sub(std::ofstream& rec_write);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		multipool_resource_sync(const welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>&) = delete;
		welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>& operator=
			(const welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>&) = delete;
		multipool_resource_sync(welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>&&) = delete;
		welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>& operator=
			(welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>&&) = delete;
	};
#endif // WELP_MULTIPOOL_INCLUDE_MUTEX
#endif // WELP_MULTIPOOL_NO_TEMPLATE

	// non-templated memory resource single thread with a maximum of 4 pools
	class quadpool_resource
	{

	private:

		char** current_address_ptr[4] = { nullptr };
		char** first_address_ptr[4] = { nullptr };
		char* data_ptr[4] = { nullptr };
		char* data_ptr_unaligned[4] = { nullptr };

		std::size_t block_size[4] = { 0 };
		std::size_t block_instances[4] = { 0 };
		std::size_t number_of_pools = 0;
		std::size_t pool_align_size = 0;

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		char** top_address_ptr[4] = { nullptr };
		WELP_MULTIPOOL_RECORD_INT record_allocations[4] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_deallocations[4] = { 0 };
		std::size_t record_max_occupancy[4] = { 0 };
		WELP_MULTIPOOL_RECORD_INT record_denied_block_requests[4] = { 0 };
		std::size_t record_biggest_request = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_allocations = 0;
		WELP_MULTIPOOL_RECORD_INT record_failed_deallocations = 0;
		bool record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE

	public:

		inline void* allocate_byte(std::size_t bytes) noexcept;
		inline void* allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept;
		inline void* allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept;
		inline void* allocate_byte_in_pool_range(std::size_t bytes,
			std::size_t first_pool, std::size_t end_pool) noexcept;
		inline void* allocate_byte_padded_in_pool_range(std::size_t bytes, std::size_t line_size,
			std::size_t first_pool, std::size_t end_pool) noexcept;

		inline bool deallocate_ptr(void* ptr) noexcept;
		inline bool deallocate_ptr_in_pool(void* ptr, std::size_t pool_number) noexcept;
		inline bool deallocate_ptr_in_pool_range(void* ptr, std::size_t first_pool, std::size_t end_pool) noexcept;

		inline std::size_t blocks_remaining_byte(std::size_t bytes) noexcept;
		inline std::size_t blocks_remaining_in_pool(std::size_t pool_number) noexcept;
		inline std::size_t block_size_in_pool(std::size_t pool_number) const noexcept { return block_size[pool_number]; }

		inline bool owns_resources() const noexcept { return number_of_pools != 0; }
		inline std::size_t number_of_pools_allocated() const noexcept { return number_of_pools; }
		constexpr inline std::size_t maximum_number_of_pools() const noexcept { return 4; }

#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
		inline void sort_pools() noexcept;
		inline void sort_pool(std::size_t pool_number) noexcept;
		inline void sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM

		inline void reset_pools() noexcept;
		inline void reset_pool(std::size_t pool_number) noexcept;
		inline void reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept;

		bool new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
			const std::size_t* const input_block_instances, std::size_t pool_align);

#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
		bool new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
			std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align);
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST

		void delete_pools();

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_start() noexcept { record_on = true; };
		void record_stop() noexcept { record_on = false; };
		void record_reset() noexcept;

		void record_say();
		void record_say(const char* const msg);
		void record_say(const char* const msg1, const char* const msg2);
		void record_say(const char* const msg1, const char* const msg2, const char* const msg3);
		void record_say(const char* const msg1, const char* const msg2, const char* const msg3, const char* const msg4);

#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write(const char* const filename);
		void record_write(const char* const filename, const char* const msg);
		void record_write(const char* const filename, const char* const msg1, const char* const msg2);
		void record_write(const char* const filename, const char* const msg1, const char* const msg2, const char* const msg3);
		void record_write(const char* const filename, const char* const msg1, const char* const msg2, const char* const msg3, const char* const msg4);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		quadpool_resource() = default;
		~quadpool_resource() { delete_pools(); }

	private:

#ifdef WELP_MULTIPOOL_DEBUG_MODE
		void record_say_sub();
#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
		void record_write_sub(std::ofstream& rec_write);
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

		quadpool_resource(const welp::quadpool_resource&) = delete;
		welp::quadpool_resource& operator=(const welp::quadpool_resource&) = delete;
		quadpool_resource(welp::quadpool_resource&&) = delete;
		welp::quadpool_resource& operator=(welp::quadpool_resource&&) = delete;
	};
}



////// IMPLEMENTATIONS //////

#ifndef WELP_MULTIPOOL_NO_TEMPLATE
// ALLOCATE
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline Ty* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_type(std::size_t instances) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE PADDED
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline Ty* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_type_padded(std::size_t instances, std::size_t line_size) noexcept
{
	instances *= sizeof(Ty);
	{
		std::size_t line_size_m1 = line_size - 1;
		instances += ((line_size - (instances & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline Ty* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_type_in_pool(std::size_t instances, std::size_t pool_number) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	if (instances <= block_size[pool_number])
	{
		if (first_address_ptr[pool_number] < current_address_ptr[pool_number])
		{
			current_address_ptr[pool_number]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on)
			{
				record_allocations[pool_number]++;
				record_max_occupancy[pool_number] = (static_cast<std::size_t>(top_address_ptr[pool_number]
					- current_address_ptr[pool_number]) > record_max_occupancy[pool_number]) ?
					static_cast<std::size_t>(top_address_ptr[pool_number] - current_address_ptr[pool_number]) : record_max_occupancy[pool_number];
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[pool_number]));
		}
		else
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_denied_block_requests[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return nullptr;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline Ty* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_type_in_pool_range(std::size_t instances,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (instances <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE PADDED IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline Ty* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_type_padded_in_pool_range(
	std::size_t instances, std::size_t line_size, std::size_t first_pool, std::size_t end_pool) noexcept
{
	instances *= sizeof(Ty);
	{
		std::size_t line_size_m1 = line_size - 1;
		instances += ((line_size - (instances & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (instances <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE
template <std::size_t max_number_of_pools, class sub_allocator>
inline void* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_byte(std::size_t bytes) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED
template <std::size_t max_number_of_pools, class sub_allocator>
inline void* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator>
inline void* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	if (bytes <= block_size[pool_number])
	{
		if (first_address_ptr[pool_number] < current_address_ptr[pool_number])
		{
			current_address_ptr[pool_number]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on)
			{
				record_allocations[pool_number]++;
				record_max_occupancy[pool_number] = (static_cast<std::size_t>(top_address_ptr[pool_number]
					- current_address_ptr[pool_number]) > record_max_occupancy[pool_number]) ?
					static_cast<std::size_t>(top_address_ptr[pool_number] - current_address_ptr[pool_number]) : record_max_occupancy[pool_number];
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return static_cast<void*>(*current_address_ptr[pool_number]);
		}
		else
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_denied_block_requests[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return nullptr;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator>
inline void* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_byte_in_pool_range(std::size_t bytes,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator>
inline void* welp::multipool_resource<max_number_of_pools, sub_allocator>::allocate_byte_padded_in_pool_range(
	std::size_t bytes, std::size_t line_size, std::size_t first_pool, std::size_t end_pool) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// DEALLOCATE
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline bool welp::multipool_resource<max_number_of_pools, sub_allocator>::deallocate_ptr(Ty* ptr) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline bool welp::multipool_resource<max_number_of_pools, sub_allocator>::deallocate_ptr_in_pool(Ty* ptr, std::size_t pool_number) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	if ((data_ptr[pool_number] <= char_ptr) && (char_ptr < data_ptr[pool_number] + block_instances[pool_number] * block_size[pool_number]))
	{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		if (record_on) { record_deallocations[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
		* current_address_ptr[pool_number] = char_ptr;
		current_address_ptr[pool_number]++;
		return true;
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline bool welp::multipool_resource<max_number_of_pools, sub_allocator>::deallocate_ptr_in_pool_range(Ty* ptr,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// BLOCKS REMAINING
template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline std::size_t welp::multipool_resource<max_number_of_pools, sub_allocator>::blocks_remaining_type() noexcept
{
	std::size_t N = sizeof(Ty);
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (N <= block_size[n])
		{
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <class Ty> inline std::size_t welp::multipool_resource<max_number_of_pools, sub_allocator>::blocks_remaining_type(std::size_t instances) noexcept
{
	instances *= sizeof(Ty);
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline std::size_t welp::multipool_resource<max_number_of_pools, sub_allocator>::blocks_remaining_byte(std::size_t bytes) noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline std::size_t welp::multipool_resource<max_number_of_pools, sub_allocator>::blocks_remaining_in_pool(std::size_t pool_number) noexcept
{
	return static_cast<std::size_t>(current_address_ptr[pool_number] - first_address_ptr[pool_number]);
}


// SORT POOLS
#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::sort_pools() noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
	}
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::sort_pool(std::size_t pool_number) noexcept
{
	if (pool_number < number_of_pools)
	{
		std::sort(first_address_ptr[pool_number], current_address_ptr[pool_number], std::greater<char*>());
	}
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
		}
	}
}
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM


// RESET POOLS
template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::reset_pools() noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::reset_pool(std::size_t pool_number) noexcept
{
	if (pool_number < number_of_pools)
	{
		current_address_ptr[pool_number] = first_address_ptr[pool_number] + block_instances[pool_number];
		char* ptr = data_ptr[pool_number] + (block_instances[pool_number] - 1) * block_size[pool_number]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[pool_number]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[pool_number]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[pool_number];
		}
	}
}


template <std::size_t max_number_of_pools, class sub_allocator>
inline void welp::multipool_resource<max_number_of_pools, sub_allocator>::reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
			char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
			char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
			for (std::size_t k = block_instances[n]; k > 0; k--)
			{
				*address_ptr_iter++ = ptr;
				ptr -= block_size[n];
			}
		}
	}
}


// NEW POOLS
template <std::size_t max_number_of_pools, class sub_allocator>
bool welp::multipool_resource<max_number_of_pools, sub_allocator>::new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
	const std::size_t* const input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > max_number_of_pools) { input_number_of_pools = max_number_of_pools; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (max_number_of_pools < input_number_of_pools) ? max_number_of_pools : input_number_of_pools;
	std::memcpy(block_size, input_block_size, number_of_pools * sizeof(std::size_t));
	std::memcpy(block_instances, input_block_instances, number_of_pools * sizeof(std::size_t));
	pool_align_size = pool_align;
	sub_allocator _sub_allocator;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		try
		{
			data_ptr_unaligned[n] = _sub_allocator.allocate((block_instances[n] * block_size[n] + pool_align_m1)); // construct unaligned pool
		}
		catch (...)
		{
			delete_pools(); return false;
		}
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		try
		{
			first_address_ptr[n] = static_cast<char**>(static_cast<void*>(_sub_allocator.allocate(block_instances[n] * sizeof(char*)))); // construct pointer to first address
		}
		catch (...)
		{
			delete_pools(); return false;
		}
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}


#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
template <std::size_t max_number_of_pools, class sub_allocator>
bool welp::multipool_resource<max_number_of_pools, sub_allocator>::new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
	std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > max_number_of_pools) { input_number_of_pools = max_number_of_pools; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (max_number_of_pools < input_number_of_pools) ? max_number_of_pools : input_number_of_pools;
	const std::size_t* iter_block_size = input_block_size.begin();
	const std::size_t* iter_block_instances = input_block_instances.begin();
	for (std::size_t n = 0; n < input_number_of_pools; n++)
	{
		block_size[n] = *iter_block_size++;
		block_instances[n] = *iter_block_instances++;
	}
	pool_align_size = pool_align;
	sub_allocator _sub_allocator;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		try
		{
			data_ptr_unaligned[n] = _sub_allocator.allocate((block_instances[n] * block_size[n] + pool_align_m1)); // construct unaligned pool
		}
		catch (...)
		{
			delete_pools(); return false;
		}
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		try
		{
			first_address_ptr[n] = static_cast<char**>(static_cast<void*>(_sub_allocator.allocate(block_instances[n] * sizeof(char*)))); // construct pointer to first address
		}
		catch (...)
		{
			delete_pools(); return false;
		}
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST


// DELETE POOLS
template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::delete_pools()
{
	sub_allocator _sub_allocator;
	for (std::size_t n = 0; n < max_number_of_pools; n++)
	{
		if (data_ptr_unaligned[n] != nullptr)
		{
			_sub_allocator.deallocate(data_ptr_unaligned[n],
				((pool_align_size - 1) + block_instances[n] * block_size[n]) * sizeof(char));
		}
		if (first_address_ptr[n] != nullptr)
		{
			_sub_allocator.deallocate(static_cast<char*>(static_cast<void*>(first_address_ptr[n])), block_instances[n] * sizeof(char*));
		}
	}
	char** p = static_cast<char**>(data_ptr_unaligned);
	char*** q = static_cast<char***>(first_address_ptr);
	for (std::size_t n = max_number_of_pools; n > 0; n--)
	{
		*p++ = nullptr;
		*q++ = nullptr;
	}
	number_of_pools = 0;
	pool_align_size = 0;
}


#ifdef WELP_MULTIPOOL_DEBUG_MODE
// RECORD RESET
template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_reset() noexcept
{
	std::memset(record_allocations, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_deallocations, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_max_occupancy, 0, max_number_of_pools * sizeof(std::size_t));
	std::memset(record_denied_block_requests, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	record_biggest_request = 0;
	record_failed_allocations = 0;
	record_failed_deallocations = 0;
	record_on = false;
}


// RECORD SAY
template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say_sub()
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		std::cout << "\nPool ";
		std::cout.fill(' '); std::cout.width(4);
		std::cout << std::left << n;

		std::cout << "> block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	std::cout << "\nGlobal ";
	std::cout.fill(' '); std::cout.width(2);
	std::cout << std::left << "";

	std::cout << "> total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say()
{
	record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty> void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say(const msg_Ty& msg)
{
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2> void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
// RECORD WRITE
template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write_sub(std::ofstream& rec_write)
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		rec_write << "\nPool " << n

			<< "   > block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n]

			<< "\n   > blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n]

			<< "\n   > allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n]

			<< "\n   > maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	rec_write << "\nGlobal"

		<< "   > total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations

		<< "\n   > failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations

		<< "\n   > biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


template <std::size_t max_number_of_pools, class sub_allocator>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write(const char* const filename)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty> void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write(const char* const filename, const msg_Ty& msg)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2> void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::multipool_resource<max_number_of_pools, sub_allocator>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE


#ifdef WELP_MULTIPOOL_INCLUDE_MUTEX
// ALLOCATE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline Ty* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_type(std::size_t instances) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE PADDED
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline Ty* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_type_padded(std::size_t instances, std::size_t line_size) noexcept
{
	instances *= sizeof(Ty);
	{
		std::size_t line_size_m1 = line_size - 1;
		instances += ((line_size - (instances & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline Ty* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_type_in_pool(std::size_t instances, std::size_t pool_number) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	if (instances <= block_size[pool_number])
	{
		std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

		if (first_address_ptr[pool_number] < current_address_ptr[pool_number])
		{
			current_address_ptr[pool_number]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on)
			{
				record_allocations[pool_number]++;
				record_max_occupancy[pool_number] = (static_cast<std::size_t>(top_address_ptr[pool_number]
					- current_address_ptr[pool_number]) > record_max_occupancy[pool_number]) ?
					static_cast<std::size_t>(top_address_ptr[pool_number] - current_address_ptr[pool_number]) : record_max_occupancy[pool_number];
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[pool_number]));
		}
		else
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_denied_block_requests[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return nullptr;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline Ty* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_type_in_pool_range(std::size_t instances,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
	instances *= sizeof(Ty);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (instances <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE PADDED IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline Ty* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_type_padded_in_pool_range(
	std::size_t instances, std::size_t line_size, std::size_t first_pool, std::size_t end_pool) noexcept
{
	instances *= sizeof(Ty);
	{
		std::size_t line_size_m1 = line_size - 1;
		instances += ((line_size - (instances & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (instances > record_biggest_request) ? instances : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (instances <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<Ty*>(static_cast<void*>(*current_address_ptr[n]));
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_byte(std::size_t bytes) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	if (bytes <= block_size[pool_number])
	{
		std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

		if (first_address_ptr[pool_number] < current_address_ptr[pool_number])
		{
			current_address_ptr[pool_number]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on)
			{
				record_allocations[pool_number]++;
				record_max_occupancy[pool_number] = (static_cast<std::size_t>(top_address_ptr[pool_number]
					- current_address_ptr[pool_number]) > record_max_occupancy[pool_number]) ?
					static_cast<std::size_t>(top_address_ptr[pool_number] - current_address_ptr[pool_number]) : record_max_occupancy[pool_number];
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return static_cast<void*>(*current_address_ptr[pool_number]);
		}
		else
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_denied_block_requests[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return nullptr;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_byte_in_pool_range(std::size_t bytes,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void* welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::allocate_byte_padded_in_pool_range(
	std::size_t bytes, std::size_t line_size, std::size_t first_pool, std::size_t end_pool) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// DEALLOCATE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline bool welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::deallocate_ptr(Ty* ptr) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline bool welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::deallocate_ptr_in_pool(Ty* ptr, std::size_t pool_number) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	if ((data_ptr[pool_number] <= char_ptr) && (char_ptr < data_ptr[pool_number] + block_instances[pool_number] * block_size[pool_number]))
	{
		std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		if (record_on) { record_deallocations[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
		* current_address_ptr[pool_number] = char_ptr;
		current_address_ptr[pool_number]++;
		return true;
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL RANGE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline bool welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::deallocate_ptr_in_pool_range(Ty* ptr,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
	char* char_ptr = static_cast<char*>(static_cast<void*>(ptr));
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// BLOCKS REMAINING
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline std::size_t welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::blocks_remaining_type() noexcept
{
	std::size_t N = sizeof(Ty);
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (N <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <class Ty> inline std::size_t welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::blocks_remaining_type(std::size_t instances) noexcept
{
	instances *= sizeof(Ty);
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (instances <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline std::size_t welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::blocks_remaining_byte(std::size_t bytes) noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline std::size_t welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::blocks_remaining_in_pool(std::size_t pool_number) noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	return static_cast<std::size_t>(current_address_ptr[pool_number] - first_address_ptr[pool_number]);
}


// SORT POOLS
#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::sort_pools() noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
	}
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::sort_pool(std::size_t n) noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

	if (n < number_of_pools)
	{
		std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
	}
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
		}
	}
}
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM


// RESET POOLS
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::reset_pools() noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::reset_pool(std::size_t pool_number) noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

	if (pool_number < number_of_pools)
	{
		current_address_ptr[pool_number] = first_address_ptr[pool_number] + block_instances[pool_number];
		char* ptr = data_ptr[pool_number] + (block_instances[pool_number] - 1) * block_size[pool_number]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[pool_number]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[pool_number]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[pool_number];
		}
	}
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
inline void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		std::lock_guard<mutex_Ty> resource_lock(resource_mutex);

		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
			char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
			char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
			for (std::size_t k = block_instances[n]; k > 0; k--)
			{
				*address_ptr_iter++ = ptr;
				ptr -= block_size[n];
			}
		}
	}
}


// NEW POOLS
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
bool welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
	const std::size_t* const input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > max_number_of_pools) { input_number_of_pools = max_number_of_pools; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (max_number_of_pools < input_number_of_pools) ? max_number_of_pools : input_number_of_pools;
	std::memcpy(block_size, input_block_size, number_of_pools * sizeof(std::size_t));
	std::memcpy(block_instances, input_block_instances, number_of_pools * sizeof(std::size_t));
	pool_align_size = pool_align;
	sub_allocator _sub_allocator;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		data_ptr_unaligned[n] = _sub_allocator.allocate((block_instances[n] * block_size[n] + pool_align_m1)); // construct unaligned pool
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		first_address_ptr[n] = static_cast<char**>(static_cast<void*>(_sub_allocator.allocate(block_instances[n] * sizeof(char*)))); // construct pointer to first address
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}


#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
bool welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
	std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > max_number_of_pools) { input_number_of_pools = max_number_of_pools; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (max_number_of_pools < input_number_of_pools) ? max_number_of_pools : input_number_of_pools;
	const std::size_t* iter_block_size = input_block_size.begin();
	const std::size_t* iter_block_instances = input_block_instances.begin();
	for (std::size_t n = 0; n < input_number_of_pools; n++)
	{
		block_size[n] = *iter_block_size++;
		block_instances[n] = *iter_block_instances++;
	}
	pool_align_size = pool_align;
	sub_allocator _sub_allocator;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		data_ptr_unaligned[n] = _sub_allocator.allocate((block_instances[n] * block_size[n] + pool_align_m1)); // construct unaligned pool
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		first_address_ptr[n] = static_cast<char**>(static_cast<void*>(_sub_allocator.allocate(block_instances[n] * sizeof(char*)))); // construct pointer to first address
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST


// DELETE POOLS
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::delete_pools()
{
	sub_allocator _sub_allocator;
	for (std::size_t n = 0; n < max_number_of_pools; n++)
	{
		if (data_ptr_unaligned[n] != nullptr)
		{
			_sub_allocator.deallocate(data_ptr_unaligned[n],
				((pool_align_size - 1) + block_instances[n] * block_size[n]) * sizeof(char));
		}
		if (first_address_ptr[n] != nullptr)
		{
			_sub_allocator.deallocate(static_cast<char*>(static_cast<void*>(first_address_ptr[n])), block_instances[n] * sizeof(char*));
		}
	}
	char** p = static_cast<char**>(data_ptr_unaligned);
	char*** q = static_cast<char***>(first_address_ptr);
	for (std::size_t n = max_number_of_pools; n > 0; n--)
	{
		*p++ = nullptr;
		*q++ = nullptr;
	}
	number_of_pools = 0;
	pool_align_size = 0;
}


#ifdef WELP_MULTIPOOL_DEBUG_MODE
// RECORD RESET
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_reset() noexcept
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::memset(record_allocations, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_deallocations, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_max_occupancy, 0, max_number_of_pools * sizeof(std::size_t));
	std::memset(record_denied_block_requests, 0, max_number_of_pools * sizeof(WELP_MULTIPOOL_RECORD_INT));
	record_biggest_request = 0;
	record_failed_allocations = 0;
	record_failed_deallocations = 0;
	record_on = false;
}


// RECORD SAY
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say_sub()
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		std::cout << "\nPool ";
		std::cout.fill(' '); std::cout.width(4);
		std::cout << std::left << n;

		std::cout << "> block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	std::cout << "\nGlobal ";
	std::cout.fill(' '); std::cout.width(2);
	std::cout << std::left << "";

	std::cout << "> total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say()
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty> void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say(const msg_Ty& msg)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2> void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_say
(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
// RECORD WRITE
template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write_sub(std::ofstream& rec_write)
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		rec_write << "\nPool " << n

			<< "   > block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n]

			<< "\n   > blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n]

			<< "\n   > allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n]

			<< "\n   > maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	rec_write << "\nGlobal"

		<< "   > total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations

		<< "\n   > failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations

		<< "\n   > biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write(const char* const filename)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty> void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write(const char* const filename, const msg_Ty& msg)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2> void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


template <std::size_t max_number_of_pools, class sub_allocator, class mutex_Ty>
template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::multipool_resource_sync<max_number_of_pools, sub_allocator, mutex_Ty>::record_write
(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<mutex_Ty> resource_lock(resource_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE
#endif // WELP_MULTIPOOL_INCLUDE_MUTEX
#endif // WELP_MULTIPOOL_NO_TEMPLATE


// ALLOCATE BYTE
inline void* welp::quadpool_resource::allocate_byte(std::size_t bytes) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED
inline void* welp::quadpool_resource::allocate_byte_padded(std::size_t bytes, std::size_t line_size) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL
inline void* welp::quadpool_resource::allocate_byte_in_pool(std::size_t bytes, std::size_t pool_number) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	if (bytes <= block_size[pool_number])
	{
		if (first_address_ptr[pool_number] < current_address_ptr[pool_number])
		{
			current_address_ptr[pool_number]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on)
			{
				record_allocations[pool_number]++;
				record_max_occupancy[pool_number] = (static_cast<std::size_t>(top_address_ptr[pool_number]
					- current_address_ptr[pool_number]) > record_max_occupancy[pool_number]) ?
					static_cast<std::size_t>(top_address_ptr[pool_number] - current_address_ptr[pool_number]) : record_max_occupancy[pool_number];
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return static_cast<void*>(*current_address_ptr[pool_number]);
		}
		else
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_denied_block_requests[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			return nullptr;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE IN POOL RANGE
inline void* welp::quadpool_resource::allocate_byte_in_pool_range(std::size_t bytes,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// ALLOCATE BYTE PADDED IN POOL RANGE
inline void* welp::quadpool_resource::allocate_byte_padded_in_pool_range(
	std::size_t bytes, std::size_t line_size, std::size_t first_pool, std::size_t end_pool) noexcept
{
	{
		std::size_t line_size_m1 = line_size - 1;
		bytes += ((line_size - (bytes & line_size_m1)) & line_size_m1);
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on)
	{
		record_biggest_request = (bytes > record_biggest_request) ? bytes : record_biggest_request;
	}
#endif // WELP_MULTIPOOL_DEBUG_MODE
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if (bytes <= block_size[n])
		{
			if (first_address_ptr[n] < current_address_ptr[n])
			{
				current_address_ptr[n]--;
#ifdef WELP_MULTIPOOL_DEBUG_MODE
				if (record_on)
				{
					record_allocations[n]++;
					record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
						static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];
				}
#endif // WELP_MULTIPOOL_DEBUG_MODE
				return static_cast<void*>(*current_address_ptr[n]);
			}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			else
			{
				if (record_on) { record_denied_block_requests[n]++; }
			}
#endif // WELP_MULTIPOOL_DEBUG_MODE
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_allocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return nullptr;
}


// DEALLOCATE
inline bool welp::quadpool_resource::deallocate_ptr(void* ptr) noexcept
{
	char* char_ptr = static_cast<char*>(ptr);
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL
inline bool welp::quadpool_resource::deallocate_ptr_in_pool(void* ptr, std::size_t pool_number) noexcept
{
	char* char_ptr = static_cast<char*>(ptr);
	if ((data_ptr[pool_number] <= char_ptr) && (char_ptr < data_ptr[pool_number] + block_instances[pool_number] * block_size[pool_number]))
	{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		if (record_on) { record_deallocations[pool_number]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
		* current_address_ptr[pool_number] = char_ptr;
		current_address_ptr[pool_number]++;
		return true;
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// DEALLOCATE IN POOL RANGE
inline bool welp::quadpool_resource::deallocate_ptr_in_pool_range(void* ptr,
	std::size_t first_pool, std::size_t end_pool) noexcept
{
	char* char_ptr = static_cast<char*>(ptr);
	for (std::size_t n = first_pool; n < end_pool; n++)
	{
		if ((data_ptr[n] <= char_ptr) && (char_ptr < data_ptr[n] + block_instances[n] * block_size[n]))
		{
#ifdef WELP_MULTIPOOL_DEBUG_MODE
			if (record_on) { record_deallocations[n]++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
			* current_address_ptr[n] = char_ptr;
			current_address_ptr[n]++;
			return true;
		}
	}
#ifdef WELP_MULTIPOOL_DEBUG_MODE
	if (record_on) { record_failed_deallocations++; }
#endif // WELP_MULTIPOOL_DEBUG_MODE
	return false;
}


// BLOCKS REMAINING
inline std::size_t welp::quadpool_resource::blocks_remaining_byte(std::size_t bytes) noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		if (bytes <= block_size[n])
		{
			return static_cast<std::size_t>(current_address_ptr[n] - first_address_ptr[n]);
		}
	}
	return 0;
}


inline std::size_t welp::quadpool_resource::blocks_remaining_in_pool(std::size_t pool_number) noexcept
{
	return static_cast<std::size_t>(current_address_ptr[pool_number] - first_address_ptr[pool_number]);
}


// SORT POOLS
#ifdef WELP_MULTIPOOL_INCLUDE_ALGORITHM
inline void welp::quadpool_resource::sort_pools() noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
	}
}


inline void welp::quadpool_resource::sort_pool(std::size_t pool_number) noexcept
{
	if (pool_number < number_of_pools)
	{
		std::sort(first_address_ptr[pool_number], current_address_ptr[pool_number], std::greater<char*>());
	}
}


inline void welp::quadpool_resource::sort_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			std::sort(first_address_ptr[n], current_address_ptr[n], std::greater<char*>());
		}
	}
}
#endif // WELP_MULTIPOOL_INCLUDE_ALGORITHM


// RESET POOLS
inline void welp::quadpool_resource::reset_pools() noexcept
{
	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
}


inline void welp::quadpool_resource::reset_pool(std::size_t pool_number) noexcept
{
	if (pool_number < number_of_pools)
	{
		current_address_ptr[pool_number] = first_address_ptr[pool_number] + block_instances[pool_number];
		char* ptr = data_ptr[pool_number] + (block_instances[pool_number] - 1) * block_size[pool_number]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[pool_number]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[pool_number]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[pool_number];
		}
	}
}


inline void welp::quadpool_resource::reset_pool_range(std::size_t first_pool, std::size_t end_pool) noexcept
{
	if (first_pool < number_of_pools)
	{
		if (end_pool > number_of_pools) { end_pool = number_of_pools; }
		for (std::size_t n = first_pool; n < end_pool; n++)
		{
			current_address_ptr[n] = first_address_ptr[n] + block_instances[n];
			char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
			char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
			for (std::size_t k = block_instances[n]; k > 0; k--)
			{
				*address_ptr_iter++ = ptr;
				ptr -= block_size[n];
			}
		}
	}
}


// NEW POOLS
bool welp::quadpool_resource::new_pools(std::size_t input_number_of_pools, const std::size_t* const input_block_size,
	const std::size_t* const input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > 4) { input_number_of_pools = 4; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (4 < input_number_of_pools) ? 4 : input_number_of_pools;
	std::memcpy(block_size, input_block_size, number_of_pools * sizeof(std::size_t));
	std::memcpy(block_instances, input_block_instances, number_of_pools * sizeof(std::size_t));
	pool_align_size = pool_align;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		data_ptr_unaligned[n] = static_cast<char*>(std::malloc((block_instances[n] * block_size[n] + pool_align_m1))); // construct unaligned pool
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		first_address_ptr[n] = static_cast<char**>(std::malloc(block_instances[n] * sizeof(char*))); // construct pointer to first address
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}


#ifdef WELP_MULTIPOOL_INCLUDE_INITLIST
bool welp::quadpool_resource::new_pools(std::size_t input_number_of_pools, std::initializer_list<std::size_t> input_block_size,
	std::initializer_list<std::size_t> input_block_instances, std::size_t pool_align)
{
	delete_pools();
	if (input_number_of_pools == 0)
	{
		return false;
	}

	if (input_number_of_pools > 4) { input_number_of_pools = 4; }
	if (pool_align == 0) { pool_align = 1; }

	number_of_pools = (4 < input_number_of_pools) ? 4 : input_number_of_pools;
	const std::size_t* iter_block_size = input_block_size.begin();
	const std::size_t* iter_block_instances = input_block_instances.begin();
	for (std::size_t n = 0; n < input_number_of_pools; n++)
	{
		block_size[n] = *iter_block_size++;
		block_instances[n] = *iter_block_instances++;
	}
	pool_align_size = pool_align;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		std::size_t pool_align_m1 = pool_align - 1;
		data_ptr_unaligned[n] = static_cast<char*>(std::malloc((block_instances[n] * block_size[n] + pool_align_m1))); // construct unaligned pool
		if (data_ptr_unaligned[n] == nullptr) { delete_pools(); return false; }
		data_ptr[n] = data_ptr_unaligned[n] + ((pool_align - (reinterpret_cast<std::size_t>(data_ptr_unaligned[n]) & pool_align_m1)) & pool_align_m1); // aligned pool
		first_address_ptr[n] = static_cast<char**>(std::malloc(block_instances[n] * sizeof(char*))); // construct pointer to first address
		if (first_address_ptr[n] == nullptr) { delete_pools(); return false; }
		current_address_ptr[n] = first_address_ptr[n] + block_instances[n]; // construct pointer to current address
#ifdef WELP_MULTIPOOL_DEBUG_MODE
		top_address_ptr[n] = current_address_ptr[n]; // pointer to top address before usage
		record_reset(); record_on = false;
#endif // WELP_MULTIPOOL_DEBUG_MODE
		char* ptr = data_ptr[n] + (block_instances[n] - 1) * block_size[n]; // pointer to last block (will iter backwards)
		char** address_ptr_iter = first_address_ptr[n]; // pointer to first address (will iter forward)
		for (std::size_t k = block_instances[n]; k > 0; k--)
		{
			*address_ptr_iter++ = ptr;
			ptr -= block_size[n];
		}
	}
	return true;
}
#endif // WELP_MULTIPOOL_INCLUDE_INITLIST


// DELETE POOLS
void welp::quadpool_resource::delete_pools()
{
	for (std::size_t n = 0; n < 4; n++)
	{
		if (data_ptr_unaligned[n] != nullptr)
		{
			std::free(static_cast<void*>(data_ptr_unaligned[n]));
		}
		if (first_address_ptr[n] != nullptr)
		{
			std::free(static_cast<void*>(first_address_ptr[n]));
		}
	}
	char** p = static_cast<char**>(data_ptr_unaligned);
	char*** q = static_cast<char***>(first_address_ptr);
	for (std::size_t n = 4; n > 0; n--)
	{
		*p++ = nullptr;
		*q++ = nullptr;
	}
	number_of_pools = 0;
	pool_align_size = 0;
}


#ifdef WELP_MULTIPOOL_DEBUG_MODE
// RECORD RESET
void welp::quadpool_resource::record_reset() noexcept
{
	std::memset(record_allocations, 0, 4 * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_deallocations, 0, 4 * sizeof(WELP_MULTIPOOL_RECORD_INT));
	std::memset(record_max_occupancy, 0, 4 * sizeof(std::size_t));
	std::memset(record_denied_block_requests, 0, 4 * sizeof(WELP_MULTIPOOL_RECORD_INT));
	record_biggest_request = 0;
	record_failed_allocations = 0;
	record_failed_deallocations = 0;
	record_on = false;
}


// RECORD SAY
void welp::quadpool_resource::record_say_sub()
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		std::cout << "\nPool ";
		std::cout.fill(' '); std::cout.width(4);
		std::cout << std::left << n;

		std::cout << "> block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n] << std::endl;

		std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
		std::cout << "> maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	std::cout << "\nGlobal ";
	std::cout.fill(' '); std::cout.width(2);
	std::cout << std::left << "";

	std::cout << "> total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations << std::endl;

	std::cout.fill(' '); std::cout.width(9); std::cout << std::left << " ";
	std::cout << "> biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


void welp::quadpool_resource::record_say()
{
	record_say_sub();
}


void welp::quadpool_resource::record_say(const char* msg)
{
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}


void welp::quadpool_resource::record_say(const char* msg1, const char* msg2)
{
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}


void welp::quadpool_resource::record_say(const char* msg1, const char* msg2, const char* msg3)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}


void welp::quadpool_resource::record_say(const char* msg1, const char* msg2, const char* msg3, const char* msg4)
{
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_MULTIPOOL_INCLUDE_FSTREAM
// RECORD WRITE
void welp::quadpool_resource::record_write_sub(std::ofstream& rec_write)
{
	WELP_MULTIPOOL_RECORD_INT total_allocations = 0;
	WELP_MULTIPOOL_RECORD_INT total_deallocations = 0;

	for (std::size_t n = 0; n < number_of_pools; n++)
	{
		total_allocations += record_allocations[n];
		total_deallocations += record_deallocations[n];

		record_max_occupancy[n] = (static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) > record_max_occupancy[n]) ?
			static_cast<std::size_t>(top_address_ptr[n] - current_address_ptr[n]) : record_max_occupancy[n];

		rec_write << "\nPool " << n

			<< "   > block size in bytes : " << block_size[n]
			<< "   > number of blocks : " << block_instances[n]

			<< "\n   > blocks currently used : " << top_address_ptr[n] - current_address_ptr[n]
			<< "   > blocks currently available : " << current_address_ptr[n] - first_address_ptr[n]

			<< "\n   > allocations : " << record_allocations[n]
			<< "   > deallocations : " << record_deallocations[n]

			<< "\n   > maximum occupancy recorded : " << record_max_occupancy[n]
			<< "   > denied block requests : " << record_denied_block_requests[n] << std::endl;
	}

	rec_write << "\nGlobal"

		<< "   > total allocations : " << total_allocations
		<< "   > total deallocations : " << total_deallocations

		<< "\n   > failed allocations : " << record_failed_allocations
		<< "   > failed deallocations : " << record_failed_deallocations

		<< "\n   > biggest request in bytes : " << record_biggest_request << "\n" << std::endl;
}


void welp::quadpool_resource::record_write(const char* const filename)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}


void welp::quadpool_resource::record_write(const char* const filename, const char* const msg)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


void welp::quadpool_resource::record_write(const char* const filename, const char* const msg1, const char* const msg2)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


void welp::quadpool_resource::record_write(const char* const filename, const char* const msg1,
	const char* const msg2, const char* const msg3)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}


void welp::quadpool_resource::record_write(const char* const filename, const char* const msg1,
	const char* const msg2, const char* const msg3, const char* const msg4)
{
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}
#endif // WELP_MULTIPOOL_INCLUDE_FSTREAM
#endif // WELP_MULTIPOOL_DEBUG_MODE

#undef WELP_MULTIPOOL_RECORD_INT


#endif // WELP_MULTIPOOL_RESOURCE_HPP


// welp_multipool_resource.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
