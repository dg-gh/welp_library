// welp_bits.h - last update : 28 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)

// ATTN : requires little-endian layout for unsigned integers


#ifndef WELP_BITS_HPP
#define WELP_BITS_HPP


////// INCLUDES //////

#include <cstring>


// include all in one line with #define WELP_BITS_INCLUDE_ALL
#if defined(WELP_BITS_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_BITS_INCLUDE_IOSTREAM
#define WELP_BITS_INCLUDE_IOSTREAM
#endif
#endif // WELP_BITS_INCLUDE_ALL


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_BITS_DEBUG_MODE)
#define WELP_BITS_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_BITS_DEBUG_MODE
#include <cassert>
#ifndef WELP_BITS_INCLUDE_IOSTREAM
#define WELP_BITS_INCLUDE_IOSTREAM
#endif
#endif // WELP_BITS_DEBUG_MODE

#ifdef WELP_BITS_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_BITS_INCLUDE_IOSTREAM


////// OPTIONS //////

// #define WELP_BITS_USE_CHAR_ARITHMETIC will enforce the use of char arithemtic when converting
// to and from hexadecimals instead of using a switch statemet

// if char arithmetic is not used, any unsigned char greater than 15 will convert to '?'
// and any char different than '0', '1', ... ,'9', 'a', 'b', ... , 'f', 'A', 'B', ... , 'F'
// will convert to 0, although this situation should not occur in normal circumstances


////// DESCRIPTIONS //////

