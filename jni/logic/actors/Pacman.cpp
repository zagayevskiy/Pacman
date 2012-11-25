/*
 * Pacman.cpp
 *
 *  Created on: 13.11.2012
 *      Author: Denis Zagayevskiy
 */

#include "Pacman.h"

void Pacman::initGraphics(GLuint _shiftProgram){
	shiftProgram = _shiftProgram;
	shiftHandle = glGetUniformLocation(shiftProgram, "uShift");
	shiftVertexHandle = glGetAttribLocation(shiftProgram, "aPosition");
	shiftTextureHandle = glGetAttribLocation(shiftProgram, "aTexture");
	textureId = Art::getTexture(Art::TEXTURE_PACMAN_ANIMATION);

	const GLfloat PACMAN_SIZE = 0.5f;
	GLfloat tileSize = game->getTileSize();

	// 0 ---- 1
	// |      |
	// |      |
	// 3 ---- 2

	const int vertexLength = 8;
	const int verticesCount = 4;
	const int initVertsLength = verticesCount*vertexLength;

	animationOffsetsLength = 4;
	animationOffsets = new GLuint[animationOffsetsLength];
	animationOffsets[0] = 2*sizeof(GLfloat); //Step 0
	animationOffsets[1] = 4*sizeof(GLfloat); //Step 1
	animationOffsets[2] = 6*sizeof(GLfloat); //Step 2
	animationOffsets[3] = 4*sizeof(GLfloat); //Step 1
	animationStepTime = 1000.0 / (speed*1000.0) / animationOffsetsLength;
	animationElapsedTime = 0.0;
	animationStepNumber = 0;

	//tx1, ty1 - animation step 1 and etc.
	GLfloat initVertsData[initVertsLength] = {
		//x, y, tx1, ty1, tx2, ty2, tx3, ty3

		//Vertex 0
		0.0f, 0.0f, 0.0f, 0.0f, PACMAN_SIZE, 0.0f, 0.0f, PACMAN_SIZE,

		//Vertex 1
		tileSize, 0.0f, PACMAN_SIZE, 0.0f, PACMAN_SIZE*2, 0.0f, PACMAN_SIZE, PACMAN_SIZE,

		//Vertex 2
		tileSize, tileSize, PACMAN_SIZE, PACMAN_SIZE, PACMAN_SIZE*2, PACMAN_SIZE, PACMAN_SIZE, PACMAN_SIZE*2,

		//Vertex 3
		0.0f, tileSize, 0.0f, PACMAN_SIZE, PACMAN_SIZE, PACMAN_SIZE, 0.0f, PACMAN_SIZE*2

	};

	//Rotation indices
	GLshort initInds[4*verticesCount] = {
		0, 1, 2, 3, //Right
		1, 0, 3, 2, //Left
		3, 0, 1, 2, //Down
		1, 2, 3, 0  //Up
	};
	offsetGoRight = 0;
	offsetGoLeft = initVertsLength*sizeof(GLfloat);
	offsetGoDown = 2*initVertsLength*sizeof(GLfloat);
	offsetGoUp = 3*initVertsLength*sizeof(GLfloat);

	//Create 4 groups, each of initVertsCount vertices(for rotation)
	const int verticesDataLength = initVertsLength*4;
	GLfloat* verticesData = new GLfloat[verticesDataLength];
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < verticesCount; ++j){
			verticesData[i*initVertsLength + j*vertexLength + 0] = initVertsData[j*vertexLength + 0];
			verticesData[i*initVertsLength + j*vertexLength + 1] = initVertsData[j*vertexLength + 1];
			for(int k = 2; k < vertexLength; ++k){
				verticesData[i*initVertsLength + j*vertexLength + k] = initVertsData[initInds[i*4 + j]*vertexLength + k];
			}
		}
	}

	GLshort indicesData[6] = {
		0, 1, 2, 2, 3, 0
	};

	glGenBuffers(1, &verticesBufferId);
	checkGlError("glGenBuffers(1, &verticesBufferId);");
	LOGI("verticesBufferId: %d", verticesBufferId);

	glGenBuffers(1, &indicesBufferId);
	checkGlError("glGenBuffers(1, &indicesBufferId);");
	LOGI("indicesBufferId: %d", indicesBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
	checkGlError("glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
	checkGlError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);");

	glBufferData(GL_ARRAY_BUFFER, verticesDataLength*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ARRAY_BUFFER, verticesDataLength*sizeof(GLfloat), verticesData, GL_STATIC_DRAW);");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLshort), indicesData, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLshort), indicesData, GL_STATIC_DRAW);");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] verticesData;
}

