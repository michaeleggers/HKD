#include "r_model.h"

#include "r_common.h"

Model CreateModelFromIQM(IQMModel* model)
{
    Model result = {};

    for (int i = 0; i < model->meshes.size(); i++) {
        Mesh* mesh = &model->meshes[i];
        for (int v = 0; v < mesh->vertices.size(); v++) {
            IQMVertex iqmVert = mesh->vertices[v];
            Vertex vertex = {};

        }

    }

    return Model();
}
