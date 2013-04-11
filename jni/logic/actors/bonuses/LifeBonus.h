/*
 * LifeBonus.h
 *
 *  Created on: 09.01.2013
 *      Author: Denis
 */

#ifndef LIFEBONUS_H_
#define LIFEBONUS_H_

#include "Bonus.h"
#include "graphics/animation/Pulsation.h"
#include "managers/Audio.h"
#include "log.h"

class LifeBonus: public Bonus {
public:
	LifeBonus(Game* _game, float _x, float _y, GLuint program);
	virtual ~LifeBonus();

	bool apply(Pacman* pacman);

	void render(double elapsedTime);

private:
	Game* game;
	Pulsation* pulse;
	//float renderX, renderY;
};

#endif /* LIFEBONUS_H_ */
