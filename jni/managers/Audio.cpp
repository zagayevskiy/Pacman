/*
 * Audio.cpp
 *
 *  Created on: 13.01.2013
 *      Author: Denis
 */

#include "Audio.h"

const char* Audio::SHOULD_PLAY_BACKGROUND_MUSIC = "ShouldPlayBGMusic";
const char* Audio::SHOULD_PLAY_SOUNDS = "ShouldPlaySounds";

SLObjectItf Audio::engineObj = NULL;
SLEngineItf Audio::engine = NULL;
SLObjectItf Audio::outputMixObj = NULL;

SLObjectItf Audio::gameBackgroundPlayerObj = NULL;
SLPlayItf Audio::gameBackgroundPlayer = NULL;
SLSeekItf Audio::gameBackgroundSeek = NULL;

SLObjectItf Audio::menuBackgroundPlayerObj = NULL;
SLPlayItf Audio::menuBackgroundPlayer = NULL;
SLSeekItf Audio::menuBackgroundSeek = NULL;

SLPlayItf Audio::currentBackgroundPlayer = NULL;

SLObjectItf Audio:: soundsPlayerObj = NULL;
SLBufferQueueItf Audio::soundsBufferQueue = NULL;
SLPlayItf Audio::soundsPlayer = NULL;

bool Audio::shouldPlayBackgroundMusic = SHOULD_PLAY_BACKGROUND_MUSIC_DEFAULT;
bool Audio::shouldPlaySounds = SHOULD_PLAY_SOUNDS_DEFAULT;

void Audio::init(){
	LOGI("Audio::init");
	shouldPlayBackgroundMusic = Store::loadBool(SHOULD_PLAY_BACKGROUND_MUSIC, SHOULD_PLAY_BACKGROUND_MUSIC_DEFAULT);
	shouldPlaySounds = Store::loadBool(SHOULD_PLAY_SOUNDS, SHOULD_PLAY_SOUNDS_DEFAULT);
	free();

	 SLresult result;

	const SLuint32 lEngineMixIIDCount = 1;
	const SLInterfaceID lEngineMixIIDs[] = {SL_IID_ENGINE};
	const SLboolean lEngineMixReqs[] = {SL_BOOLEAN_TRUE};
	const SLuint32 lOutputMixIIDCount = 0;
	const SLInterfaceID lOutputMixIIDs[] = {};
	const SLboolean lOutputMixReqs[] = {};

	result = slCreateEngine(&engineObj, 0, NULL, lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Error after slCreateEngine");
		return;
	}

	result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Error after Realize");
		return;
	}

	result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Error after GetInterface");
		return;
	}

	result = (*engine)->CreateOutputMix(engine, &outputMixObj, lOutputMixIIDCount, lOutputMixIIDs, lOutputMixReqs);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Error after CreateOutputMix");
		return;
	}

	result = (*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Error after Realize");
		return;
	}

	createAudioPlayer(gameBackgroundPlayerObj, gameBackgroundPlayer, gameBackgroundSeek, Art::getGameBackgroundMusicDescriptor());
	createAudioPlayer(menuBackgroundPlayerObj, menuBackgroundPlayer, menuBackgroundSeek, Art::getMenuBackgroundMusicDescriptor());
	currentBackgroundPlayer = menuBackgroundPlayer;

	createBufferQueuePlayer(soundsPlayerObj, soundsPlayer, soundsBufferQueue);

}

void Audio::playMenuBackground(){
	LOGI("Audio::playMenuBackground");
	if(shouldPlayBackgroundMusic){
		(*gameBackgroundPlayer)->SetPlayState(gameBackgroundPlayer, SL_PLAYSTATE_STOPPED);
		(*menuBackgroundPlayer)->SetPlayState(menuBackgroundPlayer, SL_PLAYSTATE_PLAYING);
	}
	currentBackgroundPlayer = menuBackgroundPlayer;
};

void Audio::playGameBackground(){
	LOGI("Audio::playGameBackground");
	if(shouldPlayBackgroundMusic){
		(*menuBackgroundPlayer)->SetPlayState(menuBackgroundPlayer, SL_PLAYSTATE_STOPPED);
		(*gameBackgroundPlayer)->SetPlayState(gameBackgroundPlayer, SL_PLAYSTATE_PLAYING);
	}
	currentBackgroundPlayer = gameBackgroundPlayer;
};

