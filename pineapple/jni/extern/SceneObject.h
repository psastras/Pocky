/*
 * SceneObject.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef SCENEOBJECT_H_
#define SCENEOBJECT_H_

namespace Pineapple {

class SceneObject {
public:
	SceneObject();
	virtual ~SceneObject();

	void draw();
};

} /* namespace Pineapple */
#endif /* SCENEOBJECT_H_ */
