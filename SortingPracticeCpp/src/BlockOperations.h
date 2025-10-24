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
	#define MERGE_STRATEGY_MAX_STRING_LENGTH	11

namespace BlockOperations
{
	template <typename T>
	using MergeFunction = array_size_t (*)(	T* array,
								 	 	 	 array_size_t block_1_begin,
											 array_size_t block_1_end,
											 array_size_t block_2_begin,
											 array_size_t block_2_end,
											 SortMetrics &metrics);

	template <typename T>
	array_size_t insertionSortPartial(T* array,
									  array_size_t begin,
									  array_size_t ignored,
									  array_size_t mid,
									  array_size_t end,
									  SortMetrics &metrics);

	/* 	Blocks do not have to be continuous nor do they have to be the same size */
	template <typename T>
	array_size_t mergeTwoBlocksElementsByTable(	T* array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics);

	/*	Uses a binary search to identify spans of block_1 that can be
	 * rotated into the midst of the block_2 */
	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_BinarySearch(
												T* array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics);

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
												SortMetrics &metrics);

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
												SortMetrics &metrics);

	/* 	Blocks do not have to be continuous nor do they have to be the same size */
	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBuffer(
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics);

	template <typename T>
	SortMetrics swapBlockElementsOfEqualSize(T* array,
											 array_size_t block1_start,
											 array_size_t block_2_start,
											 array_size_t block_size);

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
									  array_size_t mid,
									  array_size_t end,
									  SortMetrics &metrics)
	{
		array_size_t highest_b_position = end;

		if (begin > end || mid > end) {
			return highest_b_position;
		}

		//	from here to the end of the array
		for (int i = mid; i <= end ; i++) {
			//	keep reseting highest_b_position to the end
			highest_b_position = end;
			// The first time a B_Block element is found that is
			//	in order that means all the elements to the right,
			//	which are in order b/c they came from the same
			//	B_Block, are sorted.  Abort the loop
			metrics.compares++;
			if (array[i-1] <= array[i])
				break;
			T temp = array[i];
			metrics.assignments++;
			int j = i;
			for ( ; j > begin; j--) {
				metrics.compares++;
				//	if the element to the left
				//	  is <= temp, temp goes here
				if (array[j-1] <= temp) {
					array[j] = temp;
					metrics.assignments++;
					highest_b_position = j;
					break;
				}
				//	shift the element to the right
				array[j] = array[j-1];
				metrics.assignments++;
			}
			// if the loop terminated b/c j == begin
			//	there were no elements found <= temp
			//	temp goes at begin
			if (j <= begin) {
				array[begin] = temp;
				metrics.assignments++;
				highest_b_position = begin;
			}
		}
		return highest_b_position;
	}


	/*	Merge blocks using binary searches to identify spans to rotate
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
													SortMetrics &metrics) {
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
			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
														   span_start, span_end,
														   rotate_count);
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
							<< " a_st  " 		<< std::setw(2) << a_unmerged_start
							<< " a_end "		<< std::setw(2)	<< a_unmerged_end
							<< " b_st  "		<< std::setw(2) << b_unmerged_start
							<< " b_end "		<< std::setw(2)	<< b_unmerged_end
							<< " sp_st " 	<< std::setw(2)	<< span_start
							<< " sp_en "		<< std::setw(2)	<< span_end
							<< " rrcnt "	<< std::setw(2)	<< rotate_count
							<< " b_max_pos "
						    << std::setw(2) << b_max_pos << std::endl;
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
	 *
	 *
	 *
	 * The first merge is done using a binary search. Successive merges
	 * are done searching right-to-left.  It is possible that once found,
	 * right_to_left is more efficient since blocks may be interleaved
	 *
	 *
	 *
	 *
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksBy_Rotation_Hybrid(
												T* 			 array,
												array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics) {
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
		//	 	[ M O Q S ] [ A B C D ]	first_a_grater_than_b_end = 0
		//	exit with [ A B C D M O Q S ] and b_max = 3
		if (first_a_greater_than_b_end <= a_unmerged_start) {
			span_start 	= a_unmerged_start;
			span_end 	= b_unmerged_end;
			rotate_count= b_unmerged_end - a_unmerged_end;
			b_span		= b_unmerged_end - b_unmerged_start;
			b_max_pos	= span_start + b_span;	// = a_unmerged_end
			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
													   	   span_start,
														   span_end,
														   rotate_count);
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

			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
													   	   span_start,
														   span_end,
														   rotate_count);
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
			metrics.compares++;
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
			metrics.compares++;
			while (a_i >= a_unmerged_start && array[a_i] > array[b_unmerged_end]) {
				a_i--;
				metrics.compares++;
			}

			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end)
			//			break;
			if (a_i < a_unmerged_start) {
				b_span		= b_unmerged_end - b_unmerged_start;
				span_start 	= a_i + 1;
				span_end 	= b_unmerged_end;
				rotate_count= b_unmerged_end - a_unmerged_end;
				metrics +=
					SortingUtilities::rotateArrayElementsRight(array,
														   	   span_start,
															   span_end,
															   rotate_count);
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
			metrics.compares++;
			while (array[b_i] > array[a_i] && b_i > a_unmerged_end) {
				b_i--;
				metrics.compares++;
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
			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
													   	   span_start,
														   span_end,
														   rotate_count);
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
	 * and moving right to left.
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
												SortMetrics &metrics)
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
			metrics.compares++;
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
			metrics.compares++;
			while (a_i >= a_unmerged_start && array[a_i] > array[b_unmerged_end]) {
				a_i--;
				metrics.compares++;
			}

			//	2.  all elements in a are greater than [b_end]
			//			rotate [start:b_end] by (b_end-a_end)
			//			break;
			if (a_i < a_unmerged_start) {
				b_span		= b_unmerged_end - b_unmerged_start;
				span_start 	= a_i + 1;
				span_end 	= b_unmerged_end;
				rotate_count= b_unmerged_end - a_unmerged_end;
				metrics +=
					SortingUtilities::rotateArrayElementsRight(array,
														   	   span_start,
															   span_end,
															   rotate_count);
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
			metrics.compares++;
			while (array[b_i] > array[a_i] && b_i > a_unmerged_end) {
				b_i--;
				metrics.compares++;
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
			metrics +=
				SortingUtilities::rotateArrayElementsRight(array,
													   	   span_start,
														   span_end,
														   rotate_count);
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


	/*
	 * 	mergeBlocksByTable(	array,
	 * 						block_1_start, block_1_end,
	 * 						blocK_2_start, block_2_end)
	 *
	 * 	This function merges the array elements from [b1_start:b1_end] with
	 * 	  the array elements from [b2_start:b2_end] by using a table to keep
	 * 	  track of where the elements from [b1_start:b1_end] get moved as the
	 * 	  merge proceeds.
	 *
	 * 	It is assumed that the values within each block are in ascending order.
	 *
	 * 	Note that this algorithm does not require the blocks to be contiguous, nor
	 * 	 does it require the size of block_1 to be less than the size of block_2
	 *
	 * 	The algorithm creates a table of where each element from block_1
	 * 	  moves to if it is swapped out of its position in the array.
	 * 	  This table of array indices allows the algorithm to only require
	 * 	  one temporary location for an array element during the swap.  However, it
	 * 	  requires the space necessary for table of indices that initially has
	 * 	  a quantity of elements equal to the size of the block_1
	 *
	 *	Note that updating the table of block_1 indices takes an element-wise search
	 *	  through the table to determine which where a displaced block_1 element is
	 *	  stored in the table.  This is denoted in the following example by t[x] = t[t_ptr]
	 *	  where the block_1 element at 'dst' was stored in table at table index 'x'.
	 *	  Note in the example that block_1 elements often get displaced multiple times.
	 *	  The element in the table at table index [x] which was a 'dst' is now at:
	 *	  	b2_ptr if the source of the swap was an element from block_2,
	 *	  		which do not get displaced out of their original location during merging
	 *	  	t[tpr] if the source of the swap was an element originally in block_1,
	 *	  		which do often get displaced out of their original location during merging
	 *
	 *	In the following discussion
	 *		'b1s' is block_1_start		'b1e' is block_1_end
	 *		'b2s' is blocK_2_start		'b2e' is block_2_end
	 *		b1_ptr is the array index of the location of the next element from block_1
	 *		b2_ptr is the array index of the location of the next element from block_2
	 *		t_ptr  is the index into the table of where block_1 elements have been displaced
	 *		'-' indicates an index that is no longer part of the algorithm
	 *
	 * 	Consider the blocks "BDF" and "ACEG
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [B   D   F  ]   [A   C   E   G  ] ||  m    [ m+0 m+1 m+2 ]    0      m+0       n+0
	 *
	 *  [t[t_ptr=0]=m+0] = 'B'  > [b2_ptr] = 'A' .... xchg(dst, b2_ptr);    t[0]=b2_ptr; b2_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   D   F  ]   [B   C   E   G  ] ||  m+1  [ n+0 m+1 m+2 ]    0      n+0       n+1
	 *
	 *  [t[t_ptr=0]=n+0] = 'B' <= [b2_ptr] = 'C' .... xchg(dst, t[t_ptr]);  t[x=1]=t[t_ptrr]; t_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   F  ]   [D   C   E   G  ] ||  m+2  [ -   n+0 m+2 ]    1      n+0       n+1
	 *
	 *  [t[t_ptr=1]=n+0] = 'D' >  [b2_ptr] = 'C' .... xchg(dst, b2_ptr);    t[x=2]=b2_ptr; b2_ptr++;
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   F   E   G  ] ||  n+0  [ -   n+0 n+1 ]    1      n+0       n+2
	 *
	 * 	[t[tpr=1]=n+0] = 'D' <= [r_rptr] = 'E' ... dst == t[t_ptr=1], no swap occurred, t_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			    table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   F   E   G  ] ||  n+1  [ -   -   n+1 ]    2      n+1       n+2
	 *
	 * 	[t[t_ptr=2]=n+1] = 'F' <= [r_rptr] = 'E' ... xchng(dst, b2_ptr);    t[x=2]=b2_ptr; b2_ptr++
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3			   table 't'
	 * 	b1s     b1e     b2s         b2e  ||  dst  [ 0   1   2   ]    t_ptr  t[t_ptr]  b2_ptr
	 * [A   B   C  ]   [D   E   F   G  ] ||  n+2  [ -   -   -   ]    3       -        n+3
	 *
	 * 	t_ptr has been exhausted because t_ptr = sizeof(table)),
	 * 		therefore all block_1 elements are in their correct location and all unexamined
	 * 		elements are block_2 elements which are in the originial, and correct, locations
	 *
	 *	The displacement of the block_1 elements, often multiple times, causes this algorithm
	 *	  to use more swaps than a merge sort that uses an auxilliary array, which has a
	 *	  guaranteed min & max number of swaps == nlog(n).  It was empirically observed
	 *	  that this algorithm is on the order of swap complexity of insertion sort.
	 *
	 * 	As an example of the number of displacements of block_1 elements that can occur,
	 * 	  consider the blocks "EFG" and "ABCD"
	 *
	 * 	m+0 m+1 m+2     n+0 n+1 n+2 n+3   dst      table            t_ptr  t[t_ptr] b2_ptr
	 * 	E   F   G  ...  A   B   C   D  ||  m+0  [ m+0 m+1 m+2 ]     0      [0]=m+0  n+0
	 * 	A   F   G  ...  E   B   C   D  ||  m+1  [ n+0 m+1 m+2 ]     0      [0]=n+0  n+1
	 * 	A   B   C  ...  E   F   G   D  ||  m+2  [ n+0 n+1 m+2 ]     0      [0]=n+0  n+2
	 * 	A   B   C  ...  D   F   G   E  ||  n+0  [ n+3 n+1 n+2 ]     0      [0]=n+0  n+3
	 * 	A   B   C  ...  D   F   G   E  ||  n+1  [ n+3 n+1 n+2 ]     0      [0]=n+3  n+4
	 *
	 * 	All of the right block values "A:D" have been placed b/c n+4 > right_end = n+3,
	 * 		but the left block values are no longer in sequence
	 *
	 * 	A   B   C  ...  D   F   G   E  ||  n+1  [ n+3 n+1 n+2 ]     0      [0]=n+3  -
	 * 	A   B   C  ...  D   E   G   F  ||  n+2  [ -   n+3 n+2 ]     1      [1]=n+3  -
	 * 	A   B   C  ...  D   E   F   G  ||  n+3  [ -   -   n+3 ]     2      [1]=n+3  -
	 * 	A   B   C  ...  D   E   F   G  ||  n+4  [ -   -   -   ]     3      -        -
	 *
	 * 	t_ptr= has exceeded sizeof(table)=2 and all right blocks are in place, done
	 *
	 * 									Algorithm:
	 * 	nextDestination(dst)
	 *		if (dst == block_1_end)	dst = block_2_start
	 *		else					dst++
	 *
	 * 	dst = block_1_start
	 * 	t_ptr = 0
	 * 	b2_ptr = blocK_2_start
	 * 	while (dst <= right_end) {
	 * 		b1_ptr = table[t_ptr]
	 *
	 * 		if (array[b1_ptr] <= array[b2_ptr] {
	 * 			// the element to be merged came from block_1
	 * 			if (dst != b1_ptr) {
	 * 			 	swap(dst, b1_ptr)
	 * 			 	//	the element block was from block_1
	 * 			 	for (x = t_ptr+1; x != sizeof(table), x++) {
	 * 			 		if(table[x] == dst) {
	 * 			 			table[x] = table[t_ptr]
	 * 			 			break
	 * 			 		}
	 * 			 	}
	 * 			t_ptr++
	 * 			if (t_ptr == sizeof(table))
	 * 				break;
	 * 		} else {
	 * 			// the element came from block_2
	 * 			if (dst != b2_ptr) {
	 * 				swap(dst, b2_ptr)
	 * 				//	block_2 elements always overwrite block_1 elements
	 * 				//	search table to determine if the element
	 * 				//	  displaced element was from block_1
	 * 				for (x = t_ptr; x != sizeof(table), x++) {
	 * 					if(table[x] == dst)
	 * 						table[x] = b2_ptr
	 * 				}
	 * 			}
	 * 			b2_ptr++
	 *		}
	 * 		next_destination(dst)
	 */

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableUpperSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics &metrics);

	template <typename T>
	array_size_t mergeTwoBlocksElementsByTableLowerSmallest(T * array,
											 array_size_t block_1_start,
											 array_size_t block_1_end,
											 array_size_t block_2_start,
											 array_size_t block_2_end,
											 SortMetrics &metrics) {

		/*	**************************************************************	*/
		/*							debug									*/
		/*	**************************************************************	*/

		constexpr bool debug_verbose = false;
		std::stringstream message;

#pragma push_macro("dbg_ln")
#define dbg_ln(msg)\
		do {\
			if (debug_verbose)	{	std::cout << msg << std::endl;	}\
		} while(false)

#pragma push_macro("dbg")
#define dbg(msg)\
		do {\
			if (debug_verbose)	{	std::cout << msg;	}\
		} while(false)

		//	The lambda 'debug_string' cannot be defined until after many
		//	function variables have been declared. This is typed in here
		//	so that it doesn't intrude upon the regular code after
		//	the variables 'array', etc, are defined
#pragma push_macro("define_debug_string")
#define define_debug_string\
		auto debug_string = [&] () -> std::string {\
			std::stringstream result;\
			for (array_size_t i = block_1_start; i <= block_2_end; ) {\
				result << std::setw(3) << /*array[i].key.str() */ " key value not printed " << " ";\
				i++;\
				if (i-1 == block_1_end) {\
					i = block_2_start;\
				}\
			}\
			result << " " << std::setw(3) << block_1_locations_table_index\
				   << " using table [";\
			for (int i = 0; i != block_1_locations_table_size; i++) {\
				if (i < block_1_locations_table_index) {\
					result << " - ";\
				} else {\
					result << std::setw(3) << block_1_locations_table[i];\
				}\
			}\
			result 	<< "] dst " << std::setw(2) << destination_index\
					<< " t_ptr " << std::setw(2) << block_1_locations_table_index\
					<< " b2_ptr " << std::setw(2) << block_2_index;\
			return result.str();\
		};\

		/*	**************************************************************	*/
		/*							lambdas									*/
		/*	**************************************************************	*/

		auto next_destination = [=] (array_size_t _dest) -> array_size_t {
			if (_dest != block_1_end)
				return _dest + 1;
			else
				return block_2_start;
		};

		//	If an element swapped into 'dst' was in the displacement table,
		//	update the table's entry for the element to now be located at 'src'
		auto update_locations_table = [] (array_size_t *table, array_size_t start, array_size_t end,
										  array_size_t dst, array_size_t src) {
			for (array_size_t i = start; i <= end; i++) {
				//	the element in the table which was
				//	previously at 'dst', it is now at 'src'
				if (table[i] == dst) {
					table[i] = src;
					break;
				}
			}
		};

		/*	**************************************************************	*/
		/*							algorithm								*/
		/*	**************************************************************	*/

		//	Keep track of where the final element in block 2 (B_Block) ends up
		//	after the merge.  This is the position of the last element guaranteed
		//	to be in it's final place.  Any A_Block elements at the end of
		//	the array may be larger than the next B_Block's first elements

		array_size_t block_2_end_position 	= block_2_end;
		array_size_t block_1_size 			= block_1_end - block_1_start + 1;
		array_size_t block_2_size 			= block_2_end - block_2_start + 1;

		if (block_1_size <= 0 || block_2_size <= 0) {
			return block_2_end_position;
		}

		//	Build the table of locations of the lower block's elements

		array_size_t block_1_locations_table_size = block_1_size;
		array_size_t block_1_locations_table[block_1_locations_table_size];

		for (array_size_t i = 0, src = block_1_start; i < block_1_locations_table_size; ) {
			block_1_locations_table[i++] = src++;
		}

		//	Perform the merge

		array_size_t block_1_locations_table_index 	= 0;
		array_size_t block_2_index				  	= block_2_start;
		array_size_t destination_index  			= block_1_start;

		//	define the lambda 'debug_string() after the above variables
		//	have been declared
		define_debug_string;

		/*	******************************************************	*/
		/*					the algorithm code						*/
		/*	******************************************************	*/

		while (destination_index <= block_2_end)
		{
			// Point to the current location of the next block_1 element
			//	which may not be stored in its original position b/c
			//	the block_1 element may have been displaced in a previous
			//	pass through this loop.
			array_size_t block_1_index = block_1_locations_table[block_1_locations_table_index];

			metrics.compares++;
			if (array[block_1_index] <= array[block_2_index]) {
				// the value from the left block goes into destination
				if (destination_index != block_1_index) {
					T tmp 					 = array[destination_index];
					array[destination_index] = array[block_1_index];
					array[block_1_index] 	 = tmp;
					metrics.assignments += 3;

					// Update the table location of the entry that was just displaced,
					//	which will be somewhere in the table after the current entry
					update_locations_table(block_1_locations_table,
								   	   	   block_1_locations_table_index+1, block_1_locations_table_size-1,
										   destination_index, block_1_index);
				}

				if (debug_verbose) message   << "left:  " << debug_string() << std::endl;

				destination_index = next_destination(destination_index);

				//	if we have moved / merged all of the block_1 elements, we are done
				if (++block_1_locations_table_index == block_1_locations_table_size) {
					if (debug_verbose) message << "Terminated due to table_index == table_size " << std::endl;
					break;
				}
			}
			else
			{
				// value from the right block is < value from the left block
				T tmp 					 = array[destination_index];
				array[destination_index] = array[block_2_index];
				array[block_2_index] 	 = tmp;
				metrics.assignments += 3;

				if (block_2_index == block_2_end_position) {
					block_2_end_position = destination_index;
				}

				//	Update the table entry of the location of the element
				//	that was just displaced, which may be in any position in the table
				update_locations_table(block_1_locations_table,
								   	   block_1_locations_table_index, block_1_locations_table_size-1,
									   destination_index, block_2_index);

				if (debug_verbose) message << "right: " << debug_string() << std::endl;

				destination_index = next_destination(destination_index);

				//	if all the elements from block_2 are in place, break loop
				if (++block_2_index > block_2_end) {
					if (debug_verbose) message << "Terminated due to block_2_index > block_2_end" << std::endl;
					break;
				}
			}
		}

		//	If the while loop terminated because all block_2 elements are now in place,
		//	  it is possible that there are displaced block_1 elements that are not
		//	  in order.  Reorder any remaining block_1 values that have been displaced
		while(destination_index <= block_2_end &&
			  block_1_locations_table_index < block_1_locations_table_size)
		{
			if (debug_verbose) message << "flush: " << debug_string() << std::endl;

			array_size_t block_1_index	= block_1_locations_table[block_1_locations_table_index];
			T tmp 					= array[destination_index];
			array[destination_index]= array[block_1_index];
			array[block_1_index]	= tmp;
			metrics.assignments += 3;
			//	update the table's contents from AFTER the element that was just stored
			update_locations_table(block_1_locations_table,
								   block_1_locations_table_index+1, block_1_locations_table_size-1,
								   destination_index, block_1_index);
			block_1_locations_table_index++;
			destination_index = next_destination(destination_index);
		}

		if (debug_verbose)	std::cout << message.str() << std::endl;
		return block_2_end_position;

#pragma pop_macro("define_debug_string")
#pragma pop_macro("dbg")
#pragma pop_macro("dbg_ln")
	}


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
											 SortMetrics &metrics) {
		//	trap all errors
		if (block_1_end < block_1_start)	return 0;
		if (block_2_end < block_2_start)	return 0;

		array_size_t u_size = block_1_end - block_1_start + 1;
		array_size_t v_size = block_2_end - block_2_start + 1;

		if (true || u_size <= v_size) {
			return mergeTwoBlocksElementsByTableLowerSmallest(array,
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


	/*	******************************************************************	*/
	/*																		*/
	/*		Merging blocks by allocating an auxiliary buffer that is the	*/
	/*	size of the smaller buffer.  Elements are only moved into the 		*/
	/*	auxiliary buffer whenever the element is displaced b/c its location	*/
	/*	is the destination of the merged (incoming) element					*/
	/*																		*/
	/*	Two helper functions exist: one for when the first block is equal	*/
	/*	  to or smaller than the second block, and one when the second		*/
	/*	  block is equal to or smaller than the first block.				*/
	/*	  The buffered block needs to be the smaller one to ensure that the	*/
	/*	  loop in the merge algorithm goes through ALL of the other larger	*/
	/*	  block.															*/
	/*	******************************************************************	*/

	/*	This version treats the lower as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_LOWER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics);

	/*	This version treats the upper as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_UPPER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics);

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
												SortMetrics &metrics)
	{
		array_size_t block_1_size = block_1_end - block_1_start + 1;
		array_size_t block_2_size = block_2_end - block_2_start + 1;
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


	/*	version that treats the lower as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_LOWER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics) {

		bool debug_verbose = false;
		if (debug_verbose) {
			std::cout << "Entering 'byAuxiliary_LOWER'" << std::endl;
		}

		array_size_t block_1_size 		= block_1_end-block_1_start+1;
		T auxiliary[block_1_size];
		array_size_t aux_dst			= 0;
		array_size_t aux_src			= 0;
		array_size_t b_2_src			= block_2_start;
		array_size_t dst				= block_1_start;
		array_size_t b_max_pos			= block_2_end;


		auto isAuxEmpty = [&] () -> bool {	return aux_dst == aux_src; 	};
		auto isDstInB1	= [&] () -> bool {	return dst <= block_1_end;	};
		auto nextDst	= [&] () -> array_size_t {
			return dst != block_1_end ? dst + 1 : block_2_start; };

		auto debug = [&] (std::string trailer) -> std::string {
			array_size_t dbg_sz = block_2_end - block_2_start + 1
								+ block_1_end - block_1_start + 1;
			std::stringstream msg;
			msg << SortingUtilities::arrayElementsToString(array, dbg_sz);
			msg << " dst = " 		<< std::setw(2) << dst
				<< " aux_src = " 	<< std::setw(2) << aux_src
				<< " aux_dst = " 	<< std::setw(2) << aux_dst
				<< " b_2_src = "  	<< std::setw(2) << b_2_src
				<< " aux: ";
			for (int i = aux_src; i < aux_dst; i++) {
				msg << std::setw(2) << " auxiliary value not printed " /*auxiliary[i]*/ << " ";
			}
			msg << trailer;
			return msg.str();
		};

		//	while there are still block_1 values unexamined	dst <= block_1_end
		//	 or   there are still block_1 values unmerged	!isAuxEmpty()
		//	i.e. - once dst goes past block_1_end, the only remaining
		//			unmerged block_1 values are in 'auxiliary[]'
		while (dst <= block_1_end || !isAuxEmpty())
		{
			if (debug_verbose) {
				std::cout << debug(" top of loop") << std::endl;
			}
			//	block_1 value is taken
			if (!isAuxEmpty()) {
				// block_1 element has previously been moved to auxiliary
				//	  prefer block_1 over block_2 to preserve stability
				metrics.compares++;
				if (auxiliary[aux_src] <= array[b_2_src]) {
					//	if the destination is a b1 value, move it to aux
					if (isDstInB1()) {
						metrics.assignments++;
						auxiliary[aux_dst++] = array[dst];
					}
					metrics.assignments++;
					array[dst] = auxiliary[aux_src++];
					dst = nextDst();
				} else {
					//	the element comes from block 2
					if (isDstInB1()) {
						metrics.assignments++;
						auxiliary[aux_dst++] = array[dst];
					}
					if (dst != b_2_src) {
						metrics.assignments++;
						array[dst] 		= array[b_2_src];
					}
					//	final b has been moved into position, b2 is exhausted
					if (b_2_src == b_max_pos) {
						b_max_pos 	= dst;
						dst 		= nextDst();
						//	copy what remains of block_1 to end of array
						while (!isAuxEmpty()) {
							metrics.assignments++;
							array[dst] 	= auxiliary[aux_src];
							dst 		= nextDst();
							aux_src++;
						}
						break;
					}
					//	there are remaining elements in block_2
					dst = nextDst();
					b_2_src++;
				}
			} else {
				//	auxiliary is empty - block_1 value comes from dst
				metrics.compares++;
				if (array[dst] <= array[b_2_src]) {
					dst = nextDst();
				} else {
					// block_2 value is taken
					if (isDstInB1()) {
						metrics.assignments++;
						auxiliary[aux_dst++] = array[dst];
					}
					//	if the b value is moved from further along
					if (dst != b_2_src) {
						metrics.assignments++;
						array[dst] 		= array[b_2_src];
					}
					//	If final b2 value has been moved into place, copy
					//	  what remains of block_1 into the end of the array
					if (b_2_src == b_max_pos) {
						b_max_pos 	= dst;
						dst 		= nextDst();
						while (!isAuxEmpty()) {
							metrics.assignments++;
							array[dst] = auxiliary[aux_src++];
							dst = nextDst();
						}
						break;
					}
					b_2_src++;
					dst = nextDst();
				}
			}
			if (debug_verbose) {
				std::cout << debug(" bottom of loop") << std::endl;
			}
		}
		if (debug_verbose) {
			std::cout << "Exiting 'byAuxiliary_LOWER'" << std::endl;
		}
		return b_max_pos;
	}

	/*	version that treats the upper as being the smaller block	*/

	template <typename T>
	array_size_t mergeTwoBlocksElementsUsingAuxiliaryBufferFor_UPPER (
												T * array,
											 	array_size_t block_1_start,
												array_size_t block_1_end,
												array_size_t block_2_start,
												array_size_t block_2_end,
												SortMetrics &metrics) {

		bool debug_verbose = false;
		if (debug_verbose) {
			std::cout << "Entering 'byAuxiliary_UPPER'" << std::endl;
		}

		array_size_t block_2_size		= block_2_end - block_2_start + 1;
		T auxiliary[block_2_size];
		array_size_t aux_dst			= block_2_size-1;
		array_size_t aux_src			= block_2_size-1;
		array_size_t b_1_src			= block_1_end;
		array_size_t dst				= block_2_end;
		array_size_t b_max_pos			= block_2_end;
		array_size_t b_max_locked		= false;

		auto debug = [&] (std::string trailer) -> std::string {
			array_size_t dbg_sz = block_2_end - block_2_start + 1
								+ block_1_end - block_1_start + 1;
			std::stringstream msg;
			msg << SortingUtilities::arrayElementsToString(array, dbg_sz);
			msg << " dst = " 		<< std::setw(2) << dst
				<< " aux_src = " 	<< std::setw(2) << aux_src
				<< " aux_dst = " 	<< std::setw(2) << aux_dst
				<< " b_1_src = "  	<< std::setw(2) << b_1_src
				<< " aux: ";
			for (int i = aux_dst+1; i < block_2_size; i++) {
				msg << std::setw(2) << "auxiliary[i] not printed" /* auxiliary[i]*/ << " ";
			}
			msg << trailer;
			return msg.str();
		};

		auto isAuxEmpty = [&] () -> bool	{ 	return aux_src == aux_dst;	 };
		auto isDstB2	= [&] () -> bool	{ 	return dst >= block_2_start; };
		auto nextDst	= [&] () -> array_size_t {
			return dst != block_2_start ? dst - 1 : block_1_end;
		};

		while (dst >= block_2_start || !isAuxEmpty()) {
			if (debug_verbose) {
				std::cout << debug(" top of loop") << std::endl;
			}
			if (!isAuxEmpty()) {
				//	if block_1 has the higher value
				metrics.compares++;
				if (array[b_1_src] > auxiliary[aux_src]) {
					//	if the dst is in block_2, the value at
					//	dst will be displaced
					if (isDstB2()) {
						metrics.assignments++;
						auxiliary[aux_dst--] = array[dst];
					}
					metrics.assignments++;
					array[dst] = array[b_1_src];
					b_1_src--;
					dst = nextDst();
					// if all of the block_1 values have been placed,
					//	flush the auxiliary (displaced) buffer
					if (b_1_src < block_1_start) {
						//	this flush may be first placed b2
						if (!b_max_locked) {
							b_max_pos 	 = dst;
							b_max_locked = true;
						}
						while (!isAuxEmpty()) {
							metrics.assignments++;
							array[dst] = auxiliary[aux_src];
							aux_src--;
							dst = nextDst();
						}
						break;
					}
				} else {
					//	the value in block_2 is greater or equal to block_1
					//	if this is the first value from block_2 to be
					//		placed, assign b_max_pos
					if (!b_max_locked) {
						b_max_pos = dst;
						b_max_locked = true;
					}
					//	if the destination is in block_2, it will
					//	be displaced by a greater block_2 value
					if (isDstB2()) {
						metrics.assignments++;
						auxiliary[aux_dst--] = array[dst];
					}
					metrics.assignments++;
					array[dst] = auxiliary[aux_src];
					aux_src--;
					dst = nextDst();
				}
			} else {
				//	if block_1 has the higher value,
				metrics.compares++;
				if (array[b_1_src] > array[dst]) {
					//	if dst is in block_2, the element at dst will be displaced
					if (isDstB2()) {
						metrics.assignments++;
						auxiliary[aux_dst] = array[dst];
						aux_dst--;
					}
					metrics.assignments++;
					array[dst] = array[b_1_src];
					b_1_src--;
					dst = nextDst();
					//	if all of b_1 has been merged, flush
					//	auxiliary into the remaining array positions
					if (b_1_src < block_1_start) {
						//	this flush may be first placed b2
						if (!b_max_locked) {
							b_max_pos 	 = dst;
							b_max_locked = true;
						}
						while (!isAuxEmpty()) {
							metrics.assignments++;
							array[dst] = auxiliary[aux_src];
							aux_src--;
							dst = nextDst();
						}
						break;
					}
				} else {
					//	the element in block_2 is the larger,
					//		which is already at dst
					//	if this is the first time a block_2
					//	has been placed in its final position,
					//	store it as the max b postion
					if (!b_max_locked) {
						b_max_pos 	 = dst;
						b_max_locked = true;
					}
					dst = nextDst();
				}
			}
			if (debug_verbose) {
				std::cout << debug(" bottom of loop") << std::endl;
			}
		}
		return b_max_pos;
	}

	/*
	 *	ComparesAndMoves swapBlockElements(array, b1_start, b2_start, block_size)
	 *
	 *	usage	num_ops = blockSwap(array, 40, 20, 10);
	 *	usage	num_ops = blockSwap(array, 20, 40, 10);
	 *	usage	num_ops = blockSwap(array, 20, 30, 15);
	 */

	template <typename T>
	SortMetrics swapBlockElementsOfEqualSize(T* array,
											 array_size_t block1_start,
											 array_size_t block2_start,
											 array_size_t block_size)
	{
		SortMetrics result(0,0);

		if (block_size <= 0)
			return result;

		//	if the indices are not credible, leave
		if (block1_start < 0 || block2_start < 0) {
			//	TODO - throw an exception
			return result;
		}

		for (array_size_t i = 0; i != block_size; i++) {
			result += SortingUtilities::swap(array, block1_start, block2_start);
			block1_start++;
			block2_start++;
		}

		return result;
	}

}	// namespace BlockSort

namespace std{
	std::string to_string(BlockOperations::MergeStrategy strategy);
	std::ostream& operator<<(std::ostream& out, const BlockOperations::MergeStrategy strategy);
}


#endif /* BLOCKSORTELEMENTMOVING_H_ */
