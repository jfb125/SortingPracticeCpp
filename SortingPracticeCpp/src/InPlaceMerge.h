/*
 * InPlaceMerge.cpp
 *
 *  Created on: Oct 9, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"
#include "InsertionSort.h"

namespace InPlaceMerge {
using index_t = array_size_t;

	/*	Merge blocks using binary searches to identify spans to rotate
	 *
	 * In the following discussion,
	 * 	last(start,end,key) 	refers to a binary search that
	 * 	 							returns the left-most element
	 * 	 							that is > the search term
	 * 	rotate(span_start, span_end, rotate_count)
	 * 							refers to the rotating all element righ
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
	 *[       A Block        ][    B Block
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
	SortMetrics mergeTwoAdjacentBlocksByRotation( T** array,
												  index_t block_1_start,
												  index_t block_2_start,
												  index_t block_2_end) {
		constexpr bool debug_verbose = false;
		SortMetrics metrics(0,0);
		index_t	a_start = block_1_start;
		index_t a_end	= block_2_start-1;
		index_t b_start	= block_2_start;
		index_t b_end	= block_2_end;
		index_t span_start 	= 0;
		index_t span_end 	= 1;
		index_t rotate_count;

		//	if b_start moves off the end of the array, it is sorted
		//	if there are no more a values to merge, it is sorted
		while (b_start <= block_2_end && span_start != span_end && a_start != a_end) {
			if (debug_verbose) {
				std::cout << "Start of loop ";
			}
			span_start =
				SortingUtilities::binarySearchLastElement(array,
														  a_start, a_end,
														  array[b_start],
														  metrics);
			span_end =
				SortingUtilities::binarySearchLastElement(array,
														  b_start, b_end,
														  array[span_start],
														  metrics);
			rotate_count = span_end - b_start;
			span_end--;
			if (debug_verbose) {
				std::cout 	<< " block1_start "	<< std::setw(2) << block_1_start
							<< " block2_start "	<< std::setw(2)	<< block_2_start
							<< " block2_end "	<< std::setw(2) << block_2_end
							<< " a_start " 		<< std::setw(2) << a_start
							<< " a_end "		<< std::setw(2)	<< a_end
							<< " b_start "		<< std::setw(2) << b_start
							<< " b_end "		<< std::setw(2)	<< b_end
							<< " span_start " 	<< std::setw(2)	<< span_start
							<< " span_end "		<< std::setw(2)	<< span_end
							<< " rotate_count "	<< std::setw(2)	<< rotate_count;
			}
			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
														   span_start, span_end,
														   rotate_count);
			if (debug_verbose) {
				std::cout << " got past rotateElements\n";
			}
			a_start = span_start + rotate_count;
			a_end	+= rotate_count;
			b_start = span_end+1;
		}
		return metrics;
	}

	/*
	 * 	Address the array as being made up of blocks, and merge adjacent blocks.
	 *
	 * 	Double the block size each time so what was previously two separate blocks
	 * 	  is now treated as a single block.  Note that the size of the last block
	 * 	  is possibly not an power of 2
	 *
	 * 	  	First pass	[0:15][16:31][32:47][48:63][64:79][80:95][96:102][103:104]
	 * 	  	Second pass	[    0:31   ][    32:63   ][   64:95    ][    96:104     ]
	 * 	  	Third pass	[          0:63           ][            64:104           ]
	 *
	 *	If the array size is less than 32, use an insertion sort and return
	 *
	 *	The mergeInPlace is done using different algorithms
	 */

	constexpr index_t initial_block_size = 16;

	template <typename T>
	SortMetrics sortPointerstoObjects(T **array_of_pointers, array_size_t size) {

		constexpr bool debug_verbose = false;
		SortMetrics metrics(0,0);

		SortMetrics (*mergeInPlace)(T**array,
									index_t block_1_start,
									index_t block_2_start,
									index_t block_2_end) =
			mergeTwoAdjacentBlocksByRotation;

		if (size < 2*initial_block_size) {
			metrics = InsertionSort::sortPointersToObjects(array_of_pointers, size);
			return metrics;
		}
		index_t block_size = initial_block_size;

		index_t block_1_start = 0;

		while (block_1_start < size) {
			index_t num_elements = block_size;
			if (block_1_start + num_elements > size)
				num_elements = size - block_1_start;
			metrics +=
				InsertionSort::sortPointersToObjects(&array_of_pointers[block_1_start],
													 num_elements);
			block_1_start += block_size;
		}
		if (debug_verbose) {
			std::cout << "  Made it through sorting initial blocks\n";
		}
		while (block_size < size) {
			block_1_start = 0;
			index_t block_2_start = block_1_start + block_size;
			index_t block_2_end	  = block_2_start + block_size - 1;
			//	it is possible that block size is greater than half the array
			if (block_2_end > size-1)
				block_2_end = size-1;
			while (block_2_start < size) {
				metrics += mergeInPlace(array_of_pointers,
							 	 	 	block_1_start, block_2_start, block_2_end);
//				metrics +=
//					InsertionSort::sortPointersToObjects(
//							&array_of_pointers[block_1_start],
//							block_2_end - block_1_start + 1);
				block_1_start = block_2_end+1;
				block_2_start = block_1_start + block_size;
				block_2_end	  = block_2_start + block_size - 1;
				if (block_2_end > size-1)
					block_2_end = size-1;
			}
			if (debug_verbose) {
				std::cout << "  Made it through merging blocks of size " << block_size << std::endl;
			}
			block_size *= 2;
		}
		if (debug_verbose) {
			std::cout << __FUNCTION__ << " is returning\n";
		}
		return metrics;
	}

}
