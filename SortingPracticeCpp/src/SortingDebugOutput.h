/*
 * SortingDebugOutput.h
 *
 *  Created on: Oct 10, 2025
 *      Author: joe
 */

#ifndef SORTINGDEBUGOUTPUT_H_
#define SORTINGDEBUGOUTPUT_H_

#include <iostream>
#include <iomanip>
#include <string>

#include "SortingPracticeDataTypes.h"
#include "OStreamState.h"

constexpr int 	ELEMENT_WIDTH 		= 5;
constexpr int 	VALUE_WIDTH 		= 4;
extern const uint16_t ccitt_hash[];

std::string arrayIndicesToString(array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string arrayIndicesToString(std::string trailer,
								 array_size_t size, array_size_t v,
								 int element_width = ELEMENT_WIDTH);
std::string arrayIndicesToStringLine(array_size_t size,
								 int element_width = ELEMENT_WIDTH);
std::string arrayStartMiddleEndToString(array_size_t size,
										array_size_t start, array_size_t mid, array_size_t end,
										int element_width = ELEMENT_WIDTH);
namespace SortingUtilities {

	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*									Forward Declarations						*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	template <typename T>
	uint16_t crc16_ccitt(T ** buffer, size_t size);

	template <typename T>
	void printArray(T**array, array_size_t start, array_size_t end, std::string header);

	template <typename T>
	void printArrayAndPivot(T**array, array_size_t start, array_size_t end, array_size_t pivot, std::string header);


	/*	**************************************************************************	*/
	/*	**************************************************************************	*/
	/*									Implementations								*/
	/*	**************************************************************************	*/
	/*	**************************************************************************	*/

	template <typename T>
	std::string arrayElementsToString(T** array, array_size_t size,
									  int value_width = VALUE_WIDTH,
									  int element_width = ELEMENT_WIDTH) {
		OStreamState current_state;
		std::stringstream result;
		int spacer_width = element_width - value_width;
		for (int i = 0; i < size-1; i++) {
			result << std::right << std::setw(value_width) << *array[i];
			if (spacer_width) {
				result << std::setw(spacer_width) << ' ';
			}
		}
		result << std::right << std::setw(value_width) << *array[size-1];
		return result.str();
	}

	template <typename T>
	std::string arrayElementsToString(std::string trailer,
									  T** array, array_size_t size,
									  int value_width, int element_width) {
		OStreamState current_state;
		std::stringstream result;
		result << arrayElementsToString(array, size, value_width, element_width);
		result << trailer;
		return result.str();
	}

	template <typename T>
	uint16_t SortingUtilities::crc16_ccitt(T ** buffer, size_t size)
	{
	    uint16_t crc = 0;
	    for (size_t i = 0; i != size; i++) {
	    	uint16_t _data = static_cast<uint16_t>(*buffer[i]);
	    	crc = (crc << 8) ^ ccitt_hash[((crc >> 8) ^ _data) & 0x00FF];
	    }
	    return crc;
	}

	// returns bool
	template <typename T>
	bool isSorted(T **array, array_size_t size) {
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				return false;
			}
		}
		return true;
	}

	// 	returns bool
	//	 updates the number of compares in 'metrics'
	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics) {
		for (array_size_t i = size-1; i > 0 ; --i) {
			metrics.compares++;
			if (*array[i] < *array[i-1]) {
				return false;
			}
		}
		return true;
	}

	// 	returns bool
	//	stores location of the unordered elements in i, j
	//		always updates value of i & j
	template <typename T>
	bool isSorted(T **array, array_size_t size,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered) {
		lower_index_unordered = 0;
		upper_index_unordered = 0;
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				lower_index_unordered = i-1;
				upper_index_unordered = i;
				return false;
			}
		}
		return true;
	}

	// returns bool
	// stores number of compares in 'metrics'
	// stores location of mismatch in lower_index_unordered & upper_index_unordered
	// 	lower_index_unordered & upper_index_unordered are always updated
	template <typename T>
	bool isSorted(T **array, array_size_t size, SortMetrics &metrics,
				  array_size_t &lower_index_unordered, array_size_t &upper_index_unordered) {
		lower_index_unordered = 0;
		upper_index_unordered = 0;
		for (array_size_t i = size-1; i > 0 ; --i) {
			if (*array[i] < *array[i-1]) {
				lower_index_unordered = i-1;
				upper_index_unordered = i;
				return false;
			}
		}
		return true;
	}

	template <typename T>
	void printArray(T**array, array_size_t start, array_size_t end, std::string header) {
			std::cout << header << " [" << start << ":" << end << "]"
					  << std::endl;
			for (array_size_t i = start; i <= end; i++) {
				std::cout << std::setw(2) << std::right << " " << i << "  : "
						  << array[i]->last_name << std::endl;
			}
	}
#if 0
	template <typename T>
	void printElements(std::string trailer, T** array, array_size_t size, int value_width, int element_width) {
		OStreamState current_state;

		int spacer_width = element_width - value_width;
		for (int i = 0; i != size; i++) {
			if (spacer_width) {
				std::cout << std::setw(spacer_width) << ' ';
			std::cout << std::setw(value_width) << *array[i];
			}
		}
		std::cout << trailer;
	}
#endif
	template <typename T>
	void printArrayAndPivot(T**array, array_size_t start, array_size_t end, array_size_t pivot, std::string header) {
			std::cout << header <<  " [" << start << ":" << end << "]"
					  << std::endl;
			for (array_size_t i = start; i <= end; i++) {
				if (i == pivot) {
					std::cout << std::setw(2) << ">" << i << "< : ";
				} else {
					std::cout << std::setw(2) << " " << i << "  : ";
				}
				std::cout  << array[i]->last_name << std::endl;
			}
	}
}

#endif /* SORTINGDEBUGOUTPUT_H_ */
