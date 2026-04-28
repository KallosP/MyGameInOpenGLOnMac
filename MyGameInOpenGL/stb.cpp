//	By defining STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header 
//  file such that it only contains the relevant definition source code, effectively 
//  turning the header file into a .cpp file.
//  
//	Basically, we need this file to turn stb_image.h into a .cpp file so we can use it's functions

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"