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

namespace Pineapple {

FILE *fmemopen(void *buf, size_t size,
		const char *opentype) /*this is so hacky...*/{
	FILE *f = tmpfile();
	LOGI("Writing!%p", f);
	fwrite(buf, 1, size, f);
	LOGI("Rewingin!");
	rewind(f);
	return f;
}

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
	/*FILE* file = fmemopen(data, size, "rb");
	 if (!file) return 0;
	 LOGI("WAV HEADER CHECK");
	 if (fread(header,sizeof(BasicWAVEHeader),1,file)) {
	 if (!(//these things *must* be valid with this basic header
	 memcmp("RIFF",header->riff,4) ||
	 memcmp("WAVE",header->wave,4) ||
	 memcmp("fmt ",header->fmt,4)  ||
	 memcmp("data",header->data,4)
	 )) {
	 buffer = (char*)malloc(header->dataSize);
	 if (buffer){
	 if (fread(buffer,header->dataSize,1,file)) {
	 fclose(file);
	 return buffer;
	 }
	 free(buffer);
	 }
	 }
	 }

	 fclose(file);
	 return 0;*/
}

ALuint createBufferFromWave(char* data, BasicWAVEHeader header) {
	ALuint buffer = 0;
	ALuint format = 0;
	switch (header.bitsPerSample) {
	case 8:
		format = (header.channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (header.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		return 0;
	}

	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, header.dataSize, header.samplesPerSec);
	return buffer;
}

void Audio::addSound(std::string name, std::string path, bool loadImmediate) {
	// make a new audio object for this file
	sounds_[name] = new AudioObject();
	std::string mes = "adding sound on path " + path;
	LOGI(mes.c_str());
	sounds_[name]->filepath_ = path;
	if (loadImmediate) {
		size_t size;
		unsigned char * data = Engine::instance()->readResourceFromAPK(
				sounds_[name]->filepath_.c_str(), size);
		char * actual = readWAV(data, size, &(sounds_[name]->header_));
		//free(data);
		sounds_[name]->size_ = size;
		sounds_[name]->data_ = actual;

		std::stringstream sstm;
		sstm << "loaded sound path " << path << " size " << size;
		LOGI(sstm.str().c_str());
	}
}

bool Audio::playSound(std::string name) {
	AudioObject *toplay = sounds_[name];
	std::string mes = "play request for sound name " + name + " with path " + toplay->filepath_;
	LOGI(mes.c_str());
	if (!toplay->data_) {
		// load the sound
		size_t size;
		unsigned char * data = Engine::instance()->readResourceFromAPK(
				toplay->filepath_.c_str(), size);
		char * actual = readWAV(data, size, &(toplay->header_));
		//free(data);
		toplay->size_ = size;
		toplay->data_ = actual;
	}
	// make the buffer
	toplay->buffer_ = createBufferFromWave(toplay->data_, toplay->header_);
	if(!toplay->buffer_){
		LOGI("buffer was not created!");
	}
	// Create source from buffer and play it
	alGenSources(1, &(toplay->source_id_));
	alSourcei(toplay->source_id_, AL_BUFFER, toplay->buffer_);

	// Play source
	alSourcePlay(toplay->source_id_);
	int state;
	alGetSourcei(toplay->source_id_, AL_SOURCE_STATE, &state);

//	 do {
//			   alGetSourcei(toplay->source_id_, AL_SOURCE_STATE, &state);
//
//		   } while(state == AL_PLAYING);
//	if(state != AL_PLAYING){
//		LOGI("play start unsuccessful");
//	}else{
//		LOGI("play start successful");
//	}

	std::stringstream sstm;
	sstm << "playing sound " << name << " source number " << toplay->source_id_ << " buffer number " << toplay->buffer_;
	LOGI(sstm.str().c_str());
}

void Audio::update() {
	//LOGI("in audio update");
//	std::stringstream test;
//	int size = sounds_.size();
//	test << "total num sounds " << size;
//	LOGI(test.str().c_str());
	for (auto iter = sounds_.begin(); iter != sounds_.end(); iter++) {
//		std::string m = iter->first;
//		m = "checking sound " + m;
//		LOGI(m.c_str());
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
	LOGI("CONTEXT CREATED");
	// Create audio buffer

	/*
	size_t testsize;
	unsigned char * memory = Engine::instance()->readResourceFromAPK("assets/audio/woman.wav", testsize);
	LOGI("read resource");
	ALuint buffer;
	 BasicWAVEHeader header;
	 char* data = readWAV(memory, testsize,&header);
	 if (data){
	 //Now We've Got A Wave In Memory, Time To Turn It Into A Usable Buffer
	 buffer = createBufferFromWave(data,header);
	 if (!buffer){
	 free(data);
	 }
	 LOGI("buffer made");
	 }
	 LOGI("Wav Read");
	 // Create source from buffer and play it
	 ALuint source = 0;
	 alGenSources(1, &source );
	 alSourcei(source, AL_BUFFER, buffer);

	 // Play source
	 alSourcePlay(source);
*/
//	 int sourceState = AL_PLAYING;
//	 do {
//	 alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
//
//	 } while(sourceState == AL_PLAYING);


}

Audio::~Audio() {
	// TODO Auto-generated destructor stub
}

} /* namespace Pineapple */
