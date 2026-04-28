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

//  NOTE: Code modified from https://youtu.be/z9YML6j5yDg?list=PLA0dXqQjCx0S9qG5dWLsheiCJV-_eLUM0


#include "fault_formation_terrain.h"

void FaultFormationTerrain::CreateFaultFormation(int TerrainSize, int Iterations, float MinHeight, float MaxHeight, float Filter)
{  
    terrainSize = TerrainSize;
    minHeight = MinHeight;
    maxHeight = MaxHeight;

    // enable terrain shader program
    terrainShader.use();
    // set uniforms
    terrainShader.setFloat("gMinHeight", MinHeight);
    terrainShader.setFloat("gMaxHeight", MaxHeight);

    heightMap.InitArray2D(TerrainSize, TerrainSize, 0.0f);

    CreateFaultFormationInternal(Iterations, MinHeight, MaxHeight, Filter);

    // need to normalize the min and max heights to prevent them from going outside of the required range
    heightMap.Normalize(MinHeight, MaxHeight);

    triangleList.CreateTriangleList(terrainSize, terrainSize, this);
}

// core algorithm implementation
void FaultFormationTerrain::CreateFaultFormationInternal(int Iterations, float MinHeight, float MaxHeight, float Filter)
{
    float DeltaHeight = MaxHeight - MinHeight;

    // loop over number of iterations 
    for (int CurIter = 0 ; CurIter < Iterations ; CurIter++) {
        // handles the decreasing change in height as more iterations are applied
        float IterationRatio = ((float)CurIter / (float)Iterations); // provides a fraction between 0 and almost 1
        // calculates the height by which one side of the random fault line will be increased
        float Height = MaxHeight - IterationRatio * DeltaHeight;

        TerrainPoint p1, p2;

        GenRandomTerrainPoints(p1, p2);

        // produces the vector between the two randomized points 
        int DirX = p2.x - p1.x;
        int DirZ = p2.z - p1.z;

        // iterate over all points in the grid (terrain) and increment the height of all the
        // points on one side of the random line, while keeping the other side unchanged
        for (int z = 0 ; z < terrainSize ; z++) {
            for (int x = 0 ; x < terrainSize ; x++) {
                int DirX_in = x - p1.x;
                int DirZ_in = z - p1.z;

                int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;

                // deciding to increment the height of all vectors whose cross product is positive
                // (i.e. on the 'positive' side of the random line)
                if (CrossProduct > 0) {
                    float CurHeight = heightMap.Get(x, z);
                    heightMap.Set(x, z, CurHeight + Height); // increasing the height of the current point
                }
            }
        }        
    }
	
    ApplyFIRFilter(Filter);
}


void FaultFormationTerrain::ApplyFIRFilter(float Filter)
{
    // left to right (sweep 1)
    for (int z = 0 ; z < terrainSize ; z++) {
        float PrevVal = heightMap.Get(0, z);
        for (int x = 1 ; x < terrainSize ; x++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // right to left (sweep 2)
    for (int z = 0 ; z < terrainSize ; z++) {
        float PrevVal = heightMap.Get(terrainSize - 1, z);
        for (int x = terrainSize - 2 ; x >= 0 ; x--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // bottom to top (sweep 3)
    for (int x = 0 ; x < terrainSize ; x++) {
        float PrevVal = heightMap.Get(x, 0);
        for (int z = 1 ; z < terrainSize ; z++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // top to bottom (sweep 4)
    for (int x = 0 ; x < terrainSize ; x++) {
        float PrevVal = heightMap.Get(x, terrainSize - 1);
        for (int z = terrainSize - 2 ; z >= 0 ; z--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }
}


float FaultFormationTerrain::FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter)
{
    float CurVal = heightMap.Get(x, z);
    float NewVal = Filter * PrevVal + (1 - Filter) * CurVal; // performs linear interpolation between prev value and new one
    heightMap.Set(x, z, NewVal);
    return NewVal;
}

// Generates two random points within the range of the terrain size
void FaultFormationTerrain::GenRandomTerrainPoints(TerrainPoint& p1, TerrainPoint& p2)
{
    p1.x = rand() % terrainSize;
    p1.z = rand() % terrainSize;

    int Counter = 0;

    // while loop to ensure we're not randomizing two identical points
    do {
        p2.x = rand() % terrainSize;
        p2.z = rand() % terrainSize;

		// safety check to prevent an infinite loop in the unlikely event that we keep randomizing the same point
        if (Counter++ == 1000) {
            printf("Endless loop detected in %s:%d\n", __FILE__, __LINE__);
            assert(0);
        }
    } while (p1.IsEqual(p2));
}