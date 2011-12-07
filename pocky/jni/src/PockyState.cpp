/*
 * PockyState.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#include "../include/PockyState.h"

namespace Pocky {

PockyState::PockyState(PockyGame *pg) {
	cells_ = pg->getGrid(ncellsx_, ncellsy_);
	activeCells_ = new std::vector<PockyGridCell *>();
	score_ = 0;
	lastUpdate_.tv_sec = 0;
	lastUpdate_.tv_nsec = 0;
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
	for (std::vector<PockyGridCell *>::iterator i = activeCells_->begin(); i != activeCells_->end();) {
		PockyGridCell *current = *i;//activeCells_->at(i);
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
		if(current->life >= 0){
			current->life -= 0.0003 * dt;

		}
		if(current -> life <= 0){
			LOGI("erasing");
			i = activeCells_->erase(i);
			continue;
		}
		i++;


	}
	// spawn another one?
//	for (int i = 0; i < ncellsx_ * ncellsy_; i++) {
//		if (cell_[i].life <= 0.f) {
//			while (true) {
	int idx = rand() % (ncellsx_ * ncellsy_);
	if (cells_[idx].life <= 0 && activeCells_->size() < 10) {
		LOGI("spawn new cell at index %d", idx);
		cells_[idx].life = 1.f;
		activeCells_->push_back(&cells_[idx]);
	}
	lastUpdate_ = current;
//			}
//		}
//	}
}

void PockyState::touch(float x, float y) {

}
}
