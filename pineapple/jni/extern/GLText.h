/*
 * GLText.h
 *
 *  Created on: Nov 30, 2011
 *      Author: psastras
 */

#ifndef GLTEXT_H_
#define GLTEXT_H_

#include <unordered_map>

namespace Pineapple {

class GLText {
public:
	GLText();
	virtual ~GLText();
protected:
	std::unordered_map<char, int> letterIdxs_;
};

} /* namespace Pineapple */
#endif /* GLTEXT_H_ */
