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
#include "managers/Store.h"
#include "log.h"

class Audio {
public:

	static const char* STORE_BG_MUSIC_STATE;
	static const bool BG_MUSIC_DEFAULT = true;

	static void init();

	static void initBackgroundMusic();
	static void playBackgroungMusic();
	static void pauseBackgroundMusic();
	static void stopBackgroundMusic();

	static inline void backgroundMusicOn(){
		shouldPlayBackgroundMusic = true;
		playBackgroungMusic();
		Store::saveBool(STORE_BG_MUSIC_STATE, true);
	};
	static inline void backgroundMusicOff(){
		shouldPlayBackgroundMusic = false;
		pauseBackgroundMusic();
		Store::saveBool(STORE_BG_MUSIC_STATE, false);
	};

	static inline bool isBackgroundMusicOn() {return shouldPlayBackgroundMusic;};

	static void free();

private:
	static SLObjectItf engineObj, outputMixObj;
	static SLEngineItf engine;
	static SLObjectItf bgmPlayerObj;
	static SLPlayItf bgmPlayer;
	static SLSeekItf bgmPlayerSeek;

	static bool shouldPlayBackgroundMusic;

	static void destroyAndNull(SLObjectItf& obj);

};

#endif /* AUDIO_H_ */
