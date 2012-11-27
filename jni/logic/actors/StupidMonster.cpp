/*
 * StupidMonster.cpp
 *
 *  Created on: 19.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "StupidMonster.h"

void StupidMonster::step(double elapsedTime){
	if(remainingTime <= 0.0){
		remainingTime = drand48()*5000;
		randomDirectionEvent();
	}else{
		int iX, iY;
		float fCurrentXFloor = floorf(x);
		float fCurrentYFloor = floorf(y);
		int iCurrentX = (int) fCurrentXFloor;
		int iCurrentY = (int) fCurrentYFloor;

		switch(state){
			case SM_GO_LEFT:
				iX = (int) floor(x - radius + speedX*elapsedTime);
				iY = iCurrentY;

				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					x += speedX*elapsedTime;
				}else{
					x = fCurrentXFloor + 0.5;
					randomDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(false);
				}
			break;

			case SM_GO_RIGHT:
				iX = (int) floor(x + radius + speedX*elapsedTime);
				iY = iCurrentY;
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					x += speedX*elapsedTime;
				}else{
					x = fCurrentXFloor + 0.5;
					randomDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(false);
				}
			break;

			case SM_GO_UP:
				iX = iCurrentX;
				iY = (int) floor(y - radius + speedY*elapsedTime);
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					y += speedY*elapsedTime;
				}else{
					y = fCurrentYFloor + 0.5;
					randomDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(true);
				}
			break;

			case SM_GO_DOWN:
				iX = iCurrentX;
				iY = (int) floor(y + radius + speedY*elapsedTime);
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					y += speedY*elapsedTime;
				}else{
					y = fCurrentYFloor + 0.5;
					randomDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(true);
				}
			break;

			default: break;
		}
		remainingTime -= elapsedTime;
		totalPathLength += speed*elapsedTime;
		totalStepsCount += 1.0;
		averageStepLength = totalPathLength / totalStepsCount;
	}
}

void StupidMonster::randomDirectionEvent(){
	switch(rand() % 4){
		case 0: lastEvent = EVENT_MOVE_LEFT; break;
		case 1: lastEvent = EVENT_MOVE_UP; break;
		case 2: lastEvent = EVENT_MOVE_RIGHT; break;
		case 3: lastEvent = EVENT_MOVE_DOWN; break;
	}

}

void StupidMonster::switchDirection(bool verticalDirectionNow){
	switch(lastEvent){
		case EVENT_MOVE_LEFT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = -speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = SM_GO_LEFT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = -speed;
				state = SM_GO_LEFT;
			}
		break;

		case EVENT_MOVE_RIGHT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = SM_GO_RIGHT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = speed;
				state = SM_GO_RIGHT;
			}
		break;

		case EVENT_MOVE_UP:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = -speed;
				state = SM_GO_UP;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = -speed;
					lastEvent = EVENT_NONE;
					state = SM_GO_UP;
				}
			}
		break;

		case EVENT_MOVE_DOWN:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = speed;
				state = SM_GO_DOWN;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = speed;
					lastEvent = EVENT_NONE;
					state = SM_GO_DOWN;
				}
			}
		break;

		default: break;
	}
}

void StupidMonster::initGraphics(GLuint _shiftProgram){
	shiftProgram = _shiftProgram;
	shiftHandle = glGetUniformLocation(shiftProgram, "uShift");
	shiftVertexHandle = glGetAttribLocation(shiftProgram, "aPosition");
	shiftTextureHandle = glGetAttribLocation(shiftProgram, "aTexture");
}

void StupidMonster::render(double elapsedTime){
	glUseProgram(shiftProgram);

	GLfloat texCoords[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0
	};
	GLfloat tileSize = game->getTileSize();

	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_MONSTER));

	GLfloat shiftX = (x - radius)*tileSize;
	GLfloat shiftY = (y - radius)*tileSize;
	glUniform2f(shiftHandle, shiftX, shiftY);
	//LOGI("(%f, %f)", shiftX, shiftY);


	GLfloat monsterCoords[] = {
		0.0, 0.0, tileSize, 0.0, tileSize, tileSize,
		tileSize, tileSize, 0.0, tileSize, 0.0, 0.0
	};

	glVertexAttribPointer(shiftVertexHandle, 2, GL_FLOAT, GL_FALSE, 0, monsterCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftVertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(shiftTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftTextureHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(shiftTextureHandle);
	glDisableVertexAttribArray(shiftVertexHandle);
}

StupidMonster::~StupidMonster() {
}

