// welp_bit_flags.h - last update : 16 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_BIT_FLAGS_H
#define WELP_BIT_FLAGS_H


////// INCLUDES //////

#include <cstdlib>
#include <cstdint>
#include <cstring>

#ifdef WELP_BIT_FLAGS_DEBUG_MODE
#include <cassert>
#endif // WELP_BIT_FLAGS_DEBUG_MODE

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM


////// DESCRIPTIONS //////

namespace welp
{
	template <std::size_t bits> class bit_flags
	{

	public:

		inline bool load(std::size_t offset) const noexcept;
		inline welp::bit_flags<bits>& store(std::size_t offset, bool flag) noexcept;

		inline welp::bit_flags<bits>& set(bool flag) noexcept;
		template <class Ty> inline welp::bit_flags<bits>& cpy(const Ty& rhs) noexcept;

		inline welp::bit_flags<bits>& operator&=(const welp::bit_flags<bits>& rhs) noexcept;
		inline welp::bit_flags<bits>& operator|=(const welp::bit_flags<bits>& rhs) noexcept;
		inline welp::bit_flags<bits>& operator^=(const welp::bit_flags<bits>& rhs) noexcept;
		inline welp::bit_flags<bits>& flip() noexcept;
		
		constexpr std::size_t size() const noexcept { return bits; }

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
		const welp::bit_flags<bits>& say() const;
		welp::bit_flags<bits>& say();
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM

		bit_flags() noexcept;
		bit_flags(const welp::bit_flags<bits>&) noexcept = default;
		welp::bit_flags<bits>& operator=(const welp::bit_flags<bits>&) noexcept;
		bit_flags(welp::bit_flags<bits>&&) noexcept = default;
		welp::bit_flags<bits>& operator=(welp::bit_flags<bits>&&) noexcept;
		~bit_flags() = default;

		template <std::size_t bits2>
		friend inline bool operator==(const welp::bit_flags<bits2>&, const welp::bit_flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline bool operator!=(const welp::bit_flags<bits2>&, const welp::bit_flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bit_flags<bits2> operator&(const welp::bit_flags<bits2>&, const welp::bit_flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bit_flags<bits2> operator|(const welp::bit_flags<bits2>&, const welp::bit_flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bit_flags<bits2> operator^(const welp::bit_flags<bits2>&, const welp::bit_flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bit_flags<bits2> operator~(const welp::bit_flags<bits2>&) noexcept;

	private:

		std::uint8_t field[(bits + ((8 - (bits & 7)) & 7)) >> 3];

		inline std::uint8_t shift_true(std::size_t offset) const noexcept;
		inline std::uint8_t shift_false(std::size_t offset) const noexcept;
		inline std::uint8_t bitmask_true(std::size_t digits) const noexcept;
		inline std::uint8_t bitmask_false(std::size_t digits) const noexcept;
	};

	template <std::size_t bits> inline bool operator==(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline bool operator!=(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;

	template <std::size_t bits> inline welp::bit_flags<bits> operator&(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::bit_flags<bits> operator|(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::bit_flags<bits> operator^(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;

	template <std::size_t bits, class Ty> inline const welp::bit_flags<bits>& as_bit_flags(const Ty& rhs) noexcept;
	template <std::size_t bits, class Ty> inline welp::bit_flags<bits>& as_bit_flags(Ty& rhs) noexcept;
}


////// IMPLEMENTATIONS //////

template <std::size_t bits>
inline bool welp::bit_flags<bits>::load(std::size_t offset) const noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(offset < bits);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	return (field[offset >> 3] & shift_true(offset & 7)) != static_cast<std::uint8_t>(0);
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::store(std::size_t offset, bool flag) noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(offset < bits);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	if (flag)
	{
		field[offset >> 3] |= shift_true(offset & 7);
		return *this;
	}
	else
	{
		field[offset >> 3] &= shift_false(offset & 7);
		return *this;
	}
}


template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::set(bool flag) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	if (flag)
	{
		std::memset(static_cast<std::uint8_t*>(field), -static_cast<std::uint8_t>(1), bytes);
		*(static_cast<std::uint8_t*>(field) + bytes) |= bitmask_true(remainder_bits);
		return *this;
	}
	else
	{
		std::memset(static_cast<std::uint8_t*>(field), static_cast<std::uint8_t>(0), bytes);
		*(static_cast<std::uint8_t*>(field) + bytes) &= bitmask_false(remainder_bits);
		return *this;
	}
}

template <std::size_t bits> template <class Ty>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::cpy(const Ty& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	const std::uint8_t* rhs_ptr = static_cast<const std::uint8_t*>(static_cast<const void*>(&rhs));
	std::memcpy(static_cast<std::uint8_t*>(field), rhs_ptr, bytes);
	field[bytes] |= (*(rhs_ptr + bytes) & bitmask_true(remainder_bits));
	return *this;
}


template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::operator&=(const welp::bit_flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] &= rhs.field[k];
	}
	field[bytes] &= (rhs.field[bytes] | bitmask_false(remainder_bits));
	return *this;
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::operator|=(const welp::bit_flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] |= rhs.field[k];
	}
	field[bytes] |= (rhs.field[bytes] & bitmask_true(remainder_bits));
	return *this;
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::operator^=(const welp::bit_flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] ^= rhs.field[k];
	}
	field[bytes] ^= (rhs.field[bytes] & bitmask_true(remainder_bits));
	return *this;
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::flip() noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	constexpr std::uint8_t ones = -static_cast<std::uint8_t>(1);
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] ^= ones;
	}
	field[bytes] ^= bitmask_true(remainder_bits);
	return *this;
}


#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bits>
const welp::bit_flags<bits>& welp::bit_flags<bits>::say() const
{
	std::cout << ">>>  bit 0 : " << load(0) << "\n";
	for (std::size_t k = 1; k < bits; k++)
	{
		std::cout << "     bit " << k << " : " << load(k) << "\n";
	}
	std::cout << std::endl;
	return *this;
}

template <std::size_t bits>
welp::bit_flags<bits>& welp::bit_flags<bits>::say()
{
	std::cout << ">>>  bit 0 : " << load(0) << "\n";
	for (std::size_t k = 1; k < bits; k++)
	{
		std::cout << "     bit " << k << " : " << load(k) << "\n";
	}
	std::cout << std::endl;
	return *this;
}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM


template <std::size_t bits>
welp::bit_flags<bits>::bit_flags() noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memset(static_cast<std::uint8_t*>(field), static_cast<std::uint8_t>(0), bytes);
	field[bytes] &= bitmask_false(remainder_bits);
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::operator=(const welp::bit_flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memcpy(static_cast<std::uint8_t*>(field), static_cast<const std::uint8_t*>(rhs.field), bytes);
	field[bytes] &= bitmask_false(remainder_bits);
	field[bytes] |= (rhs.field[bytes] & bitmask_true(remainder_bits));
	return *this;
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::operator=(welp::bit_flags<bits>&& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memcpy(static_cast<std::uint8_t*>(field), static_cast<const std::uint8_t*>(rhs.field), bytes);
	field[bytes] &= bitmask_false(remainder_bits);
	field[bytes] |= (rhs.field[bytes] & bitmask_true(remainder_bits));
	return *this;
}


template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::shift_true(std::size_t offset) const noexcept
{
	switch (offset)
	{

	case 0:
		return static_cast<std::uint8_t>(1);
		break;

	case 1:
		return static_cast<std::uint8_t>(2);
		break;

	case 2:
		return static_cast<std::uint8_t>(4);
		break;

	case 3:
		return static_cast<std::uint8_t>(8);
		break;

	case 4:
		return static_cast<std::uint8_t>(16);
		break;

	case 5:
		return static_cast<std::uint8_t>(32);
		break;

	case 6:
		return static_cast<std::uint8_t>(64);
		break;

	case 7:
		return static_cast<std::uint8_t>(128);
		break;

	default:
		return static_cast<std::uint8_t>(0);
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::shift_false(std::size_t offset) const noexcept
{
	switch (offset)
	{

	case 0:
		return static_cast<std::uint8_t>(254);
		break;

	case 1:
		return static_cast<std::uint8_t>(253);
		break;

	case 2:
		return static_cast<std::uint8_t>(251);
		break;

	case 3:
		return static_cast<std::uint8_t>(247);
		break;

	case 4:
		return static_cast<std::uint8_t>(239);
		break;

	case 5:
		return static_cast<std::uint8_t>(223);
		break;

	case 6:
		return static_cast<std::uint8_t>(191);
		break;

	case 7:
		return static_cast<std::uint8_t>(127);
		break;

	default:
		return static_cast<std::uint8_t>(255);
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::bitmask_true(std::size_t digits) const noexcept
{
	switch (digits)
	{

	case 0:
		return static_cast<std::uint8_t>(0);
		break;

	case 1:
		return static_cast<std::uint8_t>(1);
		break;

	case 2:
		return static_cast<std::uint8_t>(3);
		break;

	case 3:
		return static_cast<std::uint8_t>(7);
		break;

	case 4:
		return static_cast<std::uint8_t>(15);
		break;

	case 5:
		return static_cast<std::uint8_t>(31);
		break;

	case 6:
		return static_cast<std::uint8_t>(63);
		break;

	case 7:
		return static_cast<std::uint8_t>(127);
		break;

	default:
		return static_cast<std::uint8_t>(255);
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::bitmask_false(std::size_t digits) const noexcept
{
	switch (digits)
	{

	case 0:
		return static_cast<std::uint8_t>(255);
		break;

	case 1:
		return static_cast<std::uint8_t>(254);
		break;

	case 2:
		return static_cast<std::uint8_t>(252);
		break;

	case 3:
		return static_cast<std::uint8_t>(248);
		break;

	case 4:
		return static_cast<std::uint8_t>(240);
		break;

	case 5:
		return static_cast<std::uint8_t>(224);
		break;

	case 6:
		return static_cast<std::uint8_t>(192);
		break;

	case 7:
		return static_cast<std::uint8_t>(128);
		break;

	default:
		return static_cast<std::uint8_t>(0);
	}
}


namespace welp
{
	template <std::size_t bits> inline bool operator==(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = bits >> 3;
		constexpr std::size_t remainder_bits = bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return false; }
		}

		std::uint8_t bitmask = A.bitmask_true(remainder_bits);
		if ((A.field[bytes] & bitmask) != (B.field[bytes] & bitmask)) { return false; }
		else { return true; }
	}
	template <std::size_t bits> inline bool operator!=(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = bits >> 3;
		constexpr std::size_t remainder_bits = bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return true; }
		}

		std::uint8_t bitmask = A.bitmask_true(remainder_bits);
		if ((A.field[bytes] & bitmask) != (B.field[bytes] & bitmask)) { return true; }
		else { return true; }
	}

	template <std::size_t bits> inline welp::bit_flags<bits> operator&(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::bit_flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] & B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::bit_flags<bits> operator|(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::bit_flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] | B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::bit_flags<bits> operator^(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::bit_flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] ^ B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::bit_flags<bits> operator~(const welp::bit_flags<bits>& A) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::bit_flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = ~A.field[k];
		}
		return C;
	}

	template <std::size_t bits, class Ty> inline const welp::bit_flags<bits>& as_bit_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::bit_flags<bits>&>(rhs);
	}
	template <std::size_t bits, class Ty> inline welp::bit_flags<bits>& as_bit_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::bit_flags<bits>&>(rhs);
	}
}

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bits> std::ostream& operator<<(std::ostream& out, const welp::bit_flags<bits>& A)
{
	A.say(); return out;
}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM


#endif // WELP_BIT_FLAGS_H


// welp_bit_flags.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
