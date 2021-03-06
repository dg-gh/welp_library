// welp_chrono.hpp - last update : 24 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_CHRONO_HPP
#define WELP_CHRONO_HPP


////// INCLUDES //////

#include <chrono>


// include all in one line with #define WELP_CHRONO_INCLUDE_ALL
#if defined(WELP_CHRONO_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_CHRONO_INCLUDE_IOSTREAM
#define WELP_CHRONO_INCLUDE_IOSTREAM
#endif
#endif // WELP_CHRONO_INCLUDE_ALL


#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


////// DESCRIPTIONS //////

namespace welp
{
	template <typename float_Ty, typename int_Ty> class chrono_steady
	{

	public:

		// capture

		inline welp::chrono_steady<float_Ty, int_Ty>& start() noexcept;
		inline welp::chrono_steady<float_Ty, int_Ty>& stop() noexcept;
		inline float_Ty get() const noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_steady<float_Ty, int_Ty>& say() const;
		template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& say(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// total time

		inline welp::chrono_steady<float_Ty, int_Ty>& add_time() noexcept;
		inline float_Ty get_time() const noexcept;
		inline welp::chrono_steady<float_Ty, int_Ty>& reset_time() noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_steady<float_Ty, int_Ty>& say_time() const;
		template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& say_time(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// total count

		inline welp::chrono_steady<float_Ty, int_Ty>& add_count() noexcept;
		inline welp::chrono_steady<float_Ty, int_Ty>& add_count(int_Ty n) noexcept;
		inline int_Ty get_count() const noexcept;
		inline welp::chrono_steady<float_Ty, int_Ty>& reset_count() noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_steady<float_Ty, int_Ty>& say_count() const;
		template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& say_count(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// average

		inline float_Ty get_average() const noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_steady<float_Ty, int_Ty>& say_average() const;
		template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& say_average(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// global reset

		inline welp::chrono_steady<float_Ty, int_Ty>& reset_all() noexcept;


		chrono_steady() : m_start(std::chrono::steady_clock::now()), m_stop(std::chrono::steady_clock::now()),
			m_time(static_cast<float_Ty>(0)), m_count(static_cast<int_Ty>(0)) {}
		chrono_steady(const welp::chrono_steady<float_Ty, int_Ty>& rhs) = default;
		welp::chrono_steady<float_Ty, int_Ty>& operator=(const welp::chrono_steady<float_Ty, int_Ty>&) = default;
		chrono_steady(welp::chrono_steady<float_Ty, int_Ty>&&) = default;
		welp::chrono_steady<float_Ty, int_Ty>& operator=(welp::chrono_steady<float_Ty, int_Ty>&&) = default;
		~chrono_steady() = default;

	private:

		std::chrono::time_point<std::chrono::steady_clock> m_start;
		std::chrono::time_point<std::chrono::steady_clock> m_stop;
		float_Ty m_time;
		int_Ty m_count;
	};

	template <typename float_Ty, typename int_Ty> class chrono_system
	{

	public:

		// capture

		inline welp::chrono_system<float_Ty, int_Ty>& start() noexcept;
		inline welp::chrono_system<float_Ty, int_Ty>& stop() noexcept;
		inline float_Ty get() const noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_system<float_Ty, int_Ty>& say() const;
		template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& say(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// total time

		inline welp::chrono_system<float_Ty, int_Ty>& add_time() noexcept;
		inline float_Ty get_time() const noexcept;
		inline welp::chrono_system<float_Ty, int_Ty>& reset_time() noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_system<float_Ty, int_Ty>& say_time() const;
		template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& say_time(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// total count

		inline welp::chrono_system<float_Ty, int_Ty>& add_count() noexcept;
		inline welp::chrono_system<float_Ty, int_Ty>& add_count(int_Ty n) noexcept;
		inline int_Ty get_count() const noexcept;
		inline welp::chrono_system<float_Ty, int_Ty>& reset_count() noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_system<float_Ty, int_Ty>& say_count() const;
		template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& say_count(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// average

		inline float_Ty get_average() const noexcept;
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
		welp::chrono_system<float_Ty, int_Ty>& say_average() const;
		template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& say_average(const msg_Ty& msg) const;
#endif // WELP_CHRONO_INCLUDE_IOSTREAM

		// global reset

		inline welp::chrono_system<float_Ty, int_Ty>& reset_all() noexcept;


		chrono_system() : m_start(std::chrono::system_clock::now()), m_stop(std::chrono::system_clock::now()),
			m_time(static_cast<float_Ty>(0)), m_count(static_cast<int_Ty>(0)) {}
		chrono_system(const welp::chrono_system<float_Ty, int_Ty>& rhs) = default;
		welp::chrono_system<float_Ty, int_Ty>& operator=(const welp::chrono_system<float_Ty, int_Ty>& rhs) = default;
		chrono_system(welp::chrono_system<float_Ty, int_Ty>&& rhs) = default;
		welp::chrono_system<float_Ty, int_Ty>& operator=(welp::chrono_system<float_Ty, int_Ty>&& rhs) = default;
		~chrono_system() = default;

	private:

		std::chrono::time_point<std::chrono::system_clock> m_start;
		std::chrono::time_point<std::chrono::system_clock> m_stop;
		float_Ty m_time;
		int_Ty m_count;
	};
}


////// IMPLEMENTATIONS //////


// capture

template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::start() noexcept
{
	m_start = std::chrono::steady_clock::now(); return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::stop() noexcept
{
	m_stop = std::chrono::steady_clock::now(); return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_steady<float_Ty, int_Ty>::get() const noexcept
{
	return static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f);
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say() const
{
	std::cout << "time elapsed: " << static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f)
		<< " s" << std::endl; return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] time elapsed: "
		<< static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f) << " s" << std::endl;
	return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// total

template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::add_time() noexcept
{
	m_time += static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f); return *this;
}
template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_steady<float_Ty, int_Ty>::get_time() const noexcept { return m_time; }
template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::reset_time() noexcept
{
	m_time = static_cast<float_Ty>(0); return *this;
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_time() const
{
	std::cout << "total time elapsed: " << m_time << " s" << std::endl; return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_time(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] total time elapsed: " << m_time << " s" << std::endl; return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// count

template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::add_count() noexcept
{
	m_count++; return *this;
}
template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::add_count(int_Ty n) noexcept
{
	m_count += n; return *this;
}
template <typename float_Ty, typename int_Ty> inline int_Ty welp::chrono_steady<float_Ty, int_Ty>::get_count() const noexcept { return m_count; }
template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::reset_count() noexcept
{
	m_count = static_cast<int_Ty>(0); return *this;
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_count() const
{
	std::cout << "number of trials: " << m_count << std::endl; return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_count(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] number of trials: " << m_count << std::endl;
	return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// average

template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_steady<float_Ty, int_Ty>::get_average() const noexcept
{
	return m_time / static_cast<float_Ty>(m_count);
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty>welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_average() const
{
	std::cout << "average time per trial: " << m_time / m_count << std::endl; return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::say_average(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] average time per trial: " << m_time / static_cast<float_Ty>(m_count) << std::endl;
	return const_cast<welp::chrono_steady<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// reset all

template <typename float_Ty, typename int_Ty> inline welp::chrono_steady<float_Ty, int_Ty>& welp::chrono_steady<float_Ty, int_Ty>::reset_all() noexcept
{
	m_time = static_cast<float_Ty>(0); m_count = static_cast<int_Ty>(0); return *this;
}


// capture

template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::start() noexcept
{
	m_start = std::chrono::system_clock::now(); return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::stop() noexcept
{
	m_stop = std::chrono::system_clock::now(); return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_system<float_Ty, int_Ty>::get() const noexcept
{
	return static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f);
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say() const
{
	std::cout << "time elapsed: " << static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f)
		<< " s" << std::endl; return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] time elapsed: "
		<< static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f) << " s" << std::endl;
	return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// total

template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::add_time() noexcept
{
	m_time += static_cast<float_Ty>((m_stop - m_start).count()) * static_cast<float_Ty>(0.000000001f); return *this;
}
template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_system<float_Ty, int_Ty>::get_time() const noexcept { return m_time; }
template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::reset_time() noexcept
{
	m_time = static_cast<float_Ty>(0); return *this;
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_time() const
{
	std::cout << "total time elapsed: " << m_time << " s" << std::endl; return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_time(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] total time elapsed: " << m_time << " s" << std::endl; return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// count

template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::add_count() noexcept
{
	m_count++; return *this;
}
template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::add_count(int_Ty n) noexcept
{
	m_count += n; return *this;
}
template <typename float_Ty, typename int_Ty> inline int_Ty welp::chrono_system<float_Ty, int_Ty>::get_count() const noexcept { return m_count; }
template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::reset_count() noexcept
{
	m_count = static_cast<int_Ty>(0); return *this;
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_count() const
{
	std::cout << "number of trials: " << m_count << std::endl; return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_count(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] number of trials: " << m_count << std::endl;
	return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// average

template <typename float_Ty, typename int_Ty> inline float_Ty welp::chrono_system<float_Ty, int_Ty>::get_average() const noexcept
{
	return m_time / static_cast<float_Ty>(m_count);
}
#ifdef WELP_CHRONO_INCLUDE_IOSTREAM
template <typename float_Ty, typename int_Ty>welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_average() const
{
	std::cout << "average time per trial: " << m_time / m_count << std::endl; return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
template <typename float_Ty, typename int_Ty>
template <typename msg_Ty> welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::say_average(const msg_Ty& msg) const
{
	std::cout << "[ " << msg << " ] average time per trial: " << m_time / static_cast<float_Ty>(m_count) << std::endl;
	return const_cast<welp::chrono_system<float_Ty, int_Ty>&>(*this);
}
#endif // WELP_CHRONO_INCLUDE_IOSTREAM


// reset all

template <typename float_Ty, typename int_Ty> inline welp::chrono_system<float_Ty, int_Ty>& welp::chrono_system<float_Ty, int_Ty>::reset_all() noexcept
{
	m_time = static_cast<float_Ty>(0); m_count = static_cast<int_Ty>(0); return *this;
}

#endif // WELP_CHRONO_HPP


// welp_chrono.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
