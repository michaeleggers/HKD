#include "r_gl_batch.h"

#include <glad/glad.h>

#include <string>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "r_common.h"
#include "r_gl_texture.h" 

//struct Vertex {
//    glm::vec3 pos;
//    glm::vec2 uv;
//    glm::vec3 bc;
//    glm::vec3 normal;
//    glm::vec4 color;
//    uint32_t  blendindices[4];
//    glm::vec4 blendweights;
//};

GLBatch::GLBatch(uint32_t maxVerts)
{
    m_MaxVerts = maxVerts;

    m_NumVerts = 0;
    m_VertOffsetIndex = 0;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_MaxVerts * sizeof(Vertex), nullptr, GL_STATIC_DRAW);

    // Input assembly for vertex shader

    glEnableVertexAttribArray(0); // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_POS_OFFSET);

    glEnableVertexAttribArray(1); // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_UV_OFFSET);

    glEnableVertexAttribArray(2); // bc
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_BC_OFFSET);

    glEnableVertexAttribArray(3); // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_NORMAL_OFFSET);

    glEnableVertexAttribArray(4); // color
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_COLOR_OFFSET);

    glEnableVertexAttribArray(5); // blendindices
    glVertexAttribIPointer(5, 4, GL_UNSIGNED_INT, sizeof(Vertex), (void*)VERT_BLENDINDICES_OFFSET);

    glEnableVertexAttribArray(6); // blendweights
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)VERT_BLENDWEIGHTS_OFFSET);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int GLBatch::Add(Tri* tris, uint32_t numTris, bool cullFace, DrawMode drawMode)
{
    if (m_VertOffsetIndex + 3*numTris > m_MaxVerts) {
        printf("No more space on GPU to upload more triangles!\nSpace available: %d\n", m_MaxVerts - m_VertOffsetIndex);
        return -1;
    }

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_VertOffsetIndex * sizeof(Vertex), numTris * sizeof(Tri), tris);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);    

    int offset = m_VertOffsetIndex;
    m_VertOffsetIndex += 3*numTris;

    return offset;
}

int GLBatch::Add(Vertex* verts, uint32_t numVerts, bool cullFace, DrawMode drawMode)
{
    if (m_VertOffsetIndex + numVerts > m_MaxVerts) {
        printf("No more space on GPU to upload more vertices!\nSpace available: %d\n", m_MaxVerts - m_VertOffsetIndex);
        return -1;
    }

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, m_VertOffsetIndex * sizeof(Vertex), numVerts * sizeof(Vertex), verts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int offset = m_VertOffsetIndex;
    m_VertOffsetIndex += numVerts;

    return offset;
}

void GLBatch::Bind()
{
    glBindVertexArray(m_VAO);
}

void GLBatch::Reset()
{
    m_NumVerts = 0;
    m_VertOffsetIndex = 0;    
}

void GLBatch::Kill()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

uint32_t GLBatch::VertCount()
{
    return m_VertOffsetIndex;
}


