// welp_date_time.hpp - last update : 20 / 03 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef DGS_DATE_TIME_INCLUDED_H
#define DGS_DATE_TIME_INCLUDED_H


////// INCLUDES //////

#define WELP_DATE_TIME_INCLUDE_ALL

#if defined(WELP_DATE_TIME_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)

#ifndef WELP_TIMER_INCLUDE_CTIME
#define WELP_TIMER_INCLUDE_CTIME
#endif
#ifndef WELP_TIMER_INCLUDE_CHRONO
#define WELP_TIMER_INCLUDE_CHRONO
#endif
#ifndef WELP_TIMER_INCLUDE_IOSTREAM
#define WELP_TIMER_INCLUDE_IOSTREAM
#endif
#ifndef WELP_TIMER_INCLUDE_STRING
#define WELP_TIMER_INCLUDE_STRING
#endif

#endif // WELP_DATE_TIME_INCLUDE_ALL


#ifdef WELP_TIMER_INCLUDE_CTIME
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // _CRT_SECURE_NO_WARNINGS
#include <ctime>
#endif // WELP_TIMER_INCLUDE_CTIME

#ifdef WELP_TIMER_INCLUDE_CHRONO
#include <chrono>
#endif // WELP_TIMER_INCLUDE_CHRONO

#ifdef WELP_TIMER_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_TIMER_INCLUDE_IOSTREAM

#ifdef WELP_TIMER_INCLUDE_STRING
#include <string>
#endif // WELP_TIMER_INCLUDE_STRING


////// DESCRIPTIONS //////

namespace welp
{
	class date
	{

	public:

		inline int day() const noexcept;
		inline int month() const noexcept;
		inline int year() const noexcept;

#ifdef WELP_TIMER_INCLUDE_STRING
		std::string get_date_dmy(char delim) const;
		std::string get_date_mdy(char delim) const;
		std::string get_date_ymd(char delim) const;
		template <class _month_function> std::string get_date_text(char delim, _month_function month_func) const;
#endif // WELP_TIMER_INCLUDE_STRING

		inline int get_day_of_the_week() const noexcept;
#ifdef WELP_TIMER_INCLUDE_STRING
		template <class _day_function> std::string get_day_of_the_week_text(_day_function day_func) const;
#endif // WELP_TIMER_INCLUDE_STRING

		welp::date& set_date_numbers_dmy(int _day, int _month, int _year) noexcept;
#ifdef WELP_TIMER_INCLUDE_STRING
		welp::date& set_date_dmy(const std::string& str, char delim);
		welp::date& set_date_mdy(const std::string& str, char delim);
		welp::date& set_date_ymd(const std::string& str, char delim);
		template <class _month_function> welp::date& set_date_text(const std::string& str, char delim, _month_function month_func);
#endif // WELP_TIMER_INCLUDE_STRING

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
		welp::date& set_date_now_local();
		welp::date& set_date_now_gm();
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

		welp::date& operator++() noexcept;
		welp::date operator++(int) noexcept;
		welp::date& operator+=(int n) noexcept;
		welp::date& operator--() noexcept;
		welp::date operator--(int) noexcept;
		welp::date& operator-=(int n) noexcept;

		inline bool operator==(const welp::date& rhs) const noexcept;
		inline bool operator!=(const welp::date& rhs) const noexcept;
		inline bool operator<(const welp::date& rhs) const noexcept;
		inline bool operator>(const welp::date& rhs) const noexcept;
		inline bool operator<=(const welp::date& rhs) const noexcept;
		inline bool operator>=(const welp::date& rhs) const noexcept;

		date() = default;
		date(const welp::date&) = default;
		welp::date& operator=(const welp::date&) = default;
		date(welp::date&&) = default;
		welp::date& operator=(welp::date&&) = default;
		~date() = default;

	private:

		int m_day = 1;
		int m_month = 1;
		int m_year = 2000;

		int m_day_of_the_week = 6;
	};


	class time
	{

	public:

		inline int hour() const noexcept;
		inline int minute() const noexcept;
		inline int second() const noexcept;

#ifdef WELP_TIMER_INCLUDE_STRING
		std::string get_time_hms(char delim) const;
		std::string get_time_hm(char delim) const;
#endif // WELP_TIMER_INCLUDE_STRING

		welp::time& set_time(int _hour, int _minute, int _second) noexcept;
#ifdef WELP_TIMER_INCLUDE_STRING
		welp::time& set_time_hms(const std::string& str, char delim);
		welp::time& set_time_hm(const std::string& str, char delim);
#endif // WELP_TIMER_INCLUDE_STRING

