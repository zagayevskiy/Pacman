/*
 * SwipeGameMenu.h
 *
 *  Created on: 22.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef SWIPEGAMEMENU_H_
#define SWIPEGAMEMENU_H_

#include "GameMenu.h"

#include "graphics/ui/controls/Label.h"
#include "graphics/animation/Animation.h"

#include "managers/Statistics.h"

class SwipeGameMenu: public GameMenu {
public:
	SwipeGameMenu();
	bool action(Action act, float x, float y);
	void initGraphics(float maxX, float maxY, GLuint shiftProgram, GLuint vertexHandle, GLuint textureHandle);
	inline EngineEvent getEvent() const {return event;};
	void render(double elapsedTime);
	virtual ~SwipeGameMenu();

private:

	static const char* INFO_STRING;

	float lastX, lastY;
	EngineEvent event;

	Animation* lifeImage;
	Label* labelInfo;
};

#endif /* SWIPEGAMEMENU_H_ */
