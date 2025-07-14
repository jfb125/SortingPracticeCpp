/*
 * stringUtilites.h
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

/*	**********************************************	*/
/*	a homegrown singly linked list of std::string	*/
/*	**********************************************	*/

class MessageListNode;

class MessageList {
private:
	MessageListNode *head;	// where value gets removed
	MessageListNode *tail;	// where value gets added
	int num_messages;
public:
	// return the ith messages without removing
	// if i is out of bounds, "" is returned
	std::string peek(int i) const;
	// return the messages that will be returned
	//	at the next dequeue() operation
	std::string peek() const;
	std::string dequeue();
	MessageList& enqueue(const std::string&);
	MessageList& enqueue(char*);
	MessageList& operator+=(const std::string&);
	MessageList& operator+=(char*);
	void clear();
	int size() const;
	bool isEmpty() const;

	MessageList();
	MessageList(const MessageList &);
	MessageList& operator=(const MessageList&);
	MessageList(MessageList &&) noexcept;
	MessageList& operator=(MessageList&&) noexcept;
	~MessageList();

	friend std::ostream& operator<<(std::ostream&, const MessageList&);
};


bool testMessageList();

/*
 *	these functions make attempts to sanitize the sources by not copying nullptrs
 */

// does not copy and nullptrs in src
int	copySanitizeMessageArray(std::string ***dst, std::string **src, int src_cnt);

// does not copy and nullptrs in src
int	moveSanitizeMessageArray(std::string ***dst, std::string ***src, int *src_cnt);

//	examine all pointers until src_cnt and count only non-nullptrs
int countUntilNullptr(std::string **src, int src_cnt);
//	return an integer representing the index of the first nullptr,
//	  or return src_cnt if no nullptrs are found
int countNonNullptr(std::string **src, int src_cnt);

void deleteMessageArray(std::string ***src, int *src_cnt);

#endif /* MESSAGELIST_H_ */
