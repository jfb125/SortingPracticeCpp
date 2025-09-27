/*
 * nChooseK_test.cpp
 *
 *  Created on: Sep 25, 2025
 *      Author: Joe Baker
 */

#include <iostream>
#include <iomanip>

#include "nChoosek.h"

bool testnChoosek() {

	auto dwidth = [] (int n) -> int {
		int nd = 0;
		do {
			nd++;
			n /= 10;
		} while (n);
		return nd;
	};

	using datatype = int;
	constexpr datatype min_least_value = 1;

	constexpr int min_n = 5;
	constexpr int max_n = 7;
//	constexpr int min_k = 1;
//	constexpr int max_k = 4;

//	constexpr int advance_size = 1;

	for (int n = min_n; n <= max_n; n++) {

		std::vector<datatype> values;
		for (int i = 0; i != n; i++) {
			values.emplace_back(min_least_value + i);
		}
		for (int k = 1; k <= n; k++) {
			nChoosek<datatype> UUT(n, k, values);
 			std::cout << UUT.str() << std::endl;
			std::vector<std::vector<datatype>> combos;
			uint32_t num_combos = UUT.num_combinations();
			int dw = dwidth(num_combos);
			for (uint32_t combo_num = 0; combo_num < num_combos; ++combo_num) {
				std::vector<datatype> combo;
				UUT.next(combo);
				combos.emplace_back(combo);
				std::cout << std::setw(dw) << combo_num << ": { ";
				for (size_t elem_num = 0; elem_num != combo.size()-1; ++elem_num) {
					std::cout << combo.at(elem_num) << ", ";
				}
				std::cout << combo.at(combo.size()-1) << " }\n";
			}
			//	determine that all of the vectors are unique
			for (int i = combos.size()-1; i != 0; i--) {
				for (int j = i-1; j >= 0; j--) {
					if (combos[i] == combos[j]) {
						std::cout << "ERROR: combination duplicated at "
								  << i << " and " << j << std::endl;
						return false;
					}
				}
			}
			std::cout << "There were no duplicate combinations generated\n\n";
		}
	}
	return true;
}
