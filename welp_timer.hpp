// welp_timer.hpp - last update : 15 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_TIMER_HPP
#define WELP_TIMER_HPP


////// INCLUDES //////

#include <cstdlib>
#include <chrono>


////// DESCRIPTIONS //////

namespace welp
{
	template <typename float_Ty, std::size_t set_number> class timer_steady
	{

	public:

		template <std::size_t offset> inline void set_timeframe(float_Ty timeframe) noexcept;

		inline void start_timer() noexcept;

		template <std::size_t offset> inline void finish_timeframe() noexcept;
		inline void finish_timeframe(float_Ty timeframe) noexcept;

		template <std::size_t offset> inline void finish_timeframe_restart_timer() noexcept;
		inline void finish_timeframe_restart_timer(float_Ty timeframe) noexcept;

		timer_steady() = default;
		timer_steady(const welp::timer_steady<float_Ty, set_number>&) = default;
		timer_steady& operator=(const welp::timer_steady<float_Ty, set_number>&) = default;
		timer_steady(welp::timer_steady<float_Ty, set_number>&&) = default;
		timer_steady& operator=(welp::timer_steady<float_Ty, set_number>&&) = default;
		~timer_steady() = default;

	private:

		std::chrono::time_point<std::chrono::steady_clock> _fstart = std::chrono::steady_clock::now();
		std::chrono::time_point<std::chrono::steady_clock>::rep clock_sleep_time[set_number] = {
			static_cast<std::chrono::time_point<std::chrono::steady_clock>::rep>(0) };
	};

	template <typename float_Ty, std::size_t set_number> class timer_system
	{

	public:

		template <std::size_t offset> inline void set_timeframe(float_Ty timeframe) noexcept;

		inline void start_timer() noexcept;

		template <std::size_t offset> inline void finish_timeframe() noexcept;
		inline void finish_timeframe(float_Ty timeframe) noexcept;

		template <std::size_t offset> inline void finish_timeframe_restart_timer() noexcept;
		inline void finish_timeframe_restart_timer(float_Ty timeframe) noexcept;

		timer_system() = default;
		timer_system(const welp::timer_system<float_Ty, set_number>&) = default;
		timer_system& operator=(const welp::timer_system<float_Ty, set_number>&) = default;
		timer_system(welp::timer_system<float_Ty, set_number>&&) = default;
		timer_system& operator=(welp::timer_system<float_Ty, set_number>&&) = default;
		~timer_system() = default;

	private:

		std::chrono::time_point<std::chrono::system_clock> _fstart = std::chrono::system_clock::now();
		std::chrono::time_point<std::chrono::system_clock>::rep clock_sleep_time[set_number] = {
			static_cast<std::chrono::time_point<std::chrono::system_clock>::rep>(0) };
	};
}


////// IMPLEMENTATIONS //////

template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_steady<float_Ty, set_number>::set_timeframe(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::steady_clock::duration::period::den / std::chrono::steady_clock::duration::period::num);
	clock_sleep_time[offset] =
		static_cast<std::chrono::time_point<std::chrono::steady_clock>::rep>(
			factor_s_to_tick * timeframe);
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_steady<float_Ty, set_number>::start_timer() noexcept
{
	_fstart = std::chrono::steady_clock::now();
}

template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_steady<float_Ty, set_number>::finish_timeframe() noexcept
{
	while ((std::chrono::steady_clock::now() - _fstart).count() < clock_sleep_time[offset]) {}
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_steady<float_Ty, set_number>::finish_timeframe(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::steady_clock::duration::period::den / std::chrono::steady_clock::duration::period::num);
	std::chrono::time_point<std::chrono::steady_clock>::rep unique_clock_sleep_time =
		static_cast<std::chrono::time_point<std::chrono::steady_clock>::rep>(factor_s_to_tick * timeframe);
	while ((std::chrono::steady_clock::now() - _fstart).count() < unique_clock_sleep_time) {}
}

template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_steady<float_Ty, set_number>::finish_timeframe_restart_timer() noexcept
{
	while ((std::chrono::steady_clock::now() - _fstart).count() < clock_sleep_time[offset]) {}
	_fstart = std::chrono::steady_clock::now();
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_steady<float_Ty, set_number>::finish_timeframe_restart_timer(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::steady_clock::duration::period::den / std::chrono::steady_clock::duration::period::num);
	std::chrono::time_point<std::chrono::steady_clock>::rep unique_clock_sleep_time =
		static_cast<std::chrono::time_point<std::chrono::steady_clock>::rep>(
			factor_s_to_tick * timeframe);
	while ((std::chrono::steady_clock::now() - _fstart).count() < unique_clock_sleep_time) {}
	_fstart = std::chrono::steady_clock::now();
}


template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_system<float_Ty, set_number>::set_timeframe(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::system_clock::duration::period::den / std::chrono::system_clock::duration::period::num);
	clock_sleep_time[offset] =
		static_cast<std::chrono::time_point<std::chrono::system_clock>::rep>(
			factor_s_to_tick * timeframe);
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_system<float_Ty, set_number>::start_timer() noexcept
{
	_fstart = std::chrono::system_clock::now();
}

template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_system<float_Ty, set_number>::finish_timeframe() noexcept
{
	while ((std::chrono::system_clock::now() - _fstart).count() < clock_sleep_time[offset]) {}
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_system<float_Ty, set_number>::finish_timeframe(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::system_clock::duration::period::den / std::chrono::system_clock::duration::period::num);
	std::chrono::time_point<std::chrono::system_clock>::rep unique_clock_sleep_time =
		static_cast<std::chrono::time_point<std::chrono::system_clock>::rep>(factor_s_to_tick * timeframe);
	while ((std::chrono::system_clock::now() - _fstart).count() < unique_clock_sleep_time) {}
}

template <typename float_Ty, std::size_t set_number> template <std::size_t offset>
inline void welp::timer_system<float_Ty, set_number>::finish_timeframe_restart_timer() noexcept
{
	while ((std::chrono::system_clock::now() - _fstart).count() < clock_sleep_time[offset]) {}
	_fstart = std::chrono::system_clock::now();
}

template <typename float_Ty, std::size_t set_number>
inline void welp::timer_system<float_Ty, set_number>::finish_timeframe_restart_timer(float_Ty timeframe) noexcept
{
	constexpr float_Ty factor_s_to_tick = static_cast<float_Ty>(
		std::chrono::system_clock::duration::period::den / std::chrono::system_clock::duration::period::num);
	std::chrono::time_point<std::chrono::system_clock>::rep unique_clock_sleep_time =
		static_cast<std::chrono::time_point<std::chrono::system_clock>::rep>(
			factor_s_to_tick * timeframe);
	while ((std::chrono::system_clock::now() - _fstart).count() < unique_clock_sleep_time) {}
	_fstart = std::chrono::system_clock::now();
}


#endif // WELP_TIMER_HPP


// welp_timer.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
