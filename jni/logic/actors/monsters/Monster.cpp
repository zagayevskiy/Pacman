/*
 * Monster.cpp
 *
 *  Created on: 14.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Monster.h"

void Monster::step(double elapsedTime){
	if(remainingTime <= 0.0){
		remainingTime = drand48()*maxRemainingTime;
		newDirectionEvent();
	}else{
		int iX, iY;
		float fCurrentXFloor = floorf(x);
		float fCurrentYFloor = floorf(y);
		int iCurrentX = (int) fCurrentXFloor;
		int iCurrentY = (int) fCurrentYFloor;

		switch(state){
			case MONSTER_GO_LEFT:
				iX = (int) floor(x - radius + speedX*elapsedTime);
				iY = iCurrentY;

				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					x += speedX*elapsedTime;
				}else{
					x = fCurrentXFloor + 0.5;
					newDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(false);
				}
			break;

			case MONSTER_GO_RIGHT:
				iX = (int) floor(x + radius + speedX*elapsedTime);
				iY = iCurrentY;
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					x += speedX*elapsedTime;
				}else{
					x = fCurrentXFloor + 0.5;
					newDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(false);
				}
			break;

			case MONSTER_GO_UP:
				iX = iCurrentX;
				iY = (int) floor(y - radius + speedY*elapsedTime);
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					y += speedY*elapsedTime;
				}else{
					y = fCurrentYFloor + 0.5;
					newDirectionEvent();
				}
				if(lastEvent != EVENT_NONE){
					switchDirection(true);
				}
			break;

			case MONSTER_GO_DOWN:
				iX = iCurrentX;
				iY = (int) floor(y + radius + speedY*elapsedTime);
				if(game->getMapAt(iX, iY) != Game::TILE_WALL){
					y += speedY*elapsedTime;
				}else{
					y = fCurrentYFloor + 0.5;
					newDirectionEvent();
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

void Monster::switchDirection(bool verticalDirectionNow){
	switch(lastEvent){
		case EVENT_MOVE_LEFT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = -speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = MONSTER_GO_LEFT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = -speed;
				state = MONSTER_GO_LEFT;
			}
		break;

		case EVENT_MOVE_RIGHT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = MONSTER_GO_RIGHT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = speed;
				state = MONSTER_GO_RIGHT;
			}
		break;

		case EVENT_MOVE_UP:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = -speed;
				state = MONSTER_GO_UP;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = -speed;
					lastEvent = EVENT_NONE;
					state = MONSTER_GO_UP;
				}
			}
		break;

		case EVENT_MOVE_DOWN:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = speed;
				state = MONSTER_GO_DOWN;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = speed;
					lastEvent = EVENT_NONE;
					state = MONSTER_GO_DOWN;
				}
			}
		break;

		default: break;
	}
}

void Monster::initGraphics(GLuint _shiftProgram){
	//animation = new Animation(_shiftProgram, Art::getTexture(Art::TEXTURE_MONSTER_ANIMATION), 2, 2, 2, 500.0, game->getTileSize(), game->getTileSize());
	plume = new Plume(game->getTileSize(), Art::getTexture(Art::TEXTURE_WATER), 15.0, 1.0f);
}

void Monster::render(double elapsedTime){
	GLfloat tileSize = game->getTileSize();
	//animation->render(elapsedTime, (x - radius)*tileSize + game->getShiftX(), (y - radius)*tileSize + game->getShiftY());
	plume->pushPoint((x - radius)*tileSize + game->getShiftX(), (y - radius)*tileSize + game->getShiftY());
	plume->render(elapsedTime);

}

Monster::~Monster() {
	//if(animation){
	//	delete animation;
	//}
	if(plume){
		delete plume;
	}
}

