/*
 * SimpleRandomizer_test.cpp
 *
 *  Created on: Jun 13, 2025
 *      Author: joe
 */

#include "SimpleRandomizer.h"

#pragma push_macro("toIndex")
#define toIndex(i)	(static_cast<uint32_t>(i) - min_value)

#define FREQUENCY_WIDTH 12

int numDigits(uint64_t value);
void printTestHeader(uint64_t min_value, uint64_t max_value, int z);
void printTestFooter();
void printSimpleRandomizerTestResults(uint32_t* results, uint64_t min_value, uint64_t max_value);

uint64_t testSimpleRandomizer(uint64_t min_value, uint64_t max_value) {

	int num_repeats = 10;
	uint64_t num_errors = 0;

	if (min_value >= max_value) {
		std::cout << "ERROR: testSimpleRanomizer(min, max) with min " << min_value
				  << " >= max " << max_value << std::endl;
		return num_errors;
	}

	std::cout << "Testing [" << min_value << ", " << max_value
			  << "] for randomness" << std::endl;
	SimpleRandomizer randomizer(0);
	uint32_t number_of_tries = 100000000;

	for (int z = 0; z != num_repeats; z++) {

		uint32_t num_values = (max_value - min_value + 1) % UINT32_MAX;
		uint32_t occurences[num_values];
		for (uint32_t i = 0; i != num_values; i++) {
			occurences[i] = 0;
		}

		uint32_t random_value;

		for (uint32_t i = 0; i != number_of_tries; i++) {
			random_value = randomizer.rand(min_value, max_value);
			if (random_value >= max_value)
				std::cout << "ERROR: random_value " << random_value << " is greater than max_value-1 " << max_value << std::endl;
			else
			if (random_value < min_value)
				std::cout << "ERROR: random_value " << random_value << " is less than min_value " << min_value << std::endl;
			occurences[random_value - min_value]++;
		}
		printTestHeader(min_value, max_value, z);
		printSimpleRandomizerTestResults(occurences, min_value, max_value);
		printTestFooter();
		if (z % 2 == 1) {
			randomizer.seed(z);
		}
		randomizer.restart();
	}

	return num_errors;
}

void printSimpleRandomizerTestResults(uint32_t *occurences,
					  uint64_t min_value,
					  uint64_t max_value) {
#if 0
	std::cout << "Results of test generating values on ["
			  <<  min_value << ", " << max_value << "]"
			  << std::endl;
#endif
	uint64_t num_values = max_value - min_value + 1;

	uint32_t num_runs = 0;
	for (uint32_t i = 0; i != num_values; i++) {
		num_runs += occurences[i];
	}

	// calculate the width of the representation of each frequency
	uint64_t max_count = 0;
	for (uint32_t i = 0; i != num_values; i++) {
		if (occurences[i] > max_count) {
			max_count = occurences[i];
		}
	}

	// print the frequencies, while summing for the average
	uint64_t sum = 0;
	for (uint32_t i = 0; i != num_values; i++) {
		std::cout << std::setw(FREQUENCY_WIDTH) << std::right
				<< occurences[i] << "  ";
		sum += occurences[i];
	}
	std::cout << std::setw(FREQUENCY_WIDTH) << sum << std::endl;

	double deltas[num_values];

	double average = static_cast<double>(sum) / (num_values-1);
	double delta;
	double delta_sum = 0.0;
	double smallest_delta = 0.0;
	double largest_delta = 0.0;
	for (uint32_t i = 0; i != num_values; i++) {
		delta = (static_cast<double>(occurences[i])) - average;
		delta /= average;
		delta /= num_values;
//		delta *= 1000;	// improves readability
		if (delta < smallest_delta)
			smallest_delta = delta;
		else if (delta > largest_delta)
			largest_delta = delta;
		delta_sum += delta;
		deltas[i] = delta;
	}

	int booster = 10;
	double delta_seeker = largest_delta;

	while (delta_seeker < 1.0) {
		booster *= 10;
		delta_seeker *= 10.0;
	}

	booster /= 10;

	for (uint32_t i = 0; i != num_values; i++) {
		std::cout << std::setw(FREQUENCY_WIDTH-1) << std::fixed << std::setprecision(1) << std::right
				  << deltas[i] * 100000.0 << "m% ";
//				  << deltas[i] * booster << "  ";
	}

	std::cout << " sum of deltas: " << (delta_sum < 1e-15 ? 0 : delta_sum);
	std::cout << std::endl
			  << std::setw(FREQUENCY_WIDTH) << std::right
			  << "smallest delta: " << smallest_delta
			  << " largest delta: " << largest_delta
			  << std::endl;
}

void printTestHeader(uint64_t min_value, uint64_t max_value, int z) {

	std::cout << "Results of test generating values on ["
			  <<  min_value << ", " << max_value << "]"
			  << " using seed: " << z
			  << std::endl;

	//	print the header
	for (uint64_t i = min_value; i <= max_value; i++) {
		std::cout << std::setw(FREQUENCY_WIDTH) << i << "  ";
	}
	std::cout << "Total Occurences: " << std::setw(0) << std::endl;
}

void printTestFooter(void) {
	std::cout << "---------------------------" << std::endl;
}

int numDigits(uint64_t value) {

	int num = 0;

	do {
		num++;
		value /= 10;
	} while (value);

	return num;
}

#pragma pop_macro("toIndex")
