/*
 * SortFailureLog.h
 *
 *  Created on: Jun 17, 2025
 *      Author: joe
 */

#ifndef SORTFAILURELOG_H_
#define SORTFAILURELOG_H_

#include <iostream>
#include <iomanip>
#include <sstream>

//#include "SortingDataType.h"
#include "SortingDataTypes.h"

template <typename T>
class SortFailureLog {

public:
	array_size_t array_size;
	T* input_array;
	T* result_array;
	IsSortedResult _diagnostics;
	std::string *_message;


	std::string to_string(void) const {
		std::stringstream retval;

		if (_diagnostics._is_sorted) {
			retval << "    no failure occurred" << std::endl;
		} else {
			retval << "!!!!!!!!!! **** failure occurred **** !!!!!!!!!!" << std::endl;
			retval 	<< "  elements out-of-place were ["
					<< _diagnostics._mismatched_index_i
					<< "] vs ["
					<< _diagnostics._mismatched_index_j << "]"
					<< std::endl;
		}

		if (_message) {
			retval << "  " << *_message;
		} else {
			retval << "   (no failure log messages)";
		}

		return retval.str();
	}

	void clear(void)
	{

		if (input_array) {
			delete input_array;
			input_array = nullptr;
		}
		if (result_array) {
			delete result_array;
			result_array = nullptr;
		}
		if (_message) {
			delete _message;
			_message = nullptr;
		}
		_diagnostics._is_sorted = false;
		_diagnostics._mismatched_index_i = 0;
		_diagnostics._mismatched_index_j = 0;
	}


	SortFailureLog() {
		_diagnostics.clear();
		input_array 	= nullptr;
		result_array 	= nullptr;
		array_size		= 0;
		_message 		= nullptr;
	}
	virtual ~SortFailureLog() {
		if (input_array) {
			delete[] input_array;
			input_array = nullptr;
		}
		if (result_array) {
			delete[] result_array;
			result_array = nullptr;
		}
		if (_message) {
			delete _message;
			_message = nullptr;
		}
		_diagnostics.clear();
	}
	// deep copies of pointer objects
	SortFailureLog(IsSortedResult diagnostics,
				   SortingDataType<T> *input,
				   SortingDataType<T> *result,
				   array_size_t size,
				   std::string *message)
	{
		array_size 	= size;

		if (input) {
			input_array = new SortingDataType<T>[array_size];
			for (array_size_t i = 0; i != array_size; i++) {
				input_array[i] = input[i];
			}
		} else {
			input_array = nullptr;
		}

		if (result) {
			result_array = new SortingDataType<T>[array_size];
			for (array_size_t i = 0; i != array_size; i++) {
				result_array[i] = result[i];
			}
		} else {
			result_array = nullptr;
		}

		if (message) {
			_message = new std::string(*message);
		} else {
			_message = nullptr;
		}

		_diagnostics= diagnostics;
	}

	SortFailureLog(const SortFailureLog &other)
	{
		if (this != &other) {
			array_size = other.array_size;

			if(other.input_array) {
				for (array_size_t i = 0; i != array_size; i++) {
					input_array[i] = other.input_array[i];
				}
			} else {
				input_array = nullptr;
			}

			if (other.result_array) {
				for (array_size_t i = 0; i != array_size; i++) {
					result_array[i] = other.result_array[i];
				}
			} else {
				result_array = nullptr;
			}

			if (other._message) {
				_message = new std::string(*other._message);
			} else {
				_message = nullptr;
			}

			_diagnostics = other._diagnostics;
		}
	}

	SortFailureLog& operator=(const SortFailureLog &other) {

		if (this != &other) {
			array_size = other.array_size;

			if (other.input_array) {
				for (array_size_t i = 0; i != array_size; i++) {
					input_array[i] = other.input_array[i];
				}
			} else {
				input_array = nullptr;
			}

			if (other.result_array) {
				for (array_size_t i = 0; i != array_size; i++) {
					result_array[i] = other.result_array[i];
				}
			} else {
				result_array = nullptr;
			}

			if (other._message) {
				_message = new std::string(*other._message);
			} else {
				_message = nullptr;
			}
			_diagnostics = other._diagnostics;
		}
	}

	SortFailureLog(SortFailureLog &&other) noexcept
	{
		array_size			= other.array_size;
		input_array 		= other.input_array;
		other.input_array 	= nullptr;
		result_array 		= other.result_array;
		other.result_array 	= nullptr;
		_message = other._message;
		other._message 		= nullptr;
		_diagnostics 		= other._diagnostics;
	}

	SortFailureLog& operator=(SortFailureLog &&other) noexcept {
		if (this != &other) {
			array_size			= other.array_size;
			input_array 		= other.input_array;
			other.input_array 	= nullptr;
			result_array 		= other.result_array;
			other.result_array 	= nullptr;
			_message 			= other._message;
			other._message 		= nullptr;
			_diagnostics 		= other._diagnostics;
		}
		return *this;
	}

	SortFailureLog& set_input(T*input, array_size_t size) {
		if (input_array) {
			delete input_array;
		}

		if (size < array_size) {
			array_size = size;
		}

		input_array = new T[array_size];
		for (array_size_t i = 0; i != array_size; i++) {
			input_array[i] = input[i];
		}
		return *this;
	}

	SortFailureLog& set_result(T* result, array_size_t size) {
		if (result_array) {
			delete result_array;
		}

		if (size < array_size) {
			array_size = size;
		}

		result_array = new T[array_size];
		for (array_size_t i = 0; i != array_size; i++) {
			result_array[i] = result[i];
		}
		return *this;
	}
};

template <typename T>
std::ostream& operator<<(std::ostream &out, const SortFailureLog<T> &log) {
	out << log.to_string();
	return out;
}

#endif /* SORTFAILURELOG_H_ */
