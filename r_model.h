#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include <vector>

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "r_common.h"
#include "iqm_loader.h"

struct Model {
	std::vector<Tri> tris;
	int				 textureID;
};

Model CreateModelFromIQM(IQMModel* model);

#endif
