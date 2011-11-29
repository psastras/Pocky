/*
 * PineappleGLSurface.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef PINEAPPLEGLSURFACE_H_
#define PINEAPPLEGLSURFACE_H_
#include "Common.h"
#include "GLPrimitive.h"
#include "GLShaderProgram.h"
#include "VSML.h"

namespace Pineapple {

class GLShader;

struct GLObject {
	GLShader *shader;

};

class GL {
	public:
	GL();
	GL(int w, int h);
		virtual ~GL();

		void test();

		void initializeGL(int w, int h);
	protected:
		int width_, height_;
	};
}
#endif /* PINEAPPLEGLSURFACE_H_ */
