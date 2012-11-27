/*
 * CleverMonster.h
 *
 *  Created on: 27.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef CLEVERMONSTER_H_
#define CLEVERMONSTER_H_

#include "logic/Game.h"
#include "Actor.h"
#include "actions_events.h"

class CleverMonster: public Actor {
public:

	CleverMonster(Game* _game, float _x, float _y, GLuint _shiftProgram): game(_game){
		radius = 0.5f;
		x = _x + 0.5f;
		y = _y + 0.5f;
		speed = 0.0025;
		speedX = speed;
		speedY = 0;
		lastEvent = EVENT_NONE;
		state = CM_GO_DOWN;
		totalPathLength = 0.0;
		totalStepsCount = 0.0;
		averageStepLength = 0.0;
		remainingTime = -1.0;

		if(monstersCount == 0){
			initMaps();
		}
		++monstersCount;

		initGraphics(_shiftProgram);
	}

	void step(double elapsedTime);
	void initGraphics(GLuint shiftProgram);
	void render(double elapsedTime);

	virtual ~CleverMonster();

private:

	static int monstersCount;
	static int mapsCountX;
	static int mapsCountY;
	static int mapSize;
	static int** maps;

	void initMaps();
	void freeMaps();
	int* getMap(int x, int y);
	int* buildMap(int targetX, int targetY);

	enum CleverMonsterState{
		CM_GO_LEFT = 1,
		CM_GO_RIGHT = 2,
		CM_GO_UP = 3,
		CM_GO_DOWN = 4
	};

	CleverMonsterState state;

	Game* game;
	EngineEvent lastEvent;
	double remainingTime;
	float totalPathLength;
	float totalStepsCount;
	float averageStepLength;

	void switchDirection(bool verticalDirectionNow);
	void newDirectionEvent();



	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;
};

#endif /* CLEVERMONSTER_H_ */
