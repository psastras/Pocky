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

#define BUFFER_SIZE 1048576
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
char array[32768]; // Local fixed size array

char *bufferOgg(AudioObject &ao, OggVorbis_File *oggFile, size_t &buffersize) {
	std::vector<char> *buffer = new std::vector<char>();
	long bytes;
	long total = 0;

	//LOGI("reading new bytes into buffer");
	do {
		// Read up to a buffer's worth of decoded sound data
		//LOGI("bitstream is %d", ao.bitStream_);
#ifdef _DESKTOP
		bytes = ov_read(oggFile, array, 32768, 0, 2, 1, &ao.bitStream_);
#else
		bytes = ov_read(oggFile, array, 32768,  &ao.bitStream_);
#endif
		if (bytes == 0) {
			break;
		}
		//LOGI("read %d bytes", (int)bytes);
		// Append to end of buffer
		buffer->insert(buffer->end(), array, array + bytes);
		total += bytes;
		//LOGI("inserted %d bytes", bytes);
	} while (total <= BUFFER_SIZE);
	buffersize = total;
	//LOGI("read %d total bytes", buffersize);
	//size = buffer->size();
	char *toreturn = new char[buffer->size()];
	std::copy(buffer->begin(), buffer->end(), toreturn);
	delete buffer;
	return toreturn;
}

