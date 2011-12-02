/*
 * Objects.h
 *
 *  Created on: Dec 1, 2011
 *      Author: psastras
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "Float3.h"

class GLPrimitive;

struct ObjectParams {
	GLPrimitive *primitive;

	bool isVisible;
	bool isTouchable;
	Float3 position;
	Float3 rotation;
	float theta;

	Float3 dPosition;
	Float3 rotAxis;
	float dTheta;
};

class Object {
public:
	Object(const ObjectParams &params) : params_(params)
	{

	}

	~Object();

	ObjectParams *params() { return &params_; }

protected:
	ObjectParams params_;

};




#endif /* OBJECTS_H_ */
