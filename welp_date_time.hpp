// welp_date_time.hpp - last update : 20 / 03 / 2021
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef DGS_DATE_TIME_INCLUDED_H
#define DGS_DATE_TIME_INCLUDED_H


////// INCLUDES //////

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

		welp::date& set_date_numbers_dmy(int __day, int __month, int __year) noexcept;
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

		int _day = 1;
		int _month = 1;
		int _year = 2000;

		int _day_of_the_week = 6;
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

		welp::time& set_time(int __hour, int __minute, int __second) noexcept;
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

		int _hour = 0;
		int _minute = 0;
		int _second = 0;

		int _seconds_from_midnight = 0;
		int _step_size = 0;
		welp::date* date_ptr = nullptr;
	};
}


////// IMPLEMENTATION DATE //////

inline int welp::date::day() const noexcept { return _day; }
inline int welp::date::month() const noexcept { return _month; }
inline int welp::date::year() const noexcept { return _year; }

#ifdef WELP_TIMER_INCLUDE_STRING
std::string welp::date::get_date_dmy(char delim) const
{
	if (_day < 10)
	{
		std::string string_date = "0" + std::to_string(_day);
		string_date += delim;
		if (_month < 10) { string_date += '0'; }
		string_date += std::to_string(_month);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(_day);
		string_date += delim;
		if (_month < 10) { string_date += '0'; }
		string_date += std::to_string(_month);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
}
std::string welp::date::get_date_mdy(char delim) const
{
	if (_month < 10)
	{
		std::string string_date = "0" + std::to_string(_month);
		string_date += delim;
		if (_day < 10) { string_date += '0'; }
		string_date += std::to_string(_day);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(_month);
		string_date += delim;
		if (_day < 10) { string_date += '0'; }
		string_date += std::to_string(_day);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
}
std::string welp::date::get_date_ymd(char delim) const
{
	std::string string_date = std::to_string(_year) + delim;
	if (_month < 10) { string_date += '0'; }
	string_date += std::to_string(_month);
	string_date += delim;
	if (_day < 10) { string_date += '0'; }
	string_date += std::to_string(_day);
	return string_date;
}
template <class _month_function> std::string welp::date::get_date_text(char delim, _month_function month_func) const
{
	if (_day < 10)
	{
		std::string string_date = "0" + std::to_string(_day);
		string_date += delim;
		string_date += month_func(_month);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
	else
	{
		std::string string_date = std::to_string(_day);
		string_date += delim;
		string_date += month_func(_month);
		string_date += delim;
		string_date += std::to_string(_year);
		return string_date;
	}
}
#endif // WELP_TIMER_INCLUDE_STRING

inline int welp::date::get_day_of_the_week() const noexcept { return _day_of_the_week; }
#ifdef WELP_TIMER_INCLUDE_STRING
template <class _day_function> std::string  welp::date::get_day_of_the_week_text(_day_function day_func) const
{
	return day_func(_day_of_the_week);
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::date& welp::date::set_date_numbers_dmy(int __day, int __month, int __year) noexcept
{
	_day = __day;
	_month = __month;
	_year = __year;

	int m = _month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (_year - 1) / 100;
		K = (_year - 1) % 100;
	}
	else
	{
		J = _year / 100;
		K = _year % 100;
	}
	int q = _day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	_day_of_the_week = (h + 5) % 7 + 1;

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

	_day = std::stoi(string_day);
	_month = std::stoi(string_month);
	_year = std::stoi(string_year);

	int m = _month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (_year - 1) / 100;
		K = (_year - 1) % 100;
	}
	else
	{
		J = _year / 100;
		K = _year % 100;
	}
	int q = _day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
welp::date& welp::date::set_date_mdy(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_month = str.substr(0, delim1);
	std::string string_day = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_year = str.substr(delim2 + 1, str.size() - delim2);

	_day = std::stoi(string_day);
	_month = std::stoi(string_month);
	_year = std::stoi(string_year);

	int m = _month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (_year - 1) / 100;
		K = (_year - 1) % 100;
	}
	else
	{
		J = _year / 100;
		K = _year % 100;
	}
	int q = _day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
welp::date& welp::date::set_date_ymd(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_year = str.substr(0, delim1);
	std::string string_month = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_day = str.substr(delim2 + 1, str.size() - delim2);

	_day = std::stoi(string_day);
	_month = std::stoi(string_month);
	_year = std::stoi(string_year);

	int m = _month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (_year - 1) / 100;
		K = (_year - 1) % 100;
	}
	else
	{
		J = _year / 100;
		K = _year % 100;
	}
	int q = _day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
template <class _month_function> welp::date& welp::date::set_date_text(const std::string& str, char delim, _month_function month_func)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	std::string string_day = str.substr(0, delim1);
	std::string string_month = str.substr(delim1 + 1, delim2 - delim1 - 1);
	std::string string_year = str.substr(delim2 + 1, str.size() - delim2);

	_day = std::stoi(string_day);
	_month = month_func(string_month);
	_year = std::stoi(string_year);

	int m = _month;
	int J;
	int K;
	if (m < 3)
	{
		m += 12;
		J = (_year - 1) / 100;
		K = (_year - 1) % 100;
	}
	else
	{
		J = _year / 100;
		K = _year % 100;
	}
	int q = _day;

	int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J);

	_day_of_the_week = (h + 5) % 7 + 1;

	return *this;
}
#endif // WELP_TIMER_INCLUDE_STRING

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
welp::date& welp::date::set_date_now_local()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::localtime(&system_clock_time);

	_day = time_data->tm_mday;
	_month = time_data->tm_mon + 1;
	_year = time_data->tm_year + 1900;

	_day_of_the_week = time_data->tm_wday;
	if (_day_of_the_week == 0) { _day_of_the_week = 7; }

	return *this;
}
welp::date& welp::date::set_date_now_gm()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::gmtime(&system_clock_time);

	_day = time_data->tm_mday;
	_month = time_data->tm_mon + 1;
	_year = time_data->tm_year + 1900;

	_day_of_the_week = time_data->tm_wday;
	if (_day_of_the_week == 0) { _day_of_the_week = 7; }

	return *this;
}
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

