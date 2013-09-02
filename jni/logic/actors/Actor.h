/*
 * Actor.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef ACTOR_H_
#define ACTOR_H_

#include <math.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "logic/StateMachine.h"
#include "graphics/IRenderable.h"
#include "actions_events.h"
#include "log.h"

class Actor: public StateMachine, public IRenderable {
public:
	virtual void event(EngineEvent e){};
	virtual bool intersect(const Actor* other) const;
	virtual float getXCellCenterDistance() const {return fabs(x - floorf(x) - 0.5f);};
	virtual float getYCellCenterDistance() const {return fabs(y - floorf(y) - 0.5f);};
	virtual float getX() const {return x;};
	virtual float getY() const {return y;};
	virtual ~Actor();

	virtual void saveForChild(const char* childId) const;
	virtual void save() = 0;
	virtual void loadForChild(const char* childId);
	virtual void load() = 0;

protected:
	float x, y;
	float radius;
	float speed;
	float speedX, speedY;

	GLuint textureId;
	GLfloat* vertices;
	GLfloat* textureCoords;

private:
	static const char* ACTOR_X;
	static const char* ACTOR_Y;
	static const char* ACTOR_R;
	static const char* ACTOR_SPEED;
	static const char* ACTOR_SPEED_X;
	static const char* ACTOR_SPEED_Y;

};

#endif /* ACTOR_H_ */
