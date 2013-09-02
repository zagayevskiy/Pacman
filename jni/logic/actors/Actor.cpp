/*
 * Actor.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Actor.h"
#include "managers/Store.h"
#include <stdio.h>

const char* Actor::ACTOR_X = "Actor%sX";
const char* Actor::ACTOR_Y = "Actor%sY";
const char* Actor::ACTOR_R = "Actor%sR";
const char* Actor::ACTOR_SPEED = "Actor%sS";
const char* Actor::ACTOR_SPEED_X = "Actor%sSX";
const char* Actor::ACTOR_SPEED_Y = "Actor%sSY";

bool Actor::intersect(const Actor* other) const{
	return sqrtf((x - other->x)*(x - other->x) + (y - other->y)*(y - other->y)) < radius + other->radius;
}

void Actor::saveForChild(const char* childId) const{
	LOGI("Actor::saveForChild(%s)", childId);
	char* name = new char[64];

	sprintf(name, ACTOR_X, childId);
	Store::saveFloat(name, x);

	sprintf(name, ACTOR_Y, childId);
	Store::saveFloat(name, y);

	sprintf(name, ACTOR_R, childId);
	Store::saveFloat(name, radius);

	sprintf(name, ACTOR_SPEED, childId);
	Store::saveFloat(name, speed);

	sprintf(name, ACTOR_SPEED_X, childId);
	Store::saveFloat(name, speedX);

	sprintf(name, ACTOR_SPEED_Y, childId);
	Store::saveFloat(name, speedY);

	delete[] name;
}

void Actor::loadForChild(const char* childId){
	LOGI("Actor::loadForChild(%s)", childId);
	char* name = new char[64];

	sprintf(name, ACTOR_X, childId);
	x = Store::loadFloat(name, x);

	sprintf(name, ACTOR_Y, childId);
	y = Store::loadFloat(name, y);

	sprintf(name, ACTOR_R, childId);
	radius =Store::loadFloat(name, radius);

	sprintf(name, ACTOR_SPEED, childId);
	speed = Store::loadFloat(name, speed);

	sprintf(name, ACTOR_SPEED_X, childId);
	speedX = Store::loadFloat(name, speedX);

	sprintf(name, ACTOR_SPEED_Y, childId);
	speedY = Store::loadFloat(name, speedY);

	delete[] name;
}

Actor::~Actor() {
	// TODO Auto-generated destructor stub
}

