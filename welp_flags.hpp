// welp_flags.h - last update : 27 / 12 / 2020
// License <http://unlicense.org/> (statement below at the end of the file)


#ifndef WELP_FLAGS_H
#define WELP_FLAGS_H


////// INCLUDES //////

#include <cstdlib>
#include <cstdint>
#include <cstring>


// include all in one line with #define WELP_FLAGS_INCLUDE_ALL
#if defined(WELP_FLAGS_INCLUDE_ALL) || defined(WELP_ALWAYS_INCLUDE_ALL)
#ifndef WELP_FLAGS_INCLUDE_IOSTREAM
#define WELP_FLAGS_INCLUDE_IOSTREAM
#endif
#endif // WELP_FLAGS_INCLUDE_ALL


#if defined(WELP_ALWAYS_DEBUG_MODE) && !defined(WELP_FLAGS_DEBUG_MODE)
#define WELP_FLAGS_DEBUG_MODE
#endif // WELP_ALWAYS_DEBUG_MODE

#ifdef WELP_FLAGS_DEBUG_MODE
#include <cassert>
#ifndef WELP_FLAGS_INCLUDE_IOSTREAM
#define WELP_FLAGS_INCLUDE_IOSTREAM
#endif
#endif // WELP_FLAGS_DEBUG_MODE

#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
#include <iostream>
#endif // WELP_FLAGS_INCLUDE_IOSTREAM


////// DESCRIPTIONS //////

