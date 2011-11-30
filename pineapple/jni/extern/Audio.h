/*
 * Audio.h
 *
 *  Created on: Nov 29, 2011
 *      Author: psastras
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdlib.h>

namespace Pineapple {

class Audio {
public:
	Audio(void *data, size_t size);
	virtual ~Audio();
};

} /* namespace Pineapple */
#endif /* AUDIO_H_ */
