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
		s_APKArchive = 0;
	}

	Engine::~Engine() {
		// TODO Auto-generated destructor stub
	}

	unsigned char *Engine::readResourceFromAPK(const char* filename, size_t &size) {
		if(s_APKArchive == 0) return 0;
		zip_file *file = zip_fopen(s_APKArchive, filename, 0);
		if (!file) {
		   return 0 ;
		}

		struct zip_stat info;
		zip_stat(file->za, filename, ZIP_FL_NOCASE, &info);
		if(info.size <= 0)
			return 0;

		unsigned char *buffer = new unsigned char[info.size];
		size = zip_fread(file, buffer, info.size);
		//LOGI("SIZE: %d, %d", info.size, success);
		zip_fclose(file);
		return buffer;
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
