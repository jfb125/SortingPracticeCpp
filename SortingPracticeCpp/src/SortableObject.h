/*
 * SortableObject.h
 *
 *  Created on: Apr 17, 2025
 *      Author: joe
 */

#ifndef SORTABLEOBJECT_H_
#define SORTABLEOBJECT_H_

#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>

class SortableObject {
private:
	SortableObject();

public:
	void **list;
	int size_of_list;
	size_t size_of_element;
	int (*compare)(void *u, void *v);
	bool credible;	// true means no nullptrs in the object

	SortableObject(void **list, int num_elements, size_t size_of_element, int (*compare)(void*, void*));
	SortableObject(const SortableObject&);
	SortableObject(SortableObject&&);

	SortableObject& operator=(SortableObject&);
	SortableObject& operator=(SortableObject&&);

	bool isCredible() const	{	return credible;	}
};

std::ostream& operator<<(std::ostream&, const SortableObject&);

#endif /* SORTABLEOBJECT_H_ */
