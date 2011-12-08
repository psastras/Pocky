/*
 * PineappleEngine.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/Engine.h"
#include "../extern/Common.h"
#include "../extern/Audio.h"
#include "../libpng/png.h"
#include <pthread.h>
#include <time.h>

bool gIsRunning = false;
pthread_t thread_ = 0;
pthread_mutex_t mutex;

struct timespec sleepTime;
struct timespec returnTime;

void* run(void*) {
	//LOGI("engine update");
	while (gIsRunning) {
#ifndef _DESKTOP
		if (Pineapple::Audio::instance()) {
		//	LOGI("audio update");
			Pineapple::Audio::instance()->update();
		}
#endif
	//	LOGI("before mutex");
		pthread_mutex_lock(&mutex);
//		int res = pthread_mutex_trylock(&mutex);
//
//		if (res == 0) {
//			LOGI("mutex lock successful");
		// do stuff

		if (Pineapple::Engine::instance()->updatable()) {
			Pineapple::Engine::instance()->updatable()->update();
		}

//		LOGI("after audio update");
		pthread_mutex_unlock(&mutex);
		//LOGI("after mutex");

//
//		} else {
//			LOGI("mutex lock failed");
//		}

		nanosleep(&sleepTime, &returnTime);
	}
}

namespace Pineapple {

Engine::Engine() {
	thread_ = 0;
	sleepTime.tv_sec = 0;
	sleepTime.tv_nsec = 33333333;
	s_APKArchive = 0;
	updatable_ = 0;
#ifndef _DESKTOP
	Pineapple::Audio::init();
#endif
}


Engine::~Engine() {
#ifndef _DESKTOP
	Pineapple::Audio::release();
#endif
}
#ifndef _DESKTOP
unsigned char *Engine::readResourceFromAPK(const char* filename, size_t &size) {
	if (s_APKArchive == 0)
		return 0;
	zip_file *file = zip_fopen(s_APKArchive, filename, 0);
	if (!file) {
		return 0;
	}

	struct zip_stat info;
	zip_stat(file->za, filename, ZIP_FL_NOCASE, &info);
	if (info.size <= 0)
		return 0;

	unsigned char *buffer = new unsigned char[info.size];
	size = zip_fread(file, buffer, info.size);
	//LOGI("SIZE: %d, %d", info.size, success);
	zip_fclose(file);
	return buffer;
}

void png_zip_read(png_structp png, png_bytep data, png_size_t size) {
	struct zip_file *zfp = (struct zip_file *) png->io_ptr;
	zip_fread(zfp, data, size);
}

void abort_(const char * s, ...) {
	va_list args;
	va_start(args, s);
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
	abort();
}
unsigned char *Engine::readPNGFromAPK(const char* filename, int *w2, int *g2) {
	/* adapted from http://androgeek.info/ */
	int x, y;

	int width, height;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
	zip_file *file = zip_fopen(s_APKArchive, filename, 0);
	png_byte header[8];
	zip_fread(file, header, 8);
	if (png_sig_cmp(header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG file",
				filename);

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		abort_("[read_png_file] png_create_read_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	//png_init_io(png_ptr, fp);
	png_set_read_fn(png_ptr, NULL, png_zip_read);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++)
		row_pointers[y] = (png_byte*) malloc(
				png_get_rowbytes(png_ptr, info_ptr));

	png_read_image(png_ptr, row_pointers);
	zip_fclose(file);

	return 0;
}
#else
unsigned char *Engine::readResourceFromAPK(const char* filename, size_t &size) {
	FILE *file = fopen(filename, "r");
	fseek(file, 0, SEEK_END);
	size=ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char *buffer = new unsigned char[size];
	fread(buffer, size, 1, file);
	fclose(file);
	return buffer;
}
#endif

void Engine::start() {
	pthread_mutex_init(&mutex, 0);
	this->stop();
	gIsRunning = true;
	pthread_create(&thread_, 0, run, 0);
	LOGI("Engine Thread Started");
	pthread_mutex_unlock(&mutex);

//	int res = pthread_mutex_trylock(&mutex);
//	if(res != 0){
//		LOGI("mutex locked after init");
//	}else{
//		LOGI("mutex okay in init");
//	}
}

void Engine::lock() {
	pthread_mutex_lock(&mutex);
}

void Engine::unlock() {
	pthread_mutex_unlock(&mutex);
}

void Engine::stop() {
	if (gIsRunning) {
		if (thread_)
			pthread_join(thread_, 0);

		gIsRunning = false;
		thread_ = 0;
	}

	pthread_mutex_destroy(&mutex);
}

} /* namespace Pineapple */
