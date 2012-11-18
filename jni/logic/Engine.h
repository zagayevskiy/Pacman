/*
 * Engine.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "log.h"
#include "actions_events.h"

#include "StateMachine.h"
#include "Game.h"
#include "graphics/ui/menu/MainMenu.h"
#include "graphics/ui/menu/GameMenu.h"

#include "managers/Art.h"



class Engine: public StateMachine, public IRenderable {
public:

	void init(int width, int height);
	void step(double elapsedTime);
	void render(double elapsedTime);

	void performAction(Action act, float x, float y);

	virtual ~Engine();

private:
	enum EngineState{
		STATE_LOADING = 0,
		STATE_AFTER_LOADING = 1,
		STATE_MAIN_MENU = 2,
		STATE_PLAY = 3,
		STATE_PAUSE = 4
	};

	EngineState state;
	EngineEvent lastEvent;

	bool setupGraphics(int width, int height);

	GLuint stableProgram, vertexHandle, textureHandle, matrixHandle, mapHandle;
	GLuint shiftProgram, shiftMapHandle, shiftMatrixHandle;
	GLfloat* matrix;

	float screenPixelWidth, screenPixelHeight;
	float maxX, maxY;

	Menu* currentMenu;
	MainMenu* mainMenu;
	GameMenu* gameMenu;

	Game* game;
};

#endif /* ENGINE_H_ */
