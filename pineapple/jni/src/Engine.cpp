/*
 * PineappleEngine.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/Engine.h"
#include "../extern/Common.h"
#include <pthread.h>
#include <time.h>

bool gIsRunning = false;
pthread_t thread_ = 0;
pthread_mutex_t mutex;

struct timespec sleepTime;
struct timespec returnTime;

void* run(void*) {



	while(gIsRunning)
	{
		pthread_mutex_lock(&mutex);

		// do stuff

		pthread_mutex_unlock(&mutex);

		nanosleep(&sleepTime, &returnTime);
	}
}

namespace Pineapple {

	Engine::Engine() {
		thread_ = 0;
		sleepTime.tv_sec = 0;
		sleepTime.tv_nsec = 33333333;
	}

	Engine::~Engine() {
		// TODO Auto-generated destructor stub
	}

	void Engine::start()
	{
		pthread_mutex_init(&mutex, 0);
		this->stop();
		gIsRunning = true;
		pthread_create(&thread_, 0, run, 0);
	}

	void Engine::lock() {
		pthread_mutex_lock(&mutex);
	}

	void Engine::unlock() {
		pthread_mutex_unlock(&mutex);
	}

	void Engine::stop() {
		if(gIsRunning)
		{
			if(thread_)
				pthread_join(thread_, 0);

			gIsRunning = false;
			thread_ = 0;
		}

		pthread_mutex_destroy(&mutex);
	}

} /* namespace Pineapple */
