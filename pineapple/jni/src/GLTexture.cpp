/*
 * GLTexture.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: psastras
 */

#include "../extern/GLTexture.h"

namespace Pineapple {

GLTexture::GLTexture(GLTextureParams params, unsigned char *data) : id_(0) {
	this->init(params, data);
}

GLTexture::GLTexture(GLTextureParams params) : id_(0) {
	this->init(params);
}

void GLTexture::init(GLTextureParams params)
{
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height, 0, params.format, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::init(GLTextureParams params, unsigned char *data)
{
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height, 0, params.format, GL_UNSIGNED_BYTE, data);
	if(params.mipmap) glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::releaseGL()
{
	if(id_)
		glDeleteTextures(1, &id_);
}

GLTexture::~GLTexture() {
	this->releaseGL();
}

} /* namespace Pineapple */
