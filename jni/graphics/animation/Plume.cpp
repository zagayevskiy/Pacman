/*
 * Plume.cpp
 *
 *  Created on: 31.03.2013
 *      Author: Denis
 */

#include "Plume.h"

Plume::Plume() {
	length = 16;
	cursor = 0;
	points = new Point[length];
}

void Plume::pushPoint(float x, float y){
	points[cursor] = Point(x, y);
	++cursor;
}

Plume::~Plume() {
	if(points){
		delete[] points;
	}
}