welp::date& welp::date::operator++() noexcept
{
	_day_of_the_week++;
	if (_day_of_the_week == 8) { _day_of_the_week = 1; }

	_day++;
	if (_day > 28)
	{
		switch (_month)
		{
		case 1: if (_day > 31) { _day = 1; _month = 2; } break;
		case 2: if ((_day == 29) && (((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0))) { break; }
			  else { _day = 1; _month = 3; break; }
		case 3: if (_day > 31) { _day = 1; _month = 4; } break;
		case 4: if (_day > 30) { _day = 1; _month = 5; } break;
		case 5: if (_day > 31) { _day = 1; _month = 6; } break;
		case 6: if (_day > 30) { _day = 1; _month = 7; } break;
		case 7: if (_day > 31) { _day = 1; _month = 8; } break;
		case 8: if (_day > 31) { _day = 1; _month = 9; } break;
		case 9: if (_day > 30) { _day = 1; _month = 10; } break;
		case 10: if (_day > 31) { _day = 1; _month = 11; } break;
		case 11: if (_day > 30) { _day = 1; _month = 12; } break;
		case 12: if (_day > 31) { _day = 1; _month = 1; _year++; } break;

		default: _day = 1; _month = 1; _year = 2000; _day_of_the_week = 6; break;
		}
	}
	return *this;
}
welp::date welp::date::operator++(int) noexcept
{
	welp::date temp = *this;

	_day_of_the_week++;
	if (_day_of_the_week == 8) { _day_of_the_week = 1; }

	_day++;
	if (_day > 28)
	{
		switch (_month)
		{
		case 1: if (_day > 31) { _day = 1; _month = 2; } break;
		case 2: if ((_day == 29) && (((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0))) { break; }
			  else { _day = 1; _month = 3; break; }
		case 3: if (_day > 31) { _day = 1; _month = 4; } break;
		case 4: if (_day > 30) { _day = 1; _month = 5; } break;
		case 5: if (_day > 31) { _day = 1; _month = 6; } break;
		case 6: if (_day > 30) { _day = 1; _month = 7; } break;
		case 7: if (_day > 31) { _day = 1; _month = 8; } break;
		case 8: if (_day > 31) { _day = 1; _month = 9; } break;
		case 9: if (_day > 30) { _day = 1; _month = 10; } break;
		case 10: if (_day > 31) { _day = 1; _month = 11; } break;
		case 11: if (_day > 30) { _day = 1; _month = 12; } break;
		case 12: if (_day > 31) { _day = 1; _month = 1; _year++; } break;

		default: _day = 1; _month = 1; _year = 2000; _day_of_the_week = 6; break;
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
	_day_of_the_week--;
	if (_day_of_the_week == 0) { _day_of_the_week = 7; }

	_day--;
	if (_day <= 0)
	{
		switch (_month)
		{
		case 1: _day = 31; _month = 12; _year--; break;
		case 2: _day = 31; _month = 1; break;
		case 3: if (((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0)) { _day = 29; }
			  else { _day = 28; }
			  _month = 2; break;
		case 4: _day = 31; _month = 3; break;
		case 5: _day = 30; _month = 4; break;
		case 6: _day = 31; _month = 5; break;
		case 7: _day = 30; _month = 6; break;
		case 8: _day = 31; _month = 7; break;
		case 9: _day = 31; _month = 8; break;
		case 10: _day = 30; _month = 9; break;
		case 11: _day = 31; _month = 10; break;
		case 12: _day = 30; _month = 11; break;

		default: _day = 1; _month = 1; _year = 2000; _day_of_the_week = 6; break;
		}
	}
	return *this;
}
welp::date welp::date::operator--(int) noexcept
{
	welp::date temp = *this;

	_day_of_the_week--;
	if (_day_of_the_week == 0) { _day_of_the_week = 7; }

	_day--;
	if (_day <= 0)
	{
		switch (_month)
		{
		case 1: _day = 31; _month = 12; _year--; break;
		case 2: _day = 31; _month = 1; break;
		case 3: if (((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0)) { _day = 29; }
			  else { _day = 28; }
			  _month = 2; break;
		case 4: _day = 31; _month = 3; break;
		case 5: _day = 30; _month = 4; break;
		case 6: _day = 31; _month = 5; break;
		case 7: _day = 30; _month = 6; break;
		case 8: _day = 31; _month = 7; break;
		case 9: _day = 31; _month = 8; break;
		case 10: _day = 30; _month = 9; break;
		case 11: _day = 31; _month = 10; break;
		case 12: _day = 30; _month = 11; break;

		default: _day = 1; _month = 1; _year = 2000; _day_of_the_week = 6; break;
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
	return ((_day == rhs._day) && (_month == rhs._month) && (_year == rhs._year));
}
inline bool welp::date::operator!=(const welp::date& rhs) const noexcept
{
	return ((_day != rhs._day) || (_month != rhs._month) || (_year != rhs._year));
}
inline bool welp::date::operator<(const welp::date& rhs) const noexcept
{
	if (_year < rhs._year) { return true; }
	else if (_year > rhs._year) { return false; }
	else
	{
		if (_month < rhs._month) { return true; }
		else if (_month > rhs._month) { return false; }
		else { return (_day < rhs._day); }
	}
}
inline bool welp::date::operator>(const welp::date& rhs) const noexcept
{
	if (_year > rhs._year) { return true; }
	else if (_year < rhs._year) { return false; }
	else
	{
		if (_month > rhs._month) { return true; }
		else if (_month < rhs._month) { return false; }
		else { return (_day > rhs._day); }
	}
}
inline bool welp::date::operator<=(const welp::date& rhs) const noexcept
{
	if (_year < rhs._year) { return true; }
	else if (_year > rhs._year) { return false; }
	else
	{
		if (_month < rhs._month) { return true; }
		else if (_month > rhs._month) { return false; }
		else { return (_day <= rhs._day); }
	}
}
inline bool welp::date::operator>=(const welp::date& rhs) const noexcept
{
	if (_year > rhs._year) { return true; }
	else if (_year < rhs._year) { return false; }
	else
	{
		if (_month > rhs._month) { return true; }
		else if (_month < rhs._month) { return false; }
		else { return (_day >= rhs._day); }
	}
}


////// IMPLEMENTATION TIME //////

inline int welp::time::hour() const noexcept { return _hour; }
inline int welp::time::minute() const noexcept { return _minute; }
inline int welp::time::second() const noexcept { return _second; }

#ifdef WELP_TIMER_INCLUDE_STRING
std::string welp::time::get_time_hms(char delim) const
{
	if (_hour < 10)
	{
		std::string string_time = "0" + std::to_string(_hour);
		string_time += delim;
		if (_minute < 10) { string_time += '0'; }
		string_time += std::to_string(_minute);
		string_time += delim;
		if (_second < 10) { string_time += '0'; }
		string_time += std::to_string(_second);
		return string_time;
	}
	else
	{
		std::string string_time = std::to_string(_hour);
		string_time += delim;
		if (_minute < 10) { string_time += '0'; }
		string_time += std::to_string(_minute);
		string_time += delim;
		if (_second < 10) { string_time += '0'; }
		string_time += std::to_string(_second);
		return string_time;
	}
}
std::string welp::time::get_time_hm(char delim) const
{
	if (_hour < 10)
	{
		std::string string_time = "0" + std::to_string(_hour);
		string_time += delim;
		if (_minute < 10) { string_time += '0'; }
		string_time += std::to_string(_minute);
		return string_time;
	}
	else
	{
		std::string string_time = std::to_string(_hour);
		string_time += delim;
		if (_minute < 10) { string_time += '0'; }
		string_time += std::to_string(_minute);
		return string_time;
	}
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::time& welp::time::set_time(int __hour, int __minute, int __second) noexcept
{
	_hour = __hour;
	_minute = __minute;
	_second = __second;

	_seconds_from_midnight = 3600 * _hour + 60 * _minute + _second;
	return *this;
}
#ifdef WELP_TIMER_INCLUDE_STRING
welp::time& welp::time::set_time_hms(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	std::size_t delim2 = str.find_last_of(delim);
	_hour = std::stoi(str.substr(0, delim1));
	_minute = std::stoi(str.substr(delim1 + 1, delim2 - delim1 - 1));
	_second = std::stoi(str.substr(delim2 + 1, str.size() - delim2));

	_seconds_from_midnight = 3600 * _hour + 60 * _minute + _second;
	return *this;
}
welp::time& welp::time::set_time_hm(const std::string& str, char delim)
{
	std::size_t delim1 = str.find_first_of(delim);
	_hour = std::stoi(str.substr(0, delim1));
	_minute = std::stoi(str.substr(delim1 + 1, str.size() - delim1));
	_second = 0;

	_seconds_from_midnight = 3600 * _hour + 60 * _minute;
	return *this;
}
#endif // WELP_TIMER_INCLUDE_STRING

welp::time& welp::time::set_stepsize(int hours_forward, int minutes_forward, int seconds_forward) noexcept
{
	_step_size = 3600 * hours_forward + 60 * minutes_forward + seconds_forward;
	return *this;
}

welp::time& welp::time::tie_to_date(const welp::date* date_obj_ptr) noexcept
{
	date_ptr = const_cast<welp::date*>(date_obj_ptr); return*this;
}
welp::time& welp::time::tie_to_date() noexcept
{
	date_ptr = nullptr; return *this;
}

#if defined(WELP_TIMER_INCLUDE_CTIME) && defined(WELP_TIMER_INCLUDE_CHRONO)
welp::time& welp::time::set_time_now_local()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::localtime(&system_clock_time);

	_hour = time_data->tm_hour;
	_minute = time_data->tm_min;
	_second = time_data->tm_sec;

	_seconds_from_midnight = 3600 * _hour + 60 * _minute + _second;

	if (date_ptr != nullptr)
	{
		date_ptr->set_date_now_local();
	}
	return *this;
}
welp::time& welp::time::set_time_now_gm()
{
	std::time_t system_clock_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm* time_data = std::gmtime(&system_clock_time);

	_hour = time_data->tm_hour;
	_minute = time_data->tm_min;
	_second = time_data->tm_sec;

	_seconds_from_midnight = 3600 * _hour + 60 * _minute + _second;

	if (date_ptr != nullptr)
	{
		date_ptr->set_date_now_gm();
	}
	return *this;
}
#endif // WELP_TIMER_INCLUDE_CTIME && WELP_TIMER_INCLUDE_CHRONO

inline welp::time& welp::time::forward_hms(int hours_forward, int minutes_forward, int seconds_forward) noexcept
{
	int days_of_overflow;
	_seconds_from_midnight += 3600 * hours_forward + 60 * minutes_forward + seconds_forward;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return *this;
}
inline welp::time& welp::time::operator++() noexcept
{
	int days_of_overflow;
	_seconds_from_midnight += _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return *this;
}
inline welp::time welp::time::operator++(int) noexcept
{
	welp::time temp = *this;

	int days_of_overflow;
	_seconds_from_midnight += _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return temp;
}
inline welp::time& welp::time::operator+=(int n) noexcept
{
	int days_of_overflow;
	_seconds_from_midnight += n * _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return *this;
}
inline welp::time& welp::time::operator--() noexcept
{
	int days_of_overflow;
	_seconds_from_midnight -= _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return *this;
}
inline welp::time welp::time::operator--(int) noexcept
{
	welp::time temp = *this;

	int days_of_overflow;
	_seconds_from_midnight -= _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return temp;
}
inline welp::time& welp::time::operator-=(int n) noexcept
{
	int days_of_overflow;
	_seconds_from_midnight -= n * _step_size;
	if (0 <= _seconds_from_midnight && _seconds_from_midnight < 86400) { days_of_overflow = 0; }
	else if (_seconds_from_midnight < 0)
	{
		days_of_overflow = (_seconds_from_midnight / 86400) - 1;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	else
	{
		days_of_overflow = _seconds_from_midnight / 86400;
		_seconds_from_midnight -= 86400 * days_of_overflow;
	}
	_hour = _seconds_from_midnight / 3600;
	_minute = (_seconds_from_midnight - 3600 * _hour) / 60;
	_second = (_seconds_from_midnight - 3600 * _hour - 60 * _minute);

	if ((days_of_overflow > 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow > 0; days_of_overflow--) { ++(*date_ptr); }
	}
	else if ((days_of_overflow < 0) && (date_ptr != nullptr))
	{
		for (; days_of_overflow < 0; days_of_overflow++) { --(*date_ptr); }
	}

	return *this;
}

inline bool welp::time::operator==(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight == rhs._seconds_from_midnight);
}
inline bool welp::time::operator!=(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight != rhs._seconds_from_midnight);
}
inline bool welp::time::operator<(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight < rhs._seconds_from_midnight);
}
inline bool welp::time::operator>(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight > rhs._seconds_from_midnight);
}
inline bool welp::time::operator<=(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight <= rhs._seconds_from_midnight);
}
inline bool welp::time::operator>=(const welp::time& rhs) const noexcept
{
	return (_seconds_from_midnight >= rhs._seconds_from_midnight);
}

inline bool welp::time::operator==(int n) const noexcept
{
	return (_seconds_from_midnight == n);
}
inline bool welp::time::operator!=(int n) const noexcept
{
	return (_seconds_from_midnight != n);
}
inline bool welp::time::operator<(int n) const noexcept
{
	return (_seconds_from_midnight < n);
}
inline bool welp::time::operator>(int n) const noexcept
{
	return (_seconds_from_midnight > n);
}
inline bool welp::time::operator<=(int n) const noexcept
{
	return (_seconds_from_midnight <= n);
}
inline bool welp::time::operator>=(int n) const noexcept
{
	return (_seconds_from_midnight >= n);
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
