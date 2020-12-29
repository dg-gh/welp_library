# welp_bits.hpp

welp_bits.hpp provides one class welp::bits<number_of_bits> is a class that can store n booleans packed as 8 booleans per byte.

# Member functions of welp::bits<number_of_bits> B

Template parameter number_of_bits is the number of booleans stored in welp::bit_flags<number_of_bits>.

### Setting, getting and printing booleans

	B.load_bit(n); 

Loads the nth boolean of B (B packs 8 booleans in every byte).

	B.store_bit(n, b); 

Stores boolean b at the nth position of B.

	B.store_bit(m, n, b); 

Stores boolean b at the nth position after the mth byte of B.

	B.set_bits(b); 
	
Sets all the booleans of B as b.

	B.say_bits(); 

Prints the bits of B. Requires the macro WELP_BITS_INCLUDE_IOSTREAM to be defined.

	B.capacity_in_bits(); 

Returns the number of bits that can be stored in B.

	B.cpy(A); 

Copies the first bits of A (A can be any class or object) into B.

### Bitwise operations

	B1 &= B2; 
	B1 |= B2; 
	B1 ^= B2; 
	B1.flip();

Bitwise operations and, or, xor, not of B1 with B2 (modifies B1).

	B1 & B2; 
	B1 | B2; 
	B1 ^ B2;
	~B;

Bitwise operations and, or, xor, not of B1 with B2 (returns a new object).

### Setting, getting and printing hexidecimals

	B.load_hex_lc(n); 

Loads the nth hexidecimal (half of a byte) of B and returns a char that prints a number or an lower-case letter representing that hexadecimal (B packs two hex in every byte).

	B.load_hex_uc(n); 

Loads the nth hexidecimal (half of a byte) of B and returns a char that prints a number or an upper-case letter representing that hexadecimal.

	B.store_hex(n, h); 

Stores char h as a hexidecimal at the nth hex position of B. h can be '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f'.

	B.set_hex(h); 

Sets all the hex of B as h.

	B.say_hex(); 

Prints the hexidecimals of B. Requires the macro WELP_BITS_INCLUDE_IOSTREAM to be defined.

	B.capacity_in_hex(); 

Returns the number of hexadecimals that can be stored in B.

### Reinterpret as bits

	welp::as_bits(A);

Reinterprets any object A as a reference of welp::bits<8 * sizeof(A)>.

	welp::as_bits<n>(A);

Reinterprets any object A as a reference of welp::bits<n * sizeof(A)>.

### Code example

	#define WELP_BITS_INCLUDE_IOSTREAM
	#include "welp_bits.hpp"
	#include <string>
	#include <fstream>
	
	
	class vec4
	{
	
	public:

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};
	
	
	int main()
	{
		std::string buffer;
		buffer.reserve(std::size_t(1) << 10);
	
		vec4 A = { .x = 1.3f, .y = 2.3f, .z = -0.999f, .w = 2000.0f };
	
		std::cout << "vec4 initially :  " << A.x << "  " << A.y << "  "
			<< A.z << "  " << A.w << "\n" << std::endl;
	
		{
			std::ofstream filedump;
			filedump.open("dump.bin");
			if (filedump.good())
			{
				welp::bits<8 * sizeof(A)>& ref = welp::as_bits(A);
				buffer.resize(ref.capacity_in_hex());
				for (std::size_t n = 0; n < ref.capacity_in_hex(); n++)
				{
					buffer[n] = ref.load_hex_lc(n);
				}
				filedump << buffer;
			}
			filedump.close();
		}
	
		vec4 B;
	
		{
			std::ifstream filedump;
			filedump.open("dump.bin");
			if (filedump.good())
			{
				welp::bits<8 * sizeof(A)>& ref = welp::as_bits(B);
				buffer.resize(ref.capacity_in_hex());
				std::getline(filedump, buffer);
				for (std::size_t n = 0; n < ref.capacity_in_hex(); n++)
				{
					ref.store_hex(n, buffer[n]);
				}
				ref.say_bytes();
			}
			filedump.close();
		}
	
		std::cout << "vec4 after reconstruction :  " << B.x << "  "
			<< B.y << "  " << B.z << "  " << B.w << std::endl;
	
		std::cin.get();
		return 0;
	}
