#include "material.h"
#include "stb_image.h"

Material::Material(const char* filename) {
    int width, height, channels;
    // make image up-right
    stbi_set_flip_vertically_on_load(true);  

    // Add relative file path to texture file
    std::string filepath = std::string("../MyGameInOpenGL/") + filename;
    // load the image data from the file into this data array
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha); // data is an array of unsigned bytes, loads the texture

    // create a texture object on the GPU
    glGenTextures(1, &texture); // we're asking openGL for texture ID and storing it in texture (defined in material.h)
    glBindTexture(GL_TEXTURE_2D, texture); // we're telling openGL "ok I want to work with this texture now" -> that's what binding is/means
    // now there's an empty texture object on the GPU // we're telling openGL "ok I want to work with this texture now" -> that's what binding is/means
    // now there's an empty texture object on the GPU

    // upload the image data (aka pixel data) to the graphics card (pixel array goes from CPU -> GPU)
    glTexImage2D(GL_TEXTURE_2D, 
        0, GL_RGBA, width, height, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, data); 
    // now the texture object on the GPU is no longer empty and contains the image data 

    // free data (b/c we've succeeded in copying our image data (i.e. our texture) to the GPU)
    stbi_image_free(data);

    // configure the sampler (defining how the GPU will read the texture (that is, the texture that is currently bound/binded)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //TODO: will need to change the filtering method for terrain texture (i.e. add a condition)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // minifying filter describes how to sample an image when it is shrunk down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnifying filter 

    glGenerateMipmap(GL_TEXTURE_2D);
}

Material::~Material() { // (destructor)
    glDeleteTextures(1, &texture); // 1 = num textures we're deleting
}

void Material::use(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture); // binding texture #2, texture #3, etc
}