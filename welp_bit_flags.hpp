// welp_bit_flags.h - last update : 25 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_BIT_FLAGS_H
#define WELP_BIT_FLAGS_H


////// INCLUDES //////

#include <cstdlib>
#include <cstdint>
#include <cstring>


#ifdef WELP_BIT_FLAGS_INCLUDE_ALL // include all in one line with #define WELP_BIT_FLAGS_INCLUDE_ALL
#ifndef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#define WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#endif
#endif // WELP_BIT_FLAGS_INCLUDE_ALL


#ifdef WELP_BIT_FLAGS_DEBUG_MODE
#include <cassert>
#ifndef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#define WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#endif
#endif // WELP_BIT_FLAGS_DEBUG_MODE

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM


////// DESCRIPTIONS //////

namespace welp
{
	// bits
	
	template <std::size_t bits> class bit_flags
	{

	public:

		inline bool load(std::size_t bit_offset) const noexcept;
		inline welp::bit_flags<bits>& store(std::size_t bit_offset, bool flag) noexcept;

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

		inline std::uint8_t shift_true(std::size_t bit_offset) const noexcept;
		inline std::uint8_t shift_false(std::size_t bit_offset) const noexcept;
		inline std::uint8_t bitmask_true(std::size_t digits) const noexcept;
		inline std::uint8_t bitmask_false(std::size_t digits) const noexcept;
	};

