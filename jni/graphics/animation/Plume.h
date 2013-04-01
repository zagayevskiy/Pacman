/*
 * Plume.h
 *
 *  Created on: 31.03.2013
 *      Author: Denis
 */

#ifndef PLUME_H_
#define PLUME_H_

class Plume {
public:
	Plume();

	void pushPoint(float x, float y);

	virtual ~Plume();

private:

	struct Point{
		float x;
		float y;

		Point(float _x = 0.0f, float _y = 0.0f): x(_x), y(_y){}
	};

	Point* points;
	int cursor;
	int length;

};

#endif /* PLUME_H_ */
