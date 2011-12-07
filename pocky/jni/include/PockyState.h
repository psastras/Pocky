/*
 * PockyState.h
 *
 *  Created on: Dec 6, 2011
 *      Author: jhk2
 */

#ifndef POCKYSTATE_H_
#define POCKYSTATE_H_

class PockyGame;

class PockyState {
public:
	PockyState(PockyGame *pg);
	virtual ~PockyState();

	void update();


};

#endif /* POCKYSTATE_H_ */
