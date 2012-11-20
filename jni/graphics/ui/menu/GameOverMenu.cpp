/*
 * GameOverMenu.cpp
 *
 *  Created on: 20.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "GameOverMenu.h"

GameOverMenu::GameOverMenu() {
}

void GameOverMenu::initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle){
	free();

	float buttonSize = maxX/3;
	RectButton* button = new RectButton(maxX/2 - buttonSize*1.05f, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_RETRY, NULL, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_RETRY);
	controls.pushTail(button);

	button = new RectButton(maxX/2 + buttonSize*0.05f, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_LIST, NULL, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MAINMENU);
	controls.pushTail(button);

}

GameOverMenu::~GameOverMenu() {
	// TODO Auto-generated destructor stub
}


