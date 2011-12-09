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

using namespace Pineapple;
namespace Pocky {

PockyState::PockyState(PockyGame *pg) {
	game_ = pg;
	cells_ = pg->getGrid(ncellsx_, ncellsy_);
	activeCells_ = new std::vector<PockyGridCell *>();
	score_ = 0;
	lastUpdate_.tv_sec = 0;
	lastUpdate_.tv_nsec = 0;
	simfile_ = NULL;

	loadSimfile("assets/simfiles/eternus.sim");
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
	Audio::instance()->addSound("sim", soundpath, true, AudioType::OGG);
	Audio::instance()->playSound("sim");
}

void PockyState::update() {
	if (lastUpdate_.tv_sec == 0 && lastUpdate_.tv_nsec == 0) {
		clock_gettime(CLOCK_MONOTONIC, &lastUpdate_);
		return;
	}
	timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	// dt in milliseconds
	int dt = diff_time(lastUpdate_, current);
	//LOGI("dt is %d", dt);
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
		double msperbeat = simfile_->getData()->msperbeat_;

		if (current->life > 0.5) {
			// it's flying in
			// it should decrease by 0.5/msperbeat * dt
			current->life -= 0.5 / msperbeat * dt;
		}

		if (current->life >= 0) {
			// it should stay alive for DIFFICULTY_LIFE * msperbeat
			// so it will decrease by 0.5/(DIFFICULTY_LIFE*msperbeat) * dt
			current->life -= 0.5 / (EASY_LIFE * msperbeat) * dt;
		}
		if (current->life <= 0) {
			current->life -= 1.0 / msperbeat * dt;
		}
		if (current->life <= -1) {
			//			LOGI("erasing");
			i = activeCells_->erase(i);
			continue;
		}
		i++;

	}
	if (simfile_->getPosition() == -1) {
		return;
	}
	// check if we need to spawn new notes
	bool spawning = true;
	while (spawning) {
		SimNote * next = simfile_->getNextNote();
		double prog = Audio::instance()->getProgress("sim");
		// want to spawn it a beat before its time
		double leadtime = simfile_->getData()->msperbeat_;
		if (prog > (next->time_ * 1000.0 - leadtime)) {
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
			}
		} else {
			spawning = false;
		}

	}

	// spawn another one?
	//	for (int i = 0; i < ncellsx_ * ncellsy_; i++) {
	//		if (cell_[i].life <= 0.f) {
	//			while (true) {
	//	int idx = rand() % (ncellsx_ * ncellsy_);
	//	if (cells_[idx].life <= -1 && activeCells_->size() < 10) {
	//		cells_[idx].life = 1.0f;
	//		LOGI("spawn new cell at index %d with life %f", idx, cells_[idx].life);
	//		activeCells_->push_back(&cells_[idx]);
	//	}
	lastUpdate_ = current;
	//			}
	//		}
	//	}
}

void PockyState::touch(float x, float y) {
	// get the cell id
	lastTouch_.x = x;
	lastTouch_.y = y;
	int index = game_->getGridLocation((int) x, (int) y);
	if (index < 0) {
		return;
	}
	Engine::instance()->lock();
	//	LOGI("touched cell %d", index);
//	LOGI("touched cell %d, life is %f", index, cells_[index].life);
	if (cells_[index].life > 0 && cells_[index].life < 0.5) {
		// kill it
		LOGI("killing cell %d", index);
		cells_[index].life = 0;
	}
	Engine::instance()->unlock();
}
}
