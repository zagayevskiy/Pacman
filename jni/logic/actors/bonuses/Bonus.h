/*
 * Bonus.h
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#ifndef BONUS_H_
#define BONUS_H_

#include "logic/actors/Pacman.h"

class Bonus: public Actor {
public:
	Bonus();
	virtual ~Bonus();

	/*
	 * @return true if this bonus should be removed from map
	 */
	virtual bool apply(Pacman* pacman) = 0;
	void step(double elapsedTime){};
	void save(){

	};
	void load(){

	};

};

#endif /* BONUS_H_ */
