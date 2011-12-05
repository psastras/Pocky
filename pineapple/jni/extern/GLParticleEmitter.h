/*
 * GLParticleEmitter.h
 *
 *  Created on: Dec 4, 2011
 *      Author: psastras
 */

#ifndef GLPARTICLEEMITTER_H_
#define GLPARTICLEEMITTER_H_

#include "GLPrimitive.h"
#include "GLShaderProgram.h"

namespace Pineapple {

class GLParticleEmitter {
public:
	GLParticleEmitter(GLPrimitive *primitive, GLShaderProgram *program, int nParticles = 100);
	virtual ~GLParticleEmitter();

	void draw(int time);
	void update(int time);

protected:
	GLPrimitive *primitive_;
	GLShaderProgram *shaderprogram_;

	struct ParticleData {
		Float3 pos;
		Float3 vel;
		float life;
	};

	int numParticles_;
	ParticleData *particleData_;

};

} /* namespace Pineapple */
#endif /* GLPARTICLEEMITTER_H_ */
