/*
 * Engine.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Engine.h"

const char* Engine::NAME_STATE = "Engine_state";
const char* Engine::NAME_LEVEL_TO_LOAD_NUMBER = "Engine_level_to_load_number";
const char* Engine::NAME_SAVE_OK = "Engine_saved";


GLfloat texCoords[] = {
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0, 0.0, 0.0
};

void Engine::initGraphics(int w, int h){
	LOGI("Engine::initGraphics");

	setState(STATE_LOADING);
	setupGraphics(w, h);
	screenPixelWidth = w;
	screenPixelHeight = h;

}

void Engine::initLogic(){
	LOGI("Engine::initLogic");
	//LOGE("From Store String : [%s]", Store::loadString("SomeStringKey", "wtf"));

	levelToLoadNumber = 0;

	mainMenu = new MainMenu(maxX, maxY, vertexHandle, textureHandle);
	gameMenu = new SwipeGameMenu();
	gameMenu->initGraphics(maxX, maxY, shiftProgram, vertexHandle, textureHandle);
	gameOverMenu = new GameOverMenu();
	gameOverMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
	pauseMenu = new PauseMenu();
	pauseMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
	winMenu = new WinMenu();
	winMenu->initGraphics(maxX, maxY, vertexHandle, textureHandle);
	game = new Game();
	game->initGraphics(maxX, maxY, stableProgram, shiftProgram);

	if(Store::loadBool(NAME_SAVE_OK, false)){
		load();
	}else{
		setState(STATE_AFTER_LOADING);
	}

	//setState(STATE_AFTER_LOADING);
}

void Engine::save(){
	LOGI("Engine::save");
	Store::saveInt(NAME_LEVEL_TO_LOAD_NUMBER, levelToLoadNumber);

	switch(state){
		case STATE_PLAY:
			state = STATE_PAUSE;
			game->save();
		break;
		case STATE_PAUSE:
			game->save();
		break;

		default: break;
	}
	Store::saveInt(NAME_STATE, state);

	Store::saveBool(NAME_SAVE_OK, true);

}

void Engine::load(){
	LOGI("Engine::load");

	state = static_cast<EngineState>(Store::loadInt(NAME_STATE, STATE_AFTER_LOADING));
	LOGE("Loaded state: [%d]", state);
	levelToLoadNumber = Store::loadInt(NAME_LEVEL_TO_LOAD_NUMBER, 0);

	switch(state){
		case STATE_MAIN_MENU:
			currentMenu = mainMenu;
		break;

		case STATE_PLAY:
		case STATE_PAUSE:
			currentMenu = pauseMenu;
			game->load();
		break;

		case STATE_GAME_OVER:
			currentMenu = gameOverMenu;
			game->load();
		break;

		case STATE_WIN:
			currentMenu = winMenu;
			game->load();
		break;


		default: break;
	}

}

void Engine::step(double elapsedTime){
	Level* level = NULL;

	switch(state){
		case STATE_LOADING: break;

		case STATE_AFTER_LOADING:
			setMenu(mainMenu);
			setState(STATE_MAIN_MENU);
			LOGI("State: STATE_MAIN_MENU");
		break;

		case STATE_MAIN_MENU:
			if(lastEvent == EVENT_PLAY){
				lastEvent = EVENT_NONE;
				levelToLoadNumber = mainMenu->getLevelToLoadNumber();
				level = Art::getLevel(levelToLoadNumber);
				game->enterLevel(levelToLoadNumber);
				Statistics::enterLevel(level->name);
				setMenu(gameMenu);
				setState(STATE_PLAY);
				LOGI("State: STATE_PLAY");
			}
		break;

		case STATE_PLAY:
			switch(lastEvent){
				case EVENT_NONE: break;

				case EVENT_STOP:
					Statistics::pauseLevel();
					setMenu(pauseMenu);
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
				Statistics::leaveLevel();
				setMenu(gameOverMenu);
				setState(STATE_GAME_OVER);
				LOGI("State: STATE_GAME_OVER");
			}
			if(game->isWin()){
				Statistics::winLevel();
				setMenu(winMenu);
				setState(STATE_WIN);
				LOGI("State: STATE_WIN");
			}
		break;

		case STATE_GAME_OVER:
			switch(lastEvent){
				case EVENT_RETRY:
					lastEvent = EVENT_NONE;
					level = Art::getLevel(levelToLoadNumber);
					game->enterLevel(levelToLoadNumber);
					Statistics::enterLevel(level->name);
					setMenu(gameMenu);
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_MAINMENU:
					lastEvent = EVENT_NONE;
					setMenu(mainMenu);
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
					Statistics::resumeLevel();
					setMenu(gameMenu);
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_RETRY:
					lastEvent = EVENT_NONE;
					level = Art::getLevel(levelToLoadNumber);
					game->enterLevel(levelToLoadNumber);
					Statistics::leaveLevel();
					Statistics::enterLevel(level->name);
					setMenu(gameMenu);
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_MAINMENU:
					lastEvent = EVENT_NONE;
					Statistics::leaveLevel();
					setMenu(mainMenu);
					setState(STATE_MAIN_MENU);
					LOGI("State: STATE_MAIN_MENU");
				break;

				default: break;
			}
		break;

		case STATE_WIN:
			switch(lastEvent){

				case EVENT_NEXT_LEVEL:
					lastEvent = EVENT_NONE;
					levelToLoadNumber = (levelToLoadNumber + 1) % Art::getLevelsCount();
					level = Art::getLevel(levelToLoadNumber);
					game->enterLevel(levelToLoadNumber);
					Statistics::enterLevel(level->name);
					setMenu(gameMenu);
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_RETRY:
					lastEvent = EVENT_NONE;
					level = Art::getLevel(levelToLoadNumber);
					game->enterLevel(levelToLoadNumber);
					Statistics::enterLevel(level->name);
					setMenu(gameMenu);
					setState(STATE_PLAY);
					LOGI("State: STATE_PLAY");
				break;

				case EVENT_MAINMENU:
					lastEvent = EVENT_NONE;
					setMenu(mainMenu);
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
	if(nextState == STATE_MAIN_MENU){
		Audio::playMenuBackground();
	}else{
		if(nextState == STATE_PLAY){
			Audio::playGameBackground();
		}
	}
	LOGI("Engine::state = %d", nextState);
	exitOnStop = nextState != STATE_PLAY;
	state = nextState;
}

void Engine::performAction(Action act, float x, float y){
	if(currentMenu->action(act, x / screenPixelWidth * maxX, y / screenPixelHeight * maxY)){
		lastEvent = currentMenu->getEvent();
		switch (lastEvent) {
			case EVENT_MUSIC_ON:
				Audio::backgroundMusicOn();
				lastEvent = EVENT_NONE;
			break;

			case EVENT_MUSIC_OFF:
				Audio::backgroundMusicOff();
				lastEvent = EVENT_NONE;
			break;

			case EVENT_SOUNDS_ON:
				Audio::soundsOn();
				lastEvent = EVENT_NONE;
			break;

			case EVENT_SOUNDS_OFF:
				Audio::soundsOff();
				lastEvent = EVENT_NONE;
			break;

			default: break;
		}
	}
}

bool Engine::stop(){
	lastEvent = EVENT_STOP;
	return exitOnStop;
}

void Engine::render(double elapsedTime){
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClearColor");
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	if(state == STATE_PLAY){
		game->render(elapsedTime);
	}else if(state == STATE_GAME_OVER || state == STATE_PAUSE || state == STATE_WIN){
		game->render(elapsedTime);
		glUseProgram(stableProgram);
		gameMenu->render(elapsedTime);
	}
	glUseProgram(stableProgram);
	currentMenu->render(elapsedTime);
}

Engine::~Engine() {
	LOGI("Engine::~Engine");

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
	LOGI("Engine::~Engine finished");
}

bool Engine::setupGraphics(int w, int h) {
    LOGI("Engine::setupGraphics(%d, %d)", w, h);

    Art::initOpenGL();

	float right = 1.0f / (float) h * (float) w, bottom = 1.0;
	maxX = right;
	maxY = bottom;

	GLfloat* matrix = Art::getMVPMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stableProgram = Art::getShaderProgram(Art::SHADER_PROGRAM_0);
    if (stableProgram == Art::SHADER_PROGRAM_NONE) {
        LOGE("Art could not create program.");
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



	shiftProgram = Art::getShaderProgram(Art::SHADER_PROGRAM_SHIFT);
	if(shiftProgram == Art::SHADER_PROGRAM_NONE){
		LOGE("Art could not create shift program");
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

    glActiveTexture(GL_TEXTURE0);

    return true;
}
