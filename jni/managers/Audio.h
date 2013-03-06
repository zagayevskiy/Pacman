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

	static const char* SHOULD_PLAY_BACKGROUND_MUSIC;
	static const bool SHOULD_PLAY_BACKGROUND_MUSIC_DEFAULT = true;

	static void init();

	static void playGameBackground();
	static void playMenuBackground();

	static inline void stopBackgroundMusic(){
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_STOPPED);
	}

	static inline void backgroundMusicOn(){
		LOGI("Audio::backgroundMusicOn");
		isBackgroundOn = true;
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_PLAYING);
		Store::saveBool(SHOULD_PLAY_BACKGROUND_MUSIC, true);
	};
	static inline void backgroundMusicOff(){
		LOGI("Audio::backgroundMusicOff");
		isBackgroundOn = false;
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_STOPPED);
		Store::saveBool(SHOULD_PLAY_BACKGROUND_MUSIC, false);
	};

	static inline bool isBackgroundMusicOn() {return isBackgroundOn;};

	static void free();

private:
	static SLObjectItf engineObj, outputMixObj;
	static SLEngineItf engine;

	static SLObjectItf gameBackgroundPlayerObj, menuBackgroundPlayerObj;
	static SLPlayItf gameBackgroundPlayer, menuBackgroundPlayer, currentBackgroundPlayer;
	static SLSeekItf gameBackgroundSeek, menuBackgroundSeek;

	static bool isBackgroundOn;

	static SLuint32 createAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek, ResourseDescriptor resourseDescriptor);
	static void destroyAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek);
	static void destroyAndNull(SLObjectItf& obj);

};

#endif /* AUDIO_H_ */
