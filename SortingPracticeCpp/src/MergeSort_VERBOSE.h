/*
 * MergeSort.h
 *
 *	This is a verbose debugging version of a Top Down merrgeSort
 */

#if 0
#ifndef MERGESORT_H_
#define MERGESORT_H_

#include "SortingPracticeDataTypes.h"
#include <iostream>
#include <iomanip>

template <typename T>

class MergeSort final {
private:

	ComparesAndMoves sortObjects(T* array, array_size_t start, array_size_t end) {

		ComparesAndMoves retval(0,0);

		return retval;
	}

	ComparesAndMoves sortPointersToObjects(T**array, array_size_t start, array_size_t end) {

		static int safety_count = 0;
		bool verbose = false;

		ComparesAndMoves retval(0,0);
		ComparesAndMoves partition_result(0,0);


		if (start >= end) {
			if (verbose) {
			std::cout << "early return from sortPointersToObject(" << array
				      << "[" << start << ":" << end << "])"
				      << std::endl;
			std::cout << std::endl;
			}
			return retval;
		}

		if (verbose) {
		std::cout << "Entered sortPointersToObject(" << array
				  << "[" << start << ":" << end << "])"
				  << std::endl;
		}

		array_size_t span = end-start+1;	// determines size of temp array
		array_size_t mid = (span / 2);	// determines starting place of right side
		array_size_t start_of_left_partition = start;
		array_size_t end_of_left_partition = start + mid-1;
		array_size_t start_of_right_partition = start + mid;
		array_size_t end_of_right_partition = end;

		if (verbose) {
			std::cout << "array paritioned into [" << start_of_left_partition
					<< ":" << end_of_left_partition
					<< "] and [" << start_of_right_partition
					<< ":" << end_of_right_partition
					<< "]" << std::endl
					<< std::endl;
		}

		// partition this span of the array and mergeSort each partition
		partition_result = sortPointersToObjects(array, start_of_left_partition, end_of_left_partition);
		retval._compares += partition_result._compares;
		retval._moves	 += partition_result._moves;
		if(verbose) {
			std::cout << "partition_result [" << start_of_left_partition
					  << ":" << end_of_left_partition << "] " << partition_result << " retval " << retval << std::endl;
		}
		partition_result = sortPointersToObjects(array, start_of_right_partition, end_of_right_partition);
		if(verbose) {
			std::cout << "partition_result [" << start_of_right_partition
					  << ":" << end_of_right_partition << "] " << partition_result << " retval " << retval << std::endl;
		}
		retval._compares += partition_result._compares;
		retval._moves	 += partition_result._moves;
		if (verbose) {
			std::cout << "retval " << retval << std::endl;
			std::cout << " MOVING ON " << std::endl;
		}

		if (verbose) {
		std::cout << array << " merging " << span
				  << " elements from " << start
				  << " to " << end << std::endl;
		std::cout << std::endl;
		}

		//	each of the two partitions has all of their elements in order
		//	it is now time to merge the partitions onto this span
		T** temp = new T*[span];
		array_size_t dst = 0;
		array_size_t src_left  = start_of_left_partition;
		array_size_t src_right = start_of_right_partition;
		if (verbose) {
		std::cout << "src_left " << src_left
				  << ", src_right " << src_right
				  << " end " << end_of_right_partition << std::endl;
		}
		safety_count = 0;
		while (dst < span) {
			if (verbose) {
				std::cout << std::endl
						  << "AT TOP OF MERGE BLOCK" << std::endl;
                if (src_left <= end) {
                    std::cout  << "src_left="  << src_left  << ": " << array[src_left]  << " " << array[src_left]->last_name << " ";
                } else {
                    std::cout << "src_left " << src_left << ": is beyond end of array " << end << " ";
                }

                if (src_right <= end) {
                    std::cout << ", src_right=" << src_right << ": " << array[src_right] << " " << array[src_right]->last_name << " ";
                } else {
                    std::cout << "src_right " << src_right << ": is beyond end of array " << end << " ";
                }

                std::cout << ", dst=" << dst
                          << std::endl;
			}
			// if the left side has been exhausted, copy the right side
			if (src_left > end_of_left_partition) {
				while (src_right <= end_of_right_partition) {
					if (verbose) {
						std::cout << "flushing right: dst " << dst << " src_left: " << src_left << " src_right " << src_right << std::endl;
					}
					temp[dst++] = array[src_right++];
				}
			} else
			// if the right side has been exhausted, copy the left side
			if (src_right > end_of_right_partition) {
				while (src_left <= end_of_left_partition) {
					if (verbose) {
						std::cout << "flushing left:  dst " << dst << " src_left: " << src_left << " src_right " << src_right << std::endl;
					}
					temp[dst++] = array[src_left++];
				}
			} else  {
				// is  array[src_left] <= array[src_right] ? )
				//  i.e. = is array[src_right] < array[src_left]
				if (verbose) {
					std::cout << "comparing   dst " << dst << " src_left: " << src_left << " src_right " << src_right << std::endl;
				}
				retval._compares++;
				if (*array[src_right] < *array[src_left]) {
					if (verbose) {
						std::cout << "moving src_right" << std::endl;
					}
					temp[dst++] = array[src_right++];
				} else {
					if (verbose) {
						std::cout << "moving src_left" << std::endl;
					}
					temp[dst++] = array[src_left++];
				}
			}
            if (verbose) {
                std::cout << "AFTER MERGE BLOCK IN WHILE LOOP " << std::endl;
                if (src_left <= end) {
                    std::cout  << "src_left="  << src_left  << ": " << array[src_left]  << " " << array[src_left]->last_name << " ";
                } else {
                    std::cout << "src_left " << src_left << ": is beyond end of array " << end << " ";
                }

                if (src_right <= end) {
                    std::cout << ", src_right=" << src_right << ": " << array[src_right] << " " << array[src_right]->last_name << " ";
                } else {
                    std::cout << "src_right " << src_right << ": is beyond end of array " << end << " ";
                }

                std::cout << ", dst=" << dst
                          << std::endl;

                std::cout << "temp[0:" << dst << "] contains: " << std::endl;
                for (array_size_t q = 0; q != dst; q++) {
                    std::cout 	<< "q="   << q << ": " << temp[q] << " " << temp[q]->last_name
                                << std::endl;
                }
            }
			if (++safety_count == 10) {
				std::cout << "safety counter triggered" << std::endl;
				while(1);
			}
		}

		array_size_t copy_src = 0;
		array_size_t copy_dst = start;

		if (verbose) {
        std::cout << "array contains:" << std::endl;
		for (array_size_t i = start; i <= end; i++) {
			std::cout << i << ": " << array[i] << " " << array[i]->last_name << std::endl;
		}
		std::cout << std::endl;
		std::cout << "temp contains:" << std::endl;
		for (array_size_t i = 0; i < span; i++) {
			std::cout << i << ": " << temp[i] << " " << temp[i]->last_name << std::endl;
		}
		std::cout << std::endl;
		}

		while (copy_dst <= end) {
			array[copy_dst++] = temp[copy_src++];
		}
		delete[] temp;

		if (verbose) {
		std::cout << "array contains:" << std::endl;
		for (array_size_t i = start; i <= end; i++) {
			std::cout << i << ": " << array[i] << " " << array[i]->last_name << std::endl;
		}
		std::cout << std::endl;
		}

		retval._moves += span;
		std::cout << array << "[" << start << ":" << end << "] took: " << retval << std::endl; // << retval << std::endl;
		return retval;
	}

public:
	MergeSort() {}
	~MergeSort() {}

	ComparesAndMoves sortObjects(T* array, array_size_t size) {

		return sortObjects(array, 0, size-1);
	}

	ComparesAndMoves sortPointersToObjects(T** array, array_size_t size) {

		return sortPointersToObjects(array, 0, size-1);
	}
};

#endif /* MERGESORT_H_ */
#endif

