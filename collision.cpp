//
// Created by me on 9/2/24.
//

#include "r_common.h"
#include "collision.h"
#include <stdio.h>

EllipsoidCollider CreateEllipsoidColliderFromAABB(glm::vec3 mins, glm::vec3 maxs)
{
    float width = glm::abs(maxs.x - mins.x);
    float height = glm::abs(maxs.z - mins.z);
    EllipsoidCollider result{};
    float radiusA = width / 2.0f;
    float radiusB = height / 2.0f;
    result.radiusA = radiusA;
    result.radiusB = radiusB;
    result.center = glm::vec3(0.0f);
    glm::vec3 s = glm::vec3(1.0f / radiusA, 1.0f / radiusA, 1.0f / radiusB);
    glm::mat3 invESpace = glm::scale(glm::mat4(1.0f), s);
    result.toESpace = invESpace;

    return result;
}

bool CollideEllipsoidWithTriPlane(EllipsoidCollider ec, glm::vec3 velocity, TriPlane tp)
{
    Plane p = tp.plane;
    Tri tri = tp.tri;

    // Convert to ellipsoid space

    Tri eSpaceTri = TriToEllipsoidSpace(tri, ec.toESpace);
    Plane eSpacePlane = CreatePlaneFromTri(eSpaceTri);
    glm::vec3 eSpaceNormal = eSpacePlane.normal;
    glm::vec3 eSpaceVel = ec.toESpace * velocity;
    glm::vec3 eSpacePos = ec.toESpace * ec.center;

    // From now on the Radius of the ellipsoid is 1.0 in X, Y, Z.
    // This, it is a unit sphere.

    // Signed distance from plane to unit sphere's center

    float sD = glm::dot(eSpaceNormal, eSpacePos) + eSpacePlane.d;
    printf("sD: %f\n", sD);
    if (sD >= -1.0f && sD <= 1.0f) {
        return true;
    }

    // Project velocity along the plane normal
    float velDotNormal = glm::dot(eSpaceNormal, eSpaceVel);

    // Calculate t0, that is the time when the unit sphere hits the
    // front face of the plane.
    float t0 = (1.0f - sD) / velDotNormal;

    // As above, calculate t1, that is the time when the unit sphere
    // hits the back face of the plane.
    float t1 = (-1.0f - sD) / velDotNormal;

    // If both t0, t1 are outside the range [0, 1], then there is no collision!
    if ( (t0 < 0.0f || t0 > 1.0f) && (t1 < 0.0f || t1 > 1.0f) ) {
        return false;
    }

    return true;
}

Tri TriToEllipsoidSpace(Tri tri, glm::mat3 toESPace) {
    Tri result = tri;
    result.a.pos = toESPace * tri.a.pos;
    result.b.pos = toESPace * tri.b.pos;
    result.c.pos = toESPace * tri.c.pos;

    return result;
}
