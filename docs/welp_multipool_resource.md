# welp_multipool_resource.hpp

welp_multipool_resource.h provides two classes :

- welp::multipool_resource is a class containing several pools of memory storing blocks to give to allocators. Two blocks in the same pool will have the same size. However, different pools will accomodate blocks of different sizes.
- welp::multipool_resource_sync is the same as welp::multipool_resource except that it contains a mutex that prevents block attribution/restitution issues when dealing with multiple threads. This class is only accessible if the macro WELP_MULTIPOOL_INCLUDE_MUTEX is defined before the inclusion of the header.

These classes aim to allocate and deallocate memory blocks of varying sizes in a deterministic O(1) timeframe.

# Member functions of welp::multipool_resource R

Template parameter max_number_of_pools is the maximal number of pools of R.

### Creating pools

	R.new_pools(N, {n1, n2, ... , nN}, {s1, s2, ... , sN}, Al); 

Creates (or recreates) N pools. Pool 1 stores n1 blocks of s1 bytes, pool 2 stores n2 blocks of s2 bytes, ... , Pool N stores nN blocks of sN bytes. First byte of each pool is aligned to a multiple of Al. The value of Al has to be a power of 2. Choose numbers such that n1 < n2 < ... < nN. The function returns boolean true if the creation of new resource is successful. If a resource was previously allocated, the creation of a new one will destroy the previous one, so make sure you don't have objects owning parts of it still in use. If stats recording is enabled, creating resource will stop the recording and reset stats.
In case of a failed allocation, the member function returns boolean false. In this case, the pools allocated (if not all of them failed) can be cleaned up automatically, so attemptying to use R.new_pools(...) again should not leak memory. Creating pools with initializer lists requires the macro WELP_MULTIPOOL_INCLUDE_INITLIST to be defined.
        
	R.new_pools(N, p, q, Al); 

Same with size_t* pointers p, q instead of initializer lists.

### Destroying pools

	R.delete_pools(); 

Deletes the pools and pointers.

### Allocating typed and non-type arrays

	R.allocate_type<Ty>(N); 

Allocates for an array of N objects of type Ty and returns a pointer to the first element.

	R.allocate_type_padded<Ty>(N, line_size); 

Allocates for an array of N objects of type Ty and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2.

	R.allocate_type_in_pool<Ty>(N, i); 

Allocates for an array of N objects of type Ty in pool number i.

	R.allocate_type_in_pool_range(N, i, j); 

Allocates for an array of N objects of type Ty searching for a pool of appropriate size from pool i up to pool j (j not included).

	R.allocate_type_padded_in_pool_range<Ty>(N, line_size, i, j); 

Allocates for an array of N objects of type Ty and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2. Searches for a pool of appropriate size from pool i up to pool j (j not included).

	R.allocate_byte(N); 

Allocates for an array of N bytes and returns a void* pointer to the first memory address.

	R.allocate_byte_padded(N, line_size); 

Allocates for an array of N bytes and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2. Returns à void* pointer.

	R.allocate_byte_in_pool(N, i); 

Allocates for an array of N bytes in pool number i and returns a void* pointer.

	R.allocate_byte_in_pool_range(N, i, j); 

Allocates for an array of N bytes searching for a pool of appropriate size from pool i up to pool j (j not included) and returns a void* pointer.

	R.allocate_byte_padded_in_pool_range(N, line_size, i, j); 

Allocates for an array of N bytes and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2. Searches for a pool of appropriate size from pool i up to pool j (j not included) and returns a void* pointer.

### Deallocating typed and non-typed arrays

	R.deallocate_ptr(ptr); 

Deallocate the array with ptr pointing to the first memory address. Returns true if and only if ptr is found to be in one of the pools.

	R.deallocate_ptr_in_pool(ptr, n); 

Deallocate the array with ptr pointing to the first memory address in pool number n. Returns true if and only if ptr is found to be in pool n.

	R.deallocate_ptr_in_pool(ptr, i, j); 

Deallocate the array with ptr pointing to the first memory address from pool i up to pool j (j not included). Returns true if and only if ptr is found to be in a pool of number between i and j (j not included).

### Checking the number of blocks remaining

	R.blocks_remaining_type<Ty>(); 

Returns how many times an object of type Ty can be stored.

	R.blocks_remaining_type<Ty>(N); 

Returns how many times an array of N objects of type Ty can be stored.

	R.blocks_remaining_byte(N); 

Returns how many times an array of N bytes can be stored.

	R.blocks_remaining_in_pool(N); 

Returns how many block remain available in pool number n.

### Sorting and resetting the pointers in the pools

	R.sort_pools()

Sorts all the pools to minimize internal fragmentation and keep better spatial locality for the future allocations. Necessitates the macro WELP_MULTIPOOL_INCLUDE_ALGORITHM to be defined.

	R.sort_pool(n)

Sorts the pointers of the remaining blocks in pool number n of R to minimize internal fragmentation and keep better spatial locality for the future allocations. Necessitates the macro WELP_MULTIPOOL_INCLUDE_ALGORITHM to be defined.

	R.sort_pool_range(i, j)

Sorts the pointers of the remaining blocks from pool i up to pool j (j not included) of R to minimize internal fragmentation and keep better spatial locality for the future allocations. Necessitates the macro WELP_MULTIPOOL_INCLUDE_ALGORITHM to be defined.

	R.reset_pools()
        
Resets all the pools. ATTN : Make sure no object still owns a memory block in any pool when you call this function, otherwise deallocations of still existing objects will make it bug. This will automatically get all the pools sorted with full capacity and offers an alternative to R.sort_pools() (this calls a sorting algorithm, which has a higher cost) or R.new_resource() (this calls malloc) if the condition for safety is fullfilled.

	R.reset_pool(n)
        
