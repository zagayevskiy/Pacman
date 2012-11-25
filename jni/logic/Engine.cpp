/*
 * Engine.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Engine.h"



GLfloat texCoords[] = {
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0, 0.0, 0.0
};

void Engine::init(int w, int h){
	levelToLoadNumber = 0;
	setState(STATE_LOADING);
	LOGI("Engine::init");
	setupGraphics(w, h);
	screenPixelWidth = w;
	screenPixelHeight = h;
	setState(STATE_AFTER_LOADING);
}

void Engine::step(double elapsedTime){

	switch(state){
		case STATE_LOADING: break;

		case STATE_AFTER_LOADING:
			currentMenu = mainMenu;
			setState(STATE_MAIN_MENU);
			LOGI("State: STATE_MAIN_MENU");
		break;

		case STATE_MAIN_MENU:
			if(lastEvent == EVENT_PLAY){
				lastEvent = EVENT_NONE;
				currentMenu = gameMenu;
				levelToLoadNumber = mainMenu->getLevelToLoadNumber();
				game->loadLevel(Art::getLevel(levelToLoadNumber));
				setState(STATE_PLAY);
				LOGI("State: STATE_PLAY");
			}
		break;

		case STATE_PLAY:
			switch(lastEvent){
				case EVENT_NONE: break;

				case EVENT_STOP:
					currentMenu = pauseMenu;
					setState(STATE_PAUSE);
					LOGI("State: STATE_PAUSE");
				break;

				default:
					game->event(lastEvent);
				break;
			}
			lastEvent = EVENT_NONE;
			game->step(elapsedTime);
			if(game->isGameOver()){
				currentMenu = gameOverMenu;
				setState(STATE_GAME_OVER);
				LOGI("State: STATE_GAME_OVER");
			}
		break;

		case STATE_GAME_OVER:
			switch(lastEvent){
				case EVENT_RETRY:
					lastEvent = EVENT_NONE;
					currentMenu = gameMenu;
					game->loadLevel(Art::getLevel(levelToLoadNumber));
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_MAINMENU:
					lastEvent = EVENT_NONE;
					currentMenu = mainMenu;
					setState(STATE_MAIN_MENU);
					LOGI("State: STATE_MAIN_MENU");
				break;

				default: break;
			}
		break;

		case STATE_PAUSE:
			switch(lastEvent){

				case EVENT_PLAY:
					lastEvent = EVENT_NONE;
					currentMenu = gameMenu;
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_RETRY:
					lastEvent = EVENT_NONE;
					currentMenu = gameMenu;
					game->loadLevel(Art::getLevel(levelToLoadNumber));
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_MAINMENU:
					lastEvent = EVENT_NONE;
					currentMenu = mainMenu;
					setState(STATE_MAIN_MENU);
					LOGI("State: STATE_MAIN_MENU");
				break;

				default: break;
			}
		break;

		default: LOGI("default state in engine");
	}

}

void Engine::setState(EngineState nextState){
	exitOnStop = nextState != STATE_PLAY;
	state = nextState;
}

void Engine::performAction(Action act, float x, float y){
	if(currentMenu->action(act, x / screenPixelWidth * maxX, y / screenPixelHeight * maxY)){
		lastEvent = currentMenu->getEvent();
	}
}

bool Engine::stop(){
	lastEvent = EVENT_STOP;
	return exitOnStop;
}

void Engine::render(double elapsedTime){

	glClearColor(0.0, 0.17, 0.0, 1.0f);
	checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	if(state == STATE_PLAY){
		game->render(elapsedTime);
	}else if(state == STATE_GAME_OVER || state == STATE_PAUSE){
		game->render(elapsedTime);
		glUseProgram(stableProgram);
		gameMenu->render(elapsedTime);
	}
	glUseProgram(stableProgram);
	currentMenu->render(elapsedTime);

}

Engine::~Engine() {
	if(matrix){
		delete[] matrix;
	}
	if(gameMenu){
		delete gameMenu;
	}
	if(mainMenu){
		delete mainMenu;
	}
	if(pauseMenu){
		delete pauseMenu;
	}
	if(gameOverMenu){
		delete gameOverMenu;
	}
	if(game){
		delete game;
	}
}

bool Engine::setupGraphics(int w, int h) {
    LOGI("Engine::setupGraphics(%d, %d)", w, h);

	float near = 1.0, far = -1.0;
	float left = 0.0, right = 1.0f / (float) h * (float) w, bottom = 1.0, top = 0.0;
	maxX = right;
	maxY = bottom;

	/*const mat4 OrthoProjection(float left, float right,
	        float bottom, float top, float zNear, float zFar)
	{
	        const float tx = - (right + left) / (right - left),
	                    ty = - (top + bottom) / (top - bottom),
	                    tz = - (zFar + zNear) / (zFar - zNear);

	        return mat4(2 / (right - left), 0, 					0, 						tx,
	                    0, 					2 / (top - bottom), 0, 						ty,
	                    0, 					0, 					-2 / (zFar - zNear), 	tz,
	                    0, 					0, 					0, 						1);*/

	matrix = new GLfloat[16];

	// First Column
	matrix[0] = 2.0 / (right - left);
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;

	// Second Column
	matrix[4] = 0.0;
	matrix[5] = 2.0 / (top - bottom);
	matrix[6] = 0.0;
	matrix[7] = 0.0;

	// Third Column
	matrix[8] = 0.0;
	matrix[9] = 0.0;
	matrix[10] = -2.0 / (far - near);
	matrix[11] = 0.0;

	// Fourth Column
	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(far + near) / (far - near);
	matrix[15] = 1;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stableProgram = ShadersManager::createProgram(Art::getShaderSource(Art::SHADER_VERTEX_0), Art::getShaderSource(Art::SHADER_FRAGMENT_0));
    if (!stableProgram) {
        LOGE("Could not create program.");
        return false;
    }

    vertexHandle = glGetAttribLocation(stableProgram, "aPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"aPosition\") = %d\n", vertexHandle);

    textureHandle = glGetAttribLocation(stableProgram, "aTexture");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"aTexture\") = %d\n", textureHandle);

	mapHandle = glGetUniformLocation(stableProgram, "uMap");
	checkGlError("glGetUniformLocation");
	LOGI("glGetAttribLocation(\"uMap\") = %d\n", mapHandle);

	matrixHandle = glGetUniformLocation(stableProgram, "uMatrix");
	checkGlError("glGetUniformLocation");
	LOGI("glGetAttribLocation(\"uMatrix\") = %d\n", matrixHandle);

	shiftProgram = ShadersManager::createProgram(Art::getShaderSource(Art::SHADER_VERTEX_SHIFT), Art::getShaderSource(Art::SHADER_FRAGMENT_0));
	if(!shiftProgram){
		LOGE("Could not create shift program");
		return false;
	}

	shiftMapHandle = glGetUniformLocation(shiftProgram, "uMap");
	checkGlError("glGetUniformLocation");

	shiftMatrixHandle = glGetUniformLocation(shiftProgram, "uMatrix");
	checkGlError("glGetUniformLocation");

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    glUseProgram(stableProgram);
	checkGlError("glUseProgram");

	// Sets the texture units to an uniform.
	glUniform1i(mapHandle, 0);
	checkGlError("glUniform1i");

	glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, matrix);
	checkGlError("glUniformMatrix4fv");

	glUseProgram(shiftProgram);

	glUniform1i(shiftMapHandle, 0);
	checkGlError("glUniform1i");

	glUniformMatrix4fv(shiftMatrixHandle, 1, GL_FALSE, matrix);
	checkGlError("glUniformMatrix4fv");

    Art::generateTextures();

    mainMenu = new MainMenu(maxX, maxY, vertexHandle, textureHandle);
    gameMenu = new SwipeGameMenu(); //new GameMenu();
    gameMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
    gameOverMenu = new GameOverMenu();
    gameOverMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
    pauseMenu = new PauseMenu();
    pauseMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
    game = new Game();
    game->initGraphics(maxX, maxY, stableProgram, shiftProgram);

    return true;
}