namespace welp
{
	template <std::size_t bits> class flags
	{

	public:

		inline bool load_bit(std::size_t bit_offset) const noexcept;
		inline welp::flags<bits>& store_bit(std::size_t bit_offset, bool flag) noexcept;
		inline bool load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept;
		inline welp::flags<bits>& store_bit(std::size_t byte_offset, std::size_t bit_offset, bool flag) noexcept;

		inline char load_hex_lc(std::size_t hex_offset) const noexcept;
		inline char load_hex_uc(std::size_t hex_offset) const noexcept;
		inline flags<bits>& store_hex(std::size_t hex_offset, char hex) noexcept;
		inline char load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept;
		inline char load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept;
		inline flags<bits>& store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept;

		inline std::uint8_t load_byte(std::size_t byte_offset) const noexcept;
		inline flags<bits>& store_byte(std::size_t byte_offset, std::uint8_t number) noexcept;
		inline const std::uint8_t& operator[](std::size_t byte_offset) const noexcept;
		inline std::uint8_t& operator[](std::size_t byte_offset) noexcept;

		constexpr const std::uint8_t* data() const noexcept { return static_cast<const std::uint8_t*>(field); }
		constexpr std::uint8_t* data() noexcept { return static_cast<std::uint8_t*>(field); }


		// member functions below are unavailable when template parameter bits == 0

		inline welp::flags<bits>& set_bits(bool bit) noexcept;
		inline welp::flags<bits>& set_hex(char hex) noexcept;
		inline welp::flags<bits>& set_bytes(std::uint8_t number) noexcept;

		constexpr std::uint8_t* begin() noexcept { return static_cast<std::uint8_t*>(field); }
		constexpr const std::uint8_t* cbegin() const noexcept { return static_cast<const std::uint8_t*>(field); }
		constexpr std::uint8_t* end() noexcept { return static_cast<std::uint8_t*>(field) + ((bits + ((8 - (bits & 7)) & 7)) >> 3); }
		constexpr const std::uint8_t* cend() const noexcept { return static_cast<const std::uint8_t*>(field) + ((bits + ((8 - (bits & 7)) & 7)) >> 3); }

		template <class Ty> inline welp::flags<bits>& cpy(const Ty& rhs) noexcept;

		inline welp::flags<bits>& operator&=(const welp::flags<bits>& rhs) noexcept;
		inline welp::flags<bits>& operator|=(const welp::flags<bits>& rhs) noexcept;
		inline welp::flags<bits>& operator^=(const welp::flags<bits>& rhs) noexcept;
		inline welp::flags<bits>& flip() noexcept;

		constexpr std::size_t size_in_bits() const noexcept { return bits; }
		constexpr std::size_t size_in_hex() const noexcept { return bits >> 2; }
		constexpr std::size_t size_in_bytes() const noexcept { return bits >> 3; }
		constexpr std::size_t size_in_memory() const noexcept { return (bits + ((8 - (bits & 7)) & 7)) >> 3; }


#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
		const welp::flags<bits>& say_bits() const;
		welp::flags<bits>& say_bits();
		const welp::flags<bits>& say_hex() const;
		welp::flags<bits>& say_hex();
		const welp::flags<bits>& say_bytes() const;
		welp::flags<bits>& say_bytes();
#endif // WELP_FLAGS_INCLUDE_IOSTREAM


		// constructor, copy ops and move ops are deleted when template parameter bits == 0,
		// welp::flags<0> should only exist as a reference

		flags() noexcept;
		flags(const welp::flags<bits>&) noexcept = default;
		welp::flags<bits>& operator=(const welp::flags<bits>&) noexcept;
		flags(welp::flags<bits>&&) noexcept = default;
		welp::flags<bits>& operator=(welp::flags<bits>&&) noexcept;
		~flags() = default;

		template <std::size_t bits2>
		friend inline bool operator==(const welp::flags<bits2>&, const welp::flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline bool operator!=(const welp::flags<bits2>&, const welp::flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::flags<bits2> operator&(const welp::flags<bits2>&, const welp::flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::flags<bits2> operator|(const welp::flags<bits2>&, const welp::flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::flags<bits2> operator^(const welp::flags<bits2>&, const welp::flags<bits2>&) noexcept;
		template <std::size_t bits2>
		friend inline welp::flags<bits2> operator~(const welp::flags<bits2>&) noexcept;

	private:

		std::uint8_t field[(bits + ((8 - (bits & 7)) & 7)) >> 3];

		inline std::uint8_t shift_true(std::size_t bit_offset) const noexcept;
		inline std::uint8_t shift_false(std::size_t bit_offset) const noexcept;
		inline std::uint8_t bitmask_true(std::size_t digits) const noexcept;
		inline std::uint8_t bitmask_false(std::size_t digits) const noexcept;
		inline std::uint8_t char_to_uint8_t(char hex) const noexcept;
		inline char uint8_t_to_char_lc(std::uint8_t number) const noexcept;
		inline char uint8_t_to_char_uc(std::uint8_t number) const noexcept;

		void say_bits_sub() const;
		void say_hex_sub() const;
		void say_bytes_sub() const;
	};

	template <std::size_t bits> inline bool operator==(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept;
	template <std::size_t bits> inline bool operator!=(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept;

	template <std::size_t bits> inline welp::flags<bits> operator&(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::flags<bits> operator|(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::flags<bits> operator^(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept;
	template <std::size_t bits> inline welp::flags<bits> operator~(const welp::flags<bits>& A) noexcept;

	template <class Ty> inline const welp::flags<8 * sizeof(Ty)>& as_flags(const Ty& rhs) noexcept;
	template <class Ty> inline welp::flags<8 * sizeof(Ty)>& as_flags(Ty& rhs) noexcept;
	template <std::size_t bits, class Ty> inline const welp::flags<bits>& as_flags(const Ty& rhs) noexcept;
	template <std::size_t bits, class Ty> inline welp::flags<bits>& as_flags(Ty& rhs) noexcept;
}


////// IMPLEMENTATIONS //////

template <std::size_t bits>
inline bool welp::flags<bits>::load_bit(std::size_t bit_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	return (field[bit_offset >> 3] & shift_true(bit_offset & 7)) != static_cast<std::uint8_t>(0);
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::store_bit(std::size_t bit_offset, bool flag) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(bit_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
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
inline bool welp::flags<bits>::load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset + bit_offset < bits);
	assert(bit_offset < 8);
#endif // WELP_FLAGS_DEBUG_MODE
	return (field[byte_offset] & shift_true(bit_offset)) != static_cast<std::uint8_t>(0);
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::store_bit(std::size_t byte_offset, std::size_t bit_offset, bool flag) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset + bit_offset < bits);
	assert(bit_offset < 8);
#endif // WELP_FLAGS_DEBUG_MODE
	if (flag)
	{
		field[byte_offset] |= shift_true(bit_offset);
		return *this;
	}
	else
	{
		field[byte_offset] &= shift_false(bit_offset);
		return *this;
	}
}

template <std::size_t bits>
inline char welp::flags<bits>::load_hex_lc(std::size_t hex_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
		(field[hex_offset >> 1] >> 4) : (field[hex_offset >> 1] & static_cast<std::uint8_t>(15));
	return uint8_t_to_char_lc(temp);
}

template <std::size_t bits>
inline char welp::flags<bits>::load_hex_uc(std::size_t hex_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
		(field[hex_offset >> 1] >> 4) : (field[hex_offset >> 1] & static_cast<std::uint8_t>(15));
	return uint8_t_to_char_uc(temp);
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::store_hex(std::size_t hex_offset, char hex) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(4 * hex_offset + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = char_to_uint8_t(hex);
	std::size_t byte_offset = hex_offset >> 1;
	if ((hex_offset & static_cast<std::size_t>(1)) != 0)
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(15)) | (temp << 4);
	}
	else
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(240)) | temp;
	}

	return *this;
}

template <std::size_t bits>
inline char welp::flags<bits>::load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = (upper_half_byte) ? (field[byte_offset] >> 4) : (field[byte_offset] & static_cast<std::uint8_t>(15));
	return uint8_t_to_char_lc(temp);
}

template <std::size_t bits>
inline char welp::flags<bits>::load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = (upper_half_byte) ? (field[byte_offset] >> 4) : (field[byte_offset] & static_cast<std::uint8_t>(15));
	return uint8_t_to_char_uc(temp);
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset + 4 * static_cast<std::size_t>(upper_half_byte) + 3 < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	std::uint8_t temp = char_to_uint8_t(hex);

	if (upper_half_byte)
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(15)) | (temp << 4);
	}
	else
	{
		field[byte_offset] = (field[byte_offset] & static_cast<std::uint8_t>(240)) | temp;
	}

	return *this;
}

template <std::size_t bits>
inline std::uint8_t welp::flags<bits>::load_byte(std::size_t byte_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	return field[byte_offset];
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::store_byte(std::size_t byte_offset, std::uint8_t number) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	field[byte_offset] = number;
	return *this;
}

template <std::size_t bits>
inline const std::uint8_t& welp::flags<bits>::operator[](std::size_t byte_offset) const noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	return field[byte_offset];
}

