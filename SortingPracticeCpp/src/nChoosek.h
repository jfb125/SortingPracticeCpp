/*
 * nChooseK.h
 *
 *  Created on: Sep 25, 2025
 *      Author: Joe Baker
 */

#ifndef NCHOOSEK_H_
#define NCHOOSEK_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <inttypes.h>
#include <limits>

#include "OStreamState.h"

#if 0
enum class nChoosekMode {
//	USER_DEFINED,
	AUTO_GENERATED
};
#endif

enum nChoosekStatus {
	NO_ERROR 							= 0x00,
	ERROR_NUM_COMBINATIONS_TOO_LARGE 	= 0x01,
	WARNING_k_TRUNCATED_TO_n 			= 0x02,
	WARNING_n_TRUNCATED_TO_NUM_VALUES	= 0x04
};

using nCk_index_t 					= uint32_t;
using nCk_status_t					= unsigned;
using nCk_num_combos_t				= uint32_t;

template <typename T>
class nChoosek {
private:

	uint64_t 	m_max_num_combinations 	= INT32_MAX;
	//	m_num_combinations has to be at least 1 for seq = (++seq) % num_combos;
	nCk_index_t m_min_num_combinations	= 1;

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							member variables							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	nCk_index_t m_n;	// total number of elements in array
	nCk_index_t m_k;	// number of elements selected in returned vector

	//	In USER_DEFINED mode the set of values from which to choose
	std::vector<T>	m_values;

	//	The current state of the sample generator
	std::vector<nCk_index_t> m_indices;

	nCk_num_combos_t m_num_combinations;
	nCk_num_combos_t m_sequence_number;
	nCk_status_t m_status;

public:

	/*	******************************************************************	*/
	/*						constructors / destructor						*/
	/*	******************************************************************	*/

	nChoosek() = delete;

	nChoosek(nCk_index_t n, nCk_index_t k, std::vector<T> &values) {

		initialize(n, k, values);
	}

	virtual ~nChoosek() {}

	nChoosek(const nChoosek &other) {
		if (*this != other) {
			m_n 				= other.m_n;
			m_k 				= other.m_k;
			m_values			= other.m_values;
			m_indices			= other.m_indices;
			m_num_combinations	= other.m_num_combinations;
			m_sequence_number	= other.m_sequence_number;
			m_status			= other.m_status;
		}
	}

	nChoosek& operator=(const nChoosek &other) {
		if (*this != other) {
			m_n 				= other.m_n;
			m_k 				= other.m_k;
			m_values			= other.m_values;
			m_indices			= other.m_indices;
			m_num_combinations	= other.m_num_combinations;
			m_sequence_number	= other.m_sequence_number;
			m_status			= other.m_status;
		}
		return *this;
	}

	/*	**************************************************************	*/
	/*						initializing modes							*/
	/*	**************************************************************	*/

	nCk_status_t initialize(nCk_index_t n, nCk_index_t k, std::vector<T> &values) {

		m_status = nChoosekStatus::NO_ERROR;
		validateAndStoreUserVariables(n, k, values);	// updates status
		m_num_combinations = calcNumCombinations(n, k);	// updates status
		initializeState();
		return m_status;
	}

	/*	initializes the indices state vector	*/

	nCk_status_t reset() {

		initializeState();
		return m_status;
	}

	/*	**************************************************************	*/
	/*						getting a combination						*/
	/*	**************************************************************	*/

	nCk_num_combos_t current(std::vector<T> &combination) {
		assignCombination(combination);
		return m_sequence_number;
	}

	//	returns the sequence number of the vector stored to 'combination'
	nCk_num_combos_t next(std::vector<T> &combination) {
		nCk_index_t result = m_sequence_number;
		assignCombination(combination);
		m_sequence_number++;
		m_sequence_number %= m_n;
		nextStateForward();
		return result;
	}

	//	returns the sequence number of the vector stored to 'combination'
	nCk_num_combos_t prev(std::vector<T> &combination) {
		nCk_index_t result = m_sequence_number;
		assignCombination(combination);
		if (m_sequence_number == 0) {
			m_sequence_number = m_n;
		}
		m_sequence_number--;
		nextStateBackwards();
		return result;
	}

	/*	******************************************************************	*/
	/*								accessors								*/
	/*	******************************************************************	*/

	nCk_index_t k() 					const	{ 	return m_k; 				}
	nCk_index_t n() 					const	{ 	return m_n; 				}
	nCk_num_combos_t num_combinations() const	{	return m_num_combinations; 	}
	nCk_num_combos_t seq_number() 		const	{	return m_sequence_number;	}
	nCk_status_t status() 				const	{ 	return m_status; 			}
	std::vector<T> values()				const	{ 	return m_values;			}

	/*	******************************************************************	*/
	/*								formatted output						*/
	/*	******************************************************************	*/

	std::string str() {

		OStreamState ostream_state;	// restores ostream state in destructor
		std::stringstream result;

		result  << "(n = " << std::setw(3) << m_n
				<< ", k = " << std::setw(3) << m_k
				<< ") generates " <<m_num_combinations << " combinations of values { ";
		if (m_values.size()) {
			for (auto it = m_values.begin(); it != std::prev(m_values.end()); ++it) {
				result << *it << " ";
			}
			result << m_values.at(m_values.size()-1);
		}
		result << " }";
		return result.str();
	}

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							private methods								*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

private:
	/*
	 * creates a combination by copying individual m_values into 'combination'
	 * 	based upon the indices of each digit { 0 : k-1 }
	 * 		combination[i] = m_values[m_indices[i]]
	 */
	void	assignCombination(std::vector<T> &combination) {
		combination.clear();
		for (int i :  m_indices) {
			combination.emplace_back(m_values.at(i));
		}
	}

