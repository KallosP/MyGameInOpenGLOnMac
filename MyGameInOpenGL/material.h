#ifndef MATERIAL_H
#define MATERIAL_H
#include "config.h"

class Material {
    public:
        Material() {}
        Material(const char* filename);
        ~Material(); // destructor (frees up resources)
        void use(int unit);
    private:
        unsigned int texture; // acts like an ID number
};

#endif // MATERIAL_H
