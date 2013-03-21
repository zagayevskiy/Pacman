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

#include "managers/Statistics.h"

class Pacman: public Actor {
public:

	static const float MAX_DIED_TIME = 300.0f;
	static const int DEFAULT_LIFES_COUNT = 3;

	Pacman(Game* _game, float _x, float _y, GLuint _shiftProgram): game(_game){
		radius = 0.5f;
		x = initialX = _x + 0.5f;
		y = initialY = _y + 0.5f;
		speed = 0.003;
		speedX = speed;
		speedY = 0;
		lastEvent = EVENT_NONE;
		state = initialState = PACMAN_GO_RIGHT;
		totalPathLength = 0.0f;
		totalStepsCount = 0.0f;
		averageStepLength = 0.0f;
		diedTime = 0.0f;
		lifes = DEFAULT_LIFES_COUNT;
		eatenFoodCount = score = 0;
		animationOffsets = NULL;
		initGraphics(_shiftProgram);
	}

	void initGraphics(GLuint shiftProgram);
	void freeGraphics();

	void event(EngineEvent);
	void step(double elapsedTime);
	void render(double elapsedTime);

	inline bool isDead() const {return state == PACMAN_DIED;};
	inline bool isGameOver() const {return state == PACMAN_GAME_OVER;};
	inline bool isWin() const {return state == PACMAN_WIN;};
	inline int getLifes() const {return lifes;};
	inline void incLifes(){
		++lifes;
		Statistics::incLifes();
	}

	int getScore() const {return score;};
	inline int getEatenFoodCount() const {return eatenFoodCount;};

	virtual ~Pacman();

private:
	void switchDirection(bool verticalDirectionNow);

private:

	enum PacmanState{
		PACMAN_GO_LEFT = 1,
		PACMAN_GO_RIGHT = 2,
		PACMAN_GO_UP = 3,
		PACMAN_GO_DOWN = 4,
		PACMAN_DIED = 10,
		PACMAN_GAME_OVER = 11,
		PACMAN_WIN = 12
	};

	Game* game;
	EngineEvent lastEvent;
	PacmanState state;
	PacmanState initialState;
	float totalPathLength;
	float totalStepsCount;
	float averageStepLength;
	float diedTime;
	int lifes;
	int score;
	int eatenFoodCount;

	float initialX, initialY;

	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;
	GLuint verticesBufferId, indicesBufferId, textureId;
	GLuint* animationOffsets;
	GLuint animationOffsetsLength;
	int animationStepNumber;
	double animationStepTime, animationElapsedTime;
	GLuint offsetGoLeft, offsetGoRight, offsetGoDown, offsetGoUp;

};

#endif /* PACMAN_H_ */
