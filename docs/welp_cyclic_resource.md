# welp_cyclic_resource.hpp

welp_cyclic_resource.hpp provides one class :

- welp::cyclic_resource<mem_align> is a class containing a pool of memory to give to allocators. It will allocate monotonically and cycle back of the beginning at the pool when the capacity is exhausted. It uses a first in first out storage strategy where the least recently used data gets overwritten. Allocations have to come from only one thread at a time. This type of memory resource can be used to sneakily overload the new operator for children who frequently forget to call delete.

# Member functions of welp::cyclic_resource<mem_align> R

Template parameter mem_align is the memory alignment of the pool and its pointers returned in allocations. It must be chosen as a power of 2.

### Creating and destroying the pool

	R.new_pool(N); 

Creates a pool of N bytes.

	R.delete_pool(); 

Deletes the pool.

### Allocating typed and non-typed arrays

	R.allocate_type<Ty>(N); 

Allocates for an array of N objects of type Ty and returns a pointer to the first element.

	R.allocate_type_padded<Ty>(N, line_size); 

Allocates for an array of N objects of type Ty and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2 and a bigger value than the default byte alignment of R.

	R.allocate_byte(N); 

Allocates for an array of N bytes and returns a void* pointer to the first memory address.

	R.allocate_byte_padded(N, line_size); 

Allocates for an array of N bytes and makes sure that the block allocated is padded such that it is a multiple of line_size. Usually, line_size would be the size in bytes of a cache line of the CPU. The value of line_size has to be a power of 2 and a bigger value than the default byte alignment of R.

### Deallocating typed and non-typed arrays

	R.deallocate_ptr(ptr); 

Deallocates the array with ptr pointing to the first memory address. Doesn't have any effect outside of debug mode.

### Recording stats

All the functions recording stats are enabled if the macro WELP_CYCLIC_DEBUG_MODE is defined.

	R.record_start();

Starts recording stats.

	R.record_stop();

Stops recording cumulative stats. Doesn't destroy existing stats, can be switched on again with R.record_start() and will add new events cumulatively to the stats recorded before.

	R.record_reset();

Resets stats to zero. Stops recording stats.

	R.record_say();

Displays the recorded stats. Will display for each pool : Allocations, deallocations, bytes allocated, number of cycles.

	R.record_say(msg);

Same and displays message msg. Overloads can display up to 4 messages.

### Code example with the std::allocator template

	#define WELP_CYCLIC_DEBUG_MODE
	#include "welp_cyclic_resource.h"
	#include <vector>
	
	welp::cyclic_resource<16> R; // R memory blocks will be aligned on 16 bytes
	
	template <class Ty> class cyclic_allocator
	{
	
	public:

		using value_type = Ty;
		
		inline Ty* allocate(size_t instances)
		{
			return R.allocate_type<Ty>(instances);
		}
		inline void deallocate(Ty* const ptr, size_t)
		{
			R.deallocate_ptr(ptr); // without any effect
		}
		template <class rhs_Ty> inline bool operator==(
			const cyclic_allocator<rhs_Ty>) const noexcept
		{
			return true;
		}
	};

	int main()
	{
		// reserve a pool of 2^12 bytes
		if (!R.new_pool(size_t(1) << 12))
		{
			return -1;
		}
	
		R.record_start(); // only available in debug mode
	
		std::vector<int, cyclic_allocator<int>> V = { 1,2,3 };
		std::cout << "content of vector V : " << V[0] << " " << V[1]
			<< " " << V[2] << "\n" << std::endl;
	
		R.record_stop(); // only available in debug mode
		R.record_say(); // only available in debug mode
	
		std::cin.get();
		return 0;
	}

### Code example with std::pmr::memory_resource (requires C++17)

	#define WELP_CYCLIC_DEBUG_MODE
	#include "welp_cyclic_resource.h"
	#include <vector>
	#include <memory_resource>
	
	template <size_t mem_align> class cyclic_pmr : public std::pmr::memory_resource,
	public welp::cyclic_resource<mem_align>
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
			return this->allocate_byte(bytes);
		}
		void do_deallocate(void* const ptr, size_t, size_t) override
		{
			this->deallocate_ptr(ptr); // without any effect
		}
		bool do_is_equal(const std::pmr::memory_resource& rhs) const noexcept override
		{
			return this == &rhs;
		}
	};
	
	cyclic_pmr<16> R; // R memory blocks will be aligned on 16 bytes
	
	int main()
	{
		// reserve a pool of 2^12 bytes
		if (!R.new_pool(size_t(1) << 12))
		{
			return -1;
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
