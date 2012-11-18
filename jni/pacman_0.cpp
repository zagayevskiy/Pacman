/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  LOG_TAG    "pacman"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
/*
static const char gVertexShader[] =
    "attribute vec4 vPosition;\n"
	"attribute vec3 vColor;\n"
	"varying vec4 color;\n"
    "void main() {\n"
    "   gl_Position = vPosition;\n"
		"color = vec4(vColor, 1.0);\n"
    "}\n";

static const char gFragmentShader[] =
    "precision mediump float;\n"
	"varying vec4 color;\n"
    "void main() {\n"
    "  gl_FragColor = color;\n"
    "}\n";
*/

/*static const char gVertexShader[] =
    "attribute vec4 aPosition;\n"
	"attribute vec2 aTexture;\n"
	"varying vec2 vTexture;\n"
    "void main() {\n"
	"	vTexture = aTexture;\n"
    "   gl_Position = aPosition;\n"
    "}\n";
*/

static const char gVertexShader[] =
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = /*gl_FragColor = texture2D(map, vTexture)*/
    "precision mediump float;\n"
	"varying vec2 vTexture;\n"
	"uniform sampler2D map;\n"
    "void main() {\n"
    "   gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttach vertex Shader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttach fragment Shader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
GLuint textureHandle;
GLuint mapHandle;
GLuint colorPositionHandle;

GLfloat* verticesArray;
GLfloat* textureArray;
GLshort* trianglesIndicesArray;
GLint squaresCountX, squaresCountY;
GLint verticesCountX, verticesCountY, verticesCount;
GLint trianglesCountX, trianglesCountY, trianglesCount;
GLuint verticesBuffer, indicesBuffer;
GLuint textureId;
GLuint* texArray;
GLuint textureWidth, textureHeight;

bool setupGraphics(int w, int h) {

	glEnable(GL_ALPHA);
	squaresCountX = squaresCountY = 20;
	verticesCountX = squaresCountX + 1;
	verticesCountY = squaresCountY + 1;
	verticesCount = verticesCountX*verticesCountY;
	trianglesCount = squaresCountX*squaresCountY;

	verticesArray = (GLfloat*) malloc(2*verticesCount*sizeof(GLfloat));
	textureArray = (GLfloat*) malloc(3*trianglesCount*sizeof(GLfloat));
	trianglesIndicesArray = (GLshort*) malloc(2*3*trianglesCount*sizeof(GLshort));

	GLfloat deltaX = 2.0 / squaresCountX;
	GLfloat deltaY = -2.0 /squaresCountY;
	GLfloat initX = -1.0, initY = 1.0;
	GLfloat currentX = initX, currentY = initY;

	/*
	for(int y = 0; y < verticesCountY; ++y){
		LOGI("%i-----------------------------------------", y);
		int offset = y*verticesCountX;
		currentX = -1.0;
		for(int x = 0; x < verticesCountX*2; x += 2){
			verticesArray[offset + x] = currentX;
			verticesArray[offset + x + 1] = currentY;
			currentX += deltaX;
			LOGI("%f, %f", verticesArray[offset + x], verticesArray[offset + x + 1]);
		}
		currentY -= deltaY;
	}*/

	for(int i = 0; i < verticesCountY; ++i){
		currentX = initX;
		int offset = i*verticesCountX*2;
		for(int j = 0; j < verticesCountX*2; j += 2){
			verticesArray[offset + j] = currentX;
			verticesArray[offset + j + 1] = currentY;
			currentX += deltaX;
		}
		currentY += deltaY;
	}

	/*for(int i = 0; i < 2*verticesCount; i += 2){
		LOGI("vertex[%i]: %f, %f", i / 2, verticesArray[i], verticesArray[i + 1]);
	}*/

	/*GLshort number = 0;
	for(int k = 0; k < 3*squaresCountX*squaresCountY; k += 3){
		trianglesIndicesArray[k] = number;
		trianglesIndicesArray[k + 1] = number + 1;
		trianglesIndicesArray[k + 2] = number + 1 + verticesCountX;
		LOGE("triangle[%i]: %i, %i, %i", number, trianglesIndicesArray[k], trianglesIndicesArray[k + 1], trianglesIndicesArray[k + 2]);
		++number;
	}*/

	GLshort number = 0;
	for(int i = 0; i < squaresCountY; ++i){
		int offset = i*3*squaresCountX;
		for(int j = 0; j < 3*squaresCountX; j += 3){
			trianglesIndicesArray[offset + j] = number;
			trianglesIndicesArray[offset + j + 1] = number + 1;
			trianglesIndicesArray[offset + j + 2] = number + 1 + verticesCountX;
			++number;
		}
		++number;
	}

	for(int i = 0; i < squaresCountX*squaresCountY*3; i +=3 ){
		LOGI("triangle[%i]: %i, %i, %i", i / 3, trianglesIndicesArray[i], trianglesIndicesArray[i + 1], trianglesIndicesArray[i + 2]);
	}

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"aPosition\") = %d\n", gvPositionHandle);
/*
    textureHandle = glGetAttribLocation(gProgram, "aTexture");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"aTexture\") = %d\n", textureHandle);
    mapHandle = glGetUniformLocation(gProgram, "map");
    checkGlError("glGetUnifirmLocation");
    LOGI("glGetUnifirmLocation(\"map\") = %d\n", mapHandle);
*/

    /*colorPositionHandle = glGetAttribLocation(gProgram, "vColor");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vColor\") = %d\n", colorPositionHandle);
*/
    glViewport(0, 0, w, h);
    checkGlError("glViewport");


    glGenBuffers(1, &verticesBuffer);
    checkGlError("glGenBuffers(1, &verticesBuffer);");
	glGenBuffers(1, &indicesBuffer);
	checkGlError("glGenBuffers(1, &indicesBuffer);");

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
	checkGlError("glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	checkGlError("glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);");

	glBufferData(GL_ARRAY_BUFFER, verticesCount*2*sizeof(GLfloat), verticesArray, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ARRAY_BUFFER, verticesCount*2, verticesArray, GL_STATIC_DRAW);");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, trianglesCount*3*sizeof(GLshort), trianglesIndicesArray, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9*sizeof(GLshort), testIndices, GL_STATIC_DRAW);
	checkGlError("glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesCount*3, trianglesIndicesArray, GL_STATIC_DRAW);");

	glGenTextures(1, &textureId);

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA ,GL_UNSIGNED_BYTE, texArray);

	glUniform1i(mapHandle, 0);

    return true;
}

