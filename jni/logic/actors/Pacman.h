/*
 * Pacman.h
 *
 *  Created on: 13.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef PACMAN_H_
#define PACMAN_H_

#include "log.h"
#include "Actor.h"

#include "logic/Game.h"

class Pacman: public Actor {
public:
	Pacman(Game* _game, float _x, float _y, GLuint _shiftProgram): game(_game){
		radius = 0.5f;
		x = _x + 0.5f;
		y = _y + 0.5f;
		speed = 0.003;
		speedX = speed;
		speedY = 0;
		LOGI("Pacman::Pacman(%f, %f, Game)", x, y);
		lastEvent = EVENT_NONE;
		state = PACMAN_GO_RIGHT;
		totalPathLength = 0.0;
		totalStepsCount = 0.0;
		averageStepLength = 0.0;
		initGraphics(_shiftProgram);
	}

	void initGraphics(GLuint shiftProgram);

	void event(EngineEvent);
	void step(double elapsedTime);
	void render(double elapsedTime);

	virtual ~Pacman();

private:
	void switchDirection(bool verticalDirectionNow);

private:

	enum PacmanState{
		PACMAN_GO_LEFT = 1,
		PACMAN_GO_RIGHT = 2,
		PACMAN_GO_UP = 3,
		PACMAN_GO_DOWN = 4
	};

	Game*game;
	float speed;
	EngineEvent lastEvent;
	PacmanState state;
	float totalPathLength;
	float totalStepsCount;
	float averageStepLength;

	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;
};

#endif /* PACMAN_H_ */
