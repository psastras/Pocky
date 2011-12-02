/*
 * Scene.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/Scene.h"
#include "../extern/Objects.h"

namespace Pineapple {

Scene::Scene() {
	// TODO Auto-generated constructor stub

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::update(int dt)
{
	for(auto it = objects_.begin(); it != objects_.end(); it++)
	{
		Object *obj = it->second;
		ObjectParams *params = obj->params();

		params->position += params->dPosition * dt;
	}
}

} /* namespace Pineapple */
