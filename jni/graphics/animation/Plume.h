/*
 * Plume.h
 *
 *  Created on: 31.03.2013
 *      Author: Denis
 */

#ifndef PLUME_H_
#define PLUME_H_


#include "graphics/IRenderable.h"
#include "managers/Art.h"

class Plume: public IRenderable {
public:
	Plume(float size, GLuint texture, int stepDistance, GLfloat alpha);

	void pushPoint(GLfloat x, GLfloat y);
	void render(double elapsedTime);
	void initGraphics();

	virtual ~Plume();

private:

	static const GLfloat FI_DELTA = 0.001f;
	static const GLfloat MAX_FI_ABS = 1.0f;

	struct Point{
		float x;
		float y;

		Point(float _x = 0.0f, float _y = 0.0f): x(_x), y(_y){}
	};

	float size;
	Point* points;
	int cursor;
	int length, maxLength;
	int stepNumber;
	GLfloat fi;
	GLfloat fiDelta;
	GLuint texture;
	int stepDistance;
	GLfloat alpha;

	GLuint program;
	GLuint vertexHandle, textureHandle, shiftHandle, maskCoordsHandle, texShiftHandle, alphaHandle;
	GLfloat** maskTexCoords;

	GLfloat* trianglesVertices;

};

#endif /* PLUME_H_ */
