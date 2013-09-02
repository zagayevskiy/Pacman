/*
 * Counter.h
 *
 *  Created on: Sep 2, 2013
 *      Author: Denis Zagayevskiy
 *
 *      This template class count instances of other classes.
 *
 *      DO NOT USE IN MULTITHREADED ENVIRONMENT! NOT THREAD-SAFE
 *
 *      Usage:
 *
 *      class MyClass : public Counter<MyClass>{
 *      	...
 *      	void foo(){
 *      		cout << " Count: " << getCount()
 *      			 << " My index: " << getIndex() << endl;
 *      	}
 *      	...
 *      }
 *
 *      OR
 *
 *      class MyOtherClass{
 *      private:
 *      	Counter<MyOtherClass> counter;
 *      	...
 *      	void foo(){
 *      		cout << " Count: " << counter.getCount()
 *      			 << " My index: " << counter.getIndex() << endl;
 *      	}
 *      	...
 *      }
 *
 */

#ifndef COUNTER_H_
#define COUNTER_H_

template<class T>
class Counter{
private:
	static int count;
	int index;
public:
	static inline int getCount(){
		return count;
	}

	Counter<T>(){
		index = count;
		++count;
	};

	virtual ~Counter<T>(){
		--count;
	}

	inline int getIndex() const{
		return index;
	}

	inline bool isFirst() const{
		return index == 0;
	}

	inline bool isLast() const{
		return count == 1;
	}
};

template<class T>
int Counter<T>::count = 0;


#endif /* COUNTER_H_ */
