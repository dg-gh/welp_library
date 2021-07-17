// welp_threads.hpp - last update : 24 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_THREADS_HPP
#define WELP_THREADS_HPP


////// INCLUDES //////

#include <cstdlib>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <utility>


// include all in one line with #define WELP_THREADS_INCLUDE_ALL
#if defined(WELP_THREADS_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_THREADS_INCLUDE_FSTREAM
#define WELP_THREADS_INCLUDE_FSTREAM
#endif
#endif // WELP_THREADS_INCLUDE_ALL


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_THREADS_DEBUG_MODE)
#define WELP_THREADS_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_THREADS_DEBUG_MODE
#include <iostream>
#ifdef WELP_THREADS_INCLUDE_FSTREAM
#include <fstream>
#endif // WELP_THREADS_INCLUDE_FSTREAM
#endif // WELP_THREADS_DEBUG_MODE

#ifndef WELP_THREADS_RECORD_INT
#define WELP_THREADS_RECORD_INT unsigned int
#endif // WELP_THREADS_RECORD_INT


////// DESCRIPTIONS //////

namespace welp
{
	// allocate and deallocate should preferably not throw
	// allocate should preferably return a nullptr in case of allocation failure
	// can be substituted by std::allocator<char> although a non-throwing version would be preferable
	class default_threads_allocator
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

		std::atomic<bool> m_task_running{ false };
		std::atomic<bool> m_task_denied{ false };

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

		async_task_result() = default;
		~async_task_result();

	private:

		Ty m_stored_value = Ty();
		std::atomic<bool> m_task_running{ false };
		std::atomic<bool> m_task_denied{ false };

