
#include "../extern/GLFramebufferObject.h"
#include "../extern/Common.h"

GLFramebufferObject::GLFramebufferObject(GLFramebufferObjectParams params) {
    color_ = 0;
    params_ = params;
    // @todo: should check the parameters to maker sure they make sense
    // instead were just going to check for a glError, if one of the parameters
    // is not correct it will probably result in a gl error

    this->allocFramebuffer(params_);
   // GLERROR("creating framebuffer object");

}

GLFramebufferObject::~GLFramebufferObject() {
    this->bind();
	glDeleteTextures(1, &color_[0]);
	glDeleteTextures(1, &depth_);
    this->release();
    glDeleteFramebuffers(1, &id_);
    if(color_) delete[] color_;
}

void GLFramebufferObject::bindsurface(int idx) {
	glBindTexture(GL_TEXTURE_2D, color_[idx]);

}

// @todo: add stencil buffer support and error handling (esp. for nonsupported formats)
void GLFramebufferObject::allocFramebuffer(GLFramebufferObjectParams &params) {

    glGenFramebuffers(1, &id_);

    this->bind();

    if(params.type == GL_TEXTURE_2D)
	color_ = new GLuint[1];

//create regular targets

	    glGenTextures(1, &color_[0]);

	    for(int i=0; i<1; i++) {
		glBindTexture(params.type, color_[i]);
		glTexParameterf(params.type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(params.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height, 0, GL_LUMINANCE, GL_FLOAT, 0);
	        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_[i], 0);
	    }

	    glBindTexture(params.type, 0);



    if(params.hasDepth) {

	    glGenTextures(1, &depth_);
	    glBindTexture(params.type, depth_);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	    glTexImage2D(params.type, 0, params.depthFormat, params.width, params.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_, 0);
	    glBindTexture(params.type, 0);

    }

    this->release();
}

GLuint* GLFramebufferObject::texture() {
    return color_;
}

GLuint GLFramebufferObject::depth() {
    return depth_;
}

void GLFramebufferObject::bind() {

     glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

void GLFramebufferObject::release() {
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebufferObject::resize(int width, int height) {
    if(params_.width == width &&  params_.height == height) return;
    params_.width = width;
    params_.height = height;

    this->bind();
    //create regular targets
	for(int i=0; i<1; i++) {
	    glBindTexture(GL_TEXTURE_2D, color_[i]);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexImage2D(GL_TEXTURE_2D, 0, params_.format, params_.width, params_.height, 0, GL_LUMINANCE, GL_FLOAT, 0);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);



    if(params_.hasDepth) {
	    glBindTexture(GL_TEXTURE_2D, depth_);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexImage2D(GL_TEXTURE_2D, 0, params_.depthFormat, params_.width, params_.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_, 0);
	    glBindTexture(GL_TEXTURE_2D, 0);

    }
    this->release();
}
