/*
 * StudentDataArray.h
 *
 *  Created on: Jun 14, 2025
 *      Author: joe
 *
 *  An array of fixed size that cannot be resized
 */

#ifndef STUDENTDATAARRAY_H_
#define STUDENTDATAARRAY_H_

#include <iostream>
#include <string>
#include <sstream>

#include "StudentData.h"
#include "SortingPracticeDataTypes.h"

void testStudentDataArray();

class StudentDataArray {
public:
	StudentData** _array;
	array_size_t _size;


public:
	array_size_t size(void) const;
	StudentData*& operator[](array_size_t);
	std::string dump(void) const;
	void swap(array_size_t i, array_size_t j);
	StudentDataArray() = delete;
	StudentDataArray(array_size_t);
	virtual ~StudentDataArray();
	StudentDataArray(const StudentDataArray &other);
	StudentDataArray& operator=(const StudentDataArray &other);
	StudentDataArray(StudentDataArray &&other) noexcept;
	StudentDataArray& operator=(StudentDataArray &&other) noexcept;

	friend std::ostream& operator<<(std::ostream&, const StudentDataArray&);
};

#endif /* STUDENTDATAARRAY_H_ */
