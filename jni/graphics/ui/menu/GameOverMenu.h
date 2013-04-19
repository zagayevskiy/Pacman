/*
 * GameOverMenu.h
 *
 *  Created on: 20.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef GAMEOVERMENU_H_
#define GAMEOVERMENU_H_

#include "Menu.h"
#include "graphics/ui/controls/RectButton.h"
#include "graphics/ui/controls/Label.h"

class GameOverMenu: public Menu {
public:

	GameOverMenu();
	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle);
	virtual ~GameOverMenu();

};

#endif /* GAMEOVERMENU_H_ */
