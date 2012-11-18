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
		//radius = game->getTileSize() / 2.0;
		x = _x;
		y = _y;
		LOGI("Monster::Monster(%f, %f)", x, y);
	}

	void step(double elapsedTime){};
	void render(double elapsedTime){};

	virtual ~Monster();
};

#endif /* MONSTER_H_ */
