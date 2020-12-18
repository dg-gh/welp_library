// welp_threads.hpp - last update : 15 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_THREADS_H
#define WELP_THREADS_H


////// INCLUDES //////

#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>


////// DESCRIPTIONS //////

namespace welp
{
	// allocate and deallocate should preferably not throw
	// allocate should preferably return a nullptr in case of allocation failure
	// can be substituted by std::allocator<char> although a non-throwing version would be preferable
	class _default_threads_allocator
	{

	public:

		inline char* allocate(std::size_t bytes) const noexcept { return static_cast<char*>(std::malloc(bytes)); }
		inline void deallocate(char* ptr, std::size_t) const noexcept { std::free(static_cast<void*>(ptr)); }
	};

	class async_task_end
	{

	public:

		template <class _Allocator> friend class threads;

		inline void finish_task() const noexcept;

		inline bool task_running() const noexcept;
		inline bool task_denied() const noexcept;

		void reset();

		async_task_end() = default;
		~async_task_end();

	private:

		std::atomic<bool> _task_running;
		std::atomic<bool> _task_denied;

		async_task_end(const welp::async_task_end& rhs) = delete;
		welp::async_task_end& operator=(const welp::async_task_end& rhs) = delete;
		async_task_end(welp::async_task_end&& rhs) = delete;
		welp::async_task_end& operator=(welp::async_task_end&& rhs) = delete;
	};

	template <class Ty> class async_task_result
	{

	public:

		template <class _Allocator> friend class threads;

		inline const Ty& get() const noexcept;
		inline Ty& get() noexcept;

		inline bool task_running() const noexcept;
		inline bool task_denied() const noexcept;

		void reset();

		async_task_result();
		~async_task_result();

	private:

		Ty stored_value;
		std::atomic<bool> _task_running;
		std::atomic<bool> _task_denied;

		async_task_result(const welp::async_task_result<Ty>& rhs) = delete;
		welp::async_task_result<Ty>& operator=(const welp::async_task_result<Ty>& rhs) = delete;
		async_task_result(welp::async_task_result<Ty>&& rhs) = delete;
		welp::async_task_result<Ty>& operator=(welp::async_task_result<Ty>&& rhs) = delete;
	};

	template <class _Allocator = welp::_default_threads_allocator> class threads : private _Allocator
	{

	public:

		template <class function_Ty, class ... _Args> bool async_task(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_async_task(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool priority_async_task(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_priority_async_task(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool priority_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_priority_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		void force_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool priority_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		void force_priority_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);

		void finish_all_tasks() noexcept;
		inline std::size_t unfinished_task_count() const noexcept;

		inline bool owns_resources() const noexcept;
		inline std::size_t number_of_threads() const noexcept;
		inline std::size_t task_buffer_size() const noexcept;

		bool new_threads(std::size_t input_number_of_threads, std::size_t input_task_buffer_size);
		void delete_threads() noexcept;

		threads() = default;
		virtual ~threads() { delete_threads(); }

	private:

		std::condition_variable task_condition_var;
		std::mutex task_mutex;

		std::thread* threads_data_ptr = nullptr;
		std::size_t _number_of_threads = 0;

		std::function<void()>* task_buffer_data_ptr = nullptr;
		std::function<void()>* task_buffer_end_ptr = nullptr;

		std::function<void()>* last_task_ptr = nullptr;
		std::function<void()>* next_task_ptr = nullptr;

		std::atomic<std::size_t> unfinished_tasks{ 0 };

		bool stop_threads = true;
		bool threads_running = false;

		std::atomic<bool> waiting_for_finish{ false };

		threads(const welp::threads<_Allocator>&) = delete;
		welp::threads<_Allocator>& operator=(const welp::threads<_Allocator>&) = delete;
		threads(welp::threads<_Allocator>&&) = delete;
		welp::threads<_Allocator>& operator=(welp::threads<_Allocator>&&) = delete;
	};
}


////// IMPLEMENTATIONS //////

inline void welp::async_task_end::finish_task() const noexcept
{
	while (_task_running.load()) {}
}

inline bool welp::async_task_end::task_running() const noexcept
{
	return _task_running.load();
}
inline bool welp::async_task_end::task_denied() const noexcept
{
	return _task_denied.load();
}

void welp::async_task_end::reset()
{
	while (_task_running.load()) {}
	_task_running.store(false);
	_task_denied.store(false);
}

welp::async_task_end::~async_task_end()
{
	while (_task_running.load()) {}
}


template <class Ty> inline const Ty& welp::async_task_result<Ty>::get() const noexcept
{
	while (_task_running.load()) {}
	return stored_value;
}
template <class Ty> inline Ty& welp::async_task_result<Ty>::get() noexcept
{
	while (_task_running.load()) {}
	return stored_value;
}

template <class Ty> inline bool welp::async_task_result<Ty>::task_running() const noexcept
{
	return _task_running.load();
}
template <class Ty> inline bool welp::async_task_result<Ty>::task_denied() const noexcept
{
	return _task_denied.load();
}

template <class Ty> void welp::async_task_result<Ty>::reset()
{
	while (_task_running.load()) {}
	stored_value = Ty();
	_task_running.store(false);
	_task_denied.store(false);
}

template <class Ty> welp::async_task_result<Ty>::async_task_result() : stored_value(Ty())
{
	_task_running.store(false);
	_task_denied.store(false);
}
template <class Ty> welp::async_task_result<Ty>::~async_task_result()
{
	while (_task_running.load()) {}
}


template <class _Allocator>
template <class function_Ty, class ... _Args> bool welp::threads<_Allocator>::async_task(const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)))
	{
		try
		{
			*last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
			};
		}
		catch (...)
		{
			return false;
		}

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		return false;
	}
}

