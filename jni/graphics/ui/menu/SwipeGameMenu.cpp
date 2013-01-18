/*
 * SwipeGameMenu.cpp
 *
 *  Created on: 22.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "SwipeGameMenu.h"

const char* SwipeGameMenu::INFO_STRING = "x%d Score:%d";

SwipeGameMenu::SwipeGameMenu()
	: lastX(-1.0f), lastY(-1.0f), event(EVENT_NONE), pacman(NULL), lastPacmanLifesCount(0), lastPacmanScore(0), labelInfo(NULL){
}

bool SwipeGameMenu::action(Action act, float x, float y){

	float dx, dy, absDx, absDy;

	switch(act){
		case ACTION_DOWN:
			lastX = x;
			lastY = y;
			event = EVENT_NONE;
		break;

		case ACTION_MOVE:
			dx = x - lastX;
			dy = y - lastY;
			absDx = fabs(dx);
			absDy = fabs(dy);

			if(absDx < 0.05 && absDy < 0.05){
				break;
			}

			if(absDx > absDy){
				if(dx > 0.0f){
					event = EVENT_MOVE_RIGHT;
				}else{
					event = EVENT_MOVE_LEFT;
				}
			}else{
				if(dy > 0.0f){
					event = EVENT_MOVE_DOWN;
				}else{
					event = EVENT_MOVE_UP;
				}
			}

			lastX = x;
			lastY = y;
			return true;
		break;

		case ACTION_UP:
			event = EVENT_NONE;
		break;

		default: break;
	}

	return false;
}

void SwipeGameMenu::initGraphics(float maxX, float maxY, GLuint shiftProgram, GLuint vHandle, GLuint tHandle){
	float shiftY = maxY*0.05f;
	lifeImage = new Animation(shiftProgram, Art::getTexture(Art::TEXTURE_HEART), 4, 2, 2, 800.0f, shiftY, shiftY);
	char buffer[16];
	sprintf(buffer, INFO_STRING, 0, 0);
	labelInfo = new Label(shiftY, 0.0f, buffer, vHandle, tHandle, shiftY);
	controls.pushHead(labelInfo);
}

void SwipeGameMenu::assignPacman(Pacman* _pacman){
	pacman = _pacman;
	if(!pacman){
		return;
	}
}

void SwipeGameMenu::render(double elapsedTime){
	int score = pacman->getScore();
	int lifes = pacman->getLifes();
	if(lastPacmanScore != score || lastPacmanLifesCount != lifes){
		lastPacmanScore = score;
		lastPacmanLifesCount = lifes;
		char buffer[16];
		sprintf(buffer, INFO_STRING, lifes, score);
		labelInfo->setText(buffer);
	}
	lifeImage->render(elapsedTime, 0.0f, 0.0f);
	Menu::render(elapsedTime);
}

SwipeGameMenu::~SwipeGameMenu() {
}

