/*
 * RectButton.h
 *
 *  Created on: 03.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef RECTBUTTON_H_
#define RECTBUTTON_H_

#include "managers/Art.h"
#include "Control.h"
#include "log.h"

class RectButton: public Control {

public:

	float x, y; //left-up corner of the rectangle
 	float width, height;
 	float rightDownX, rightDownY;


	RectButton(float _x, float _y, float _width, float _height)
 			: x(_x), y(_y), width(_width), height(_height), rightDownX(x + width), rightDownY(y + height){

		LOGI("RectButton::RectButton(%f, %f, %f, %f", x, y, width, height);

		lastAction = ACTION_NONE;
		pressed = false;

		initGraphics(0, NULL, NULL, 0, 0);
	}


	void initGraphics(GLuint _textureId, GLfloat* _texCoords, GLfloat* _texCoordsPressed, GLuint _vertexHandle, GLuint _textureHandle);
	void render(double elapsedTime);

	bool action(Action act, float _x, float _y);

	EngineEvent getEvent() const;

	void setActionEvent(Action on, EngineEvent what);

	virtual ~RectButton();

private:

	bool pressed;
	List<Action> actions;
	List<EngineEvent> events;

	/*
	 0 -- 1
	 |	  |
	 3 -- 2
	 */

	static GLshort indices[];
	static GLfloat texCoordsDefault[];
	GLfloat vertices[12];
	GLuint textureId;
	GLuint vertexHandle, textureHandle;
	GLfloat* texCoords;
	GLfloat* texCoordsPressed;
};

#endif /* RECTBUTTON_H_ */
