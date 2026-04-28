#ifndef CUBE_H
#define CUBE_H
#include "config.h"
#include "material.h"
#include "cube_mesh.h"
#include "shader.h"
#include "entity.h"

// forward declaration (removes need for including camera.h which avoids circular dependency)
class Camera;

class Cube : public Entity {
	public:
		CubeMesh* cubeMesh;
		Material* material;
		Material* mask;
		Cube(const char* textureMaterialSrc, const char* textureMaskSrc = NULL);
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, 
			float SCR_HEIGHT, glm::vec3* pos, glm::vec3 scale, float yaw);
	private: 
};

#endif
