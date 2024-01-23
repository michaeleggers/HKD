#include "r_model.h"

#include "r_common.h"

//glm::vec3 pos;
//glm::vec3 uv;
//glm::vec3 bc;
//glm::vec3 normal;
//glm::vec3 color;
//uint32_t  blendindices[4];
//glm::vec4 blendweights;

Vertex IQMVertexToVertex(IQMVertex iqmVert, glm::vec3 bc) {
    Vertex vertex = {
        .pos = glm::vec3(iqmVert.pos[0], iqmVert.pos[1], iqmVert.pos[2]),
        .uv = glm::vec2(iqmVert.texCoord[0], iqmVert.texCoord[1]),
        .bc = bc,
        .normal = glm::vec3(iqmVert.normal[0], iqmVert.normal[1], iqmVert.normal[2]),
        .color = glm::vec4(iqmVert.color[0], iqmVert.color[1], iqmVert.color[2], iqmVert.color[3]),
        .blendweights = glm::vec4(iqmVert.blendweights[0], iqmVert.blendweights[1], iqmVert.blendweights[2], iqmVert.blendweights[3])
    };
    vertex.blendindices[0] = iqmVert.blendindices[0];
    vertex.blendindices[1] = iqmVert.blendindices[1];
    vertex.blendindices[2] = iqmVert.blendindices[2];
    vertex.blendindices[3] = iqmVert.blendindices[3];

    return vertex;
}

HKD_Model CreateModelFromIQM(IQMModel* model)
{
    HKD_Model result = {};

    for (int i = 0; i < model->meshes.size(); i++) {
        IQMMesh* iqmMesh = &model->meshes[i];
        HKD_Mesh mesh = {};
        for (int v = 0; v < iqmMesh->vertices.size(); v += 3) {

            IQMVertex iqmVertA = iqmMesh->vertices[v + 0];
            IQMVertex iqmVertB = iqmMesh->vertices[v + 1];
            IQMVertex iqmVertC = iqmMesh->vertices[v + 2];

            Vertex vertA = IQMVertexToVertex(iqmVertA, glm::vec3(1.0, 0.0, 0.0));
            Vertex vertB = IQMVertexToVertex(iqmVertB, glm::vec3(0.0, 1.0, 0.0));
            Vertex vertC = IQMVertexToVertex(iqmVertC, glm::vec3(0.0, 0.0, 1.0));

            Tri tri = { vertA, vertB, vertC };

            mesh.tris.push_back(tri);
            mesh.textureFileName = iqmMesh->material;
        }
        result.meshes.push_back(mesh);
    }

    return result;
}
