/*
 * BlockSort.h
 *
 *  Created on: Aug 11, 2025
 *      Author: joe
 */
/*
 * BubbleSort.h
 *
 *  Created on: Jun 18, 2025
 *      Author: joe
 */

#ifndef BLOCKSORT_H_
#define BLOCKSORT_H_

#include "SortingPracticeDataTypes.h"
#include "SortingUtilities.h"

namespace BlockSort {

	template <typename T>
	void testBlockSort() {
		std::cout << "testBlockSort()" << std::endl;
	}
	template <typename T>
	ComparesAndMoves sortPointersToObjects(T **array_of_pointers, array_size_t size) {
		return *(new ComparesAndMoves(0,0));
	}
}

#endif /* BLOCKSORT_H_ */
