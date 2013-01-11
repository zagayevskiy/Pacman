/*
 * Label.h
 *
 *  Created on: 10.01.2013
 *      Author: Denis
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "stdlib.h"

#include "managers/Art.h"
#include "Control.h"

class Label: public Control {
public:
	Label(float _x, float _y, const char* _text, GLuint _vertexHandle, GLuint _textureHandle, GLfloat _fontHeight);
	virtual ~Label();

	virtual bool action(Action act, float x, float y){return false;};
	virtual EngineEvent getEvent() const {return EVENT_NONE;};

	void render(double elapsedTime);

	void setText(const char* _text);
	const char* getText() const {return text;};
	int getTextLength() const {return textLength;};
	void setFontHeight(GLfloat height);

private:

	float x, y;
	char* text;
	int textLength;

	GLfloat fontWidth;
	GLfloat fontHeight;

	GLuint vertexHandle, textureHandle;
	GLfloat* vertices;
	GLfloat*texCoords;

	void updateFontSize(GLfloat height);
	void updateVerticesArray();
	void updateTexCoordsArray();

};

#endif /* LABEL_H_ */
