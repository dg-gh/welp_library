# welp_threads.hpp

welp_threads.hpp provides this main class :

- welp::threads< Allocator> is a thread pool that works with a fixed size circular buffer working like a queue for incoming tasks.

It also provides these two classes :

- welp::async_task_end for not allowing the main thread to progress until a task gets completed
- welp::async_task_result< Ty> for not allowing the main thread to progress until a task with a return type Ty gets completed and for storing the return value of type Ty of the task executed on the other thread.

# Member functions of welp::threads< Allocator> T

Template parameter Allocator can be of the form of std::allocator< char>, or any allocator that allocates arrays of chars.

### Creating and destroying threads

	T.new_threads(n, m); 

Creates n threads and a queue with a capacity for m tasks.

	T.delete_threads(); 

Deletes all the threads and the queue.

### Queuing tasks of the type f(arg1, ... , argn) -> void or f(arg1, ... , argn) -> Ty (discards the return)

	T.async_task(f, arg1, ... , argn); 

Attemps to queue the function f(arg1, ... , argn) -> void. Return true if the task is accepted, returns false if the buffer is full.

	T.force_async_task(f, arg1, ... , argn) 

Will queue the function f(arg1, ... , argn) -> void as soon as the buffer gets one free spot.

	T.priority_task(f, arg1, ... , argn); 

Attemps to queue the function f(arg1, ... , argn) -> void as the next task in the buffer to execute. Return true if the task is accepted, returns false if the buffer is full.

	T.force_priority_async_task(f, arg1, ... , argn) 

Will queue the function f(arg1, ... , argn) -> void as the next task in the buffer to execute as soon as the buffer gets one free spot.

### Queuing tasks of the type f(arg1, ... , argn) -> void or f(arg1, ... , argn) -> Ty (discards the return) with welp::async_task_end A

	welp::async_task_end A;
	T.async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.finish_task();
	// can't continue there until the task is finished

Similar to async_task except that the program can't go past A.finish_task() until the task is finished.

	welp::async_task_end A;
	T.force_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.finish_task();
	// can't continue there until the task is finished

Similar to force_async_task except that the program can't go past A.finish_task() until the task is finished.

	welp::async_task_end A;
	T.priority_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.finish_task();
	// can't continue there until the task is finished

Similar to priority_async_task except that the program can't go past A.finish_task() until the task is finished.

	welp::async_task_end A;
	T.force_priority_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.finish_task();
	// can't continue there until the task is finished

Similar to force_priority_async_task except that the program can't go past A.finish_task() until the task is finished.

### Queuing tasks of the type f(arg1, ... , argn) -> Ty with welp::async_task_result< Ty> A to store the return

	welp::async_task_result< Ty> A;
	T.async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.get(); // yields a reference to the return value of f
	// can't continue there until the task is finished

Similar to async_task except that the program can't go past A.get() until the task is finished.

	welp::async_task_result< Ty> A;
	T.force_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.get(); // yields a reference to the return value of f
	// can't continue there until the task is finished

Similar to force_async_task except that the program can't go past A.get() until the task is finished.

	welp::async_task_result< Ty> A;
	T.priority_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.get(); // yields a reference to the return value of f
	// can't continue there until the task is finished

Similar to priority_async_task except that the program can't go past A.get() until the task is finished.

	welp::async_task_result< Ty> A;
	T.force_priority_async_task(A, f, arg1, ... , argn);
	// can continue there even if the task is not finished
	A.get(); // yields a reference to the return value of f
	// can't continue there until the task is finished

Similar to force_priority_async_task except that the program can't go past A.get() until the task is finished.

### Other methods of welp::threads< Allocator>

	T.finish_all_tasks();
	// can't continue there until every task is finished

The program can't go past T.finish_all_tasks() until every the task is finished.

	T.number_of_threads(); 

Returns the number of threads maintained by T.

	T.task_buffer_size(); 

Returns the maximal number of tasks that can be stored in the queue of T.

# Other member functions of welp::async_task_end A or welp::async_task_result< Ty> A

	A.task_denied(); 

Returns true if T.async_task(A, f, arg1, ... , argn) or T.priority_async_task(A, f, arg1, ... , argn) was called when the task buffer was full and the task got denied.

	A.task_running(); 

Returns true if T.async_task(A, f, arg1, ... , argn) or T.priority_async_task(A, f, arg1, ... , argn) was called and the task hasn't been completed yet.
