/*
 * SortingDataType_test.cpp
 *
 *  Created on: Oct 17, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "SortingDataTypes.h"

bool sortingDataTypesTest() {
	bool test_result = true;

	constexpr array_size_t 	test_array_size 	= 20;
	constexpr int			num_distinct_values = 10;

	SortingDataType<int> 	test_array[test_array_size];
	int						values[num_distinct_values];

	for (int i = 0; i != num_distinct_values; i++) {
		values[i] = i;
	}
	SortingDataTypes::assignValues(	test_array, test_array_size,
									values, num_distinct_values,
									8,
									&SortingDataTypes::to_strng<int>);

	for (int i = 0; i != test_array_size; i++) {
		std::cout << std::setw(4) << i << ": {"
				  << test_array[i].value << ", "
				  << test_array[i].index << "}\n";
	}
	return test_result;
}
