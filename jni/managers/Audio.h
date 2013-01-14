/*
 * Audio.h
 *
 *  Created on: 13.01.2013
 *      Author: Denis
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include "stdlib.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "managers/Art.h"
#include "log.h"

class Audio {
public:
	static void init();

	static void initBackgroundMusic();
	static void playBackgroungMusic();
	static void pauseBackgroundMusic();
	static void stopBackgroundMusic();

	static void free();

private:
	static SLObjectItf engineObj, outputMixObj;
	static SLEngineItf engine;
	static SLObjectItf bgmPlayerObj;
	static SLPlayItf bgmPlayer;
	static SLSeekItf bgmPlayerSeek;

	static void destroyAndNull(SLObjectItf& obj);

};

#endif /* AUDIO_H_ */
