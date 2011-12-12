/*
 * PockyState.h
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#ifndef POCKYSTATE_H_
#define POCKYSTATE_H_

#include "../include/PockyGame.h"
#include <pineapple/jni/extern/Engine.h>
#include "../include/Simfile.h"
#include <sys/time.h>
#include <sys/times.h>



//class PockyGame;
using namespace Pineapple;
namespace Pocky {

	enum PockyGameState {
		PLAY, MENU
	};

struct TouchTracker {
	float2 touchpoint_;
	float life_;
};
class PockyGame;
struct PockyGridCell;
class PockyState: public EngineUpdatable {
public:
	PockyState(PockyGame *pg);
	virtual ~PockyState();

	void update();
	void touch(float x, float y);
	void drag(float x, float y);
        void release();
	void loadSimfile(std::string path);

	int getScore(){
		return score_;
	}

	TouchTracker *getTouchPoints(){
		return touchPoints_;
	}

	float2 dragOffset();
	PockyGameState state() {return curState_; }
        float getBeat();

protected:
	PockyGame *game_;
	PockyGridCell *cells_;
	int ncellsx_, ncellsy_;
	int dx_, dy_;
	std::vector<PockyGridCell *> *activeCells_;

	struct timespec lastUpdate_;

	int score_;

	float2 lastTouch_, firstTouch_, deltaTouch_, totalTouch_;

	Simfile *simfile_;

	TouchTracker *touchPoints_;
	int nextfreetouch_;
	PockyGameState curState_;

};
}
#endif /* POCKYSTATE_H_ */
