/*
 * PockyState.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#include "../include/PockyState.h"
#include <pineapple/jni/extern/Audio.h>
#include "../include/Simfile.h"

#define EASY_LIFE 4
// LATENCY is 500 for archos, 350 for htc evo, 200 for nexus s
#define LATENCY 400
#define TOUCH_LAG 100
#define NUM_TOUCHPOINTS 100
#define TOUCH_INC 16

using namespace Pineapple;
namespace Pocky {

PockyState::PockyState(PockyGame *pg) {
	game_ = pg;
	curState_ = PLAY;
	cells_ = pg->getGrid(ncellsx_, ncellsy_);
	activeCells_ = new std::vector<PockyGridCell *>();
	score_ = 0;
	lastUpdate_.tv_sec = 0;
	lastUpdate_.tv_nsec = 0;
	simfile_ = NULL;

        touchPoints_ = new TouchTracker[NUM_TOUCHPOINTS];
	nextfreetouch_ = 0;

        pg->setState(this);

		loadSimfile("assets/simfiles/virtual.sim");
}

PockyState::~PockyState() {
	delete activeCells_;
}

int diff_time(timespec start, timespec end) {
	timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp.tv_sec * 1000 + (temp.tv_nsec / 1000000.0);

}

void PockyState::loadSimfile(std::string path) {
	simfile_ = Simfile::parse(path);
	// play the song here for now
	std::string soundpath = "assets/audio/" + simfile_->getData()->music_;
	Audio::instance()->addSound("sim", soundpath, true, AudioType::OGG,
			simfile_->getData()->length_);
	Audio::instance()->playSound("sim", simfile_->getData()->length_);
}

float PockyState::getBeat(){
    // get the progress of the simfile
    double msprog = Audio::instance()->getProgress("sim") - LATENCY;
    msprog -= simfile_->getData()->offset_ * 1000;
    // get the remainder after dividing by the bpm time
    double quot = msprog / simfile_->getData()->msperbeat_;
    // round down to int
    quot = floor(quot);
    double offset = msprog - quot*simfile_->getData()->msperbeat_;
    // it can be either early or late
    double actual = MIN(offset, simfile_->getData()->msperbeat_ - offset);
    // return ratio of total beat time
    // the smaller the offset is, the better
    return 1.0 - (2*actual / simfile_->getData()->msperbeat_);
}

void PockyState::update() {
	if (lastUpdate_.tv_sec == 0 && lastUpdate_.tv_nsec == 0) {
		clock_gettime(CLOCK_MONOTONIC, &lastUpdate_);
		return;
	}
//        LOGI("goodness is %f", getBeat());
	timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	// dt in milliseconds
	int dt = diff_time(lastUpdate_, current);

	if(curState_ == PLAY)
	{
	//LOGI("dt is %d", dt);
	//Engine::instance()->lock();
	double msperbeat = simfile_->getData()->msperbeat_;
	for (std::vector<PockyGridCell *>::iterator i = activeCells_->begin();
			i != activeCells_->end();) {
		PockyGridCell *current = *i; //activeCells_->at(i);
		//		if (current->life > 0.5) {
		//			// it's coming in and we need to decrement
		//			current->life -= 0.0002 * dt;
		//			i++;
		//		} else if (current->life > 0) {
		//			// it's landed and we need to decrement
		//			current->life -= 0.0001 * dt;
		//			i++;
		//		} else {
		//			// otherwise it's dead and we don't care
		//			// remove from actives
		//			i = activeCells_->erase(i);
		//		}


		if (current->life > 0.5) {
			// it's flying in
			// it should decrease by 0.5/msperbeat * dt
			current->life -= 0.5 / msperbeat * dt;
		}

		if (current->life >= 0) {
			// it should stay alive for DIFFICULTY_LIFE * msperbeat
			// so it will decrease by 0.5/(DIFFICULTY_LIFE*msperbeat) * dt
			current->life -= 0.5 / (EASY_LIFE * msperbeat) * dt;
			if (current->life <= 0) {
				// i didn't hit it yet so it's a miss
				current->life = -1;
			}
		}
		if (current->life < 0) {
			current->life -= 1.0 / msperbeat * dt;
		}
		if (current->life <= -1) {
			//			LOGI("erasing");
			i = activeCells_->erase(i);
			continue;
		}
		i++;

	}

        for(int i = 0; i < NUM_TOUCHPOINTS; i++){
		TouchTracker *cur = &touchPoints_[i];
		if(cur->life_ > 0){
                        cur->life_ -= 1.0 / (msperbeat) * dt;
		}
	}
	if (simfile_->getPosition() == -1) {
		return;
	}

	// check if we need to spawn new notes
	bool spawning = true;
	while (spawning) {
		SimNote * next = simfile_->getNextNote();
		double prog = Audio::instance()->getProgress("sim");
//		game_->setScore(prog);
		// want to spawn it a beat before its time
		double leadtime = simfile_->getData()->msperbeat_;
		if (prog > (next->time_ * 1000.0 - leadtime + LATENCY)) {
			// spawn the note
			if (next->x_ < 0) {
				// random position
				bool found = false;
				while (!found) {
					int idx = rand() % (ncellsx_ * ncellsy_);
					if (cells_[idx].life <= -1 && activeCells_->size() < 50) {
						cells_[idx].life = 1.0f;
						activeCells_->push_back(&cells_[idx]);
						//						LOGI("spawn new cell at index %d with life %f", idx, cells_[idx].life);
						found = true;
					}
				}
			} else {
				// set position
				int idx = next->y_ * ncellsx_ + next->x_;
				cells_[idx].life = 1.0f;
				activeCells_->push_back(&cells_[idx]);
			}
			int newpos = simfile_->incrementPosition();
			if (newpos == -1) {
				//				delete simfile_;
				//				simfile_ = 0;
				spawning = false;
				LOGI("end of simfile");
				exit(0);
			}
		} else {
			spawning = false;
		}

	}
} else if(curState_ == MENU) {
//	if(totalTouch_.y > 0) {
//		totalTouch_.y -= 1.f * dt;
//	}
	//deltaTouch_ *= 0.5f;

}

	// spawn another one?
//	int idx = rand() % (ncellsx_ * ncellsy_);
//	if (cells_[idx].life <= -1 && activeCells_->size() < 10) {
//
//		cells_[idx].life = 1.0f;
//
//		//			LOGI("spawn new cell at index %d with life %f", idx, cells_[idx].life);
//		activeCells_->push_back(&cells_[idx]);
//	}
	//Engine::instance()->unlock();

	lastUpdate_ = current;
	//			}
	//		}
	//	}
}

float2 PockyState::dragOffset() {
	totalTouch_ = (totalTouch_).min(0.0);
	return (totalTouch_ + deltaTouch_).min(0.0);
}

void PockyState::drag(float x, float y) {
	if(curState_ == PLAY) {
		this->touch(x, y);
	} else if(curState_ == MENU) {
		deltaTouch_ = float2(x,y)-firstTouch_;
//		deltaTouch_.x += x;
//		deltaTouch_.y += y;
	}
}

void PockyState::touch(float x, float y) {
	// get the cell id

	lastTouch_.set(x, y);

	if(curState_ == PLAY) {
		// get the line between the last touch point and this one
		float2 line;
		float slope;
		float2 start;

		int prevtouch = (nextfreetouch_ == 0) ? (NUM_TOUCHPOINTS - 1) : nextfreetouch_ - 1;
		TouchTracker prev = touchPoints_[prevtouch];
		if(prev.life_> 0){
//            LOGI("previous touchpoint %d at %f, %f", prevtouch, prev.touchpoint_.x, prev.touchpoint_.y);
		line.y = y - prev.touchpoint_.y;
		line.x = x - prev.touchpoint_.x;
		slope = line.y / line.x;
		LOGI("slope of %f", slope);
		start = prev.touchpoint_;
	}else{
		line = float2(0,0);
		slope = 0;
		start = float2(x, y);
	}

	float samplerate = sqrt(line.y*line.y - line.x*line.x);
	int divisions = 1;
	while(samplerate > 5){
		divisions++;
		samplerate /= 2;
	}

	line.x /= divisions;
	line.y /= divisions;
	for(int i = 0; i < divisions; i++){
		start.x += line.x;
		start.y += line.y;
		touchPoints_[nextfreetouch_].touchpoint_ = start;
		touchPoints_[nextfreetouch_].life_ = 1.0;
//        LOGI("setting touch point %d at %f, %f", nextfreetouch_, start.x, start.y);
		nextfreetouch_ = (nextfreetouch_ + 1) % NUM_TOUCHPOINTS;
	}
	} else if(curState_ == MENU)
	{
//		loadSimfile("assets/simfiles/eternus.sim");
//		curState_ = PLAY;
		firstTouch_.set(x, y);
		totalTouch_ += deltaTouch_;
		deltaTouch_.set(0.f, 0.f);

//		printf("%f --> %f" ,totalTouch_.y, deltaTouch_.y);
//		fflush(stdout);
	}


//	lastTouch_.x = x;
//	lastTouch_.y = y;



    /*

        // now fill in the line in between
//        float2 start = lastTouch_;
        while(start.x <= x && start.y <= y){
            // make a thing
            touchPoints_[nextfreetouch_].touchpoint_ = start;
            touchPoints_[nextfreetouch_].life_ = 1.0;

//            LOGI("setting touch point %d to life 1.0", nextfreetouch_);
            nextfreetouch_ = (nextfreetouch_ + 1) % NUM_TOUCHPOINTS;
            start.x += 100;
            start.y += slope * 100;
        }
        */

