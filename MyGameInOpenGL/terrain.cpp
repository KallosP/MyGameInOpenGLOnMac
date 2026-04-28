#include "terrain.h"

void BaseTerrain::InitTerrain(float w, float TextureScale) {
	worldScale = w;
	textureScale = TextureScale;

	terrainShader = Shader("terrain.vs", "terrain.fs");

}

void BaseTerrain::LoadFromFile(const char* pFilename) {
	LoadHeightMapFile(pFilename);
	triangleList.CreateTriangleList(terrainSize, terrainSize, this);
}

void BaseTerrain::LoadHeightMapFile(const char* pFilename) {
	int FileSize = 0;
	unsigned char* p = (unsigned char*) ReadBinaryFile(pFilename, FileSize);

	// ensure the file has a whole number of floating point values
	assert(FileSize % sizeof(float) == 0); 

	// heightmap is square, so width = depth = sqrt(total number of floats)
	// need to divide by sizeof(float) to get the number of floats in the file
	terrainSize = (int)sqrt((float)FileSize / (float)sizeof(float));

	heightMap.InitArray2D(terrainSize, terrainSize, p);
}

float BaseTerrain::GetHeightInterpolated(float x, float z) const
{
    float BaseHeight = GetHeight((int)x, (int)z);

    if (((int)x + 1 >= terrainSize) ||  ((int)z + 1 >= terrainSize)) {
        return BaseHeight;
    }

    float NextXHeight = GetHeight((int)x + 1, (int)z);

    float RatioX = x - floorf(x);

    float InterpolatedHeightX = (float)(NextXHeight - BaseHeight) * RatioX + (float)BaseHeight;

    float NextZHeight = GetHeight((int)x, (int)z + 1);

    float RatioZ = z - floorf(z);

    float InterpolatedHeightZ = (float)(NextZHeight - BaseHeight) * RatioZ + (float)BaseHeight;

    float FinalHeight = (InterpolatedHeightX + InterpolatedHeightZ) / 2.0f;

    return FinalHeight;
}

void BaseTerrain::Render(Camera& camera, Material& terrainMat, std::vector<std::unique_ptr<Material>>& textureMats, float SCR_WIDTH, float SCR_HEIGHT) {
	glm::mat4 VP = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, camera.RenderDistance) * camera.GetViewMatrix();
	terrainShader.use();
	terrainShader.setMat4("gVP", VP);
	terrainMat.use(0);
    for (int i = 0; i < textureMats.size(); i++) {
		textureMats[i]->use(i+1);
		// generating uniform variables dynamically,
		// corresponds to the uniform variables declared in fragment shader
		std::string uniformName = "gTextureHeight" + std::to_string(i); 
		terrainShader.setInt(uniformName, i + 1);
    }

	triangleList.Render();
}
