/*
 * BlockSortElementMoving.h
 *
 *  Created on: Oct 6, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTELEMENTMOVING_H_
#define BLOCKSORTELEMENTMOVING_H_

#include <iostream>
#include <iomanip>
#include <string>

#include "BlockSortDataTypes.h"
#include "SortingUtilities.h"

	/*	**********************************************************	*/
	/*							MergeStrategy						*/
	/*	**********************************************************	*/

	namespace BlockOperations {
		enum class MergeStrategy {
			 AUXILLIARY,
			 BINARY,
			 HYBRID,
			 INSERTION,
			 RGT_TO_LFT,
			 TABLE
		};
	}	// namespace BlockOperations

	//	Generate a compiler error if 'MergeStrategy' is defined in std
	namespace std {
		enum class MergeStrategy {
			 AUXILLIARY,
			 BINARY,
			 HYBRID,
			 INSERTION,
			 RGT_TO_LFT,
			 TABLE
		};
	}

	#define MERGE_STRATEGY_AUXILLIARY_STRING	"AUXILLIARY"
	#define MERGE_STRATEGY_BINARY_STRING		"BINARY"
	#define MERGE_STRATEGY_HYBRID_STRING		"HYBRID"
	#define MERGE_STRATEGY_INSERTION_STRING     "INSERTION"
	#define MERGE_STRATEGY_ROTATE_STRING		"RGT_TO_LFT"
	#define MERGE_STRATEGY_TABLE_STRING			"TABLE"
	#define MERGE_STRATEGY_UNKNOWN_STRING		"UNKNOWN"
	#define MERGE_STRATEGY_MAX_STRING_LENGTH	10

namespace BlockOperations
{
	template <typename T>
	using MergeFunction = array_size_t (*)(	T* array,
								 	 	 	 array_size_t block_1_begin,
											 array_size_t block_1_end,
											 array_size_t block_2_begin,
											 array_size_t block_2_end,
											 SortMetrics *metrics);

	/*	Sorts an array from [begin:end] using an insertion sort
	 * 	Parameter 'ignored' is provided to make its signature match
	 * 	BlockMerging algorithms	*/
	template <typename T>
	array_size_t insertionSortPartial(T* array,
									  array_size_t begin,
									  array_size_t ignored,
									  array_size_t suspect,
									  array_size_t end,
									  SortMetrics *metrics = nullptr);

	/* 	Blocks do not have to be continuous nor do they have to be the same size */
	template <typename T>
	array_size_t mergeTwoBlocksElementsByTable(	T* array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics = nullptr);

	/*	Uses a binary search to identify spans of block_1 that can be
	 * rotated into the midst of the block_2 */
	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_BinarySearch(
												T* array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics = nullptr);

	/*	The first merge is done using a binary search. Successive merges
	 * are done searching right-to-left.  It is possible that this is a
	 * right_to_left is more efficient since blocks may be interleaved */
	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_Hybrid(
												T* array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics = nullptr);

	/*	Starts with a_index & b_index at the end of their blocks, then
	 * moves right_to_left to identify spans of block_2 that can be
	 * rotated into the midst of the block_1 */
	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_RightToLeft(
												T* array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics = nullptr);

	/* 	Blocks do not have to be continuous nor do they have to be the same size */
	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBuffer(
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics = nullptr);