namespace welp
{
	template <std::size_t number_of_bits> class bits
	{

	public:

		inline bool load_bit(std::size_t bit_offset) const noexcept;
		inline welp::bits<number_of_bits>& store_bit(std::size_t bit_offset, bool bit) noexcept;
		inline bool load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept;
		inline welp::bits<number_of_bits>& store_bit(std::size_t byte_offset, std::size_t bit_offset, bool bit) noexcept;

		inline char load_hex_lc(std::size_t hex_offset) const noexcept;
		inline char load_hex_uc(std::size_t hex_offset) const noexcept;
		inline bits<number_of_bits>& store_hex(std::size_t hex_offset, char hex) noexcept;
		inline char load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept;
		inline char load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept;
		inline bits<number_of_bits>& store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept;

		inline unsigned char load_byte(std::size_t byte_offset) const noexcept;
		inline bits<number_of_bits>& store_byte(std::size_t byte_offset, unsigned char number) noexcept;

		inline char load_char(std::size_t byte_offset) const noexcept;
		inline bits<number_of_bits>& store_char(std::size_t byte_offset, char character) noexcept;

		constexpr const unsigned char* data() const noexcept { return static_cast<const unsigned char*>(m_field); }
		constexpr unsigned char* data() noexcept { return static_cast<unsigned char*>(m_field); }


		// member functions below are unavailable when template parameter bits == 0

		inline welp::bits<number_of_bits>& set_bits(bool bit) noexcept;
		inline welp::bits<number_of_bits>& set_hex(char hex) noexcept;
		inline welp::bits<number_of_bits>& set_bytes(unsigned char number) noexcept;
		inline welp::bits<number_of_bits>& set_chars(char character) noexcept;

		template <class Ty> inline welp::bits<number_of_bits>& cpy(const Ty& rhs) noexcept;

		inline welp::bits<number_of_bits>& operator&=(const welp::bits<number_of_bits>& rhs) noexcept;
		inline welp::bits<number_of_bits>& operator|=(const welp::bits<number_of_bits>& rhs) noexcept;
		inline welp::bits<number_of_bits>& operator^=(const welp::bits<number_of_bits>& rhs) noexcept;
		inline welp::bits<number_of_bits>& flip() noexcept;

		constexpr std::size_t capacity_in_bits() const noexcept { return number_of_bits; }
		constexpr std::size_t capacity_in_hex() const noexcept { return number_of_bits >> 2; }
		constexpr std::size_t capacity_in_bytes() const noexcept { return number_of_bits >> 3; }
		constexpr std::size_t capacity_in_chars() const noexcept { return number_of_bits >> 3; }
		constexpr std::size_t size_in_bytes() const noexcept { return (number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3; }


#ifdef WELP_BITS_INCLUDE_IOSTREAM
		const welp::bits<number_of_bits>& say_bits() const;
		welp::bits<number_of_bits>& say_bits();
		const welp::bits<number_of_bits>& say_hex() const;
		welp::bits<number_of_bits>& say_hex();
		const welp::bits<number_of_bits>& say_bytes() const;
		welp::bits<number_of_bits>& say_bytes();
		const welp::bits<number_of_bits>& say_chars() const;
		welp::bits<number_of_bits>& say_chars();
#endif // WELP_BITS_INCLUDE_IOSTREAM


		// constructor, copy ops and move ops are deleted when template parameter bits == 0,
		// welp::bits<0> should only exist as a reference

		bits() noexcept;
		bits(const welp::bits<number_of_bits>&) noexcept = default;
		welp::bits<number_of_bits>& operator=(const welp::bits<number_of_bits>&) noexcept;
		bits(welp::bits<number_of_bits>&&) noexcept = default;
		welp::bits<number_of_bits>& operator=(welp::bits<number_of_bits>&&) noexcept;
		~bits() = default;

		template <std::size_t bits2>
		friend inline bool operator==(const welp::bits<bits2>&, const welp::bits<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline bool operator!=(const welp::bits<bits2>&, const welp::bits<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bits<bits2> operator&(const welp::bits<bits2>&, const welp::bits<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bits<bits2> operator|(const welp::bits<bits2>&, const welp::bits<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bits<bits2> operator^(const welp::bits<bits2>&, const welp::bits<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::bits<bits2> operator~(const welp::bits<bits2>&) noexcept;

	private:

		unsigned char m_field[(number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3];

		inline unsigned char shift_true(std::size_t bit_offset) const noexcept;
		inline unsigned char shift_false(std::size_t bit_offset) const noexcept;
		inline unsigned char bitmask_true(std::size_t digits) const noexcept;
		inline unsigned char bitmask_false(std::size_t digits) const noexcept;
		inline unsigned char char_to_uint8_t(char hex) const noexcept;
		inline char uint8_t_to_char_lc(unsigned char number) const noexcept;
		inline char uint8_t_to_char_uc(unsigned char number) const noexcept;

		void say_bits_sub() const;
		void say_hex_sub() const;
		void say_bytes_sub() const;
		void say_chars_sub() const;
	};

	template <std::size_t number_of_bits> inline bool operator==(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept;
	template <std::size_t number_of_bits> inline bool operator!=(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept;

	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator&(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept;
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator|(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept;
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator^(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept;
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator~(const welp::bits<number_of_bits>& A) noexcept;

	template <class Ty> inline const welp::bits<8 * sizeof(Ty)>& as_bits(const Ty& rhs) noexcept;
	template <class Ty> inline welp::bits<8 * sizeof(Ty)>& as_bits(Ty& rhs) noexcept;
	template <std::size_t number_of_bits, class Ty> inline const welp::bits<number_of_bits>& as_bits(const Ty& rhs) noexcept;
	template <std::size_t number_of_bits, class Ty> inline welp::bits<number_of_bits>& as_bits(Ty& rhs) noexcept;
}


////// IMPLEMENTATIONS //////

template <std::size_t number_of_bits>
inline bool welp::bits<number_of_bits>::load_bit(std::size_t bit_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	return (m_field[bit_offset >> 3] & shift_true(bit_offset & 7)) != static_cast<unsigned char>(0);
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_bit(std::size_t bit_offset, bool bit) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	if (bit)
	{
		m_field[bit_offset >> 3] |= shift_true(bit_offset & 7);
		return *this;
	}
	else
	{
		m_field[bit_offset >> 3] &= shift_false(bit_offset & 7);
		return *this;
	}
}

template <std::size_t number_of_bits>
inline bool welp::bits<number_of_bits>::load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset + bit_offset < bits);
	assert(bit_offset < 8);
#endif // WELP_BITS_DEBUG_MODE
	return (m_field[byte_offset] & shift_true(bit_offset)) != static_cast<unsigned char>(0);
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_bit(std::size_t byte_offset, std::size_t bit_offset, bool bit) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset + bit_offset < bits);
	assert(bit_offset < 8);
#endif // WELP_BITS_DEBUG_MODE
	if (bit)
	{
		m_field[byte_offset] |= shift_true(bit_offset);
		return *this;
	}
	else
	{
		m_field[byte_offset] &= shift_false(bit_offset);
		return *this;
	}
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::load_hex_lc(std::size_t hex_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
		(m_field[hex_offset >> 1] >> 4) : (m_field[hex_offset >> 1] & static_cast<unsigned char>(15));
	return uint8_t_to_char_lc(temp);
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::load_hex_uc(std::size_t hex_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
		(m_field[hex_offset >> 1] >> 4) : (m_field[hex_offset >> 1] & static_cast<unsigned char>(15));
	return uint8_t_to_char_uc(temp);
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_hex(std::size_t hex_offset, char hex) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = char_to_uint8_t(hex);
	std::size_t byte_offset = hex_offset >> 1;
	if ((hex_offset & static_cast<std::size_t>(1)) != 0)
	{
		m_field[byte_offset] = (m_field[byte_offset] & static_cast<unsigned char>(15)) | (temp << 4);
	}
	else
	{
		m_field[byte_offset] = (m_field[byte_offset] & static_cast<unsigned char>(240)) | temp;
	}

	return *this;
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = (upper_half_byte) ? (m_field[byte_offset] >> 4) : (m_field[byte_offset] & static_cast<unsigned char>(15));
	return uint8_t_to_char_lc(temp);
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = (upper_half_byte) ? (m_field[byte_offset] >> 4) : (m_field[byte_offset] & static_cast<unsigned char>(15));
	return uint8_t_to_char_uc(temp);
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_BITS_DEBUG_MODE
	unsigned char temp = char_to_uint8_t(hex);

	if (upper_half_byte)
	{
		m_field[byte_offset] = (m_field[byte_offset] & static_cast<unsigned char>(15)) | (temp << 4);
	}
	else
	{
		m_field[byte_offset] = (m_field[byte_offset] & static_cast<unsigned char>(240)) | temp;
	}

	return *this;
}

template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::load_byte(std::size_t byte_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	return m_field[byte_offset];
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_byte(std::size_t byte_offset, unsigned char number) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	m_field[byte_offset] = number;
	return *this;
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::load_char(std::size_t byte_offset) const noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	return *(reinterpret_cast<const char*>(m_field) + byte_offset);
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::store_char(std::size_t byte_offset, char character) noexcept
{
#ifdef WELP_BITS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_BITS_DEBUG_MODE
	* (reinterpret_cast<char*>(m_field) + byte_offset) = character;
	return *this;
}


template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::set_bits(bool bit) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	if (bit)
	{
		std::memset(static_cast<unsigned char*>(m_field), -static_cast<unsigned char>(1), bytes);
		*(static_cast<unsigned char*>(m_field) + bytes) |= bitmask_true(remainder_bits);
		return *this;
	}
	else
	{
		std::memset(static_cast<unsigned char*>(m_field), static_cast<unsigned char>(0), bytes);
		*(static_cast<unsigned char*>(m_field) + bytes) &= bitmask_false(remainder_bits);
		return *this;
	}
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::set_hex(char hex) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	unsigned char temp = char_to_uint8_t(hex);
	if ((number_of_bits & 7) > 3)
	{
		*(static_cast<unsigned char*>(m_field) + bytes) &= bitmask_false(4);
		*(static_cast<unsigned char*>(m_field) + bytes) |= temp;
	}
	temp |= (temp << 4);
	std::memset(static_cast<unsigned char*>(m_field), temp, bytes);
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::set_bytes(unsigned char number) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	std::memset(static_cast<unsigned char*>(m_field), number, bytes);
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::set_chars(char character) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	std::memset(reinterpret_cast<char*>(m_field), character, bytes);
	return *this;
}


template <std::size_t number_of_bits> template <class Ty>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::cpy(const Ty& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	const unsigned char* rhs_ptr = static_cast<const unsigned char*>(static_cast<const void*>(&rhs));
	std::memcpy(static_cast<unsigned char*>(m_field), rhs_ptr, bytes);
	if (remainder_bits != 0)
	{
		m_field[bytes] |= (*(rhs_ptr + bytes) & bitmask_true(remainder_bits));
	}
	return *this;
}


template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::operator&=(const welp::bits<number_of_bits>& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		m_field[k] &= rhs.m_field[k];
	}
	if (remainder_bits != 0)
	{
		m_field[bytes] &= (rhs.m_field[bytes] | bitmask_false(remainder_bits));
	}
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::operator|=(const welp::bits<number_of_bits>& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		m_field[k] |= rhs.m_field[k];
	}
	if (remainder_bits != 0)
	{
		m_field[bytes] |= (rhs.m_field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::operator^=(const welp::bits<number_of_bits>& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		m_field[k] ^= rhs.m_field[k];
	}
	if (remainder_bits != 0)
	{
		m_field[bytes] ^= (rhs.m_field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::flip() noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	constexpr unsigned char ones = -static_cast<unsigned char>(1);
	for (std::size_t k = 0; k < bytes; k++)
	{
		m_field[k] ^= ones;
	}
	if (remainder_bits != 0)
	{
		m_field[bytes] ^= bitmask_true(remainder_bits);
	}
	return *this;
}


#ifdef WELP_BITS_INCLUDE_IOSTREAM
template <std::size_t number_of_bits>
const welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_bits() const
{
	say_bits_sub(); return *this;
}

template <std::size_t number_of_bits>
welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_bits()
{
	say_bits_sub(); return *this;
}

template <std::size_t number_of_bits>
const welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_hex() const
{
	say_hex_sub(); return *this;
}

template <std::size_t number_of_bits>
welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_hex()
{
	say_hex_sub(); return *this;
}

template <std::size_t number_of_bits>
const welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_bytes() const
{
	say_bytes_sub(); return *this;
}

template <std::size_t number_of_bits>
welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_bytes()
{
	say_bytes_sub(); return *this;
}

template <std::size_t number_of_bits>
const welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_chars() const
{
	say_chars_sub(); return *this;
}

template <std::size_t number_of_bits>
welp::bits<number_of_bits>& welp::bits<number_of_bits>::say_chars()
{
	say_chars_sub(); return *this;
}
#endif // WELP_BITS_INCLUDE_IOSTREAM


template <std::size_t number_of_bits>
welp::bits<number_of_bits>::bits() noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	std::memset(static_cast<unsigned char*>(m_field), static_cast<unsigned char>(0), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<unsigned char*>(m_field) + bytes) &= bitmask_false(remainder_bits);
	}
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::operator=(const welp::bits<number_of_bits>& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	std::memcpy(static_cast<unsigned char*>(m_field), static_cast<const unsigned char*>(rhs.m_field), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<unsigned char*>(m_field) + bytes) &= bitmask_false(remainder_bits);
		*(static_cast<unsigned char*>(m_field) + bytes)& bitmask_true(remainder_bits);
	}
	return *this;
}

template <std::size_t number_of_bits>
inline welp::bits<number_of_bits>& welp::bits<number_of_bits>::operator=(welp::bits<number_of_bits>&& rhs) noexcept
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	constexpr std::size_t remainder_bits = number_of_bits & 7;
	std::memcpy(static_cast<unsigned char*>(m_field), static_cast<const unsigned char*>(rhs.m_field), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<unsigned char*>(m_field) + bytes) &= bitmask_false(remainder_bits);
		*(static_cast<unsigned char*>(m_field) + bytes) |= (rhs.m_field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}


template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::shift_true(std::size_t bit_offset) const noexcept
{
	switch (bit_offset)
	{
	case 0: return static_cast<unsigned char>(1); break;
	case 1: return static_cast<unsigned char>(2); break;
	case 2: return static_cast<unsigned char>(4); break;
	case 3: return static_cast<unsigned char>(8); break;
	case 4: return static_cast<unsigned char>(16); break;
	case 5: return static_cast<unsigned char>(32); break;
	case 6: return static_cast<unsigned char>(64); break;
	case 7: return static_cast<unsigned char>(128); break;
	default: return static_cast<unsigned char>(0); break;
	}
}

template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::shift_false(std::size_t bit_offset) const noexcept
{
	switch (bit_offset)
	{
	case 0: return static_cast<unsigned char>(254); break;
	case 1: return static_cast<unsigned char>(253); break;
	case 2: return static_cast<unsigned char>(251); break;
	case 3: return static_cast<unsigned char>(247); break;
	case 4: return static_cast<unsigned char>(239); break;
	case 5: return static_cast<unsigned char>(223); break;
	case 6: return static_cast<unsigned char>(191); break;
	case 7: return static_cast<unsigned char>(127); break;
	default: return static_cast<unsigned char>(255); break;
	}
}

template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::bitmask_true(std::size_t digits) const noexcept
{
	switch (digits)
	{
	case 0: return static_cast<unsigned char>(0); break;
	case 1: return static_cast<unsigned char>(1); break;
	case 2: return static_cast<unsigned char>(3); break;
	case 3: return static_cast<unsigned char>(7); break;
	case 4: return static_cast<unsigned char>(15); break;
	case 5: return static_cast<unsigned char>(31); break;
	case 6: return static_cast<unsigned char>(63); break;
	case 7: return static_cast<unsigned char>(127); break;
	default: return static_cast<unsigned char>(255); break;
	}
}

template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::bitmask_false(std::size_t digits) const noexcept
{
	switch (digits)
	{
	case 0: return static_cast<unsigned char>(255); break;
	case 1: return static_cast<unsigned char>(254); break;
	case 2: return static_cast<unsigned char>(252); break;
	case 3: return static_cast<unsigned char>(248); break;
	case 4: return static_cast<unsigned char>(240); break;
	case 5: return static_cast<unsigned char>(224); break;
	case 6: return static_cast<unsigned char>(192); break;
	case 7: return static_cast<unsigned char>(128); break;
	default: return static_cast<unsigned char>(0); break;
	}
}

template <std::size_t number_of_bits>
inline unsigned char welp::bits<number_of_bits>::char_to_uint8_t(char hex) const noexcept
{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
	switch (hex)
	{
	case '0': return static_cast<unsigned char>(0); break;
	case '1': return static_cast<unsigned char>(1); break;
	case '2': return static_cast<unsigned char>(2); break;
	case '3': return static_cast<unsigned char>(3); break;
	case '4': return static_cast<unsigned char>(4); break;
	case '5': return static_cast<unsigned char>(5); break;
	case '6': return static_cast<unsigned char>(6); break;
	case '7': return static_cast<unsigned char>(7); break;
	case '8': return static_cast<unsigned char>(8); break;
	case '9': return static_cast<unsigned char>(9); break;

	case 'A': return static_cast<unsigned char>(10); break;
	case 'B': return static_cast<unsigned char>(11); break;
	case 'C': return static_cast<unsigned char>(12); break;
	case 'D': return static_cast<unsigned char>(13); break;
	case 'E': return static_cast<unsigned char>(14); break;
	case 'F': return static_cast<unsigned char>(15); break;

	case 'a': return static_cast<unsigned char>(10); break;
	case 'b': return static_cast<unsigned char>(11); break;
	case 'c': return static_cast<unsigned char>(12); break;
	case 'd': return static_cast<unsigned char>(13); break;
	case 'e': return static_cast<unsigned char>(14); break;
	case 'f': return static_cast<unsigned char>(15); break;

	default: return static_cast<unsigned char>(0); break;
	}
#else // WELP_BITS_USE_CHAR_ARITHMETIC
	if (hex < ':') { return static_cast<unsigned char>(hex - '0'); }
	else if (hex < 'G') { return static_cast<unsigned char>(hex - '7'); }
	else { return static_cast<unsigned char>(hex - 'W'); }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::uint8_t_to_char_lc(unsigned char number) const noexcept
{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
	switch (number)
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

	case 10: return 'a'; break;
	case 11: return 'b'; break;
	case 12: return 'c'; break;
	case 13: return 'd'; break;
	case 14: return 'e'; break;
	case 15: return 'f'; break;

	default: return '?'; break;
	}
#else // WELP_BITS_USE_CHAR_ARITHMETIC
	if (number < static_cast<unsigned char>(10)) { return reinterpret_cast<char&>(number) + '0'; }
	else { return reinterpret_cast<char&>(number) + 'W'; }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
}

template <std::size_t number_of_bits>
inline char welp::bits<number_of_bits>::uint8_t_to_char_uc(unsigned char number) const noexcept
{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
	switch (number)
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
#else // WELP_BITS_USE_CHAR_ARITHMETIC
	if (number < static_cast<unsigned char>(10)) { return reinterpret_cast<char&>(number) + '0'; }
	else { return reinterpret_cast<char&>(number) + '7'; }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
}


#ifdef WELP_BITS_INCLUDE_IOSTREAM
template <std::size_t number_of_bits>
void welp::bits<number_of_bits>::say_bits_sub() const
{
	std::cout << ">>>  bit 0  >  " << load_bit(0) << "\n";
	for (std::size_t k = 1; k < number_of_bits; k++)
	{
		std::cout << "     bit " << k << "  >  " << load_bit(k) << "\n";
	}
	std::cout << std::endl;
}

template <std::size_t number_of_bits>
void welp::bits<number_of_bits>::say_hex_sub() const
{
	constexpr std::size_t hex = number_of_bits >> 2;
	if (number_of_bits < 4)
	{
		std::cout << ">>>  capacity smaller than a hexadecimal" << std::endl;
	}
	else
	{
		std::cout << ">>>  hex 0  >  " << load_hex_uc(0)
			<< " :: " << load_bit(3) << load_bit(2) << load_bit(1) << load_bit(0) << "\n";
		for (std::size_t k = 1; k < hex; k++)
		{
			std::cout << "     hex " << k << "  >  " << load_hex_uc(k)
				<< " :: " << load_bit(4 * k + 3) << load_bit(4 * k + 2)
				<< load_bit(4 * k + 1) << load_bit(4 * k) << "\n";
		}
		std::cout << std::endl;
	}
}

template <std::size_t number_of_bits>
void welp::bits<number_of_bits>::say_bytes_sub() const
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	if (number_of_bits < 8)
	{
		std::cout << ">>>  capacity smaller than a byte" << std::endl;
	}
	else
	{
		std::cout << ">>>  byte 0  >  " << static_cast<unsigned int>(load_byte(0))
			<< " :: " << load_hex_uc(0, true) << load_hex_uc(0, false)
			<< " :: " << load_bit(0, 7) << load_bit(0, 6) << load_bit(0, 5) << load_bit(0, 4)
			<< load_bit(0, 3) << load_bit(0, 2) << load_bit(0, 1) << load_bit(0, 0) << "\n";
		for (std::size_t k = 1; k < bytes; k++)
		{
			std::cout << "     byte " << k << "  >  " << static_cast<unsigned int>(load_byte(k))
				<< " :: " << load_hex_uc(k, true) << load_hex_uc(k, false)
				<< " :: " << load_bit(k, 7) << load_bit(k, 6) << load_bit(k, 5) << load_bit(k, 4)
				<< load_bit(k, 3) << load_bit(k, 2) << load_bit(k, 1) << load_bit(k, 0) << "\n";
		}
		std::cout << std::endl;
	}
}

template <std::size_t number_of_bits>
void welp::bits<number_of_bits>::say_chars_sub() const
{
	constexpr std::size_t bytes = number_of_bits >> 3;
	if (number_of_bits < 8)
	{
		std::cout << ">>>  capacity smaller than a char" << std::endl;
	}
	else
	{
		std::cout << ">>>  char 0  >  " << load_char(0)
			<< " :: " << load_hex_uc(0, true) << load_hex_uc(0, false)
			<< " :: " << load_bit(0, 7) << load_bit(0, 6) << load_bit(0, 5) << load_bit(0, 4)
			<< load_bit(0, 3) << load_bit(0, 2) << load_bit(0, 1) << load_bit(0, 0) << "\n";
		for (std::size_t k = 1; k < bytes; k++)
		{
			std::cout << "     char " << k << "  >  " << load_char(k)
				<< " :: " << load_hex_uc(k, true) << load_hex_uc(k, false)
				<< " :: " << load_bit(k, 7) << load_bit(k, 6) << load_bit(k, 5) << load_bit(k, 4)
				<< load_bit(k, 3) << load_bit(k, 2) << load_bit(k, 1) << load_bit(k, 0) << "\n";
		}
		std::cout << std::endl;
	}
}
#endif // WELP_BITS_INCLUDE_IOSTREAM


namespace welp
{
	template <std::size_t number_of_bits> inline bool operator==(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept
	{
		constexpr std::size_t bytes = number_of_bits >> 3;
		constexpr std::size_t remainder_bits = number_of_bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.m_field[k] != B.m_field[k]) { return false; }
		}
		if (remainder_bits != 0)
		{
			unsigned char bitmask = A.bitmask_true(remainder_bits);
			if ((A.m_field[bytes] & bitmask) != (B.m_field[bytes] & bitmask)) { return false; }
			else { return true; }
		}
		else { return true; }
	}
	template <std::size_t number_of_bits> inline bool operator!=(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept
	{
		constexpr std::size_t bytes = number_of_bits >> 3;
		constexpr std::size_t remainder_bits = number_of_bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.m_field[k] != B.m_field[k]) { return true; }
		}
		if (remainder_bits != 0)
		{
			unsigned char bitmask = A.bitmask_true(remainder_bits);
			if ((A.m_field[bytes] & bitmask) != (B.m_field[bytes] & bitmask)) { return true; }
			else { return true; }
		}
		else { return true; }
	}

	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator&(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept
	{
		constexpr std::size_t bytes = (number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3;
		welp::bits<number_of_bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.m_field[k] = A.m_field[k] & B.m_field[k];
		}
		return C;
	}
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator|(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept
	{
		constexpr std::size_t bytes = (number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3;
		welp::bits<number_of_bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.m_field[k] = A.m_field[k] | B.m_field[k];
		}
		return C;
	}
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator^(const welp::bits<number_of_bits>& A, const welp::bits<number_of_bits>& B) noexcept
	{
		constexpr std::size_t bytes = (number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3;
		welp::bits<number_of_bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.m_field[k] = A.m_field[k] ^ B.m_field[k];
		}
		return C;
	}
	template <std::size_t number_of_bits> inline welp::bits<number_of_bits> operator~(const welp::bits<number_of_bits>& A) noexcept
	{
		constexpr std::size_t bytes = (number_of_bits + ((8 - (number_of_bits & 7)) & 7)) >> 3;
		welp::bits<number_of_bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.m_field[k] = ~A.m_field[k];
		}
		return C;
	}

	template <class Ty> inline const welp::bits<8 * sizeof(Ty)>& as_bits(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::bits<8 * sizeof(Ty)>&>(rhs);
	}
	template <class Ty> inline welp::bits<8 * sizeof(Ty)>& as_bits(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::bits<8 * sizeof(Ty)>&>(rhs);
	}
	template <std::size_t number_of_bits, class Ty> inline const welp::bits<number_of_bits>& as_bits(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::bits<number_of_bits>&>(rhs);
	}
	template <std::size_t number_of_bits, class Ty> inline welp::bits<number_of_bits>& as_bits(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::bits<number_of_bits>&>(rhs);
	}
}


namespace welp
{
	template <> class bits<0>
	{

	public:

		inline bool load_bit(std::size_t bit_offset) const noexcept
		{
			return (*(static_cast<const unsigned char*>(m_field) + (bit_offset >> 3)) & shift_true(bit_offset & 7)) != static_cast<unsigned char>(0);
		}
		inline welp::bits<0>& store_bit(std::size_t bit_offset, bool bit) noexcept
		{
			if (bit)
			{
				*(static_cast<unsigned char*>(m_field) + (bit_offset >> 3)) |= shift_true(bit_offset & 7);
				return *this;
			}
			else
			{
				*(static_cast<unsigned char*>(m_field) + (bit_offset >> 3)) &= shift_false(bit_offset & 7);
				return *this;
			}
		}
		inline bool load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept
		{
#ifdef WELP_BITS_DEBUG_MODE
			assert(bit_offset < 8);
#endif // WELP_BITS_DEBUG_MODE
			return (*(static_cast<const unsigned char*>(m_field) + byte_offset) & shift_true(bit_offset)) != static_cast<unsigned char>(0);
		}
		inline welp::bits<0>& store_bit(std::size_t byte_offset, std::size_t bit_offset, bool bit) noexcept
		{
#ifdef WELP_BITS_DEBUG_MODE
			assert(bit_offset < 8);
#endif // WELP_BITS_DEBUG_MODE
			if (bit)
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) |= shift_true(bit_offset);
				return *this;
			}
			else
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) &= shift_false(bit_offset);
				return *this;
			}
		}

		inline char load_hex_lc(std::size_t hex_offset) const noexcept
		{
			unsigned char temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
				(*(static_cast<const unsigned char*>(m_field) + (hex_offset >> 1)) >> 4)
				: (*(static_cast<const unsigned char*>(m_field) + (hex_offset >> 1)) & static_cast<unsigned char>(15));
			return uint8_t_to_char_lc(temp);
		}
		inline char load_hex_uc(std::size_t hex_offset) const noexcept
		{
			unsigned char temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
				(*(static_cast<const unsigned char*>(m_field) + (hex_offset >> 1)) >> 4)
				: (*(static_cast<const unsigned char*>(m_field) + (hex_offset >> 1)) & static_cast<unsigned char>(15));
			return uint8_t_to_char_uc(temp);
		}
		inline welp::bits<0>& store_hex(std::size_t hex_offset, char hex) noexcept
		{
			unsigned char temp = char_to_uint8_t(hex);
			std::size_t byte_offset = hex_offset >> 1;
			if ((hex_offset & static_cast<std::size_t>(1)) != 0)
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) = (*(static_cast<unsigned char*>(m_field) + byte_offset)
					& static_cast<unsigned char>(15)) | (temp << 4);
			}
			else
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) = (*(static_cast<unsigned char*>(m_field) + byte_offset)
					& static_cast<unsigned char>(240)) | temp;
			}

			return *this;
		}
		inline char load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept
		{
			unsigned char temp = (upper_half_byte) ?
				(*(static_cast<const unsigned char*>(m_field) + byte_offset) >> 4)
				: (*(static_cast<const unsigned char*>(m_field) + byte_offset) & static_cast<unsigned char>(15));
			return uint8_t_to_char_lc(temp);
		}
		inline char load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept
		{
			unsigned char temp = (upper_half_byte) ?
				(*(static_cast<const unsigned char*>(m_field) + byte_offset) >> 4)
				: (*(static_cast<const unsigned char*>(m_field) + byte_offset) & static_cast<unsigned char>(15));
			return uint8_t_to_char_uc(temp);
		}
		inline welp::bits<0>& store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
		{
			unsigned char temp = char_to_uint8_t(hex);

			if (upper_half_byte)
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) = (*(static_cast<unsigned char*>(m_field) + byte_offset)
					& static_cast<unsigned char>(15)) | (temp << 4);
			}
			else
			{
				*(static_cast<unsigned char*>(m_field) + byte_offset) = (*(static_cast<unsigned char*>(m_field) + byte_offset)
					& static_cast<unsigned char>(240)) | temp;
			}

			return *this;
		}

		inline unsigned char load_byte(std::size_t byte_offset) const noexcept
		{
			return *(static_cast<const unsigned char*>(m_field) + byte_offset);
		}
		inline welp::bits<0>& store_byte(std::size_t byte_offset, unsigned char number) noexcept
		{
			*(static_cast<unsigned char*>(m_field) + byte_offset) = number;
			return *this;
		}

		inline char load_char(std::size_t byte_offset) const noexcept
		{
			return *(reinterpret_cast<const char*>(m_field) + byte_offset);
		}
		inline welp::bits<0>& store_char(std::size_t byte_offset, char character) noexcept
		{
			*(reinterpret_cast<char*>(m_field) + byte_offset) = character;
			return *this;
		}

		constexpr const unsigned char* data() const noexcept { return static_cast<const unsigned char*>(m_field); }
		constexpr unsigned char* data() noexcept { return static_cast<unsigned char*>(m_field); }

#ifdef WELP_BITS_INCLUDE_IOSTREAM
		const welp::bits<0>& say_bits(std::size_t bits) const { say_bits_sub(0, bits); return *this; }
		welp::bits<0>& say_bits(std::size_t bits) { say_bits_sub(0, bits); return *this; }
		const welp::bits<0>& say_bits(std::size_t start_bit, std::size_t end_bit) const { say_bits_sub(start_bit, end_bit); return *this; }
		welp::bits<0>& say_bits(std::size_t start_bit, std::size_t end_bit) { say_bits_sub(start_bit, end_bit); return *this; }

		const welp::bits<0>& say_hex(std::size_t hex) const { say_hex_sub(0, hex); return *this; }
		welp::bits<0>& say_hex(std::size_t hex) { say_hex_sub(0, hex); return *this; }
		const welp::bits<0>& say_hex(std::size_t start_hex, std::size_t end_hex) const { say_hex_sub(start_hex, end_hex); return *this; }
		welp::bits<0>& say_hex(std::size_t start_hex, std::size_t end_hex) { say_hex_sub(start_hex, end_hex); return *this; }

		const welp::bits<0>& say_bytes(std::size_t bytes) const { say_bytes_sub(0, bytes); return *this; }
		welp::bits<0>& say_bytes(std::size_t bytes) { say_bytes_sub(0, bytes); return *this; }
		const welp::bits<0>& say_bytes(std::size_t start_byte, std::size_t end_byte) const { say_bytes_sub(start_byte, end_byte); return *this; }
		welp::bits<0>& say_bytes(std::size_t start_byte, std::size_t end_byte) { say_bytes_sub(start_byte, end_byte); return *this; }

		const welp::bits<0>& say_chars(std::size_t bytes) const { say_chars_sub(0, bytes); return *this; }
		welp::bits<0>& say_chars(std::size_t bytes) { say_chars_sub(0, bytes); return *this; }
		const welp::bits<0>& say_chars(std::size_t start_byte, std::size_t end_byte) const { say_chars_sub(start_byte, end_byte); return *this; }
		welp::bits<0>& say_chars(std::size_t start_byte, std::size_t end_byte) { say_chars_sub(start_byte, end_byte); return *this; }
#endif // WELP_BITS_INCLUDE_IOSTREAM

		bits() = delete;
		bits(const welp::bits<0>&) = delete;
		welp::bits<0>& operator=(const welp::bits<0>&) = delete;
		bits(welp::bits<0>&&) = delete;
		welp::bits<0>& operator=(welp::bits<0>&&) = delete;
		~bits() = default;

	private:

		unsigned char m_field[1];

		inline unsigned char shift_true(std::size_t bit_offset) const noexcept
		{
			switch (bit_offset)
			{
			case 0: return static_cast<unsigned char>(1); break;
			case 1: return static_cast<unsigned char>(2); break;
			case 2: return static_cast<unsigned char>(4); break;
			case 3: return static_cast<unsigned char>(8); break;
			case 4: return static_cast<unsigned char>(16); break;
			case 5: return static_cast<unsigned char>(32); break;
			case 6: return static_cast<unsigned char>(64); break;
			case 7: return static_cast<unsigned char>(128); break;
			default: return static_cast<unsigned char>(0); break;
			}
		}
		inline unsigned char shift_false(std::size_t bit_offset) const noexcept
		{
			switch (bit_offset)
			{
			case 0: return static_cast<unsigned char>(254); break;
			case 1: return static_cast<unsigned char>(253); break;
			case 2: return static_cast<unsigned char>(251); break;
			case 3: return static_cast<unsigned char>(247); break;
			case 4: return static_cast<unsigned char>(239); break;
			case 5: return static_cast<unsigned char>(223); break;
			case 6: return static_cast<unsigned char>(191); break;
			case 7: return static_cast<unsigned char>(127); break;
			default: return static_cast<unsigned char>(255); break;
			}
		}
		inline unsigned char char_to_uint8_t(char hex) const noexcept
		{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
			switch (hex)
			{
			case '0': return static_cast<unsigned char>(0); break;
			case '1': return static_cast<unsigned char>(1); break;
			case '2': return static_cast<unsigned char>(2); break;
			case '3': return static_cast<unsigned char>(3); break;
			case '4': return static_cast<unsigned char>(4); break;
			case '5': return static_cast<unsigned char>(5); break;
			case '6': return static_cast<unsigned char>(6); break;
			case '7': return static_cast<unsigned char>(7); break;
			case '8': return static_cast<unsigned char>(8); break;
			case '9': return static_cast<unsigned char>(9); break;

			case 'A': return static_cast<unsigned char>(10); break;
			case 'B': return static_cast<unsigned char>(11); break;
			case 'C': return static_cast<unsigned char>(12); break;
			case 'D': return static_cast<unsigned char>(13); break;
			case 'E': return static_cast<unsigned char>(14); break;
			case 'F': return static_cast<unsigned char>(15); break;

			case 'a': return static_cast<unsigned char>(10); break;
			case 'b': return static_cast<unsigned char>(11); break;
			case 'c': return static_cast<unsigned char>(12); break;
			case 'd': return static_cast<unsigned char>(13); break;
			case 'e': return static_cast<unsigned char>(14); break;
			case 'f': return static_cast<unsigned char>(15); break;

			default: return static_cast<unsigned char>(0); break;
			}
#else // WELP_BITS_USE_CHAR_ARITHMETIC
			if (hex < ':') { return static_cast<unsigned char>(hex - '0'); }
			else if (hex < 'G') { return static_cast<unsigned char>(hex - '7'); }
			else { return static_cast<unsigned char>(hex - 'W'); }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
		}
		inline char uint8_t_to_char_lc(unsigned char number) const noexcept
		{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
			switch (number)
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

			case 10: return 'a'; break;
			case 11: return 'b'; break;
			case 12: return 'c'; break;
			case 13: return 'd'; break;
			case 14: return 'e'; break;
			case 15: return 'f'; break;

			default: return '?'; break;
			}
#else // WELP_BITS_USE_CHAR_ARITHMETIC
			if (number < static_cast<unsigned char>(10)) { return reinterpret_cast<char&>(number) + '0'; }
			else { return reinterpret_cast<char&>(number) + 'W'; }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
		}
		inline char uint8_t_to_char_uc(unsigned char number) const noexcept
		{
#ifndef WELP_BITS_USE_CHAR_ARITHMETIC
			switch (number)
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
#else // WELP_BITS_USE_CHAR_ARITHMETIC
			if (number < static_cast<unsigned char>(10)) { return reinterpret_cast<char&>(number) + '0'; }
			else { return reinterpret_cast<char&>(number) + '7'; }
#endif // WELP_BITS_USE_CHAR_ARITHMETIC
		}

#ifdef WELP_BITS_INCLUDE_IOSTREAM
		void say_bits_sub(std::size_t start_bit, std::size_t end_bit) const
		{
			std::cout << ">>>  bit " << start_bit << "  >  " << load_bit(start_bit) << "\n";
			for (std::size_t k = start_bit + 1; k < end_bit; k++)
			{
				std::cout << "     bit " << k << "  >  " << load_bit(k) << "\n";
			}
			std::cout << std::endl;
		}
		void say_hex_sub(std::size_t start_hex, std::size_t end_hex) const
		{
			if (start_hex >= end_hex)
			{
				std::cout << ">>>" << std::endl;
			}
			else
			{
				std::cout << ">>>  hex " << start_hex << "  >  " << load_hex_uc(start_hex)
					<< " :: " << load_bit(4 * start_hex + 3) << load_bit(4 * start_hex + 2)
					<< load_bit(4 * start_hex + 1) << load_bit(4 * start_hex) << "\n";
				for (std::size_t k = start_hex + 1; k < end_hex; k++)
				{
					std::cout << "     hex " << k << "  >  " << load_hex_uc(k)
						<< " :: " << load_bit(4 * k + 3) << load_bit(4 * k + 2)
						<< load_bit(4 * k + 1) << load_bit(4 * k) << "\n";
				}
			}
		}
		void say_bytes_sub(std::size_t start_byte, std::size_t end_byte) const
		{
			if (start_byte >= end_byte)
			{
				std::cout << ">>>" << std::endl;
			}
			else
			{
				std::cout << ">>>  byte " << start_byte << "  >  " << static_cast<unsigned int>(load_byte(start_byte))
					<< " :: " << load_hex_uc(start_byte, true) << load_hex_uc(start_byte, false)
					<< " :: " << load_bit(start_byte, 7) << load_bit(start_byte, 6) << load_bit(start_byte, 5) << load_bit(start_byte, 4)
					<< load_bit(start_byte, 3) << load_bit(start_byte, 2) << load_bit(start_byte, 1) << load_bit(start_byte, 0) << "\n";
				for (std::size_t k = start_byte + 1; k < end_byte; k++)
				{
					std::cout << "     byte " << k << "  >  " << static_cast<unsigned int>(load_byte(k))
						<< " :: " << load_hex_uc(k, true) << load_hex_uc(k, false)
						<< " :: " << load_bit(k, 7) << load_bit(k, 6) << load_bit(k, 5) << load_bit(k, 4)
						<< load_bit(k, 3) << load_bit(k, 2) << load_bit(k, 1) << load_bit(k, 0) << "\n";
				}
				std::cout << std::endl;
			}
		}
		void say_chars_sub(std::size_t start_byte, std::size_t end_byte) const
		{
			if (start_byte >= end_byte)
			{
				std::cout << ">>>" << std::endl;
			}
			else
			{
				std::cout << ">>>  char " << start_byte << "  >  " << load_char(start_byte)
					<< " :: " << load_hex_uc(start_byte, true) << load_hex_uc(start_byte, false)
					<< " :: " << load_bit(start_byte, 7) << load_bit(start_byte, 6) << load_bit(start_byte, 5) << load_bit(start_byte, 4)
					<< load_bit(start_byte, 3) << load_bit(start_byte, 2) << load_bit(start_byte, 1) << load_bit(start_byte, 0) << "\n";
				for (std::size_t k = start_byte + 1; k < end_byte; k++)
				{
					std::cout << "     char " << k << "  >  " << load_char(k)
						<< " :: " << load_hex_uc(k, true) << load_hex_uc(k, false)
						<< " :: " << load_bit(k, 7) << load_bit(k, 6) << load_bit(k, 5) << load_bit(k, 4)
						<< load_bit(k, 3) << load_bit(k, 2) << load_bit(k, 1) << load_bit(k, 0) << "\n";
				}
				std::cout << std::endl;
			}
		}
#endif // WELP_BITS_INCLUDE_IOSTREAM
	};
}


#endif // WELP_BITS_HPP


// welp_bits.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
