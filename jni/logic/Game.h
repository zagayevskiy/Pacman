/*
 * Game.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef GAME_H_
#define GAME_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include "logic/StateMachine.h"
#include "logic/actors/Actor.h"

#include "graphics/IRenderable.h"

#include "templates/list.h"
#include "managers/Art.h"
#include "actions_events.h"
#include "log.h"

class Pacman;
class Monster;

class Game: public StateMachine, public IRenderable {
public:

	static const int TILE_FREE = 1;
	static const int TILE_WALL = 10;
	static const int TILE_FOOD = 20;
	static const char MAX_WALL_R = 200;
	static const char MAX_FOOD_G = 200;
	static const char MAX_STUPED_MONSTER_B = 200;
	static const char MIN_CLEVER_MONSTER_R = 200;
	static const char MIN_CLEVER_MONSTER_B = 200;
	static const char MIN_PACMAN_R = 200;
	static const char MIN_PACMAN_G = 200;

	void loadLevel(const Texture* level);
	void initGraphics(float maxX, float maxY, GLuint stableProgram, GLuint shiftProgram);

	void event(EngineEvent e);
	void step(double elapsedTime);
	void render(double elapsedTime);
	void clear();

	bool isGameOver() const {return state == GAME_OVER;};
	bool isWin() const {return state == WIN;};

	int getMapAt(int x, int y) const;
	void setMapAt(int x, int y, int value);
	List<Monster*>& getMonsters(){return monsters;};
	void getPacmanMapPos(int& x, int& y) const;
	int getMaxLevelScore() const {return maxLevelScore;};


	float getTileSize() const {return tileSize;}
	int getMapWidth() const {return mapWidth;}
	int getMapHeight() const {return mapHeight;}

	virtual ~Game();

	float maxX;
	float maxY;
private:

	enum GameState{
		PACMAN_DEAD = 0,
		PACMAN_ALIVE = 1,
		GAME_OVER = 2,
		WIN = 3
	};

	GameState state;
	int* map;
	int mapWidth, mapHeight;
	int maxLevelScore;
	int lastChangedX, lastChangedY;
	bool isMapChanged;

	Pacman* pacman;
	List<Monster*> monsters;
	List<IRenderable*> objectsToRender;
	EngineEvent lastEvent;

	float x, y;

	float tileSize;
	GLuint stableProgram;
	GLuint stableVertexHandle, stableTextureHandle;
	GLuint stableMapHandle, stableMatrixHandle;
	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftMapHandle, shiftMatrixHandle;
	GLuint shiftHandle;

	GLuint verticesBufferId, indicesBufferId;
	GLuint lifesVerticesBufferId, lifesIndicesBufferId;

	void createBuffers();
	void freeBuffers();

};

#endif /* GAME_H_ */

