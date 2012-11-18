/*
 * StateMachine.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

class StateMachine {
public:
	virtual void step(double elapsedTime) = 0;
	virtual ~StateMachine();
};

#endif /* STATEMACHINE_H_ */
