/*
 * PineappleGLSurface.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/GL.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


namespace Pineapple {

	GL::GL() {

	}

	GL::GL(int w, int h) {
		initializeGL(w, h);
	}

	GL::~GL() {
		// TODO Auto-generated destructor stub
	}

	void GL::initializeGL(int w, int h) {
		width_ = w;
		height_ = h;

		glViewport(0, 0, w, h);

	}


	void GL::test() {
		LOGI("ASDASDASD");
	}
}
