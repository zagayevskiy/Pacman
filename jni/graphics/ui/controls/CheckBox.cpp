/*
 * CheckBox.cpp
 *
 *  Created on: 15.01.2013
 *      Author: Denis
 */

#include "CheckBox.h"

/*
 * CheckBox.cpp
 *
 *  Created on: 15.01.2013
 *      Author: Denis Zagayevskiy
 */

#include "CheckBox.h"

GLshort CheckBox::indices[] = {0, 1, 2, 2, 3, 0};
GLfloat CheckBox::texCoordsDefault[] =
{
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0, 0.0, 0.0
};


void CheckBox::initGraphics(GLuint _textureId, GLfloat* _texCoordsOn, GLfloat* _texCoordsOff, GLuint _vertexHandle, GLuint _textureHandle){
	vertexHandle = _vertexHandle;
	textureHandle = _textureHandle;
	textureId = _textureId;
	texCoordsOn = _texCoordsOn != NULL ? _texCoordsOn : texCoordsDefault;
	texCoordsOff = _texCoordsOff != NULL ? _texCoordsOff : texCoordsOn;
	vertices[0] = x;
	vertices[1] = y;

	vertices[2] = x + width;
	vertices[3] = y;

	vertices[4] = x + width;
	vertices[5] = y + height;

	vertices[6] = x + width;
	vertices[7] = y + height;

	vertices[8] = x;
	vertices[9] = y + height;
	vertices[10] = x;
	vertices[11] = y;
}

void CheckBox::render(double elapsedTime){

	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(vertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, value ? texCoordsOn : texCoordsOff);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(textureHandle);
	checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(textureHandle);
	glDisableVertexAttribArray(vertexHandle);
}

bool CheckBox::action(Action act, float _x, float _y){
	bool result = _x > x && _y > y && _x < rightDownX && _y < rightDownY;
	if(result){
		switch(act){
			case ACTION_DOWN:
			break;

			case ACTION_UP:
				value = !value;
			break;
		}

	}
	return result;
}

EngineEvent CheckBox::getEvent() const{
	return value ? eventOn : eventOff;
}

CheckBox::~CheckBox() {
}


