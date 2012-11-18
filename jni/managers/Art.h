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
	Texture(char* p, int w, int h): pixels(p), width(w), height(h){}
	~Texture(){
		if(pixels){
			delete[] pixels;
		}
	}
};

class Art {

public:

	static const int MAX_PATH = 32;
	static const char* LEVELS_PATH;

	static const int TEXTURE_PLAY_BUTTON = 0;
	static const int TEXTURE_GRAD = 1;
	static const int TEXTURE_YELLOW = 2;
	static const int TEXTURE_PACMAN = 3;
	static const int TEXTURE_MAP_0 = 4;
	static const int TEXTURE_MONSTER = 5;
	static const int TEXTURE_LEVEL0 = 6;
	static const int TEXTURE_BUTTONS = 7;
	static const int TEXTURES_COUNT = 8;
	static const int TEXTURE_NONE = 0;

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

	static void init(JNIEnv* env, jobject _pngManager, jobject javaAssetManager);

	static void generateTextures();
	static GLuint getTexture(int id);

	static char* getShaderSource(int id);

	static int getLevelsCount() {return levelsCount;};
	static Texture* getLevel(int number);

	static void free();


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

	static Texture** levels;
	static int levelsCount;

	static char* argb2rgba(unsigned int* src, int width, int weight);
	static Texture* loadPng(const char* filename);
	static GLuint createTexture(Texture* texture);
	static List<char*> loadFilesList(const char* path);
	static char* loadTextFile(const char* filename);
	static void loadMaps();

};

#endif /* ART_H_ */
