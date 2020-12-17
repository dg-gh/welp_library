# welp_bit_flags.hpp

welp_bit_flags.hpp provides one class :

- welp::bit_flags<N> is a class that can store n booleans packed as 8 booleas per byte.

# Member functions of welp::bit_flags<N> B

Template parameter N is the number of booleans stored in welp::bit_flags<N>.

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

Copies the first bits of A into B.

	B.flip(); 

Flips the bits of B.

	B.say(); 

Prints the bits of B.
