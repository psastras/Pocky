/*
 * Scene.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <unordered_map>
#include <string>
#include "Objects.h"
namespace Pineapple {
class Scene {
public:
	Scene();
	virtual ~Scene();

	std::unordered_map<std::string, Object *> *objects() { return &objects_; }

	void addObject(const std::string &name, Object *object);
	void removeObject(const std::string &name);

	void update(int dt);

protected:
	std::unordered_map<std::string, Object *> objects_;

};

} /* namespace Pineapple */
#endif /* SCENE_H_ */
