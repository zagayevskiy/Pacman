/*
 * pacmanlib.cpp
 *
 *  Created on: 10.11.2012
 *      Author: Denis Zagayevskiy
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"

#include "logic/Engine.h"
#include "managers/Art.h"
#include "managers/Audio.h"
#include "managers/Store.h"

#define MAX_ELAPSED_TIME 100.0f

static double getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}

double lastTime;
double up2Second;
int framesCount;

Engine* engine;

extern "C" {

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_init(JNIEnv* env, jobject obj, jint width, jint height, jobject pngManager, jobject assetManager, jobject storeManager){
		srand48(time(NULL));
		lastTime = getTime();
		up2Second = 0;
		framesCount = 0;
		Store::init(env, storeManager);
		Art::init(env, width, height, pngManager, assetManager);
		Audio::init();
		engine = new Engine();
		engine->initGraphics(width, height);
		engine->initLogic();
	}


	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_step(JNIEnv* env, jobject obj){
		double time = getTime();
		double elapsedTime = time - lastTime;
		lastTime = time;
		if(elapsedTime > MAX_ELAPSED_TIME){
			LOGE("Critical elapsed time: %f", elapsedTime);
			elapsedTime = MAX_ELAPSED_TIME;
		}

		Statistics::step(elapsedTime);
		engine->step(elapsedTime);
		engine->render(elapsedTime);

		up2Second += elapsedTime;
		++framesCount;
		if(up2Second >= 1000){
			//LOGI("FPS: %d", framesCount);
			up2Second = 0;
			framesCount = 0;
		}
	}

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_actionDown(JNIEnv* env, jobject obj, jfloat x, jfloat y){
		engine->performAction(ACTION_DOWN, x, y);
	}

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_actionMove(JNIEnv* env, jobject obj, jfloat x, jfloat y){
		engine->performAction(ACTION_MOVE, x, y);
	}

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_actionUp(JNIEnv* env, jobject obj, jfloat x, jfloat y){
		engine->performAction(ACTION_UP, x, y);
	}


	JNIEXPORT jboolean JNICALL Java_com_zagayevskiy_pacman_PacmanLib_stop(JNIEnv* env, jobject obj){
		if(engine->stop()){
			Audio::stopBackgroundMusic();
			return true;
		}
		return false;
	}

	JNIEXPORT jboolean JNICALL Java_com_zagayevskiy_pacman_PacmanLib_free(JNIEnv* env, jobject obj){
		LOGI("native free");
		Store::saveString("SomeStringKey", "OK");
		engine->stop();
		engine->save();
		delete engine;
		Audio::free();
		Art::free(env);
		LOGI("native free OK");
	}

}

