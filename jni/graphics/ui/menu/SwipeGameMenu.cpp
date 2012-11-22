/*
 * SwipeGameMenu.cpp
 *
 *  Created on: 22.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "SwipeGameMenu.h"

SwipeGameMenu::SwipeGameMenu() {
	lastX = lastY = -1.0f;
	event = EVENT_NONE;
}

bool SwipeGameMenu::action(Action act, float x, float y){

	float dx, dy, absDx, absDy;

	switch(act){
		case ACTION_DOWN:
			lastX = x;
			lastY = y;
			event = EVENT_NONE;
		break;

		case ACTION_MOVE:
			dx = x - lastX;
			dy = y - lastY;
			absDx = fabs(dx);
			absDy = fabs(dy);

			if(absDx < 0.05 && absDy < 0.05){
				break;
			}

			if(absDx > absDy){
				if(dx > 0.0f){
					event = EVENT_MOVE_RIGHT;
				}else{
					event = EVENT_MOVE_LEFT;
				}
			}else{
				if(dy > 0.0f){
					event = EVENT_MOVE_DOWN;
				}else{
					event = EVENT_MOVE_UP;
				}
			}

			lastX = x;
			lastY = y;
			return true;
		break;

		case ACTION_UP:
			event = EVENT_NONE;
		break;

		default: break;
	}

	return false;
}

SwipeGameMenu::~SwipeGameMenu() {
}