char *readOgg(AudioObject &ao, unsigned char *data, size_t &size,
		BasicWAVEHeader *header, size_t &buffersize) {
	LOGI("reading");

	vorbis_info *pInfo;
	ao.file_ = new OggVorbis_File();
#ifdef _DESKTOP
	ov_fopen((char *)ao.filepath_.c_str(), ao.file_);
#else
	ov_open(0, ao.file_, (char *) data, size);
#endif
	pInfo = ov_info(ao.file_, -1);

	LOGI("Channels: %d, %d", pInfo->channels, pInfo->rate);
	header->bitsPerSample = 16;
	header->channels = pInfo->channels;
	header->samplesPerSec = pInfo->rate;

	// get the total length
	ao.totalLength_ = ov_time_total(ao.file_, -1);
	LOGI("total length of file %d", ao.totalLength_);

	char *toreturn = bufferOgg(ao, ao.file_, buffersize);
	//ov_clear(&oggFile);
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

void repurposeBuffer(ALuint &buffer, char* data, int datasize, int nChannels,
		int bitspersample, int samplespersecond) {
	ALuint format = 0;
	switch (bitspersample) {
	case 8:
		format = (nChannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (nChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		return;
//		return 0;
	}

	//alGenBuffers(1, &	buffer);
	alBufferData(buffer, format, data, datasize, samplespersecond);
	//return buffer;
}

bool rebufferOgg(AudioObject &ao, unsigned char *data, size_t &size) {
	// check the number of currently playing buffers
	int val;
	alGetSourcei(ao.source_id_, AL_BUFFERS_QUEUED, &val);
	int state;
	alGetSourcei(ao.source_id_, AL_SOURCE_STATE, &state);
	int freebuffers;
	alGetSourcei(ao.source_id_, AL_BUFFERS_PROCESSED, &freebuffers);
	val -= freebuffers;
	// if there is less than two queued buffers
	if (val != 0 && val < 2 && state != AL_STOPPED) {
	//	LOGI("only have %d queued buffers, so adding more", val);
		//OggVorbis_File oggFile;
		//ov_open(0, &oggFile, (char *) data, size);
		size_t buffersize;
		while (val < 2) {
		//	LOGI("get new buffer");
			char *newbuffer = bufferOgg(ao, ao.file_, buffersize);
			if (newbuffer == 0) {
				// we've reached the end of the file
		//		LOGI("reached end of file");
				ao.finished_ = true;
				return true;
			}
			ALuint buffer;
			//LOGI("unqueue buffers");
			alSourceUnqueueBuffers(ao.source_id_, 1, &buffer);
			//LOGI("unqueued buffers");
			int error = alGetError();
			if (error != AL_NO_ERROR) {
				LOGI("unqueue returned error %d", error);
			}
			if (error == AL_INVALID_VALUE || freebuffers <= 0) {
				LOGI("unqueue failed or no free buffers");
				// create a new buffer
				buffer = createBuffer(newbuffer, buffersize, ao.nChannels,
						ao.bitspersample, ao.samplespersecond);
				LOGI("new buffer %d created", buffer);
			} else {
			//	LOGI("reusing buffer %d", buffer);
				// repurpose the existing buffer
				repurposeBuffer(buffer, newbuffer, buffersize, ao.nChannels,
						ao.bitspersample, ao.samplespersecond);
			//	LOGI("buffer %d repurposed", buffer);
				delete[] ao.buffers_[buffer];
			}
			ao.buffers_[buffer] = newbuffer;
			// queue it up
			alSourceQueueBuffers(ao.source_id_, 1, &buffer);
			if (alGetError() != AL_NO_ERROR) {
				LOGI("error requeuing %d", alGetError());
			}
			//LOGI("rebuffered with buffer %d", buffer);
			val++;
		}
		//ov_clear(&oggFile);
	}
	ao.finished_ = false;
	return false;
}

void Audio::addSound(std::string name, std::string path, bool loadImmediate,
		AudioType type) {
	LOGI("Loading music file at path %s", path.c_str());
	// make a new audio object for this file
	AudioObject *toadd = new AudioObject();
	toadd->filepath_ = path;
	if (loadImmediate) {
		size_t size;
		size_t buffersize;
		unsigned char * data = Engine::instance()->readResourceFromAPK(
				toadd->filepath_.c_str(), size);
		char *actual = 0;
		if (type == AudioType::WAV) {

			BasicWAVEHeader header;
			actual = readWAV(data, size, &header);
			//free(data);
			toadd->size_ = size;
//			toadd->data_ = actual;
			//int datasize, nChannels,  bitspersample,  samplespersecond;
			toadd->nChannels = header.channels;
			toadd->bitspersample = header.bitsPerSample;
			toadd->samplespersecond = header.samplesPerSec;
			toadd->type_ = AudioType::WAV;
			free(data);
		} else if (type == AudioType::OGG) {
			BasicWAVEHeader header;
			actual = readOgg(*toadd, data, size, &header, buffersize);
			//free(data);
			toadd->size_ = size;
			toadd->data_ = data;
			//int datasize, nChannels,  bitspersample,  samplespersecond;
			toadd->nChannels = header.channels;
			toadd->bitspersample = header.bitsPerSample;
			toadd->samplespersecond = header.samplesPerSec;
			toadd->type_ = AudioType::OGG;

			free(data);

		}
		// make the buffer
		ALuint buffer = createBuffer(actual, buffersize,
				toadd->nChannels, toadd->bitspersample,
				toadd->samplespersecond);
		toadd->buffers_[buffer] = actual;
		// Create source from buffer and play it
		alGenSources(1, &(toadd->source_id_));
		alSourceQueueBuffers(toadd->source_id_, 1, &buffer);
		LOGI("added new buffer %d", buffer);
		sounds_[name] = toadd;
	}
}

bool Audio::playSound(std::string name) {
	AudioObject *toplay = sounds_[name];
	if (toplay->buffers_.empty()) {
		// load the sound
		this->addSound(name, toplay->filepath_, true, toplay->type_);
	}

//	alSourcei(toplay->source_id_, AL_BUFFER, toplay->buffer_);
	// Play source
	alSourcePlay(toplay->source_id_);
	clock_gettime(CLOCK_MONOTONIC, &toplay->startTime_);
//	int state;
//	alGetSourcei(toplay->source_id_, AL_SOURCE_STATE, &state);
}

bool Audio::stopSound(std::string name) {
	AudioObject *tostop = sounds_[name];
	alSourceStop(tostop->source_id_);
}

timespec diff_time(timespec start, timespec end){
	timespec temp;
		if ((end.tv_nsec-start.tv_nsec)<0) {
			temp.tv_sec = end.tv_sec-start.tv_sec-1;
			temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
		} else {
			temp.tv_sec = end.tv_sec-start.tv_sec;
			temp.tv_nsec = end.tv_nsec-start.tv_nsec;
		}
		return temp;

}

double Audio::getProgress(std::string name) {
	AudioObject *ao = sounds_[name];
//	float pos = 0;
//	alGetSourcef(ao->source_id_, AL_SEC_OFFSET, &pos);
	struct timespec end;
	clock_gettime(CLOCK_MONOTONIC, &end);
	timespec diff = diff_time(ao->startTime_, end);
	// convert to milliseconds
	return diff.tv_sec * 1000 + (diff.tv_nsec / 1000000.0);
}

void Audio::update() {
//	LOGI("we have %d sounds", sounds_.size());
	for (auto iter = sounds_.begin(); iter != sounds_.end(); iter++) {
//		LOGI("inside sound update loop");
//		LOGI(("checking sound " + iter->first).c_str());
		AudioObject *ao = iter->second;
		int sourceState;
		alGetSourcei(ao->source_id_, AL_SOURCE_STATE, &sourceState);

//			LOGI("source state is %d", ao->source_id_);
		if (sourceState == AL_STOPPED && !ao->buffers_.empty()
				&& ao->source_id_ > 0) {
			//std::string mes = "deleting sound " + iter->first;
			//LOGI(mes.c_str());
			// delete stuff
			alDeleteSources(1, &ao->source_id_);
//			alDeleteBuffers(1, &ao->buffer_);
			if (!ao->keepLoaded_) {
				for (auto itertwo = ao->buffers_.begin();
						itertwo != ao->buffers_.end(); itertwo++) {
					LOGI("trying delete %d", itertwo->first);
					alDeleteBuffers(1, &itertwo->first);
					delete[] itertwo->second;
					LOGI("removed buffer %d", itertwo->first);
//					ao->buffers_.erase(itertwo->first);
				}
				ao->buffers_.clear();
				std::string m = "removing sound " + iter->first + " from map";
				LOGI(m.c_str());
				iter = sounds_.erase(iter);
				ov_clear(ao->file_);
				delete ao;
//				LOGI("finished deleting");
				break;
			}

			ao->source_id_ = -1;
		} else if (sourceState != AL_STOPPED && ao->type_ == AudioType::OGG
				&& !ao->finished_) {
			// check for rebuffers
			bool result = rebufferOgg(*ao, ao->data_, ao->size_);
			// if result is true, we've reached the end of file
			if (sourceState != AL_PLAYING) {
				alSourcePlay(ao->source_id_);
			}
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

	sounds_.clear();
	LOGI("cleared sounds to %d", sounds_.size());

}

Audio::~Audio() {
	// TODO Auto-generated destructor stub
}

} /* namespace Pineapple */
