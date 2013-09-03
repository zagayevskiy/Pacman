/*
 * StupidMonster.h
 *
 *  Created on: 19.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef STUPIDMONSTER_H_
#define STUPIDMONSTER_H_

#include "logic/Game.h"
#include "Monster.h"
#include "actions_events.h"
#include "templates/Counter.h"

class StupidMonster: public Monster, protected Counter<StupidMonster> {
public:
	StupidMonster(Game* _game, float _x, float _y, GLuint _shiftProgram) {
		game = _game;
		radius = 0.5f;
		x = _x + 0.5f;
		y = _y + 0.5f;
		speed = 0.003;
		speedX = speed;
		speedY = 0;
		lastEvent = EVENT_NONE;
		state = MONSTER_GO_DOWN;
		totalPathLength = 0.0;
		totalStepsCount = 0.0;
		averageStepLength = 0.0;
		remainingTime = -1.0;
		maxRemainingTime = 2000;
		initGraphics(_shiftProgram);
	}

	void save();
	void load();

	virtual ~StupidMonster();

protected:
	void newDirectionEvent();

};

#endif /* STUPIDMONSTER_H_ */
