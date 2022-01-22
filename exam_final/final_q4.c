// COMP1521 21T2 ... final exam, question 4

#include <stdlib.h>

long
mul (long a, long b_)
{
	unsigned long b = b_;

	// NOTE: Maximum 5 operations allowed
	//       for each multiplication
	// NOTE: Permitted operations:
	//       [x + y, x - y, -x, x << y]

	if (a == 7) {
		// Two operations:
		//        1     2
		return (b << 3) - b;
	}

	if (a == 17) {
	    // (2^4)b + b	    
		return (b << 4) + b; 
	}

	if (a == -3) {
	    // (2^0)b - (2^2)b
		return b - (b << 2);
	}

	if (a == 60) {
	    // (2^6)b - (2^2)b
		return (b << 6) - (b << 2); 
	}

	if (a == -112) {
	    // (2^4)b + (2^7)b 
		return (b << 4) - (b << 7); 
	}

	// Invalid inputs will simply abort.
	abort();
}
