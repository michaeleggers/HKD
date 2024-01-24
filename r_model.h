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
	std::vector<Tri> tris;
	std::string		 textureFileName;
};

struct HKD_Model {
	std::string			  filename;
	std::vector<HKD_Mesh> meshes;
};

HKD_Model CreateModelFromIQM(IQMModel* model);

#endif
