#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "r_common.h"
#include "iqm_loader.h"

struct HKD_Mesh {
	uint32_t	firstTri, numTris;
	std::string	textureFileName;
};

struct HKD_Model {
	std::string				filename;
	std::vector<Tri>		tris;
	std::vector<HKD_Mesh>	meshes;
	int						gpuModelHandle;
	std::vector<Pose>		poses;
	std::vector<glm::mat4>	invBindPoses;
	std::vector<glm::mat4>	bindPoses;
	std::vector<glm::mat4>	palette;
	uint32_t				numJoints;
	std::vector<Anim>		animations;
	uint32_t				currentAnim;
};

HKD_Model CreateModelFromIQM(IQMModel* model);
void	  UpdateModel(HKD_Model* model);

#endif