void Pacman::freeGraphics(){
	glDeleteBuffers(1, &verticesBufferId);
	glDeleteBuffers(1, &indicesBufferId);
	if(animationOffsets){
		delete[] animationOffsets;
	}
}

void Pacman::event(EngineEvent e){
	LOGI("Pacman::event(%d)", e);

	lastEvent = e;
}

void Pacman::step(double elapsedTime){

	if(state != PACMAN_DIED){
		List<Actor*>& monsters = game->getMonsters();
		Actor* monster;
		bool exists = monsters.getHead(monster);

		while(exists){
			if(intersect(monster)){
				diedTime = 0.0f;
				state = PACMAN_DIED;
				return;
			}
			exists = monsters.getNext(monster);
		}
	}

	int iX, iY;
	float fCurrentXFloor = floorf(x);
	float fCurrentYFloor = floorf(y);
	int iCurrentX = (int) fCurrentXFloor;
	int iCurrentY = (int) fCurrentYFloor;
	if(game->getMapAt(iCurrentX, iCurrentY) == Game::TILE_FOOD){
		game->setMapAt(iCurrentX, iCurrentY, Game::TILE_FREE);
	}


	switch(state){
		case PACMAN_GO_LEFT:
			iX = (int) floor(x - radius + speedX*elapsedTime);
			iY = iCurrentY;

			if(game->getMapAt(iX, iY) != Game::TILE_WALL){
				x += speedX*elapsedTime;
			}else{
				x = fCurrentXFloor + 0.5;
			}
			if(lastEvent != EVENT_NONE){
				switchDirection(false);
			}
		break;

		case PACMAN_GO_RIGHT:
			iX = (int) floor(x + radius + speedX*elapsedTime);
			iY = iCurrentY;
			if(game->getMapAt(iX, iY) != Game::TILE_WALL){
				x += speedX*elapsedTime;
			}else{
				x = fCurrentXFloor + 0.5;
			}
			if(lastEvent != EVENT_NONE){
				switchDirection(false);
			}
		break;

		case PACMAN_GO_UP:
			iX = iCurrentX;
			iY = (int) floor(y - radius + speedY*elapsedTime);
			if(game->getMapAt(iX, iY) != Game::TILE_WALL){
				y += speedY*elapsedTime;
			}else{
				y = fCurrentYFloor + 0.5;
			}
			if(lastEvent != EVENT_NONE){
				switchDirection(true);
			}
		break;

		case PACMAN_GO_DOWN:
			iX = iCurrentX;
			iY = (int) floor(y + radius + speedY*elapsedTime);
			if(game->getMapAt(iX, iY) != Game::TILE_WALL){
				y += speedY*elapsedTime;
			}else{
				y = fCurrentYFloor + 0.5;
			}
			if(lastEvent != EVENT_NONE){
				switchDirection(true);
			}
		break;

		case PACMAN_DIED:
			if(diedTime > MAX_DIED_TIME){
				x = initialX;
				y = initialY;
				state = initialState;
				speedX = speed;
				speedY = 0;
			}else{
				diedTime += elapsedTime;
			}
		break;

		default: break;
	}

	totalPathLength += speed*elapsedTime;
	totalStepsCount += 1.0;
	averageStepLength = totalPathLength / totalStepsCount;

	//LOGI("Total path: %f, Speed: %f, Average step length: %f, Current step length: %f", totalPathLength, speed, averageStepLength, speed*elapsedTime);
}

