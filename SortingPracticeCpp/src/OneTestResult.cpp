/*
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#include "OneTestResult.h"

bool isValid(TestResultMember object) {
	switch (object) {
	case TestResultMember::ALGORITHM:
	case TestResultMember::ARRAY_COMPOSITION:
	case TestResultMember::INITIAL_ORDER:
	case TestResultMember::ARRAY_SIZE:
	case TestResultMember::SORT_FAILURE_LOG:
	case TestResultMember::SORT_METRICS:
		return true;
	default:
		return false;
	}
}

TestResultMember& operator++(TestResultMember& value) {

	if (value == MAX_TEST_RESULT_MEMBER) {
		value = MIN_TEST_RESULT_MEMBER;
	} else {
		int v_i = static_cast<int>(value);
		value = static_cast<TestResultMember>(++v_i);
	}
	return value;
}

TestResultMember  operator++(TestResultMember &value, int unused) {
	TestResultMember before = value;
	++value;
	return before;
}
//	fundamental
bool operator==(TestResultMember u, TestResultMember v) {
	return static_cast<int>(u) == static_cast<int>(v);
}
bool operator<(TestResultMember u, TestResultMember v) {
	return static_cast<int>(u) < static_cast<int>(v);
}
//	derived
bool operator<=(TestResultMember u, TestResultMember v) {
	return u == v || u < v;
}
bool operator>(TestResultMember u, TestResultMember v) {
	return !(u == v) && !(u < v);
}
bool operator>=(TestResultMember u, TestResultMember v) {
	return !(u < v);
}
bool operator!=(TestResultMember u, TestResultMember v) {
	return !(u == v);
}
