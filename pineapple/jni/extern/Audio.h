/*
 * Audio.h
 *
 *  Created on: Nov 29, 2011
 *      Author: psastras
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "../openal/include/AL/al.h"
#include  "../openal/include/AL/alc.h"

namespace Pineapple {

struct BasicWAVEHeader {
	char riff[4]; //'RIFF'
	unsigned int riffSize;
	char wave[4]; //'WAVE'
	char fmt[4]; //'fmt '
	unsigned int fmtSize;
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char data[4]; //'data'
	unsigned int dataSize;
};

enum AudioType {
	OGG, WAV
};

class AudioObject {
public:
	int nChannels,  bitspersample,  samplespersecond;
	std::string filepath_;
	char *data_;
	ALuint buffer_;
	size_t size_;
	ALuint source_id_;
	bool keepLoaded_;
	AudioType type_;

	AudioObject(){
		source_id_ = -1;
		buffer_ = -1;
		data_ = NULL;
	}

	virtual ~AudioObject(){
		delete data_;
	}
};

class Audio {
public:
	static void init(){
		if(!s_instance) s_instance = new Audio();
	}

	static void release(){
		delete s_instance;
		s_instance = NULL;
	}

	static Audio *instance(){
		return s_instance;
	}

	virtual ~Audio();
	void addSound(std::string name, std::string path, bool loadImmediate, AudioType type);
	void addSound2(const char *name);
	bool playSound(std::string name);
	bool stopSound(std::string name);
	void update();
protected:
	Audio();
	static Audio *s_instance;
	std::unordered_map<std::string, AudioObject*> sounds_;
	ALCdevice *device_;
	ALCcontext *context_;
};

} /* namespace Pineapple */
#endif /* AUDIO_H_ */
