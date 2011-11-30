/*
 * Audio.cpp
 *
 *  Created on: Nov 29, 2011
 *      Author: psastras
 */

#include "../extern/Common.h"
#include "../extern/Audio.h"
#include "../openal/include/AL/al.h"
#include  "../openal/include/AL/alc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace Pineapple {

struct BasicWAVEHeader {
	char  riff[4];//'RIFF'
	unsigned int riffSize;
	char  wave[4];//'WAVE'
	char  fmt[4];//'fmt '
	unsigned int fmtSize;
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char  data[4];//'data'
	unsigned int dataSize;
};

FILE *fmemopen (void *buf, size_t size, const char *opentype) /*this is so hacky...*/ {
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
	if (!(//these things *must* be valid with this basic header
			memcmp("RIFF",header->riff,4) ||
			memcmp("WAVE",header->wave,4) ||
			memcmp("fmt ",header->fmt,4)  ||
			memcmp("data",header->data,4)
			)) {
		LOGI("Header...%d", (int)header->dataSize);
		buffer = (char*)malloc(header->dataSize);
		if (buffer){
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

ALuint createBufferFromWave(char* data,BasicWAVEHeader header){
	ALuint buffer = 0;
	ALuint format = 0;
	switch (header.bitsPerSample){
		case 8:
			format = (header.channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
			break;
		case 16:
			format = (header.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
			break;
		default:
			return 0;
	}

	alGenBuffers(1,&buffer);
	alBufferData(buffer,format,data,header.dataSize,header.samplesPerSec);
	return buffer;
}

Audio::Audio(void *memory, size_t size) {
	// TODO Auto-generated constructor stub
	 // Global Variables
	 ALCdevice* device = 0;
	 ALCcontext* context = 0;
	 const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };

	 // Initialization
	 device = alcOpenDevice(0);
	 context = alcCreateContext(device, context_attribs);
	 alcMakeContextCurrent(context);
	 LOGI("CONTEXT CREATED");
	 // Create audio buffer
	  ALuint buffer;
	  BasicWAVEHeader header;
	  char* data = readWAV(memory, size,&header);
	  if (data){
		  //Now We've Got A Wave In Memory, Time To Turn It Into A Usable Buffer
		  buffer = createBufferFromWave(data,header);
		  if (!buffer){
			  free(data);
		  }
	  }
	  LOGI("Wav Read");
	  // Create source from buffer and play it
	   ALuint source = 0;
	   alGenSources(1, &source );
	   alSourcei(source, AL_BUFFER, buffer);

	   // Play source
	   alSourcePlay(source);

	   int sourceState = AL_PLAYING;
	   do {
		   alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	   } while(sourceState == AL_PLAYING);

}

Audio::~Audio() {
	// TODO Auto-generated destructor stub
}

} /* namespace Pineapple */
