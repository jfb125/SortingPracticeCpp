/*
 * TestFixtures.cpp
 *
 *  Created on: Oct 26, 2025
 *      Author: joe
 */

#include "TestFixtures.h"

bool testPermutationGenerator() {

	bool debug_verbose = false;
	bool test_passed = true;

	int gen_values[] = { 1, 2, 3, 4, 6 };
	int test_vector_size = 6;
	PermutationGenerator<int> test_vector_generator(gen_values, test_vector_size);
	int num_vectors = test_vector_generator.num_vectors();
	int **vectors = new int*[num_vectors];
	for (int i = 0; i != num_vectors; i++) {
		vectors[i] = new int[test_vector_size];
	}

	std::cout << "number " << arrayIndicesToString(test_vector_size) << std::endl;
	int vector_number = 0;
	while (!test_vector_generator.is_done() && vector_number != num_vectors) {
		test_vector_generator.next(vectors[vector_number]);
		if (debug_verbose) {
			std::cout << std::setw(5) << vector_number << ": "
				<< SortingUtilities::arrayElementsToString(
						vectors[vector_number],test_vector_size)
				<< std::endl;
		}
		vector_number++;
	}
	std::cout << "Verifying uniqueness\n";
	bool are_same = true;
	for (int i = 0; i < num_vectors-1; i++) {
		for (int j = i+1; j < num_vectors; j++) {
			are_same = true;
			for (int k = 0; k != test_vector_size; k++) {
				if (vectors[j][k] != vectors[i][k]) {
					are_same = false;
					break;
				}
			}
			if (are_same) {
				test_passed = false;
				std::cout << "ERROR: TWO IDENTICAL TEST VECTORS:\n"
						  << std::setw(5) << i << ": "
						  << SortingUtilities::arrayElementsToString(
								  vectors[i], test_vector_size)
						  << std::endl
						  << std::setw(5) << j << ": "
						  << SortingUtilities::arrayElementsToString(
								  vectors[j], test_vector_size)
						  << std::endl;
				goto TEST_PERMUTATION_GENERATOR_RETURN;
			}
		}
	}

TEST_PERMUTATION_GENERATOR_RETURN:
	if (test_passed) {
		std::cout << "PermutationGenerator generated: " << vector_number
				  << " vectors when expected " << num_vectors
				  << " which are all unique\n";
	}

	return test_passed;
}


