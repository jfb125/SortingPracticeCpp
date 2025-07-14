/*
 * StudentDataArray.cpp
 *
 *  Created on: Jun 14, 2025
 *      Author: joe
 */

#include "StudentDataArray.h"

/* ********************************************************************	*/
/*								member functions						*/
/* ********************************************************************	*/

array_size_t StudentDataArray::size(void) const {
	return _size;
}

void StudentDataArray::swap(array_size_t i, array_size_t j) {

	StudentData *tmp = _array[i];
	_array[i] = _array[j];
	_array[j] = tmp;
}

/* ********************************************************************	*/
/*								operators								*/
/* ********************************************************************	*/

StudentData*& StudentDataArray::operator[](array_size_t i) {

	return _array[i];
}

/* ********************************************************************	*/
/*						formatted output								*/
/* ********************************************************************	*/

std::string StudentDataArray::dump(void) const {

	std::stringstream stream;

	stream << *this << std::endl;
	array_size_t i  = 0;
	for (; i != _size-1; i++) {
		if (_array[i]) {
			stream 	<< std::setw(5) << std::right
					<< i << ": "
					<< *_array[i] << std::endl;
		} else {
			stream << "nullptr" << std::endl;
		}
	}
	// do not print a line feed after the final element
	if (i) {
		if (_array[i]) {
			stream 	<< std::setw(5) << std::right
					<< i << ": "
					<< *_array[i];
		} else {
			stream << "nullptr";
		}
	}
	return stream.str();
}


std::ostream& operator<<(std::ostream& out, const StudentDataArray& object) {
	out << object._size << " elements starting at " << object._array;
	return out;
}

/* ********************************************************************	*/
/*					ctors / dtor / assignment operators					*/
/* ********************************************************************	*/

StudentDataArray::StudentDataArray(array_size_t length) {

	_array = nullptr;
	_size = 0;

	if (length >= 0) {
		_size = length;
		_array = new StudentData*[_size];
		for (array_size_t i = 0; i != _size; i++) {
			_array[i] = nullptr;
		}
	}
}

StudentDataArray::~StudentDataArray() {

	if (_array) {
		for (array_size_t i = 0; i != _size; i++) {
			if (_array[i])
				delete _array[i];
		}
		delete[] _array;
		_array = nullptr;
	}
	_size = 0;
}

StudentDataArray::StudentDataArray(const StudentDataArray &other) {

	if (&other == this) {
		return;
	}

	if (other._array) {
		_size = other._size;
		_array = new StudentData*[_size];
		for (array_size_t i = 0; i != _size; i++) {
			if (other._array[i])
				_array[i] = new StudentData(*other._array[i]);
			else
				_array[i] = nullptr;
		}
	}
}

StudentDataArray& StudentDataArray::operator=(const StudentDataArray &other) {

	if (&other != this) {
		//	delete the existing array
		if (_array) {
			for (array_size_t i = 0; i != _size; i++) {
				if (_array[i])
					delete _array[i];
			}
			delete[] _array;
			_array = nullptr;
		}
		_size = other._size;
		if (_size) {
			_array = new StudentData*[_size];
			for (array_size_t i = 0; i != _size; i++) {
				if (other._array[i])
					_array[i] = new StudentData(*other._array[i]);
				else
					_array[i] = nullptr;
			}
		}
	}
	return *this;
}

StudentDataArray::StudentDataArray(StudentDataArray &&other) noexcept {

	if (&other != this) {
		_size = other._size;
		_array = other._array;
		other._array = nullptr;
		other._size = 0;
	}
}

StudentDataArray& StudentDataArray::operator=(StudentDataArray &&other) noexcept {

	if (&other != this) {
		// delete existing array
		if (_array) {
			for (array_size_t i = 0; i != _size; i++) {
				if (_array[i])
					delete _array[i];
			}
			delete[] _array;
			_array = nullptr;
		}
		// take ownership of other's array
		_size = other._size;
		_array = other._array;
		other._array = nullptr;
		other._size = 0;
	}
	return *this;
}