		async_task_result(const welp::async_task_result<Ty>& rhs) = delete;
		welp::async_task_result<Ty>& operator=(const welp::async_task_result<Ty>& rhs) = delete;
		async_task_result(welp::async_task_result<Ty>&& rhs) = delete;
		welp::async_task_result<Ty>& operator=(welp::async_task_result<Ty>&& rhs) = delete;
	};

	template <class _Allocator = welp::default_threads_allocator> class threads : private _Allocator
	{

	public:

		template <class function_Ty, class ... _Args> bool async_task(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_async_task(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool priority_async_task(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_priority_async_task(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool priority_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> void force_priority_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		void force_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool priority_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		void force_priority_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);

		void finish_all_tasks() noexcept;
		inline std::size_t waiting_task_count() const noexcept;
		inline std::size_t unfinished_task_count() const noexcept;

		inline bool owns_resources() const noexcept;
		inline std::size_t number_of_threads() const noexcept;
		inline std::size_t task_buffer_size() const noexcept;

		bool new_threads(std::size_t input_number_of_threads, std::size_t input_task_buffer_size);
		void delete_threads() noexcept;

#ifdef WELP_THREADS_DEBUG_MODE
		void record_start() noexcept { std::lock_guard<std::mutex> resource_lock(m_mutex); m_DEBUG_record_on = true; };
		void record_stop() noexcept { std::lock_guard<std::mutex> resource_lock(m_mutex); m_DEBUG_record_on = false; };
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

		std::condition_variable m_condition_var;
		mutable std::mutex m_mutex;

		std::thread* m_threads_data_ptr = nullptr;
		std::size_t m_number_of_threads = 0;

		std::function<void()>* m_task_buffer_data_ptr = nullptr;
		std::function<void()>* m_task_buffer_end_ptr = nullptr;

		std::function<void()>* m_last_task_ptr = nullptr;
		std::function<void()>* m_next_task_ptr = nullptr;

		std::atomic<std::size_t> m_waiting_tasks{ 0 };
		std::atomic<std::size_t> m_unfinished_tasks{ 0 };

		bool m_stop_threads = true;
		bool m_threads_running = false;

		std::atomic<bool> m_waiting_for_finish{ false };

		threads(const welp::threads<_Allocator>&) = delete;
		welp::threads<_Allocator>& operator=(const welp::threads<_Allocator>&) = delete;
		threads(welp::threads<_Allocator>&&) = delete;
		welp::threads<_Allocator>& operator=(welp::threads<_Allocator>&&) = delete;


		template <class function_Ty, class ... _Args> bool force_async_task_sub(const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> bool force_priority_async_task_sub(const function_Ty& task, _Args&& ... args);

		template <class function_Ty, class ... _Args> bool force_async_task_sub(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);
		template <class function_Ty, class ... _Args> bool force_priority_async_task_sub(welp::async_task_end& box, const function_Ty& task, _Args&& ... args);

		template <class return_Ty, class function_Ty, class ... _Args>
		bool force_async_task_sub(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);
		template <class return_Ty, class function_Ty, class ... _Args>
		bool force_priority_async_task_sub(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args);

#ifdef WELP_THREADS_DEBUG_MODE
		std::atomic<WELP_THREADS_RECORD_INT> m_DEBUG_record_max_occupancy{ 0 };
		std::atomic<WELP_THREADS_RECORD_INT> m_DEBUG_record_completed_task_count{ 0 };
		WELP_THREADS_RECORD_INT m_DEBUG_record_accepted_task_count = 0;
		std::atomic<WELP_THREADS_RECORD_INT> m_DEBUG_record_denied_task_count{ 0 };
		std::atomic<WELP_THREADS_RECORD_INT> m_DEBUG_record_delayed_task_count{ 0 };
		bool m_DEBUG_record_on = false;

		void record_say_sub();
		void record_write_sub(std::ofstream& rec_write);
#endif // WELP_THREADS_DEBUG_MODE
	};
}


////// IMPLEMENTATIONS //////

inline void welp::async_task_end::finish_task() const noexcept
{
	while (m_task_running.load(std::memory_order_acquire)) {}
}

inline bool welp::async_task_end::task_running() const noexcept
{
	return m_task_running.load(std::memory_order_acquire);
}
inline bool welp::async_task_end::task_denied() const noexcept
{
	return m_task_denied.load(std::memory_order_acquire);
}

void welp::async_task_end::reset()
{
	while (m_task_running.load(std::memory_order_acquire)) {}
	m_task_running.store(false, std::memory_order_release);
	m_task_denied.store(false, std::memory_order_release);
}

welp::async_task_end::~async_task_end()
{
	while (m_task_running.load(std::memory_order_acquire)) {}
}


template <class Ty> inline const Ty& welp::async_task_result<Ty>::get() const noexcept
{
	while (m_task_running.load(std::memory_order_acquire)) {}
	return m_stored_value;
}
template <class Ty> inline Ty& welp::async_task_result<Ty>::get() noexcept
{
	while (m_task_running.load(std::memory_order_acquire)) {}
	return m_stored_value;
}

template <class Ty> inline bool welp::async_task_result<Ty>::task_running() const noexcept
{
	return m_task_running.load(std::memory_order_seq_cst);
}
template <class Ty> inline bool welp::async_task_result<Ty>::task_denied() const noexcept
{
	return m_task_denied.load(std::memory_order_seq_cst);
}

template <class Ty> void welp::async_task_result<Ty>::reset()
{
	while (m_task_running.load(std::memory_order_acquire)) {}
	m_stored_value = Ty();
	m_task_running.store(false, std::memory_order_release);
	m_task_denied.store(false, std::memory_order_release);
}

template <class Ty> welp::async_task_result<Ty>::~async_task_result()
{
	while (m_task_running.load(std::memory_order_acquire)) {}
}


template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on)
			{
				m_DEBUG_record_denied_task_count.fetch_add(1);
			}
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}

		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE

		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on)
			{
				m_DEBUG_record_denied_task_count.fetch_add(1);
			}
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE

		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE
}


template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
				box.m_task_running.store(false, std::memory_order_release);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on)
			{
				m_DEBUG_record_denied_task_count.fetch_add(1);
			}
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}

		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE

		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(box, task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1, std::memory_order_relaxed);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(box, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1, std::memory_order_relaxed); }
#endif // WELP_THREADS_DEBUG_MODE
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on)
			{
				m_DEBUG_record_denied_task_count.fetch_add(1);
			}
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE

		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(box, task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1, std::memory_order_relaxed);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(box, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1, std::memory_order_relaxed); }
#endif // WELP_THREADS_DEBUG_MODE
}


template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load())
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				box.m_stored_value = task(std::forward<_Args>(args)...);
				box.m_task_running.store(false, std::memory_order_release);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on) { m_DEBUG_record_denied_task_count.fetch_add(1); }
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}


		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE

		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_async_task_sub(box, task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1, std::memory_order_relaxed);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_async_task_sub(box, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1, std::memory_order_relaxed); }
