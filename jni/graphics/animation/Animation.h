/*
 * Animation.h
 *
 *  Created on: 08.01.2013
 *      Author: Denis
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdlib.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"

class Animation {

public:
	Animation(GLuint program, GLuint texId, int _framesCount, int _framesCountX, int _framesCountY, double duration, GLfloat _sizeX, GLfloat _sizeY, bool randomize = true);
	~Animation();

	void render(double _elapsedTime, float x, float y);

private:

	static const GLuint COORDS_ARRAY_SIZE = 12;

	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftHandle;
	GLuint textureId;

	int framesCount, frameNumber;

	GLfloat sizeX, sizeY;
	GLfloat **framesTexCoords;
	GLfloat animationCoords[COORDS_ARRAY_SIZE];
	double frameTime, elapsedTime;
};

#endif /* ANIMATION_H_ */
