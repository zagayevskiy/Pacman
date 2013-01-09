/*
 * Animation.cpp
 *
 *  Created on: 08.01.2013
 *      Author: Denis
 */

#include "Animation.h"

Animation::Animation(GLuint program, GLuint _textureId, int _framesCount, int framesCountX, int framesCountY, double duration, GLfloat _sizeX, GLfloat _sizeY, bool randomize)
	: shiftProgram(program), textureId(_textureId), framesCount(_framesCount), sizeX(_sizeX), sizeY(_sizeY),
	  frameNumber(0), frameTime(duration / (double) framesCount), elapsedTime(0.0)
{
	shiftHandle = glGetUniformLocation(shiftProgram, "uShift");
	shiftVertexHandle = glGetAttribLocation(shiftProgram, "aPosition");
	shiftTextureHandle = glGetAttribLocation(shiftProgram, "aTexture");


	GLfloat tempAnimationCoords[COORDS_ARRAY_SIZE] = {
		0.0, 0.0, sizeX, 0.0, sizeX, sizeY,
		sizeX, sizeY, 0.0, sizeY, 0.0, 0.0
	};

	memcpy(animationCoords, tempAnimationCoords, COORDS_ARRAY_SIZE*sizeof(GLfloat));


	framesTexCoords = new GLfloat*[framesCount];

	GLfloat frameSizeX = 1.0 / (GLfloat)framesCountX;
	GLfloat frameSizeY = 1.0 / (GLfloat)framesCountY;

	// 0 ---- 1
	// |      |
	// |      |
	// 3 ---- 2
	// x0, y0, x1, y1, x2, y2,
	// x2, y2, x3, y3, x0, y0
	for(int i = 0, frNumber = 0; (i < framesCountY) && (frNumber < framesCount); ++i){
		for(int j = 0; (j < framesCountX) && (frNumber < framesCount); ++j, ++frNumber){
			framesTexCoords[frNumber] = new GLfloat[COORDS_ARRAY_SIZE];
			GLfloat tempFramesTexCoords[COORDS_ARRAY_SIZE] = {
				j*frameSizeX, i*frameSizeY, (j + 1)*frameSizeX, i*frameSizeY, (j + 1)*frameSizeX, (i + 1)*frameSizeY,
				(j + 1)*frameSizeX, (i + 1)*frameSizeY, j*frameSizeX, (i + 1)*frameSizeY, j*frameSizeX, i*frameSizeY
			};

			memcpy(framesTexCoords[frNumber], tempFramesTexCoords, COORDS_ARRAY_SIZE*sizeof(GLfloat));
		}
	}

	if(randomize){
		frameNumber = rand() % framesCount;
		elapsedTime = drand48() * duration;
	}

}

void Animation::render(double _elapsedTime, float x, float y){
	elapsedTime += _elapsedTime;
	if(elapsedTime > frameTime){
		elapsedTime = 0.0;
		frameNumber = (frameNumber + 1) % framesCount;
	}

	glUseProgram(shiftProgram);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glUniform2f(shiftHandle, x, y);

	glVertexAttribPointer(shiftVertexHandle, 2, GL_FLOAT, GL_FALSE, 0, animationCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftVertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(shiftTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, framesTexCoords[frameNumber]);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftTextureHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, COORDS_ARRAY_SIZE / 2);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(shiftTextureHandle);
	glDisableVertexAttribArray(shiftVertexHandle);

}

Animation::~Animation(){
	if(framesTexCoords){
		for(int i = 0; i < framesCount; ++i){
			if(framesTexCoords[i]){
				delete[] framesTexCoords[i];
			}
		}
		delete[] framesTexCoords;
	}
}
