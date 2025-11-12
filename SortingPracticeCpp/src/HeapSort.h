/*
 * HeapSort.h
 *
 *  Created on: Jul 1, 2025
 *      Author: Joe Baker
 */

#ifndef HEAPSORT_H_
#define HEAPSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"


namespace HeapSort {

	// used by 'printHeap()'
	constexpr int max_line_size = 120;
	constexpr char tab_stop = ' ';

	/*  **********************************************************	*/
	/*						indices management macros				*/
	/*  **********************************************************	*/

	// use macros to avoid having to have these as non-templated
	// functions in a .cpp file somewhere
	#pragma push_macro("farthestNode")
	#pragma push_macro("parent")
	#pragma push_macro("leftChild")
	#pragma push_macro("rightChild")

	#define farthestNode(size) ((size)/2 - 1)
	#define parent(i) 			(((i)-1)/2)
	#define leftChild(i) 		((i)*2 + 1)
	#define rightChild(i) 		((i)*2 + 2)


	/*	**********************************************************	*/
	/*						function declarations					*/
	/*	**********************************************************	*/

	// 	Declare the functions with default value of SortMetrics pointer
	// 	Order the array as a max heap, with largest at 0
	template <typename T>
	void heapify(T*array, array_size_t size,
				 SortMetrics *metrics=nullptr);

	//	Debugging - verify heapify & sinkNode produced a max heap
	template <typename T>
	bool isMaxHeap(T*array, array_size_t size,
				   SortMetrics *metrics=nullptr);

	//	Debugging - Print out the array on multiple lines as a tree
	template <typename T>
	void printHeap(T*array, array_size_t size);

	//	Sink an element below all larger elements
	template <typename T>
	void sinkNode(array_size_t this_node, T*array, array_size_t size,
				  SortMetrics *metrics=nullptr);


	/*	**********************************************************	*/
	/*						function definitions					*/
	/*	**********************************************************	*/

	/*
	 * 	The sort algorithm using convention of other sort algorithm
	 */

	template <typename T>
	void sort(T* array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1)
			return;

		if (SortingUtilities::isSorted(array, size, metrics))
			return;

		//	Organize the array as a max heap with largest on top
		heapify(array, size, metrics);

		// starting at the end of the array
		for(array_size_t last_node = size-1; last_node != 0; last_node--) {
			// swap the largest element [0] with the element in the last node
			SortingUtilities::swap(array, last_node, 0, metrics);
			// sink the smaller element that was in the last node
			//	downward to preserve heap ordering
			sinkNode(0, array, last_node, metrics);
		}
	}

	/* ****************************************	*/
	/*				array management			*/
	/* ****************************************	*/

	/*
	 * Rearrange the elements to conform to the max heap property
	 *   where each parent node is >= its children
	 */

	template <typename T>
	void heapify(T*array, array_size_t size, SortMetrics *metrics) {

		// node will be pre-decremented before each sink
		array_size_t node = farthestNode(size)+1;

		do {
			node--;
			HeapSort::sinkNode(node, array, size, metrics);
		} while (node != 0);
	}


	/*
	 * 	Move the element at 'this_node' down to a place where its parent
	 *	  is >= to the element
	 */

	template <typename T>
	void sinkNode(array_size_t this_node, T*array, array_size_t size,
				  SortMetrics *metrics) {

		if (size <= 1)
			return;

		array_size_t final_leaf		= size-1;
		array_size_t farthest_node 	= farthestNode(size);

		while (this_node <= farthest_node) {
			// find the largest of the two child nodes
			array_size_t left_child 	= leftChild(this_node);
			array_size_t right_child 	= rightChild(this_node);
			array_size_t largest_child;
			// if there are no children, done
			if (left_child > final_leaf) {
				break;
			}
			// if there is a left child, but no right child
			if (right_child > final_leaf) {
				largest_child = left_child;
			} else {
				// there is both a left & right child
				if (metrics) metrics->compares++;
				if (array[left_child] > array[right_child]) {
					largest_child = left_child;
				} else {
					largest_child = right_child;
				}
			}

			// compare the larger of the two children to this_node
			if (metrics) metrics->compares++;
			if (array[this_node] < array[largest_child]) {
				// swap the nodes
				SortingUtilities::swap(array, this_node, largest_child, metrics);
				// move down to the lower node
				//  that had previously housed the largest child
				this_node = largest_child;
			} else {
				// both children are smaller or equal to this node
				//   thus this node is in the correct place
				break;
			}
		}
		return;
	}


	/* ****************************************	*/
	/*				debugging resources			*/
	/* ****************************************	*/

	/*
	 * Verify the array satisfies the max heap property where every parent
	 * is >= its children
	 */

	template <typename T>
	bool isMaxHeap(T*array, array_size_t size, SortMetrics *metrics) {

		if (size <= 1)
			return true;

		// add one b/c there is a pre-decrement in the loop
		array_size_t node 		= farthestNode(size)+1;
		array_size_t final_leaf = size-1;

		do {
			node = node-1;	// examine the next node
			array_size_t left_child 	= leftChild(node);
			array_size_t right_child	= rightChild(node);
			array_size_t largest_child;
			// it should not happen that the left child is outside the heap
			if (left_child <= final_leaf) {
				// if there are two children, find the largest
				if (right_child <= final_leaf) {
					if (metrics) metrics->compares++;
					if (array[left_child] > array[right_child]) {
						largest_child = left_child;
					} else {
						largest_child = right_child;
					}
				} else {
					// there is only the left child
					largest_child = left_child;
				}
				if (metrics) metrics->compares++;
				if (array[node] < array[largest_child])
					return false;
			}
		} while (node != 0);

		return true;
	}


	/*
	 * Print the array to the screen in a way the illustrates the
	 * hierarchical structure of the max heap property
	 */

	template <typename T>
	void printHeap(T*array, array_size_t size) {

		OStreamState ostream_state;	// restores ostream state in destructor
		int width = max_line_size / 2;
		int nodes_per_line = 1;
		int line_node_count = 1;

		for (array_size_t i = 0; i != size; i++) {
			std::cout << std::setw(width) << std::right
					  << array[i];
			line_node_count--;
			// determine whether a line feed or tab stop goes next
			if (line_node_count == 0) {
				std::cout << std::endl;
				width /= 2;
				nodes_per_line *= 2;
				line_node_count = nodes_per_line;
			} else {
				std::cout << std::setw(width) << tab_stop;
			}
		}
		if (line_node_count != 0) {
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

#pragma pop_macro("farthestNode")
#pragma pop_macro("parent")
#pragma pop_macro("leftChild")
#pragma pop_macro("rightChild")

#endif
