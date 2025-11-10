/*
 * BlockOperationsMergeTable.h
 *
 *  Created on: Nov 3, 2025
 *      Author: joe
 */

#ifndef BLOCKOPERATIONSMERGETABLE_H_
#define BLOCKOPERATIONSMERGETABLE_H_

//	For debugging
#include <istream>
#include <iomanip>
#include <string>

//	For data types such as array_size_t
#include "BlockSortDataTypes.h"
#include "SortingUtilities.h"
#include "BlockOperations.h"

namespace BlockOperations {


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
}



#endif /* BLOCKOPERATIONSMERGETABLE_H_ */
