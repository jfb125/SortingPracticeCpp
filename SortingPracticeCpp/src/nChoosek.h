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

enum class nChoosekMode {
	USER_DEFINED,
	AUTO_GENERATED
};

template <typename T>
class nChoosek {
private:

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							member variables							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	nChoosekMode m_mode;

	uint32_t m_n;	// total number of elements in array
	uint32_t m_k;	// number of elements selected in returned vector

	//	In AUTO_GENERATED mode, the least, greatest, and gap between values
	//	Note that T must be a type for which operator++() and operator<=() is defined
	T m_least_value;
	T m_greatest_value;
	uint32_t m_value_gap;

	//	In USER_DEFINED mode the set of values from which to choose
	std::vector<T>	m_values;

	//	The current state of the sample generator
	std::vector<T>	m_current_state;

	uint32_t m_num_states;
	uint32_t m_sequence_number;

	uint32_t m_max_num_states = UINT32_MAX;

public:
	nChoosek() = delete;
	//	automatically sets mode to AUTO_GENERATED
	nChoosek(uint32_t n, uint32_t k, T least, T greatest, uint32_t incr_size) :
		m_mode(nChoosekMode::AUTO_GENERATED),
		m_n(n), m_k(k),
		m_least_value(least), m_greatest_value(greatest), m_value_gap(incr_size) {
		reset();
	}
	//	automatically sets mode to USER_DEFINED
	nChoosek(int n, int k, std::vector<T> &values) :
		m_mode(nChoosekMode::USER_DEFINED),
		m_n(n), m_k(k),
		m_values(values) {
		m_value_gap = 0;
		reset();
	}

	virtual ~nChoosek() {}

	nChoosek(const nChoosek &other) {
		if (*this != other) {
			m_mode 			= other.m_mode;
			m_n 			= other.m_n;
			m_k 			= other.m_k;
			m_least_value	= other.m_least_value;
			m_greatest_value= other.m_greatest_value;
			m_value_gap		= other.m_value_gap;
			m_values		= other.m_values;
			m_current_state	= other.m_current_state;
		}
	}

	nChoosek& operator=(const nChoosek &other) {
		if (*this != other) {
			m_mode 			= other.m_mode;
			m_n 			= other.m_n;
			m_k 			= other.m_k;
			m_least_value	= other.m_least_value;
			m_greatest_value= other.m_greatest_value;
			m_value_gap		= other.m_value_gap;
			m_values		= other.m_values;
			m_current_state	= other.m_current_state;
		}
		return *this;
	}

	//	returns the current state, then moves teh generator forward by one
	std::vector<T> next() {
		std::vector<T> *result = new std::vector<T>(m_current_state);
		nextStateForward();
		return *result;
	}
	//	returns the current state, then moves the generator backwards by one
	std::vector<T> prev() {
		std::vector<T> *result = new std::vector<T>(m_current_state);
		nextStateBackwards();
		return *result;
	}

	//	Changing the mode should only be done after the parameters are set:
	//	  m_values should be set before mode(mChoosekMode::USER_DEFINED)
	//	  m_least_value, m_greatest_value, m_incr should be set before
	//		mode(mChooseMode::AUTO_GENERATED)
	nChoosekMode mode(nChoosekMode new_mode);
	nChoosekMode mode();

	//	This does not automatically assume that 'n' should be set to _values.size()
	//	  if _n is < _values.size(), the first _n elements from _values are stored
	//	  if _values.size() < n, the first .size() values are stored
	//	The caller is responsible for checking that the returned vector matches
	//	  the passed vector
	const std::vector<T> values(int _n, std::vector<T> &_values);
	const std::vector<T> values();

	int	n() const;
	int n(int _n);
	int k() const;
	int k(int _k);
	int incr() const;
	int incr(int _incr);

	//	returns (n!)/((k!)*(n-k)!)
	int length();

	std::string str();


	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							algorithm methods							*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

	//	Restores _state to initial state.  If mode is AUTO_GENERATED, the m_values
	//	is regenerated
	void reset() {

	}

	/*	******************************************************************	*/
	/*	******************************************************************	*/
	/*							private methods								*/
	/*	******************************************************************	*/
	/*	******************************************************************	*/

private:
	void nextStateForward() {

	}

	void nextStateBackwards() {

	}

	void generateValues() {

	}
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const nChoosek<T> & object);

#endif /* NCHOOSEK_H_ */
