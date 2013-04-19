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

#include "managers/Statistics.h"

class WinMenu: public Menu {
public:

	static const char* HIGH_SCORE_FORMAT_STRING;

	WinMenu();
	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle);
	void onShow();
	virtual ~WinMenu();

private:
	Label* labelHighScore;
};

#endif /* WINMENU_H_ */