void Pacman::switchDirection(bool verticalDirectionNow){
	switch(lastEvent){
		case EVENT_MOVE_LEFT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = -speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = PACMAN_GO_LEFT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = -speed;
				state = PACMAN_GO_LEFT;
			}
		break;

		case EVENT_MOVE_RIGHT:
			if(verticalDirectionNow){
				if(getYCellCenterDistance() < averageStepLength){
					y = floorf(y) + 0.5;
					speedX = speed;
					speedY = 0.0;
					lastEvent = EVENT_NONE;
					state = PACMAN_GO_RIGHT;
				}
			}else{
				lastEvent = EVENT_NONE;
				speedX = speed;
				state = PACMAN_GO_RIGHT;
			}
		break;

		case EVENT_MOVE_UP:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = -speed;
				state = PACMAN_GO_UP;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = -speed;
					lastEvent = EVENT_NONE;
					state = PACMAN_GO_UP;
				}
			}
		break;

		case EVENT_MOVE_DOWN:
			if(verticalDirectionNow){
				lastEvent = EVENT_NONE;
				speedY = speed;
				state = PACMAN_GO_DOWN;
			}else{
				if(getXCellCenterDistance() < averageStepLength){
					x = floorf(x) + 0.5;
					speedX = 0.0f;
					speedY = speed;
					lastEvent = EVENT_NONE;
					state = PACMAN_GO_DOWN;
				}
			}
		break;

		default: break;
	}
}

void Pacman::render(double elapsedTime){

	animationElapsedTime += elapsedTime;
	if(animationElapsedTime >= animationStepTime){
		animationStepNumber = (animationStepNumber + 1) % animationOffsetsLength;
		animationElapsedTime = 0.0;
	}

	GLfloat tileSize = game->getTileSize();
	GLfloat shiftX = (x - radius)*tileSize;
	GLfloat shiftY = (y - radius)*tileSize;
	GLuint vertexOffset, textureOffset;
	switch(state){
		case PACMAN_GO_RIGHT:
			vertexOffset = offsetGoRight;
		break;
		case PACMAN_GO_LEFT:
			vertexOffset = offsetGoLeft;
		break;
		case PACMAN_GO_DOWN:
			vertexOffset = offsetGoDown;
		break;
		case PACMAN_GO_UP:
			vertexOffset = offsetGoUp;
		break;
		default: break;
	}
	textureOffset = vertexOffset + animationOffsets[animationStepNumber];

	glUseProgram(shiftProgram);

	glUniform2f(shiftHandle, shiftX, shiftY);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);

	//x, y, tx1, ty1, tx2, ty2, tx3, ty3
	GLsizei stride = 8 * sizeof(float);

	glVertexAttribPointer(shiftVertexHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*)(vertexOffset));
	glVertexAttribPointer(shiftTextureHandle, 2, GL_FLOAT, GL_FALSE, stride, (void*) (textureOffset));

	glEnableVertexAttribArray(shiftVertexHandle);
	glEnableVertexAttribArray(shiftTextureHandle);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(shiftTextureHandle);
	glDisableVertexAttribArray(shiftVertexHandle);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*GLfloat texCoords[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0
	};

	GLfloat tileSize = game->getTileSize();

	glBindTexture(GL_TEXTURE_2D, Art::getTexture(Art::TEXTURE_PACMAN));

	GLfloat shiftX = (x - radius)*tileSize;
	GLfloat shiftY = (y - radius)*tileSize;
	glUniform2f(shiftHandle, shiftX, shiftY);
	//LOGI("(%f, %f)", shiftX, shiftY);


	GLfloat pacmanCoords[] = {
		0.0, 0.0, tileSize, 0.0, tileSize, tileSize,
		tileSize, tileSize, 0.0, tileSize, 0.0, 0.0
	};

	glVertexAttribPointer(shiftVertexHandle, 2, GL_FLOAT, GL_FALSE, 0, pacmanCoords);
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
	glDisableVertexAttribArray(shiftVertexHandle);*/

}

Pacman::~Pacman() {
	freeGraphics();
}

