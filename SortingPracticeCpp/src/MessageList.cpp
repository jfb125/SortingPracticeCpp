/*
 * stringUtilities.cpp
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#include "MessageList.h"

using namespace std;


class MessageListNode {
public:
	std::string *string;
	MessageListNode *next;
	MessageListNode() : string(nullptr), next(nullptr) {}
	MessageListNode(const std::string &str) {
		string = new std::string(str);
		next = nullptr;

	}
	~MessageListNode() {
		if (string) {
			delete string;
			string = nullptr;
		}
		next = nullptr;
	}
};


/* 	**********************************************************	*/
/*			forward declaration of private helper functions		*/
/* 	**********************************************************	*/

int copyMessageList(MessageListNode **dst_head, MessageListNode **dst_tail, MessageListNode *src_head);
int deleteMessageList(MessageListNode **head, MessageListNode **tail);	// always returns 0


/*	**********************************************************	*/
/*						member functions						*/
/*	**********************************************************	*/

std::string MessageList::peek() const {
	return peek(0);
}

std::string MessageList::peek(int i) const {

	if (i >= num_messages) {
		std::stringstream retval;
		retval 	<< "ERROR: MessageList::peek(" << i << ") on list with "
				<< num_messages << " messages";
		return retval.str();
	}

	MessageListNode *seek = head;

	while (i--) {
		seek = seek->next;
	}

	return *seek->string;
}

std::string MessageList::dequeue() {

	if (head != nullptr) {
		std::string retval(*head->string);
		head = head->next;
		if (head == nullptr)
			tail = nullptr;
		num_messages--;
		return retval;
	} else {
		std::stringstream retval;
		retval << "-- ERROR -- MessageList::peek() on isEmpty() true";
		return retval.str();
	}
}

MessageList& MessageList::enqueue(const std::string &string_to_add) {

	// if the list is empty
	if (tail == nullptr) {
		head = new MessageListNode(string_to_add);
		tail = head;
		num_messages++;
		return *this;
	}

	tail->next = new MessageListNode(string_to_add);
	tail = tail->next;
	num_messages++;
	return *this;
}

MessageList& MessageList::enqueue(char* str) {

	return this->enqueue(std::string(str));
}

MessageList& MessageList::operator+=(const std::string& string_to_add) {

	return this->enqueue(string_to_add);
}

MessageList& MessageList::operator+=(char *str) {

	return this->enqueue(str);
}

void MessageList::clear() {
	num_messages = deleteMessageList(&head, &tail);
}

int MessageList::size() const {
	return num_messages;
}

bool MessageList::isEmpty() const {
	return num_messages == 0;
}

MessageList::MessageList() : head(nullptr), tail(nullptr), num_messages(0) {}

MessageList::MessageList(const MessageList &other) {

	num_messages = 0;
	head = nullptr;
	tail = nullptr;

	if (this == &other)
		return;

	if (other.head == nullptr)
		return;

	num_messages = copyMessageList(&head, &tail, other.head);
}

MessageList& MessageList::operator=(const MessageList &other) {

	if (this != &other) {
		deleteMessageList(&head, &tail);
		num_messages = copyMessageList(&head, &tail, other.head);
	}

	return *this;
}

MessageList::~MessageList() {

	num_messages = deleteMessageList(&head, &tail);
}

MessageList::MessageList(MessageList &&other) noexcept {

	if (this != &other) {
		num_messages = other.num_messages;
		head = other.head;
		tail = other.tail;
		other.head = nullptr;
		other.tail = nullptr;
		other.num_messages = 0;
	}
}

MessageList& MessageList::operator=(MessageList &&other) noexcept {

	if (this != &other) {
		deleteMessageList(&head, &tail);
		num_messages = other.num_messages;
		head = other.head;
		tail = other.tail;
		other.head = nullptr;
		other.tail = nullptr;
		other.num_messages = 0;
	}

	return *this;
}


/*	**************************************************************	*/
/*					non-member logging functiions					*/
/*	**************************************************************	*/

