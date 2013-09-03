/*
 * StupidMonster.cpp
 *
 *  Created on: 19.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "StupidMonster.h"

#define STUPID_ID "Stupid%d"
#define NAME_STATE "Stupid_state%d"

void StupidMonster::save(){
	char* buffer = new char[16];

	sprintf(buffer, STUPID_ID, getIndex());
	saveForChild(buffer);

	sprintf(buffer, NAME_STATE, getIndex());
	Store::saveInt(buffer, state);

	delete[] buffer;
}

void StupidMonster::load(){
	char* buffer = new char[16];

	sprintf(buffer, STUPID_ID, getIndex());
	loadForChild(buffer);

	sprintf(buffer, NAME_STATE, getIndex());
	state = static_cast<MonsterState>(Store::loadInt(buffer, state));

	delete[] buffer;
}

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

