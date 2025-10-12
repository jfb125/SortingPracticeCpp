/*
 * BlockSortElementMoving.h
 *
 *  Created on: Oct 6, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTELEMENTMOVING_H_
#define BLOCKSORTELEMENTMOVING_H_

#include "BlockSortDataTypes.h"
#include "SortingUtilities.h"

namespace BlockSort {

	/*	Performs an insertion sort starting with the 'mid' element.  This is used
	 * In cases where all of the elements to the left of mid are in order, but
	 * elements to the right of 'mid' may be smaller than elements on the left. */
//	template <typename T>
//	SortMetrics insertionSortPartial(T** array,
//										  array_size_t begin, array_size_t mid, array_size_t end);

	/*
	 * 	Blocks must be contiguous
	 */
//	template <typename T>
//	SortMetrics mergeContiguousElementsByRotating(T** array, array_size_t start,
//																array_size_t mid, array_size_t end);
//

	template <typename T>
	SortMetrics swapBlockElementsOfEqualSize( T** array,
										array_size_t block1_start, array_size_t block_2_start,
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
	//	passed to them, such as mergeToAdjacentBlocksByRotation or
	//	mergeTwoBlocksElementsByTable
	template <typename T>
	array_size_t insertionSortPartial(T** array,
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
			if (*array[i-1] <= *array[i])
				break;
			T* temp = array[i];
			metrics.assignments++;
			int j = i;
			for ( ; j > begin; j--) {
				metrics.compares++;
				//	if the elemet to the left
				//	  is <= temp, temp goes here
				if (*array[j-1] <= *temp) {
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

#if 0
	/*
	 * ComparesAndMoves mergeBlocksByRotating(array, start, mid, end);
	 *
	 *		start 	is the index of the start of the block on the left
	 *		mid		is the index of the start of the block on the right,
	 *				  which is one past the end of the block on the left
	 *		end		is the index of the end of the block on the right
	 *
	 *   This uses rotate operations to put the { u[], v[] } array in order
	 *   The requirement is that the sub portions of uv_array, u[] and v[]
	 *   	are in ascending order.
	 *   		Consider an array where start = 5, mid = 9 & end is equal to 12
	 *   					 s           m        e
	 *   		             5  6  7  8  9 10 11 12
	 *      then array[] = { B, C, C, D, A, C, G, G }
	 *
	 *   The algorithm looks for all elements in [mid:end] that are less
	 *   	than the current element in u, then rotates all those elements
	 *   	in v that are less than u.  This is equivalent to finding the
	 *   	first element in v that is > u.
	 *      	start with u = start = 5 & v = mid = 9, count = 0
	 *
	 *      Search u for a value > v
	 *        5  6  7  8  9 10 11 12
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' > arr[v= 9].'A'       count = 0
	 * 		Search v for a value < u
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' <= arr[v= 9].'A' v++  count++;
	 * 		{ B, C, C, D, A, C, G, G }	arr[u=5].'B' >  arr[v=10].'C'      count = 1
	 * 		arrayRotate(array, count = 1, start = u = 5, end = v-1 = 10 -1 = 9)
	 * 		{ A, B, C, C, E, C, G, G }

	 * 		Search u for a value > v
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=6].'B' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=7].'C' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=8].'C' <= arr[v=10].'C', u++ count = 0
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=10].'C',     count = 0
	 * 		Search v for a  value < u
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=10].'C', v++ count++
	 * 		{ A, B, C, C, E, C, G, G }	arr[u=9].'E' <= arr[v=11].'G'      count = 1
	 * 		arrayRotate(array, count = 1, start = u = 9, end = v-1 = 11 - 10)
	 * 		{ A, B, C, C, C, E, G, G }
	 *
	 * 		Search u for a value > v
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=9].'E' <= arr[v=11].'G' u++  count = 0
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=10].'E' <= arr[v=11].'G' u++  count = 0
	 * 		{ A, B, C, C, C, E, G, G }	arr[u=11].'E' <= arr[v=11].'G' u++  count = 0
	 * 		STOP u == v
	 */

	template <typename T>
	SortMetrics mergeContiguousElementsByRotating(T** array, array_size_t start, array_size_t mid, array_size_t end) {

		bool debug_verbose = false;

		if(debug_verbose) {
			std::cout << __FUNCTION__ << std::endl;
		}

		SortMetrics metrics(0,0);

		array_size_t u = mid-1;
		array_size_t v = end;
		array_size_t rotate_count;

		//	These choose between:
		//	  PURE_RIGHT_TO_LEFT	Always start from right-to-left both u & v indices
		//	  PURE_BINARY			Always use a binary search
		//	  HYBRID				Binary search first time, the right-to-left
		bool binary_search 					= true;
		bool binary_search_first_time_only	= true;
		//	binary_search		binary_search_first_time_only		mode
		//		false					d.c.						PURE_RIGHT_TO_LEFT
		//		true					false						PURE_BINARY
		//		true					true						HYBRID

		while (u >= start) {

			array_size_t lower_shift_bound;

			if (!binary_search) {
				//	find the first [v] that is less than [u]
				while (v > u) {
					metrics.compares++;
					if (*array[u] > *array[v])
						break;
					--v;
				}
				//	If v is equal to u, then all elements in [v+1:end]
				//	  and all elements [start:u] are in order
				//	  therefore the array is in order
				if (u==v)
					break;
				//	u is pointing to a value that is > v
				//	it is possible that lesser values to the left of [u]
				//	  are also > [v]. Keep moving --u until a value is
				//	  found that is <= [v].  At that point, everything
				//	  between [u+1] to [v] is greater than [v]
				rotate_count = -1;
				u--;
				while (u >= start) {
					metrics.compares++;
					if (*array[u] <= *array[v]) {
						break;
					}
					--u;
					--rotate_count;
				}
				lower_shift_bound = u+1;
			} else {
				//	find the rightmost (least) value remaining in v that is > u
				v = SortingUtilities::binarySearchFirstElement(array, u+1, v, array[u], metrics);
				//	the values to rotate are to the left of the value > u
				v--;
				//	it there were no values in v that are less than u, done
				if (u==v)
					break;
				//	find the leftmost (least) value in u that is > v
				lower_shift_bound =
					SortingUtilities::binarySearchLastElement(array, start, u, array[v], metrics);
				rotate_count = -(u-lower_shift_bound+1);
				u = lower_shift_bound-1;
				if (binary_search_first_time_only)
					binary_search = false;
			}

			if (debug_verbose) {
				std::cout << "BEFORE\n";
				std::cout << "start " << start << " end " << end
						  << " u+1 " << u+1 << " v " << v
						  << " rotating " << rotate_count
						  << std::endl;
				for (int i = start; i <= end; i++) {
					std::cout << std::setw(3) << i << ' ';
				}
				std::cout << std::endl;
				std::cout << SortingUtilities::arrayElementsToString(&array[start], end-start+1, 3, 4)
						  << std::endl;
			}
			//	All elements between {u+1:v} are greater than v, and [u] <= [v].
			//	Rotate left a sufficient amove to the element a [u+1] to the right of [v]
			//	Rotate_count is < 0, which indicates a shift left
			metrics += SortingUtilities::rotateArrayElementsRight(array, lower_shift_bound, v, rotate_count);

			if (debug_verbose) {
				std::cout << "AFTER\n";
				std::cout << SortingUtilities::arrayElementsToString(&array[start], end-start+1, 3, 4)
						  << std::endl;
			}
			//    Point to the v element that is one before the element
			// that u was pointing to which has been rotated left
			//
			//	Consider assume that the value at u6 which is in position 12
			//	     is greater than the value at v4 which is in position 17
			//		 rotate count will be -2 (left)
			//			  11 12 13 14 15 16 17 18 19
			//		Was:  u5 u6 u7 v1 v2 v3 v4 v5 v6
			//		Is:	  u5 v1 v2 v3 v4 u6 u7 v5 v6
			//		v4 has moved from 17 to 15
			v = v + rotate_count;
		}

		if (debug_verbose)
			std::cout << "Exiting mergeBlocksByRotation()\n\n";

		return metrics;
	}
#endif
	/*
	 *	ComparesAndMoves swapBlockElements(array, b1_start, b2_start, block_size)
	 *
	 *	usage	num_ops = blockSwap(array, 40, 20, 10);
	 *	usage	num_ops = blockSwap(array, 20, 40, 10);
	 *	usage	num_ops = blockSwap(array, 20, 30, 15);
	 */

	template <typename T>
	SortMetrics swapBlockElementsOfEqualSize(
										T** array,
										array_size_t block1_start, array_size_t block2_start,
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

		T* temp;
		for (array_size_t i = 0; i != block_size; i++) {
			result.assignments += 3;
			temp = array[block1_start];
			array[block1_start] = array[block2_start];
			array[block2_start] = temp;
			block1_start++;
			block2_start++;
		}

		return result;
	}

}	// namespace BlockSort

#endif /* BLOCKSORTELEMENTMOVING_H_ */
