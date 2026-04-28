#pragma once
#include "config.h"

class Entity {
	public:
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Size = glm::vec3(1.0f, 1.0f, 1.0f); //scale
};