	template <std::size_t bits> inline bool operator==(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline bool operator!=(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;

	template <std::size_t bits> inline welp::bit_flags<bits> operator&(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::bit_flags<bits> operator|(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::bit_flags<bits> operator^(const welp::bit_flags<bits>& A, const welp::bit_flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::bit_flags<bits> operator~(const welp::bit_flags<bits>& A) noexcept;

	template <class Ty> inline const welp::bit_flags<8 * sizeof(Ty)>& as_bit_flags(const Ty& rhs) noexcept;
	template <class Ty> inline welp::bit_flags<8 * sizeof(Ty)>& as_bit_flags(Ty& rhs) noexcept;
	template <std::size_t bits, class Ty> inline const welp::bit_flags<bits>& as_bit_flags(const Ty& rhs) noexcept;
	template <std::size_t bits, class Ty> inline welp::bit_flags<bits>& as_bit_flags(Ty& rhs) noexcept;
	
	// bytes
	
	template <std::size_t bytes> class byte_flags
	{

	public:

		inline char load(std::size_t byte_offset, bool upper_half_byte) const noexcept;
		inline byte_flags<bytes>& store(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept;

		inline welp::byte_flags<bytes>& set(std::uint8_t number) noexcept;
		inline welp::byte_flags<bytes>& set(char lower_half_byte, char upper_half_byte) noexcept;
		template <class Ty> inline welp::byte_flags<bytes>& cpy(const Ty& rhs) noexcept;

		inline const std::uint8_t& operator[](std::size_t offset) const noexcept;
		inline std::uint8_t& operator[](std::size_t offset) noexcept;

		inline const std::uint8_t* data() const noexcept;
		inline std::uint8_t* data() noexcept;

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
		const welp::byte_flags<bytes>& say() const;
		welp::byte_flags<bytes>& say();
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM

		byte_flags() = default;
		byte_flags(const welp::byte_flags<bytes>&) noexcept = default;
		welp::byte_flags<bytes>& operator=(const welp::byte_flags<bytes>&) noexcept = default;
		byte_flags(welp::byte_flags<bytes>&&) noexcept = default;
		welp::byte_flags<bytes>& operator=(welp::byte_flags<bytes>&&) noexcept = default;
		~byte_flags() = default;

		template <std::size_t bytes2>
		friend inline bool operator==(const welp::byte_flags<bytes2>&, const welp::byte_flags<bytes2>&) noexcept;
		template <std::size_t bytes2>
		friend inline bool operator!=(const welp::byte_flags<bytes2>&, const welp::byte_flags<bytes2>&) noexcept;

	private:

		std::uint8_t field[bytes] = { static_cast<std::uint8_t>(0) };

		inline std::uint8_t char_to_uint8_t(char hex) const noexcept;
	};
	
	template <std::size_t bytes> inline bool operator==(const welp::byte_flags<bytes>& A, const welp::byte_flags<bytes>& B) noexcept;
	template <std::size_t bytes> inline bool operator!=(const welp::byte_flags<bytes>& A, const welp::byte_flags<bytes>& B) noexcept;

	template <class Ty> inline const welp::byte_flags<sizeof(Ty)>& as_byte_flags(const Ty& rhs) noexcept;
	template <class Ty> inline welp::byte_flags<sizeof(Ty)>& as_byte_flags(Ty& rhs) noexcept;
	template <std::size_t bytes, class Ty> inline const welp::byte_flags<bytes>& as_byte_flags(const Ty& rhs) noexcept;
	template <std::size_t bytes, class Ty> inline welp::byte_flags<bytes>& as_byte_flags(Ty& rhs) noexcept;
}


////// IMPLEMENTATIONS //////

// bits

template <std::size_t bits>
inline bool welp::bit_flags<bits>::load(std::size_t bit_offset) const noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	return (field[bit_offset >> 3] & shift_true(bit_offset & 7)) != static_cast<std::uint8_t>(0);
}

template <std::size_t bits>
inline welp::bit_flags<bits>& welp::bit_flags<bits>::store(std::size_t bit_offset, bool flag) noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	if (flag)
	{
		field[bit_offset >> 3] |= shift_true(bit_offset & 7);
		return *this;
	}
	else
	{
		field[bit_offset >> 3] &= shift_false(bit_offset & 7);
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
inline std::uint8_t welp::bit_flags<bits>::shift_true(std::size_t bit_offset) const noexcept
{
	switch (bit_offset)
	{
	case 0: return static_cast<std::uint8_t>(1); break;
	case 1: return static_cast<std::uint8_t>(2); break;
	case 2: return static_cast<std::uint8_t>(4); break;
	case 3: return static_cast<std::uint8_t>(8); break;
	case 4: return static_cast<std::uint8_t>(16); break;
	case 5: return static_cast<std::uint8_t>(32); break;
	case 6: return static_cast<std::uint8_t>(64); break;
	case 7: return static_cast<std::uint8_t>(128); break;
	default: return static_cast<std::uint8_t>(0); break;
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::shift_false(std::size_t bit_offset) const noexcept
{
	switch (bit_offset)
	{		
	case 0: return static_cast<std::uint8_t>(254); break;
	case 1: return static_cast<std::uint8_t>(253); break;
	case 2: return static_cast<std::uint8_t>(251); break;
	case 3: return static_cast<std::uint8_t>(247); break;
	case 4: return static_cast<std::uint8_t>(239); break;
	case 5: return static_cast<std::uint8_t>(223); break;
	case 6: return static_cast<std::uint8_t>(191); break;
	case 7: return static_cast<std::uint8_t>(127); break;
	default: return static_cast<std::uint8_t>(255); break;			
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::bitmask_true(std::size_t digits) const noexcept
{
	switch (digits)
	{
	case 0: return static_cast<std::uint8_t>(0); break;
	case 1: return static_cast<std::uint8_t>(1); break;
	case 2: return static_cast<std::uint8_t>(3); break;
	case 3: return static_cast<std::uint8_t>(7); break;
	case 4: return static_cast<std::uint8_t>(15); break;
	case 5: return static_cast<std::uint8_t>(31); break;
	case 6: return static_cast<std::uint8_t>(63); break;
	case 7: return static_cast<std::uint8_t>(127); break;
	default: return static_cast<std::uint8_t>(255); break;
	}
}

template <std::size_t bits>
inline std::uint8_t welp::bit_flags<bits>::bitmask_false(std::size_t digits) const noexcept
{
	switch (digits)
	{
	case 0: return static_cast<std::uint8_t>(255); break;
	case 1: return static_cast<std::uint8_t>(254); break;
	case 2: return static_cast<std::uint8_t>(252); break;
	case 3: return static_cast<std::uint8_t>(248); break;
	case 4: return static_cast<std::uint8_t>(240); break;
	case 5: return static_cast<std::uint8_t>(224); break;
	case 6: return static_cast<std::uint8_t>(192); break;
	case 7: return static_cast<std::uint8_t>(128); break;
	default: return static_cast<std::uint8_t>(0); break;
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

	template <class Ty> inline const welp::bit_flags<8 * sizeof(Ty)>& as_bit_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::bit_flags<8 * sizeof(Ty)>&>(rhs);
	}
	template <class Ty> inline welp::bit_flags<8 * sizeof(Ty)>& as_bit_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::bit_flags<8 * sizeof(Ty)>&>(rhs);
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

namespace welp
{
	template <> class bit_flags<0>
	{

	public:

		inline bool load(std::size_t bit_offset) const noexcept
		{
			return (*(static_cast<const std::uint8_t*>(field) + (bit_offset >> 3)) & shift_true(bit_offset & 7)) != static_cast<std::uint8_t>(0);
		}
		inline welp::bit_flags<0>& store(std::size_t bit_offset, bool flag) noexcept
		{
			if (flag)
			{
				*(static_cast<std::uint8_t*>(field) + (bit_offset >> 3)) |= shift_true(bit_offset & 7);
				return *this;
			}
			else
			{
				*(static_cast<std::uint8_t*>(field) + (bit_offset >> 3)) &= shift_false(bit_offset & 7);
				return *this;
			}
		}

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
		const welp::bit_flags<0>& say(std::size_t bits) const
		{
			std::cout << ">>>  bit 0 : " << load(0) << "\n";
			for (std::size_t k = 1; k < bits; k++)
			{
				std::cout << "     bit " << k << " : " << load(k) << "\n";
			}
			std::cout << std::endl;
			return *this;
		}
		welp::bit_flags<0>& say(std::size_t bits)
		{
			std::cout << ">>>  bit 0 : " << load(0) << "\n";
			for (std::size_t k = 1; k < bits; k++)
			{
				std::cout << "     bit " << k << " : " << load(k) << "\n";
			}
			std::cout << std::endl;
			return *this;
		}
		const welp::bit_flags<0>& say(std::size_t start_bit, std::size_t end_bit) const
		{
			std::cout << ">>>  bit  " << start_bit << " : " << load(start_bit) << "\n";
			for (std::size_t k = start_bit + 1; k < end_bit; k++)
			{
				std::cout << "     bit " << k << " : " << load(k) << "\n";
			}
			std::cout << std::endl;
			return *this;
		}
		welp::bit_flags<0>& say(std::size_t start_bit, std::size_t end_bit)
		{
			std::cout << ">>>  bit  " << start_bit << " : " << load(start_bit) << "\n";
			for (std::size_t k = start_bit + 1; k < end_bit; k++)
			{
				std::cout << "     bit " << k << " : " << load(k) << "\n";
			}
			std::cout << std::endl;
			return *this;
		}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM

		bit_flags() = delete;
		bit_flags(const welp::bit_flags<0>&) = delete;
		welp::bit_flags<0>& operator=(const welp::bit_flags<0>&) = delete;
		bit_flags(welp::bit_flags<0>&&) = delete;
		welp::bit_flags<0>& operator=(welp::bit_flags<0>&&) = delete;
		~bit_flags() = default;

	private:

		std::uint8_t field[1];

		inline std::uint8_t shift_true(std::size_t bit_offset) const noexcept
		{
			switch (bit_offset)
			{
			case 0: return static_cast<std::uint8_t>(1); break;
			case 1: return static_cast<std::uint8_t>(2); break;
			case 2: return static_cast<std::uint8_t>(4); break;
			case 3: return static_cast<std::uint8_t>(8); break;
			case 4: return static_cast<std::uint8_t>(16); break;
			case 5: return static_cast<std::uint8_t>(32); break;
			case 6: return static_cast<std::uint8_t>(64); break;
			case 7: return static_cast<std::uint8_t>(128); break;
			default: return static_cast<std::uint8_t>(0); break;
			}
		}
		inline std::uint8_t shift_false(std::size_t bit_offset) const noexcept
		{
			switch (bit_offset)
			{
			case 0: return static_cast<std::uint8_t>(254); break;
			case 1: return static_cast<std::uint8_t>(253); break;
			case 2: return static_cast<std::uint8_t>(251); break;
			case 3: return static_cast<std::uint8_t>(247); break;
			case 4: return static_cast<std::uint8_t>(239); break;
			case 5: return static_cast<std::uint8_t>(223); break;
			case 6: return static_cast<std::uint8_t>(191); break;
			case 7: return static_cast<std::uint8_t>(127); break;
			default: return static_cast<std::uint8_t>(255); break;
			}
		}
	};
}

// bytes

template <std::size_t bytes>
inline char welp::byte_flags<bytes>::load(std::size_t byte_offset, bool upper_half_byte) const noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(byte_offset < bytes);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	std::uint8_t temp = (upper_half_byte) ? (field[byte_offset] >> 4) : (field[byte_offset] & static_cast<std::uint8_t>(15));

	switch (temp)
	{
	case 0: return '0'; break;
	case 1: return '1'; break;
	case 2: return '2'; break;
	case 3: return '3'; break;
	case 4: return '4'; break;
	case 5: return '5'; break;
	case 6: return '6'; break;
	case 7: return '7'; break;
	case 8: return '8'; break;
	case 9: return '9'; break;

	case 10: return 'A'; break;
	case 11: return 'B'; break;
	case 12: return 'C'; break;
	case 13: return 'D'; break;
	case 14: return 'E'; break;
	case 15: return 'F'; break;

	default: return '?'; break;
	}
}

template <std::size_t bytes>
inline welp::byte_flags<bytes>& welp::byte_flags<bytes>::store(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
{
#ifdef WELP_BIT_FLAGS_DEBUG_MODE
	assert(byte_offset < bytes);
#endif // WELP_BIT_FLAGS_DEBUG_MODE
	std::uint8_t temp = char_to_uint8_t(hex);

	if (upper_half_byte)
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(240)) | (temp << 4);
	}
	else
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(15)) | temp;
	}

	return *this;
}

template <std::size_t bytes>
inline welp::byte_flags<bytes>& welp::byte_flags<bytes>::set(std::uint8_t number) noexcept
{
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] = number;
	}
	return *this;
}

template <std::size_t bytes>
inline welp::byte_flags<bytes>& welp::byte_flags<bytes>::set(char lower_half_byte, char upper_half_byte) noexcept
{
	std::uint8_t temp = char_to_uint8_t(lower_half_byte) | (char_to_uint8_t(upper_half_byte) << 4);

	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] = temp;
	}
	return *this;
}

template <std::size_t bytes>
template <class Ty> inline welp::byte_flags<bytes>& welp::byte_flags<bytes>::cpy(const Ty& rhs) noexcept
{
	std::memcpy(static_cast<std::uint8_t*>(field), &rhs, bytes);
	return *this;
}

template <std::size_t bytes>
inline const std::uint8_t& welp::byte_flags<bytes>::operator[](std::size_t offset) const noexcept
{
	return field[offset];
}

template <std::size_t bytes>
inline std::uint8_t& welp::byte_flags<bytes>::operator[](std::size_t offset) noexcept
{
	return field[offset];
}

template <std::size_t bytes>
inline const std::uint8_t* welp::byte_flags<bytes>::data() const noexcept
{
	return static_cast<const std::uint8_t*>(field);
}

template <std::size_t bytes>
inline std::uint8_t* welp::byte_flags<bytes>::data() noexcept
{
	return static_cast<std::uint8_t*>(field);
}


#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bytes>
const welp::byte_flags<bytes>& welp::byte_flags<bytes>::say() const
{
	std::cout << ">>>  byte 0 : " << load(0, false) << load(0, true)
		<< "   (" << static_cast<unsigned int>(field[0]) << ")\n";
	for (std::size_t k = 1; k < bytes; k++)
	{
		std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
			<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
	}
	std::cout << std::endl;
	return *this;
}

template <std::size_t bytes>
welp::byte_flags<bytes>& welp::byte_flags<bytes>::say()
{
	std::cout << ">>>  byte 0 : " << load(0, false) << load(0, true)
		<< "   (" << static_cast<unsigned int>(field[0]) << ")\n";
	for (std::size_t k = 1; k < bytes; k++)
	{
		std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
			<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
	}
	std::cout << std::endl;
	return *this;
}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM


template <std::size_t bytes>
inline std::uint8_t welp::byte_flags<bytes>::char_to_uint8_t(char hex) const noexcept
{
	switch (hex)
	{
	case '0': return static_cast<std::uint8_t>(0); break;
	case '1': return static_cast<std::uint8_t>(1); break;
	case '2': return static_cast<std::uint8_t>(2); break;
	case '3': return static_cast<std::uint8_t>(3); break;
	case '4': return static_cast<std::uint8_t>(4); break;
	case '5': return static_cast<std::uint8_t>(5); break;
	case '6': return static_cast<std::uint8_t>(6); break;
	case '7': return static_cast<std::uint8_t>(7); break;
	case '8': return static_cast<std::uint8_t>(8); break;
	case '9': return static_cast<std::uint8_t>(9); break;

	case 'A': return static_cast<std::uint8_t>(10); break;
	case 'B': return static_cast<std::uint8_t>(11); break;
	case 'C': return static_cast<std::uint8_t>(12); break;
	case 'D': return static_cast<std::uint8_t>(13); break;
	case 'E': return static_cast<std::uint8_t>(14); break;
	case 'F': return static_cast<std::uint8_t>(15); break;

	case 'a': return static_cast<std::uint8_t>(10); break;
	case 'b': return static_cast<std::uint8_t>(11); break;
	case 'c': return static_cast<std::uint8_t>(12); break;
	case 'd': return static_cast<std::uint8_t>(13); break;
	case 'e': return static_cast<std::uint8_t>(14); break;
	case 'f': return static_cast<std::uint8_t>(15); break;

	default: return static_cast<std::uint8_t>(0); break;
	}
}

namespace welp
{
	template <std::size_t bytes> inline bool operator==(const welp::byte_flags<bytes>& A, const welp::byte_flags<bytes>& B) noexcept
	{
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return false; }
		}
		return true;
	}
	template <std::size_t bytes> inline bool operator!=(const welp::byte_flags<bytes>& A, const welp::byte_flags<bytes>& B) noexcept
	{
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return true; }
		}
		return false;
	}

	template <class Ty> inline const welp::byte_flags<sizeof(Ty)>& as_byte_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::byte_flags<sizeof(Ty)>&>(rhs);
	}
	template <class Ty> inline welp::byte_flags<sizeof(Ty)>& as_byte_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::byte_flags<sizeof(Ty)>&>(rhs);
	}
	template <std::size_t bytes, class Ty> inline const welp::byte_flags<bytes>& as_byte_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::byte_flags<bytes>&>(rhs);
	}
	template <std::size_t bytes, class Ty> inline welp::byte_flags<bytes>& as_byte_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::byte_flags<bytes>&>(rhs);
	}
}

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bytes> std::ostream& operator<<(std::ostream& out, const welp::byte_flags<bytes>& A)
{
	A.say(); return out;
}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM

namespace welp
{
	template <> class byte_flags<0>
	{

	public:

		inline char load(std::size_t byte_offset, bool upper_half_byte) const noexcept
		{
			std::uint8_t temp = (upper_half_byte) ? (field[byte_offset] >> 4) : (field[byte_offset] & static_cast<std::uint8_t>(15));

			switch (temp)
			{
			case 0: return '0'; break;
			case 1: return '1'; break;
			case 2: return '2'; break;
			case 3: return '3'; break;
			case 4: return '4'; break;
			case 5: return '5'; break;
			case 6: return '6'; break;
			case 7: return '7'; break;
			case 8: return '8'; break;
			case 9: return '9'; break;

			case 10: return 'A'; break;
			case 11: return 'B'; break;
			case 12: return 'C'; break;
			case 13: return 'D'; break;
			case 14: return 'E'; break;
			case 15: return 'F'; break;

			default: return '?'; break;
			}
		}
		inline byte_flags<0>& store(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
		{
			std::uint8_t temp = char_to_uint8_t(hex);

			if (upper_half_byte)
			{
				field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(240)) | (temp << 4);
			}
			else
			{
				field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(15)) | temp;
			}

			return *this;
		}

#ifdef WELP_BIT_FLAGS_INCLUDE_IOSTREAM
		const welp::byte_flags<0>& say(std::size_t bytes) const
		{
			std::cout << ">>>  byte 0 : " << load(0, false) << load(0, true)
				<< "   (" << static_cast<unsigned int>(field[0]) << ")\n";
			for (std::size_t k = 1; k < bytes; k++)
			{
				std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
					<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
			}
			std::cout << std::endl;
			return *this;
		}
		welp::byte_flags<0>& say(std::size_t bytes)
		{
			std::cout << ">>>  byte 0 : " << load(0, false) << load(0, true)
				<< "   (" << static_cast<unsigned int>(field[0]) << ")\n";
			for (std::size_t k = 1; k < bytes; k++)
			{
				std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
					<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
			}
			std::cout << std::endl;
			return *this;
		}
		const welp::byte_flags<0>& say(std::size_t start_byte, std::size_t end_byte) const
		{
			std::cout << ">>>  byte " << start_byte << " : " << load(start_byte, false) << load(start_byte, true)
				<< "   (" << static_cast<unsigned int>(field[start_byte]) << ")\n";
			for (std::size_t k = start_byte + 1; k < end_byte; k++)
			{
				std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
					<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
			}
			std::cout << std::endl;
			return *this;
		}
		welp::byte_flags<0>& say(std::size_t start_byte, std::size_t end_byte)
		{
			std::cout << ">>>  byte " << start_byte << " : " << load(start_byte, false) << load(start_byte, true)
				<< "   (" << static_cast<unsigned int>(field[start_byte]) << ")\n";
			for (std::size_t k = start_byte + 1; k < end_byte; k++)
			{
				std::cout << "     byte " << k << " : " << load(k, false) << load(k, true)
					<< "   (" << static_cast<unsigned int>(field[k]) << ")\n";
			}
			std::cout << std::endl;
			return *this;
		}
#endif // WELP_BIT_FLAGS_INCLUDE_IOSTREAM

