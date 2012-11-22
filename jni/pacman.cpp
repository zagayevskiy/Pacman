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



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"

#include "logic/Engine.h"
#include "graphics/ui/RectButton.h"
#include "graphics/ui/menu/MainMenu.h"
#include "graphics/IRenderable.h"
#include "managers/Art.h"
#include "managers/ShadersManager.h"
#include "templates/list.h"

/*
#define  LOG_TAG    "pacman"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
*/
static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}


/*static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}*/

static double now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}

GLuint loadShader(GLenum shaderType, const char* pSource, const long len) {
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

GLuint createProgram(const char* pVertexSource, long vertLen, const char* pFragmentSource, long fragLen) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource, vertLen);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource, fragLen);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
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
GLuint vertexHandle, textureHandle, mapHandle, matrixHandle;

GLuint textureId;

GLuint textureWidth, textureHeight;
char* textureArray = NULL;
/*
static const char vertexShader[] =
    "attribute vec4 aPosition;\n"
	"attribute vec2 aTexture;\n"
	"uniform mat4 uMatrix;\n"
	"varying vec2 vTexture;\n"
    "void main() {\n"
	"	vTexture = aTexture;\n"
    "	gl_Position = uMatrix * vec4(aPosition.x, aPosition.y, 0.5, aPosition.w);\n"
    "}";

static const char fragmentShader[] =
    "precision mediump float;\n"
	"varying vec2 vTexture;\n"
	"uniform sampler2D uMap;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(uMap, vTexture);\n"
    "}";
*/

char* vertexShader;
char* fragmentShader;
long vLength, fLength;

//const GLfloat trVerts[] = { 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, -0.5f };

//GLfloat* gTriangleVertices;

GLfloat gTriangleVertices[] = {
		0.0, 0.0, 0.5, 0.0, 0.5, 0.5,
		0.5, 0.5, 0.0, 0.5, 0.0, 0.0
};

const GLfloat textureCoords[] = //{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
{
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0
};

GLfloat gTriangleVertices1[] = {
		0.0, 0.0, 0.7, 0.0, 0.7, 1.0
};

GLfloat textureCoords1[] = //{ 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
{
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0
};

GLfloat* matrix = (GLfloat*) malloc(16*sizeof(GLfloat));


float degreesToRadians(float angle){
	return angle*180.0/3.14159565;
}


RectButton *btn;

