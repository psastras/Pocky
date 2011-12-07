/*
 * PineappleEngine.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef PINEAPPLEENGINE_H_
#define PINEAPPLEENGINE_H_
#include "../libzip/zip.h"
#include "../libzip/config.h"
#include "../libzip/zipint.h"

namespace Pineapple {

class EngineUpdatable {
public:
	virtual void update() = 0;
};

class Engine {
public:
	static void init() {
		if(!s_instance)	s_instance = new Engine();
	}

	static Engine *instance() {
		return s_instance;
	}

	unsigned char *readResourceFromAPK(const char* filename, size_t &size);
	unsigned char *readPNGFromAPK(const char* filename, int *width, int *height);
	void start();
	void stop();
	void setUpdatable(EngineUpdatable *updatable) { updatable_ = updatable; }
	EngineUpdatable *updatable() { return updatable_; }
	void setAPKArchive(zip *ark) {
		s_APKArchive = ark;
	}

	void lock();
	void unlock();

protected:
	zip* s_APKArchive;
	EngineUpdatable *updatable_;
	static Engine *s_instance;
	Engine();

	virtual ~Engine();

};

} /* namespace Pineapple */
#endif /* PINEAPPLEENGINE_H_ */