	template <typename T>
	void swapBlockElementsOfEqualSize(T* array,
									 array_size_t block1_start,
									 array_size_t block_2_start,
									 array_size_t block_size,
									 SortMetrics *metrics = nullptr);

	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*								Functions									*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	/*
	 * 	This performs an insertion sort on the array starting from element 'suspect',
	 * 	moving leftwards if necessary until 'start', and continuing with successive
	 * 	elements to the right until 'size-1'.
	 *
	 * 		A_Blocks are defined as coming from the left sub-array and use their
	 * 		  first value as their key
	 * 		B_Blocks are defines as coming form the right sub-array and use their
	 * 		  last value as their key
	 *
	 *   The keys are chosen such that if an A_Block goes to the right of a B_Block
	 *  it is guaranteed that keys with equal values that are in each of the blocks
	 *  get do not have their absolute order re-arranged.  Consider:
	 *
	 *    A_Block = [C,a  C,a  D,a  D,a] and B_Block = [B,b B,b C,b C,b]
	 *
	 *  If B's key was 'B,b' and A's key is C,a placing A_Block after B_Block
	 *    would cause the elements with identical keys 'C,?' to be out of order
	 *
	 *    			[Bb Bb Cb Cb][Ca Ca Da Da]
	 *
	 * 	  The only time that it is necessary to	merge two blocks
	 * 	is when a B_Block is placed after an A_Block:
	 *
	 *			  0  1  2  3  4  5  6  7  8  9 10 11              keys
	 * 	A0:A1:A2 [C  D  E  F][G  H  I  J][P  Q  R  S]  A0 = C; A1 = G; A2 = P
	 * 	B0:B1:B2 [A  B  C  K][L  M  N  O][T  U  V  W]  B0 = K; B1 = O; B2 = W
	 * 	B0:B1:A2 [A  B  C  K][L  M  N  O][P  Q  R  S]  B0 = K; B1 = O; A2 = P
	 * 	A0:A1:B0 [C  D  E  F][G  H  I  J][A  B  C  K]  A0 = C; A1 = G; B0 = K
	 *
	 * 	  In the last example, B0 on the right side has elements [A, B] that
	 * 	need to go to the *left* of all of the A_Blocks, not just A1. The element
	 * 	in B0 with key 'C' goes to the *right* of the elements in A0 with key 'C'.
	 *
	 * 	   Source block:  B0     A0  B0  A0      A1        B0
	 * 	   Sequence    : [A  B] [C] [C] [D E F] [G H I J] [K]
	 *
	 *	The elements in the B0 block(s) on the right are already in order.  Thus,
	 *	as soon as a B_Block value is found that it greater than it's neighbor
	 *	to the left, all the values out to 'end' are in order and the function returns
	 *	Usage:
	 *
	 * 	This returns the index of the final position of the element that was
	 * 	  at the end of the array. This is useful for keeping track of where the rightmost value that is in the correct place is located.  knowing
	 * 	  where the rightmost value in the array is that is in the correct order
	 * 	  allows the next merge to know what portion of the array is potentially
	 * 	  not ordered
	 *
	 * 	highest_b_position = mergeByInsertionSort(array, a[0].start, b[0].start, b[0].end)
	 */

	//	'ignored' is only used so that the signature of this function matches
	//	the signature of merging functions that get the bounds of both blocks
	//	passed to them, such as mergeToAdjacentBlocksByRotation() or
	//	mergeTwoBlocksElementsByTable()