std::ostream& operator<<(std::ostream &out, const MessageList &object) {

	const char *head_string = " head: &";
	const char *tail_string = " tail: &";
	const char *node_string = "                      ";	// indent
	//	avoid std::left and std::right persisting after a call to printStudentList()

	std::ios_base::fmtflags _flags = std::cout.flags();

	out << "list at " << &object << " contains " << object.num_messages << " elements:" << endl;
	if (object.num_messages == 0) {
		out << head_string << tail_string << "   nullptr  ";
		std::cout.flags(_flags);
		return out;
	}

	MessageListNode *p = object.head;
	out << head_string << &object.head << " " << object.head << ": ->next ";
	if (object.head->next)
		out << object.head->next;
	else
		out << "    nullptr   ";
	out << " string: " << object.head->string << "  \"" << *object.head->string << "\"" << endl;

	// head & tail are printed separately, this 2 .. num_messages
	for (int i = 2; i < object.num_messages; i++) {
		p = p->next;
		out << node_string << " " << p << ": ->next "
			<< p->next
			<< " string: " << p->string << "  \"" << *p->string << "\"" << endl;
 	}

	out << tail_string << &object.tail << " " << object.tail << ": ->next ";
	if (object.tail->next)
		out << object.head->next;
	else
		out << "    nullptr   ";
	out	<< " string: " << object.tail->string << "  \"" << *object.tail->string << "\"" << endl;

	std::cout.flags(_flags);

	return out;
}

/* 	**********************************************************	*/
/*			declaration of private helper functions		*/
/* 	**********************************************************	*/

//	copies all of the nodes in the linked list pointed to by 'src_head'
//	  into the linked_list at 'dst_head',
//	  also updates 'dst_tail' to point to final node in copied list
//	  returns number of messages in *dst_head list

int copyMessageList(MessageListNode **dst_head, MessageListNode **dst_tail, MessageListNode *src_head) {

	int num_copied = 0;
	if (dst_head == nullptr)
		return num_copied;

	*dst_head = nullptr;
	*dst_tail = nullptr;

	if(src_head == nullptr)
		return num_copied;

	//	the copy src & dst will be from/to the ->next pointers
	//	head has no next pointer, so it is done separately
	(*dst_head) = new MessageListNode(*src_head->string);
	num_copied++;

	// improves readability by better showing intent
	MessageListNode *dst = *dst_head;
	MessageListNode *src = src_head;

	while (src->next) {
		dst->next = new MessageListNode(*src->next->string);
		num_copied++;
		dst = dst->next;
		src = src->next;
	}

	*dst_tail = dst;

	return num_copied;
}

int deleteMessageList(MessageListNode **list_head, MessageListNode **list_tail) {

	if (list_head == nullptr)
		return 0;

	if (*list_head == nullptr)
		return 0;

	// stores node->next while *node is being deleted
	MessageListNode *was_next;
	// make a copy of the head
	MessageListNode *node = *list_head;

	// detach the head (and tail) of moved list
	*list_head = nullptr;
	*list_tail = nullptr;

	while (node) {
		was_next = node->next;
		delete node;
		node = nullptr;
		node = was_next;
	}

	return 0;
}

//	do not assume that all of the strings in (*src) are non-null

int copySanitizeMessageArray(std::string ***dst, std::string **src, int num) {

	int num_copied = 0;

	if (dst == nullptr)
		return num_copied;

	*dst = nullptr;
	if (src == nullptr || num == 0)
		return num_copied;

	// only allocate enough space for the non-null pointers in the src
	int num_non_null = countNonNullptr(src, num);

	if (num_non_null == 0)
		return num_copied;

	*dst = new std::string*[num_non_null];

	// only copy the pointers that are not null from the src
	for (int src_i = 0, dst_i = 0; src_i != num; src_i++) {
		if (src[src_i] != nullptr) {
			(*dst)[dst_i] = new std::string(*(src[src_i]));
			num_copied++;
			dst_i++;
		}
	}

	return num_copied;
}

//	do not assume that all of the strings in (*src) are non-null

int moveSanitizeMessageArray(std::string ***dst, std::string ***src, int *num_messages) {

	int num_copied = 0;

	// if passed a bad dst, nothing can me done
	if (dst == nullptr)
		return num_copied;

	*dst = nullptr;

	// if passed a bad src, or src is not a valid array, leave
	if (src == nullptr || *src == nullptr || *num_messages == 0)
		return num_copied;


	// only allocate enough space for the non-null pointers in the src
	int num_non_null = countNonNullptr(*src, *num_messages);

	if (num_non_null == 0)
		return num_copied;

	*dst = new std::string*[num_non_null];

	// only copy the pointers that are not null from the src
	for (int src_i = 0, dst_i = 0; src_i != *num_messages; src_i++) {
		if ((*src)[src_i] != nullptr) {
			(*dst)[dst_i] = (*src)[src_i];
			(*src)[src_i] = nullptr;
			num_copied++;
			dst_i++;
		}
	}

	delete[] *src;
	*src = nullptr;
	*num_messages = 0;

	return num_copied;
}

