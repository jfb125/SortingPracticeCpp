/*
 * HeapSort.h
 *
 *  Created on: Jul 1, 2025
 *      Author: joe
 */

#ifndef HEAPSORT_H_
#define HEAPSORT_H_

#include <iostream>
#include <iomanip>

#include "SortingDataTypes.h"
#include "SortingUtilities.h"

// used by 'printHeap()'
#define MAX_LINE_SIZE	120
#define TAB_STOP " "

namespace HeapSort {

	template <typename T>
	SortMetrics sinkNode(array_size_t this_node, T*array, array_size_t size);

	template <typename T>
	bool isMaxHeap(T*array, array_size_t size);

	template <typename T>
	SortMetrics heapify(T*array, array_size_t size);

	/* ****************************************	*/
	/*			indices management				*/
	/* ****************************************	*/

#pragma push_macro("farthestNode")
#pragma push_macro("parent")
#pragma push_macro("leftChild")
#pragma push_macro("rightChild")

	#define farthestNode(i) ((i)/2 - 1)
	#define parent(i) 		(((i)-1)/2)
	#define leftChild(i) 	((i)*2 + 1)
	#define rightChild(i) 	((i)*2 + 2)

	template <typename T>
	bool isMaxHeap(T*array, array_size_t size) {

		if (size <= 1)
			return true;

		// add one b/c there is a pre-decrement in the loop
		array_size_t node = farthestNode(size)+1;
		array_size_t final_leaf = size-1;

		do {
			node = node-1;	// examine the next node
			array_size_t left_child = leftChild(node);
			array_size_t right_child = rightChild(node);
			array_size_t largest_child;
			// it should not happen that the left child is outside the heap
			if (left_child <= final_leaf) {
				// if there are two children, find the largest
				if (right_child <= final_leaf) {
					if (array[left_child] > array[right_child]) {
						largest_child = left_child;
					} else {
						largest_child = right_child;
					}
				} else {
					// there is only the left child
					largest_child = left_child;
				}
				if (array[node] < array[largest_child])
					return false;
			}
		} while (node != 0);

		return true;
	}

	/* ****************************************	*/
	/*				array management			*/
	/* ****************************************	*/

	template <typename T>
	SortMetrics heapify(T*array, array_size_t size) {

		SortMetrics retval(0,0);

		// node will be pre-decremented before each sink
		array_size_t node = farthestNode(size)+1;

		do {
			node--;
			retval += HeapSort::sinkNode(node, array, size);
		} while (node != 0);

		return retval;
	}

	template <typename T>
	SortMetrics sinkNode(array_size_t this_node, T*array, array_size_t size) {

		SortMetrics retval(0,0);

		if (size <= 1)
			return retval;

		array_size_t final_leaf	= size-1;
		array_size_t farthest_node = farthestNode(size);

		while (this_node <= farthest_node) {
			// find the largest of the two child nodes
			array_size_t left_child = leftChild(this_node);
			array_size_t right_child = rightChild(this_node);
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
				retval.compares++;
				if (array[left_child] > array[right_child]) {
					largest_child = left_child;
				} else {
					largest_child = right_child;
				}
			}

			// compare the larger of the two children to this_node
			retval.compares++;
			if (array[this_node] < array[largest_child]) {
				// swap the nodes
				SortingUtilities::swap(array, this_node, largest_child, &retval);
				// move down to the lower node
				//  that had previously housed the largest child
				this_node = largest_child;
			} else {
				// both children are smaller or equal to this node
				//   thus this node is in the correct place
				break;
			}
		}
		return retval;
	}

	template <typename T>
	SortMetrics sort(T* array, array_size_t size) {

		SortMetrics retval(0,0);
		if (size <= 1)	return retval;

		if (SortingUtilities::isSorted(array, size, &retval))
			return retval;

		retval += heapify(array, size);

		// starting at the end of the array
		for(array_size_t last_node = size-1; last_node != 0; last_node--) {
			// swap the largest element [0] with the element in the last node
			SortingUtilities::swap(array, last_node, 0, &retval);
			// sink the element that was in the last node downward to preserve heap
			retval += sinkNode(0, array, last_node);
		}
		return retval;
	}

	template <typename T>
	void printHeap(T*array, array_size_t size) {

		OStreamState ostream_state;	// restores ostream state in destructor
		int width = MAX_LINE_SIZE / 2;
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
				std::cout << std::setw(width) << TAB_STOP;
			}
		}
		if (line_node_count != 0) {
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
#pragma pop_macro("farthestNode")
#pragma pop_macro("parent")
#pragma pop_macro("leftChild")
#pragma pop_macro("rightChild")
}

#endif
