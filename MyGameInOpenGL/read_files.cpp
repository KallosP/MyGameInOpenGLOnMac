/*

        Copyright 2014 Etay Meiri

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

#include "read_files.h"
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

char* ReadBinaryFile(const char* pFilename, int& size)
{
    FILE* f = fopen(pFilename, "rb");

    if (!f) {
        std::cout << "Error opening '" << pFilename << "': " << strerror(errno) << std::endl;
        exit(0);
    }

    struct stat stat_buf;
    int error = stat(pFilename, &stat_buf);

    if (error) {
        std::cout << "Error getting file stats:" << strerror(errno) << std::endl;
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    size_t bytes_read = fread(p, 1, size, f);

    if (bytes_read != size) {
        std::cout << "Read file error:" << strerror(errno) << std::endl;
        exit(0);
    }

    fclose(f);

    return p;
}

void WriteBinaryFile(const char* pFilename, const void* pData, int size)
{
    FILE* f = fopen(pFilename, "wb");

    if (!f) {
        std::cout << "Error opening '" << pFilename << "': " << strerror(errno) << std::endl;
        exit(0);
    }

    size_t bytes_written = fwrite(pData, 1, size, f);

    if (bytes_written != size) {
        std::cout << "Error write file: " << strerror(errno) << std::endl;
        exit(0);
    }

    fclose(f);
}
