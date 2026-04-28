/*

        Copyright 2022 Etay Meiri

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

#include "midpoint_disp_terrain.h"

void MidpointDispTerrain::CreateMidpointDisplacement(int TerrainSize, float Roughness, float MinHeight, float MaxHeight)
{
    if (Roughness < 0.0f) {
        printf("%s: roughness must be positive - %f\n", __FUNCTION__, Roughness);
        exit(0);
    }

    terrainSize = TerrainSize;
    minHeight = MinHeight;
    maxHeight = MaxHeight;

    terrainShader.use();
	terrainShader.setFloat("gMinHeight", MinHeight);
	terrainShader.setFloat("gMaxHeight", MaxHeight);

    heightMap.InitArray2D(TerrainSize, TerrainSize, 0.0f);

    CreateMidpointDisplacementF32(Roughness);

    heightMap.Normalize(MinHeight, MaxHeight);

    triangleList.CreateTriangleList(terrainSize, terrainSize, this);
}


void MidpointDispTerrain::CreateMidpointDisplacementF32(float Roughness)
{
    int RectSize = CalcNextPowerOfTwo(terrainSize);
    float CurHeight = (float)RectSize / 2.0f;
    float HeightReduce = pow(2.0f, -Roughness);

    while (RectSize > 0) {

        DiamondStep(RectSize, CurHeight);

        SquareStep(RectSize, CurHeight);

        RectSize /= 2;
        CurHeight *= HeightReduce;
    }
}


void MidpointDispTerrain::DiamondStep(int RectSize, float CurHeight)
{
    int HalfRectSize = RectSize / 2;

    for (int y = 0 ; y < terrainSize ; y += RectSize) {
        for (int x = 0 ; x < terrainSize ; x += RectSize) {
			int next_x = (x + RectSize) % terrainSize; // using mod to wrap around to the other side of the heightmap when we reach the end of the current row/column
            int next_y = (y + RectSize) % terrainSize;

            if (next_x < x) {
                next_x = terrainSize - 1;
            }

            if (next_y < y) {
                next_y = terrainSize - 1;
            }

            float TopLeft     = heightMap.Get(x, y);
            float TopRight    = heightMap.Get(next_x, y);
            float BottomLeft  = heightMap.Get(x, next_y);
            float BottomRight = heightMap.Get(next_x, next_y);

            int mid_x = (x + HalfRectSize) % terrainSize;
            int mid_y = (y + HalfRectSize) % terrainSize;

            float RandValue = RandomFloatRange(-CurHeight, CurHeight);
            float MidPoint = (TopLeft + TopRight + BottomLeft + BottomRight) / 4.0f;

            heightMap.Set(mid_x, mid_y, MidPoint + RandValue);
        }
    }
}


void MidpointDispTerrain::SquareStep(int RectSize, float CurHeight)
{
    /*                ----------------------------------
                      |                                |
                      |           PrevYCenter          |
                      |                                |
                      |                                |
                      |                                |
    ------------------CurTopLeft..CurTopMid..CurTopRight
                      |                                |
                      |                                |
       CurPrevXCenter CurLeftMid   CurCenter           |
                      |                                |
                      |                                |
                      CurBotLeft------------------------

       CurTopMid = avg(PrevYCenter, CurTopLeft, CurTopRight, CurCenter)
       CurLeftMid = avg(CurPrevXCenterm CurTopleft, CurBotLeft, CurCenter)
    */

    int HalfRectSize = RectSize / 2;

    for (int y = 0 ; y < terrainSize ; y += RectSize) {
        for (int x = 0 ; x < terrainSize ; x += RectSize) {
            int next_x = (x + RectSize) % terrainSize;
            int next_y = (y + RectSize) % terrainSize;

            if (next_x < x) {
                next_x = terrainSize - 1;
            }

            if (next_y < y) {
                next_y = terrainSize - 1;
            }

            int mid_x = (x + HalfRectSize) % terrainSize;
            int mid_y = (y + HalfRectSize) % terrainSize;
              
            int prev_mid_x = (x - HalfRectSize + terrainSize) % terrainSize;
            int prev_mid_y = (y - HalfRectSize + terrainSize) % terrainSize;

            // 6 points required in average height calculation of 2 midpoints of adjacent rectangles
            float CurTopLeft  = heightMap.Get(x, y);
            float CurTopRight = heightMap.Get(next_x, y);
            float CurCenter   = heightMap.Get(mid_x, mid_y);
            float PrevYCenter = heightMap.Get(mid_x, prev_mid_y);
            float CurBotLeft  = heightMap.Get(x, next_y);
            float PrevXCenter = heightMap.Get(prev_mid_x, mid_y);

            // midpoint calculations
            float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);
            float CurTopMid  = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);

            heightMap.Set(mid_x, y, CurTopMid);
            heightMap.Set(x, mid_y, CurLeftMid);
        }
    }
}