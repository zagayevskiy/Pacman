/*
 * Plume.cpp
 *
 *  Created on: 31.03.2013
 *      Author: Denis
 */

#include "Plume.h"

Plume::Plume(float _size, GLuint _texture, double _frameDuration, GLfloat _alpha):
	maxLength(16), cursor(0), length(0), size(_size), frameElapsedTime(0.0), frameDuration(_frameDuration), texture(_texture),
	fi(0.0f), fiDelta(FI_DELTA), alpha(_alpha)
{
	points = new Point[maxLength];
	initGraphics();

}

void Plume::pushPoint(GLfloat x, GLfloat y){
	if(frameElapsedTime < frameDuration){
		return;
	}
	frameElapsedTime = frameElapsedTime - frameDuration;
	points[cursor] = Point(x, y);
	cursor = (cursor + 1) % maxLength;
	if(length < maxLength){
		++length;
	}
}

void Plume::initGraphics(){
	program = Art::getShaderProgram(Art::SHADER_PROGRAM_MASK_OVERLAY);
	vertexHandle = glGetAttribLocation(program, "aPosition");
	textureHandle = glGetAttribLocation(program, "aTexture");
	maskCoordsHandle = glGetAttribLocation(program, "aMaskCoords");
	shiftHandle = glGetUniformLocation(program, "uShift");
	texShiftHandle = glGetUniformLocation(program, "uTexShift");
	alphaHandle = glGetUniformLocation(program, "uAlpha");

	glUseProgram(program);
	GLint matrixHandle = glGetUniformLocation(program, "uMatrix");
	glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, Art::getMVPMatrix());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_BRUSHES));
	glActiveTexture(GL_TEXTURE0);

	GLint mapHandle = glGetUniformLocation(program, "uMap");
	glUniform1i(mapHandle, 0);

	GLint maskHandle = glGetUniformLocation(program, "uMask");
	glUniform1i(maskHandle, 1);

	//Hardcoded, brushs count = 4x4
	maskTexCoords = new GLfloat*[maxLength];
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			maskTexCoords[i*4 + j] = new GLfloat[12];
			GLfloat temp[12] = {
				j / 4.0, i / 4.0, (j + 1.0) / 4.0, i / 4.0, (j + 1.0) / 4.0, (i + 1.0) / 4.0,
				(j + 1.0) / 4.0, (i + 1.0) / 4.0, j / 4.0, (i + 1.0) / 4.0, j / 4.0, i / 4.0
			};
			memcpy(maskTexCoords[i*4 + j], temp, 12*sizeof(GLfloat));
		}
	}

	GLfloat vertices[12] = {
		0.0f, 0.0f, size, 0.0f, size, size,
		size, size, 0.0f, size, 0.0f, 0.0f
	};

	trianglesVertices = new GLfloat[12];
	memcpy(trianglesVertices, vertices, 12*sizeof(GLfloat));

}

void Plume::render(double elapsedTime){
	frameElapsedTime += elapsedTime;
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1f(alphaHandle, alpha);

	if(fi > MAX_FI_ABS){
		fiDelta = -FI_DELTA;
	}else if(fi < -MAX_FI_ABS){
		fiDelta = FI_DELTA;
	}

	fi += fiDelta;
	float r = 0.1*fi;
	glUniform2f(texShiftHandle, r*cos(fi), r*sin(fi));

	glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, trianglesVertices);
	glEnableVertexAttribArray(vertexHandle);

	glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, trianglesVertices);
	glEnableVertexAttribArray(textureHandle);

	glEnableVertexAttribArray(maskCoordsHandle);

	int number = 0;
	for(int i = cursor - 1; i > cursor - 1 - length; --i){

		glVertexAttribPointer(maskCoordsHandle, 2, GL_FLOAT, GL_FALSE, 0, maskTexCoords[number + maxLength - length]);
		++number;

		int k = i >= 0 ? i : maxLength + i;

		glUniform2f(shiftHandle, points[k].x, points[k].y);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisableVertexAttribArray(vertexHandle);
	glDisableVertexAttribArray(textureHandle);
	glDisableVertexAttribArray(maskCoordsHandle);
}

Plume::~Plume() {
	if(points){
		delete[] points;
	}
	if(trianglesVertices){
		delete[] trianglesVertices;
	}

	if(maskTexCoords){
		for(int i = 0; i < 16; ++i){
			if(maskTexCoords[i]){
				delete[] maskTexCoords[i];
			}
		}
		delete[] maskTexCoords;
	}
}

