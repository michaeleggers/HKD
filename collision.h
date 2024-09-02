//
// Created by me on 9/2/24.
//

#ifndef COLLISION_H
#define COLLISION_H

#define GLM_FORCE_RADIANS
#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/ext.hpp"

#include "r_common.h"

struct EllipsoidCollider {
    glm::vec3	center;  // Pos in worldspace
    float		radiusA; // horizontal radius
    float		radiusB; // vertical radius
    glm::mat3   toESpace; // Maps from World to ellipsoid (unit-sphere) space
};

EllipsoidCollider CreateEllipsoidColliderFromAABB(glm::vec3 mins, glm::vec3 maxs);
bool CollideEllipsoidWithTriPlane(EllipsoidCollider ec, glm::vec3 velocity, TriPlane tp);
Tri  TriToEllipsoidSpace(Tri tri, glm::mat3 toESPace);

#endif //COLLISION_H
