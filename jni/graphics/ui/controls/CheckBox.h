/*
 * CheckBox.h
 *
 *  Created on: 15.01.2013
 *      Author: Denis
 */

#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include <stdlib.h>

#include "Control.h"

class CheckBox: public Control {
public:
	CheckBox(float _x, float _y, float _width, float _height, bool _value=true)
	: x(_x), y(_y), width(_width), height(_height), rightDownX(x + width), rightDownY(y + height),
	  value(_value), eventOn(EVENT_NONE), eventOff(EVENT_NONE){
		lastAction = ACTION_NONE;

		initGraphics(0, NULL, NULL, 0, 0);
	}


	void initGraphics(GLuint _textureId, GLfloat* _texCoordsOn, GLfloat* _texCoordsOff, GLuint _vertexHandle, GLuint _textureHandle);
	void render(double elapsedTime);

	bool action(Action act, float _x, float _y);

	inline void setEvents(EngineEvent _on, EngineEvent _off){ eventOn = _on; eventOff = _off;};

	EngineEvent getEvent() const;

	virtual ~CheckBox();

private:
	float x, y; //left-up corner of the rectangle
	float width, height;
	float rightDownX, rightDownY;

	bool value;
	EngineEvent eventOn, eventOff;

	static GLshort indices[];
	static GLfloat texCoordsDefault[];
	GLfloat vertices[12];
	GLuint textureId;
	GLuint vertexHandle, textureHandle;
	GLfloat* texCoordsOn;
	GLfloat* texCoordsOff;
};

#endif /* CHECKBOX_H_ */
