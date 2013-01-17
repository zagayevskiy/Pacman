/*
 * Store.cpp
 *
 *  Created on: 16.01.2013
 *      Author: Denis
 */

#include "Store.h"

JavaVM* Store::javaVM = NULL;
jobject Store::storeManager = NULL;
jclass Store::storeManagerClass = NULL;
jmethodID Store::saveBoolId = NULL;
jmethodID Store::loadBoolId = NULL;
jmethodID Store::saveIntId = NULL;
jmethodID Store::loadIntId = NULL;

void Store::init(JNIEnv* env, jobject _storeManager){
	LOGI("Store::init");
	if(env->GetJavaVM(&javaVM) != JNI_OK){
		LOGE("Can not Get JVM");
		return;
	}

	storeManager = env->NewGlobalRef(_storeManager);
	if(!storeManager){
		LOGE("Can not create NewGlobalRef on storeManager");
		return;
	}
	storeManagerClass = env->GetObjectClass(storeManager);
	if(!storeManagerClass){
		LOGE("Can not get StoreManager class");
		return;
	}

	saveBoolId = env->GetMethodID(storeManagerClass, "saveBoolean", "(Ljava/lang/String;Z)V");
	if(!saveBoolId){
		LOGE("Can not find method saveBoolean");
		return;
	}

	loadBoolId = env->GetMethodID(storeManagerClass, "loadBoolean", "(Ljava/lang/String;Z)Z");
	if(!loadBoolId){
		LOGE("Can not find method loadBoolean");
		return;
	}

	saveIntId = env->GetMethodID(storeManagerClass, "saveInt", "(Ljava/lang/String;I)V");
	if(!saveIntId){
		LOGE("Can not find method saveInt");
		return;
	}

	loadIntId = env->GetMethodID(storeManagerClass, "loadInt", "(Ljava/lang/String;I)I");
	if(!loadIntId){
		LOGE("Can not find method loadInt");
		return;
	}
}

void Store::saveBool(const char* name, bool value){
	LOGI("Store::saveBool(%s, %d)", name, value);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not getJNIEnv");
		return;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	env->CallVoidMethod(storeManager, saveBoolId, key, value);
}

bool Store::loadBool(const char* name, bool defValue){
	LOGI("Store::loadBool(%s, %d)", name, defValue);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not getJNIEnv");
		return defValue;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	return env->CallBooleanMethod(storeManager, loadBoolId, key, defValue);
}

void Store::saveInt(const char* name, int value){
	LOGI("Store::saveINt(%s, %d)", name, value);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not getJNIEnv");
		return;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	env->CallVoidMethod(storeManager, saveIntId, key, value);
}

int Store::loadInt(const char* name, int defValue){
	LOGI("Store::loadInt(%s, %d)", name, defValue);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not getJNIEnv");
		return defValue;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	return env->CallIntMethod(storeManager, loadIntId, key, defValue);
}


JNIEnv* Store::getJNIEnv(JavaVM* jvm){
	JavaVMAttachArgs args;
	args.version = JNI_VERSION_1_6;
	args.name = "PacmanNativeThread";
	args.group = NULL;
	JNIEnv* result;
	if(jvm->AttachCurrentThread(&result, &args) != JNI_OK){
		result = NULL;
	}
	return result;
}
