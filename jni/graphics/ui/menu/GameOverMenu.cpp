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
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_RETRY, Art::TEX_COORDS_BUTTON_RETRY_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_RETRY);
	controls.pushTail(button);

	button = new RectButton(maxX/2 + buttonSize*0.05f, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_LIST, Art::TEX_COORDS_BUTTON_LIST_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MAINMENU);
	controls.pushTail(button);


	const char* extinguished = "Extinguished!";
	GLfloat extinguishedHeight = 0.08;
	float extinguishedX = (maxX - Label::getWidthForHeight(extinguishedHeight)*strlen(extinguished)) / 2.0;
	controls.pushTail(new Label(extinguishedX, 0.2, extinguished, vertexHandle, textureHandle, extinguishedHeight));
}

GameOverMenu::~GameOverMenu() {
	// TODO Auto-generated destructor stub
}


