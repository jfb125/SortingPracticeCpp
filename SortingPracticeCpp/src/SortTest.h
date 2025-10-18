/*
 * Sort_Test.h
 *
 *  Created on: Apr 15, 2025
 *      Author: joe
 */

#ifndef SORTTEST_H_
#define SORTTEST_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstddef>

#include "ArrayComposition.h"
#include "Disorganizer.h"
#include "SortFailureLog.h"
#include "InitialOrdering.h"
#include "MessageList.h"
#include "OneTestResult.h"		// results of running n tests of a given array length
#include "ResultOutput.h"
#include "SimpleRandomizer.h"
#include "SortableObject.h"
#include "SortAlgorithm.h"
#include "SortingDataTypes.h"
#include "StudentData.h"
#include "StudentDataArray.h"
#include "StudentDataGenerator.h"
#include "SortingDataTypes.h"


int initializeArraySequential(StudentData ***, int size);
int initializeArrayFewDistinct(StudentData ***, int size, int num_distinct);
int initializeArray(StudentData ***, StudentData*, int size);

void	printSideBySide(StudentDataArray &a, StudentDataArray &b);


/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*							data types - input & outputs						*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*
 * given an algorithm & an ordering
 *	for each size min ... max	returns OneTestResult
 */

OneTestResult* testOneAlgorithm(SortAlgorithms&,
								ArrayComposition&,
								InitialOrdering&,
								SimpleRandomizer&,
								array_size_t size,
								num_repetitions_t num_repetitions);


/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*								utilities										*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

void reverseArray(SortableObject *);
void minimallyRandomizeArray(SortableObject *, array_size_t num_outof_place);
bool isSorted(SortableObject *);

int getNumSizes(array_size_t min, array_size_t max, array_size_t (*next)(array_size_t));

#endif /* SORTTEST_H_ */
