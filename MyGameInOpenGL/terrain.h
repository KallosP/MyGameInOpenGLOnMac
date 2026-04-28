#ifndef TERRAIN_H
#define TERRAIN_H

#include "read_files.h"
#include "config.h"
#include "array_2D.h"
#include "triangle_list.h"
#include "camera.h"
#include "shader.h"
#include "math.h"
#include "material.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <string.h>
// stores/renders the heightmap

class BaseTerrain {
	public:
		BaseTerrain() {};

		void InitTerrain(float worldScale, float TextureScale);

		void Render(Camera& camera, Material& terrainMat, std::vector<std::unique_ptr<Material>>& textureMats, float SCR_WIDTH, float SCR_HEIGHT);
		 
		void LoadFromFile(const char* pFilename); // loads a heightmap from a file

		// fetches the corresponding height value (y) from the provided x,z coordinates
		float GetHeight(int x, int z) const { return heightMap.Get(x, z); }

		float GetHeightInterpolated(float x, float z) const;

		int GetSize() const { return terrainSize; }

		float GetWorldScale() const { return worldScale; }

		float GetTextureScale() const { return textureScale; }

	protected:
		void LoadHeightMapFile(const char* pFilename);

		// width * depth of the height map = terrain size
		int terrainSize = 0;
		// attribute for the heightmap itself
		Array2D<float> heightMap;
		TriangleList triangleList;
		float worldScale = 1.0f;
		float textureScale = 1.0f;
		float minHeight = 0.0f;
		float maxHeight = 0.0f;

		// location of the "VP" uniform variable declared in the GLSL vertex shader code (VP = view-projection matrix)
		//unsigned int m_VPLoc = -1;
		Shader terrainShader;
};


#endif 

