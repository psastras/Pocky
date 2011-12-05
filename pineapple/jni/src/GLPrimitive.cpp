#include "../extern/GL.h"
#include "../extern/GLPrimitive.h"
#include "../extern/GLShaderProgram.h"
#include "../extern/VSML.h"
#include "../extern/Common.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GLPrimitive::GLPrimitive(Float3 &tess, Float3 &translate, Float3 &scale) : vertexId_(0), indexId_(0) {
    translate_ = translate;
    scale_ = scale;
    shader_ = 0;
}

GLPrimitive::~GLPrimitive() {
    if(vertexId_) glDeleteBuffers(1, &vertexId_);
    if(indexId_) glDeleteBuffers(1, &indexId_);


}

void GLPrimitive::draw(const std::string &shadername) {
	this->draw(Pineapple::GL::instance()->shader(shadername));
}

void GLPrimitive::draw(GLShaderProgram *program, int instances) {

    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    GLint ids[2] = {
		program->getAttributeLocation("in_Position"),
		program->getAttributeLocation("in_TexCoord")
    };
    if(ids[0] >= 0)
	glVertexAttribPointer(ids[0], 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid *)vOffset_);
    if(ids[1] >= 0)
	glVertexAttribPointer(ids[1], 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid *)tOffset_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(ids[0] >= 0) glEnableVertexAttribArray(ids[0]);
    if(ids[1] >= 0) glEnableVertexAttribArray(ids[1]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    if(ids[0] >= 0) glDisableVertexAttribArray(ids[0]);
    if(ids[1] >= 0) glDisableVertexAttribArray(ids[1]);

}

void GLPrimitive::draw(GLShaderProgram *program) {
	GLint ids[2] = {
		program->getAttributeLocation("in_Position"),
		program->getAttributeLocation("in_TexCoord")
	};
	glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
	if(ids[0] >= 0)
	glVertexAttribPointer(ids[0], 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid *)vOffset_);
	if(ids[1] >= 0)
	glVertexAttribPointer(ids[1], 3, GL_FLOAT, GL_FALSE, sizeof(GLVertex), (GLvoid *)tOffset_);
	if(ids[0] >= 0) glEnableVertexAttribArray(ids[0]);
	if(ids[1] >= 0) glEnableVertexAttribArray(ids[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glDrawElements(type_, idxCount_, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
}


GLQuad::GLQuad(Float3 tess, Float3 translate, Float3 scale) : GLPrimitive(tess, translate, scale) {
     this->tesselate(tess, translate, scale);
}


GLQuad::~GLQuad() {
}


void GLQuad::tesselate(Float3 tess, Float3 translate, Float3 scale) {

    if(vertexId_) glDeleteBuffers(1, &vertexId_);
    if(indexId_) glDeleteBuffers(1, &indexId_);


    type_ = GL_TRIANGLES;
    idxCount_ = 6 * tess.x * tess.y;
    Float3 delta = scale / tess;
    Float3 tdelta = 1.0 / tess;
    delta.z = 0;

    GLVertex *pVertex = new GLVertex[(int)((tess.x + 1) * (tess.y + 1))];
    for(int y=0, i=0; y<=tess.y; y++) {
	for(int x=0; x<=tess.x; x++, i++) {
	    pVertex[i].p = Float3(-0.5, -0.5, 0.0) * scale  + translate + delta * Float3(x, y, 0);
	    pVertex[i].t = Float3(x, tess.y - y, 0) * tdelta;
	}
    }
    glGenBuffers(1, &vertexId_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex)*((tess.x + 1) * (tess.y + 1)), &pVertex[0].p.x, GL_STATIC_DRAW);

    unsigned short *pIndices = new unsigned short[idxCount_];
    for(int y=0, i=0; y<tess.y; y++) {
	for(int x=0; x<tess.x; x++, i+=6) {
	   pIndices[i] = y*(tess.x+1)+x;
	   pIndices[i+1] = y*(tess.x+1)+x+1;
	   pIndices[i+2] = (y+1)*(tess.x+1)+x+1;

	   pIndices[i+3] = (y+1)*(tess.x+1)+x+1;
	   pIndices[i+4] = (y+1)*(tess.x+1)+x;
	   pIndices[i+5] = (y)*(tess.x+1)+x;

	}
    }

    glGenBuffers(1, &indexId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*idxCount_, &pIndices[0], GL_STATIC_DRAW);

    delete[] pVertex, delete[] pIndices;

    vOffset_ = 0;
    tOffset_ = 12;
}

GLPlane::GLPlane(Float3 tess, Float3 translate, Float3 scale) : GLPrimitive(tess, translate, scale) {
     this->tesselate(tess, translate, scale);
}

void GLPlane::tesselate(Float3 tess, Float3 translate, Float3 scale) {

    if(vertexId_) glDeleteBuffers(1, &vertexId_);
    if(indexId_) glDeleteBuffers(1, &indexId_);

    type_ = GL_TRIANGLES;
    idxCount_ = 6 * tess.x * tess.z;
    Float3 delta = scale / tess;
    Float3 tdelta = 1.0 / tess;
    delta.y = 0;
    GLVertex *pVertex = new GLVertex[(int)((tess.x + 1) * (tess.z + 1))];
    for(int z=0, i=0; z<=tess.z; z++) {
	for(int x=0; x<=tess.x; x++, i++) {
	    pVertex[i].p = Float3(-0.5, 0.0, -0.5) * scale + translate + delta * Float3(x, 0, z);
	    pVertex[i].t = Float3(x, z, 0) * tdelta;
	}
    }
    glGenBuffers(1, &vertexId_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex)*((tess.x + 1) * (tess.z + 1)), &pVertex[0].p.x, GL_STATIC_DRAW);

    unsigned short *pIndices = new unsigned short[idxCount_];
    for(int y=0, i=0; y<tess.z; y++) {
	for(int x=0; x<tess.x; x++, i+=6) {
	   pIndices[i] = y*(tess.x+1)+x;
	   pIndices[i+1] = y*(tess.x+1)+x+1;
	   pIndices[i+2] = (y+1)*(tess.x+1)+x+1;

	   pIndices[i+3] = y*(tess.x+1)+x;
	   pIndices[i+4] = (y+1)*(tess.x+1)+x+1;
	   pIndices[i+5] = (y+1)*(tess.x+1)+x;

	}
    }

    glGenBuffers(1, &indexId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*idxCount_, &pIndices[0], GL_STATIC_DRAW);

    delete[] pVertex, delete[] pIndices;

    vOffset_ = 0;
    tOffset_ = 12;
}

