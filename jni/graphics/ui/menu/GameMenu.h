/*
 * GameMenu.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef GAMEMENU_H_
#define GAMEMENU_H_

#include "Menu.h"
#include "graphics/ui/RectButton.h"

class GameMenu: public Menu {
public:

	GameMenu();

	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle);

	virtual ~GameMenu();
};

#endif /* GAMEMENU_H_ */
