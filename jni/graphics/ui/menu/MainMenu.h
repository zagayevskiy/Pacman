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
	MainMenu(GLuint vHandle, GLuint tHandle){
		RectButton* button = new RectButton(0.1, 0.1, 0.3, 0.3);
		button->initGraphics(Art::getTexture(Art::TEXTURE_PLAY_BUTTON), NULL, NULL, vHandle, tHandle);
		//, , vHandle, tHandle
		button->setActionEvent(ACTION_UP, EVENT_PLAY);
		controls.pushHead(button);
	}
	virtual ~MainMenu();
};

#endif /* MAINMENU_H_ */