GLuint artTex;
MainMenu* menu;
bool setupGraphics(int w, int h) {

	// These paramaters are lens properties.
	// The "near" and "far" create the Depth of Field.
	// The "left", "right", "bottom" and "top" represent the rectangle formed
	// by the near area, this rectangle will also be the size of the visible area.
	float near = 1.0, far = -1.0;
	//float left = -1.0, right = 1.0, bottom = -1.0, top = 1.0;
	float left = 0.0, right = 1.0f / (float) h * (float) w, bottom = 1.0, top = 0.0;

	// First Column
	matrix[0] = 2.0 / (right - left);
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;

	// Second Column
	matrix[4] = 0.0;
	matrix[5] = 2.0 / (top - bottom);
	matrix[6] = 0.0;
	matrix[7] = 0.0;

	// Third Column
	matrix[8] = 0.0;
	matrix[9] = 0.0;
	matrix[10] = -2.0 / (far - near);
	matrix[11] = 0.0;

	// Fourth Column
	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(far + near) / (far - near);
	matrix[15] = 1;

/*
	matrix[0] = 1.0;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;

	// Second Column
	matrix[4] = 0.0;
	matrix[5] = 1.0;
	matrix[6] = 0.0;
	matrix[7] = 0.0;

	// Third Column
	matrix[8] = 0.0;
	matrix[9] = 0.0;
	matrix[10] = 1.0;
	matrix[11] = 0.0;

	// Fourth Column
	matrix[12] = 0.0;//-(right + left) / (right - left);
	matrix[13] = 0.0;//-(top + bottom) / (top - bottom);
	matrix[14] = 0.0;//-(far + near) / (far - near);
	matrix[15] = 1.0;
*/
/*
	float near = 0.001, far = 100.0;
	    float angleOfView = 45.0;
	    float aspectRatio = 0.75;

	    // Some calculus before the formula.
	    float size = near * tanf(degreesToRadians(angleOfView) / 2.0);
	    float left = -size, right = size, bottom = -size / aspectRatio, top = size / aspectRatio;

	    // First Column
	    matrix[0] = 2 * near / (right - left);
	    matrix[1] = 0.0;
	    matrix[2] = 0.0;
	    matrix[3] = 0.0;

	    // Second Column
	    matrix[4] = 0.0;
	    matrix[5] = 2 * near / (top - bottom);
	    matrix[6] = 0.0;
	    matrix[7] = 0.0;

	    // Third Column
	    matrix[8] = (right + left) / (right - left);
	    matrix[9] = (top + bottom) / (top - bottom);
	    matrix[10] = -(far + near) / (far - near);
	    matrix[11] = -1;

	    // Fourth Column
	    matrix[12] = 0.0;
	    matrix[13] = 0.0;
	    matrix[14] = -(2 * far * near) / (far - near);
	    matrix[15] = 0.0;
*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//gTriangleVertices = (GLfloat*) malloc(6*sizeof(float));
/*
	for(int i = 0; i < 6; i += 2){
		gTriangleVertices[i] = trVerts[i];
		gTriangleVertices[i + 1] = trVerts[i + 1] * ((GLfloat) w) / ((GLfloat) h);
	}*/

    LOGI("setupGraphics(%d, %d)", w, h);

    char* vsh = Art::getShaderSource(Art::SHADER_VERTEX_0);
    char* fsh = Art::getShaderSource(Art::SHADER_FRAGMENT_0);
    gProgram = ShadersManager::createProgram(vsh, fsh);//createProgram(vertexShader, vLength, fragmentShader, fLength);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

    vertexHandle = glGetAttribLocation(gProgram, "aPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"aPosition\") = %d\n", vertexHandle);

    textureHandle = glGetAttribLocation(gProgram, "aTexture");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"aTexture\") = %d\n", textureHandle);

	mapHandle = glGetUniformLocation(gProgram, "uMap");
	checkGlError("glGetUniformLocation");
	LOGI("glGetAttribLocation(\"uMap\") = %d\n", mapHandle);

	matrixHandle = glGetUniformLocation(gProgram, "uMatrix");
	checkGlError("glGetUniformLocation");
	LOGI("glGetAttribLocation(\"uMatrix\") = %d\n", matrixHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    /************************************************************************************/
   // glActiveTexture(GL_TEXTURE0);

    Art::generateTextures();
    artTex = Art::getTexture(Art::TEXTURE_GRAD);
/*
	glGenTextures(1, &textureId);
	LOGI("Generated texture id = %i", textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	LOGI("w = %d, h = %d", textureWidth, textureHeight);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureArray);

	checkGlError("glTexImage2D");*/

    /******************************************************************************************************/


    return true;
}



void renderFrame() {
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glBindTexture(GL_TEXTURE_2D, artTex);
    glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(vertexHandle);
    checkGlError("glEnableVertexAttribArray");

    // The {0,1} correspond to the activated textures units.

    // Sets the texture units to an uniform.
    glUniform1i(mapHandle, 0);
    checkGlError("glUniform1i");

    glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, matrix);
    checkGlError("glUniformMatrix4fv");

    glVertexAttribPointer(textureHandle, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(textureHandle);
    checkGlError("glEnableVertexAttribArray");

    glDrawArrays(GL_TRIANGLES, 0, 6);
    checkGlError("glDrawArrays");

    /******************/
    glVertexAttribPointer(vertexHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices1);
	checkGlError("glVertexAttribPointer");
	glEnableVertexAttribArray(vertexHandle);
	checkGlError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLES, 0, 6);
	checkGlError("glDrawArrays");

	menu->render(0);

}

Engine* engine = new Engine();
extern "C" {

	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_init(JNIEnv* env, jobject obj, jint width, jint height, jobject pngManager, jobject assetManager){
		//setupGraphics(width, height);
		Art::init(env, pngManager, assetManager);
		engine->init(width, height);
	}


	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_step(JNIEnv* env, jobject obj){
		//renderFrame();
		engine->render(0.0);
	}
/*
	JNIEXPORT void JNICALL Java_com_zagayevskiy_pacman_PacmanLib_initResourceManager(JNIEnv* env, jobject obj, jobject pngManager, jobject assetManager){

		Art::init(env, pngManager, assetManager);
		ResourceManager::init(env, pngManager, assetManager);
		Texture* tex = ResourceManager::loadTexture("tex2.png");
		textureWidth = tex->width;
		textureHeight = tex->height;
		textureArray = tex->pixels;

	}
*/
}