		welp::time& set_stepsize(int hours_forward, int minutes_forward, int seconds_forward) noexcept;

		welp::time& tie_to_date(const welp::date* date_obj_ptr) noexcept;
		welp::time& tie_to_date() noexcept;

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
		welp::time& set_time_now_local();
		welp::time& set_time_now_gm();
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

		inline welp::time& forward_hms(int hours_forward, int minutes_forward, int seconds_forward) noexcept;
		inline welp::time& operator++() noexcept;
		inline welp::time operator++(int) noexcept;
		inline welp::time& operator+=(int n) noexcept;
		inline welp::time& operator--() noexcept;
		inline welp::time operator--(int) noexcept;
		inline welp::time& operator-=(int n) noexcept;

		inline bool operator==(const welp::time& rhs) const noexcept;
		inline bool operator!=(const welp::time& rhs) const noexcept;
		inline bool operator<(const welp::time& rhs) const noexcept;
		inline bool operator>(const welp::time& rhs) const noexcept;
		inline bool operator<=(const welp::time& rhs) const noexcept;
		inline bool operator>=(const welp::time& rhs) const noexcept;

		inline bool operator==(int n) const noexcept;
		inline bool operator!=(int n) const noexcept;
		inline bool operator<(int n) const noexcept;
		inline bool operator>(int n) const noexcept;
		inline bool operator<=(int n) const noexcept;
		inline bool operator>=(int n) const noexcept;

		time() = default;
		time(const welp::time&) = default;
		welp::time& operator=(const welp::time&) = default;
		time(welp::time&&) = default;
		welp::time& operator=(welp::time&&) = default;
		~time() = default;

	private:

		int m_hour = 0;
		int m_minute = 0;
		int m_second = 0;

		int m_seconds_from_midnight = 0;
		int m_step_size = 0;
		welp::date* m_date_ptr = nullptr;
	};
}


////// IMPLEMENTATION DATE //////

inline int welp::date::day() const noexcept { return m_day; }
inline int welp::date::month() const noexcept { return m_month; }
inline int welp::date::year() const noexcept { return m_year; }

