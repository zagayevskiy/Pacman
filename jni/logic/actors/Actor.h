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
/*
	void virtual step(double elapsedTime);

	void virtual render(double elapsedTime);
*/
	virtual void event(EngineEvent e){};
	virtual bool intersect(const Actor* other) const;
	virtual float getXCellCenterDistance() const {return fabs(x - floorf(x) - 0.5f);};
	virtual float getYCellCenterDistance() const {return fabs(y - floorf(y) - 0.5f);};
	virtual ~Actor();

protected:
	float x, y;
	float radius;
	float speed;
	float speedX, speedY;

	GLuint textureId;
	GLfloat* vertices;
	GLfloat* textureCoords;
};

#endif /* ACTOR_H_ */
