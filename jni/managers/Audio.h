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
	static const char* SHOULD_PLAY_SOUNDS;
	static const bool SHOULD_PLAY_BACKGROUND_MUSIC_DEFAULT = true;
	static const bool SHOULD_PLAY_SOUNDS_DEFAULT = false;

	static void init();

	static void playGameBackground();
	static void playMenuBackground();

	static inline void stopBackgroundMusic(){
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_STOPPED);
	}

	static inline void backgroundMusicOn(){
		LOGI("Audio::backgroundMusicOn");
		shouldPlayBackgroundMusic = true;
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_PLAYING);
		Store::saveBool(SHOULD_PLAY_BACKGROUND_MUSIC, true);
	};

	static inline void backgroundMusicOff(){
		LOGI("Audio::backgroundMusicOff");
		shouldPlayBackgroundMusic = false;
		(*currentBackgroundPlayer)->SetPlayState(currentBackgroundPlayer, SL_PLAYSTATE_STOPPED);
		Store::saveBool(SHOULD_PLAY_BACKGROUND_MUSIC, false);
	};

	static inline bool isBackgroundMusicOn() {return shouldPlayBackgroundMusic;};

	static inline void playSound(unsigned int id){
		if(shouldPlaySounds){
			const SoundBuffer* sound = Art::getSound(id);
			(*soundsBufferQueue)->Clear(soundsBufferQueue);
			(*soundsBufferQueue)->Enqueue(soundsBufferQueue, sound->buffer, sound->length);
		}
	};

	static inline void soundsOn(){
		shouldPlaySounds = true;
		Store::saveBool(SHOULD_PLAY_SOUNDS, true);
	}

	static inline void soundsOff(){
		shouldPlaySounds = false;
		Store::saveBool(SHOULD_PLAY_SOUNDS, false);
	}

	static inline bool isSoundsOn(){return shouldPlaySounds;};

	static void free();

private:
	static SLObjectItf engineObj, outputMixObj;
	static SLEngineItf engine;

	static SLObjectItf gameBackgroundPlayerObj, menuBackgroundPlayerObj;
	static SLPlayItf gameBackgroundPlayer, menuBackgroundPlayer, currentBackgroundPlayer;
	static SLSeekItf gameBackgroundSeek, menuBackgroundSeek;

	static SLObjectItf soundsPlayerObj;
	static SLBufferQueueItf soundsBufferQueue;
	static SLPlayItf soundsPlayer;

	static bool shouldPlayBackgroundMusic;
	static bool shouldPlaySounds;

	static SLuint32 createAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek, ResourseDescriptor resourseDescriptor);
	static void destroyAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek);

	static SLuint32 createBufferQueuePlayer(SLObjectItf& playerObj, SLPlayItf& player, SLBufferQueueItf& bufferQueue);
	static void destroyBufferQueuePlayer(SLObjectItf& playerObj, SLPlayItf& player, SLBufferQueueItf& bufferQueue);

	static void destroyAndNull(SLObjectItf& obj);

};

#endif /* AUDIO_H_ */