#ifdef WELP_TIMER_INCLUDE_STRING
std::string welp::date::get_date_dmy(char delim) const
{
	if (m_day < 10)
	{
		std::string string_date = "0" + std::to_string(m_day);
		string_date += delim;
		if (m_month < 10) { string_date += '0'; }
		string_date += std::to_string(m_month);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(m_day);
		string_date += delim;
		if (m_month < 10) { string_date += '0'; }
		string_date += std::to_string(m_month);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
}
std::string welp::date::get_date_mdy(char delim) const
{
	if (m_month < 10)
	{
		std::string string_date = "0" + std::to_string(m_month);
		string_date += delim;
		if (m_day < 10) { string_date += '0'; }
		string_date += std::to_string(m_day);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(m_month);
		string_date += delim;
		if (m_day < 10) { string_date += '0'; }
		string_date += std::to_string(m_day);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
}
std::string welp::date::get_date_ymd(char delim) const
{
	std::string string_date = std::to_string(m_year) + delim;
	if (m_month < 10) { string_date += '0'; }
	string_date += std::to_string(m_month);
	string_date += delim;
	if (m_day < 10) { string_date += '0'; }
	string_date += std::to_string(m_day);
	return string_date;
}
template <class _month_function> std::string welp::date::get_date_text(char delim, _month_function month_func) const
{
	if (m_day < 10)
	{
		std::string string_date = "0" + std::to_string(m_day);
		string_date += delim;
		string_date += month_func(m_month);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(m_day);
		string_date += delim;
		string_date += month_func(m_month);
		string_date += delim;
		string_date += std::to_string(m_year);
		return string_date;
	}
}
#endif // WELP_TIMER_INCLUDE_STRING

inline int welp::date::get_day_of_the_week() const noexcept { return m_day_of_the_week; }
#ifdef WELP_TIMER_INCLUDE_STRING
template <class _day_function> std::string  welp::date::get_day_of_the_week_text(_day_function day_func) const
{
	return day_func(m_day_of_the_week);
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::date& welp::date::set_date_numbers_dmy(int _day, int _month, int _year) noexcept
{
	m_day = _day;
	m_month = _month;
	m_year = _year;

	int m = m_month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (m_year - 1) / 100;
		K = (m_year - 1) % 100;
	}
	else
	{
		J = m_year / 100;
		K = m_year % 100;
	}
	int q = m_day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	m_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
#ifdef WELP_TIMER_INCLUDE_STRING
welp::date& welp::date::set_date_dmy(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_day = str.substr(0, delim1);
	std::string string_month = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_year = str.substr(delim2 + 1, str.size() - delim2);

	m_day = std::stoi(string_day);
	m_month = std::stoi(string_month);
	m_year = std::stoi(string_year);

	int m = m_month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (m_year - 1) / 100;
		K = (m_year - 1) % 100;
	}
	else
	{
		J = m_year / 100;
		K = m_year % 100;
	}
	int q = m_day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	m_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
welp::date& welp::date::set_date_mdy(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_month = str.substr(0, delim1);
	std::string string_day = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_year = str.substr(delim2 + 1, str.size() - delim2);

	m_day = std::stoi(string_day);
	m_month = std::stoi(string_month);
	m_year = std::stoi(string_year);

	int m = m_month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (m_year - 1) / 100;
		K = (m_year - 1) % 100;
	}
	else
	{
		J = m_year / 100;
		K = m_year % 100;
	}
	int q = m_day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	m_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
welp::date& welp::date::set_date_ymd(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_year = str.substr(0, delim1);
	std::string string_month = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_day = str.substr(delim2 + 1, str.size() - delim2);

	m_day = std::stoi(string_day);
	m_month = std::stoi(string_month);
	m_year = std::stoi(string_year);

	int m = m_month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (m_year - 1) / 100;
		K = (m_year - 1) % 100;
	}
	else
	{
		J = m_year / 100;
		K = m_year % 100;
	}
	int q = m_day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	m_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
template <class _month_function> welp::date& welp::date::set_date_text(const std::string& str, char delim, _month_function month_func)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_day = str.substr(0, delim1);
	std::string string_month = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_year = str.substr(delim2 + 1, str.size() - delim2);

	m_day = std::stoi(string_day);
	m_month = month_func(string_month);
	m_year = std::stoi(string_year);

	int m = m_month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (m_year - 1) / 100;
		K = (m_year - 1) % 100;
	}
	else
	{
		J = m_year / 100;
		K = m_year % 100;
	}
	int q = m_day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	m_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
#endif // WELP_TIMER_INCLUDE_STRING

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
welp::date& welp::date::set_date_now_local()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::localtime(&system_clock_time);

	m_day = time_data->tm_mday;
	m_month = time_data->tm_mon + 1;
	m_year = time_data->tm_year + 1900;

	m_day_of_the_week = time_data->tm_wday;
	if (m_day_of_the_week == 0) { m_day_of_the_week = 7; }

	return *this;
}
welp::date& welp::date::set_date_now_gm()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::gmtime(&system_clock_time);

	m_day = time_data->tm_mday;
	m_month = time_data->tm_mon + 1;
	m_year = time_data->tm_year + 1900;

	m_day_of_the_week = time_data->tm_wday;
	if (m_day_of_the_week == 0) { m_day_of_the_week = 7; }

	return *this;
}
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

