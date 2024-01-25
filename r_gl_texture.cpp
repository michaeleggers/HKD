#include "r_gl_texture.h"

#include "r_itexture.h"
#include "platform.h"
#include "stb_image.h"

#include <string>

// TODO: This is the texture manager at the moment...

GLTexture::GLTexture(std::string filename)
{
    std::string exePath = hkd_GetExePath();
    int x, y, n;
    unsigned char* data = stbi_load((exePath + "../../assets/textures/" + filename).c_str(), &x, &y, &n, 0);

    if (!data) {
        printf("WARNING: Failed to load texture: %s\n", filename.c_str());
        // return {}; // TODO: Load checkerboard texture instead.
    }

    GLuint glTextureHandle;
    glGenTextures(1, &glTextureHandle);
    glBindTexture(GL_TEXTURE_2D, glTextureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLuint)x, (GLuint)y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    m_Filename = filename;
    m_gl_Handle = glTextureHandle;
    m_Width= x;
    m_Height = y;
    m_Channels = 3;

    m_hGPU = (uint64_t)glTextureHandle;    
}




