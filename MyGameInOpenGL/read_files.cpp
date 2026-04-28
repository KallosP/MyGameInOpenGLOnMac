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

char* ReadBinaryFile(const char* pFilename, int& size)
{
    FILE* f = NULL;

    errno_t err = fopen_s(&f, pFilename, "rb");

    if (!f) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        std::cout << "Error opening '" << pFilename << "': " << buf << std::endl;
        exit(0);
    }

    struct stat stat_buf;
    int error = stat(pFilename, &stat_buf);

    if (error) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        std::cout << "Error getting file stats:" << buf << std::endl;
        return NULL;
    }

    size = stat_buf.st_size;

    char* p = (char*)malloc(size);
    assert(p);

    size_t bytes_read = fread(p, 1, size, f);

    if (bytes_read != size) {
        char buf[256] = { 0 };
        strerror_s(buf, sizeof(buf), err);
        std::cout << "Read file error file:" << buf << std::endl;
        exit(0);
    }

    fclose(f);

    return p;
}

void WriteBinaryFile(const char* pFilename, const void* pData, int size)
{
    FILE* f = NULL;
    
    errno_t err = fopen_s(&f, pFilename, "wb"); 

    if (!f) {
		std::cout << "Error opening '" << pFilename << "': " << err << std::endl;
        exit(0);
    }

    size_t bytes_written = fwrite(pData, 1, size, f);

    if (bytes_written != size) {
		std::cout << "Error write file: " << err << std::endl;
        exit(0);
    }

    fclose(f);
}
