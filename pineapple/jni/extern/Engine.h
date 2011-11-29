/*
 * PineappleEngine.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef PINEAPPLEENGINE_H_
#define PINEAPPLEENGINE_H_

namespace Pineapple {


class Engine {
public:
	static void init() {
		if(!s_instance)	s_instance = new Engine();
	}

	static Engine *instance() {
		return s_instance;
	}

	void start();
	void stop();

	void lock();
	void unlock();

protected:
	static Engine *s_instance;
	Engine();

	virtual ~Engine();

};

} /* namespace Pineapple */
#endif /* PINEAPPLEENGINE_H_ */