	template <typename T>
	array_size_t insertionSortPartial(T* 		   array,
									  array_size_t begin,
									  array_size_t ignored,
									  array_size_t suspect,
									  array_size_t end,
									  SortMetrics *metrics)
	{
		array_size_t highest_b_position = end;

		if (begin > end || suspect > end) {
			return highest_b_position;
		}

		//	from here to the end of the array
		for (int i = suspect; i <= end ; i++) {
			//	keep reseting highest_b_position to the end
			highest_b_position = end;
			// The first time a B_Block element is found that is
			//	in order that means all the elements to the right,
			//	which are in order b/c they came from the same
			//	B_Block, are sorted.  Abort the loop
			if (metrics) metrics->compares++;
			if (array[i-1] <= array[i])
				break;
			T temp = array[i];
			if (metrics) metrics->assignments++;
			int j = i;
			for ( ; j > begin; j--) {
				if (metrics) metrics->compares++;
				//	if the element to the left
				//	  is <= temp, temp goes here
				if (array[j-1] <= temp) {
					if (metrics) metrics->assignments++;
					array[j] = temp;
					highest_b_position = j;
					break;
				}
				//	shift the element to the right
				if (metrics) metrics->assignments++;
				array[j] = array[j-1];
			}
			// if the loop terminated b/c j == begin
			//	there were no elements found <= temp
			//	temp goes at begin
			if (j <= begin) {
				if (metrics) metrics->assignments++;
				array[begin] = temp;
				highest_b_position = begin;
			}
		}
		return highest_b_position;
	}


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
	 *
	 *
	 *	Although it is not necessary to pass 'block_1_end' as a parameter, it
	 *	is included so this function is compatible with other merge functions
	 *	that have the capability to merge non-contiguous blocks and thus need
	 *	to know explicitly where block_1 ends.
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_BinarySearch(
													T* 			 array,
													array_size_t block_1_start,
													array_size_t block_1_end,
													array_size_t block_2_start,
													array_size_t block_2_end,
													SortMetrics *metrics) {
		constexpr bool debug_verbose= false;
		bool announce_initial_array = true;
		bool a_rotation_occurred	= false;
		std::stringstream initial_array_msg;

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

	/*	Merge blocks using identifying spans to rotate into place by starting
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
	/*						MERGE BLOCKS BY ROTATION END OF SECTION						*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/


	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*																					*/
	/*								MERGING BLOCKS BY TABLE								*/
	/*																					*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableLowerSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics *metrics);
	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableUpperSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics *metrics);

	/*
	 *	template <typename T>
	 * 	array_size_t mergeTwoBlocksElementsByTable(
	 * 										T * array,
	 *										array_size_t block_1_start,
	 *	 									array_size_t block_1_end,
	 *										array_size_t block_2_start,
	 *										array_size_t block_2_end,
	 *										SortMetrics *metrics) {
	 *
	 *	Two blocks, which do not have to be contiguous, are merged in place
	 *	by maintaining a table of to where a block elements get displaced
	 *	prior to moving into its final place.
	 *
	 *	Returns the final position of the element that was in block_2_end.	All elements,
	 *	including that element, are now in their final position. Successive merges by the
	 *	calling sort routine will start after the largest block 2's final position.
	 *
	 *	  Block 1||  Block 2	Dst	  Table of  Table
	 *	  							  b1 pstns  index
	 *	  0  1  2  3  4  5  6	      0  1  2	 0	 Block 1 value		 Block 2 value
	 *	[ E  F  I  C  D  G  H ]	 0	  0  1  2	 0	 [Table[0]=0] = 'E'  [3] = 'C'
	 *	[ C  F  I  E  D  G  H ]  1    3  1  2    0   [Table[0]=3] = 'E'  [4] = 'D'
	 *	[ C  D  I  E  F  G  H ]  2    3  4  2    0   [Table[0]=3] = 'E'  [5] = 'G'
	 *  [ C  D  E  I  F  G  H ]  3    x  4  3    1   [Table[1]=4] = 'F'  [5] = 'G'
	 *	[ C  D  E  I  F  G  H ]  4    x  4  3    1   [Table[1]=4] = 'F'  [5] = 'G'
	 *	[ C  D  E  F  I  G  H ]  5    x  x  4    2   [Table[2]=4] = 'I'  [5] = 'G'
	 *	[ C  D  E  F  G  I  H ]  6    x  x  5    2   [Table[2]=5] = 'I'  [6] = 'H'
	 *	[ C  D  E  F  G  H  I ]  x    x  x  x    3  Table Index = 3, done
	 *		returns 5, which is where the 'H' from block 2 ended up
	 *
	 * 	Managing the overhead of the table is an expense. To minimize this expense,
	 * 	  there is a function perform the merge for when block_1 is smaller than
	 * 	  or equal to block_2's size, and a separate function for when block_2 is
	 * 	  the smaller block
	 */

	/*
	 * 	Primary function that determines which sub-function to call,
	 * 		mergeTwoBlocksElemetnsByTableLowerSmaller or
	 * 		mergeTwoBlocksElemetnsByTableUpperSmaller
	 */

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTable(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics *metrics) {

		array_size_t block_1_size = block_1_end - block_1_start + 1;
		array_size_t block_2_size = block_2_end - block_2_start + 1;

		//	if either block is empty, no need to merge
		if (block_1_size <= 0 || block_2_size <= 0)
			return block_2_end;

		if (block_1_size <= block_2_size) {
			return  mergeTwoBlocksElementsByTableLowerSmallest(array,
															  block_1_start,
															  block_1_end,
															  block_2_start,
															  block_2_end,
															  metrics);
		} else {
			return mergeTwoBlocksElementsByTableUpperSmallest(array,
															  block_1_start,
															  block_1_end,
															  block_2_start,
															  block_2_end,
															  metrics);
		}
	}

	/*
	 * 		FUNCTION FOR MERGING BY TABLE WHEN BLOCK 1 SIZE <= BLOCK 2 SIZE
	 *
	 * 	The algorithm creates a table of the location of each element from block 1
	 * 	Every time a location of a block 1 element gets displaced, the table is
	 * 	updated with the new location of the block 1 element that was at 'dest'
	 *
	 * 	Note that frequently a block 1 element will displace another block 1 element.
	 * 	The example above demonstrates this.
	 *
	 * 	When 'dest' == block_2_end, and the element at block_2_end is moved, the
	 * 	final location of that element is stored to return to caller
	 */

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableLowerSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics *metrics)
	{
		array_size_t largest_b2_position 	= block_2_end;

		//	Check to see if merging is necessary

		array_size_t block_1_size 			= block_1_end - block_1_start + 1;
		array_size_t block_2_size 			= block_2_end - block_2_start + 1;

		if (block_1_size <= 0 || block_2_size <= 0) {
			return largest_b2_position;
		}

		//	Build the table of locations of block_1's elements

		array_size_t table_size = block_1_size;
		array_size_t block_1_locations_table[table_size];

		for (array_size_t i = 0, src = block_1_start; i < table_size; ) {
			block_1_locations_table[i++] = src++;
		}

		//	Perform the merge

		array_size_t table_index 	= 0;
		array_size_t block_2_source	= block_2_start;
		array_size_t dest  			= block_1_start;

		//	This loop will terminate when either:
		//	   the table of block_1 elements is empty (all block_1 values merged)
		//	OR all of block_2's elements have been merged
		//
		//	Note that if it exits b/c all of block 2 elements are in place, the remaining
		//	  block 1 referenced by the table may no longer be in order - flush the table
		while (true)
		{
			// Point to the current location of the next block_1 element which may
			//	have been displaced in a previous pass through this loop.
			array_size_t block_1_source = block_1_locations_table[table_index];
			if (metrics) metrics->compares++;

			if (array[block_1_source] <= array[block_2_source]) {
				// the value from block 1 goes into 'dest'
				if (dest != block_1_source) {
					SortingUtilities::swap( array,
											dest,
											block_1_source,
											metrics);
					// Update the table location of the block_1 element that was
					//	at 'dest', which has been swapped to location 'block_1_source'
					for (array_size_t i = table_index+1; i != table_size; i++) {
						if (block_1_locations_table[i] == dest) {
							block_1_locations_table[i] = block_1_source;
						}
					}
				}
				//	update dest assuming blocks may not be contiguous
				if (dest != block_1_end)	dest++;
				else  						dest = block_2_start;
				//	if all block_1 elements have been merged into place, done
				if (++table_index == table_size)
					break;
			}
			else
			{
				// value from the block 2 is < value from the block 1
				if (dest != block_2_source) {
					SortingUtilities::swap(	array,
											dest,
											block_2_source,
											metrics);
					//	Was the element at 'block_2_end' swapped to 'dest'?
					if (block_2_source == block_2_end)
						largest_b2_position = dest;
					//	Ensure that if the displaced element was in the table,
					//	  it's location gets updated to where it was swapped
					for (array_size_t i = table_index; i != table_size; i++) {
						if (block_1_locations_table[i] == dest) {
							block_1_locations_table[i] = block_2_source;
						}
					}
				}
				//	update dest assuming blocks may not be contiguous
				if (dest != block_1_end)	dest++;
				else 						dest = block_2_start;
				//	if all the elements from block_2 are in place, break loop
				if (++block_2_source > block_2_end)
					break;
			}
		}

		//	If break out of while loop because all block_2 elements are now in place,
		//	  it is possible that there remain displaced block_1 elements that are no
		//	  longer in order. Finish positioning remaining block_1 elements from the table
		while(table_index < table_size)
		{
			array_size_t block_1_source	= block_1_locations_table[table_index];
			if (block_1_source != dest) {
				SortingUtilities::swap(	array,
										dest,
										block_1_source,
										metrics);
				// Update the table location of the block_1 element that was
				//	at 'dest', which has been swapped to 'block_1_source'
				for (array_size_t i = table_index+1; i != table_size; i++) {
					if (block_1_locations_table[i] == dest) {
						block_1_locations_table[i] = block_1_source;
					}
				}
			}
			//	update dest assuming blocks may not be contiguous
			if (dest != block_1_end) 	dest++;
			else  						dest = block_2_start;
			table_index++;
		}

		return largest_b2_position;
	}

