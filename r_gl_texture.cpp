#include "r_gl_texture.h"

#include "platform.h"
#include "stb_image.h"

GLuint CreateTexture(std::string textureFileName)
{
    std::string exePath = hkd_GetExePath();
    int x,y,n;
    unsigned char *data = stbi_load( (exePath + "../../assets/textures/" + textureFileName).c_str(), &x, &y, &n, 0);

    if (!data) {
        printf("WARNING: Failed to load texture: %s\n", textureFileName.c_str());
        return 0;
    }

    GLuint glTextureHandle;
    glGenTextures(1, &glTextureHandle);
    glBindTexture(GL_TEXTURE_2D, glTextureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLuint)x, (GLuint)y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return glTextureHandle;
}

