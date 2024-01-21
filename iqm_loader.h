#ifndef _IQM_LOADER_H_
#define _IQM_LOADER_H_

#include <stdint.h>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

struct IQMHeader {
    uint8_t  magic[16]; // "INTERQUAKEMODEL\0"
    uint32_t version;  // Must be version 2
    uint32_t filesize;
    uint32_t flags;
    uint32_t numText, ofsText;
    uint32_t numMeshes, ofsMeshes;
    uint32_t numVertArrays, numVertices, ofsVertArrays;
    uint32_t numTris, ofsTris, ofsAdjacency;
    uint32_t numJoints, ofsJoints;
    uint32_t numPoses, ofsPoses;
    uint32_t numAnims, ofsAnims;
    uint32_t numFrames, numFrameChannels, ofsFrames, ofsBounds;
    uint32_t numComment, ofsComment;
    uint32_t numExt, ofsExt;
};

struct IQMMesh {
    uint32_t name;
    uint32_t material;
    uint32_t firstVert, numVerts;
    uint32_t firstTri, numTris;
};

// Default Vertex format

struct IQMVertex {
    float pos[3];
    float texCoord[2];
    float normal[3];
    float tangent[4];
    uint8_t blendindices[4];
    uint8_t blendweights[4];
    uint8_t color[4];
};

// NOTE: Mesh, Pose and Anim structs are not part of IQM.
//       Maybe move them out of here later or something.

struct Mesh {
    char* material;
    IQMVertex* vertices;
    uint32_t firstTri, numTris;
};

struct Pose {
    int32_t parent;
    glm::vec3 translations;
    glm::vec3 scale;
    glm::quat rotation;
};

struct Anim {
    char*    name;
    uint32_t firstFrame, numFrames;
    float    framerate;
};

struct IQMData {
    Mesh* meshes;
    glm::mat4 bindPoses;
    glm::mat4 invBindPoses;
    Pose* poses;
    uint32_t numJoints;
    uint32_t numFrames;
    Anim* animations;
};

struct IQMVertArray {
    uint32_t type;
    uint32_t flags;
    uint32_t format;
    uint32_t size;
    uint32_t offset;
};

enum IQMVertArrayType {
    POSITION,
    TEXCOORD,
    NORMAL,
    TANGENT,
    BLENDINDIXES,
    BLENDWEIGHTS,
    COLOR,

    CUSTOM = 0x10
};

enum IQMVertArrayFormat {
    BYTE,
    UBYTE,
    USHORT,
    INT,
    UINT,
    HALF,
    FLOAT,
    DOUBLE
};

struct IQMTri {
    uint32_t vertex[3];
};

struct IQMJoint {
    uint32_t name;
    int32_t  parent;
    float translate[3];
    float rotate[4];
    float scale[3];
};

struct IQMPose {
    int32_t parent;
    uint32_t channelmask;
    float channeloffset[10];
    float channelscale[10];
};

struct IQMAnim {
    uint32_t name;
    uint32_t firstFrame, numFrames;
    float framerate;
    uint32_t flags;
};


IQMData LoadIQM(const char* file);

#endif
