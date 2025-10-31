/*
 * BlockSortBlockSwapping.h
 *
 *  Created on: Oct 6, 2025
 *      Author: joe
 */

#ifndef BLOCKSORTBLOCKSWAPPING_H_
#define BLOCKSORTBLOCKSWAPPING_H_

#include "BlockSortDataTypes.h"
#include "BlockSortBlockDescriptors.h"
#include "BlockSortIncludes.h"

/*
 * 	  These move the descriptors and the underlying array elements in the block
 * 	according to different algorithms.  If the function only moves the block
 * 	descriptors, but not the underlying array, the word _ONLY_ is in the name
 */

namespace BlockSort {

	/*
	 * 	This is used to find the rightmost A_Block that is greater than the
	 * B_Block in question.  B blocks should go the right of all A_Blocks with
	 * equal or lesser keys.
	 */
	template <typename T>
	void binarySearchLastBlock(Descriptors<T> &descriptors,
							   array_size_t range_left,
							   array_size_t range_right,
						   	   T &key,
							   array_size_t &key_location,
							   SortMetrics *metrics = nullptr);

	/*
	 *  Rotates the block descriptors, and the underlying array, to the right
	 */
	template <typename T>
	void rotateBlocksRight(T* array,
						   Descriptors<T> &descriptors,
						   array_size_t first,
						   array_size_t last,
						   array_size_t block_rotate_count,
						   SortMetrics *metrics = nullptr);

	/*
	 * 	This rotates Descriptor positions within the list of Descriptors
	 * 	but does not rotate the underlying array.  rotate_count < 0 is rotate_left
	 */

	template<typename T>
	void rotate_ONLY_BlockDescriptorsRight(	Descriptors<T> &descriptors,
											array_size_t list_start,
											array_size_t list_end,
											array_size_t rotate_count,
											SortMetrics *metrics = nullptr);

	/*	Swaps two blocks and the underlying array regardless of the relative size
	 * of each block.  Calls the appropriate EqualSize, LeftSmallerThanRight or
	 * RightSmallerThanLeft
	 */
	template <typename T>
	void swapBlocks(T* array,
					Descriptors<T> &descriptors,
					array_size_t i,
					array_size_t j,
					SortMetrics *metrics = nullptr);

	template <typename T>
	void swapBlocksLeftSmallerThanRight(T* array,
										Descriptors<T> &descriptors,
										array_size_t u,
										array_size_t v,
										SortMetrics *metrics = nullptr);

	template <typename T>
	void swapBlocksOfEqualSize(			T* array,
										Descriptors<T> &descriptors,
										array_size_t u,
										array_size_t v,
										SortMetrics *metrics = nullptr);

	template <typename T>
	void swapBlocksRightSmallerThanLeft(T* array,
										Descriptors<T> &descriptors,
										array_size_t u,
										array_size_t v,
										SortMetrics *metrics = nullptr);

	/*
	 * 	This swaps block descriptors, but DOES NOT move the underlying array
	 */
	template<typename T>
	void swap_ONLY_BlockDescriptors(Descriptors<T> &descriptors,
									array_size_t i,
									array_size_t j,
									SortMetrics *metrics = nullptr);

	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							Algorithm functions								*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*
	 *	metrics binarySearchLastBlock(p_blocks, start, end, p_key, p_location);
	 *
	 *	  Perform a binary search for the index of the left-most block that is greater
	 *	than the passed 'key' on the array_size_terval [start:end].
	 *
	 *	If *p_key is <= than *p_blocks[start].key, 	'start' is assigned to p_location
	 *	If *p_key is >  than *p_blocks[end].key, 	'end+1' is assigned to p_location
	 *
	 *	The number of compares of p_key is returned for diagnostic purposes
	 */