template <class _Allocator>
template <class function_Ty, class ... _Args> void welp::threads<_Allocator>::force_async_task(const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!async_task(task, std::forward<_Args>(args)...)) {};
	}
}

template <class _Allocator>
template <class function_Ty, class ... _Args> bool welp::threads<_Allocator>::priority_async_task(const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)))
	{
		try
		{
			if (next_task_ptr != task_buffer_data_ptr)
			{
				*(next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				next_task_ptr--;
			}
			else
			{
				*(task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				next_task_ptr = task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			return false;
		}

		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		return false;
	}
}

template <class _Allocator>
template <class function_Ty, class ... _Args> void welp::threads<_Allocator>::force_priority_async_task(const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!priority_async_task(task, std::forward<_Args>(args)...)) {};
	}
}


template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)) &&
		!package._task_running.load())
	{
		try
		{
			*last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
				package._task_running.store(false);
			};
		}
		catch (...)
		{
			package._task_denied.store(true);
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!async_task(package, task, std::forward<_Args>(args)...)) {};
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)) &&
		!package._task_running.load())
	{
		try
		{
			if (next_task_ptr != task_buffer_data_ptr)
			{
				*(next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					package._task_running.store(false);
				};
				next_task_ptr--;
			}
			else
			{
				*(task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					package._task_running.store(false);
				};
				next_task_ptr = task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			package._task_denied.store(true);
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!priority_async_task(package, task, std::forward<_Args>(args)...)) {};
	}
}


