/*
 * SortableObject.cpp
 *
 *  Created on: Apr 17, 2025
 *      Author: joe
 */

#include "SortableObject.h"

bool isSortableObjectCredible(SortableObject& obj) {
	return obj.list != nullptr && obj.compare != nullptr;
}

//	void **list;
//	int size_in_list;
//	size_t size_of_element;
//	int (*compare)(void *u, void *v);
//	bool credible;

SortableObject::SortableObject() :
	list(nullptr),
	size_of_list(0),
	size_of_element(0),
	compare(nullptr),
	credible(false)
{ }

SortableObject::SortableObject(void **list, int size_of_list, size_t size_of_element, int (*compare)(void*, void*)) :
	list(list), size_of_list(size_of_list), size_of_element(size_of_element), compare(compare)
{
	credible = isSortableObjectCredible(*this);
}

SortableObject::SortableObject(const SortableObject &other) {

	if (this == &other)
		return;

	list = other.list;
	size_of_list = other.size_of_list;
	size_of_element = other.size_of_element;
	compare = other.compare;
	credible = isSortableObjectCredible(*this);

}

//	this is provided for completeness,
//	although it is no different than the copy constructor
SortableObject::SortableObject(SortableObject &&other) {

	if (this == &other)
		return;

	list = other.list;
	size_of_list = other.size_of_list;
	size_of_element= other.size_of_element;
	compare = other.compare;
	credible = isSortableObjectCredible(*this);
}

SortableObject& SortableObject::operator=(SortableObject &other)
{
	if (this == &other)
		return *this;

	list = other.list;
	size_of_list = other.size_of_list;
	size_of_element = other.size_of_element;
	compare = other.compare;
	credible = isSortableObjectCredible(*this);

	return *this;
}

//	this is provided for completeness,
//	although it is not different than the copy assignment operator
SortableObject& SortableObject::operator=(SortableObject &&other) {

	if (this == &other)
		return *this;

	list = other.list;
	size_of_list = other.size_of_list;
	size_of_element = other.size_of_element;
	compare = other.compare;
	credible = isSortableObjectCredible(*this);
	return *this;
}


std::ostream& operator<<(std::ostream &out, const SortableObject &object) {

	out << (object.credible ? "Credible " : "Suspect ");
	out << "list " << object.list
		<< " has " << std::setw(8) << object.size_of_list << " elements"
		<< " that are each " << object.size_of_element << " bytes"
		<< " with compare() " << object.compare;
	return out;
}
