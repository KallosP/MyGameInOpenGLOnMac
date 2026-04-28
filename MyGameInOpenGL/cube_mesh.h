#pragma once
#include "config.h"

class CubeMesh {
	public:
		CubeMesh();
		void draw();
		~CubeMesh();
	private:
		unsigned int VBO, VAO, EBO;
};