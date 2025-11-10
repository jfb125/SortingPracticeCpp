/*
 * BlockOperationsMergeRotation.h
 *
 *  Created on: Nov 3, 2025
 *      Author: joe
 */

#ifndef BLOCKOPERATIONSMERGEROTATION_H_
#define BLOCKOPERATIONSMERGEROTATION_H_

//	for debugging
#include <iostream>
#include <iomanip>
#include <string>

#include "BlockSortDataTypes.h"		// Descriptor datatype
#include "SortingUtilities.h"		// swap, debug utilities
#include "BlockOperations.h"

/*
 * 	The order of function definitions in this file is:
 * 	BINARY_SEARCH
 * 	RIGHT_TO_LEFT
 * 	HYBRID
 * to reflect that the HYBRID method is best comprehended by studying
 * the specific methods.
 *
 *	Discussion:
 *
 * Merging blocks by rotating elements into place by identifying
 * spans of values [bm:bn] that fit between two given elements ai, aj
 * in the other half of the array.
 *
 * Finding a span [bm:bn] and it's locations ai, aj is done in one of
 * two ways:
 * 	BINARY SEARCH - uses a binary search to identify the boundaries
 * 	RIGHT_TO_LEFT - starts at the right end of each block and searches
 * 		to the left to identify the boundaries
 *
 *	In the following discussion consider adjacent blocks separated by "||":
 *		two blocks slightly interleaved  { A, B, E, F || C, D, G, H }
 *		two blocks highly interleaves    { A, C, E, G || B, D, F, H }
 *		and lg(n) refers to the log to the base 2 of (n)
 *
 * 	The binary search method requires 'lg(q)' comparisons each time
 * 	a search for a boundary is performed.  The number 'q' gets smaller
 * 	each time the un-merged portion of the blocks get smaller.  However,
 * 	in a highly interleaved set of blocks, the binary search will have
 * 	to be done many times.
 *
 * 	The right-to-left method moves step-wise through the block, which
 * 	can have as many as (n) compares to move through both blocks.
 *
 *	HYBRID - uses a binary search to identify the locations of the first
 *		boundaries, which may be more efficient than stepping through
 *		the whole array.  Successive searches for the boundaries are
 *		done step wise to the left which will be more efficient as long
 *		as the values are somewhat interleaved .
 */

namespace BlockOperations {


	/*	Forward declaration of special rotate function that does not require
	 *	the blocks to be contiguous */

