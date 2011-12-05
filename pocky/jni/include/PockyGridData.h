/*
 * PockyGridData.h
 *
 *  Created on: Dec 4, 2011
 *      Author: psastras
 */

#ifndef POCKYGRIDDATA_H_
#define POCKYGRIDDATA_H_

#include <pineapple/jni/extern/Float3.h>

namespace Pocky {



struct GridData
{
	int x, y; // grid x and y


	Float3 wspos;
};


class PockyGridData {
public:
	PockyGridData();
	virtual ~PockyGridData();
};

} /* namespace Pineapple */
#endif /* POCKYGRIDDATA_H_ */
