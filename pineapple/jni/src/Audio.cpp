/*
 * Audio.cpp
 *
 *  Created on: Nov 29, 2011
 *      Author: psastras
 */

#include "../extern/Common.h"
#include "../extern/Audio.h"
#include "../extern/Engine.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../openal/tremolo/ivorbisfile.h"
namespace Pineapple {

char* readWAV(void *data, size_t size, BasicWAVEHeader* header) {
	char* buffer = 0;
	LOGI("Wav Reading...%d", (int)size);
	memcpy(header, data, sizeof(BasicWAVEHeader));
	if (!( //these things *must* be valid with this basic header
	memcmp("RIFF", header->riff, 4) || memcmp("WAVE", header->wave, 4)
			|| memcmp("fmt ", header->fmt, 4) || memcmp("data", header->data, 4))) {
		LOGI("Header...%d", (int)header->dataSize);
		buffer = (char*) malloc(header->dataSize);
		if (buffer) {
			memcpy(buffer, data + sizeof(header), header->dataSize);
			LOGI("ok...");
			return buffer;
		}
		free(buffer);
	}
	return 0;
}
char array[32768];    // Local fixed size array
char *readOgg(unsigned char *data, size_t &size, BasicWAVEHeader *header) {
	LOGI("reading");
	std::vector < char > *buffer = new std::vector < char > ();
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	ov_open(0, &oggFile, (char *)data, size);
	pInfo = ov_info(&oggFile, -1);
	int bitStream;
	long bytes;

	 do {
	    // Read up to a buffer's worth of decoded sound data
	    bytes = ov_read(&oggFile, array, 32768,  &bitStream);
	    // Append to end of buffer
	    buffer->insert(buffer->end(), array, array + bytes);
	  } while (bytes > 0);
	LOGI("Channels: %d, %d", pInfo->channels, pInfo->rate);
	header->bitsPerSample = 16;
	header->channels = pInfo->channels;
	header->samplesPerSec = pInfo->rate;
	size = buffer->size();
	ov_clear(&oggFile);
	char *toreturn = new char[buffer->size()];
	std::copy(buffer->begin(), buffer->end(), toreturn);
	delete buffer;
	return toreturn;
}

ALuint createBuffer(char* data, int datasize, int nChannels, int bitspersample,
		int samplespersecond) {
	ALuint buffer = 0;
	ALuint format = 0;
	switch (bitspersample) {
	case 8:
		format = (nChannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (nChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		return 0;
	}

	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, datasize, samplespersecond);
	return buffer;
}

void Audio::addSound(std::string name, std::string path, bool loadImmediate, AudioType type) {
	// make a new audio object for this file
	sounds_[name] = new AudioObject();
	sounds_[name]->filepath_ = path;
	if (loadImmediate) {
		size_t size;
		unsigned char * data = Engine::instance()->readResourceFromAPK(
				sounds_[name]->filepath_.c_str(), size);
		if(type == AudioType::WAV) {

			BasicWAVEHeader header;
			char * actual = readWAV(data, size, &header);
			//free(data);
			sounds_[name]->size_ = size;
			sounds_[name]->data_ = actual;
			//int datasize, nChannels,  bitspersample,  samplespersecond;
			sounds_[name]->nChannels = header.channels;
			sounds_[name]->bitspersample = header.bitsPerSample;
			sounds_[name]->samplespersecond = header.samplesPerSec;
			free(data);
		} else if(type == AudioType::OGG) {
			BasicWAVEHeader header;
			char * actual = readOgg(data, size, &header);
			//free(data);
			sounds_[name]->size_ = size;
			sounds_[name]->data_ = actual;
			//int datasize, nChannels,  bitspersample,  samplespersecond;
			sounds_[name]->nChannels = header.channels;
			sounds_[name]->bitspersample = header.bitsPerSample;
			sounds_[name]->samplespersecond = header.samplesPerSec;

			free(data);

		}
	}
}

bool Audio::playSound(std::string name) {
	AudioObject *toplay = sounds_[name];
	if (!toplay->data_) {
		// load the sound
		//this->addSound(name, toplay->filepath_, true);
	}
	// make the buffer
	toplay->buffer_ = createBuffer(toplay->data_, toplay->size_,
			toplay->nChannels, toplay->bitspersample, toplay->samplespersecond);
	// Create source from buffer and play it
	alGenSources(1, &(toplay->source_id_));
	alSourcei(toplay->source_id_, AL_BUFFER, toplay->buffer_);
	// Play source
	alSourcePlay(toplay->source_id_);
	int state;
	alGetSourcei(toplay->source_id_, AL_SOURCE_STATE, &state);
}

void Audio::update() {
	for (auto iter = sounds_.begin(); iter != sounds_.end(); iter++) {
		AudioObject *ao = iter->second;
		int sourceState;
		alGetSourcei(ao->source_id_, AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_STOPPED && ao->data_ && ao->source_id_ > 0) {
			std::string mes = "deleting sound " + iter->first;
							LOGI(mes.c_str());
			// delete stuff
			alDeleteSources(1, &ao->source_id_);
			alDeleteBuffers(1, &ao->buffer_);
			if (!ao->keepLoaded_) {
				delete[] ao->data_;
				ao->data_ = NULL;
				sounds_.erase(iter->first);
				std::string m = "removed sound " + iter->first + "from map";
				LOGI(m.c_str());
				delete ao;
			}

			ao->source_id_ = -1;
			ao->buffer_ = -1;
		}
	}

}

Audio::Audio() {
	// TODO Auto-generated constructor stub
	// Global Variables
	device_ = 0;
	context_ = 0;
	const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };

	// Initialization
	device_ = alcOpenDevice(0);
	context_ = alcCreateContext(device_, context_attribs);
	alcMakeContextCurrent(context_);

}

void Audio::addSound2(const char *test) {
	LOGI("reading");
	size_t size;
	std::vector < char > *buffer = new std::vector < char > ();
	unsigned char *data = Engine::instance()->readResourceFromAPK(test, size);
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	LOGI("attemptin open");
	ov_open(0, &oggFile, (char *)data, size);
	pInfo = ov_info(&oggFile, -1);

	int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char array[32768];    // Local fixed size array
	 do {
	    // Read up to a buffer's worth of decoded sound data
	    bytes = ov_read(&oggFile, array, 32768,  &bitStream);
	    // Append to end of buffer
	    buffer->insert(buffer->end(), array, array + bytes);
	  } while (bytes > 0);
	LOGI("Channels: %d", pInfo->channels);
	delete buffer;
	ov_clear(&oggFile);
}

Audio::~Audio() {
	// TODO Auto-generated destructor stub
}

} /* namespace Pineapple */
