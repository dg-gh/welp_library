# The C++ welp files
Depedency free single-header files all under the Unlicense license that could be welpful some day.

### Description of the files
- **welp_chrono.hpp** : Provides classes for measuring timings.
- **welp_cyclic_resource.hpp** : Provides a cyclic memory buffer that acts a a resource backing an allocator.
- **welp_global_type.hpp** : Wraps any class into a unique and globally accessible instance.
- **welp_link_node.hpp** : Provides nodes as observers receiving notification on events or nodes that can receive/transmit/create events.
- **welp_matrix.hpp** : Row-major layout. Provides linear algebra subroutines (70% efficiency on clock cycles in matrix multiplication if compiled with Clang with AVX&FMA), a resizable matrix class, some solvers, some data manipulation/selection helpers and some functions to crunch and parse flat files.
- **welp_multipool_resource.hpp** : Provide a multipool memory resource (with and without thread sync) backing an allocator.
- **welp_threads** : Provide a thread pool that avoids std::future and can avoid dynamic allocations if functions launched as asynchronous tasks don't take too many arguments, can queue tasks at the end or the beginning of the waiting line.
