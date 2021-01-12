# The C++ welp library
Depedency free single-header files all under the Unlicense license that could be welpful some day.

### Description of the files
- **welp_bits.hpp** : Wraps arrays of std::uint8_t into a class where bits, hexadecimals and bytes (as std::uint8_t or char) can be accessed and modified individually. Starting from a memory space of one byte. Needs little-endian memory layout.
- **welp_chrono.hpp** : Provides classes for measuring timings.
- **welp_cyclic_resource.hpp** : Provides a cyclic memory buffer that acts a a resource backing an allocator.
- **welp_global_type.hpp** : Wraps any class into a unique and globally accessible instance.
- **welp_link_node.hpp** : Provides nodes as observers receiving notification on events or nodes that can receive/transmit/create events.
- **welp_matrix.hpp** : Row-major layout. Provides linear algebra subroutines (70% efficiency on clock cycles in matrix multiplication if compiled with Clang with AVX&FMA), a resizable matrix class, some solvers, some data manipulation/selection helpers and some functions to crunch, parse and write flat files.
- **welp_multipool_resource.hpp** : Provide a multipool memory resource (with and without thread sync) backing an allocator.
- **welp_string_cat.hpp** : Concatenate strings a wee bit faster.
- **welp_threads.hpp** : Provide a thread pool that avoids std::future, std::result_of, std::invoke_result, and can avoid dynamic allocations if the functions launched as asynchronous tasks don't take too many arguments by using a fixed buffer for tasks. It can queue tasks at the end or the beginning of the waiting line depending on the desired level of priority.
- **welp_timer.hpp** : Provides classes for scheduling timings.

### Optional includes and debug mode
Every file contains its own mandatory includes and can include more with some preprocessor macros to unlock additional options. However, for every file welp_name.hpp, the macro WELP_NAME_INCLUDE_ALL will include all the necessary headers (those without hardware specifics) at once to unlock all the possible options. A full include would be done like this :

	#define WELP_NAME_INCLUDE_ALL
	#include "welp_name.hpp"

To avoid typing one macro per file to include every option possible, the following macro before all the inclusions can achieve full includes all at once :

	#define WELP_ALWAYS_INCLUDE_ALL

Every file welp_name.hpp can have its own debug mode (if available) activated with the following macro :

	#define WELP_NAME_DEBUG_MODE
	#include "welp_name.hpp"

To avoid typing one macro per file to activate the debug mode, the following macro before all the inclusions can be used :

	#define WELP_ALWAYS_DEBUG_MODE