template <std::size_t bits>
inline std::uint8_t& welp::flags<bits>::operator[](std::size_t byte_offset) noexcept
{
#ifdef WELP_FLAGS_DEBUG_MODE
	assert(8 * byte_offset < bits);
#endif // WELP_FLAGS_DEBUG_MODE
	return field[byte_offset];
}


template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::set_bits(bool bit) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	if (bit)
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

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::set_hex(char hex) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	std::uint8_t temp = char_to_uint8_t(hex);
	if ((bits & 7) > 3)
	{
		*(static_cast<std::uint8_t*>(field) + bytes) &= bitmask_false(4);
		*(static_cast<std::uint8_t*>(field) + bytes) |= temp;
	}
	temp |= (temp << 4);
	std::memset(static_cast<std::uint8_t*>(field), temp, bytes);
	return *this;
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::set_bytes(std::uint8_t number) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	std::memset(static_cast<std::uint8_t*>(field), number, bytes);
	return *this;
}



template <std::size_t bits> template <class Ty>
inline welp::flags<bits>& welp::flags<bits>::cpy(const Ty& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	const std::uint8_t* rhs_ptr = static_cast<const std::uint8_t*>(static_cast<const void*>(&rhs));
	std::memcpy(static_cast<std::uint8_t*>(field), rhs_ptr, bytes);
	if (remainder_bits != 0)
	{
		field[bytes] |= (*(rhs_ptr + bytes) & bitmask_true(remainder_bits));
	}
	return *this;
}