SLuint32 Audio::createAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek, ResourseDescriptor resourseDescriptor){
	SLDataLocator_AndroidFD lDataLocatorIn;
	lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;

	lDataLocatorIn.fd = resourseDescriptor.decriptor;
	lDataLocatorIn.offset = resourseDescriptor.start;
	lDataLocatorIn.length = resourseDescriptor.length;

	SLDataFormat_MIME lDataFormat;
	lDataFormat.formatType = SL_DATAFORMAT_MIME;
	lDataFormat.mimeType = NULL;
	lDataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

	SLDataSource lDataSource;
	lDataSource.pLocator = &lDataLocatorIn;
	lDataSource.pFormat = &lDataFormat;

	SLDataLocator_OutputMix lDataLocatorOut;
	lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	lDataLocatorOut.outputMix = outputMixObj;

	SLDataSink lDataSink;
	lDataSink.pLocator = &lDataLocatorOut;
	lDataSink.pFormat = NULL;

	const SLuint32 lBGMPlayerIIDCount = 2;
	const SLInterfaceID lBGMPlayerIIDs[] = {SL_IID_PLAY, SL_IID_SEEK};
	const SLboolean lBGMPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

	SLresult result = (*engine)->CreateAudioPlayer(
			engine,
			&playerObj,
			&lDataSource,
			&lDataSink,
			lBGMPlayerIIDCount,
			lBGMPlayerIIDs,
			lBGMPlayerReqs
	);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not CreateAudioPlayer %d", result);
		playerObj = NULL;
		return result;
	}

	result = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not Realize playerObj");
		playerObj = NULL;
		return result;
	}

	result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &player);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not GetInterface player");
		destroyAndNull(playerObj);
		player = NULL;
		return result;
	}

	result = (*playerObj)->GetInterface(playerObj, SL_IID_SEEK, &seek);

	(*seek)->SetLoop(seek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
	(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);

	return SL_RESULT_SUCCESS;

}

void Audio::destroyAudioPlayer(SLObjectItf& playerObj, SLPlayItf& player, SLSeekItf& seek){
	if(player){
		(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
		player = NULL;
		seek = NULL;
	}
	destroyAndNull(playerObj);
};

SLuint32 Audio::createBufferQueuePlayer(SLObjectItf& playerObj, SLPlayItf& player, SLBufferQueueItf& bufferQueue){
	 SLresult result;

	// configure audio source
	SLDataLocator_AndroidSimpleBufferQueue locatorBufferQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
	SLDataFormat_PCM formatPCM = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
		SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
		SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
	SLDataSource audioSrc = {&locatorBufferQueue, &formatPCM};

	// configure audio sink
	SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObj};
	SLDataSink audioSnk = {&loc_outmix, NULL};

	// create audio player
	const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
			/*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
	const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
			/*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
	result = (*engine)->CreateAudioPlayer(engine, &playerObj, &audioSrc, &audioSnk, 3, ids, req);

	// realize the player
	result = (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);

	// get the play interface
	result = (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &player);

	// get the buffer queue interface
	result = (*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &bufferQueue);

	/*
	// register callback on the buffer queue
	result = (*bufferQueue)->RegisterCallback(bufferQueue, bqPlayerCallback, NULL);
	assert(SL_RESULT_SUCCESS == result);

	// get the effect send interface
	result = (*playerObj)->GetInterface(playerObj, SL_IID_EFFECTSEND,
			&bqPlayerEffectSend);
	assert(SL_RESULT_SUCCESS == result);
#if 0   // mute/solo is not supported for sources that are known to be mono, as this is
	// get the mute/solo interface
	result = (*playerObj)->GetInterface(playerObj, SL_IID_MUTESOLO, &bqPlayerMuteSolo);
	assert(SL_RESULT_SUCCESS == result);
#endif

	// get the volume interface
	result = (*playerObj)->GetInterface(playerObj, SL_IID_VOLUME, &bqPlayerVolume);
	assert(SL_RESULT_SUCCESS == result);
*/
	// set the player's state to playing
	result = (*player)->SetPlayState(player, SL_PLAYSTATE_PLAYING);

	return result;
}

void Audio::destroyBufferQueuePlayer(SLObjectItf& playerObj, SLPlayItf& player, SLBufferQueueItf& bufferQueue){
	if(player){
		(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
		player = NULL;
		bufferQueue = NULL;
	}
	destroyAndNull(playerObj);
}

void Audio::free(){
	LOGI("Audio::free");

	destroyAudioPlayer(gameBackgroundPlayerObj, gameBackgroundPlayer, gameBackgroundSeek);
	destroyAudioPlayer(menuBackgroundPlayerObj, menuBackgroundPlayer, menuBackgroundSeek);

	destroyBufferQueuePlayer(soundsPlayerObj, soundsPlayer, soundsBufferQueue);

	destroyAndNull(outputMixObj);
	destroyAndNull(engineObj);
	engine = NULL;
}

void Audio::destroyAndNull(SLObjectItf& obj){
	if(obj){
		(*obj)->Destroy(obj);
		obj = NULL;
	}
}
