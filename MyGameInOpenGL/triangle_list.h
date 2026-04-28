#ifndef TRIANGLE_LIST_H
#define TRIANGLE_LIST_H

#include <vector>
#include "config.h"

// This code is based on tutorial: https://www.youtube.com/watch?v=xoqESu9iOUE&list=PLA0dXqQjCx0S9qG5dWLsheiCJV-_eLUM0&index=2
// Original source code: https://github.com/emeiri/ogldev/blob/master/Terrain1/triangle_list.h

// this header is included by terrain.h so we have a forward 
// declaration for BaseTerrain.
class BaseTerrain;

// NOTE/TODO: the implementation of this class is generic, might want to refactor to fit your project after it works

class TriangleList {
public:
	TriangleList();

	void CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain);

	void Render();

private:

	struct Vertex {
		glm::vec3 Pos;
		glm::vec2 Tex; // texture

		void InitVertex(const BaseTerrain* pTerrain, int x, int z);
	};

	void CreateGLState();

	void PopulateBuffers(const BaseTerrain* pTerrain);
	void InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices);
	void InitIndices(std::vector<unsigned int>& Indices);

	int m_width = 0;
	int m_depth = 0;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

#endif