template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::operator&=(const welp::flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] &= rhs.field[k];
	}
	if (remainder_bits != 0)
	{
		field[bytes] &= (rhs.field[bytes] | bitmask_false(remainder_bits));
	}
	return *this;
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::operator|=(const welp::flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] |= rhs.field[k];
	}
	if (remainder_bits != 0)
	{
		field[bytes] |= (rhs.field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::operator^=(const welp::flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] ^= rhs.field[k];
	}
	if (remainder_bits != 0)
	{
		field[bytes] ^= (rhs.field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::flip() noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	constexpr std::uint8_t ones = -static_cast<std::uint8_t>(1);
	for (std::size_t k = 0; k < bytes; k++)
	{
		field[k] ^= ones;
	}
	if (remainder_bits != 0)
	{
		field[bytes] ^= bitmask_true(remainder_bits);
	}
	return *this;
}


#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bits>
const welp::flags<bits>& welp::flags<bits>::say_bits() const
{
	say_bits_sub(); return *this;
}

template <std::size_t bits>
welp::flags<bits>& welp::flags<bits>::say_bits()
{
	say_bits_sub(); return *this;
}

template <std::size_t bits>
const welp::flags<bits>& welp::flags<bits>::say_hex() const
{
	say_hex_sub(); return *this;
}

template <std::size_t bits>
welp::flags<bits>& welp::flags<bits>::say_hex()
{
	say_hex_sub(); return *this;
}

template <std::size_t bits>
const welp::flags<bits>& welp::flags<bits>::say_bytes() const
{
	say_bytes_sub(); return *this;
}

template <std::size_t bits>
welp::flags<bits>& welp::flags<bits>::say_bytes()
{
	say_bytes_sub(); return *this;
}
#endif // WELP_FLAGS_INCLUDE_IOSTREAM


template <std::size_t bits>
welp::flags<bits>::flags() noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memset(static_cast<std::uint8_t*>(field), static_cast<std::uint8_t>(0), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<std::uint8_t*>(field) + bytes) &= bitmask_false(remainder_bits);
	}
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::operator=(const welp::flags<bits>& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memcpy(static_cast<std::uint8_t*>(field), static_cast<const std::uint8_t*>(rhs.field), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<std::uint8_t*>(field) + bytes) &= bitmask_false(remainder_bits);
		*(static_cast<std::uint8_t*>(field) + bytes)& bitmask_true(remainder_bits);
	}
	return *this;
}

template <std::size_t bits>
inline welp::flags<bits>& welp::flags<bits>::operator=(welp::flags<bits>&& rhs) noexcept
{
	constexpr std::size_t bytes = bits >> 3;
	constexpr std::size_t remainder_bits = bits & 7;
	std::memcpy(static_cast<std::uint8_t*>(field), static_cast<const std::uint8_t*>(rhs.field), bytes);
	if (remainder_bits != 0)
	{
		*(static_cast<std::uint8_t*>(field) + bytes) &= bitmask_false(remainder_bits);
		*(static_cast<std::uint8_t*>(field) + bytes) |= (rhs.field[bytes] & bitmask_true(remainder_bits));
	}
	return *this;
}


template <std::size_t bits>
inline std::uint8_t welp::flags<bits>::shift_true(std::size_t bit_offset) const noexcept
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
inline std::uint8_t welp::flags<bits>::shift_false(std::size_t bit_offset) const noexcept
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
inline std::uint8_t welp::flags<bits>::bitmask_true(std::size_t digits) const noexcept
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
inline std::uint8_t welp::flags<bits>::bitmask_false(std::size_t digits) const noexcept
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

template <std::size_t bits>
inline std::uint8_t welp::flags<bits>::char_to_uint8_t(char hex) const noexcept
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

template <std::size_t bits>
inline char welp::flags<bits>::uint8_t_to_char_uc(std::uint8_t number) const noexcept
{
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
}

template <std::size_t bits>
inline char welp::flags<bits>::uint8_t_to_char_lc(std::uint8_t number) const noexcept
{
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
}


