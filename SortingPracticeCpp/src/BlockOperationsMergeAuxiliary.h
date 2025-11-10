/*
 * BlockSortMergeAuxiliary.h
 *
 *  Created on: Nov 3, 2025
 *      Author: joe
 */

#ifndef BLOCKOPERATIONSMERGEAUXILIARY_H_
#define BLOCKOPERATIONSMERGEAUXILIARY_H_

//	for debugging
#include <iostream>
#include <iomanip>
#include <string>

//	data types such as array_size_t
#include "BlockSortDataTypes.h"
#include "SortingUtilities.h"
#include "BlockOperations.h"

namespace BlockOperations {

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


}


#endif /* BLOCKOPERATIONSMERGEAUXILIARY_H_ */
