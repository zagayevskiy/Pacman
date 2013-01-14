/*
 * Audio.cpp
 *
 *  Created on: 13.01.2013
 *      Author: Denis
 */

#include "Audio.h"

SLObjectItf Audio::engineObj;
SLEngineItf Audio::engine;
SLObjectItf Audio::outputMixObj;
SLObjectItf Audio::bgmPlayerObj;
SLPlayItf Audio::bgmPlayer;
SLSeekItf Audio::bgmPlayerSeek;

void Audio::init(){
	LOGI("Audio::init");
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

	initBackgroundMusic();
}

void Audio::initBackgroundMusic(){
	LOGI("Audio::initBackgroungMusic");
	SLDataLocator_AndroidFD lDataLocatorIn;
	lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
	lDataLocatorIn.fd = Art::bgMusicDescriptor.decriptor;
	lDataLocatorIn.offset = Art::bgMusicDescriptor.start;
	lDataLocatorIn.length = Art::bgMusicDescriptor.length;

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
			&bgmPlayerObj,
			&lDataSource,
			&lDataSink,
			lBGMPlayerIIDCount,
			lBGMPlayerIIDs,
			lBGMPlayerReqs
	);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not CreateAudioPlayer");
		return;
	}

	result = (*bgmPlayerObj)->Realize(bgmPlayerObj, SL_BOOLEAN_FALSE);
	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not Realize bgmPlayerObj");
		return;
	}

	result = (*bgmPlayerObj)->GetInterface(bgmPlayerObj, SL_IID_PLAY, &bgmPlayer);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not GetInterface bgmPlayer");
		return;
	}

	result = (*bgmPlayerObj)->GetInterface(bgmPlayerObj, SL_IID_SEEK, &bgmPlayerSeek);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not GetInterface bgmPlayerSeek");
		return;
	}

	result = (*bgmPlayerSeek)->SetLoop(bgmPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);

	if(result != SL_RESULT_SUCCESS){
		LOGE("Can not SetLoop infinity");
		return;
	}

	LOGI("Background music inited");

}

void Audio::playBackgroungMusic(){
	LOGI("Audio::playBackgroungMusic");

	if(bgmPlayer){
		SLuint32 state;
		(*bgmPlayerObj)->GetState(bgmPlayerObj, &state);
		if(state != SL_OBJECT_STATE_REALIZED){
			LOGE("bgmPlayerObj not Realized");
			return;
		}

		if((*bgmPlayer)->SetPlayState(bgmPlayer, SL_PLAYSTATE_PLAYING) != SL_RESULT_SUCCESS){
			LOGE("Can not SetPlayState PLAYING");
			return;
		}

		LOGI("Background music play now");
	}

};

void Audio::pauseBackgroundMusic(){
	LOGI("Audio::pauseBackgroundMusic");
	if(bgmPlayer){
		SLuint32 state;
		(*bgmPlayerObj)->GetState(bgmPlayerObj, &state);
		if(state != SL_OBJECT_STATE_REALIZED){
			LOGE("bgmPlayerObj not Realized");
			return;
		}

		if((*bgmPlayer)->SetPlayState(bgmPlayer, SL_PLAYSTATE_PAUSED) != SL_RESULT_SUCCESS){
			LOGE("Can not SetPlayState PAUSED");
			return;
		}

		LOGI("Background music paused");
	}
}

void Audio::stopBackgroundMusic(){
	LOGI("Audio::stopBackgroundMusic");
	if(bgmPlayer){
		SLuint32 state;
		(*bgmPlayerObj)->GetState(bgmPlayerObj, &state);
		if(state == SL_OBJECT_STATE_REALIZED){
			(*bgmPlayer)->SetPlayState(bgmPlayer, SL_PLAYSTATE_PAUSED);
			(*bgmPlayerObj)->Destroy(bgmPlayerObj);
			bgmPlayerObj = NULL;
			bgmPlayer = NULL;
			bgmPlayerSeek = NULL;
		}

		LOGI("Background music stopped");
	}
};

void Audio::free(){
	LOGI("Audio::free");

	if(bgmPlayer){
		SLuint32 state;
		(*bgmPlayerObj)->GetState(bgmPlayerObj, &state);
		if(state == SL_OBJECT_STATE_REALIZED){
			(*bgmPlayer)->SetPlayState(bgmPlayer, SL_PLAYSTATE_STOPPED);
			destroyAndNull(bgmPlayerObj);
			bgmPlayer = NULL;
			bgmPlayerSeek = NULL;
		}
	}
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
