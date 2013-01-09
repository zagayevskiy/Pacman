/*
 * LifeBonus.h
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#ifndef LIFEBONUS_H_
#define LIFEBONUS_H_

#include "Bonus.h"
#include "graphics/animation/Animation.h"
#include "log.h"

class LifeBonus: public Bonus {
public:
	LifeBonus(Game* _game, float _x, float _y, GLuint program);
	virtual ~LifeBonus();

	bool apply(Pacman* pacman);

	void render(double elapsedTime);

private:
	Game* game;
	Animation* animation;
	float renderX, renderY;
};

#endif /* LIFEBONUS_H_ */
