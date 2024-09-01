//
// Created by me on 9/1/24.
//

#ifndef CWORLD_H
#define CWORLD_H

#include "r_common.h"
#include <vector>
#include <stdint.h>

// Static world geometry is made out of these
struct TriPlane {
    Plane plane;
    Tri   tri;
};

class CWorld {
public:
    void InitWorld(TriPlane* triPlanes, uint32_t triPlaneCount);

    std::vector<TriPlane> m_TriPlanes;
};



#endif //CWORLD_H
