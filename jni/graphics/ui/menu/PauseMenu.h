/*
 * PauseMenu.h
 *
 *  Created on: 22.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef PAUSEMENU_H_
#define PAUSEMENU_H_

#include "Menu.h"
#include "graphics/ui/controls/RectButton.h"

class PauseMenu: public Menu {
public:
	PauseMenu();
	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle);
	virtual ~PauseMenu();
};

#endif /* PAUSEMENU_H_ */