welp::date& welp::date::operator++() noexcept
{
	m_day_of_the_week++;
	if (m_day_of_the_week == 8) { m_day_of_the_week = 1; }

	m_day++;
	if (m_day > 28)
	{
		switch (m_month)
		{
		case 1: if (m_day > 31) { m_day = 1; m_month = 2; } break;
		case 2: if ((m_day == 29) && (((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0))) { break; }
			  else { m_day = 1; m_month = 3; break; }
		case 3: if (m_day > 31) { m_day = 1; m_month = 4; } break;
		case 4: if (m_day > 30) { m_day = 1; m_month = 5; } break;
		case 5: if (m_day > 31) { m_day = 1; m_month = 6; } break;
		case 6: if (m_day > 30) { m_day = 1; m_month = 7; } break;
		case 7: if (m_day > 31) { m_day = 1; m_month = 8; } break;
		case 8: if (m_day > 31) { m_day = 1; m_month = 9; } break;
		case 9: if (m_day > 30) { m_day = 1; m_month = 10; } break;
		case 10: if (m_day > 31) { m_day = 1; m_month = 11; } break;
		case 11: if (m_day > 30) { m_day = 1; m_month = 12; } break;
		case 12: if (m_day > 31) { m_day = 1; m_month = 1; m_year++; } break;

		default: m_day = 1; m_month = 1; m_year = 2000; m_day_of_the_week = 6; break;
		}
	}
	return *this;
}
welp::date welp::date::operator++(int) noexcept
{
	welp::date temp = *this;

	m_day_of_the_week++;
	if (m_day_of_the_week == 8) { m_day_of_the_week = 1; }

	m_day++;
	if (m_day > 28)
	{
		switch (m_month)
		{
		case 1: if (m_day > 31) { m_day = 1; m_month = 2; } break;
		case 2: if ((m_day == 29) && (((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0))) { break; }
			  else { m_day = 1; m_month = 3; break; }
		case 3: if (m_day > 31) { m_day = 1; m_month = 4; } break;
		case 4: if (m_day > 30) { m_day = 1; m_month = 5; } break;
		case 5: if (m_day > 31) { m_day = 1; m_month = 6; } break;
		case 6: if (m_day > 30) { m_day = 1; m_month = 7; } break;
		case 7: if (m_day > 31) { m_day = 1; m_month = 8; } break;
		case 8: if (m_day > 31) { m_day = 1; m_month = 9; } break;
		case 9: if (m_day > 30) { m_day = 1; m_month = 10; } break;
		case 10: if (m_day > 31) { m_day = 1; m_month = 11; } break;
		case 11: if (m_day > 30) { m_day = 1; m_month = 12; } break;
		case 12: if (m_day > 31) { m_day = 1; m_month = 1; m_year++; } break;

		default: m_day = 1; m_month = 1; m_year = 2000; m_day_of_the_week = 6; break;
		}
	}
	return temp;
}
welp::date& welp::date::operator+=(int n) noexcept
{
	if (n > 0)
	{
		for (; n > 0; n--) { ++(*this); }
	}
	else if (n < 0)
	{
		for (; n < 0; n++) { --(*this); }
	}
	return *this;
}
welp::date& welp::date::operator--() noexcept
{
	m_day_of_the_week--;
	if (m_day_of_the_week == 0) { m_day_of_the_week = 7; }

	m_day--;
	if (m_day <= 0)
	{
		switch (m_month)
		{
		case 1: m_day = 31; m_month = 12; m_year--; break;
		case 2: m_day = 31; m_month = 1; break;
		case 3: if (((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0)) { m_day = 29; }
			  else { m_day = 28; }
			  m_month = 2; break;
		case 4: m_day = 31; m_month = 3; break;
		case 5: m_day = 30; m_month = 4; break;
		case 6: m_day = 31; m_month = 5; break;
		case 7: m_day = 30; m_month = 6; break;
		case 8: m_day = 31; m_month = 7; break;
		case 9: m_day = 31; m_month = 8; break;
		case 10: m_day = 30; m_month = 9; break;
		case 11: m_day = 31; m_month = 10; break;
		case 12: m_day = 30; m_month = 11; break;

		default: m_day = 1; m_month = 1; m_year = 2000; m_day_of_the_week = 6; break;
		}
	}
	return *this;
}
welp::date welp::date::operator--(int) noexcept
{
	welp::date temp = *this;

	m_day_of_the_week--;
	if (m_day_of_the_week == 0) { m_day_of_the_week = 7; }

	m_day--;
	if (m_day <= 0)
	{
		switch (m_month)
		{
		case 1: m_day = 31; m_month = 12; m_year--; break;
		case 2: m_day = 31; m_month = 1; break;
		case 3: if (((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0)) { m_day = 29; }
			  else { m_day = 28; }
			  m_month = 2; break;
		case 4: m_day = 31; m_month = 3; break;
		case 5: m_day = 30; m_month = 4; break;
		case 6: m_day = 31; m_month = 5; break;
		case 7: m_day = 30; m_month = 6; break;
		case 8: m_day = 31; m_month = 7; break;
		case 9: m_day = 31; m_month = 8; break;
		case 10: m_day = 30; m_month = 9; break;
		case 11: m_day = 31; m_month = 10; break;
		case 12: m_day = 30; m_month = 11; break;

		default: m_day = 1; m_month = 1; m_year = 2000; m_day_of_the_week = 6; break;
		}
	}
	return temp;
}
welp::date& welp::date::operator-=(int n) noexcept
{
	if (n > 0)
	{
		for (; n > 0; n--) { --(*this); }
	}
	else if (n < 0)
	{
		for (; n < 0; n++) { ++(*this); }
	}
	return *this;
}

inline bool welp::date::operator==(const welp::date& rhs) const noexcept
{
	return ((m_day == rhs.m_day) && (m_month == rhs.m_month) && (m_year == rhs.m_year));
}
inline bool welp::date::operator!=(const welp::date& rhs) const noexcept
{
	return ((m_day != rhs.m_day) || (m_month != rhs.m_month) || (m_year != rhs.m_year));
}
inline bool welp::date::operator<(const welp::date& rhs) const noexcept
{
	if (m_year < rhs.m_year) { return true; }
	else if (m_year > rhs.m_year) { return false; }
	else
	{
		if (m_month < rhs.m_month) { return true; }
		else if (m_month > rhs.m_month) { return false; }
		else { return (m_day < rhs.m_day); }
	}
}
inline bool welp::date::operator>(const welp::date& rhs) const noexcept
{
	if (m_year > rhs.m_year) { return true; }
	else if (m_year < rhs.m_year) { return false; }
	else
	{
		if (m_month > rhs.m_month) { return true; }
		else if (m_month < rhs.m_month) { return false; }
		else { return (m_day > rhs.m_day); }
	}
}
inline bool welp::date::operator<=(const welp::date& rhs) const noexcept
{
	if (m_year < rhs.m_year) { return true; }
	else if (m_year > rhs.m_year) { return false; }
	else
	{
		if (m_month < rhs.m_month) { return true; }
		else if (m_month > rhs.m_month) { return false; }
		else { return (m_day <= rhs.m_day); }
	}
}
inline bool welp::date::operator>=(const welp::date& rhs) const noexcept
{
	if (m_year > rhs.m_year) { return true; }
	else if (m_year < rhs.m_year) { return false; }
	else
	{
		if (m_month > rhs.m_month) { return true; }
		else if (m_month < rhs.m_month) { return false; }
		else { return (m_day >= rhs.m_day); }
	}
}


////// IMPLEMENTATION TIME //////

inline int welp::time::hour() const noexcept { return m_hour; }
inline int welp::time::minute() const noexcept { return m_minute; }
inline int welp::time::second() const noexcept { return m_second; }

#ifdef WELP_TIMER_INCLUDE_STRING
std::string welp::time::get_time_hms(char delim) const
{
	if (m_hour < 10)
	{
		std::string string_time = "0" + std::to_string(m_hour);
		string_time += delim;
		if (m_minute < 10) { string_time += '0'; }
		string_time += std::to_string(m_minute);
		string_time += delim;
		if (m_second < 10) { string_time += '0'; }
		string_time += std::to_string(m_second);
		return string_time;
	}
	else
	{
		std::string string_time = std::to_string(m_hour);
		string_time += delim;
		if (m_minute < 10) { string_time += '0'; }
		string_time += std::to_string(m_minute);
		string_time += delim;
		if (m_second < 10) { string_time += '0'; }
		string_time += std::to_string(m_second);
		return string_time;
	}
}
std::string welp::time::get_time_hm(char delim) const
{
	if (m_hour < 10)
	{
		std::string string_time = "0" + std::to_string(m_hour);
		string_time += delim;
		if (m_minute < 10) { string_time += '0'; }
		string_time += std::to_string(m_minute);
		return string_time;
	}
	else
	{
		std::string string_time = std::to_string(m_hour);
		string_time += delim;
		if (m_minute < 10) { string_time += '0'; }
		string_time += std::to_string(m_minute);
		return string_time;
	}
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::time& welp::time::set_time(int _hour, int _minute, int _second) noexcept
{
	m_hour = _hour;
	m_minute = _minute;
	m_second = _second;

	m_seconds_from_midnight = 3600 * m_hour + 60 * m_minute + m_second;
	return *this;
}
#ifdef WELP_TIMER_INCLUDE_STRING
welp::time& welp::time::set_time_hms(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	m_hour = std::stoi(str.substr(0, delim1));
	m_minute = std::stoi(str.substr(delim1 + 1, delim2 - delim1 - 1));
	m_second = std::stoi(str.substr(delim2 + 1, str.size() - delim2));

	m_seconds_from_midnight = 3600 * m_hour + 60 * m_minute + m_second;
	return *this;
}
welp::time& welp::time::set_time_hm(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	m_hour = std::stoi(str.substr(0, delim1));
	m_minute = std::stoi(str.substr(delim1 + 1, str.size() - delim1));
	m_second = 0;

	m_seconds_from_midnight = 3600 * m_hour + 60 * m_minute;
	return *this;
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::time& welp::time::set_stepsize(int hours_forward, int minutes_forward, int seconds_forward) noexcept
{
	m_step_size = 3600 * hours_forward + 60 * minutes_forward + seconds_forward;
	return *this;
}

welp::time& welp::time::tie_to_date(const welp::date* date_obj_ptr) noexcept
{
	m_date_ptr = const_cast<welp::date*>(date_obj_ptr); return*this;
}
welp::time& welp::time::tie_to_date() noexcept
{
	m_date_ptr = nullptr; return *this;
}

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
welp::time& welp::time::set_time_now_local()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::localtime(&system_clock_time);

	m_hour = time_data->tm_hour;
	m_minute = time_data->tm_min;
	m_second = time_data->tm_sec;

	m_seconds_from_midnight = 3600 * m_hour + 60 * m_minute + m_second;

	if (m_date_ptr != nullptr)
	{
		m_date_ptr->set_date_now_local();
	}
	return *this;
}
welp::time& welp::time::set_time_now_gm()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::gmtime(&system_clock_time);

	m_hour = time_data->tm_hour;
	m_minute = time_data->tm_min;
	m_second = time_data->tm_sec;

	m_seconds_from_midnight = 3600 * m_hour + 60 * m_minute + m_second;

	if (m_date_ptr != nullptr)
	{
		m_date_ptr->set_date_now_gm();
	}
	return *this;
}
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

inline welp::time& welp::time::forward_hms(int hours_forward, int minutes_forward, int seconds_forward) noexcept
{
	int days_of_overflow;
	m_seconds_from_midnight += 3600 * hours_forward + 60 * minutes_forward + seconds_forward;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return *this;
}
inline welp::time& welp::time::operator++() noexcept
{
	int days_of_overflow;
	m_seconds_from_midnight += m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return *this;
}
inline welp::time welp::time::operator++(int) noexcept
{
	welp::time temp = *this;

	int days_of_overflow;
	m_seconds_from_midnight += m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return temp;
}
inline welp::time& welp::time::operator+=(int n) noexcept
{
	int days_of_overflow;
	m_seconds_from_midnight += n * m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return *this;
}
inline welp::time& welp::time::operator--() noexcept
{
	int days_of_overflow;
	m_seconds_from_midnight -= m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return *this;
}
inline welp::time welp::time::operator--(int) noexcept
{
	welp::time temp = *this;

	int days_of_overflow;
	m_seconds_from_midnight -= m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return temp;
}
inline welp::time& welp::time::operator-=(int n) noexcept
{
	int days_of_overflow;
	m_seconds_from_midnight -= n * m_step_size;
	if (0 <= m_seconds_from_midnight && m_seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (m_seconds_from_midnight < 0)
	{
		days_of_overflow = (m_seconds_from_midnight / 86400) - 1;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = m_seconds_from_midnight / 86400;
		m_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	m_hour = m_seconds_from_midnight / 3600;
	m_minute = (m_seconds_from_midnight - 3600 * m_hour) / 60;
	m_second = (m_seconds_from_midnight - 3600 * m_hour - 60 * m_minute);

	if ((days_of_overflow > 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*m_date_ptr); }
	}
	else if ((days_of_overflow < 0) && (m_date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*m_date_ptr); }
	}

	return *this;
}

inline bool welp::time::operator==(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight == rhs.m_seconds_from_midnight);
}
inline bool welp::time::operator!=(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight != rhs.m_seconds_from_midnight);
}
inline bool welp::time::operator<(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight < rhs.m_seconds_from_midnight);
}
inline bool welp::time::operator>(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight > rhs.m_seconds_from_midnight);
}
inline bool welp::time::operator<=(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight <= rhs.m_seconds_from_midnight);
}
inline bool welp::time::operator>=(const welp::time& rhs) const noexcept
{
	return (m_seconds_from_midnight >= rhs.m_seconds_from_midnight);
}

inline bool welp::time::operator==(int n) const noexcept
{
	return (m_seconds_from_midnight == n);
}
inline bool welp::time::operator!=(int n) const noexcept
{
	return (m_seconds_from_midnight != n);
}
inline bool welp::time::operator<(int n) const noexcept
{
	return (m_seconds_from_midnight < n);
}
inline bool welp::time::operator>(int n) const noexcept
{
	return (m_seconds_from_midnight > n);
}
inline bool welp::time::operator<=(int n) const noexcept
{
	return (m_seconds_from_midnight <= n);
}
inline bool welp::time::operator>=(int n) const noexcept
{
	return (m_seconds_from_midnight >= n);
}


#endif // WELP_DATE_TIME_INCLUDED_H


// welp_date_time.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
