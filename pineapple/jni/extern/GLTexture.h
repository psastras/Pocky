/*
 * GLTexture.h
 *
 *  Created on: Nov 30, 2011
 *      Author: psastras
 */

#ifndef GLTEXTURE_H_
#define GLTEXTURE_H_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace Pineapple {

struct GLTextureParams {
	int width, height;
	GLuint format;
	bool mipmap;
};

class GLTexture {
public:
	GLTexture(GLTextureParams);
	GLTexture(GLTextureParams, unsigned char *data);
	virtual ~GLTexture();

	GLuint id() { return id_; }
	void init(GLTextureParams);
	void init(GLTextureParams, unsigned char *data);

	void bind() { glBindTexture(GL_TEXTURE_2D, id_); }
	void release() { glBindTexture(GL_TEXTURE_2D, 0); }

	void releaseGL();

protected:
	GLuint id_;
};

} /* namespace Pineapple */
#endif /* GLTEXTURE_H_ */
