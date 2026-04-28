#include <stdio.h>
#include <vector>

//#include "ogldev_math_3d.h"
#include "triangle_list.h"
#include "terrain.h"

// NOTE: The purpose of this code is to abstract a lot of the OpenGL code away
//		 (e.g. all the vertex array/buffer setup) for the terrain related code

TriangleList::TriangleList()
{
}


void TriangleList::CreateTriangleList(int Width, int Depth, const BaseTerrain* pTerrain)
{
	m_width = Width;
	m_depth = Depth;

	CreateGLState();

	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void TriangleList::CreateGLState()
{
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	int POS_LOC = 0;
	int TEX_LOC = 1;

	size_t NumFloats = 0;
	
	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;

	glEnableVertexAttribArray(TEX_LOC);
	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;
}

// Populates the vertex buffer with the positions of the vertices in the grid (where grid = the terrain)
void TriangleList::PopulateBuffers(const BaseTerrain* pTerrain)
{
	std::vector<Vertex> Vertices;
	Vertices.resize(m_width * m_depth); // width * depth of grid yields the total number of vertices in the grid

	InitVertices(pTerrain, Vertices);

	std::vector<unsigned int> Indices;
	int NumQuads = (m_width - 1) * (m_depth - 1); // num quads in the grid (draw this out on paper if confused)
	Indices.resize(NumQuads * 6); // each quad is 2 triangles, each triangle is 3 vertices, so 6 total per quad. NumQuads * 6 = total number of vertices which = num indices
	InitIndices(Indices);

	// Called once vertices is populated with vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	// Upload the indices into the index buffer (EBO)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}


void TriangleList::Vertex::InitVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float y = pTerrain->GetHeight(x, z);

	float WorldScale = pTerrain->GetWorldScale();

	Pos = glm::vec3(x * WorldScale, y, -z * WorldScale);

	float Size = (float)pTerrain->GetSize();
	float TextureScale = pTerrain->GetTextureScale();
	// maps the x,z coordinates of the vertex to a value between 0 and 1, 
	// which is used for texture mapping (e.g. for the grass texture on the terrain)
	Tex = glm::vec2(TextureScale * (float)x / Size, TextureScale * (float)z / Size); 
}


// Performs the actual population of the Vertices vector (c++ vector aka dynamic list) with vertex positions
void TriangleList::InitVertices(const BaseTerrain* pTerrain, std::vector<Vertex>& Vertices)
{
	int Index = 0;

	// initializes a vertex for every point in the grid (i.e. every point in the terrain)
	for (int z = 0; z < m_depth; z++) {
		for (int x = 0; x < m_width; x++) {
			assert(Index < Vertices.size());
			Vertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}

    assert(Index == Vertices.size());
}


void TriangleList::InitIndices(std::vector<unsigned int>& Indices)
{
	int Index = 0;

	// iterate through every quad
	for (int z = 0; z < m_depth - 1; z++) {
		for (int x = 0; x < m_width - 1; x++) {
			// calculate the index of the 4 corners of the current quad from just the bottom left vertex of the current quad 
			// indices will increase from left to right on the grid, reading it from bottom to top, draw on paper if needed
			// --------------------------------------------------------
			unsigned int IndexBottomLeft = z * m_width + x;
			unsigned int IndexTopLeft = (z + 1) * m_width + x;
			unsigned int IndexTopRight = (z + 1) * m_width + x + 1;
			unsigned int IndexBottomRight = z * m_width + x + 1;

			// TODO: gain a better understanding of how exactly the winding order of
			//       the vertices in the triangles is affected by changing from a left handed
			//		 coord system to a right handed coord system

			// Left handed coordinate system, counter clockwise winding order
			// Add top left triangle
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexBottomLeft;
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexTopLeft;
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexTopRight;

			//// Add bottom right triangle
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexBottomLeft;
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexTopRight;
            //assert(Index < Indices.size());
			//Indices[Index++] = IndexBottomRight;

			// Right handed coordinate system, clockwise winding order
			// Add top left triangle
            assert(Index < Indices.size());
			Indices[Index++] = IndexBottomLeft;
            assert(Index < Indices.size());
			Indices[Index++] = IndexTopRight;
            assert(Index < Indices.size());
			Indices[Index++] = IndexTopLeft;

			// Add bottom right triangle
            assert(Index < Indices.size());
			Indices[Index++] = IndexBottomLeft;
            assert(Index < Indices.size());
			Indices[Index++] = IndexBottomRight;
            assert(Index < Indices.size());
			Indices[Index++] = IndexTopRight;
		}
	}

    assert(Index == Indices.size());
}


void TriangleList::Render()
{
	glBindVertexArray(VAO);

	int numIndices = (m_depth - 1) * (m_width - 1) * 6; // num quads * 6 indices per quad
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}