	template <typename T>
	void binarySearchLastBlock(Descriptors<T> &blocks,
							   array_size_t range_left,
							   array_size_t range_right,
						   	   T &key,
							   array_size_t &key_location,
							   SortMetrics *metrics)
	{
		//	debugging resources
		constexpr bool 		debug_verbose = false;
		OStreamState 		ostream_state;
		std::stringstream 	msg;

		array_size_t start 	= range_left;
		array_size_t end	= range_right;
		while (start != end) {
			//	debug
			array_size_t mid = start + (end-start)/2;
			if (debug_verbose) {
				msg << "start = " 		<< std::setw(2) << start
					<< " end = " 		<< std::setw(2) << end
					<< " evaluating [" 	<< std::setw(2) << mid
//					<< "] = " 			<< std::setw(2) << blocks[mid].key
					<< "] = " 			<< std::setw(2) << " key not prarray_size_ted "
					<< (blocks[mid].key > key ? " is    " : " is NOT")
//					<< " greater than " << key;
					<< " greater than " << " key not prarray_size_ted ";
			}

			if (metrics) metrics->compares++;
			if (blocks[mid].key <= key) {
				start = mid+1;
			} else {
				end = mid;
			}

			if (debug_verbose) {
				//	debug stuff
				msg << " next guess = " << std::setw(2) << mid << std::endl;
			}
		}

		//	  If the search ended on the right side of the array
		//	and the element at the right side IS NOT > key,
		//	return the element after the right side.
		//	  It is the caller's responsibility to ensure the range_right+1
		//	is within the bounds of the caller's array of blocks
		if (metrics) metrics->compares++;
		if (start == range_right && blocks[start].key <= key) {
			msg << "start = " 				<< std::setw(2) << start
				<< " end = " 				<< std::setw(2) << end
				<< " evaluating [" 			<< std::setw(2) << start
//				<< "] = " << std::setw(2) 	<< blocks[start].key
				<< "] = " << std::setw(2) 	<< " key value not prarray_size_ted "
				<< (blocks[start].key > key ? " is    " : " is NOT")
//				<< " greater than " << key
				<< " greater than " << " key value not prarray_size_ted "
				<< " therefore returning 'end+1'\n";
			start = start+1;
		} else {
			msg << "start = " 				<< std::setw(2) << start
				<< " end = " 				<< std::setw(2) << end
				<< " evaluating [" 			<< std::setw(2) << start
//				<< "] = " << std::setw(2) 	<< blocks[start].key
				<< "] = " << std::setw(2) 	<< " key value not prarray_size_ted "
				<< (blocks[start].key > key ? " is    " : " is NOT")
//				<< " greater than " << key
				<< " greater than " << " key value not prarray_size_ted "
				<< " therefore returning 'end'\n";
		}

		key_location = start;

		if (debug_verbose) {
			msg << " returning " << start << std::endl;
			std::cout << msg.str();
		}
	}


	template <typename T>
	void rotateBlocksRight(	T* array,
							Descriptors<T> &descriptors,
					        array_size_t first,
							array_size_t last,
							array_size_t block_rotate_count,
							SortMetrics *metrics)
	{
		/*	**************************************	*/
		/*				Debugging					*/
		/*	**************************************	*/

#pragma push_macro("rbr_dbg")
#if 0
#define rbr_dbg(msg) do {\
		std::cout << msg << std::endl;\
		std::cout << "first = " 	<< first\
				  << ", last = " 	<< last\
				  << ", count = " 	<< block_rotate_count\
				  << std::endl;\
		/*	array indices	*/\
		for (array_size_t i =  descriptors[first].start_index;\
				 i <= descriptors[last].end_index;\
				 i++) {\
			std::cout << std::setw(3) << i << " ";\
			}\
		std::cout << std::endl;\
		/*	array elements	*/\
		for (array_size_t i =  descriptors[first].start_index;\
				 i <= descriptors[last].end_index;\
				 i++) {\
			std::cout << std::setw(3) << array[i] << " ";\
		}\
		std::cout << std::endl;\
		/*	descriptors		*/\
		for (array_size_t i = first ; i < last; i++) {\
			std::cout << descriptors[i];\
		}\
	} while (false)
#endif
#define rbr_dbg(msg)

