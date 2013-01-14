/*
 * RectButton.cpp
 *
 *  Created on: 03.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "RectButton.h"

GLshort RectButton::indices[] = {0, 1, 2, 2, 3, 0};
GLfloat RectButton::texCoordsDefault[] =
{
	0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
	1.0, 1.0, 0.0, 1.0, 0.0, 0.0
};


void RectButton::initGraphics(GLuint _textureId, GLfloat* _texCoords, GLfloat* _texCoordsPressed, GLuint _vertexHandle, GLuint _textureHandle){
	vertexHandle = _vertexHandle;
	textureHandle = _textureHandle;
	textureId = _textureId;
	texCoords = _texCoords != NULL ? _texCoords : texCoordsDefault;
	texCoordsPressed = _texCoordsPressed != NULL ? _texCoordsPressed : texCoords;
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

void RectButton::render(double elapsedTime){
	//LOGI("Presssed: %d", pressed);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(vertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, pressed ? texCoordsPressed : texCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(textureHandle);
	checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(textureHandle);
	glDisableVertexAttribArray(vertexHandle);
}

bool RectButton::action(Action act, float _x, float _y){
	bool result = _x > x && _y > y && _x < rightDownX && _y < rightDownY;
	if(result){
		lastAction = act;

		switch(act){
			case ACTION_DOWN:
				pressed = true;
			break;

			case ACTION_UP:
				pressed = false;
			break;
		}

	}else{
		pressed = false;
		lastAction = ACTION_NONE;
	}
	return result;
}

EngineEvent RectButton::getEvent() const{
	int i = actions.indexOf(lastAction);
	return i < 0 ? EVENT_NONE : events[i];
}

void RectButton::setActionEvent(Action on, EngineEvent what){

	int i = actions.indexOf(on);

	if(i >= 0){
		events[i] = what;
	}else{
		actions.pushHead(on);
		events.pushHead(what);
	}
}

RectButton::~RectButton() {
	actions.clear();
	events.clear();
}

