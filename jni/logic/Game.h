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
#include "graphics/ui/controls/Label.h"
#include "graphics/animation/Animation.h"

#include "templates/list.h"

#include "managers/Art.h"
#include "managers/Store.h"

#include "actions_events.h"
#include "log.h"

class Pacman;
class Monster;
class Bonus;

class Game: public StateMachine, public IRenderable {
public:

	static const int TILE_FREE = '_';
	static const int TILE_WALL = 'W';
	static const int TILE_FOOD = 'F';

	static const unsigned int R_OFFSET = 24;
	static const unsigned int G_OFFSET = 16;
	static const unsigned int B_OFFSET = 8;

	static const unsigned int PACMAN_COLOR	= 255	<< R_OFFSET | 220	<< G_OFFSET | 0		<< B_OFFSET;
	static const unsigned int WALL_COLOR	= 100	<< R_OFFSET | 75	<< G_OFFSET | 50	<< B_OFFSET;
	static const unsigned int FOOD_COLOR	= 5		<< R_OFFSET | 150	<< G_OFFSET | 5		<< B_OFFSET;
	static const unsigned int FREE_COLOR	= 0		<< R_OFFSET | 0		<< G_OFFSET | 0		<< B_OFFSET;
	static const unsigned int STUPID_COLOR	= 0		<< R_OFFSET | 0		<< G_OFFSET | 175	<< B_OFFSET;
	static const unsigned int CLEVER_COLOR	= 50	<< R_OFFSET | 175	<< G_OFFSET | 175	<< B_OFFSET;
	static const unsigned int LIFE_COLOR	= 10	<< R_OFFSET | 210	<< G_OFFSET | 10	<< B_OFFSET;

	void enterLevel(int number);
	void initGraphics(float maxX, float maxY, GLuint stableProgram, GLuint shiftProgram);

	void event(EngineEvent e);
	void step(double elapsedTime);
	void render(double elapsedTime);
	void clear();
	void save();
	void load();

	inline bool isGameOver() const {return state == GAME_OVER;};
	inline bool isWin() const {return state == WIN;};

	char getMapAt(int x, int y) const;
	void setMapAt(int x, int y, char value);
	inline List<Monster*>& getMonsters(){return monsters;};
	void getPacmanMapPos(int& x, int& y) const;
	inline Pacman* getPacman() const {return pacman;};
	inline int getLevelFoodCount() const {return levelFoodCount;};

	inline float getTileSize() const {return tileSize;}
	inline int getMapWidth() const {return mapWidth;}
	inline int getMapHeight() const {return mapHeight;}
	inline float getShiftX() const {return shiftX;};
	inline float getShiftY() const {return shiftY;};

	virtual ~Game();

	float maxX;
	float maxY;

private:

	static const char* NAME_STATE;
	static const char* NAME_LEVEL_NUMBER;
	//static const char* NAME_LEVEL_FOOD_COUNT;
	static const char* NAME_GAME_MAP;

	enum GameState{
		PACMAN_DEAD = 0,
		PACMAN_ALIVE = 1,
		GAME_OVER = 2,
		WIN = 3
	};

	GameState state;
	char* map;
	int levelNumber;
	int mapWidth, mapHeight;
	int levelFoodCount;
	int lastChangedX, lastChangedY;
	bool isMapChanged;

	Pacman* pacman;
	List<Monster*> monsters;
	List<Bonus*> bonuses;
	List<IRenderable*> objectsToRender;
	EngineEvent lastEvent;

	float shiftX, shiftY;

	float tileSize;
	GLuint stableProgram;
	GLuint stableVertexHandle, stableTextureHandle;
	GLuint stableMapHandle, stableMatrixHandle;
	GLuint shiftProgram;
	GLuint shiftVertexHandle, shiftTextureHandle;
	GLuint shiftMapHandle, shiftMatrixHandle;
	GLuint shiftHandle;

	GLuint verticesBufferId, indicesBufferId;

	void loadLevel(int number);
	void createBuffers();
	void freeBuffers();
};

#endif /* GAME_H_ */

