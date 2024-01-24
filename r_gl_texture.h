#ifndef _R_GL_TEXTURE_H_
#define _R_GL_TEXTURE_H_

#include <glad/glad.h>

#include <string>

struct GLTexture {
    std::string filename;
    GLuint      handle;
    int         width, height, channels;
};

GLTexture CreateTexture(std::string filename);


#endif
