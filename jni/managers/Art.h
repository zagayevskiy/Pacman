/*
 * Art.h
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef ART_H_
#define ART_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


#include "log.h"
#include "templates/list.h"
#include "ShadersManager.h"

struct Texture{
	char* pixels; //should be allocated with new[width*height*4]; RGBA
	int width;
	int height;
	Texture(): pixels(NULL), width(0), height(0){}
	Texture(char* p, int w, int h): pixels(p), width(w), height(h){};
	~Texture(){
		if(pixels){
			delete[] pixels;
			pixels = NULL;
		}
	}
};

struct Level{
	Texture* map;
	char* name;
	Level(): name(NULL), map(NULL){};
	Level(char* _name, Texture* _map):name(_name), map(_map){};
	~Level(){
		if(name){
			delete[] name;
			name = NULL;
		}
		if(map){
			delete map;
			map = NULL;
		}
	}
};

struct ResourseDescriptor{
	int32_t decriptor;
	off_t start;
	off_t length;
};

class Art {

public:

	static ResourseDescriptor bgMusicDescriptor;

	static const int MAX_PATH = 32;
	static const char* LEVELS_PATH;
	static const int LEVELS_ON_SIDE_COUNT = 4;
	static const int MAX_LEVELS_COUNT = LEVELS_ON_SIDE_COUNT*LEVELS_ON_SIDE_COUNT;
	static const int MAX_LEVEL_SIZE = 32;

	static const int TEXTURE_NONE = INT_MAX;
	static const int TEXTURE_PACMAN_ANIMATION = 0;
	static const int TEXTURE_BUTTONS = 1;
	static const int TEXTURE_TILES = 2;
	static const int TEXTURE_MONSTER = 3;
	static const int TEXTURE_ALL_LEVELS = 4;
	static const int TEXTURE_MONSTER_ANIMATION = 5;
	static const int TEXTURE_HEART = 6;
	static const int TEXTURE_FONT_CONSOLAS = 7;
	static const int TEXTURES_COUNT = 8;

	static const int SHADER_VERTEX_0 = 0;
	static const int SHADER_FRAGMENT_0 = 1;
	static const int SHADER_VERTEX_SHIFT = 2;
	static const int SHADERS_COUNT = 3;

	static GLfloat TEX_COORDS_BUTTON_LEFT[12];
	static GLfloat TEX_COORDS_BUTTON_LEFT_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_RIGHT[12];
	static GLfloat TEX_COORDS_BUTTON_RIGHT_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_UP[12];
	static GLfloat TEX_COORDS_BUTTON_UP_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_DOWN[12];
	static GLfloat TEX_COORDS_BUTTON_DOWN_PRESSED[12];

	/*Texture coords for menu buttons*/
	static GLfloat TEX_COORDS_BUTTON_PLAY[12];
	static GLfloat TEX_COORDS_BUTTON_PLAY_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_RETRY[12];
	static GLfloat TEX_COORDS_BUTTON_RETRY_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_NEXT[12];
	static GLfloat TEX_COORDS_BUTTON_NEXT_PRESSED[12];
	static GLfloat TEX_COORDS_BUTTON_LIST[12];
	static GLfloat TEX_COORDS_BUTTON_LIST_PRESSED[12];

	static GLfloat TEX_COORDS_BUTTON_MUSIC_ON[12];
	static GLfloat TEX_COORDS_BUTTON_MUSIC_OFF[12];

	static GLfloat TEX_COORDS_TILE_WALL[8];
	static GLfloat TEX_COORDS_TILE_FREE[8];
	static GLfloat TEX_COORDS_TILE_FOOD[8];

	static void init(JNIEnv* env, jobject _pngManager, jobject javaAssetManager);

	static void generateTextures();
	static GLuint getTexture(int id);

	static char* getShaderSource(int id);

	static int getLevelsCount() {return levelsCount;};
	static Level* getLevel(int number);
	static GLfloat* getLevelTexCoords(int number);

	static void free(JNIEnv* env);


private:

	static JNIEnv* pmEnv;
	static jobject pngManager;
	static AAssetManager* assetManager;
	static jclass pmClass;
	static jmethodID pmOpenId;
	static jmethodID pmCloseId;
	static jmethodID pmGetWidthId;
	static jmethodID pmGetHeightId;
	static jmethodID pmGetPixelsId;

	static Texture** texturesSources;
	static GLuint* textures;

	static char** shadersSources;

	static Level** levels;
	static int levelsCount;
	static GLfloat** levelsTexCoords;

	static char* argb2rgba(unsigned int* src, int width, int weight);
	static Texture* loadPng(const char* filename);
	static GLuint createTexture(Texture* texture);
	static List<char*> loadFilesList(const char* path);
	static char* loadTextFile(const char* filename);
	static void loadLevels();
	static Texture* makeTextureFromLevels();

};

#endif /* ART_H_ */
