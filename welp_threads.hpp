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

#ifdef WELP_THREADS_DEBUG_MODE
#include <iostream>
#ifdef WELP_THREADS_INCLUDE_FSTREAM
#include <fstream>
#endif //  WELP_THREADS_INCLUDE_FSTREAM
#endif // WELP_THREADS_DEBUG_MODE


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

#ifdef WELP_THREADS_DEBUG_MODE
		void record_start() noexcept { std::lock_guard<std::mutex> resource_lock(task_mutex); record_on = true; };
		void record_stop() noexcept { std::lock_guard<std::mutex> resource_lock(task_mutex); record_on = false; };
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

#ifdef WELP_THREADS_INCLUDE_FSTREAM
		void record_write(const char* const filename);
		template <typename msg_Ty> void record_write
		(const char* const filename, const msg_Ty& msg);
		template <typename msg_Ty1, typename msg_Ty2> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3);
		template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4> void record_write
		(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4);
#endif // WELP_THREADS_INCLUDE_FSTREAM
#endif // WELP_THREADS_DEBUG_MODE

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


		template <class function_Ty, class ... _Args> bool force_async_task_sub(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> bool force_priority_async_task_sub(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool force_async_task_sub(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> bool force_priority_async_task_sub(welp::async_task_end& package, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool force_async_task_sub(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		bool force_priority_async_task_sub(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args);

#ifdef WELP_THREADS_DEBUG_MODE
		std::atomic<std::size_t> record_max_occupancy{ 0 };
		std::atomic<std::size_t> record_completed_task_count{ 0 };
		std::size_t record_accepted_task_count = 0;
		std::size_t record_denied_task_count = 0;
		std::atomic<std::size_t> record_delayed_task_count{ 0 };
		bool record_on = false;

		void record_say_sub();
		void record_write_sub(std::ofstream& rec_write);
#endif // WELP_THREADS_DEBUG_MODE
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


template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(package, task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(package, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(package, task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(package, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);

		last_task_ptr++;
		if (last_task_ptr == task_buffer_end_ptr) { last_task_ptr = task_buffer_data_ptr; }
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(package, task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(package, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
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
#ifdef WELP_THREADS_DEBUG_MODE
			if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
			return false;
		}

		package._task_running.store(true);
		package._task_denied.store(false);
		unfinished_tasks.fetch_add(1);
		task_condition_var.notify_one();
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on) { record_denied_task_count++; }
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
{
	if (threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(package, task, std::forward<_Args>(args)...))
		{
			record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(package, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
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
	
	input_task_buffer_size = (input_number_of_threads > input_task_buffer_size) ? input_number_of_threads : input_task_buffer_size;

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
#ifdef WELP_THREADS_DEBUG_MODE
					record_completed_task_count.fetch_add(1);
#endif // WELP_THREADS_DEBUG_MODE
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


#ifdef WELP_THREADS_DEBUG_MODE
template <class _Allocator>
void welp::threads<_Allocator>::record_reset() noexcept
{
	record_max_occupancy.store(0);
	record_completed_task_count.store(0);
	record_accepted_task_count = 0;
	record_denied_task_count = 0;
	record_delayed_task_count.store(0);
	record_on = false;
}


template <class _Allocator>
void welp::threads<_Allocator>::record_say_sub()
{
	std::cout << "\nThreads   > number of threads : " << _number_of_threads
		<< "   > task buffer size : " << ((task_buffer_data_ptr != nullptr) ? static_cast<std::size_t>(task_buffer_end_ptr - task_buffer_data_ptr) - 1 : 0)
		<< "\n          > tasks accepted : " << record_accepted_task_count
		<< "   > tasks accepted with delay : " << record_delayed_task_count.load()
		<< "   > tasks denied : " << record_denied_task_count
		<< "\n          > tasks completed : " << record_completed_task_count.load()
		<< "   > currently unfinished tasks : " << unfinished_tasks.load()
		<< "\n          > maximum occupancy recorded in task buffer : " << record_max_occupancy.load() << "\n" << std::endl;
}

template <class _Allocator>
void welp::threads<_Allocator>::record_say()
{
	std::lock_guard<std::mutex> lock(task_mutex);
	record_say_sub();
}

template <class _Allocator> template <typename msg_Ty>
void welp::threads<_Allocator>::record_say(const msg_Ty& msg)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_THREADS_INCLUDE_FSTREAM
template <class _Allocator>
void welp::threads<_Allocator>::record_write_sub(std::ofstream& rec_write)
{
	rec_write << "\nThreads   > number of threads : " << _number_of_threads
		<< "   > task buffer size : " << ((task_buffer_data_ptr != nullptr) ? static_cast<std::size_t>(task_buffer_end_ptr - task_buffer_data_ptr) - 1 : 0)
		<< "\n     > tasks accepted : " << record_accepted_task_count
		<< "   > tasks accepted with delay : " << record_delayed_task_count.load()
		<< "   > tasks denied : " << record_denied_task_count
		<< "\n     > tasks completed : " << record_completed_task_count.load()
		<< "   > currently unfinished tasks : " << unfinished_tasks.load()
		<< "\n     > maximum occupancy recorded in task buffer : " << record_max_occupancy.load() << "\n" << std::endl;
}

template <class _Allocator>
void welp::threads<_Allocator>::record_write(const char* const filename)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty& msg)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<std::mutex> lock(task_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}
#endif // WELP_THREADS_INCLUDE_FSTREAM
#endif // WELP_THREADS_DEBUG_MODE


template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_async_task_sub(const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_priority_async_task_sub(const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_async_task_sub(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_priority_async_task_sub(welp::async_task_end& package, const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_async_task_sub(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_priority_async_task_sub(welp::async_task_result<return_Ty>& package, const function_Ty& task, _Args&& ... args)
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
#ifdef WELP_THREADS_DEBUG_MODE
		if (record_on)
		{
			record_accepted_task_count++;
			std::size_t temp = unfinished_tasks.load();
			if (temp > record_max_occupancy.load()) { record_max_occupancy.store(temp); }
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		package._task_denied.store(true);
		return false;
	}
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
