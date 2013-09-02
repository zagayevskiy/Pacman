/*
 * CleverMonster.cpp
 *
 *  Created on: 27.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "CleverMonster.h"

int CleverMonster::mapsCountX = 0;
int CleverMonster::mapsCountY = 0;
int CleverMonster::mapSize = 0;
int** CleverMonster::maps = NULL;

void CleverMonster::newDirectionEvent(){
	int minX = 0, minY = 0, min = mapSize + 1;

	int pX, pY;
	int width = mapsCountX, height = mapsCountY;
	game->getPacmanMapPos(pX, pY);

	int iX = floorf(x), iY = floorf(y);

	int* map = getMap(pX, pY);

	for(int i = -1; i <= 1; ++i){
		for(int j = -1; j <= 1; ++j){
			if((i == 0) ^ (j == 0)){
				int curX = iX + i;
				int curY = iY + j;
				if(curX >= 0 && curX < width && curY >= 0 && curY < height && map[curY*width + curX]){
					if(min > map[curY*width + curX] || (min == map[curY*width + curX] && rand() % 2)){
						min = map[curY*width + curX];
						minX = i;
						minY = j;
					}
				}

			}
		}
	}

	if(minX == 1){
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
	return maps[y*mapsCountX + x] != NULL ? maps[y*mapsCountX + x] : buildMap(x, y);
}

int* CleverMonster::buildMap(int targetX, int targetY){
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

CleverMonster::~CleverMonster() {
	if(isLast()){
		freeMaps();
	}
}