		/*	**************************************	*/
		/*				function logic				*/
		/*	**************************************	*/

		//	determine the start & end tag
		if (last == first) {
			return;
		}

		//	if they are in the opposite order, correct them
		if (last < first) {
			array_size_t tmp= first;
			first 			= last;
			last 			= tmp;
		}

		//	Convert the rotate_count to a positive (right) number < span
		array_size_t descriptor_span = last - first + 1;
		block_rotate_count = calcRotationWithModulo(block_rotate_count, descriptor_span);
		if (block_rotate_count == 0) {
			return;
		}

		//	Calculate the span and amount that the underlying array will rotate
		array_size_t first_start_index 	= descriptors[first].start_index;
		array_size_t last_end_index		= descriptors[last].end_index;
		//	Calculate how far the array elements on the right of the array span
		//	  will be rotated off of the end of the span back to the start
		array_size_t array_rotate_count = 0;
		for (array_size_t count = block_rotate_count, i = last; count > 0; --count, --i) {
			array_rotate_count += descriptors[i].getWidth();
		}

		/*	rotate the underlying array	*/
		rbr_dbg("Before rotate");
		SortingUtilities::rotateArrayElementsRight(array,
												   first_start_index,
												   last_end_index,
												   array_rotate_count,
												   metrics);
		rbr_dbg("After element rotate");

		/*	rotate the descriptors	*/
		rotate_ONLY_BlockDescriptorsRight(descriptors, first, last,
										  block_rotate_count, metrics);
		rbr_dbg("After block rotate");

		/*	assign indices	*/

		// 	Both array and descriptors have been rotated, so the keys are correct.
		//	Block indices still poarray_size_t to original position in the array.