int countNonNullptr(std::string **src, int src_cnt) {

	if (src == nullptr)
		return 0;

	int num_non_null = 0;

	for (int i = 0; i != src_cnt; i++) {
		if (src[i] != nullptr) {
			num_non_null++;
		}
	}

	return num_non_null;
}

int countUntilNullptr(std::string **src, int src_cnt) {

	if (src == nullptr)
		return 0;

	int first_non_null = 0;

	for (int i = 0; i != src_cnt; i++) {
		if (src[i] != nullptr) {
			first_non_null++;
		} else {
			break;
		}
	}

	return first_non_null;
}

void deleteMessageArray(std::string ***src, int *src_cnt) {

	//	if passed a nullptr
	if (src == nullptr)
		return;

	//	if the array to be deleted is nullptr
	if (*src == nullptr)
		return;

	for (int i = 0; i != *src_cnt; i++) {
		if ((*src)[i] != nullptr) {
			delete (*src)[i];
			(*src)[i] = nullptr;
		}
	}

	delete[] *src;
	*src = nullptr;
	*src_cnt = 0;
}

/*	**********************************************************	*/
/*						object test fixture						*/
/*	**********************************************************	*/

bool testMessageList() {

#define btwa(str, _list)	do { cout << "added \"" << str << "\", list is now: " << _list << endl;	} while(false)
#define btwr(str, _list)	do { cout << "removed \"" << str << "\", list is now: " << _list << endl;	} while(false)
	cout << "testMessageList()" << endl;
	bool passed = true;
#if 0
	MessageList list;
	cout << "created list: " << endl << list << endl;

	list += "Message One";
	btwa("Message One", list);

	list.enqueue("Message Two");
	btwa("Message Two", list);

	std::string msg_three("Message_Three");
	list += msg_three;
	btwa("Message Three", list);

	std::string msg_four("Message_Four");
	list.enqueue(msg_four);
	btwa("Message Three", list);

	cout << "peek() returns \"" << list.peek() << "\"" << endl;
	cout << "list is now " << list << endl;

	cout << "Testing [] operator:" << endl;
	for (int i = 0; i != list.size(); i++) {
		cout << " " << i << ": " << list[i] << endl;
	}

	cout << "Testing .pop() and .isEmpty:" << endl;

	int i = 0;
	while (!list.isEmpty()) {
		cout << " " << i++ << ": " << list.pop() << endl;
		cout << "list is now:" << endl << list << endl;
	}
	list += "Whoops one";
	list += "Whoops two";
	list += "Whoops three";
	list += "Whoops four";
	cout << "Added four strings to list: " << list << endl;

	MessageList copied_list(list);
	cout << "original list: " << list << endl;
	cout << "copied list:   " << copied_list << endl;

	list.clear();
	cout << "original list: " << list << endl;
	cout << "copied list:   " << copied_list << endl;

	list += "Whoops five";
	list += "whoops six";
	list += "Whoops seven";
	list += "Whoops eight";
	cout << "original list: " << list << endl;
	cout << "copied list:   " << copied_list << endl;

	cout << "copy assign copied_list = list" << endl;
	copied_list = list;
	cout << "original list: " << list << endl;
	cout << "copied list:   " << copied_list << endl;

	cout << "moving original list into moved_list" << endl;
	MessageList moved_list = std::move(list);
	cout << "original list: " << endl << list << endl;
	cout << "copied list:   " << endl << copied_list << endl;
	cout << "moved list:    " << endl << moved_list << endl;

	cout << "move assign moved_list into copied list" << endl;
	copied_list = std::move(moved_list);
	cout << "copied list:   " << endl << copied_list << endl;
	cout << "moved list:    " << endl << moved_list << endl;

	cout << "testMessageList() returns " << (passed ? "passed" : "failed") << endl;
#endif
	return passed;
}
