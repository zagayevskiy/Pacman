/*
 * Game.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "logic/actors/Pacman.h"
#include "logic/actors/Monster.h"

#include "Game.h"

void Game::initGraphics(float _maxX, float _maxY, GLuint _stableProgram, GLuint _shiftProgram){
	maxX = _maxX;
	maxY = _maxY;
	stableProgram = _stableProgram;
	shiftProgram = _shiftProgram;

	stableVertexHandle = glGetAttribLocation(stableProgram, "aPosition");
	checkGlError("glGetAttribLocation");

	stableTextureHandle = glGetAttribLocation(stableProgram, "aTexture");
	checkGlError("glGetAttribLocation");

	stableMapHandle = glGetUniformLocation(stableProgram, "uMap");
	checkGlError("glGetUniformLocation");

	stableMatrixHandle = glGetUniformLocation(stableProgram, "uMatrix");
	checkGlError("glGetUniformLocation");

	shiftVertexHandle = glGetAttribLocation(shiftProgram, "aPosition");
	checkGlError("glGetAttribLocation");

	shiftTextureHandle = glGetAttribLocation(shiftProgram, "aTexture");
	checkGlError("glGetAttribLocation");

	shiftMapHandle = glGetUniformLocation(shiftProgram, "uMap");
	checkGlError("glGetUniformLocation");

	shiftMatrixHandle = glGetUniformLocation(shiftProgram, "uMatrix");
	checkGlError("glGetUniformLocation");

	shiftHandle = glGetUniformLocation(shiftProgram, "uShift");
	checkGlError("glGetUniformLocation");
	x = y = 0.0;
}

void Game::event(EngineEvent e){
	lastEvent = e;
}

void Game::step(double elapsedTime){
	if(lastEvent != EVENT_NONE){
		pacman->event(lastEvent);
		lastEvent = EVENT_NONE;
	}
	pacman->step(elapsedTime);
	Actor* monster;
	bool exists = monsters.getHead(monster);
	while(exists){
		monster->step(elapsedTime);
		exists = monsters.getNext(monster);
	}

}

void Game::render(double elapsedTime){
	/*glUseProgram(shiftProgram);

	float top = 0.0;

	GLfloat triangles[] = {
		0.0, top, maxX, top, maxX, top + maxX,
		maxX, top + maxX, 0.0, top + maxX, 0.0, top
	};

	GLfloat texCoords[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0
	};

	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_LEVEL0));
	glVertexAttribPointer(shiftVertexHandle, 2, GL_FLOAT, GL_FALSE, 0, triangles);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftVertexHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(shiftTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(shiftTextureHandle);
	checkGlError("glEnableVertexAttribArray");

	//x += 0.0005;
	//y += 0.0005;
	glUniform2f(shiftHandle, x, y);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	glDisableVertexAttribArray(shiftTextureHandle);
	glDisableVertexAttribArray(shiftVertexHandle);
*/
	glUseProgram(stableProgram);

	GLfloat freeTexCoords[] = {
		0.0, 0.0, 0.5, 0.0, 0.5, 0.5,
		0.5, 0.5, 0.0, 0.5, 0.0, 0.0
	};

	GLfloat wallTexCoords[] = {
		0.5, 0.0, 1.0, 0.0, 1.0, 0.5,
		1.0, 0.5, 0.5, 0.5, 0.5, 0.0
	};

	GLfloat foodTexCoords[] = {
		0.0, 0.5, 0.5, 0.5, 0.5, 1.0,
		0.5, 1.0, 0.0, 1.0, 0.0, 0.5
	};




	GLfloat top = 0.05;
	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_MAP_0));
	for(int i = 0; i < mapHeight; ++i){
		for(int j = 0; j < mapWidth; ++j){

			GLfloat triangles[] = {
				tileSize*j, tileSize*i, tileSize*(j + 1), tileSize*i, tileSize*(j + 1), tileSize*(i + 1),
				tileSize*(j + 1), tileSize*(i + 1), tileSize*j, tileSize*(i + 1), tileSize*j, tileSize*i
			};


			glVertexAttribPointer(stableVertexHandle, 2, GL_FLOAT, GL_FALSE, 0, triangles);
			checkGlError("glVertexAttribPointer");
			glEnableVertexAttribArray(stableVertexHandle);
			checkGlError("glEnableVertexAttribArray");

			int m = map[i*mapWidth + j];
			glVertexAttribPointer(stableTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, m == TILE_FREE ? freeTexCoords : m == TILE_WALL ? wallTexCoords : foodTexCoords);
			checkGlError("glVertexAttribPointer");
			glEnableVertexAttribArray(stableTextureHandle);
			checkGlError("glEnableVertexAttribArray");
			glDrawArrays(GL_TRIANGLES, 0, 6);
			checkGlError("glDrawArrays");

			glDisableVertexAttribArray(shiftTextureHandle);
			glDisableVertexAttribArray(shiftVertexHandle);
		}
	}

	pacman->render(elapsedTime);
	Actor* monster;
	bool exists = monsters.getHead(monster);
	while(exists){
		monster->render(elapsedTime);
		exists = monsters.getNext(monster);
	}

}

void Game::loadLevel(const Texture* level){
	clear();
	pacman = NULL;
	LOGI("Game::loadLevel %dx%d", level->width, level->height);
	mapWidth = level->width;
	mapHeight = level->height;
	tileSize = maxX / ((float) mapWidth);
	LOGI("TileSize: %f", tileSize);
	map = new int[mapWidth * mapHeight];
	char r, g, b;
	for(int i = 0; i < mapHeight; ++i){
		int offset = i*mapWidth*4;
		for(int j = 0; j < mapWidth*4; j += 4){
			map[i*mapWidth + j/4] = TILE_FREE;
			r = level->pixels[offset + j + 0];
			g = level->pixels[offset + j + 1];
			b = level->pixels[offset + j + 2];

			if(r > 0 && r <= MAX_WALL_R){
				map[i*mapWidth + j/4] = TILE_WALL;
				continue;
			}

			if(g > 0 && g <= MAX_FOOD_G){
				map[i*mapWidth + j/4] = TILE_FOOD;
				continue;
			}

			if(b > 0 && b <= MAX_MONSTER_B){
				LOGW("Monster: %d, %d", j/4, i);
				monsters.pushTail(new Monster((float)(j/4), (float)i));
				continue;
			}

			if(r > MIN_PACMAN_R && g > MIN_PACMAN_G){
				LOGW("Pacman: %d, %d", j/4, i);
				pacman = new Pacman(this, (float)(j/4), (float)i, shiftProgram);
				continue;
			}

		}
	}
	if(!pacman){
		LOGE("LEVEL FORMAT ERROR: CAN NOT FIND PACMAN!");
	}

}

void Game::clear(){
	LOGI("Game::clear");
	if(map){
		delete[] map;
	}
	if(pacman){
		delete pacman;
	}
	if(!monsters.isEmpty()){
		Actor* monster;
		bool exists = monsters.getHead(monster);
		while(exists){
			if(monster){
				delete monster;
			}
			exists = monsters.getNext(monster);
		}
		monsters.clear();
	}
}

int Game::getMapAt(int _x, int _y) const{
	if(_x >= 0 && _x < mapWidth && _y >= 0 && _y < mapHeight){
		return map[_y*mapWidth + _x];
	}else{
		return 0;
	}
}

void Game::setMapAt(int _x, int _y, int value){
	if(_x >= 0 && _x < mapWidth && _y >= 0 && _y < mapHeight){
		map[_y*mapWidth + _x] = value;
	}
}

Game::~Game() {
	clear();
}

