//
// Created by me on 9/2/24.
//

#ifndef COLLISION_H
#define COLLISION_H

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

struct EllipsoidCollider {
    glm::vec3	center;  // Pos in worldspace
    float		radiusA; // horizontal radius
    float		radiusB; // vertical radius
};



#endif //COLLISION_H
