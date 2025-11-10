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

/*
 * 	This file contains the functions for:
 * 		 performing an InsertionSort on	two contiguous blocks.
 * 		 element-wise
 * 	 It also contains the function declarations
 * 	for the functions in other files that merge two blocks through
 * 	rotation (binary, right_to_left, hybrid), an auxiliary buffer
 * 	and using a table.  The files that contain the function definitions
 * 	for those merge strategies are included at the bottom of this file.
 */
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
	 * are done searching right-to-left.  It is possible that right to
	 * left is more efficient if block values are well interleaved */
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

#include "BlockOperationsMergeAuxiliary.h"
#include "BlockOperationsMergeRotation.h"
#include "BlockOperationsMergeTable.h"


#endif /* BLOCKSORTELEMENTMOVING_H_ */
