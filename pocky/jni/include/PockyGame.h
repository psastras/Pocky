/*
 * PockyGame.h
 *
 *  Created on: Dec 4, 2011
 *      Author: psastras
 */

#ifndef POCKYGAME_H_
#define POCKYGAME_H_

#include <pineapple/jni/extern/Float3.h>
#include <pineapple/jni/extern/GL.h>

#define MAX_ACTIVE 10 //if you change this make sure to change the shader as well

namespace Pocky {

struct PockyGameParams
{
	int gridx, gridy;
};

class PockyGame {


public:
	PockyGame(const PockyGameParams &params);
	virtual ~PockyGame();

	void init(); // initializes all resources needed by Pocky
	void release(); // releases all resources

	void draw(int time);

protected:



	struct PockyGridCell {
		Float3 wspos; //world space position
		float2 sspos; //screen space position
		float life;
	};

	float fps_;
	int previousTime_; // holds the previous draw time

	PockyGridCell *cell_;
	int ncellsx_, ncellsy_;

	GLPrimitive *quad_, *square_;
	GLFramebufferObject *framebuffer0_, *framebuffer1_;
	GLShaderProgram *texLight_, *hexShader_;
	Float3 lightPositions_[MAX_ACTIVE];
	PockyGameParams params_;


private:
	void DrawGrid(int radx, int rady);
	void applyGLSettings();
	void loadShaders();
	void generateAssets();

};

} /* namespace Pineapple */
#endif /* POCKYGAME_H_ */
