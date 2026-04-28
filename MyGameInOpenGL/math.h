#pragma once

/*

        Copyright 2010 Etay Meiri

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


// Code modified from tutorial https://youtu.be/qNoSliX_6Gc?list=PLA0dXqQjCx0S9qG5dWLsheiCJV-_eLUM0

#include <random>

// inline allows for multiple identical definitions of this function across files
inline float RandomFloatRange(float min, float max) {
    // Random device (seed)
    static std::random_device rd; // static to avoid re-seeding every call, make the initial seed persist between calls
    // Mersenne Twister engine
    static std::mt19937 gen(rd());
    // Define float range [min, max]
    std::uniform_real_distribution<float> dist(min, max);
    // Generate random number
    float randomValue = dist(gen);
    return randomValue;
}

inline int CalcNextPowerOfTwo(int x) {
    int ret = 1;

    if (x == 1) {
        return 2;
    }

    while (ret < x) {
        ret = ret * 2;
    }

    return ret;
}
