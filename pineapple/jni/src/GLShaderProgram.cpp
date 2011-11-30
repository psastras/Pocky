#include "../extern/GLShaderProgram.h"
#include "../extern/VSML.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;
GLShaderProgram::GLShaderProgram() {
    programId_ =  glCreateProgram();
}

GLShaderProgram::~GLShaderProgram() {
    while(!shaders_.empty()) {
	glDeleteShader(shaders_.back());
	shaders_.pop_back();
    }
    glDeleteProgram(programId_);
}

void printLog(GLuint obj) {
    int infologLength = 0;
    char infoLog[1024];
	if (glIsShader(obj))
		glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
    if (infologLength > 0)
		LOGE(infoLog);
    fflush(stdout);
}
void GLShaderProgram::loadShaderFromData(GLenum type, unsigned char *data) {
	stringstream ss;
	ss << data;
	this->loadShaderFromSource(type, ss.str());
}
struct InvalidChar
{
    bool operator()(char c) const {
        return (!isprint((unsigned)c) && c != '\n') || (unsigned)c == 26;
    }
};
void GLShaderProgram::loadShaderFromSource(GLenum type, std::string source) {

    stringstream ss;
   // ss << "#version 100 core" << endl;
    if(type == GL_FRAGMENT_SHADER)
	ss << "#define _FRAGMENT_" << endl;
    else if(type == GL_VERTEX_SHADER)
	ss << "#define _VERTEX_" << endl;
    ss << source;
    std::string str = ss.str();
    str.erase(std::remove_if(str.begin(),str.end(), InvalidChar()), str.end());
    LOGI("SHADER:\n%s", str.c_str());
    int length = str.length();
    const char *data = str.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);
    printLog(id);
    glAttachShader(programId_, id);
    shaders_.push_back(id);
}


bool GLShaderProgram::link() {
  //  glBindAttribLocation(programId_, 0, "in_Position"); // Bind a constant attribute location for positions of vertices
//    glBindAttribLocation(programId_, 1, "in_Normal"); // Bind another constant attribute location, this time for color
//    glBindAttribLocation(programId_, 2, "in_TexCoord"); // Bind another constant attribute location, this time for color
    glLinkProgram(programId_);
    return true;
}

void GLShaderProgram::bind(VSML *instance) {
    this->bind();
    instance->initUniformLocs(this->getUniformLocation("modelviewMatrix"),
			      this->getUniformLocation("projMatrix"));
    instance->matrixToUniform(VSML::MODELVIEW);
    instance->matrixToUniform(VSML::PROJECTION);
}