		inline const std::uint8_t& operator[](std::size_t offset) const noexcept
		{
			return *(static_cast<const std::uint8_t*>(field) + offset);
		}
		inline std::uint8_t& operator[](std::size_t offset) noexcept
		{
			return *(static_cast<std::uint8_t*>(field) + offset);
		}

		inline const std::uint8_t* data() const noexcept
		{
			return static_cast<const std::uint8_t*>(field);
		}
		inline std::uint8_t* data() noexcept
		{
			return static_cast<std::uint8_t*>(field);
		}

		byte_flags() = delete;
		byte_flags(const welp::byte_flags<0>&) = delete;
		welp::byte_flags<0>& operator=(const welp::byte_flags<0>&) = delete;
		byte_flags(welp::byte_flags<0>&&) = delete;
		welp::byte_flags<0>& operator=(welp::byte_flags<0>&&) = delete;
		~byte_flags() = default;

	private:

		std::uint8_t field[1];

		inline std::uint8_t char_to_uint8_t(char hex) const noexcept
		{
			switch (hex)
			{
			case '0': return static_cast<std::uint8_t>(0); break;
			case '1': return static_cast<std::uint8_t>(1); break;
			case '2': return static_cast<std::uint8_t>(2); break;
			case '3': return static_cast<std::uint8_t>(3); break;
			case '4': return static_cast<std::uint8_t>(4); break;
			case '5': return static_cast<std::uint8_t>(5); break;
			case '6': return static_cast<std::uint8_t>(6); break;
			case '7': return static_cast<std::uint8_t>(7); break;
			case '8': return static_cast<std::uint8_t>(8); break;
			case '9': return static_cast<std::uint8_t>(9); break;

			case 'A': return static_cast<std::uint8_t>(10); break;
			case 'B': return static_cast<std::uint8_t>(11); break;
			case 'C': return static_cast<std::uint8_t>(12); break;
			case 'D': return static_cast<std::uint8_t>(13); break;
			case 'E': return static_cast<std::uint8_t>(14); break;
			case 'F': return static_cast<std::uint8_t>(15); break;

			case 'a': return static_cast<std::uint8_t>(10); break;
			case 'b': return static_cast<std::uint8_t>(11); break;
			case 'c': return static_cast<std::uint8_t>(12); break;
			case 'd': return static_cast<std::uint8_t>(13); break;
			case 'e': return static_cast<std::uint8_t>(14); break;
			case 'f': return static_cast<std::uint8_t>(15); break;

			default: return static_cast<std::uint8_t>(0); break;
			}
		}
	};
}


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
