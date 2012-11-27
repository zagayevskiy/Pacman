/*
 * StupidMonster.cpp
 *
 *  Created on: 19.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "StupidMonster.h"

void StupidMonster::newDirectionEvent(){
	switch(rand() % 4){
		case 0: lastEvent = EVENT_MOVE_LEFT; break;
		case 1: lastEvent = EVENT_MOVE_UP; break;
		case 2: lastEvent = EVENT_MOVE_RIGHT; break;
		case 3: lastEvent = EVENT_MOVE_DOWN; break;
	}

}

StupidMonster::~StupidMonster() {
}

