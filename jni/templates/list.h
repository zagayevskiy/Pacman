/*
 * list.h
 *
 *  Created on: 04.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

template<class T>
class List{

private:

	struct Node{
		T data;
		struct Node* next;
		struct Node* prev;
	};

	Node* head;
	Node* tail;
	Node* cursor;
	int length;

public:
	List():head(NULL), tail(NULL), cursor(NULL), length(0){}

	List(const List& other){
		operator=(other);
	}

	virtual ~List(){
		clear();
	}

	int getLength() const{
		return length;
	}

	bool isEmpty() const{
		return length == 0;
	}

	int indexOf(const T& what) const{
		Node* temp = head;
		int index = 0;
		while(temp){
			if(temp->data == what){
				return index;
			}
			temp = temp->next;
			++index;
		}
		return -1;
	}

	void pushHead(const T& data){
		++length;
		if(!head){
			head = new Node();
			head->data = data;
			head->prev = NULL;
			head->next = NULL;
			tail = head;
		}else{
			Node* temp = new Node();
			temp->data = data;
			temp->next = head;
			head->prev = temp;
			head = temp;
		}
	}

	void pushTail(const T& data){
		++length;
		if(!tail){
			head = new Node();
			head->data = data;
			head->prev = NULL;
			head->next = NULL;
			tail = head;
		}else{
			Node* temp = new Node();
			temp->data = data;
			temp->prev = tail;
			tail->next = temp;
			tail = temp;
		}
	}

	bool getHead(T& dest){
		if(head){
			dest = head->data;
			cursor = head;
			return true;
		}
		return false;
	}

	bool getTail(T& dest){
		if(tail){
			dest = tail->data;
			cursor = tail;
			return true;
		}
		return false;
	}

	bool getNext(T& dest){
		if(cursor){
			cursor = cursor->next;
			if(cursor){
				dest = cursor->data;
				return true;
			}
		}
		return false;
	}

	bool getPrev(T& dest){
		if(cursor){
			cursor = cursor->prev;
			if(cursor){
				dest = cursor->data;
				return true;
			}
		}
		return false;
	}

	void clear(){
		cursor = head;
		Node* temp = cursor;
		while(cursor){
			cursor = cursor->next;
			delete temp;
			temp = cursor;
		}
		length = 0;
		head = tail = cursor = NULL;
	}

	bool removeAt(int index){

		if(index >= 0 && index < length){
			cursor = head;
			for(int i = 0; i < index; ++i){
				cursor = cursor->next;
			}

			if(cursor->prev){
				cursor->prev->next = cursor->next;
			}else{
				head = cursor->next;
			}
			if(cursor->next){
				cursor->next->prev = cursor->prev;
			}else{
				tail = cursor->prev;
			}

			--length;

			delete cursor;
			cursor = NULL;

			return true;
		}

		return false;
	}

	int removeItem(const T& item){
		int result = 0;
		cursor = head;
		while(cursor){
			if(cursor->data == item){
				Node* temp;

				if(cursor->prev){
					cursor->prev->next = cursor->next;
					temp = cursor->prev;
				}else{
					head = cursor->next;
					temp = head;
				}
				if(cursor->next){
					cursor->next->prev = cursor->prev;
				}else{
					tail = cursor->prev;
				}

				delete cursor;
				cursor = temp;
				--length;
				++result;
			}
			cursor = cursor->next;
		}

		return result;
	}

	T& operator[](int k) const{

		Node* temp = head;
		for(int i = 0; i < k; ++i){
			temp = temp->next;
		}

		return temp->data;

	}

	List<T>& operator=(const List<T>& other){
		if(this == &other){
			return (*this);
		}
		clear();
		Node* temp = other.head;
		while(temp){
			pushTail(temp->data);
			temp = temp->next;
		}

		return (*this);
	}

};


#endif /* LIST_H_ */
