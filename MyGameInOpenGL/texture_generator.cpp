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


#include "texture_generator.h"
//#include "ogldev_stb_image.h"

//#include "3rdparty/stb_image_write.h"

TextureGenerator::TextureGenerator()
{
}

void TextureGenerator::LoadTile(const char* pFilename)
{
    if (m_numTextureTiles == MAX_TEXTURE_TILES) {
        printf("%s:%d: exceeded the maximum of texture tiles with '%s'\n", __FILE__, __LINE__, pFilename);
        exit(0);
    }

    // FIXME: might not need this here (already being set to true in Material class).
	//         also it might be better to just set this to true globally once at start of program
    //         since textures always need to be flipped? keep here for now
	stbi_set_flip_vertically_on_load(true);

	TextureTile& currTile = m_textureTiles[m_numTextureTiles];
	// load the image data from the file into this data array
	unsigned char* imageData = stbi_load(pFilename, &currTile.width, 
		&currTile.height, &currTile.channels, 0);
	// store this tiles texture data
	m_textureTiles[m_numTextureTiles].imageData = imageData;

    m_numTextureTiles++;
}

// where is generatetexture called 
const char* TextureGenerator::GenerateTexture(int TextureSize, BaseTerrain* pTerrain, float MinHeight, float MaxHeight)
{
    if (m_numTextureTiles == 0) {
        printf("%s:%d: no texture tiles loaded\n", __FILE__, __LINE__);
        exit(0);
    }

    // calculates low, optimal, and hight height values
    CalculateTextureRegions(MinHeight, MaxHeight);

    int BPP = 3;
    int TextureBytes = TextureSize * TextureSize * BPP;
    unsigned char* pTextureData = (unsigned char*)malloc(TextureBytes);
    unsigned char* p = pTextureData;

    float HeightMapToTextureRatio = (float)pTerrain->GetSize() / (float)TextureSize;

    printf("Height map to texture ratio: %f\n", HeightMapToTextureRatio);

    for (int y = 0 ; y < TextureSize ; y++) {
        for (int x = 0 ; x < TextureSize ; x++) {

            // maps the texture pixel to the correct coordinate in the heightmap
            float InterpolatedHeight = pTerrain->GetHeightInterpolated((float)x * HeightMapToTextureRatio, 
                                                                       (float)y * HeightMapToTextureRatio);

            float Red = 0.0f;
            float Green = 0.0f;
            float Blue = 0.0f;

            for (int Tile = 0 ; Tile < m_numTextureTiles ; Tile++) {
                glm::vec3 Color = GetColor(x, y, Tile);

                float BlendFactor = RegionPercent(Tile, InterpolatedHeight);

                Red   += BlendFactor * Color.r;
                Green += BlendFactor * Color.g;
                Blue  += BlendFactor * Color.b;
            }

            if (Red > 255.0f || Green > 255.0f || Blue > 255.0f) {
                printf("%d:%d: %f %f %f\n", y, x, Red, Green, Blue);
                exit(0);
            }

            p[0] = (unsigned char)Red;
            p[1] = (unsigned char)Green;
            p[2] = (unsigned char)Blue;

            p += 3;
        }
    }

    const char* textureResultSrc = "texture.png";
    stbi_write_png(textureResultSrc, TextureSize, TextureSize, BPP, pTextureData, TextureSize * BPP);

    // after upload we can free also free stbi_image data
    free(pTextureData);
	FreeTextureData();

    return textureResultSrc;
}


void TextureGenerator::CalculateTextureRegions(float MinHeight, float MaxHeight)
{
    float HeightRange = MaxHeight - MinHeight;

    float RangePerTile = HeightRange / m_numTextureTiles;
    float Remainder = HeightRange - RangePerTile * m_numTextureTiles;

    if (Remainder < 0.0f) {
        printf("%s:%d: negative remainder %f (num tiles %d range per tile %f)\n", __FILE__, __LINE__, Remainder, m_numTextureTiles, RangePerTile);
        exit(0);
    }

    float LastHeight = -1.0f;

    for (int i = 0 ; i < m_numTextureTiles ; i++) {
        m_textureTiles[i].HeightDesc.Low = LastHeight + 1;
        LastHeight += RangePerTile;
        m_textureTiles[i].HeightDesc.Optimal = LastHeight;
        m_textureTiles[i].HeightDesc.High = m_textureTiles[i].HeightDesc.Optimal + RangePerTile;

        m_textureTiles[i].HeightDesc.Print(); printf("\n");
    }
}


float TextureGenerator::RegionPercent(int Tile, float Height)
{
    float Percent = 0.0f;

    if (Height < m_textureTiles[Tile].HeightDesc.Low) {
        Percent = 0.0f;
    } else if (Height > m_textureTiles[Tile].HeightDesc.High) {
        Percent = 0.0f;
    } else if (Height < m_textureTiles[Tile].HeightDesc.Optimal) {
        float Nom = (float)Height - (float)m_textureTiles[Tile].HeightDesc.Low;
        float Denom = (float)m_textureTiles[Tile].HeightDesc.Optimal - (float)m_textureTiles[Tile].HeightDesc.Low;
        Percent = Nom / Denom;
    } else if (Height >= m_textureTiles[Tile].HeightDesc.Optimal) {
        float Nom = (float)m_textureTiles[Tile].HeightDesc.High - (float)Height;
        float Denom = (float)m_textureTiles[Tile].HeightDesc.High - (float)m_textureTiles[Tile].HeightDesc.Optimal;
        Percent = Nom / Denom;
    } else {
        printf("%s:%d - shouldn't get here! tile %d Height %f\n", __FILE__, __LINE__, Tile, Height);
        exit(0);
    }

    if ((Percent < 0.0f) || (Percent > 1.0f)) {
        printf("%s:%d - Invalid percent %f\n", __FILE__, __LINE__, Percent);
        exit(0);
    }

    return Percent;
}

void TextureGenerator::FreeTextureData()
{
    for (int i = 0 ; i < m_numTextureTiles ; i++) {
        stbi_image_free(m_textureTiles[i].imageData);
    }
}

glm::vec3 TextureGenerator::GetColor(int x, int y, int tileIdx) {
    TextureTile currTile = m_textureTiles[tileIdx];

    assert(currTile.width > 0);
    assert(currTile.height > 0);

    int WrappedX = x % currTile.width;
    int WrappedY = y % currTile.height;

    glm::vec3 Color;

    unsigned char* p = currTile.imageData + (WrappedY * currTile.width + WrappedX) * currTile.channels;
    Color.r = (float)p[0];
    Color.g = (float)p[1];
    Color.b = (float)p[2];

    return Color;
}
