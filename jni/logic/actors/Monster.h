/*
 * Monster.h
 *
 *  Created on: 14.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "Actor.h"
#include "log.h"

class Monster: public Actor {
public:

	Monster(float _x, float _y){
		x = _x;
		y = _y;
	}

	void step(double elapsedTime){};
	void render(double elapsedTime){};

	virtual ~Monster();
};

#endif /* MONSTER_H_ */