	/*
	 * 		FUNCTION FOR MERGING BY TABLE WHEN BLOCK 1 SIZE > BLOCK 2 SIZE
	 *
	 * 	The algorithm creates a table of the location of each element from block 2
	 * 	Every time a location of a block 2 element gets displaced, the table is
	 * 	updated with the new location of the block 2 element that was at 'dest'
	 *
	 * 	Note that frequently a block 2 element will displace another block 2 element.
	 *
	 *	The 'dest' index starts at the end of block 2 and moves leftwards
	 * 	The table is accessed in a right-to-left order to match the right-to-left
	 * 	movement of 'dest'.
	 *
	 * 	If the table_index is at the right-most location, that indicates that
	 * 	the largest block 2 value has not been put in its final position in the array.
	 * 	It may have previously been displaced from its initial position of 'block_2_end',
	 * 	but it is not in its final position.
	 *
	 * 	Block 1  |Block 2 Dst  Table 	Table	B1 Pos		B2 Pos
	 * 	  0 1 2 3 4 5 6		   Pstns	Index
	 *	[ A B O P L M N ]  6   4 5 6     2		[3] = 'P'	[Table[2]=6] = 'N'  not in final
	 *	[ A B O N L M P ]  5   4 5 3     2      [2] = 'O'   [Table[2]=3] = 'N'  not in final
	 *	[ A B M N L O P ]  4   4 2 3     2      [1] = 'B'   [Table[2]=3] = 'N'  not in final
	 *	[ A B M L N O P ]  3   3 2 x     1      [1] = 'B'   [Table[1]=2] = 'M'  in final pos
	 *	[ A B L M N O P ]  2   3 x x     0      [1] = 'B'   [Table[0]=3] = 'L'
	 *	[ A B L M N O P ]  1   x x x    -1  Table Index = -1, done
	 *			return 4, which is where 'N' ended up
	 *
	 *	Note that in the event that all of block 1 get placed but there are still indices
	 *	of elements in the block 2 table, it is likely that the elements refered to by the
	 *	table need to be restored to their correct order by flushing the table
	 */

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableUpperSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics *metrics)
	{
		array_size_t b2_max_position 		= block_2_end;

		array_size_t block_1_size = block_1_end - block_1_start + 1;
		array_size_t block_2_size = block_2_end - block_2_start + 1;

		if (block_1_size <= 0 || block_2_size <= 0)
			return b2_max_position;

		//	build the table which will be accessed right to left
		array_size_t table_size = block_2_size;
		array_size_t block_2_locations_table[table_size];
		for (array_size_t i = table_size-1, src = block_2_end; i >= 0; ) {
			block_2_locations_table[i--] = src--;
		}

		//	initialize the indices
		array_size_t table_index	= table_size-1;
		array_size_t b1_source 		= block_1_end;
		array_size_t dest			= block_2_end;

		//	This loop exits (break) when either
		//	  	all of the block_2 elements are in place (table_index = -1)
		//	OR  there are no more block 1 elements to be placed (b1_source < block_1_start
		//
		//	Note that if it exits b/c all of block 1 elements are in place, the remaining
		//	  block 2 referenced by the table may no longer be in order - flush the table
		while (true)
		{
			// determine position of the current block 2 element to be merged
			array_size_t b2_source = block_2_locations_table[table_index];
			if (metrics) metrics->compares++;

			if (array[b1_source] > array[b2_source]) {
				// b1 is the larger element
				if (b1_source != dest) {
					SortingUtilities::swap(array, dest, b1_source, metrics);
					//	if the element from block_2_end (table_index == right)
					//	  has not been merged, and it was at 'dest'
					if (table_index == table_size-1 &&
						dest == block_2_locations_table[table_index]) {
							// block_2_end is now at b1_source
							b2_max_position = b1_source;
					}
					//	update the table entry that contains'dest'
					//	which has now been displaced to b1_source
					for (array_size_t i = table_index; i >= 0; i--) {
						if (block_2_locations_table[i] == dest) {
							block_2_locations_table[i] = b1_source;
							break;
						}
					}
				}
				//	update dest assuming blocks may not be contiguous
				if (dest != block_2_start) 	dest--;
				else						dest = block_1_end;
				//	if all of the elements from block_1 have been moved
				//	break loop and go flush block_2_locations_table[]
				if (--b1_source < block_1_start) {
					break;
				}
			} else {
				// block_2's element is the larger element
				if (b2_source != dest) {
					SortingUtilities::swap(array, dest, b2_source, metrics);
					//	did we put into place the largest b2 element?
					if (table_index == table_size-1) {
						b2_max_position = dest;
					}
					//	if the dest was the location of a block_2_element in
					//	the table, update the table with the new location
					for (array_size_t i = table_index-1; i >= 0; i--) {
						if (block_2_locations_table[i] == dest) {
							block_2_locations_table[i] = b2_source;
							break;
						}
					}
				}
				//	update dest assuming blocks may not be contiguous
				if (dest != block_2_start)	dest--;
				else						dest = block_1_end;
				//	if all block_2 elements have been placed, we are done
				if (--table_index < 0)
					break;
			}
		}

		//	flush the block_2_locations_in the table if there are still elements in it
		while (table_index >= 0) {
			array_size_t b2_source = block_2_locations_table[table_index];
			if (dest != block_2_locations_table[table_index]) {
				SortingUtilities::swap(array, dest, b2_source, metrics);
				// did we move in the largest b2 element into dest?
				if (table_index == table_size-1) {
					b2_max_position = dest;
				}
				for (array_size_t i = table_index-1; i >= 0; i--) {
					if (block_2_locations_table[i] == dest) {
						block_2_locations_table[i] = b2_source;
						break;
					}
				}
			}
			if (dest != block_2_start)	dest--;
			else						dest = block_1_end;
			table_index--;
		}

		return b2_max_position;
	}

	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*						MERGE BLOCKS BY TABLE END OF SECTION						*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/



	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*																					*/
	/*						MERGING BLOCKS BY AUXILLIARY BUFFER							*/
	/*																					*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/
	/*	******************************************************************************	*/

	/*
	 * 	Merging blocks by allocating an auxiliary 'displacement' buffer (queue) that
	 * 	is the size	of the smaller block. Elements are only moved into the
	 * 	'displacement' buffer when their position in the array is the destination
	 * 	of an element from the larger block.
	 */

	/*	This version treats the lower as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_LOWER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics);

	/*	This version treats the upper as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_UPPER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics);

	/*	******************************************************************	*/
	/*			wrapper function that calls either LOWER or UPPER			*/
	/*	******************************************************************	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBuffer(
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics)
	{
		array_size_t block_1_size = block_1_end - block_1_start + 1;
		array_size_t block_2_size = block_2_end - block_2_start + 1;

		if (block_1_size <= 0 || block_2_size <= 0) {
			return block_2_end;
		}

		if (block_1_size <= block_2_size) {
			return BlockOperations::mergeTwoBlocksElementsUsingAuxiliaryBufferFor_LOWER(
												array,
												block_1_start,
												block_1_end,
												block_2_start,
												block_2_end,
												metrics);
		} else {
			return BlockOperations::mergeTwoBlocksElementsUsingAuxiliaryBufferFor_UPPER(
												array,
												block_1_start,
												block_1_end,
												block_2_start,
												block_2_end,
												metrics);
		}
	}

	//	I did this instead of using std::queue<T> in order for the reader to
	//	  clearly see the internals of how the auxiliary buffer (queue) works
	template <typename T>
	class Queue {
		T *q;
		array_size_t size;
		array_size_t start;
		array_size_t end;
		array_size_t put;
		array_size_t get;
		bool empty;
		bool full;
	public:
		Queue(array_size_t sz) {
			size 	= sz;
			q 		= new T[size];
			start 	= 0;
			end		= size-1;
			put		= start;
			get		= start;
			empty	= true;
			full	= false;
		}
		~Queue() {
			if (q)
				delete[] q;
		}
		void enqueue(T object) {
			q[put] 	= object;
			empty 	= false;
			put 	= put != end ? put+1 : start;
			full 	= put == get;
		}
		T dequeue(void) {
			array_size_t loc = get;
			full 	= false;
			get 	= get != end ? get + 1 : start;
			empty 	= get == put;
			return q[loc];
		}
		T& peek(void) {
			return q[get];
		}
		bool is_empty(void)	{	return empty;	}
		bool is_full(void)	{	return full;	}
	};


	/*	version that treats the lower as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_LOWER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics)
	{
		auto add_one_compare = [&metrics] {
			if (metrics)	metrics->compares++;
		};
		auto add_one_assignment = [&metrics] {
			if (metrics)	metrics->assignments++;
		};

		auto next_dest = [&] (array_size_t current_dest) -> array_size_t {
			if (current_dest != block_1_end)	return current_dest+1;
			else								return block_2_start;
		};

		array_size_t b_max_position = block_2_end;
		array_size_t queue_size = (block_1_end - block_1_start + 1);
		BlockOperations::Queue<T> queue(queue_size);

		array_size_t dest 		= block_1_start;
		array_size_t b2_source 	= block_2_start;

		//	process all the elements in block_1
		while (dest <= block_1_end && b2_source <= block_2_end) {
			if (queue.is_empty()) {
				add_one_compare();
				if (array[dest] > array[b2_source]) {
					// the element at dest (b1_source) is > b2_source
					add_one_assignment();
					queue.enqueue(array[dest]);
					add_one_assignment();
					array[dest] = array[b2_source];
					if (b2_source == block_2_end)
						b_max_position = dest;
					b2_source++;
				}
			} else {
				// the dest will be overwritten by either dequeue or b2_source
				add_one_assignment();
				queue.enqueue(array[dest]);
				add_one_compare();
				if (queue.peek() <= array[b2_source]) {
					// the element in queue <= b2_source
					add_one_assignment();
					array[dest] = queue.dequeue();
				} else {
					//	the eleement in q > b2_source
					add_one_assignment();
					array[dest] = array[b2_source];
					if (b2_source == block_2_end)
						b_max_position = dest;
					b2_source++;
				}
			}
			dest = next_dest(dest);
		}

		//	compare any block_1 elements in queue vs block_2 elements
		while (!queue.is_empty() && b2_source <= block_2_end) {
			add_one_compare();
			if (queue.peek() <= array[b2_source]) {
				//	the element in the queue < b2_source
				add_one_assignment();
				array[dest] = queue.dequeue();
			} else {
				//	the element in b2 is < queue
				add_one_assignment();
				array[dest] = array[b2_source];
				if (b2_source == block_2_end)
					b_max_position = dest;
				b2_source++;
			}
			dest = next_dest(dest);
		}

		//	transfer any remaining block 1 elements from queue
		while (!queue.is_empty()) {
			add_one_assignment();
			array[dest] = queue.dequeue();
			dest = next_dest(dest);
		}

		return b_max_position;
	}

	/*	version that treats the upper as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_UPPER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics *metrics)
	{
		auto add_one_compare = [&metrics] () {
			if (metrics)	metrics->compares++;
		};
		auto add_one_assignment = [&metrics] () {
			if (metrics)	metrics->assignments++;
		};

		auto next_dest = [&] (array_size_t current_dest) -> array_size_t {
			return current_dest != block_2_start ? current_dest-1 : block_1_end;
		};

		array_size_t	b2_max_location 	= block_2_end;
		//	set if block_2_end gets enqueued,
		//	cleared when block_2_end (if in queue) gets dequeued,
		//		indicating that b2_max_location should be updated
		bool			b2_max_in_queue		= false;

		array_size_t	queue_size	= block_2_end - block_2_start + 1;
		BlockOperations::Queue<T> queue(queue_size);

		array_size_t dest		= block_2_end;
		array_size_t b1_source	= block_1_end;

		//	In all of the comments, keep in mind that b/c the array is being
		//	merged from right-to-left, the larger of the two values is moved
		while (dest >= block_2_start && b1_source >= block_1_start) {

			if (queue.is_empty()) {
				// source of b2 element will be dest which is in block_2
				add_one_compare();
				if (array[b1_source] > array[dest]) {
					// the element at b1_source > dest
					add_one_assignment();
					queue.enqueue(array[dest]);
					if (dest == block_2_end) {
						b2_max_in_queue	= true;
					}
					add_one_assignment();
					array[dest] = array[b1_source];
					b1_source--;
				}
			} else {
				//	source of b2 element will be queue
				//	regardless of where the element comes from, either
				//	b1 or from the queue, dest will be displaced
				add_one_assignment();
				queue.enqueue(array[dest]);
				add_one_compare();
				if (queue.peek() > array[b1_source]) {
					//	element in queue > b1_source
					add_one_assignment();
					array[dest] = queue.dequeue();
					if (b2_max_in_queue) {
						b2_max_location = dest;
						b2_max_in_queue = false;
					}
				} else {
					//	element at b1_source > element in queue
					add_one_assignment();
					array[dest] = array[b1_source];
					b1_source--;
				}
			}
			dest = next_dest(dest);
		}

		//	compare block 2 elements stored in queue against block 1 elements
		while (!queue.is_empty() && b1_source >= block_1_start) {
			add_one_compare();
			if (queue.peek() > array[b1_source]) {
				//	element in queue > b1_source
				add_one_assignment();
				array[dest] = queue.dequeue();
				if (b2_max_in_queue) {
					b2_max_location = dest;
					b2_max_in_queue = false;
				}
			} else {
				//	element at b1_source > queue
				add_one_assignment();
				array[dest] = array[b1_source];
				b1_source--;
			}
			dest = next_dest(dest);
		}

		//	transfer any remaining block_2 elements in queue to the array
		while (!queue.is_empty()) {
			add_one_assignment();
			array[dest] = queue.dequeue();
			if (b2_max_in_queue) {
				b2_max_location = dest;
				b2_max_in_queue = false;
			}
			dest = next_dest(dest);
		}
		return b2_max_location;
	}

	/*
	 *	ComparesAndMoves swapBlockElements(array, b1_start, b2_start, block_size)
	 *
	 *	usage	blockSwap(array, 40, 20, 10, metrics);
	 *	usage	blockSwap(array, 20, 40, 10);
	 *	usage	blockSwap(array, 20, 30, 15);
	 */

	template <typename T>
	void swapBlockElementsOfEqualSize(T* array,
									 array_size_t block1_start,
									 array_size_t block2_start,
									 array_size_t block_size,
									 SortMetrics  *metrics)
	{
		if (block_size <= 0)
			return;

		//	if the indices are not credible, leave
		if (block1_start < 0 || block2_start < 0) {
			//	TODO - throw an exception
			return;
		}

		for (array_size_t i = 0; i != block_size; i++) {
			SortingUtilities::swap(array, block1_start, block2_start, metrics);
			block1_start++;
			block2_start++;
		}
	}
}	// namespace BlockSort

namespace std{
	std::string to_string(BlockOperations::MergeStrategy strategy);
	std::ostream& operator<<(std::ostream& out, const BlockOperations::MergeStrategy strategy);
}


#endif /* BLOCKSORTELEMENTMOVING_H_ */