#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bits>
void welp::flags<bits>::say_bits_sub() const
{
	std::cout << ">>>  bit 0  >  " << load_bit(0) << "\n";
	for (std::size_t k = 1; k < bits; k++)
	{
		std::cout << "     bit " << k << "  >  " << load_bit(k) << "\n";
	}
	std::cout << std::endl;
}

template <std::size_t bits>
void welp::flags<bits>::say_hex_sub() const
{
	constexpr std::size_t hex = bits >> 2;
	if (bits < 4)
	{
		std::cout << ">>>  size smaller than a hexadecimal" << std::endl;
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

template <std::size_t bits>
void welp::flags<bits>::say_bytes_sub() const
{
	constexpr std::size_t bytes = bits >> 3;
	if (bits < 8)
	{
		std::cout << ">>>  size smaller than a byte" << std::endl;
	}
	else
	{
		std::cout << ">>>  byte 0  >  " << static_cast<unsigned int>(field[0])
			<< " :: " << load_hex_uc(0, true) << load_hex_uc(0, false)
			<< " :: " << load_bit(0, 7) << load_bit(0, 6) << load_bit(0, 5) << load_bit(0, 4)
			<< load_bit(0, 3) << load_bit(0, 2) << load_bit(0, 1) << load_bit(0, 0) << "\n";
		for (std::size_t k = 1; k < bytes; k++)
		{
			std::cout << "     byte " << k << "  >  " << static_cast<unsigned int>(field[k])
				<< " :: " << load_hex_uc(k, true) << load_hex_uc(k, false)
				<< " :: " << load_bit(k, 7) << load_bit(k, 6) << load_bit(k, 5) << load_bit(k, 4)
				<< load_bit(k, 3) << load_bit(k, 2) << load_bit(k, 1) << load_bit(k, 0) << "\n";
		}
		std::cout << std::endl;
	}
}
#endif // WELP_FLAGS_INCLUDE_IOSTREAM


namespace welp
{
	template <std::size_t bits> inline bool operator==(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = bits >> 3;
		constexpr std::size_t remainder_bits = bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return false; }
		}
		if (remainder_bits != 0)
		{
			std::uint8_t bitmask = A.bitmask_true(remainder_bits);
			if ((A.field[bytes] & bitmask) != (B.field[bytes] & bitmask)) { return false; }
			else { return true; }
		}
		else { return true; }
	}
	template <std::size_t bits> inline bool operator!=(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = bits >> 3;
		constexpr std::size_t remainder_bits = bits & 7;
		for (std::size_t k = 0; k < bytes; k++)
		{
			if (A.field[k] != B.field[k]) { return true; }
		}
		if (remainder_bits != 0)
		{
			std::uint8_t bitmask = A.bitmask_true(remainder_bits);
			if ((A.field[bytes] & bitmask) != (B.field[bytes] & bitmask)) { return true; }
			else { return true; }
		}
		else { return true; }
	}

	template <std::size_t bits> inline welp::flags<bits> operator&(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] & B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::flags<bits> operator|(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] | B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::flags<bits> operator^(const welp::flags<bits>& A, const welp::flags<bits>& B) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = A.field[k] ^ B.field[k];
		}
		return C;
	}
	template <std::size_t bits> inline welp::flags<bits> operator~(const welp::flags<bits>& A) noexcept
	{
		constexpr std::size_t bytes = (bits + ((8 - (bits & 7)) & 7)) >> 3;
		welp::flags<bits> C;
		for (std::size_t k = 0; k < bytes; k++)
		{
			C.field[k] = ~A.field[k];
		}
		return C;
	}

	template <class Ty> inline const welp::flags<8 * sizeof(Ty)>& as_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::flags<8 * sizeof(Ty)>&>(rhs);
	}
	template <class Ty> inline welp::flags<8 * sizeof(Ty)>& as_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::flags<8 * sizeof(Ty)>&>(rhs);
	}
	template <std::size_t bits, class Ty> inline const welp::flags<bits>& as_flags(const Ty& rhs) noexcept
	{
		return reinterpret_cast<const welp::flags<bits>&>(rhs);
	}
	template <std::size_t bits, class Ty> inline welp::flags<bits>& as_flags(Ty& rhs) noexcept
	{
		return reinterpret_cast<welp::flags<bits>&>(rhs);
	}
}