template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)) &&
		!package._task_running.load())
	{
		try
		{
			*last_task_ptr = [&]()
			{
				package.stored_value = task(std::forward<_Args>(args)...);
				package._task_running.store(false);
			};
		}
		catch (...)
		{
			package._task_denied.store(true);
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!async_task(package, task, std::forward<_Args>(args)...)) {};
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	while (waiting_for_finish.load()) {}
	std::unique_lock<std::mutex> lock(task_mutex);

	if (threads_running && (last_task_ptr + 1 != next_task_ptr) &&
		((next_task_ptr != task_buffer_data_ptr) || (last_task_ptr + 1 != task_buffer_end_ptr)) &&
		!package._task_running.load())
	{
		try
		{
			if (next_task_ptr != task_buffer_data_ptr)
			{
				*(next_task_ptr - 1) = [&]()
				{
					package.stored_value = task(std::forward<_Args>(args)...);
					package._task_running.store(false);
				};
				next_task_ptr--;
			}
			else
			{
				*(task_buffer_end_ptr - 1) = [&]()
				{
					package.stored_value = task(std::forward<_Args>(args)...);
					package._task_running.store(false);
				};
				next_task_ptr = task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			package._task_denied.store(true);
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();

		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
		while (!priority_async_task(package, task, std::forward<_Args>(args)...)) {};
	}
}


template <class _Allocator>
void welp::threads<_Allocator>::finish_all_tasks() noexcept
{
	waiting_for_finish.store(true);
	while (unfinished_tasks.load() != 0) {}
	waiting_for_finish.store(false);
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::unfinished_task_count() const noexcept
{
	return unfinished_tasks.load();
}


template <class _Allocator>
inline bool welp::threads<_Allocator>::owns_resources() const noexcept
{
	return _number_of_threads != 0;
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::number_of_threads() const noexcept
{
	return _number_of_threads;
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::task_buffer_size() const noexcept
{
	if (task_buffer_data_ptr != nullptr)
	{
		return static_cast<std::size_t>(task_buffer_end_ptr - task_buffer_data_ptr) - 1;
	}
	else
	{
		return 0;
	}
}


template <class _Allocator>
bool welp::threads<_Allocator>::new_threads(std::size_t input_number_of_threads, std::size_t input_task_buffer_size)
{
	delete_threads();
	stop_threads = false;
	
	if ((input_number_of_threads == 0) || (input_task_buffer_size == 0))
	{
		return false;
	}

	try
	{
		threads_data_ptr = static_cast<std::thread*>(static_cast<void*>(
			this->allocate(input_number_of_threads * sizeof(std::thread))));
		if (threads_data_ptr == nullptr) { delete_threads(); return false; }
		_number_of_threads = input_number_of_threads;

		task_buffer_data_ptr = static_cast<std::function<void()>*>(static_cast<void*>(
			this->allocate((input_task_buffer_size + 1) * sizeof(std::function<void()>))));
		if (task_buffer_data_ptr == nullptr) { delete_threads(); return false; }
		std::size_t input_task_buffer_size_p1 = input_task_buffer_size + 1;
		task_buffer_end_ptr = task_buffer_data_ptr + input_task_buffer_size_p1;
		next_task_ptr = task_buffer_data_ptr;
		last_task_ptr = task_buffer_data_ptr;

		for (std::size_t k = 0; k < input_task_buffer_size_p1; k++)
		{
			new (task_buffer_data_ptr + k) std::function<void()>();
		}

		for (std::size_t k = 0; k < input_number_of_threads; k++)
		{
			new (threads_data_ptr + k) std::thread([=]() {

				std::function<void()>* current_task_ptr;

				while (true)
				{
					{
						std::unique_lock<std::mutex> lock(task_mutex);
						task_condition_var.wait(lock, [=]() { return last_task_ptr != next_task_ptr || stop_threads; });

						if (last_task_ptr != next_task_ptr)
						{
							current_task_ptr = next_task_ptr++;
							if (next_task_ptr == task_buffer_end_ptr)
							{
								next_task_ptr = task_buffer_data_ptr;
							}
						}
						else
						{
							return;
						}
					}
					current_task_ptr->operator()();
					unfinished_tasks.fetch_sub(1);
				}

				}
			);
		}
	}
	catch (...)
	{
		delete_threads(); return false;
	}

	for (std::size_t k = 0; k < input_number_of_threads; k++)
	{
		if (!((threads_data_ptr + k)->joinable()))
		{
			delete_threads(); return false;
		}
	}

	threads_running = true;

	return true;
}

template <class _Allocator>
void welp::threads<_Allocator>::delete_threads() noexcept
{
	{
		std::unique_lock<std::mutex> lock(task_mutex);
		stop_threads = true;
	}

	task_condition_var.notify_all();

	if (threads_data_ptr != nullptr)
	{
		for (size_t k = 0; k < _number_of_threads; k++)
		{
			if ((threads_data_ptr + k)->joinable())
			{
				(threads_data_ptr + k)->join();
			}
			(threads_data_ptr + k)->~thread();
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(threads_data_ptr)), _number_of_threads * sizeof(std::thread));
	}

	if (task_buffer_data_ptr != nullptr)
	{
		std::size_t task_buffer_size = static_cast<std::size_t>(task_buffer_end_ptr - task_buffer_data_ptr);
		for (size_t k = 0; k < task_buffer_size; k++)
		{
			(task_buffer_data_ptr + k)->~function();
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(task_buffer_data_ptr)),
			static_cast<std::size_t>(task_buffer_end_ptr - task_buffer_data_ptr) * sizeof(std::function<void()>));
	}

	threads_data_ptr = nullptr;
	_number_of_threads = 0;

	task_buffer_data_ptr = nullptr;
	task_buffer_end_ptr = nullptr;
	next_task_ptr = nullptr;
	last_task_ptr = nullptr;

	threads_running = false;
}


#endif // WELP_THREADS_H


// welp_threads.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