		array_size_t _start_index = first_start_index;
		for (array_size_t i = first; i <= last; i++) {
			array_size_t _end_index 	= _start_index + descriptors[i].getWidth() - 1;
			descriptors[i].start_index 	= _start_index;
			descriptors[i].end_index   	= _end_index;
			_start_index 				= _end_index + 1;
		}

#pragma pop_macro("rbr_dbg")
	}

	/*
	 * 	rotates a list of descriptors to the right by rotate_count.
	 * 	If rotate_count is negative, the rotation is to the left
	 */
	template<typename T>
	void rotate_ONLY_BlockDescriptorsRight(	Descriptors<T> &descriptors,
											array_size_t start,
											array_size_t end,
											array_size_t rotate_count,
											SortMetrics *metrics)
	{
		auto debug = [&] () {
			if(false) {
				std::cout << "{ ";
				for (array_size_t i = start; i != end-1; i++) {
					std::cout << descriptors[i] << ", ";
				}
				std::cout << descriptors[end] << " }\n";
			}
		};

		//	convert a negative (left) rotate count array_size_to equivalent right count
		rotate_count = calcRotationWithModulo(rotate_count, end-start+1);

		//	{ 0, 1, 2, 3, 4 }	rotate count = 2

		// swap entire list of BlockDescriptors
		for (array_size_t i = start, j = end; i < j; ++i, --j) {
			swap_ONLY_BlockDescriptors(descriptors, i, j, metrics);
		}
		debug();
		//	{ 4, 3, 2, 1, 0 }

		// swap (restore) left side
		for (array_size_t i = start, j = start+ rotate_count-1; i < j; ++i, --j) {
			swap_ONLY_BlockDescriptors(descriptors, i, j, metrics);
		}
		debug();
		//	{ 3, 4, 2, 1, 0 }

		// swap (restore) right side
		for (array_size_t i = start+rotate_count, j = end; i < j; ++i, --j) {
			swap_ONLY_BlockDescriptors(descriptors, i, j, metrics);
		}
		debug();
	}


	/*
	 * 	swapBlocks(array, descriptors, u, v);
	 *
	 * This swaps the block descriptors [u] & [v] and the underlying array elements.
	 * It compares the sizes of the two blocks and determines which function to call
	 * 	swapBlocksOfEqaulSize()
	 * 	swapBlocksLeftSmallerThanRight()
	 * 	swapBlocksRightSmallerThanLeft()
	 */

	template <typename T>
	void swapBlocks(T* array,
					Descriptors<T> &descriptors,
					array_size_t u,
					array_size_t v,
					SortMetrics *metrics) {

		if (u == v) {
			return;
		}

		//	ensure u is to the left of v
		if (v < u) {
			array_size_t temp = u;
			u = v;
			v = temp;
		}

		array_size_t u_width = descriptors[u].getWidth();
		array_size_t v_width = descriptors[v].getWidth();
		if (u_width == v_width) {
			swapBlocksOfEqualSize(array, descriptors, u, v, metrics);
		} else {
			if (u_width < v_width) {
				swapBlocksLeftSmallerThanRight(array, descriptors, u, v, metrics);
			} else {
				swapBlocksRightSmallerThanLeft(array, descriptors, u, v, metrics);
			}
		}
		return;
	}

	/*
	 * 	ComparesAndMoves rotateBlocksRight(array, tags, first, last, count);
	 *
	 * 	Takes an array of descriptors which contain the information about
	 * 	    the array's blocks	and rotates the blocks on [first::last] by
	 * 	    block_rotate_count.
	 *		This requires rotating the underlying array to which the blocks refer
	 *
	 * 	Consider a tag array with the associated start:end values
	 * 		   0	  1		 2        3        4		5
	 * 		a[0:2] a[3:6] a[7:10] b[11:14] b[15:18] b[19:20]
	 *      key='d'key='e'key='f' key='b'  key='c'  key='g'
	 *
	 * 	rotateTags(array, tags, 0, 4, 2) results in
	 *
	 * 		   0	  1		  2        3        4		5
	 * 		b[0:3] b[4:7]  a[8:10] a[11:14] a[15:18] b[19:20]
	 * 		k='b'  key='c' key='d' key='e'  key='f'  key='g'
	 *
	 *	Note that blocks are not necessarily all of the same size
	 *
	 *		rotate the underlying array by
	 *			descriptor[first].start - descriptor[first+tag_rotate_count].start
	 *		over the array span
	 *			[descriptor[first].start, descriptor[last].end]
	 *
	 *		rotate the tags [first:last] by tag_rotate_count
	 *
	 *		update the .start & .end indices of the tags on [first:last]
	 */

	template <typename T>
	void swapBlocksLeftSmallerThanRight(T* array,
										Descriptors<T> &descriptors,
										array_size_t u,
										array_size_t v,
										SortMetrics *metrics) {

		/*
		 * 	left_size = lz = 1					right_size = rz = 4
		 * 	left_start 	= 0						right_start = 22
		 * 	left_end 	= 0						right_end 	= 25
		 * 	swap_count 	= sc = left_size = 1
		 *  rotate_right_count = right_size - swap_count = 3
		 *
		 * |A||A         |    ..   |        B||         B|
		 *	0  1  2  3  4  .....   18 19 20 21 22 23 24 25  lp rp sc rc
		 *	a  b  c  d  e           s  t  u  v  w  x  y  x	 0 22  1
		 *	w  b  c  d  e           s  t  u  v  a  x  y  z   1 23  0
		 *		rotate_right(lp = 1, r_end = 25, 3)
		 *	w  x  y  z  b  c  d  e           s  t  u  v  a
		 *	swap descriptors, which will result in all descriptors being misaligned
		 *	w  x  y  z  b  c  d  e           s  t  u  v  a
		 * |B||A         | ....... |        B||A         |
		 *  4  4                            4  1			descriptor's sizes (correct)
		 * 22  1                    18         0            wrong descriptor starts
		 * 25           4                  21  0            wrong descriptor ends
		 * 25  1                           21  0            wrong descriptor key indices
		 *  update descriptor start, stop & key's
		 *	w  x  y  z  b  c  d  e   ...     s  t  u  v  a
		 * [         B||A         |  ...    |         B||A|
		 *  0        3  4        7           21      24  25
		 */

		array_size_t swap_count		= descriptors[u].getWidth();
		array_size_t rotate_count	= descriptors[v].getWidth() - swap_count;
		array_size_t span_start		= descriptors[u].start_index;
		array_size_t left_index		= span_start;
		array_size_t right_index	= descriptors[v].start_index;

		//	swap as many elements as possible to reduce number of moves
		while (swap_count) {
			//	swap left & right
			SortingUtilities::swap(array, left_index, right_index, metrics);
			//	next left & right
			left_index++;
			right_index++;
			swap_count--;
		}

		//	Rotate the remaining portion of the array to get the remaining elements
		//	  of descriptor[v] on the right hand side array_size_to the left hand side
//		std::cout << "u < v Before rotate " << SortingUtilities::arrayElementsToString(array, 33) << std::endl;
		SortingUtilities::rotateArrayElementsRight(	array,
													left_index,
													descriptors[v].end_index,
													rotate_count,
													metrics);
//		std::cout << "u < v After  rotate " << SortingUtilities::arrayElementsToString(array, 33) << std::endl;
		//	  Swap the two block_descriptors
		if (metrics) metrics->assignments += num_assignments_per_swap;
		BlockDescriptor<T> temp = descriptors[u];
		descriptors[u] 			= descriptors[v];
		descriptors[v] 			= temp;
		//	the key gets moved, which is a copy of an array element

		//	update the descriptor's start index, end index & key
		for (array_size_t i = u; i <= v ; i++) {
			array_size_t size = descriptors[i].getWidth();
			descriptors[i].start_index 	= span_start;
			descriptors[i].end_index 	= span_start + size - 1;
			//	changing the key value requires moving / copying an array element
			if (metrics) metrics->assignments++;
			descriptors[i].assignKey(array);
			span_start += size;
		}
		return;
	}


	/*
	 * 	metrics = swapBlocksOfEqualSize(array, descriptors, 1, 2);
	 *
	 *    This swaps the descriptors and the underlying array elements of
	 *  two blocks at positions i vs j in the array of descriptors
	 *
	 *    The function manages the overhead of determining which swapElement()
	 *  function should be called. Currently, swapping the elements of blocks
	 *  that are the same size is accomplished through an element-wise swap.
	 *  Blocks that are of different sizes must be swapped with a rotate, which
	 *  adds considerable more time complexity.
	 */
	template <typename T>
	void swapBlocksOfEqualSize(	T* array,
								Descriptors<T> &descriptors,
								array_size_t u, array_size_t v,
								SortMetrics *metrics) {

//		std::cout << __FUNCTION__ << std::endl;
		BlockOperations::swapBlockElementsOfEqualSize(
							 	 	 array,
									 descriptors[u].start_index,
									 descriptors[v].start_index,
									 descriptors[u].getWidth(),
									 metrics);
		if (metrics) metrics->assignments += num_assignments_per_swap;
		BlockType temp 		= descriptors[u].type;
		descriptors[u].type = descriptors[v].type;
		descriptors[v].type = temp;
		if (metrics) metrics->assignments += 2;
		descriptors[u].assignKey(array);
		descriptors[v].assignKey(array);
		return;
	}

	template <typename T>
	void swapBlocksRightSmallerThanLeft(T* array,
										Descriptors<T> &descriptors,
										array_size_t u,
										array_size_t v,
										SortMetrics *metrics) {

//		std::cout << __FUNCTION__ << std::endl;

		/*
		 * 	left_size = lz = 4					right_size = rz = 1
		 * 	left_start = 0						right_start = 25
		 * 	left_end   = 3						right_end   = 25
		 * 	swap_count = sc = right_size = 1
		 * 	rotate_left_count 	= left_size - swap_count = 3
		 * 	rotate_right_count 	= -rotate_left_count = -3
		 *
		 * |A         ||A    ..  |            |          B||B|
		 *	0  1  2  3  4  5  6  7       ..   18 19 20 21 22 23 24 25  lp rp sc
		 *	a  b  c  d  e  f  g  h             s  t  u  v  w  x  y  z	0 25  1
		 *	z  b  c  d  e  f  g  h             s  t  u  v  w  x  y  a   1 26  0
		 *
		 *	rotate_right(lp = 1, right_start-1 = 24, rc = -3)	which is a rotate left
		 *	w  e  f  g  h                      v  w  x  y  a  b  c  d
		 *	swap descriptors, which will result in all descriptors being misaligned
		 *	0  1  2  3  4  5  6  7            18 19 20 21 22 23 24 25
		 *	w  e  f  g  h                      v  w  x  y  a  b  c  d
		 * |B|         |A        | ..                  |         B||A         |
		 *  1           4                                        4  4	descriptor's sizes correct
		 * 25  4                                        21          0   wrong descriptor's starts
		 * 25                   7                               24  3   wrong descriptor's ends
		 * 25  4                                                24  0   wrong descriptor's key indices
		 *  update descriptor start, stop & key's
		 *	0  1  2  3  4  5  6  7            18 19 20 21 22 23 24 25
		 *	w  e  f  g  h                      v  w  x  y  a  b  c  d
		 * [B||A         |  ...               |         B||A         |
		 *  0  1        4                      18      21 22       25
		 */

		array_size_t span_start 		= descriptors[u].start_index;
		array_size_t left_index 		= span_start;
		array_size_t right_index 		= descriptors[v].start_index;
		array_size_t swap_count 		= descriptors[v].getWidth();
		array_size_t rotate_right_count = -(descriptors[u].getWidth() - swap_count);

		while (swap_count) {
			SortingUtilities::swap(array, left_index, right_index, metrics);
			left_index++;
			right_index++;
			swap_count--;
		}

//		std::cout << "u > v Before rotate " << SortingUtilities::arrayElementsToString(array, 33) << std::endl;
		SortingUtilities::rotateArrayElementsRight(	array,
													left_index,
													descriptors[v].end_index,
													rotate_right_count,
													metrics);
//		std::cout << "u > v After  rotate " << SortingUtilities::arrayElementsToString(array, 33) << std::endl;

		BlockDescriptor<T> temp = descriptors[u];
		descriptors[u] 	= descriptors[v];
		descriptors[v] 	= temp;
		if (metrics) metrics->assignments += num_assignments_per_swap;

		for (array_size_t i = u; i <= v; i++) {
			array_size_t size = descriptors[i].getWidth();
			descriptors[i].start_index = span_start;
			descriptors[i].end_index = span_start+size-1;
			//	moving a key value array_size_to a descriptor is one move of an element
			if (metrics) metrics->assignments++;
			descriptors[i].assignKey(array);
			span_start += size;
		}
		return;
	}

	/*
	 * 	This swaps block descriptors, but DOES NOT move the underlying array.
	 * 	This will (most likely) result in the underlying keys being incorrect.
	 */
	template<typename T>
	void swap_ONLY_BlockDescriptors(Descriptors<T> &descriptors,
									array_size_t i,
									array_size_t j,
									SortMetrics *metrics) {
		//	The block descriptor has a copy of the array value at [key]
		if (metrics) metrics += num_assignments_per_swap;
		BlockDescriptor<T> temp = descriptors[i];
		descriptors[i]			= descriptors[j];
		descriptors[j]			= temp;
		return;
	}
}


#endif /* BLOCKSORTBLOCKSWAPPING_H_ */
