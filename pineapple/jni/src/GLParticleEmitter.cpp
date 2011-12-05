/*
 * GLParticleEmitter.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: psastras
 */

#include "../extern/GLParticleEmitter.h"

namespace Pineapple {

GLParticleEmitter::GLParticleEmitter(GLPrimitive *primitive, GLShaderProgram *program, int nParticles) :
	primitive_(primitive), shaderprogram_(program), numParticles_(nParticles) {

	particleData_ = new ParticleData[nParticles];

	for(int i=0; i<nParticles; i++)
	{
		particleData_[i].life = 0.f;
	}

}

GLParticleEmitter::~GLParticleEmitter() {
	delete[] particleData_;
}

void GLParticleEmitter::draw(int time) {
	for(int i=0; i<numParticles_; i++)
	{
		ParticleData *pdata = &particleData_[i];
		if(pdata->life <= 0.f) // if needs respawn
		{
			pdata->life = 1.f;
			pdata->pos.zero();
		}

		primitive_->draw(shaderprogram_);
	}
}

void GLParticleEmitter::update(int time) {

} /* namespace Pineapple */
}
