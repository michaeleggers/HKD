#include "r_model.h"


#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"
#include "dependencies/glm/gtx/quaternion.hpp"

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
    vertex.blendweights /= 255.0f;
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
        mesh.textureFileName = iqmMesh->material;
        mesh.firstTri = iqmMesh->firstTri;
        mesh.numTris = iqmMesh->numTris;
        for (int v = 0; v < iqmMesh->vertices.size(); v += 3) {

            IQMVertex iqmVertA = iqmMesh->vertices[v + 0];
            IQMVertex iqmVertB = iqmMesh->vertices[v + 1];
            IQMVertex iqmVertC = iqmMesh->vertices[v + 2];

            Vertex vertA = IQMVertexToVertex(iqmVertA, glm::vec3(1.0, 0.0, 0.0));
            Vertex vertB = IQMVertexToVertex(iqmVertB, glm::vec3(0.0, 1.0, 0.0));
            Vertex vertC = IQMVertexToVertex(iqmVertC, glm::vec3(0.0, 0.0, 1.0));

            Tri tri = { vertA, vertB, vertC };

            result.tris.push_back(tri);
        }
        result.meshes.push_back(mesh);
    }

    result.filename     = model->filename;
    result.poses        = model->poses;
    result.invBindPoses = model->invBindPoses;
    result.bindPoses    = model->bindPoses;
    result.numJoints    = model->numJoints;
    result.animations   = model->animations;
    result.numFrames    = model->numFrames;
    result.currentAnimIdx = 0;
    result.pctFrameDone = 0.0f;
    result.palette.resize(model->numJoints);

    return result;
}

static glm::mat4 PoseToMatrix(Pose pose) 
{
    glm::vec3 t = glm::vec3(pose.translations.x, pose.translations.y, pose.translations.z);
    glm::mat4 tMat = glm::translate(glm::mat4(1.0f), t); // TODO: Change to translation
    glm::vec3 s = glm::vec3(pose.scale.x, pose.scale.y, pose.scale.z);
    glm::mat4 sMat = glm::scale(glm::mat4(1.0f), s);
    //glm::quat r = glm::quat(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);
    glm::mat4 rMat = glm::toMat4(pose.rotation);

    return tMat * rMat * sMat;
}

static glm::mat4 InterpolatePoses(Pose a, Pose b, float pct) 
{
    glm::vec3 transA = a.translations;
    glm::vec3 transB = b.translations;
    glm::vec3 iTrans = (1.0f - pct) * transA + pct * transB;
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), iTrans);

    glm::vec3 scaleA = a.scale;
    glm::vec3 scaleB = b.scale;
    glm::vec3 iScale = (1.0f - pct) * scaleA + pct * scaleB;
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), iScale);

    glm::quat iRot = glm::slerp(a.rotation, b.rotation, pct);
    glm::mat4 rotMat = glm::toMat4(iRot);

    return transMat * rotMat * scaleMat;    
}

void UpdateModel(HKD_Model* model, float dt)
{
    uint32_t currentFrame = model->currentFrame;    
    uint32_t animIdx = model->currentAnimIdx;    

    Anim anim = model->animations[animIdx];
    if (currentFrame >= anim.firstFrame + anim.numFrames) {
        model->currentAnimIdx = (model->currentAnimIdx + 1) % model->animations.size();
        anim = model->animations[model->currentAnimIdx];
        currentFrame = anim.firstFrame;
    }
    uint32_t nextFrame = (currentFrame + 1) % (anim.firstFrame + anim.numFrames);

    //currentFrame = 0;

    // Build the matrix palette

    std::vector<glm::mat4> currentPoses;
    currentPoses.resize(model->numJoints);
    for (int i = 0; i < model->numJoints; i++) {
        Pose currentPoseTransform = model->poses[currentFrame * model->numJoints + i];        
        Pose nextPoseTransform = model->poses[nextFrame * model->numJoints + i];
        // parentGlobal * globalMat * poseMatrix * invGlobalMat * vertex;
        glm::mat4 poseMat = InterpolatePoses(currentPoseTransform, nextPoseTransform, model->pctFrameDone);
        //glm::mat4 poseMat = PoseToMatrix(currentPoseTransform);
        if (currentPoseTransform.parent >= 0) {
            currentPoses[i] = currentPoses[currentPoseTransform.parent] * poseMat;
        }
        else {
            currentPoses[i] = poseMat;
        }
    }    

    for (int i = 0; i < model->numJoints; i++) {
        glm::mat4 invGlobalMat = model->invBindPoses[i];               
        model->palette[i] = currentPoses[i] * invGlobalMat;
    }

    model->pctFrameDone += dt;
    if (model->pctFrameDone > 1.0f) {
        model->pctFrameDone = 0.0f;
        model->currentFrame = currentFrame;
        model->currentFrame++;    
    }
}
