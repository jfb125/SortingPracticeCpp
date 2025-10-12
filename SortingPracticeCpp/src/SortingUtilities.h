#ifndef SORTINGUTITLITES_H
#define SORTINGUTITLITES_H 1

#include <iostream>
#include <iomanip>
#include "SortingPracticeDataTypes.h"
#include "IntegerArithmetic.h"
#include "SimpleRandomizer.h"
#include "OStreamState.h"
#include "SortingDebugOutput.h"

#pragma push_macro("_verbose")
//#undef _verbose
//#define _verbose true

array_size_t blockSortModulo(array_size_t rotation_count, array_size_t span);

namespace SortingUtilities {

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							Function Declarations						*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	/*	Debugging	*/

	template <typename T>
	bool isSorted(T **array, array_size_t size);

	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics);

	template <typename T>
	bool isSorted(T **array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered);

	template <typename T>
	SortMetrics swap(T**array, array_size_t i, array_size_t j);

	/*	******************************************************************	*/
	/*					Functions used by some algorithms					*/
	/*	******************************************************************	*/

	/*	Returns the index of the first element that is greater than 'value'
	 * 	This is used to insert a value to the right of it's peers	*/
	template <typename T>
	array_size_t binarySearchLastElement(T** array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T *value,
										 SortMetrics &metrics);

	/*	Returns the index of the first element that is equal to or greater than 'value'
	 * 	This is used to insert a value to the left of it's peers	*/
	template <typename T>
	array_size_t binarySearchFirstElement(T** array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T *value,
										  SortMetrics & metrics);

	/* 	Blocks do not have to be continuous nor do they have to be the same size */
	template <typename T>
	array_size_t mergeTwoBlocksElementsByTable(T ** array,
											  array_size_t block_1_start,
											  array_size_t block_1_end,
											  array_size_t block_2_start,
											  array_size_t block_2_end,
											  SortMetrics &metrics);

	/*	Uses a binary search to identify spans of block_1 that can be
	 * rotated into the midst of the block_2 */
	template <typename T>
	array_size_t mergeTwoAdjacentBlocksByRotation( T** array,
												  array_size_t block_1_start,
												  array_size_t block_1_end,
												  array_size_t block_2_start,
												  array_size_t block_2_end,
												  SortMetrics &metrics);

	/*	rotates elements of an array [start:end] an amount, where negative
	 * values of 'amount' indicate to rotate the span to the left */
	template <typename T>
	SortMetrics rotateArrayElementsRight(T** array,
										 array_size_t start, array_size_t end,
										 array_size_t amount);

	/*	Some algorithms like QuickSort use this to ensure the array is not
	 *	 in reverse order, which can lead to n^2 performance */
	template <typename T>
	SortMetrics randomizeArray(T** array, array_size_t size);

	/*	This is used in all of the QuickSort based algorithms	*/
	template <typename T>
	SortMetrics selectAndPositionPivot( T** array,
										array_size_t start,
										array_size_t end);

	template <typename T>
	SortMetrics swap(T**array, array_size_t i, array_size_t j);


	/*	**********************************************************************	*/
	/*	**********************************************************************	*/
	/*							Function Definitions							*/
	/*	**********************************************************************	*/
	/*	**********************************************************************	*/

	/*
	 * binaryFirst(array, first, last, value);
	 *
	 * This function returns the index of the first element that is
	 *  greater than or equal to value. If the passed value is greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &0) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &1) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &2) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryFirst(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	array_size_t binarySearchFirstElement(T** array,
										  array_size_t range_start,
										  array_size_t range_end,
										  T *value,
										  SortMetrics &metrics) {

		array_size_t start = range_start;
		array_size_t end	 	= range_end;
		array_size_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
			if (*array[mid] < *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			metrics.compares++;
			if (*array[start] < *value) {
				start++;
			}
		}
		return start;
	}


	/*
	 * binaryLast(array, first, last, value);
	 *
	 * This function returns the index of first element that is greater than value
	 *	If the passed value is equal to or greater than
	 *	all of the elements on the span, an index (range_end+1) is returned.
	 *
	 *	  0  1  2  3  4  5
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &-1) returns 0
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &0) returns 2
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &1) returns 4
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &2) returns 6
	 *	{ 0, 0, 1, 1, 2, 2 }	binaryLast(array, 0, 5, &3) returns 6
	 */

	template <typename T>
	array_size_t binarySearchLastElement(T** array,
										 array_size_t range_start,
										 array_size_t range_end,
										 T *value,
										 SortMetrics &metrics) {

		array_size_t start = range_start;
		array_size_t end	 	= range_end;
		array_size_t mid		= start + (end-start)/2;

		while (start != end) {
			//	determine the midpoint in an even size span
			//	or the index on the left of mid in an odd size span
			mid = start + (end-start)/2;
			metrics.compares++;
			if (*array[mid] <= *value) {
				//	if the array value at [mid] is < value
				//	  look to the right for a [] >= value
				start = mid+1;
			} else {
				//	else the look to the right for a value < passed
				end = mid;
			}
		}
		if (start == range_end) {
			metrics.compares++;
			if (*array[start] <= *value) {
				start++;
			}
		}
		return start;
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
	array_size_t mergeTwoBlocksElementsByTable(T ** array,
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
		array_size_t block_2_end_position = block_2_end;
		array_size_t block_1_span = block_1_end - block_1_start + 1;
		array_size_t block_2_span = block_2_end - block_2_start + 1;

		if (block_1_span == 0 || block_2_span == 0) {
			return block_2_end_position;
		}

		array_size_t block_1_locations_table_size = block_1_span;
		array_size_t block_1_locations_table[block_1_locations_table_size];

		for (array_size_t i = 0, src = block_1_start; i < block_1_locations_table_size; ) {
			block_1_locations_table[i++] = src++;
		}

		array_size_t block_1_locations_table_index = 0;
		array_size_t block_2_index				  = block_2_start;
		array_size_t destination_index  			  = block_1_start;

		auto debug_string = [&]() -> std::string {
			std::stringstream result;
			for (array_size_t i = block_1_start; i <= block_2_end; ) {
				result << std::setw(3) << *array[i] << " ";
				i++;
				if (i-1 == block_1_end) {
					i = block_2_start;
				}
			}
			result << " " << std::setw(3) << block_1_locations_table_index
				   << " using table [";
			for (int i = 0; i != block_1_locations_table_size; i++) {
				if (i < block_1_locations_table_index) {
					result << " - ";
				} else {
					result << std::setw(3) << block_1_locations_table[i];
				}
			}
			result 	<< "] dst " << std::setw(2) << destination_index
					<< " t_ptr " << std::setw(2) << block_1_locations_table_index
					<< " b2_ptr " << std::setw(2) << block_2_index;
			return result.str();
		};


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
			if (*array[block_1_index] <= *array[block_2_index]) {
				// the value from the left block goes into destination
				if (destination_index != block_1_index) {
					T* tmp 					 = array[destination_index];
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
				T* tmp 					 = array[destination_index];
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
			T* temp 				= array[destination_index];
			array[destination_index]= array[block_1_index];
			array[block_1_index]	= temp;
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
	}


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
	 *
	 *
	 *	Although it is not necessary to pass 'block_1_end' as a parameter, it
	 *	is included so this function is compatible with other merge functions
	 *	that have the capability to merge non-contiguous blocks and thus need
	 *	to know explicitly where block_1 ends.
	 */

	template <typename T>
	array_size_t mergeTwoAdjacentBlocksByRotation(T** array,
												  array_size_t block_1_start,
												  array_size_t block_1_end,
												  array_size_t block_2_start,
												  array_size_t block_2_end,
												  SortMetrics &metrics) {
		constexpr bool debug_verbose= false;
		bool announce_initial_array = true;
		bool a_rotation_occurred	= false;
		std::stringstream initial_array_msg;

		array_size_t a_start	= block_1_start;
		array_size_t a_end		= block_1_end;
		array_size_t b_start	= block_2_start;
		array_size_t b_end		= block_2_end;
		array_size_t span_start = 0;
		array_size_t span_end 	= 1;
		array_size_t rotate_count;
		array_size_t final_b_pos= block_2_end;

		//	if b_start moves off the end of the array, all b_blocks are in place
		//		and therefore the remaining a_blocks will not go after b_start
		//		  As       Ae                 Bs       Be
		//		[ A  B, G, H ]  merged with [ C, D, E, F ] 	ss = [2], se = [7]
		//	   	[ A, B, C, D, E. F. G. H ]					bs = se + 1 =  [8]

		//	if span_start == span_end, the remaining span of a_blocks are in place
		//		even though b_start may not be at the end
		//		[ A, D, E, F ]  merged with [ B, C, G, H ]	ss = [1], se = [6]
		//		[ A, B, C, D, E, F, G, H ]					ss = [8], se = [8]
		if (debug_verbose) {
			array_size_t dbg_size = b_end-a_start+1;
			initial_array_msg << SortingUtilities::arrayElementsToString(array, dbg_size)
							  << " initial array state"
							  << std::endl;
		}

		//	there are three ways that signal done
		//	 a block is exhausted	a_start == a_end
		//	 b_block is exhausted	b_start == block_2_end
		//	 all elements in place	span_start == span_end
		while (b_start <= block_2_end && span_start != span_end && a_start != a_end) {
			std::stringstream dbg_msg;
			array_size_t first_a_greater_than_b;
			array_size_t first_b_greater_than_a;
			first_a_greater_than_b =
				SortingUtilities::binarySearchLastElement(array,
														  a_start, a_end,
														  array[b_start],
														  metrics);
			//	if the b element in question is greater than all a elements, leave
			if (first_a_greater_than_b == b_start) {
				break;
			}
			span_start = first_a_greater_than_b;
			first_b_greater_than_a =
				SortingUtilities::binarySearchLastElement(array,
														  b_start, b_end,
														  array[span_start],
														  metrics);
			//	the span of A values that can be rotated right ends at the
			//	element immediately before the first B value that is
			//	greater than the a_value at the start of the span
			//	0 1 2 3 4   5 6 7 8 9
			//	A B G H I | D E F J K	a[G] >_b_start[D] = 2  b[J] > a[G] = 8
			//							span [2:7] [G:F], rotaate
			span_end 	= first_b_greater_than_a - 1;
			if (span_end == span_start)
				break;
			rotate_count= first_b_greater_than_a - b_start;
			if (span_end == final_b_pos) {
				final_b_pos = final_b_pos 				// current position
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
							<< " a_st  " 		<< std::setw(2) << a_start
							<< " a_end "		<< std::setw(2)	<< a_end
							<< " b_st  "		<< std::setw(2) << b_start
							<< " b_end "		<< std::setw(2)	<< b_end
							<< " sp_st " 	<< std::setw(2)	<< span_start
							<< " sp_en "		<< std::setw(2)	<< span_end
							<< " rrcnt "	<< std::setw(2)	<< rotate_count
							<< " final_b_pos "
						    << std::setw(2) << final_b_pos << std::endl;
				std::cout   << dbg_msg.str();
			}
			a_start = span_start + rotate_count;
			a_end	+= rotate_count;
			b_start = span_end+1;
		}
		if (debug_verbose && a_rotation_occurred) {
			array_size_t dbg_size = block_2_end - block_1_start + 1;
			std::cout << SortingUtilities::arrayElementsToString(array, dbg_size)
					  << " initial array state"
					  << std::endl << std::endl;
		}
		return final_b_pos;
	}


	/*
	 * 	ComparesAndSwaps rotateArray(array, amount, start, end);
	 *
	 * 	rotate the contents of the array by 'amount' positions
	 * 	  where the array is defined as array[start]:array[end] (inclusive)
	 *
	 * 	algorithm:
	 * 		consider amount to rotate = 3
	 * 		array									{ A, B, C, D, E, F, G }
	 * 		reverse the array						{ G, F, E, D, C, B, A }
	 * 		reverse the sub array [start:amount-1]  { E, F, G  D, C, B, A }
	 * 		reverse the sub array [amount:end]		{ E, F, G, A, B, C, D }
	 *
	 * 	note that amount can be negative or greater than the span
	 * 		negative amounts are converted into the equivalent positive amount
	 * 		amounts greater than the span are modulo-division to be within the span
	 */

	template <typename T>
	SortMetrics rotateArrayElementsRight(T** array,
											  array_size_t start, array_size_t end,
											  array_size_t amount) {
		SortMetrics result(0,0);

		if (end <= start)
			return result;

		array_size_t span = end - start + 1;
		if (span == 0)
			return result;

		//	converts amounts that are not in [0,span) to in range
		amount = blockSortModulo(amount, span);

		if (amount == 0)
			return result;

		//	reverse the entire array
		T* temp;
		for (array_size_t i = start, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}

		//	reverse the left hand side
		for (array_size_t i = start, j = start+amount-1; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}

		//	reverse the right hand side
		for (array_size_t i = start + amount, j = end; i < j; i++, j--) {
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			result.assignments += 3;
		}
		return result;
	}
	template <typename T>
	SortMetrics randomizeArray(T** array, array_size_t size) {

		SortMetrics ret_val(0, 0);

		if (size <= 1)
			return ret_val;

		uint64_t randomizer_seed = getChronoSeed();
		randomizer_seed = SIMPLE_RANDOMIZER_DEFAULT_SEED;
		SimpleRandomizer randomizer(randomizer_seed);

		array_size_t r;
		array_size_t end = size-1;

		for (array_size_t i = 0; i != end; i++) {
			r = randomizer.rand(i, size);
			ret_val += SortingUtilities::swap(array, i, r);
		}

		return ret_val;
	}

	template <typename T>
	SortMetrics selectAndPositionPivot(T** array, array_size_t start, array_size_t end) {

		constexpr bool _verbose = false;
		SortMetrics returned_value(0,0);
		array_size_t half = (end-start)/2;
		array_size_t mid = start + half;
		half /= 2;
		array_size_t low = start + half;
		array_size_t high = mid + half;
		array_size_t pivot;

		if (low < start) {
			std::cout << "Error: low  " << low << " is less than start " << start << std::endl;
		}
		if (high > end) {
			std::cout << "Error: high " << low << " is more than end   " << end << std::endl;
		}

		if (_verbose) {
		std::cout << std::endl;
		std::cout << "selecting pivot based upon considering "
				  << low << ", " << mid << ", " << high << std::endl;
			SortingUtilities::printArrayAndPivot(array, start, end, 0, "prior to selecting and positioning pivot: ");
		}
		returned_value.compares++;
		if (*array[low] < *array[high])
		{
			// start is smaller than end
			returned_value.compares++;
			if (*array[low] < *array[mid]) {
				// start is smaller than end and smaller than or equal to mid
				// start is the smallest, evaluate mid vs end
				returned_value.compares++;
				if (*array[mid] < *array[high]) {
					// mid is greater than or equal to start
					//	but less than end
					pivot = mid;	// { 1, 2, 3 }
				} else {
					// mid is greater than or equal to start
					//  and greater than or equal to end
					pivot = high;	// { 1, 3, 2 }
				}
			} else {
				// start is smaller than end
				//  but equal to or greater than mid
				pivot = low;	// { 2, 1, 3 }
			}
		} else {
			returned_value.compares++;
			// end is smaller than or equal to start
			if (*array[high] < *array[mid]) {
				// end is smaller than or equal to start and smaller than or equal to mid
				// end is the smallest, evaluate start vs mid
				returned_value.compares++;
				if (*array[low] < *array[mid]) {
					// smart is greater than or equal to end but smaller than mid
					pivot = low;	// { 2, 3, 1 }
				} else {
					// smart is greater than or equal to end and greater than or equal to mid
					pivot = mid;		// { 3, 2, 1 }
				}
			} else {
				// end is smaller than or equal to start but greater than mid
				pivot = high;			// { 3, 1, 2 }
			}
		}
		if (_verbose) {
			std::cout << "selected pivot as element " << pivot << std::endl;
			printArrayAndPivot(array, start, end, pivot, "");
		}
		returned_value += SortingUtilities::swap(array, start, pivot);

		if (_verbose) {
			std::cout << "array is now " << std::endl;
			printArrayAndPivot(array, start, end, start, "");
			std::cout << std::endl;
		}
//		static int stop_count = 5;
//		static int stop_counter = stop_count;
//		if (false && --stop_counter == 0) {
//			std::cout << "selectAndPositionPivot() stops after interation #" << stop_count << std::endl;
//			while(1){}
//		}
		return returned_value;
	}

	template <typename T>
	SortMetrics swap(T**array, array_size_t i, array_size_t j) {
		T* temp  = array[i];
		array[i] = array[j];
		array[j] = temp;
		SortMetrics result(0,0);
		result.assignments = 3;
		return result;
	}


}	// namespace SortingUtilities

#pragma pop_macro("_verbose")

#endif	/*	header guard	*/