//        lastTouch_.x = x;
//        lastTouch_.y = y;

        /*
	int index = game_->getGridLocation((int) x, (int) y);
	if (index < 0) {
		return;
	}
	Engine::instance()->lock();
        double goodness = getBeat();

                        goodness = (1.0-goodness) * simfile_->getData()->msperbeat_;
                        LOGI("goodness = %f", goodness);
        //	LOGI("touched cell %d", index);
//	LOGI("touched cell %d, life is %f", index, cells_[index].life);
	if (cells_[index].life > 0 && cells_[index].life < 0.5) {
		// get the timing
		// since it takes DIFFICULTY_LIFE beats to die, the closer you are to a multiple of 0.5/DIFFICULTY_LIFE the better your timing
//		double inc = 0.5 / EASY_LIFE;
//                double timing = cells_[index].life - (floor(cells_[index].life / inc))*inc;
//                timing = MIN(timing, inc - timing);
//                // timing will be between 0 and inc / 2
//                double goodness = 2 * timing / inc;


            // get how close you are to a beat


                // goodness is from 0 to 1, smaller is better
		// kill it
                LOGI("killing cell %d", index);
                if(goodness < 250){
                cells_[index].judge = 0;
                }else if(goodness <400){
                cells_[index].judge = 1;
            }else{
                cells_[index].judge = 2;
            }

		cells_[index].life = -0.0000000001;
		score_+= (int) (goodness * 1000);
		game_->setScore(score_);
	}
	Engine::instance()->unlock();
		*/
}

void PockyState::release(){
    // we want to evaluate the touchpoints the player has so far accumulated
    Engine::instance()->lock();
    for(int i = 0; i < NUM_TOUCHPOINTS; i++){
        TouchTracker current = touchPoints_[i];
        int index = game_->getGridLocation(current.touchpoint_.x, current.touchpoint_.y);
        if(index < 0 || current.life_ <= 0){
            continue;
        }
        if(cells_[index].life > 0 && cells_[index].life < 0.5){
            // kill it
            cells_[index].judge = 1;
            cells_[index].life = -0.0000000001;
            score_ += 1;
            game_->setScore(score_);
        }
    }
    Engine::instance()->unlock();
}

}
