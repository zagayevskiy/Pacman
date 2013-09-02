/*
 * Monster.h
 *
 *  Created on: 14.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include "logic/actors/Actor.h"
#include "logic/Game.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/Plume.h"
#include "log.h"

class Monster: public Actor {
public:

	void step(double elapsedTime);
	void render(double elapsedTime);

	virtual void initGraphics(GLuint shiftProgram);
	void save(){

	};
	void load(){

	};

	virtual ~Monster();

protected:

	enum MonsterState{
		MONSTER_GO_LEFT = 1,
		MONSTER_GO_RIGHT = 2,
		MONSTER_GO_UP = 3,
		MONSTER_GO_DOWN = 4
	};

	MonsterState state;

	Game* game;
	EngineEvent lastEvent;
	double remainingTime;
	double maxRemainingTime;
	float totalPathLength;
	float totalStepsCount;
	float averageStepLength;

	void switchDirection(bool verticalDirectionNow);
	virtual void newDirectionEvent() = 0;

	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;

	//Animation* animation;
	Plume* plume;

};

#endif /* MONSTER_H_ */
