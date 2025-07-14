/*
 * OptimizedQuickSortTest.cpp
 *
 *  Created on: Jul 4, 2025
 *      Author: joe
 */

#include "OptimizedQuickSortTest.h"

static StudentData student_A("A", "z", StudentMajor::Undeclared_Major);
static StudentData student_B("B", "z", StudentMajor::Undeclared_Major);
static StudentData student_C("C", "z", StudentMajor::Undeclared_Major);

static StudentData* order_ABC[] = { &student_A, &student_B, &student_C };
static StudentData* order_ACB[] = { &student_A, &student_C, &student_B };
static StudentData* order_BAC[] = { &student_B, &student_A, &student_C };
static StudentData* order_BCA[] = { &student_B, &student_C, &student_A };
static StudentData* order_CAB[] = { &student_C, &student_A, &student_B };
static StudentData* order_CBA[] = { &student_C, &student_B, &student_A };

static StudentData* order_AAB[] = { &student_A, &student_A, &student_B };
static StudentData* order_ABA[] = { &student_A, &student_B, &student_A };
static StudentData* order_BAA[] = { &student_B, &student_A, &student_A };

static StudentData* order_AAA[] = { &student_A, &student_A, &student_A };

struct SelectMedianTestCase {
	StudentData** m_inputs;
	array_size_t m_expected;
	SelectMedianTestCase(StudentData** data, array_size_t expected) :
		m_inputs(data),
		m_expected(expected) {}
};

static array_size_t return_dont_care = 99;

SelectMedianTestCase case_ABC(order_ABC, 1);
SelectMedianTestCase case_ACB(order_ACB, 2);
SelectMedianTestCase case_BAC(order_BAC, 0);
SelectMedianTestCase case_BCA(order_BCA, 0);
SelectMedianTestCase case_CAB(order_CAB, 2);
SelectMedianTestCase case_CBA(order_CBA, 1);

SelectMedianTestCase case_AAB(order_AAB, return_dont_care);
SelectMedianTestCase case_ABA(order_ABA, return_dont_care);
SelectMedianTestCase case_BAA(order_BAA, return_dont_care);

SelectMedianTestCase case_AAA(order_AAA, return_dont_care);

SelectMedianTestCase test_cases[] {
		case_ABC, case_ACB, case_BAC, case_BCA, case_CAB, case_CBA,
		case_AAB, case_ABA, case_BAA,
		case_AAA
};

static int num_test_cases = sizeof(test_cases)/sizeof(SelectMedianTestCase);
static int num_students_per_test_case = sizeof(order_ABC)/sizeof(StudentData*);

std::string toString(const SelectMedianTestCase &object) {

	std::stringstream ret_val;
	ret_val << "{ ";
	for (int i = 0; i != num_students_per_test_case-1; i++) {
		ret_val << object.m_inputs[i]->last_name << ", ";
	}
	ret_val << object.m_inputs[num_students_per_test_case-1]->last_name << "})";

	if (object.m_expected != return_dont_care) {
		ret_val << " expecting " << std::setw(2) << object.m_expected;
	} else {
		ret_val << " <don't care>";
	}
	return ret_val.str();
}

// to use the test,
// member method selectPivot() must be made public
// in class OptimizedQuickSort<StudentData>
bool testSelectPivot() {

	bool test_passed = true;
	OptimizedQuickSort<StudentData> sort;
	array_size_t returned_pivot = 10000;

	for (int i = 0; i != num_test_cases; i++) {
//		returned_pivot = sort.selectPivot(test_cases[i].m_inputs, 0, num_students_per_test_case-1);
		std::cout << std::setw(2) << i << ": "
				  << "selectPivot(" << toString(test_cases[i])
				  << " returns " << returned_pivot
				  << std::endl;
		if (test_cases[i].m_expected != return_dont_care) {
			if (test_cases[i].m_expected != returned_pivot)
				test_passed = false;
		}
	}
	return test_passed;
}
