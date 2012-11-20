/*
 * GameMenu.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "GameMenu.h"

void GameMenu::initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle){
	free();

	GLfloat indent = 0.02;
	GLfloat buttonSize = (maxY / 7.0 < maxX / 4.0) ? (maxY / 7.0) : (maxX / 4.0 - indent);

	GLuint texture = Art::getTexture(Art::TEXTURE_BUTTONS);
	RectButton* button = new RectButton(indent, maxY - buttonSize, buttonSize, buttonSize);
	button->initGraphics(texture, Art::TEX_COORDS_BUTTON_LEFT, Art::TEX_COORDS_BUTTON_LEFT_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MOVE_LEFT);
	controls.pushTail(button);

	button = new RectButton(buttonSize + 2*indent, maxY - buttonSize, buttonSize, buttonSize);
	button->initGraphics(texture, Art::TEX_COORDS_BUTTON_DOWN, Art::TEX_COORDS_BUTTON_DOWN_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MOVE_DOWN);
	controls.pushTail(button);

	button = new RectButton(buttonSize*2 + 3*indent, maxY - buttonSize, buttonSize, buttonSize);
	button->initGraphics(texture, Art::TEX_COORDS_BUTTON_UP, Art::TEX_COORDS_BUTTON_UP_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MOVE_UP);
	controls.pushTail(button);

	button = new RectButton(buttonSize*3 + 4*indent, maxY - buttonSize, buttonSize, buttonSize);
	button->initGraphics(texture, Art::TEX_COORDS_BUTTON_RIGHT, Art::TEX_COORDS_BUTTON_RIGHT_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MOVE_RIGHT);
	controls.pushTail(button);
}

GameMenu::~GameMenu() {
}

