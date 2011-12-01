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
#include "GLFramebufferObject.h"
#include "GLTexture.h"
#include "VSML.h"
#include "../include/Fonts.h"
#include <unordered_map>
namespace Pineapple {

class GLShader;

struct GLObject {
	GLShader *shader;

};

class GL {
public:
	static void init(int w, int h) {
		if(!s_instance)	s_instance = new GL(w, h);
	}

	static void init() {
			if(!s_instance)	s_instance = new GL();
		}

	static GL *instance() {
		return s_instance;
	}

	virtual ~GL();

	void test();

	void createShader(const std::string &name, const char *filename);
	GLShaderProgram *shader(const std::string &name) { return shaders_[name]; }

	void loadFont(FONTS font);
	void releaseFont(FONTS font);
	GLTexture *fontTexture(FONTS font) { return fontTextures_[font]; }

	void createTexture(const std::string &name, GLTextureParams &parms, unsigned char *data);
	GLTexture *texture(const std::string &name) { return textures_[name]; }

	void initializeGL(int w, int h);
	void ortho();
	void perspective(float fov, float near, float far);

	inline int width() { return width_; }
	inline int height() { return height_; }
protected:
		GL();
		GL(int w, int h);
		int width_, height_;
		static GL *s_instance;
		std::unordered_map<std::string, GLShaderProgram *> shaders_;
		std::unordered_map<std::string, GLPrimitive *> primitives_;
		std::unordered_map<std::string, GLTexture *> textures_;
		std::unordered_map<char, int> letterIdxs_;

		GLTexture *fontTextures_[NUMFONTS];
	};
}
#endif /* PINEAPPLEGLSURFACE_H_ */
