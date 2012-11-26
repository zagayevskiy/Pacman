/*
 * Game.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "logic/actors/Pacman.h"
#include "logic/actors/Monster.h"
#include "logic/actors/StupidMonster.h"

#include "Game.h"

void Game::initGraphics(float _maxX, float _maxY, GLuint _stableProgram, GLuint _shiftProgram){
	LOGI("Game::initGraphics");
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

void Game::createBuffers(){
	// 0 ---- 1
	// |      |
	// |      |
	// 3 ---- 2

	// x, y, tx, ty
	GLfloat* verticesData = new GLfloat[16*mapWidth*mapHeight];
	//0, 1, 2, 2, 3, 0
	GLshort* indicesData = new GLshort[6*mapWidth*mapHeight];
	GLshort indicesNumbers[] = {0, 1, 2, 2, 3, 0};
	int squareIndex = 0;
	int iInds = 0;
	int vertsCount = 0;
	for(int i = 0; i < mapHeight; ++i){
		for(int j = 0; j < mapWidth; ++j){

			GLfloat verticesCoords[8] = {
					j*tileSize, i*tileSize, 			//Vertex 0
					(j + 1)*tileSize, i*tileSize,		//Vertex 1
					(j + 1)*tileSize, (i + 1)*tileSize, //Vertex 2
					j*tileSize, (i + 1)*tileSize		//Vertex 3
			};

			int m = map[i*mapWidth + j];
			GLfloat* textureCoords  = m == TILE_FREE ? Art::TEX_COORDS_TILE_FREE : m == TILE_WALL ? Art::TEX_COORDS_TILE_WALL : Art::TEX_COORDS_TILE_FOOD;

			squareIndex = (i*mapWidth + j)*16;

			//Square: 4 vertex (x, y, tx, ty)
			for(int k = 0; k < 4; ++k){
				verticesData[squareIndex + k*4 + 0] = verticesCoords[k*2 + 0];
				verticesData[squareIndex + k*4 + 1] = verticesCoords[k*2 + 1];
				verticesData[squareIndex + k*4 + 2] = textureCoords[k*2 + 0];
				verticesData[squareIndex + k*4 + 3] = textureCoords[k*2 + 1];
			}

			iInds = (i*mapWidth + j)*6;
			vertsCount = (i*mapWidth + j)*4;

			for(int k = 0; k < 6; ++k){
				indicesData[iInds + k] = vertsCount + indicesNumbers[k];
			}

		}
	}

	glGenBuffers(1, &verticesBufferId);
	checkGlError("glGenBuffers(1, &verticesBufferId);");
	LOGI("Game::verticesBufferId: %d", verticesBufferId);

	glGenBuffers(1, &indicesBufferId);
	checkGlError("glGenBuffers(1, &indicesBufferId);");
	LOGI("Game::indicesBufferId: %d", indicesBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
	checkGlError("glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
	checkGlError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);");

	glBufferData(GL_ARRAY_BUFFER, 16*mapWidth*mapHeight*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ARRAY_BUFFER, verticesDataLength*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*mapWidth*mapHeight*sizeof(GLshort), indicesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLshort), indicesData, GL_STATIC_DRAW);");

	delete[] verticesData;
	delete[] indicesData;

	//Lifes Texture Coords
	GLfloat ltc[] = {
		0.0, 0.0,
		0.5, 0.0,
		0.5, 0.5,
		0.0, 0.5
	};

	verticesData = new GLfloat[Pacman::MAX_LIFES_COUNT*16];
	indicesData = new GLshort[Pacman::MAX_LIFES_COUNT*6];


	for(int i = 0; i < Pacman::MAX_LIFES_COUNT; ++i){
		GLfloat squareData[16] = {
				i*tileSize, mapHeight*tileSize, ltc[0], ltc[1],
				(i + 1)*tileSize, mapHeight*tileSize, ltc[2], ltc[3],
				(i + 1)*tileSize, (mapHeight + 1)*tileSize, ltc[4], ltc[5],
				i*tileSize, (mapHeight + 1)*tileSize, ltc[6], ltc[7]
		};
		memcpy(&(verticesData[16*i]), squareData, 16*sizeof(GLfloat));
		for(int j = 0; j < 6; ++j){
			indicesData[i*6 + j] = indicesNumbers[j] + i*4;
		}
	}

	glGenBuffers(1, &lifesVerticesBufferId);
	checkGlError("glGenBuffers(1, &lifesVerticesBufferId);");
	LOGI("Game::lifesVerticesBufferId: %d", lifesVerticesBufferId);

	glGenBuffers(1, &lifesIndicesBufferId);
	checkGlError("glGenBuffers(1, &lifesIndicesBufferId);");
	LOGI("Game::lifesIndicesBufferId: %d", lifesIndicesBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, lifesVerticesBufferId);
	checkGlError("glBindBuffer(GL_ARRAY_BUFFER, lifesVerticesBufferId);");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lifesIndicesBufferId);
	checkGlError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lifesIndicesBufferId);");

	glBufferData(GL_ARRAY_BUFFER, Pacman::MAX_LIFES_COUNT*16*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ARRAY_BUFFER, verticesDataLength*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Pacman::MAX_LIFES_COUNT*6*sizeof(GLshort), indicesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLshort), indicesData, GL_STATIC_DRAW);");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkGlError("glBindBuffer(GL_ARRAY_BUFFER, 0);");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	checkGlError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);");

	delete[] verticesData;
	delete[] indicesData;

}

void Game::freeBuffers(){
	glDeleteBuffers(1, &verticesBufferId);
	glDeleteBuffers(1, &indicesBufferId);
	glDeleteBuffers(1, &lifesVerticesBufferId);
	glDeleteBuffers(1, &lifesIndicesBufferId);
}

void Game::event(EngineEvent e){
	lastEvent = e;
}

void Game::step(double elapsedTime){
	bool exists;
	switch(state){
		case PACMAN_ALIVE:
			if(lastEvent != EVENT_NONE){
				pacman->event(lastEvent);
				lastEvent = EVENT_NONE;
			}
			Actor* monster;
			exists = monsters.getHead(monster);
			while(exists){
				monster->step(elapsedTime);
				exists = monsters.getNext(monster);
			}
			pacman->step(elapsedTime);
			if(((Pacman*)pacman)->isDead()){
				state = PACMAN_DEAD;
			}
		break;

		case PACMAN_DEAD:
			pacman->step(elapsedTime);
			if(((Pacman*)pacman)->isGameOver()){
				state = GAME_OVER;
			}else{
				if(!((Pacman*)pacman)->isDead()){
					state = PACMAN_ALIVE;
				}
			}

		break;

		case GAME_OVER:
		break;

		default: break;
	}


}

void Game::render(double elapsedTime){

	glUseProgram(stableProgram);

	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_TILES));

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);

	if(isMapChanged){
		int globalOffset = lastChangedY*mapWidth + lastChangedX;
		int m = map[globalOffset];
		GLfloat* tf = m == TILE_FREE ? Art::TEX_COORDS_TILE_FREE : m == TILE_WALL ? Art::TEX_COORDS_TILE_WALL : Art::TEX_COORDS_TILE_FOOD;
		//Square: 4 vertex(x, y, tx, ty)
		GLfloat buffer[16] = {
				lastChangedX*tileSize, lastChangedY*tileSize, 				//Vertex 0
				tf[0], tf[1],
				(lastChangedX + 1)*tileSize, lastChangedY*tileSize,			//Vertex 1
				tf[2], tf[3],
				(lastChangedX + 1)*tileSize, (lastChangedY + 1)*tileSize, 	//Vertex 2
				tf[4], tf[5],
				lastChangedX*tileSize, (lastChangedY + 1)*tileSize,			//Vertex 3
				tf[6], tf[7]
		};

		glBufferSubData(GL_ARRAY_BUFFER, globalOffset*16*sizeof(GLfloat), 16*sizeof(GLfloat), buffer);

		isMapChanged = false;
		lastChangedX = lastChangedY = -1;
	}

	//Render the map
	//x, y, tx, ty
	GLsizei stride = 4 * sizeof(GLfloat);

	glVertexAttribPointer(stableVertexHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*)(0));
	glVertexAttribPointer(stableTextureHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*) (2*sizeof(GLfloat)));

	glEnableVertexAttribArray(stableVertexHandle);
	glEnableVertexAttribArray(stableTextureHandle);

	glDrawElements(GL_TRIANGLES, 6*mapWidth*mapHeight, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, lifesVerticesBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lifesIndicesBufferId);

	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_PACMAN_ANIMATION));

	glVertexAttribPointer(stableVertexHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*)(0));
	glVertexAttribPointer(stableTextureHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*) (2*sizeof(GLfloat)));

	glDrawElements(GL_TRIANGLES, 6*((Pacman*) pacman)->getLifes(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(stableTextureHandle);
	glDisableVertexAttribArray(stableVertexHandle);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
	LOGI("Game::loadLevel %dx%d", level->width, level->height);
	mapWidth = level->width;
	mapHeight = level->height;
	tileSize = maxX / ((float) mapWidth);
	isMapChanged = false;
	lastChangedX = lastChangedY = -1;
	maxLevelScore = 0;
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
				++maxLevelScore;
				continue;
			}

			if(b > 0 && b <= MAX_MONSTER_B){
				monsters.pushTail(new StupidMonster(this, (float)(j/4), (float)i, shiftProgram));
				continue;
			}

			if(r > MIN_PACMAN_R && g > MIN_PACMAN_G){
				pacman = new Pacman(this, (float)(j/4), (float)i, shiftProgram);
				continue;
			}

		}
	}
	if(!pacman){
		LOGE("LEVEL FORMAT ERROR: CAN NOT FIND PACMAN!");
	}
	createBuffers();
	state = PACMAN_ALIVE;

}

void Game::clear(){
	LOGI("Game::clear");
	if(map){
		delete[] map;
		map = NULL;
	}
	if(pacman){
		delete pacman;
		pacman = NULL;
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
	return (_x >= 0 && _x < mapWidth && _y >= 0 && _y < mapHeight) ? map[_y*mapWidth + _x] : TILE_WALL;
}

void Game::setMapAt(int _x, int _y, int value){
	if(_x >= 0 && _x < mapWidth && _y >= 0 && _y < mapHeight){
		map[_y*mapWidth + _x] = value;
		lastChangedX = _x;
		lastChangedY = _y;
		isMapChanged = true;
	}
}

Game::~Game() {
	clear();
	freeBuffers();
}

