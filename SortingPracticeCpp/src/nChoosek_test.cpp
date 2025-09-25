/*
 * nChooseK_test.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: joe
 */

#include "nChoosek.h"

bool testnChoosek() {

	using datatype = int;
	constexpr datatype min_least_value = 1;
	constexpr datatype max_least_value = 1;
	constexpr datatype min_greatest_value = 9;
	constexpr datatype max_greatest_value = 9;

	constexpr int min_n = 8;
	constexpr int max_n = 9;
	constexpr int min_k = 4;
	constexpr int max_k = 4;

	constexpr int advance_size = 1;

	for (int n = min_n; n <= max_n; n++) {
		for (int k = min_k; k <= max_k; k++) {
			nChoosek<datatype> unit_under_test(n, k,
				min_greatest_value, max_greatest_value,
				advance_size);
		}
	}
	return true;
}
