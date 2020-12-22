# welp_bit_flags.hpp

welp_bit_flags.hpp provides one class :

- welp::bit_flags<number_of_bits> is a class that can store n booleans packed as 8 booleas per byte.

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

### Reinterpret as bits

	A.as_bit_flags();

Reinterprets any object A as a bit flag reference of 8 * sizeof(A) bits.

	A.as_bit_flags<n>();

Reinterprets any object A as a bit flag refence of n bits.

### Printing the bits

	B.say(); 

Prints the bits of B. Requires the macro WELP_BIT_FLAGS_INCLUDE_IOSTREAM to be defined.
