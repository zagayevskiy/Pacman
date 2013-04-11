/*
 * Pulsation.cpp
 *
 *  Created on: 11.04.2013
 *      Author: Denis
 */

#include "Pulsation.h"


Pulsation::Pulsation(GLfloat _x, GLfloat _y, float _size, GLuint _texture, GLfloat _alpha):
	x(_x), y(_y), size(_size), texture(_texture), alpha(_alpha), cursor(0), fi(0.0f), fiDelta(FI_DELTA),
	stepTime(0.0), cursorDelta(1), duration(DEFAULT_DURATION)
{
	initGraphics();
}

void Pulsation::initGraphics(){
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

	maskTexCoords = new GLfloat*[BRUSHES_COUNT];
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

void Pulsation::render(double elapsedTime){

	stepTime += elapsedTime;
	if(stepTime > duration){
		stepTime = 0.0;
		cursor += cursorDelta;
		if(cursor >= BRUSHES_COUNT){
			cursor = BRUSHES_COUNT - 1;
			cursorDelta = -1;
		}else if(cursor <= 0){
			cursor = 0;
			cursorDelta = 1;
		}
	}

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

	glVertexAttribPointer(maskCoordsHandle, 2, GL_FLOAT, GL_FALSE, 0, maskTexCoords[cursor]);

	glUniform2f(shiftHandle, x, y);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(vertexHandle);
	glDisableVertexAttribArray(textureHandle);
	glDisableVertexAttribArray(maskCoordsHandle);
}

Pulsation::~Pulsation() {
	if(trianglesVertices){
		delete[] trianglesVertices;
	}
	if(maskTexCoords){
		for(int i = 0; i < BRUSHES_COUNT; ++i){
			if(maskTexCoords[i]){
				delete[] maskTexCoords[i];
			}
		}
		delete[] maskTexCoords;
	}
}