Resets pool n. ATTN : Make sure no object still owns a memory block in pool n when you call this function, otherwise deallocations of still existing objects will make it bug. This will automatically get the pool sorted with full capacity and offers an alternative to R.sort_pool() (this calls a sorting algorithm, which has a higher cost) or R.new_resource() (this calls malloc) if the condition for safety is fullfilled.

	R.reset_pool_range(i, j)
        
Resets from pool i up to pool j (j not included). ATTN : Make sure no object still owns a memory block in pool n when you call this function, otherwise deallocations of still existing objects will make it bug. This will automatically get the pool sorted with full capacity and offers an alternative to R.sort_pool() (this calls a sorting algorithm, which has a higher cost) or R.new_resource() (this calls malloc) if the condition for safety is fullfilled.

### Recording stats

All the functions recording stats are enabled if the macro WELP_MULTIPOOL_DEBUG_MODE is defined.

	R.record_start();

Starts recording stats.

	R.record_stop();

Stops recording cumulative stats. Doesn't destroy existing stats, can be switched on again with R.record_start() and will add new events cumulatively to the stats recorded before.

	R.record_reset();

Resets stats to zero. Stops recording stats.

	R.record_say();

Displays the recorded stats. Will display for each pool : Allocations, deallocations, maximum occupancy recorded, denied block requests, blocks currently used, blocks currently available. Also displays the number of times a block too big was requested.

	R.record_say(msg);

Same and displays message msg. Overloads can display up to 4 messages.

### Code example with the std::allocator template

	#define WELP_MULTIPOOL_DEBUG_MODE
	#include "welp_multipool_resource.h"
	#include <vector>

	welp::multipool_resource<4> R; // R can have up to 4 pools

	template <class Ty> class multipool_allocator
	{

	public:

		using value_type = Ty;

		inline Ty* allocate(size_t instances)
		{
			Ty* ptr = R.allocate_type<Ty>(instances);
			if (ptr != nullptr) { return ptr; }
			else
			{
				return static_cast<Ty*>(::operator new[](instances * sizeof(Ty)));
			}
		}
		inline void deallocate(Ty* const ptr, size_t)
		{
			if (!R.deallocate_ptr(ptr))
			{
				::operator delete[](static_cast<void*>(ptr));
			}
		}
		template <class rhs_Ty> inline bool operator==(
			const multipool_allocator<rhs_Ty>) const noexcept
		{
			return true;
		}
	};

	int main()
	{
		// reserve 2 pools with
		// 2000 blocks of 64 bytes and 1000 blocks of 1024 bytes
		// on 64 bytes alignment
		{
			size_t block_size[2] = { 64, 1024 };
			size_t block_instances[2] = { 2000, 1000 };
			if (!R.new_pools(2, block_size, block_instances, 64))
			{
				return -1;
			}
		}

		R.record_start(); // only available in debug mode

		std::vector<int, multipool_allocator<int>> V = { 1,2,3 };
		std::cout  << "content of vector V : " << V[0] << " " << V[1]
			<< " " << V[2] << "\n" << std::endl;

		R.record_stop(); // only available in debug mode
		R.record_say(); // only available in debug mode

		std::cin.get();
		return 0;
	}

### Code example with std::pmr::memory_resource (requires C++17)

	#define WELP_MULTIPOOL_DEBUG_MODE
	#include "welp_multipool_resource.h"
	#include <vector>
	#include <memory_resource>
	
	template <size_t max_number_of_pools> class multipool_pmr : public std::pmr::memory_resource,
	public welp::multipool_resource<max_number_of_pools>
	{
	
	public:
	
		void make_default_resource() noexcept
		{
			std::pmr::set_default_resource(static_cast<std::pmr::memory_resource*>(static_cast<void*>(this)));
		}
		inline std::pmr::memory_resource* resource_ptr() noexcept
		{
			return static_cast<std::pmr::memory_resource*>(static_cast<void*>(this));
		}
	
	private:
	
		void* do_allocate(const size_t bytes, size_t) override
		{
			void* ptr = this->allocate_byte(bytes);
			if (ptr != nullptr) { return ptr; }
			else
			{
				return ::operator new[](bytes);
			}
		}
		void do_deallocate(void* const ptr, size_t, size_t) override
		{
			if (!this->deallocate_ptr(ptr))
			{
				::operator delete[](ptr);
			}
		}
		bool do_is_equal(const std::pmr::memory_resource& rhs) const noexcept override
		{
			return this == &rhs;
		}
	};
	
	multipool_pmr<4> R; // R can have up to 4 pools
	
	int main()
	{
		// reserve 2 pools with
		// 2000 blocks of 64 bytes and 1000 blocks of 1024 bytes
		// on 64 bytes alignment
		{
			size_t block_size[2] = { 64, 1024 };
			size_t block_instances[2] = { 2000, 1000 };
			if (!R.new_pools(2, block_size, block_instances, 64))
			{
				return -1;
			}
		}
	
		R.make_default_resource();
	
		R.record_start(); // only available in debug mode
	
		std::vector<int, std::pmr::polymorphic_allocator<int>> V = { 1,2,3 };
		std::cout << "content of vector V : " << V[0] << " " << V[1]
			<< " " << V[2] << "\n" << std::endl;
	
		R.record_stop(); // only available in debug mode
		R.record_say(); // only available in debug mode
	
		std::cin.get();
		return 0;
	}
