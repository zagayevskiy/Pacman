/*
 * WinMenu.cpp
 *
 *  Created on: 26.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "WinMenu.h"

const char* WinMenu::HIGH_SCORE_FORMAT_STRING = "New high score: %d!";

WinMenu::WinMenu() :labelHighScore(NULL) {
}

void WinMenu::initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle){
	free();

	float indent = maxX / 30.0;
	float buttonSize = (maxX - indent*4.0)/3.0;
	RectButton* button;

	button = new RectButton(indent, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_NEXT, Art::TEX_COORDS_BUTTON_NEXT_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_NEXT_LEVEL);
	controls.pushTail(button);

	button = new RectButton(indent*2 + buttonSize, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_RETRY, Art::TEX_COORDS_BUTTON_RETRY_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_RETRY);
	controls.pushTail(button);

	button = new RectButton(indent*3 + buttonSize*2, maxY/2 - buttonSize / 2, buttonSize, buttonSize);
	button->initGraphics(Art::getTexture(Art::TEXTURE_BUTTONS), Art::TEX_COORDS_BUTTON_LIST, Art::TEX_COORDS_BUTTON_LIST_PRESSED, vertexHandle, textureHandle);
	button->setActionEvent(ACTION_UP, EVENT_MAINMENU);
	controls.pushTail(button);

	const char* excellent = "Excellent!";
	GLfloat excellentHeight = 0.09;
	float excellentX = (maxX  - Label::getWidthForHeight(excellentHeight)*strlen(excellent)) / 2.0;
	controls.pushTail(new Label(excellentX, 0.2, excellent, vertexHandle, textureHandle, excellentHeight));

	GLfloat highScoreHeight = 0.055;
	float highScoreX = (maxX - Label::getWidthForHeight(highScoreHeight)*strlen(HIGH_SCORE_FORMAT_STRING)) / 2.0;
	labelHighScore = new Label(highScoreX, 0.21 + excellentHeight, "", vertexHandle, textureHandle, highScoreHeight);
	controls.pushTail(labelHighScore);

}

void WinMenu::onShow(){
	char buffer[64];
	if(Statistics::isLevelPassedWithRecord()){
		sprintf(buffer, HIGH_SCORE_FORMAT_STRING, Statistics::getLevelRecord());
		labelHighScore->setText(buffer);
	}else{
		labelHighScore->setText("");
	}
}

WinMenu::~WinMenu() {
}

