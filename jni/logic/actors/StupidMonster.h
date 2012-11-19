/*
 * StupidMonster.h
 *
 *  Created on: 19.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef STUPIDMONSTER_H_
#define STUPIDMONSTER_H_

#include "logic/Game.h"
#include "Actor.h"
#include "actions_events.h"

class StupidMonster: public Actor {
public:
	StupidMonster(Game* _game, float _x, float _y, GLuint _shiftProgram): game(_game){
		radius = 0.5f;
		x = _x + 0.5f;
		y = _y + 0.5f;
		speed = 0.004;
		speedX = speed;
		speedY = 0;
		LOGI("StupedMonster::StupedMonster(%f, %f, Game)", x, y);
		lastEvent = EVENT_NONE;
		state = SM_GO_DOWN;
		totalPathLength = 0.0;
		totalStepsCount = 0.0;
		averageStepLength = 0.0;
		remainingTime = -1.0;
		initGraphics(_shiftProgram);
	}


	void step(double elapsedTime);
	void initGraphics(GLuint shiftProgram);
	void render(double elapsedTime);

	virtual ~StupidMonster();

private:
	void randomDirectionEvent();
	void switchDirection(bool verticalDirectionNow);

private:

	enum StupedMonsterState{
		SM_GO_LEFT = 1,
		SM_GO_RIGHT = 2,
		SM_GO_UP = 3,
		SM_GO_DOWN = 4
	};

	StupedMonsterState state;
	EngineEvent lastEvent;
	Game* game;
	double remainingTime;
	float totalPathLength;
	float totalStepsCount;
	float averageStepLength;


	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;
};

#endif /* STUPIDMONSTER_H_ */
