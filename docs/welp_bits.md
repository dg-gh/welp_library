# welp_bits.hpp

welp_flags.hpp provides one class welp::bits<number_of_bits> is a class that can store n booleans packed as 8 booleas per byte.

# Member functions of welp::bits<number_of_bits> B

Template parameter number_of_bits is the number of booleans stored in welp::bit_flags<number_of_bits>.

### Setting, getting and printing booleans

	B.load_bit(n); 

Loads the nth boolean of B.

	B.store_bit(n, false); 

Stores boolean false at the nth position of B.

	B.store_bit(n, true); 

Stores boolean true at the nth position of B.

	B.set_bits(false); 

Sets all the booleans of B as false.

	B.set_bits(true);
	
Sets all the booleans of B as true.

	B.say_bit(); 

Prints all the bits of B.

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

Bitwise operations and, or, xor, not of B1 with B2 (return new objects).

	B.say(); 

Prints the bits of B. Requires the macro WELP_BIT_FLAGS_INCLUDE_IOSTREAM to be defined.

### Reinterpret as bits

	welp::as_bits(A);

Reinterprets any object A as a bits reference of 8 * sizeof(A) bits.

	welp::as_bits<n>(A);

Reinterprets any object A as a bit flags reference of n bits.

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
		buffer.reserve(10000);
	
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