/*
 * WinMenu.h
 *
 *  Created on: 26.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef WINMENU_H_
#define WINMENU_H_

#include "Menu.h"
#include "graphics/ui/controls/RectButton.h"
#include "graphics/ui/controls/Label.h"

class WinMenu: public Menu {
public:
	WinMenu();
	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle);
	virtual ~WinMenu();
};

#endif /* WINMENU_H_ */
