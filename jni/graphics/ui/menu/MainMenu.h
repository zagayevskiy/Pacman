/*
 * MainMenu.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "Menu.h"
#include "graphics/ui/RectButton.h"

class MainMenu: public Menu {
public:
	MainMenu(float maxX, float maxY, GLuint vHandle, GLuint tHandle){
		float buttonSize = maxX / Art::LEVELS_ON_SIDE_COUNT;
		RectButton* button;

		/*button = new RectButton(0.0, 0.0, 0.5, 0.5);
		button->initGraphics(Art::getTexture(Art::TEXTURE_PLAY_BUTTON), NULL, NULL, vHandle, tHandle);
		button->setActionEvent(ACTION_UP, EVENT_PLAY);
		controls.pushHead(button);*/

		for(int i = 0; i < Art::LEVELS_ON_SIDE_COUNT; ++i){
			for(int j = 0; j < Art::LEVELS_ON_SIDE_COUNT; ++j){
				GLfloat* texCoords = Art::getLevelTexCoords(i*Art::LEVELS_ON_SIDE_COUNT + j);
				if(texCoords != NULL){
					button = new RectButton(j*buttonSize, i*buttonSize, buttonSize, buttonSize);
					button->initGraphics(Art::getTexture(Art::TEXTURE_ALL_LEVELS), texCoords, NULL, vHandle, tHandle);
					button->setActionEvent(ACTION_UP, EVENT_PLAY);
					controls.pushTail(button);
				}
			}
		}

	}

	int getLevelToLoadNumber() const {return lastActingControlNumber != LAST_CONTROL_NONE ? lastActingControlNumber : 0;};

	virtual ~MainMenu();
};

#endif /* MAINMENU_H_ */
