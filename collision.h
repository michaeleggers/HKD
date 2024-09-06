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

struct CollisionInfo {
    bool      didCollide;
    glm::vec3 hitPoint;
    float     t;        // Distance to travel before collision occurs.
    glm::vec3 normal;   // normal of the plane that was collided against.
};

EllipsoidCollider CreateEllipsoidColliderFromAABB(glm::vec3 mins, glm::vec3 maxs);
CollisionInfo CollideUnitSphereWithPlane(glm::vec3 pos, glm::vec3 velocity, Plane p);
CollisionInfo CollideEllipsoidWithTriPlane(EllipsoidCollider ec, glm::vec3 velocity, TriPlane tp);
Tri  TriToEllipsoidSpace(Tri tri, glm::mat3 toESPace);
Plane CreatePlaneFromTri(Tri tri);
bool IsPointInTriangle(glm::vec3 point, Tri tri, glm::vec3 triNormal);

#endif //COLLISION_H
