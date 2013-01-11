/*
 * Label.cpp
 *
 *  Created on: 10.01.2013
 *      Author: Denis
 */

#include "Label.h"

Label::Label(float _x, float _y, const char* _text, GLuint _vertexHandle, GLuint _textureHandle, GLfloat _fontHeight)
	:x(_x), y(_y), text(NULL), vertexHandle(_vertexHandle), textureHandle(_textureHandle), vertices(NULL), texCoords(NULL)
{
	updateFontSize(_fontHeight);
	setText(_text);
}

void Label::render(double elapsedTime){

	if(text){
		glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_FONT_CONSOLAS));

		glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		checkGlError("glVertexAttribPointer");
		glEnableVertexAttribArray(vertexHandle);
		checkGlError("glEnableVertexAttribArray");

		glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
		checkGlError("glVertexAttribPointer");
		glEnableVertexAttribArray(textureHandle);
		checkGlError("glEnableVertexAttribArray");

		glDrawArrays(GL_TRIANGLES, 0, 6*textLength);
		checkGlError("glDrawArrays");

		glDisableVertexAttribArray(textureHandle);
		glDisableVertexAttribArray(vertexHandle);
	}
}

void Label::setText(const char* _text){

	if(text){
		delete[] text;
	}

	if(_text){

		textLength = strlen(_text);
		text = new char[textLength + 1];
		strncpy(text, _text, textLength);

	}else{
		textLength = 0;
		text = NULL;
	}

	updateVerticesArray();
	updateTexCoordsArray();
}

void Label::setFontHeight(GLfloat height){
	updateFontSize(height);
	updateVerticesArray();
}

void Label::updateFontSize(GLfloat height){
	fontHeight = height;
	fontWidth = height / 15.0f * 7.0f;
}

void Label::updateVerticesArray(){
	if(vertices){
		delete[] vertices;
	}
	if(text){
		vertices = new GLfloat[textLength*12];	 //for each char: 2 triangles. each vertex of 2 coords

		// 0 ---- 1
		// |      |
		// |      |
		// 3 ---- 2
		for(int i = 0; i < textLength; ++i){
			GLfloat tmp[12] = {
				//1st triangle
				x + fontWidth*i, y,				 				//0 vertex
				x + fontWidth*(i + 1), y,				 	  	//1 vertex
				x + fontWidth*(i + 1), y + fontHeight,		 	//2 vertex
				//2nd triangle
				x + fontWidth*(i + 1), y + fontHeight,		 	//2 vertex
				x + fontWidth*i, y + fontHeight,		 		//3 vertex
				x + fontWidth*i, y,				 				//0 vertex
			};
			memcpy(vertices + i*12, tmp, 12*sizeof(GLfloat));
		}
	}else{
		vertices = NULL;
	}
}

void Label::updateTexCoordsArray(){

	if(texCoords){
		delete[] texCoords;
	}

	if(text){
		texCoords = new GLfloat[textLength*12];	 //for each char: 2 triangles. each vertex of 2 tex coords

		GLfloat texFontWidth = 1.0f / 15.0f;
		GLfloat texFontHeight = 1.0f / 7.0f;

		// 0 ---- 1
		// |      |
		// |      |
		// 3 ---- 2
		for(int i = 0; i < textLength; ++i){
			int row = (text[i] - ' ') / 15;
			int col = (text[i] - ' ') % 15;

			GLfloat tmp[12] = {
				//1st triangle
				col*texFontWidth, row*texFontHeight,			//0 vertex
				(col + 1)*texFontWidth, row*texFontHeight,		//1 vertex
				(col + 1)*texFontWidth, (row + 1)*texFontHeight,//2 vertex
				//2nd triangle
				(col + 1)*texFontWidth, (row + 1)*texFontHeight,//2 vertex
				col*texFontWidth, (row + 1)*texFontHeight,		//3 vertex
				col*texFontWidth, row*texFontHeight,			//0 vertex
			};

			memcpy(texCoords + i*12, tmp, 12*sizeof(GLfloat));
		}
	}else{
		texCoords = NULL;
	}
}

Label::~Label() {
	if(text){
		delete[] text;
		delete[] vertices;
		delete[] texCoords;
	}
}

