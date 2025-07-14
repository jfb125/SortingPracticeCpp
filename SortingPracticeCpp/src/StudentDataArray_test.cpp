/*
 * StudentDataArray_test.cpp
 *
 *  Created on: Jun 14, 2025
 *      Author: joe
 */

#include "StudentDataArray.h"
#include <vector>

void testStudentDataArray(void) {

	constexpr int dut_size = 4;
	constexpr StudentMajor major = StudentMajor::Undeclared_Major;
	StudentDataArray dut(dut_size);

	std::cout << "dut before initialization" << std::endl;
	std::cout << dut.dump() << std::endl;

	std::string student_last_names[4] = { "Alpha", "Bravo", "Charlie", "Delta" };

	for (int i = 0; i != dut_size; i++) {
		dut[i] = new StudentData(student_last_names[i], "", major, 0.0);
	}

	std::cout << "dut after initialization" << std::endl;
	std::cout << dut.dump() << std::endl;

	StudentDataArray copied_dut(dut);

	std::cout << "copied_dut after copy initialization" << std::endl;
	std::cout << copied_dut.dump() << std::endl;

	copied_dut[0]->last_name = "Zulu";
	copied_dut[1]->last_name = "Yankee";
	copied_dut[2]->last_name = "Xray";
	copied_dut[3]->last_name = "Whiskey";

	std::cout << "copied_dut is now" << std::endl;
	std::cout << copied_dut.dump() << std::endl;
	std::cout << "dut before copy assignment operator" << std::endl;
	std::cout << dut.dump() << std::endl;

	dut = copied_dut;

	std::cout << "dut after copy assignment operator" << std::endl;
	std::cout << dut.dump() << std::endl;

	StudentDataArray moved_dut(std::move(dut));
	std::cout << "dut after move assignment operator" << std::endl;
	std::cout << dut.dump() << std::endl;
	std::cout << "moved_dut" << std::endl;
	std::cout << moved_dut.dump() << std::endl;

	copied_dut[0]->last_name = "Papa";
	copied_dut[1]->last_name = "Quebec";
	copied_dut[2]->last_name = "Romeo";
	copied_dut[3]->last_name = "Sierra";

	std::cout << "copied_dut before copied_dut = std::move(moved_dut)" << std::endl;
	std::cout << copied_dut.dump() << std::endl;
	std::cout << "moved_dut before copied_dut = std::move(moved_dut)" << std::endl;
	std::cout << moved_dut.dump() << std::endl;
	copied_dut = std::move(moved_dut);
	std::cout << "copied_dut before copied_dut = std::move(moved_dut)" << std::endl;
	std::cout << copied_dut.dump() << std::endl;
	std::cout << "moved_dut before copied_dut = std::move(moved_dut)" << std::endl;
	std::cout << moved_dut.dump() << std::endl;
}

