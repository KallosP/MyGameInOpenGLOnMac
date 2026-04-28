#ifndef READ_FILES_H
#define READ_FILES_H

#include <iostream>
#include <fstream>
#include <assert.h>

//FIXME: getting link/build error  with ReadBinaryFile
char* ReadBinaryFile(const char* pFilename, int& size);
void WriteBinaryFile(const char* pFilename, const void* pData, int size);

#endif
