#pragma once
#include <GL/glew.h>

// encapsula la carga de texturas PNG a la GPU usando stb_image + OpenGL
class TextureLoader
{
public:
    // carga una imagen desde disco y devuelve el ID de textura OpenGL.
    // Devuelve 0 si la carga falla.
    GLuint Load(const char* path) const;
};