/*
 * SortingUtilities.cpp
 *
 *  Created on: Sep 22, 2025
 *      Author: joe
 */

#include "SortingUtilities.h"


std::string arrayIndicesToString(array_size_t size, array_size_t v, int element_width) {

	OStreamState io_state;	// the destructor will restore state
	std::stringstream result;
	if (size != 0) {
		for (int i = 0; i < size-1; i++) {
			if (i != v) {
				result << std::right << std::setw(element_width-1) << i;
			} else {
				result << std::setw(element_width) << "\\V/";
			}
			result << ' ';
		}
		result << std::right << std::setw(element_width-1) << size-1;
	}
	return result.str();
}

std::string arrayIndicesToString(std::string trailer,
								 array_size_t size, array_size_t v,
								 int element_width) {
	std::stringstream result;
	result << arrayIndicesToString(size, v, element_width);
	result << trailer;
	return result.str();
}

std::string arrayStartMiddleEndToString(array_size_t size,
										array_size_t start, array_size_t mid, array_size_t end,
										int element_width) {
	//	restores state of ostream flags in destructor
	OStreamState ostream_state;
	std::stringstream result_msg;

	for (array_size_t i = start; i <= end; i++) {
		if (i != mid) {
			result_msg << std::setw(element_width-1) << i << ' ';
		} else {
			result_msg << "\\" << std::setw(element_width-2) << "V/ ";
		}
	}
	return result_msg.str();
}

std::string printArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width) {
	OStreamState state;	// destructor will restore ostream state
	std::stringstream result;

	for (int i = 0; i < start ; i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 's' << ' ';
	for (int i = start+1; i < mid; i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 'm' << ' ';
	for (int i = mid+1; i < end;  i++) {
		result << std::setw(element_width) << ' ';
	}
	result << std::setw(element_width-1) << 'e' << ' ';
	return result.str();
}

std::string printLineArrayStartMiddleEnd(array_size_t size, array_size_t start, array_size_t mid, array_size_t end, int element_width) {
	std::stringstream result;
	result << printArrayStartMiddleEnd(size, start, mid, end, element_width);
	result << std::endl;
	return result.str();
}



