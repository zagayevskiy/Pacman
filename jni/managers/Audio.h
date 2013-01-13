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
	static void init(){
		LOGI("Audio::init");

		 SLresult result;

		// create engine

		const SLuint32 lEngineMixIIDCount = 1;
		const SLInterfaceID lEngineMixIIDs[] = {SL_IID_ENGINE};
		const SLboolean lEngineMixReqs[] = {SL_BOOLEAN_TRUE};
		const SLuint32 lOutputMixIIDCount = 0;
		const SLInterfaceID lOutputMixIIDs[] = {};
		const SLboolean lOutputMixReqs[] = {};

		result = slCreateEngine(&mEngineObj, 0, NULL, lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Error after slCreateEngine");
			return;
		}

		result = (*mEngineObj)->Realize(mEngineObj, SL_BOOLEAN_FALSE);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Error after Realize");
			return;
		}

		result = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE, &mEngine);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Error after GetInterface");
			return;
		}

		result = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj, lOutputMixIIDCount, lOutputMixIIDs, lOutputMixReqs);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Error after CreateOutputMix");
			return;
		}

		result = (*mOutputMixObj)->Realize(mOutputMixObj, SL_BOOLEAN_FALSE);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Error after Realize");
			return;
		}

	}

	static void playBGM(){
		LOGI("Audio::playBGM");
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
		lDataLocatorOut.outputMix = mOutputMixObj;

		SLDataSink lDataSink;
		lDataSink.pLocator = &lDataLocatorOut;
		lDataSink.pFormat = NULL;

		const SLuint32 lBGMPlayerIIDCount = 2;
		const SLInterfaceID lBGMPlayerIIDs[] = {SL_IID_PLAY, SL_IID_SEEK};
		const SLboolean lBGMPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

		SLresult result = (*mEngine)->CreateAudioPlayer(
				mEngine,
				&mBGMPlayerObj,
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

		result = (*mBGMPlayerObj)->Realize(mBGMPlayerObj, SL_BOOLEAN_FALSE);
		if(result != SL_RESULT_SUCCESS){
			LOGE("Can not Realize bgmPlayerObj");
			return;
		}

		result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_PLAY, &mBGMPlayer);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Can not GetInterface bgmPlayer");
			return;
		}

		result = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj, SL_IID_SEEK, &mBGMPlayerSeek);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Can not GetInterface bgmPlayerSeek");
			return;
		}

		result = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);

		if(result != SL_RESULT_SUCCESS){
			LOGE("Can not SetLoop infinity");
			return;
		}

		result = (*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PLAYING);


		if(result != SL_RESULT_SUCCESS){
			LOGE("Can not SetPlayState PLAYING");
			return;
		}

		LOGI("Background music play now");

	};

	static void stopBGM(){
		LOGI("Audio::stopBGM");
		if(mBGMPlayer){
			SLuint32 state;
			(*mBGMPlayerObj)->GetState(mBGMPlayerObj, &state);
			if(state == SL_OBJECT_STATE_REALIZED){
				(*mBGMPlayer)->SetPlayState(mBGMPlayer, SL_PLAYSTATE_PAUSED);
				(*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
				mBGMPlayerObj = NULL;
				mBGMPlayer = NULL;
				mBGMPlayerSeek = NULL;
			}
		}

		LOGI("Background music stopped");
	};

	static void free(){
		if(mOutputMixObj){
			(*mOutputMixObj)->Destroy(mOutputMixObj);
			mOutputMixObj = NULL;
		}

		if(mEngineObj){
			(*mEngineObj)->Destroy(mEngineObj);
			mEngineObj = NULL;
			mEngine = NULL;
		}
	}

private:
	static SLObjectItf mEngineObj, mOutputMixObj;
	static SLEngineItf mEngine;
	static SLObjectItf mBGMPlayerObj;
	static SLPlayItf mBGMPlayer;
	static SLSeekItf mBGMPlayerSeek;

};

SLObjectItf Audio::mEngineObj, Audio::mOutputMixObj;
SLEngineItf Audio::mEngine;
SLObjectItf Audio::mBGMPlayerObj;
SLPlayItf Audio::mBGMPlayer;
SLSeekItf Audio::mBGMPlayerSeek;

#endif /* AUDIO_H_ */