#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
template <std::size_t bits> std::ostream& operator<<(std::ostream& out, const welp::flags<bits>& A)
{
	A.say_bits(); return out;
}
#endif // WELP_FLAGS_INCLUDE_IOSTREAM


namespace welp
{
	template <> class flags<0>
	{

	public:

		inline bool load_bit(std::size_t bit_offset) const noexcept
		{
			return (*(static_cast<const std::uint8_t*>(field) + (bit_offset >> 3)) & shift_true(bit_offset & 7)) != static_cast<std::uint8_t>(0);
		}
		inline welp::flags<0>& store_bit(std::size_t bit_offset, bool flag) noexcept
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
		inline bool load_bit(std::size_t byte_offset, std::size_t bit_offset) const noexcept
		{
#ifdef WELP_FLAGS_DEBUG_MODE
			assert(bit_offset < 8);
#endif // WELP_FLAGS_DEBUG_MODE
			return (*(static_cast<const std::uint8_t*>(field) + byte_offset) & shift_true(bit_offset)) != static_cast<std::uint8_t>(0);
		}
		inline welp::flags<0>& store_bit(std::size_t byte_offset, std::size_t bit_offset, bool flag) noexcept
		{
#ifdef WELP_FLAGS_DEBUG_MODE
			assert(bit_offset < 8);
#endif // WELP_FLAGS_DEBUG_MODE
			if (flag)
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) |= shift_true(bit_offset);
				return *this;
			}
			else
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) &= shift_false(bit_offset);
				return *this;
			}
		}

		inline char load_hex_lc(std::size_t hex_offset) const noexcept
		{
			std::uint8_t temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
				(*(static_cast<const std::uint8_t*>(field) + (hex_offset >> 1)) >> 4)
				: (*(static_cast<const std::uint8_t*>(field) + (hex_offset >> 1)) & static_cast<std::uint8_t>(15));
			return uint8_t_to_char_lc(temp);
		}
		inline char load_hex_uc(std::size_t hex_offset) const noexcept
		{
			std::uint8_t temp = ((hex_offset & static_cast<std::size_t>(1)) != 0) ?
				(*(static_cast<const std::uint8_t*>(field) + (hex_offset >> 1)) >> 4)
				: (*(static_cast<const std::uint8_t*>(field) + (hex_offset >> 1)) & static_cast<std::uint8_t>(15));
			return uint8_t_to_char_uc(temp);
		}
		inline welp::flags<0>& store_hex(std::size_t hex_offset, char hex) noexcept
		{
			std::uint8_t temp = char_to_uint8_t(hex);
			std::size_t byte_offset = hex_offset >> 1;
			if ((hex_offset & static_cast<std::size_t>(1)) != 0)
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) = (*(static_cast<std::uint8_t*>(field) + byte_offset)
					& static_cast<std::uint8_t>(15)) | (temp << 4);
			}
			else
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) = (*(static_cast<std::uint8_t*>(field) + byte_offset)
					& static_cast<std::uint8_t>(240)) | temp;
			}

			return *this;
		}
		inline char load_hex_lc(std::size_t byte_offset, bool upper_half_byte) const noexcept
		{
			std::uint8_t temp = (upper_half_byte) ?
				(*(static_cast<const std::uint8_t*>(field) + byte_offset) >> 4)
				: (*(static_cast<const std::uint8_t*>(field) + byte_offset) & static_cast<std::uint8_t>(15));
			return uint8_t_to_char_lc(temp);
		}
		inline char load_hex_uc(std::size_t byte_offset, bool upper_half_byte) const noexcept
		{
			std::uint8_t temp = (upper_half_byte) ?
				(*(static_cast<const std::uint8_t*>(field) + byte_offset) >> 4)
				: (*(static_cast<const std::uint8_t*>(field) + byte_offset) & static_cast<std::uint8_t>(15));
			return uint8_t_to_char_uc(temp);
		}
		inline welp::flags<0>& store_hex(std::size_t byte_offset, bool upper_half_byte, char hex) noexcept
		{
			std::uint8_t temp = char_to_uint8_t(hex);

			if (upper_half_byte)
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) = (*(static_cast<std::uint8_t*>(field) + byte_offset)
					& static_cast<std::uint8_t>(15)) | (temp << 4);
			}
			else
			{
				*(static_cast<std::uint8_t*>(field) + byte_offset) = (*(static_cast<std::uint8_t*>(field) + byte_offset)
					& static_cast<std::uint8_t>(240)) | temp;
			}

			return *this;
		}

		inline std::uint8_t load_byte(std::size_t byte_offset) const noexcept
		{
			return *(static_cast<const std::uint8_t*>(field) + byte_offset);
		}
		inline welp::flags<0>& store_byte(std::size_t byte_offset, std::uint8_t number) noexcept
		{
			*(static_cast<std::uint8_t*>(field) + byte_offset) = number;
			return *this;
		}
		inline const std::uint8_t& operator[](std::size_t byte_offset) const noexcept
		{
			return *(static_cast<const std::uint8_t*>(field) + byte_offset);
		}
		inline std::uint8_t& operator[](std::size_t byte_offset) noexcept
		{
			return *(static_cast<std::uint8_t*>(field) + byte_offset);
		}

		constexpr const std::uint8_t* data() const noexcept { return static_cast<const std::uint8_t*>(field); }
		constexpr std::uint8_t* data() noexcept { return static_cast<std::uint8_t*>(field); }

