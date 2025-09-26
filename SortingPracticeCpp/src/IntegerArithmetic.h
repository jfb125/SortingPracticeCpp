/*
 * FloorLog2.h
 *
 *  Created on: Sep 22, 2025
 *      Author: joe
 */

#ifndef INTEGERARITHMETIC_H_
#define INTEGERARITHMETIC_H_

#include <stdint.h>

/*	**************************************************************	*/
/*	**************************************************************	*/
/*			 			function prototypes							*/
/*	**************************************************************	*/
/*	**************************************************************	*/

/*	returns an integer type that contains the largest integer power of 2 <= num	*/
template <typename T>
T highestPowerOf2(T num);


/*	**************************************************************	*/
/*	**************************************************************	*/
/*			 			function definitions						*/
/*	**************************************************************	*/
/*	**************************************************************	*/

/*
 * template <typename T>
 * T floorLog2(T num);
 *
 * 	returns the logarithm to the base two of
 * 	  the largest power of 2 that is <= to num
 *
 * 	  floorLog2(63) returns 32
 * 	  floorLog2(64) returns 64
 *
 *	algorithm:
 *		sign extend the msbit to the right
 *	      by ORing in a shifted version of num
 *	    mask off the lower bits by subtracting
 *	      a shifted>>1 (/2) version of the sign extended
 *
 *	  'b01_0000 | 'b00_1000	= 'b01_1000
 *	  'b01_1000 | 'b00_0110 = 'b01_1110
 *	  'b01_1110 | 'b00_0001 = 'b01_1111
 *	  'b01_1111 - 'b00_1111 = 'b01_0000
 */

template <typename T>
T highestPowerOf2(T num) {
	if (num < 0) {
		return 0;
	}
	num |= (num>>1);
	num |= (num>>2);	// produces a nibble
	num |= (num>>4);	// produces a byte
	if (sizeof(T) > 1) {
		num |= (num>>8);	// produces 2 bytes
		if (sizeof(T) > 2) {
			num |= (num>>16);	// produces 4 bytes
			if (sizeof(T) > 4) {
				num |= (num>>32);	// produces 8 bytes
			}
		}
	}
	return num - (num>>1);
}




#endif /* INTEGERARITHMETIC_H_ */