#endif // WELP_THREADS_DEBUG_MODE
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::priority_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load())
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					box.m_stored_value = task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					box.m_stored_value = task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
			if (m_DEBUG_record_on)
			{
				m_DEBUG_record_denied_task_count.fetch_add(1);
			}
#endif // WELP_THREADS_DEBUG_MODE

			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE
		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_denied_task_count.fetch_add(1);
		}
#endif // WELP_THREADS_DEBUG_MODE
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
void welp::threads<_Allocator>::force_priority_async_task(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	if (m_threads_running)
	{
#ifdef WELP_THREADS_DEBUG_MODE
		if (!force_priority_async_task_sub(box, task, std::forward<_Args>(args)...))
		{
			m_DEBUG_record_delayed_task_count.fetch_add(1, std::memory_order_relaxed);
		}
		else { return; }
#endif // WELP_THREADS_DEBUG_MODE
		while (!force_priority_async_task_sub(box, task, std::forward<_Args>(args)...)) {}
	}
#ifdef WELP_THREADS_DEBUG_MODE
	else { m_DEBUG_record_denied_task_count.fetch_add(1, std::memory_order_relaxed); }
#endif // WELP_THREADS_DEBUG_MODE
}


template <class _Allocator>
void welp::threads<_Allocator>::finish_all_tasks() noexcept
{
	m_waiting_for_finish.store(true, std::memory_order_acquire);
	while (m_unfinished_tasks.load() != 0) {}
	m_waiting_for_finish.store(false, std::memory_order_release);
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::waiting_task_count() const noexcept
{
	return m_waiting_tasks.load();
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::unfinished_task_count() const noexcept
{
	return m_unfinished_tasks.load();
}


template <class _Allocator>
inline bool welp::threads<_Allocator>::owns_resources() const noexcept
{
	return m_number_of_threads != 0;
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::number_of_threads() const noexcept
{
	return m_number_of_threads;
}

template <class _Allocator>
inline std::size_t welp::threads<_Allocator>::task_buffer_size() const noexcept
{
	if (m_task_buffer_data_ptr != nullptr)
	{
		return static_cast<std::size_t>(m_task_buffer_end_ptr - m_task_buffer_data_ptr) - 1;
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
	m_stop_threads = false;

	if ((input_number_of_threads == 0) || (input_task_buffer_size == 0))
	{
		return false;
	}

	input_task_buffer_size = (input_number_of_threads > input_task_buffer_size) ? input_number_of_threads : input_task_buffer_size;

	try
	{
		m_threads_data_ptr = static_cast<std::thread*>(static_cast<void*>(
			this->allocate(input_number_of_threads * sizeof(std::thread))));
		if (m_threads_data_ptr == nullptr) { delete_threads(); return false; }
		m_number_of_threads = input_number_of_threads;

		m_task_buffer_data_ptr = static_cast<std::function<void()>*>(static_cast<void*>(
			this->allocate((input_task_buffer_size + 1) * sizeof(std::function<void()>))));
		if (m_task_buffer_data_ptr == nullptr) { delete_threads(); return false; }
		std::size_t input_task_buffer_size_p1 = input_task_buffer_size + 1;
		m_task_buffer_end_ptr = m_task_buffer_data_ptr + input_task_buffer_size_p1;
		m_next_task_ptr = m_task_buffer_data_ptr;
		m_last_task_ptr = m_task_buffer_data_ptr;

		for (std::size_t k = 0; k < input_task_buffer_size_p1; k++)
		{
			new (m_task_buffer_data_ptr + k) std::function<void()>();
		}

		for (std::size_t k = 0; k < input_number_of_threads; k++)
		{
			new (m_threads_data_ptr + k) std::thread([=]()
				{
					std::function<void()>* current_task_ptr;

					while (true)
					{
						{
							std::unique_lock<std::mutex> lock(m_mutex);
							m_condition_var.wait(lock, [=]() { return m_last_task_ptr != m_next_task_ptr || m_stop_threads; });

							if (m_last_task_ptr != m_next_task_ptr)
							{
								current_task_ptr = m_next_task_ptr++;
								if (m_next_task_ptr == m_task_buffer_end_ptr)
								{
									m_next_task_ptr = m_task_buffer_data_ptr;
								}
								m_waiting_tasks.fetch_sub(std::memory_order_release);
							}
							else
							{
								return;
							}
						}

						current_task_ptr->operator()();
						m_unfinished_tasks.fetch_sub(std::memory_order_release);

#ifdef WELP_THREADS_DEBUG_MODE
						if (m_DEBUG_record_on)
						{
							m_DEBUG_record_completed_task_count.fetch_add(1);
						}
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
		if (!((m_threads_data_ptr + k)->joinable()))
		{
			delete_threads(); return false;
		}
	}

	m_threads_running = true;

	return true;
}

template <class _Allocator>
void welp::threads<_Allocator>::delete_threads() noexcept
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		reinterpret_cast<std::atomic<bool>&>(m_stop_threads).store(true);
	}

	m_condition_var.notify_all();

	if (m_threads_data_ptr != nullptr)
	{
		for (size_t k = 0; k < m_number_of_threads; k++)
		{
			if ((m_threads_data_ptr + k)->joinable())
			{
				(m_threads_data_ptr + k)->join();
			}
			(m_threads_data_ptr + k)->~thread();
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(m_threads_data_ptr)), m_number_of_threads * sizeof(std::thread));
	}

	if (m_task_buffer_data_ptr != nullptr)
	{
		std::size_t task_buffer_size = static_cast<std::size_t>(m_task_buffer_end_ptr - m_task_buffer_data_ptr);
		for (size_t k = 0; k < task_buffer_size; k++)
		{
			(m_task_buffer_data_ptr + k)->~function();
		}
		this->deallocate(static_cast<char*>(static_cast<void*>(m_task_buffer_data_ptr)), task_buffer_size * sizeof(std::function<void()>));
	}

	m_threads_data_ptr = nullptr;
	m_number_of_threads = 0;

	m_task_buffer_data_ptr = nullptr;
	m_task_buffer_end_ptr = nullptr;
	m_next_task_ptr = nullptr;
	m_last_task_ptr = nullptr;

	m_threads_running = false;
}


#ifdef WELP_THREADS_DEBUG_MODE
template <class _Allocator>
void welp::threads<_Allocator>::record_reset() noexcept
{
	m_DEBUG_record_max_occupancy.store(0);
	m_DEBUG_record_completed_task_count.store(0);
	m_DEBUG_record_accepted_task_count = 0;
	m_DEBUG_record_denied_task_count = 0;
	m_DEBUG_record_delayed_task_count.store(0);
	m_DEBUG_record_on = false;
}


template <class _Allocator>
void welp::threads<_Allocator>::record_say_sub()
{
	std::cout << "\nThreads   > number of threads : " << m_number_of_threads
		<< "   > task buffer size : " << ((m_task_buffer_data_ptr != nullptr) ? static_cast<std::size_t>(m_task_buffer_end_ptr - m_task_buffer_data_ptr) - 1 : 0)
		<< "\n          > tasks accepted : " << m_DEBUG_record_accepted_task_count
		<< "   > tasks accepted with delay : " << m_DEBUG_record_delayed_task_count.load()
		<< "   > tasks denied : " << m_DEBUG_record_denied_task_count
		<< "\n          > tasks completed : " << m_DEBUG_record_completed_task_count.load()
		<< "   > currently unfinished tasks : " << m_unfinished_tasks.load()
		<< "\n          > maximum occupancy recorded in task buffer : " << m_DEBUG_record_max_occupancy.load() << "\n" << std::endl;
}

template <class _Allocator>
void welp::threads<_Allocator>::record_say()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	record_say_sub();
}

template <class _Allocator> template <typename msg_Ty>
void welp::threads<_Allocator>::record_say(const msg_Ty& msg)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << "[ " << msg << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n"; record_say_sub();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::threads<_Allocator>::record_say(const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << "[ " << msg1 << " " << msg2 << " " << msg3 << " " << msg4 << " ]\n"; record_say_sub();
}


#ifdef WELP_THREADS_INCLUDE_FSTREAM
template <class _Allocator>
void welp::threads<_Allocator>::record_write_sub(std::ofstream& rec_write)
{
	rec_write << "\nThreads   > number of threads : " << m_number_of_threads
		<< "   > task buffer size : " << ((m_task_buffer_data_ptr != nullptr) ? static_cast<std::size_t>(m_task_buffer_end_ptr - m_task_buffer_data_ptr) - 1 : 0)
		<< "\n     > tasks accepted : " << m_DEBUG_record_accepted_task_count
		<< "   > tasks accepted with delay : " << m_DEBUG_record_delayed_task_count.load()
		<< "   > tasks denied : " << m_DEBUG_record_denied_task_count
		<< "\n     > tasks completed : " << m_DEBUG_record_completed_task_count.load()
		<< "   > currently unfinished tasks : " << m_unfinished_tasks.load()
		<< "\n     > maximum occupancy recorded in task buffer : " << m_DEBUG_record_max_occupancy.load() << "\n" << std::endl;
}

template <class _Allocator>
void welp::threads<_Allocator>::record_write(const char* const filename)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty& msg)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::ofstream rec_write;
	rec_write.open(filename, std::ios::app);
	rec_write << "[ " << msg1 << " " << msg2 << " " << msg3 << " ]\n";
	record_write_sub(rec_write);
	rec_write.close();
}

template <class _Allocator> template <typename msg_Ty1, typename msg_Ty2, typename msg_Ty3, typename msg_Ty4>
void welp::threads<_Allocator>::record_write(const char* const filename, const msg_Ty1& msg1, const msg_Ty2& msg2, const msg_Ty3& msg3, const msg_Ty4& msg4)
{
	std::lock_guard<std::mutex> lock(m_mutex);
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
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			return false;
		}

		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
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
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
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
bool welp::threads<_Allocator>::force_async_task_sub(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				task(std::forward<_Args>(args)...);
				box.m_task_running.store(false, std::memory_order_release);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(true, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);
			return false;
		}

		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);
		return false;
	}
}

