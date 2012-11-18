/*
 * Actor.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Actor.h"
/*
void Actor::step(double elapsedTime){
	x += speedX;
	y += speedY;
}

void Actor::render(double elapsedTime){
	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(vertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(textureHandle);
	checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(textureHandle);
	glDisableVertexAttribArray(vertexHandle);
}*/

bool Actor::intersect(const Actor* other) const{
	return sqrtf((x - other->x)*(x - other->x) + (y - other->y)*(y - other->y)) < radius + other->radius;
}

Actor::~Actor() {
	// TODO Auto-generated destructor stub
}

