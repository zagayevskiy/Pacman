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
		LOGE("Can not find method SaveBoolean");
		return;
	}

	loadBoolId = env->GetMethodID(storeManagerClass, "loadBoolean", "(Ljava/lang/String;Z)Z");
	if(!loadBoolId){
		LOGE("Can not find method loadBoolean");
		return;
	}
}

void Store::saveBool(const char* name, bool value){
	LOGI("Store::saveBool(%s, %d)", name, value);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not save boolean value");
		return;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	env->CallVoidMethod(storeManager, saveBoolId, key, value);

	//javaVM->DetachCurrentThread();
}

bool Store::loadBool(const char* name, bool defValue){
	LOGI("Store::loadBool(%s, %d)", name, defValue);
	JNIEnv* env = getJNIEnv(javaVM);

	if(!env){
		LOGE("Can not save boolean value");
		return defValue;
	}

	jstring key = env->NewStringUTF(name);
	if(!key){
		LOGE("Can not create NewStringUTF");
	}

	env->CallBooleanMethod(storeManager, loadBoolId, key, defValue);

	//javaVM->DetachCurrentThread();
}

JNIEnv* Store::getJNIEnv(JavaVM* jvm){
	JavaVMAttachArgs args;
	args.version = JNI_VERSION_1_6;
	args.name = "NativeThread";
	args.group = NULL;
	JNIEnv* result;
	if(jvm->AttachCurrentThread(&result, &args) != JNI_OK){
		result = NULL;
	}
	return result;
}
