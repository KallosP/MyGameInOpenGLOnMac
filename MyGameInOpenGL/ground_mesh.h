#pragma once
#include "config.h"

class GroundMesh {
	public:
		GroundMesh();
		void draw();
		~GroundMesh();
	private:
		unsigned int VBO, VAO, EBO;
};