	/* 	Calculates the binomial coefficient - copied off of Google AI		*/
	/*	Modifies m_status if the resulting number of combinations > maximum	*/
	nCk_num_combos_t calcNumCombinations(nCk_index_t n, nCk_index_t k) {
		if (k == 0 || k == n) {
			return 1; // Base case: C(n, 0) = 1 and C(n, n) = 1
		}
		if (k > n / 2) {
			k = n - k; // Optimization: C(n, k) = C(n, n-k)
		}

		uint64_t res = 1;
		for (nCk_index_t i = 1; i <= k; ++i) {
			res = res * (n - i + 1) / i;
		}
		if (res > m_max_num_combinations) {
			m_status |= nChoosekStatus::ERROR_NUM_COMBINATIONS_TOO_LARGE;
			m_num_combinations = 1;
		} else {
			m_num_combinations = res;
		}
		return res;
	}

	void initializeState() {
		m_sequence_number	= 0;
		m_indices.clear();
		for (nCk_index_t i = 0; i < m_k; i++) {
			m_indices.emplace_back(i);
		}
	}

	/*
	 * Consider the case n = 5, k = 3 of values { A, B, C, D, E }
	 *
	 * The sequence generated will be:
	 *   0  1  2  are the indices of the 'value_i' vector
	 *   value_i	  values
	 * { 0, 1, 2 }	{ A, B, C }		++[2] = 3
	 * { 0, 1, 3 }	{ A, B, D }		++[2] = 4
	 * { 0, 1, 4 }	{ A, B, E }		  [2] = 4, ++[1] = 2			-> { 0, 2, 3 }
	 * { 0, 2, 3 }	{ A, C, D }		++[2] = 3
	 * { 0, 2, 4 }	{ A, C, E }		++[2] = 4, ++[1] = 3			-> { 0, 3, 4 }
	 * { 0, 3, 4 }	{ A, D, E }		  [2] = 4,   [1] = 3, ++[0] = 1 -> { 1, 2, 3 }
	 * { 1, 2, 3 }	{ B, C, D }		++[2] = 4
	 * { 1, 2, 4 }	{ B, C, E }		  [2] = 4, ++[1] = 3 			-> { 1, 3, 4 }
	 * { 1, 3, 4 }	{ B, D, E }		++[3] = 4,   [1] = 3, ++[0] = 2 -> { 2, 3, 4 }
	 * { 2, 3, 4 }	{ C, D, E }		  [3] = 4,   [1] = 3,   [0] = 2 -> { 0, 1, 2 }
	 * { 0, 1, 2 }  { A, B, C }
	 *
	 *	Note that 'max_i' for any index 'v_i' is given by: n-(k-update_ptr)
	 *		update_ptr		max_i
	 *			2			5 - (3-2) = 4
	 *			1			5 - (3-1) = 3
	 *			0			5 - (3-0) = 2
	 *
	 *	When a carry does occur into the next digit to the left, the value_indices
	 *	  restart in ascending order from the digit that DID not carry
	 *	  { 1, 2, 4 } becomes { 1, 3, ? }, ? is set to 3+1 = 4
	 *
	 *	  { 1, 3, 4 } becomes { 2, ?, ? }, the unknowns are set to 2+1 = 3, 3+1 = 4
	 */

	void nextStateForward() {

		//	starting with the rightmost (least significant) value_index
		int num_value_indices = static_cast<int>(m_k);
		int active_digit = num_value_indices-1;
		while (active_digit != -1) {
			//	determine the maximum value for this value_index
			nCk_index_t max = m_n - (m_k-active_digit);
			//	if this value_index < max, just increment it and done
			if (m_indices.at(active_digit) != max) {
				m_indices.at(active_digit)++;
				break;
			}
			//	this value_index == max, must carry to the left
			active_digit--;
		}
		//	if the preceding loop terminated with at least one digit not at max
		if (active_digit != -1) {
			//	rebuild the vector of value_indices to the right
			while (active_digit < num_value_indices-1) {
				m_indices.at(active_digit+1) =
						m_indices.at(active_digit)+1;
				active_digit++;
			}
		} else {
			//	all value_index were at their max, restart the value_indices
			m_indices.clear();
			for (int i = 0; i < num_value_indices; i++) {
				m_indices.emplace_back(i);
			}
		}
	}

	void nextStateBackwards() {

	}

	nCk_status_t validateAndStoreUserVariables(nCk_index_t n, nCk_index_t k,
											   std::vector<T> &values) {

		nCk_status_t result = nChoosekStatus::NO_ERROR;
		if (values.size() < n) {
			result |= nChoosekStatus::WARNING_n_TRUNCATED_TO_NUM_VALUES;
			n = values.size();
		}

		if (k > n) {
			result |= nChoosekStatus::WARNING_k_TRUNCATED_TO_n;
			k = n;
		}
		m_k 	= k;
		m_n 	= n;
		m_values= values;
		return result;
	}
};	// class nChoosek

template <typename T>
std::ostream& operator<<(std::ostream& out, const nChoosek<T> & object) {
	out << object.str();
	return out;
}

#endif /* NCHOOSEK_H_ */
