/*
 * SwipeGameMenu.h
 *
 *  Created on: 22.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef SWIPEGAMEMENU_H_
#define SWIPEGAMEMENU_H_

#include "GameMenu.h"

class SwipeGameMenu: public GameMenu {
public:
	SwipeGameMenu();
	bool action(Action act, float x, float y);
	void initGraphics(float maxX, float maxY, GLuint vertexHandle, GLuint textureHandle){};
	EngineEvent getEvent() const {return event;};
	virtual ~SwipeGameMenu();

private:
	float lastX, lastY;
	EngineEvent event;
};

#endif /* SWIPEGAMEMENU_H_ */
