/*
 * ShadersManager.h
 *
 *  Created on: 08.11.2012
 *      Author: Denis Zagayevskiy
 */

#ifndef SHADERSMANAGER_H_
#define SHADERSMANAGER_H_

#include <stdlib.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"

class ShadersManager {
public:
	static GLuint compileShader(GLenum shaderType, const char* pSource);
	static GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
};

#endif /* SHADERSMANAGER_H_ */