const GLfloat gTriangleVertices[] = {
		0.0f, 0.5f, -0.5f, -1.0f, 0.5f, -1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f
};

GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
};

float colorDelta = 0.01f;
float delta[] = {colorDelta, -colorDelta, colorDelta};
int indexes[] = {1, 2, 0};

void renderFrame() {



	/*for(int i = 0; i < 3; ++i){
		colors[i*3 + indexes[i]] += delta[i];

		if(colors[i*3 + indexes[i]] > 1.0f - (colorDelta / 2.0f) || colors[i*3 + indexes[i]] < (colorDelta / 2.0f)){
			delta[i] = -delta[i];
			indexes[i] = indexes[i] == 0 ? 2 : indexes[i] - 1;
		}

	}*/

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    checkGlError("glClearColor");

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    /*glUseProgram(gProgram);
	checkGlError("glUseProgram");

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 3);
	checkGlError("glDrawArrays");*/


    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    void* offset = (void*) 0;

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, offset);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	//glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, textureArray);
	//checkGlError("glVertexAttribPointer texture");
	//glEnableVertexAttribArray(textureHandle);
	//checkGlError("glEnableVertexAttribArray");

	glDrawElements(GL_TRIANGLES, 3*trianglesCount, GL_UNSIGNED_SHORT, offset);
	checkGlError("glDrawElements");

/*
    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, verticesArray);
    checkGlError("glVertexAttribPointer");

    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");

    //glVertexAttribPointer(colorPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, colors);
    //checkGlError("glVertexAttribPointer");
    //glEnableVertexAttribArray(colorPositionHandle);
    //checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_LINE_STRIP, 0, verticesCount);
    checkGlError("glDrawArrays");
*/
}

extern "C" {

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_init(JNIEnv* env, jobject obj, jint width, jint height){
		/*
		jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir", "()Ljava/io/File;");
			jobject file = env->CallObjectMethod(state_param->activity->clazz, getCacheDir);
			jclass fileClass = env->FindClass("java/io/File");
			jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
			jstring jpath = (jstring)env->CallObjectMethod(file, getAbsolutePath);
			const char* app_dir = env->GetStringUTFChars(jpath, NULL);

			// chdir in the application cache directory
			LOGI("app_dir: %s", app_dir);
			chdir(app_dir);
			env->ReleaseStringUTFChars(jpath, app_dir);
		*/

		setupGraphics(width, height);
	}


	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_step(JNIEnv* env, jobject obj){
		renderFrame();
	}

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_initPngManager(JNIEnv* env, jobject obj, jobject pngManager){

		LOGE("begin loading");
		char const *path = "tex.png";

		jclass cls = env->GetObjectClass(pngManager);
		jmethodID mid;

		LOGE("Ask the PNG manager for a bitmap...");
		mid = env->GetMethodID(cls, "open", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
		jstring name = env->NewStringUTF(path);
		jobject png = env->CallObjectMethod(pngManager, mid, name);
		env->DeleteLocalRef(name);
		env->NewGlobalRef(png);

		LOGE("Get image dimensions...");
		mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
		jint width = textureWidth = env->CallIntMethod(pngManager, mid, png);
		LOGE("Image width: %i", width);
		mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
		jint height = textureHeight = env->CallIntMethod(pngManager, mid, png);
		LOGE("Image height: %i", height);

		LOGE("Get pixels...");
		jintArray array = env->NewIntArray(width * height);
		env->NewGlobalRef(array);
		mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
		env->CallVoidMethod(pngManager, mid, png, array);

		jint *pixels = env->GetIntArrayElements(array, 0);

		texArray = (unsigned int*)pixels;
		LOGE("end loading! yahooo!!");
/*
		env->ReleaseIntArrayElements(array, pixels, 0);
		env->DeleteGlobalRef(array);

		mid = env->GetMethodID(cls, "close", "(Landroid/graphics/Bitmap;)V");
		env->CallVoidMethod(pngManager, mid, png);
		env->DeleteGlobalRef(png);*/
	}

}
