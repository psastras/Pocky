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

struct PockyGridCell {
	Float3 wspos; //world space position
	float2 sspos; //screen space position
	int id;
	float life;
};

class PockyGame {


public:
	PockyGame(const PockyGameParams &params);
	virtual ~PockyGame();

	void init(); // initializes all resources needed by Pocky
	void release(); // releases all resources

	void draw(int time);

	Float4 getGridBounds() { return Float4(0.f, ncellsx_-1, 0.f, ncellsy_-1); }
	int getGridLocation(int x, int y); //get the grid cell (given screen coordinates)

	PockyGridCell *getGrid(int &x, int &y){
		x = ncellsx_;
		y = ncellsy_;
		return cell_;
	}

	void setScore(int s){
		score_ = s;
	}

protected:

	int score_;

	float fps_;
	int previousTime_; // holds the previous draw time

	PockyGridCell *cell_;
	int ncellsx_, ncellsy_;

	GLPrimitive *quad_, *square_, *topbar_;
	GLFramebufferObject *framebuffer0_, *framebuffer1_;
	GLShaderProgram *texLight_, *hexShader_, *bg_, *overlay_, *id_;
	Float3 lightPositions_[MAX_ACTIVE];
	PockyGameParams params_;

	int *ids_;

private:
	void DrawGrid(int radx, int rady, bool solid);
	void applyGLSettings();
	void loadShaders();
	void generateAssets();

};

} /* namespace Pineapple */
#endif /* POCKYGAME_H_ */
