#ifndef GROUND_H
#define GROUND_H
#include "config.h"
#include "material.h"
#include "ground_mesh.h"
#include "shader.h"
#include "camera.h"

class Ground {
	public:
		GroundMesh* groundMesh;
		Material* material;
		Material* mask;
		Ground(const char* textureMaterialSrc, const char* textureMaskSrc = NULL);
		void draw(Shader& shaderProgram, Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
	private: 
};

#endif