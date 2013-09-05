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
#include "templates/Counter.h"
#include "log.h"

class LifeBonus: public Bonus, public Counter<LifeBonus> {
public:
	LifeBonus(Game* _game, float _x, float _y, GLuint program);
	virtual ~LifeBonus();

	bool apply(Pacman* pacman);
	bool shouldBeRemovedAfterLoading();
	void onLevelStart();

	void render(double elapsedTime);

	void save();

private:
	Game* game;
	Pulsation* pulse;

	void save(bool exists);

};

#endif /* LIFEBONUS_H_ */