GLCircle::GLCircle(Float3 tess, Float3 translate, Float3 scale) : GLPrimitive(tess, translate, scale) {
     this->tesselate(tess, translate, scale);
}

GLCircle::~GLCircle() {

}

void GLCircle::tesselate(Float3 tess, Float3 translate, Float3 scale) {

    if(vertexId_) glDeleteBuffers(1, &vertexId_);
    if(indexId_) glDeleteBuffers(1, &indexId_);


    type_ = GL_LINE_LOOP;
    idxCount_ = tess.x;
    GLVertex *pVertex = new GLVertex[(int)(tess.x)];
    float r = scale.x / 2.f;

	for(int x=0; x<tess.x; x++) {
		float xx = r * cosf(x / tess.x * 2 * 3.14159f + 3.14159f*0.5f);
		float yy = r * sinf(x / tess.x * 2 * 3.14159f + 3.14159f*0.5f);
		pVertex[x].p = Float3(xx + translate.x,yy+translate.y, translate.z);
		//pVertex[i].t = Float3(x, z, 0) * tdelta;
	}
    glGenBuffers(1, &vertexId_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex)*(tess.x), &pVertex[0].p.x, GL_STATIC_DRAW);

    unsigned short *pIndices = new unsigned short[idxCount_];

	for(int x=0; x<tess.x; x++) {
	   pIndices[x] = x;
	}

    glGenBuffers(1, &indexId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*idxCount_, &pIndices[0], GL_STATIC_DRAW);

    delete[] pVertex, delete[] pIndices;

    vOffset_ = 0;
    tOffset_ = 12;
}

GLDisc::GLDisc(Float3 tess, Float3 translate, Float3 scale) : GLPrimitive(tess, translate, scale) {
     this->tesselate(tess, translate, scale);
}

GLDisc::~GLDisc() {

}

void GLDisc::tesselate(Float3 tess, Float3 translate, Float3 scale) {

    if(vertexId_) glDeleteBuffers(1, &vertexId_);
    if(indexId_) glDeleteBuffers(1, &indexId_);


    type_ = GL_TRIANGLE_FAN;
    idxCount_ = tess.x + 2;
    GLVertex *pVertex = new GLVertex[(int)(tess.x)+2];
    float r = scale.x / 2.f;
    pVertex[0].p = translate;
	for(int x=0; x<=tess.x; x++) {
		float xx = r * cosf(x / tess.x * 2 * 3.14159f + 3.14159f*0.5f);
		float yy = r * sinf(x / tess.x * 2 * 3.14159f + 3.14159f*0.5f);
		pVertex[x+1].p = Float3(xx + translate.x,yy+translate.y, translate.z);
		//pVertex[i].t = Float3(x, z, 0) * tdelta;
	}
    glGenBuffers(1, &vertexId_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex)*(tess.x+2), &pVertex[0].p.x, GL_STATIC_DRAW);

    unsigned short *pIndices = new unsigned short[idxCount_];

	for(int x=0; x<idxCount_; x++) {
	   pIndices[x] = x;
	}

    glGenBuffers(1, &indexId_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*idxCount_, &pIndices[0], GL_STATIC_DRAW);

    delete[] pVertex, delete[] pIndices;

    vOffset_ = 0;
    tOffset_ = 12;
}

//
//GLHexagon::GLHexagon(Float3 tess, Float3 translate, Float3 scale) : GLPrimitive(tess, translate, scale) {
//     this->tesselate(tess, translate, scale);
//}
//
//void GLHexagon::tesselate(Float3 tess, Float3 translate, Float3 scale) {
//
//    if(vertexId_) glDeleteBuffers(1, &vertexId_);
//    if(indexId_) glDeleteBuffers(1, &indexId_);
//
//
//    type_ = GL_LINE_LOOP;
//    idxCount_ = tess.x;
//    GLVertex *pVertex = new GLVertex[(int)(tess.x)];
//    float r = scale.x;
//
//	for(int x=0; x<tess.x; x++) {
//		float xx = r * cosf(x / tess.x * 2 * 3.14159f);
//		float yy = r * sinf(x / tess.x * 2 * 3.14159f);
//		pVertex[x].p = Float3(xx,0, yy);
//		//pVertex[i].t = Float3(x, z, 0) * tdelta;
//	}
//    glGenBuffers(1, &vertexId_);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexId_);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex)*(tess.x), &pVertex[0].p.x, GL_STATIC_DRAW);
//
//    unsigned short *pIndices = new unsigned short[idxCount_];
//
//	for(int x=0; x<tess.x; x++) {
//	   pIndices[x] = x;
//	}
//
//    glGenBuffers(1, &indexId_);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId_);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*idxCount_, &pIndices[0], GL_STATIC_DRAW);
//
//    delete[] pVertex, delete[] pIndices;
//
//    vOffset_ = 0;
//    tOffset_ = 12;
//}
