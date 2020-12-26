# welp_flags.hpp

welp_flags.hpp provides two classes :

- welp::bit_flags<number_of_bits> is a class that can store n booleans packed as 8 booleas per byte.
- welp::byte_flags<number_of_bytes> is a class that can store n uint8_t.

# Member functions of welp::bit_flags<number_of_bits> B

Template parameter number_of_bits is the number of booleans stored in welp::bit_flags<number_of_bits>.

### Setting and getting booleans

	B.load(n); 

Loads the nth boolean of B.

	B.store(n, false); 

Stores boolean false at the nth position of B.

	B.store(n, true); 

Stores boolean true at the nth position of B.

	B.set(false); 

Sets all the booleans of B as false.

	B.set(true); 

Sets all the booleans of B as true.

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

	welp::as_bit_flags(A);

Reinterprets any object A as a bit flags reference of 8 * sizeof(A) bits.

	welp::as_bit_flags<n>(A);

Reinterprets any object A as a bit flags reference of n bits.

### Reinterpret as bytes

	welp::as_byte_flags(A);

Reinterprets any object A as a byte flags reference of sizeof(A) bytes.

	welp::as_byte_flags<n>(A);

Reinterprets any object A as a byte flags reference of n bytes.