	template <typename T>
	void rotateArrayElementsRight(T* array,
								  array_size_t block_1_start,
								  array_size_t block_1_end,
								  array_size_t block_2_start,
								  array_size_t block_2_end,
								  SortMetrics *metrics);

	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*																					*/
	/*						MERGE BLOCKS BY ROTATION BINARY SEARCH						*/
	/*																					*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/


	/*	Merge blocks using binary searches to identify spans to rotate.
	 * Returns the final position of the rightmost element from b after
	 * b is merged.
	 *
	 * In the following discussion,
	 * 	last(start,end,key) 	refers to a binary search that
	 * 	 							returns the left-most element
	 * 	 							that is > the search term
	 * 	rotate(span_start, span_end, rotate_count)
	 * 							refers to the rotating all element right
	 * 	As						refers to the least (leftmost) element
	 * 								from the A_Block that has not been placed
	 * 	Ae						refers to the greatest (rightmost) element
	 * 								that has not been placed
	 * 	Bs						refers to the least (leftmost) element
	 * 								from the B_Block that has nto been placed
	 * 	Be						is always the last element in B
	 * 	Ss						is the start of the span to be rotated
	 * 	Se						is the end of the span to be rotated
	 * 	Rc						is the count to rotate the span right
	 *
	 * Consider the following array:
	 *[       A Block        ][    B Block        ]
	 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14	| As Ae Bs Be | Ss Se Rc
	 * A  G  H  I  M  N  O  P  B  C  D  E  F  J  K	  0  7  8  14   1  13  5
	 * 		span_start 	= last(As,Ae,[Bs]='B')		=  1
	 * 		span_end	= last(Bs,Be,[Ss]='G') - 1	= 12
	 * 		rotate_count= span_end - b_start + 1 	= (12 -  8 +  1) = 5
	 * 	After
	 * 		rotate(span_start, span_end, rotate_count)
	 * 		As = span_start + rotate_count			=  1 + 5 	= 6
	 * 		Ae = a_end + rotate_count				=  7 + 5 	= 12
	 * 		Bs = span_end + 1						= 12 + 1 	= 13
	 *
	 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14	| As Ae Bs Be | Ss Se Rc
	 * A  B  C  D  E  F  G  H  I  M  N  O  P  J  K	  6  12 13 14   9  12  2
	 *
	 *		span_start 	= last(As,Ae,[Bs]='J')		=  9
	 *		span_end	= last(As,Ae,[Ss]='M') - 1	= 14
	 *		rotate_count= span_end - b_start + 1	= (14 - 13 + 1) = 2
	 *	After
	 *		rotate(span_start, span_end, rotate_count)
	 *		As = span_start + rotate_count			=  9 + 2 = 11
	 *		Ae = a_end + rotate_count				= 12 + 2 = 14
	 *		Bs = span_end + 1						= 14 + 1 = 15  out of bound
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_BinarySearch(
													T* 			 array,
													array_size_t block_1_start,
													array_size_t block_1_end,
													array_size_t block_2_start,
													array_size_t block_2_end,
													SortMetrics *metrics) {
		//	debuggoing variables
		constexpr bool debug_verbose= false;
		bool announce_initial_array = true;
		bool a_rotation_occurred	= false;
		std::stringstream initial_array_msg;

		array_size_t b_max_pos  		= block_2_end;

		if (block_1_end - block_1_start < 0)
			return b_max_pos;
		if (block_2_end - block_2_start < 0)
			return b_max_pos;
		if (block_1_end != block_2_end-1)
			return b_max_pos;

		array_size_t a_unmerged_start	= block_1_start;
		array_size_t a_unmerged_end		= block_1_end;
		array_size_t b_unmerged_start	= block_2_start;
		array_size_t b_unmerged_end		= block_2_end;

		if (debug_verbose) {
			array_size_t dbg_size = b_unmerged_end-a_unmerged_start+1;
			initial_array_msg << SortingUtilities::arrayElementsToString(array, dbg_size)
							  << " initial array state"
							  << std::endl;
		}

		//	There are two ways can end:
		//	All of the remaining b span is to the left of the a_span
		//	All of the b_span
		while (b_unmerged_start <= block_2_end &&
			   b_unmerged_end > a_unmerged_end) {
			std::stringstream dbg_msg;
			array_size_t first_a_greater_than_b;
			array_size_t first_b_greater_than_a;
			array_size_t span_start;
			array_size_t span_end;
			array_size_t rotate_count;
			first_a_greater_than_b =
				SortingUtilities::binarySearchLastElement(array,
														  a_unmerged_start, a_unmerged_end,
														  array[b_unmerged_start],
														  metrics);
			// If all of a is less than of equal to b_start, we are done
			if (first_a_greater_than_b == b_unmerged_start) {
				break;
			}
			span_start = first_a_greater_than_b;
			first_b_greater_than_a =
				SortingUtilities::binarySearchLastElement(array,
														  b_unmerged_start, b_unmerged_end,
														  array[span_start],
														  metrics);
			//	the span of A values that can be rotated right ends at the
			//	element immediately before the first B value that is
			//	greater than the a_value at the start of the span
			//	0 1 2 3 4   5 6 7 8 9
			//	A B G H I | D E F J K	a[G] >_b_st[D] = 2  b[J] > a[G] = 8
			//							span [2:7] [G:F], rotate 8-5 = 3
			span_end 	= first_b_greater_than_a - 1;
			if (span_end == span_start)
				break;
			rotate_count= first_b_greater_than_a - b_unmerged_start;
			if (span_end == b_max_pos) {
				b_max_pos = b_max_pos 				// current position
							+ rotate_count			// moved to the right
							- (span_end-span_start+1);// modulo span size
			}
			SortingUtilities::rotateArrayElementsRight(array,
													   span_start, span_end,
													   rotate_count, metrics);
			a_rotation_occurred = true;
			if (debug_verbose) {
				if (announce_initial_array) {
					std::cout << initial_array_msg.str();
					announce_initial_array = false;
				}
				array_size_t dbg_size = block_2_end - block_1_start + 1;
				dbg_msg 	<< SortingUtilities::arrayElementsToString(array, dbg_size)
							<< " b1_st "	<< std::setw(2) << block_1_start
							<< " b2_st "	<< std::setw(2)	<< block_2_start
							<< " b2_nd "	<< std::setw(2) << block_2_end
							<< " a_st  " 	<< std::setw(2) << a_unmerged_start
							<< " a_end "	<< std::setw(2)	<< a_unmerged_end
							<< " b_st  "	<< std::setw(2) << b_unmerged_start
							<< " b_end "	<< std::setw(2)	<< b_unmerged_end
							<< " sp_st " 	<< std::setw(2)	<< span_start
							<< " sp_en "	<< std::setw(2)	<< span_end
							<< " rrcnt "	<< std::setw(2)	<< rotate_count
							<< " b_max_pos " << std::setw(2) << b_max_pos
							<< std::endl;
				std::cout   << dbg_msg.str();
			}
			a_unmerged_start = span_start + rotate_count;
			a_unmerged_end	+= rotate_count;
			b_unmerged_start = span_end+1;
		}
		if (debug_verbose && a_rotation_occurred) {
			array_size_t dbg_size = block_2_end - block_1_start + 1;
			std::cout << SortingUtilities::arrayElementsToString(array, dbg_size)
					  << " initial array state"
					  << std::endl << std::endl;
		}
		return b_max_pos;
	}


	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*																					*/
	/*					MERGE BLOCKS BY SEARCHING FROM  RIGHT-TO-LEFT					*/
	/*																					*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/


	/*	Merge blocks by identifying spans to rotate into place by starting
	 * an index at the end of block 1, and an index at the end of block 2,
	 * and moving the indices right to left.
	 *
	 * In the following discussion,
	 * 	rotate(span_start, span_end, rotate_count)
	 * 							refers to the rotating all element right
	 * 	As						refers to the least (leftmost) element
	 * 								from the A_Block that has not been placed
	 * 	Ae						refers to the greatest (rightmost) element
	 * 								that has not been placed
	 * 	Bs						refers to the least (leftmost) element
	 * 								from the B_Block that has nto been placed
	 * 	Be						is always the last element in B
	 * 	Ai						is the index into the a portion of the array
	 * 	Bi						is the index into the b portion of the array
	 * 	Ss						is the start of the span to be rotated
	 * 	Se						is the end of the span to be rotated
	 * 	Rc						is the count to rotate the span right
	 *
	 * Consider the following array:
	 *[       A Block        ][    B Block        ]
	 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14	| As Ai Ae  Bs Bi Be
	 * A  F  G  H  I  L  M  O  B  C  D  E  J  K  P	|  0  7  7   8 13 14
	 * 	Move from right to left through b to determine:
	 * [b_i=13] = 'K' is the first element in b that is less than [a_i= 7] = 'O'
	 * 	Move from right to left through a to determine:
	 * [a_i= 4] = 'I' is the first element in a that is less than [b_i=13] = 'K'
	 * rotate (a_i+1, b_i) by b_i - a_e = 6 counts to move a_e to b_i
	 *[  A Block    ][ B Block  ][   Merged       ]
	 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14	| As Ai Ae  Bs Bi Be
	 * A  F  G  H  I  B  C  D  E  J  K  L  M  O  P  |  0  4  4   5  8  8
	 *	Move from right to left through b to determine:
	 * [b_i= 8] = 'E' is the first element in b that is less than [a_i= 1] = 'F
	 * 	Move from right to left through a to determine:
	 * [a_i= 0] = 'A' is the first element in b that is less than [b_i= 8] = 'E'
	 * rotate (a_i+1, b_i) by b_i - a_e = 4 countes to move a_e to b_i
	 *[ ][   Merged                               ]
	 * 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14	| As Ai Ae  Bs Bi Be
	 * A  B  C  D  E  F  G  H  I  J  K  L  M  O  P  |  0  0  0   1  4  4
	 * 	Move from right to left through b to determine:
	 * 	There are no elements in b that are less than [a_e]
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_RightToLeft(
												T* 			 array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics)
	{
		constexpr bool debug_verbose		= false;
		std::stringstream initial_array_msg;

		if (debug_verbose) {
			array_size_t dbg_size = block_2_end - block_1_start+1;
			initial_array_msg << SortingUtilities::arrayElementsToString(array, dbg_size)
			<< " initial array state"
			<< std::endl;
		}

		if (debug_verbose) {
			std::cout << "Entry into RotateRightToLeft()" << std::endl;
			std::cout << initial_array_msg.str();
		}

		array_size_t b_max_pos  		= block_2_end;
		//	set to false after the first time b_unmerged_end moves
		if (block_1_end - block_1_start < 0)
			return b_max_pos;
		if (block_2_end - block_2_start < 0)
			return b_max_pos;

		array_size_t a_unmerged_start	= block_1_start;
		array_size_t a_unmerged_end		= block_1_end;
		array_size_t b_unmerged_start	= block_2_start;
		array_size_t b_unmerged_end		= block_2_end;

		//	set to false after the first time b_unmerged_end moves
		bool b_max_frozen				= false;

		//	keep going until all of a is to the right of b
		//	or there is no more b to be merged
		while (b_unmerged_end 	 > a_unmerged_end &&
			   b_unmerged_start <= block_2_end)
		{
			array_size_t a_i = a_unmerged_end;
			array_size_t b_i = b_unmerged_end;
			array_size_t b_span;	// used to update b_end
			array_size_t span_start;
			array_size_t span_end;
			array_size_t rotate_count;

			//	One of three things will happen
			//	1.  no element [a_i] is greater than [b_end]
			//			decrement b_end--
			//			continue
			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end
			//			break;
			//	3. 	a span of at least 1 element in the remaining b_block
			//	  	goes between two elements in the a_block

			//	Case 1: array[b_end] >= (to the right of) array[a_end]
			if (metrics) metrics->compares++;
			if (array[b_unmerged_end] >= array[a_unmerged_end]) {
				if (!b_max_frozen) {
					b_max_pos = b_unmerged_end;
					b_max_frozen = true;
				}
				b_unmerged_end--;
				continue;
			}
			// array[b_end] is less than a[end] - determine how many
			//	other a elements are greater than b
			if (metrics) metrics->compares++;
			while (a_i >= a_unmerged_start && array[a_i] > array[b_unmerged_end]) {
				a_i--;
				if (metrics) metrics->compares++;
			}

			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end)
			//			break;
			if (a_i < a_unmerged_start) {
				b_span		= b_unmerged_end - b_unmerged_start;
				span_start 	= a_i + 1;
				span_end 	= b_unmerged_end;
				rotate_count= b_unmerged_end - a_unmerged_end;
				SortingUtilities::rotateArrayElementsRight(array,
														   span_start,
														   span_end,
														   rotate_count,
														   metrics);
				if (!b_max_frozen) {
					//	all of the remaining b has been moved to the start
					b_max_pos = span_start  + b_span;
					b_max_frozen = true;	// doesn't matter - exiting
				}
				break;
			}

			//	3. 	a span of at least 1 element in the remaining b_block
			//	  	goes between two elements in the a_block
			//
			// 	find the first value in b that is less than array[a_i]
			//	(a_i is the element immediately to the left of the span)
			if (metrics) metrics->compares++;
			while (array[b_i] > array[a_i] && b_i > a_unmerged_end) {
				b_i--;
				if (metrics) metrics->compares++;
			}
			span_start 	= a_i + 1;
			span_end	= b_unmerged_end;
			//	how much do you need to rotate to get a_end
			//	into b_end's position?
			rotate_count= b_unmerged_end - a_unmerged_end;
			if (debug_verbose) {
				array_size_t  u_size = block_1_end - block_1_start + 1;
				array_size_t  v_size = block_2_end - block_2_start + 1;
				std::cout  << SortingUtilities::arrayElementsToString(array, u_size)
						   << " |"
						   << SortingUtilities::arrayElementsToString(&array[block_2_start], v_size)
						   <<  " a_st " << std::setw(2) << a_unmerged_start
						   << ", a_i "  << std::setw(2) << a_i
						   << ", a_e " 	<< std::setw(2) << a_unmerged_end
						   << ": b_st " << std::setw(2) << b_unmerged_start
						   << ", b_i " 	<< std::setw(2) << b_i
						   << ", b_e " 	<< std::setw(2) << b_unmerged_end
						   << ", spst " << std::setw(2) << span_start
						   << " spnd " 	<< std::setw(2) << span_end
						   << " rc " 	<< std::setw(2) << rotate_count
						   << " b_max "	<< std::setw(2) << b_max_pos
						   << " BEFORE ROTATION"
						   << std::endl;
			}
			SortingUtilities::rotateArrayElementsRight(array,
													   span_start,
													   span_end,
													   rotate_count,
													   metrics);
			//	all of the remaining b has been
			//	rotated to the start of the span
			b_span 				= b_unmerged_end-b_unmerged_start;
			b_unmerged_start	= span_start;
			b_unmerged_end		= b_unmerged_start + b_span;
			a_unmerged_end		= a_i;
			if (!b_max_frozen) {
				b_max_pos 	= b_unmerged_end;
				b_max_frozen= true;
			}
			if (debug_verbose) {
				array_size_t  u_size = block_1_end - block_1_start + 1;
				array_size_t  v_size = block_2_end - block_2_start + 1;
				std::cout  << SortingUtilities::arrayElementsToString(array, u_size)
						   << " |"
						   << SortingUtilities::arrayElementsToString(&array[block_2_start], v_size)
						   <<  " a_st " << std::setw(2) << a_unmerged_start
						   << ", a_i "  << std::setw(2) << a_i
						   << ", a_e " 	<< std::setw(2) << a_unmerged_end
						   << ": b_st " << std::setw(2) << b_unmerged_start
						   << ", b_i " 	<< std::setw(2) << b_i
						   << ", b_e " 	<< std::setw(2) << b_unmerged_end
						   << ", spst " << std::setw(2) << span_start
						   << " spnd " 	<< std::setw(2) << span_end
						   << " rc " 	<< std::setw(2) << rotate_count
						   << " b_max "	<< std::setw(2) << b_max_pos
						   << " AFTER  ROTATION"
						   << std::endl;
			}
		}
		if (debug_verbose) {
			std::cout << "Exiting RotateRightToLeft()" << std::endl;
		}
		if (!b_max_frozen) {
			b_max_pos = b_unmerged_start;
		}
		return b_max_pos;
	}


	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*																					*/
	/*				MERGE BLOCKS BY HYBRID OF BINARY SEARCH VS RIGHT-TO-LEFT			*/
	/*																					*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/

	/*
	 * The first merge is done using a binary search. Successive merges
	 * are done searching right-to-left.  It is possible that once found,
	 * right_to_left is more efficient since blocks may be interleaved
	 * and thus the gaps do not require a lot of searching.
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_Hybrid(
												T* 			 array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics) {
		constexpr bool debug_verbose = false;
		std::stringstream initial_array_msg;

		if (debug_verbose) {
			array_size_t dbg_size = block_1_end - block_1_start + 1;
			initial_array_msg << SortingUtilities::arrayElementsToString(
											array, dbg_size);
			initial_array_msg << " |";
			dbg_size = block_2_end - block_2_start + 1;
			initial_array_msg << SortingUtilities::arrayElementsToString(
											&array[block_2_start], dbg_size)
			<< " initial array state"
			<< std::endl;
		}

		if (debug_verbose) {
			std::cout << "Entry into merge_Hybrid()" << std::endl;
			std::cout << initial_array_msg.str();
		}

		if (block_1_end < block_1_start)
			return block_2_end;
		if (block_2_end < block_2_start)
			return block_2_end;	// this is an error

		array_size_t b_max_pos  		= block_2_end;
		array_size_t a_unmerged_start	= block_1_start;
		array_size_t a_unmerged_end		= block_1_end;
		array_size_t b_unmerged_start	= block_2_start;
		array_size_t b_unmerged_end		= block_2_end;
		array_size_t b_span;
		array_size_t span_start;
		array_size_t span_end;
		array_size_t rotate_count;

		bool b_max_locked				= false;
		array_size_t first_a_greater_than_b_end;
		//	identify the element in a[] that b[end] goes BEFORE
		// [A block][B block]
		//	0 1 2 3  4 5 6 7
		//	A E F L  B G H I	returns [3] = 'L'
		first_a_greater_than_b_end =
			SortingUtilities::binarySearchLastElement(
									array,
									a_unmerged_start, a_unmerged_end,
									array[b_unmerged_end],
									metrics);

		//	if this [b_end] goes before all of A
		//		  A Block	  B Block
		//	      0 1 2 3     4 5 6 7
		//	 	[ M O Q S ] [ A B C D ]	first_a_greater_than_b_end = 0
		//	exit with [ A B C D M O Q S ] and b_max = 3
		if (first_a_greater_than_b_end <= a_unmerged_start) {
			span_start 	= a_unmerged_start;
			span_end 	= b_unmerged_end;
			rotate_count= b_unmerged_end - a_unmerged_end;
			b_span		= b_unmerged_end - b_unmerged_start;
			b_max_pos	= span_start + b_span;	// = a_unmerged_end
			SortingUtilities::rotateArrayElementsRight(array,
													   span_start,
													   span_end,
													   rotate_count,
													   metrics);
			return b_max_pos;
		}

		//	if all of a is less than [b_end], move b_end to the left
		//	 by trying to find the leftmost b that is > a_end
		//		  A Block	  B Block
		//	      0 1 2 3     4 5 6 7
		//	 4  [ M N O P ] [ A B C Q ]
		if (first_a_greater_than_b_end == b_unmerged_start) {
			// do a binary search in b to find an element < [a_end]
			b_max_pos 	= b_unmerged_end;
			b_max_locked= true;
			b_unmerged_end =
				SortingUtilities::binarySearchLastElement(
									array,
									b_unmerged_start, b_unmerged_end,
									array[a_unmerged_end],
									metrics);
			b_unmerged_end--;
			//	if the leftmost B element > [a_end] is a_end,
			//	all of b is >= a and we are done
			if (b_unmerged_end == a_unmerged_end)
				return b_max_pos;
		}
		else
		{
			//	identify all Bs that are greater than the element in A
			//	that is to the left of 'first a_greater_than_b_end'
			// 		[A block][B block]	first_a_greater = 3 = 'L' > [b_end] = 'I'
			//		 0 1 2 3  4 5 6 7
			//		 A E F L  B G H I
			//	find first_b > [3-1] = 'F' which is [5] = 'G'
			//	rotate count = b_end - a_end = 7-3 = 4
			//	b_span = b_end-b_start = 3
			//	b_span will move to
			span_start		= first_a_greater_than_b_end;
			span_end		= b_unmerged_end;
			rotate_count	= b_unmerged_end - a_unmerged_end;
			b_span			= b_unmerged_end - b_unmerged_start;
			b_unmerged_start= span_start;
			b_unmerged_end	= b_unmerged_start + b_span;
			b_max_pos		= b_unmerged_end;
			b_max_locked	= true;
			a_unmerged_end	= span_start-1;

			SortingUtilities::rotateArrayElementsRight(array,
													   span_start,
													   span_end,
													   rotate_count,
													   metrics);
		}

		//	Binary search has been used to position a_end and b_end possibly
		// close to the next span of [b_i:b_end] within [a_start:a_end]
		while (b_unmerged_end 	 > a_unmerged_end &&
			   b_unmerged_start <= block_2_end)
		{
			array_size_t a_i = a_unmerged_end;
			array_size_t b_i = b_unmerged_end;

			//	One of three things will happen
			//	1.  no element [a_i] is greater than [b_end]
			//			decrement b_end--
			//			continue
			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end
			//			break;
			//	3. 	a span of at least 1 element in the remaining b_block
			//	  	goes between two elements in the a_block

			//	Case 1: array[b_end] >= (to the right of) array[a_end]
			if (metrics) metrics->compares++;
			if (array[b_unmerged_end] >= array[a_unmerged_end]) {
				if (!b_max_locked) {
					b_max_pos = b_unmerged_end;
					b_max_locked = true;
				}
				b_unmerged_end--;
				continue;
			}
			// array[b_end] is less than a[end] - determine how many
			//	other a elements are greater than b
			if (metrics) metrics->compares++;
			while (a_i >= a_unmerged_start && array[a_i] > array[b_unmerged_end]) {
				a_i--;
				if (metrics) metrics->compares++;
			}

			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end)
			//			break;
			if (a_i < a_unmerged_start) {
				b_span		= b_unmerged_end - b_unmerged_start;
				span_start 	= a_i + 1;
				span_end 	= b_unmerged_end;
				rotate_count= b_unmerged_end - a_unmerged_end;
				SortingUtilities::rotateArrayElementsRight(array,
														   span_start,
														   span_end,
														   rotate_count,
														   metrics);
				if (!b_max_locked) {
					//	all of the remaining b has been moved to the start
					b_max_pos = span_start  + b_span;
					b_max_locked = true;	// doesn't matter - exiting
				}
				break;
			}

			//	3. 	a span of at least 1 element in the remaining b_block
			//	  	goes between two elements in the a_block
			//
			// 	find the first value in b that is less than array[a_i]
			//	(a_i is the element immediately to the left of the span)
			if (metrics) metrics->compares++;
			while (array[b_i] > array[a_i] && b_i > a_unmerged_end) {
				b_i--;
				if (metrics) metrics->compares++;
			}
			span_start 	= a_i + 1;
			span_end	= b_unmerged_end;
			//	how much do you need to rotate to get a_end
			//	into b_end's position?
			rotate_count= b_unmerged_end - a_unmerged_end;
			if (debug_verbose) {
				array_size_t  u_size = block_1_end - block_1_start + 1;
				array_size_t  v_size = block_2_end - block_2_start + 1;
				std::cout  << SortingUtilities::arrayElementsToString(array, u_size)
						   << " |"
						   << SortingUtilities::arrayElementsToString(&array[block_2_start], v_size)
						   <<  " a_st " << std::setw(2) << a_unmerged_start
						   << ", a_i "  << std::setw(2) << a_i
						   << ", a_e " 	<< std::setw(2) << a_unmerged_end
						   << ": b_st " << std::setw(2) << b_unmerged_start
						   << ", b_i " 	<< std::setw(2) << b_i
						   << ", b_e " 	<< std::setw(2) << b_unmerged_end
						   << ", spst " << std::setw(2) << span_start
						   << " spnd " 	<< std::setw(2) << span_end
						   << " rc " 	<< std::setw(2) << rotate_count
						   << " b_max "	<< std::setw(2) << b_max_pos
						   << " BEFORE ROTATION"
						   << std::endl;
			}
			SortingUtilities::rotateArrayElementsRight(array,
													   span_start,
													   span_end,
													   rotate_count,
													   metrics);
			//	all of the remaining b has been
			//	rotated to the start of the span
			b_span 				= b_unmerged_end-b_unmerged_start;
			b_unmerged_start	= span_start;
			b_unmerged_end		= b_unmerged_start + b_span;
			a_unmerged_end		= a_i;
			if (!b_max_locked) {
				b_max_pos 	= b_unmerged_end;
				b_max_locked= true;
			}
			if (debug_verbose) {
				array_size_t  u_size = block_1_end - block_1_start + 1;
				array_size_t  v_size = block_2_end - block_2_start + 1;
				std::cout  << SortingUtilities::arrayElementsToString(array, u_size)
						   << " |"
						   << SortingUtilities::arrayElementsToString(&array[block_2_start], v_size)
						   <<  " a_st " << std::setw(2) << a_unmerged_start
						   << ", a_i "  << std::setw(2) << a_i
						   << ", a_e " 	<< std::setw(2) << a_unmerged_end
						   << ": b_st " << std::setw(2) << b_unmerged_start
						   << ", b_i " 	<< std::setw(2) << b_i
						   << ", b_e " 	<< std::setw(2) << b_unmerged_end
						   << ", spst " << std::setw(2) << span_start
						   << " spnd " 	<< std::setw(2) << span_end
						   << " rc " 	<< std::setw(2) << rotate_count
						   << " b_max "	<< std::setw(2) << b_max_pos
						   << " AFTER  ROTATION"
						   << std::endl;
			}
		}
		if (debug_verbose) {
			std::cout << "Exiting RotateHybrid()" << std::endl;
		}
		if (!b_max_locked) {
			b_max_pos = b_unmerged_start;
		}
		return b_max_pos;
	}


	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*					Rotate non-continuous blocks right								*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/

	template <typename T>
	void rotateArrayElementsRight(T* array,
								  array_size_t span_start,
								  array_size_t span_end,
								  array_size_t amount,
								  SortMetrics *metrics) {

		//	qualify all of the block indices
		if (span_end == span_start) {
			return;
		}

		//	ensure that the boundaries are ascending
		if (span_start > span_end) {
			array_size_t temp;
			temp		= span_start;
			span_start	= span_end;
			span_end	= temp;
		}

		array_size_t span_size = span_end - span_start + 1;
		//	converts amounts that are not in [0,span) to in range
		amount = calcRotationWithModulo(amount, span_size);
		if (amount == 0)
			return;

		//	reverse the entire array
		for (array_size_t i = span_start, j = span_end; i < j; j++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}

		//	reverse the left hand side
		for (array_size_t i = span_start, j = span_start+amount-1; i < j; i++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}

		//	reverse the right hand side
		for (array_size_t i = span_start + amount, j = span_end; i < j; i++, j--) {
			SortingUtilities::swap(array, i, j, metrics);
		}
	}

}	// namespace BlockOperations

#endif /* BLOCKOPERATIONSMERGEROTATION_H_ */
