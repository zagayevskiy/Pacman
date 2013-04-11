/*
 * Pulsation.h
 *
 *  Created on: 11.04.2013
 *      Author: Denis
 */

#ifndef PULSATION_H_
#define PULSATION_H_

#include "graphics/IRenderable.h"
#include "managers/Art.h"

class Pulsation: public IRenderable {
public:
	Pulsation(GLfloat x, GLfloat y, float size, GLuint texture, GLfloat alpha);

	void initGraphics();

	void render(double elapsedTime);

	virtual ~Pulsation();

private:

	static const float FI_DELTA = 0.001f;
	static const float MAX_FI_ABS = 1.0f;
	static const int BRUSHES_COUNT = 16;
	static const double DEFAULT_DURATION = 60.0;

	double stepTime;
	double duration;
	int cursorDelta;

	float x, y;
	float size;
	int cursor;
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

#endif /* PULSATION_H_ */
