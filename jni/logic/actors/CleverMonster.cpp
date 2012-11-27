/*
 * CleverMonster.cpp
 *
 *  Created on: 27.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "CleverMonster.h"

int CleverMonster::monstersCount = 0;
int CleverMonster::mapsCountX = 0;
int CleverMonster::mapsCountY = 0;
int CleverMonster::mapSize = 0;
int** CleverMonster::maps = NULL;

void CleverMonster::step(double elapsedTime){
	if(remainingTime <= 0.0){
		remainingTime = drand48()*700;
		newDirectionEvent();
	}else{
		int iX, iY;
		float fCurrentXFloor = floorf(x);
		float fCurrentYFloor = floorf(y);
		int iCurrentX = (int) fCurrentXFloor;
		int iCurrentY = (int) fCurrentYFloor;

		switch(state){
			case CM_GO_LEFT:
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

			case CM_GO_RIGHT:
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

			case CM_GO_UP:
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

			case CM_GO_DOWN:
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

void CleverMonster::switchDirection(bool verticalDirectionNow){
	switch(lastEvent){
		case EVENT_MOVE_LEFT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = -speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = CM_GO_LEFT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = -speed;
				state = CM_GO_LEFT;
			}
		break;

		case EVENT_MOVE_RIGHT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = CM_GO_RIGHT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = speed;
				state = CM_GO_RIGHT;
			}
		break;

		case EVENT_MOVE_UP:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = -speed;
				state = CM_GO_UP;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = -speed;
					lastEvent = EVENT_NONE;
					state = CM_GO_UP;
				}
			}
		break;

		case EVENT_MOVE_DOWN:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = speed;
				state = CM_GO_DOWN;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = speed;
					lastEvent = EVENT_NONE;
					state = CM_GO_DOWN;
				}
			}
		break;

		default: break;
	}
}

void CleverMonster::newDirectionEvent(){
	int minX = 0, minY = 0, min = mapSize + 1;

	int pX, pY;
	int width = mapsCountX, height = mapsCountY;
	game->getPacmanMapPos(pX, pY);

	int iX = floorf(x), iY = floorf(y);

	int* map = getMap(pX, pY);


	LOGI("My X:%f, Y:%f", x, y);
	for(int i = -1; i <= 1; ++i){
		for(int j = -1; j <= 1; ++j){
			if((i == 0) ^ (j == 0)){
				int curX = iX + i;
				int curY = iY + j;
				if(curX >= 0 && curX < width && curY >= 0 && curY < height && map[curY*width + curX]){
					LOGI("i = %d, j = %d, x=%d, y=%d, map:%d", i, j, curX, curY, map[curY*width + curX]);
					if(min > map[curY*width + curX]){
						min = map[curY*width + curX];
						minX = i;
						minY = j;
					}
				}

			}
		}
	}
	LOGI("min:%d, minx:%d, miny:%d,", min, minX, minY);

	if(minX  == 1){
		lastEvent = EVENT_MOVE_RIGHT;
	}else if(minX == -1){
		lastEvent = EVENT_MOVE_LEFT;
	}else if(minY == 1){
		lastEvent = EVENT_MOVE_DOWN;
	}else{
		lastEvent = EVENT_MOVE_UP;
	}

}

void CleverMonster::initMaps(){
	mapsCountX = game->getMapWidth();
	mapsCountY = game->getMapHeight();
	mapSize = mapsCountX*mapsCountY;
	maps = new int*[mapSize];
	memset(maps, 0, mapSize*sizeof(int*));
}

void CleverMonster::freeMaps(){
	if(maps){
		for(int i = 0; i < mapsCountX*mapsCountY; ++i){
			if(maps[i]){
				delete[] maps[i];
			}
		}
		delete[] maps;
		maps = NULL;
		mapsCountX = mapsCountY = mapSize = 0;
	}
}

int* CleverMonster::getMap(int x, int y){
	//LOGI("CleverMonster::getMap(%d, %d)", x, y);
	return maps[y*mapsCountX + x] != NULL ? maps[y*mapsCountX + x] : buildMap(x, y);
}

int* CleverMonster::buildMap(int targetX, int targetY){
	LOGI("CleverMonster::buildMap(%d, %d)", targetX, targetY);
	int* target = new int[mapSize];
	memset(target, 0, mapSize*sizeof(int));

	int width = mapsCountX, height = mapsCountY;
	int wave = 1;
	target[width*targetY + targetX] = wave;

	List<int> *xWave = new List<int>(), *yWave = new List<int>();
	List<int> *xWaveNew = new List<int>(), *yWaveNew = new List<int>();
	List<int> *tmpX, *tmpY;
	xWave->pushHead(targetX);
	yWave->pushHead(targetY);

	int x, y;
	bool exists;
	++wave;
	while(!xWave->isEmpty()){
		exists = xWave->getHead(x);
		yWave->getHead(y);

		while(exists){

			//Left
			if(x >= 1 && x < width && y >= 0 && y < height && game->getMapAt(x - 1, y) != Game::TILE_WALL && !target[width*y + x - 1]){
				target[width*y + x - 1] = wave;
				xWaveNew->pushHead(x - 1);
				yWaveNew->pushHead(y);
			}

			//Right
			if(x >= 0 && x < width - 1 && y >= 0 && y < height && game->getMapAt(x + 1, y) != Game::TILE_WALL && !target[width*y + x + 1]){
				target[width*y + x + 1] = wave;
				xWaveNew->pushHead(x + 1);
				yWaveNew->pushHead(y);
			}

			//Up
			if(x >= 0 && x < width && y >= 1 && y < height && game->getMapAt(x, y - 1) != Game::TILE_WALL && !target[width*(y - 1) + x]){
				target[width*(y - 1) + x] = wave;
				xWaveNew->pushHead(x);
				yWaveNew->pushHead(y - 1);
			}

			//Down
			if(x >= 0 && x < width && y >= 0 && y < height - 1 && game->getMapAt(x, y + 1) != Game::TILE_WALL && !target[width*(y + 1) + x]){
				target[width*(y + 1) + x] = wave;
				xWaveNew->pushHead(x);
				yWaveNew->pushHead(y + 1);
			}

			exists = xWave->getNext(x);
			yWave->getNext(y);
		}

		++wave;
		xWave->clear();
		yWave->clear();
		tmpX = xWave;
		tmpY = yWave;
		xWave = xWaveNew;
		yWave = yWaveNew;
		xWaveNew = tmpX;
		yWaveNew = tmpY;

	}

	delete xWave;
	delete yWave;
	delete xWaveNew;
	delete yWaveNew;

	return maps[mapsCountX*targetY + targetX] = target;
}

void CleverMonster::initGraphics(GLuint _shiftProgram){
	shiftProgram = _shiftProgram;
	shiftHandle = glGetUniformLocation(shiftProgram, "uShift");
	shiftVertexHandle = glGetAttribLocation(shiftProgram, "aPosition");
	shiftTextureHandle = glGetAttribLocation(shiftProgram, "aTexture");
}

void CleverMonster::render(double elapsedTime){
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

CleverMonster::~CleverMonster() {
	--monstersCount;
	if(monstersCount == 0){
		freeMaps();
	}
}

