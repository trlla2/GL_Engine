#pragma once
#include <GL/glew.h>

class TextureLoader
{
public:
    GLuint Load(const char* path) const;
};