template <class _Allocator> template <class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_priority_async_task_sub(welp::async_task_end& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);


	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					box.m_task_running.store(false, std::memory_order_release);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					task(std::forward<_Args>(args)...);
					box.m_task_running.store(false, std::memory_order_release);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);
			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_async_task_sub(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			*m_last_task_ptr = [&]()
			{
				box.m_stored_value = task(std::forward<_Args>(args)...);
				box.m_task_running.store(false, std::memory_order_release);
			};
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);
			return false;
		}

		m_last_task_ptr++;
		if (m_last_task_ptr == m_task_buffer_end_ptr)
		{
			m_last_task_ptr = m_task_buffer_data_ptr;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);
		return false;
	}
}

template <class _Allocator> template <class return_Ty, class function_Ty, class ... _Args>
bool welp::threads<_Allocator>::force_priority_async_task_sub(welp::async_task_result<return_Ty>& box, const function_Ty& task, _Args&& ... args)
{
	while (m_waiting_for_finish.load(std::memory_order_relaxed)) {}
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_threads_running && (m_last_task_ptr + 1 != m_next_task_ptr) &&
		((m_next_task_ptr != m_task_buffer_data_ptr) || (m_last_task_ptr + 1 != m_task_buffer_end_ptr)) &&
		!box.m_task_running.load(std::memory_order_acquire))
	{
		m_waiting_tasks.fetch_add(1, std::memory_order_acquire);
		m_unfinished_tasks.fetch_add(1, std::memory_order_acquire);
		box.m_task_running.store(true, std::memory_order_acquire);
		box.m_task_denied.store(false, std::memory_order_acquire);

		try
		{
			if (m_next_task_ptr != m_task_buffer_data_ptr)
			{
				*(m_next_task_ptr - 1) = [&]()
				{
					box.m_stored_value = task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr--;
			}
			else
			{
				*(m_task_buffer_end_ptr - 1) = [&]()
				{
					box.m_stored_value = task(std::forward<_Args>(args)...);
					box.m_task_running.store(false);
				};
				m_next_task_ptr = m_task_buffer_end_ptr - 1;
			}
		}
		catch (...)
		{
			m_waiting_tasks.fetch_sub(1, std::memory_order_release);
			m_unfinished_tasks.fetch_sub(1, std::memory_order_release);
			box.m_task_running.store(false, std::memory_order_release);
			box.m_task_denied.store(true, std::memory_order_release);
			return false;
		}

		m_condition_var.notify_one();

#ifdef WELP_THREADS_DEBUG_MODE
		if (m_DEBUG_record_on)
		{
			m_DEBUG_record_accepted_task_count++;
			std::size_t temp = m_waiting_tasks.load();
			if (temp > m_DEBUG_record_max_occupancy.load())
			{
				m_DEBUG_record_max_occupancy.store(temp);
			}
		}
#endif // WELP_THREADS_DEBUG_MODE

		return true;
	}
	else
	{
		box.m_task_denied.store(true, std::memory_order_release);
		return false;
	}
}


#endif // WELP_THREADS_HPP


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