#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
		const welp::flags<0>& say_bits(std::size_t bits) const { say_bits_sub(0, bits); return *this; }
		welp::flags<0>& say_bits(std::size_t bits) { say_bits_sub(0, bits); return *this; }
		const welp::flags<0>& say_bits(std::size_t start_bit, std::size_t end_bit) const { say_bits_sub(start_bit, end_bit); return *this; }
		welp::flags<0>& say_bits(std::size_t start_bit, std::size_t end_bit) { say_bits_sub(start_bit, end_bit); return *this; }

		const welp::flags<0>& say_hex(std::size_t hex) const { say_hex_sub(0, hex); return *this; }
		welp::flags<0>& say_hex(std::size_t hex) { say_hex_sub(0, hex); return *this; }
		const welp::flags<0>& say_hex(std::size_t start_hex, std::size_t end_hex) const { say_hex_sub(start_hex, end_hex); return *this; }
		welp::flags<0>& say_hex(std::size_t start_hex, std::size_t end_hex) { say_hex_sub(start_hex, end_hex); return *this; }

		const welp::flags<0>& say_bytes(std::size_t bytes) const { say_bytes_sub(0, bytes); return *this; }
		welp::flags<0>& say_bytes(std::size_t bytes) { say_bytes_sub(0, bytes); return *this; }
		const welp::flags<0>& say_bytes(std::size_t start_byte, std::size_t end_byte) const { say_bytes_sub(start_byte, end_byte); return *this; }
		welp::flags<0>& say_bytes(std::size_t start_byte, std::size_t end_byte) { say_bytes_sub(start_byte, end_byte); return *this; }
#endif // WELP_FLAGS_INCLUDE_IOSTREAM

		flags() = delete;
		flags(const welp::flags<0>&) = delete;
		welp::flags<0>& operator=(const welp::flags<0>&) = delete;
		flags(welp::flags<0>&&) = delete;
		welp::flags<0>& operator=(welp::flags<0>&&) = delete;
		~flags() = default;

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
		inline char uint8_t_to_char_lc(std::uint8_t number) const noexcept
		{
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
		}
		inline char uint8_t_to_char_uc(std::uint8_t number) const noexcept
		{
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
		}

#ifdef WELP_FLAGS_INCLUDE_IOSTREAM
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
#endif // WELP_FLAGS_INCLUDE_IOSTREAM
	};
}


#endif // WELP_FLAGS_H


// welp_flags.hpp
// 
// This is free software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software for any purpose and by any means.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY IN CONNECTION WITH THE SOFTWARE.
