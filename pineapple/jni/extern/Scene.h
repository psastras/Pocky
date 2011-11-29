/*
 * Scene.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

namespace Pineapple {
class SceneObject;
class Scene {
public:
	Scene();
	virtual ~Scene();
	void addObject(SceneObject *object);
protected:
	std::vector<SceneObject *> objects_;
};

} /* namespace Pineapple */
#endif /* SCENE_H_ */
