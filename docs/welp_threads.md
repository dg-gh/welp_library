# welp_threads.hpp

welp_threads.hpp provides the main class :

- welp::threads< Allocator> is a thread pool that works with a fixed size circular buffer working like a queue for incoming tasks.

# Member functions of welp::threads< Allocator> T

Template parameter Allocator can be of the form of std::allocator< char>, or any allocator that allocates arrays of chars.

### Creating and destroying threads

	T.new_threads(n, m); 

Creates n threads and a queue with a capacity for m tasks.

	T.delete_threads(); 

Deletes all the threads and the queue.

### Queuing tasks of the type (arg1, ... , argn) -> void

	T.async_task(f, arg1, ... , argn); 

Attemps to queue the function f(arg1, ... , argn) -> void. Return true if the task is accepted, returns false if the buffer is full.

	T.force_async_task(f, arg1, ... , argn) 

Will queue the function f(arg1, ... , argn) -> void as soon as the buffer gets one spot free.

	T.priority_task(f, arg1, ... , argn); 

Attemps to queue the function f(arg1, ... , argn) -> void as the next task in the buffer to execute. Return true if the task is accepted, returns false if the buffer is full.

	T.force_priority_async_task(f, arg1, ... , argn) 

Will queue the function f(arg1, ... , argn) -> void as the next task in the buffer to execute as soon as the buffer gets one spot free.
