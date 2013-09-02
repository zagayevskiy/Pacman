/*
 * CleverMonster.h
 *
 *  Created on: 27.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef CLEVERMONSTER_H_
#define CLEVERMONSTER_H_

#include "logic/Game.h"
#include "Monster.h"
#include "actions_events.h"
#include "templates/Counter.h"

class CleverMonster: public Monster, protected Counter<CleverMonster> {
public:

	CleverMonster(Game* _game, float _x, float _y, GLuint _shiftProgram){
		game = _game;
		radius = 0.5f;
		x = _x + 0.5f;
		y = _y + 0.5f;
		speed = 0.002 + (rand() % 10)*0.00006;
		speedX = speed;
		speedY = 0;
		lastEvent = EVENT_NONE;
		state = MONSTER_GO_DOWN;
		totalPathLength = 0.0;
		totalStepsCount = 0.0;
		averageStepLength = 0.0;
		remainingTime = -1.0;
		maxRemainingTime = 700.0;

		if(isFirst()){
			initMaps();
		}

		initGraphics(_shiftProgram);
	}

	virtual ~CleverMonster();

private:

	static int mapsCountX;
	static int mapsCountY;
	static int mapSize;
	static int** maps;

	void initMaps();
	void freeMaps();
	int* getMap(int x, int y);
	int* buildMap(int targetX, int targetY);

protected:
	void newDirectionEvent();
};

#endif /* CLEVERMONSTER_H_ */
