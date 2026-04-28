/*

        Copyright 2023 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef TEXTURE_GENERATOR_H
#define TEXTURE_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "terrain.h"

struct TextureHeightDesc {
    float Low = 0.0f;
    float Optimal = 0.0f;
    float High = 0.0f;

    void Print() const { printf("Low %f Optimal %f High %f", Low, Optimal, High); }
};


struct TextureTile {
    int width, height, channels;
    glm::vec3 colorData;
    TextureHeightDesc HeightDesc;
    unsigned char* imageData;

	//glm::vec3 GetColor(int x, int y, int tileIdx) {
	//	TextureTile currTile = m_textureTiles[tileIdx];

	//	assert(currTile.width > 0);
	//	assert(currTile.height > 0);

	//	int WrappedX = x % currTile.width;
	//	int WrappedY = y % currTile.height;

	//	glm::vec3 Color;

	//	unsigned char* p = currTile.imageData + (WrappedY * currTile.width + WrappedX) * currTile.channels;
	//	Color.r = (float)p[0];
	//	Color.g = (float)p[1];
	//	Color.b = (float)p[2];

	//	return Color;
	//}
};


class BaseTerrain;

class TextureGenerator {
 public:
    TextureGenerator();

    void LoadTile(const char* Filename);

    const char* GenerateTexture(int TextureSize, BaseTerrain* pTerrain, float MinHeight, float MaxHeight);

 private:
	 void FreeTextureData();

    glm::vec3 GetColor(int x, int y, int tileIdx);

    void CalculateTextureRegions(float MinHeight, float MaxHeight);

    float RegionPercent(int Tile, float Height);

    #define MAX_TEXTURE_TILES 4

    TextureTile m_textureTiles[MAX_TEXTURE_TILES] = {};
    int m_numTextureTiles = 0;
};

#endif