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

CollisionInfo CollideUnitSphereWithPlane(glm::vec3 pos, glm::vec3 velocity, Plane p)
{
    glm::vec3 normal = p.normal;
    glm::vec3 ptOnPlane = p.d * normal;
    glm::vec3 basePos = pos;

    // Signed distance from plane to unit sphere's center

    float sD = glm::dot(normal, basePos - ptOnPlane);
    printf("sD: %f\n", sD);
    if (sD >= -1.0f && sD <= 1.0f) { // Sphere is already inside the plane.
        glm::vec3 hitPoint = basePos - sD * normal;
        printf("STUCK\n");

        return { true, hitPoint, (1.0f - glm::abs(sD)), normal };
    }

    // Project velocity along the plane normal
    float velDotNormal = glm::dot(normal, velocity);

    // Calculate t0, that is the time when the unit sphere hits the
    // front face of the plane.
    float t0 = (1.0f - sD) / velDotNormal;

    // As above, calculate t1, that is the time when the unit sphere
    // hits the back face of the plane.
    float t1 = (-1.0f - sD) / velDotNormal;

    // If both t0, t1 are outside the range [0, 1], then there is no collision!
    if ( (t0 < 0.0f || t0 > 1.0f) && (t1 < 0.0f || t1 > 1.0f) ) {
        return {false, glm::vec3(0.0f), t0, glm::vec3(0.0f) }; // Actually could also be t1?
    }

    glm::vec3 hitPoint = basePos;
    float t = 0.0f;
    if (t0 <= 1.0f) {
        hitPoint += t0 * velocity + normal;
        t = t0;
    }
    else if (t1 >= -1.0f) {
        hitPoint += t1 * velocity + normal;
        t = t1;
    }

    return { true, hitPoint, t, normal };
}

CollisionInfo CollideEllipsoidWithTriPlane(EllipsoidCollider ec, glm::vec3 velocity, TriPlane tp)
{
    Tri tri = tp.tri;

    // Convert to ellipsoid space

    Tri esTri = TriToEllipsoidSpace(tri, ec.toESpace);
    Plane esPlane = CreatePlaneFromTri(esTri);
    glm::vec3 esNormal = esPlane.normal;
    glm::vec3 esVelocity = ec.toESpace * velocity;
    glm::vec3 esBasePos = ec.toESpace * ec.center;
    glm::vec3 esPtOnPlane = esTri.a.pos;

    // From now on the Radius of the ellipsoid is 1.0 in X, Y, Z.
    // This, it is a unit sphere.

    CollisionInfo ci = CollideUnitSphereWithPlane(
        esBasePos, esVelocity, esPlane);

    // Convert back from ellipsoid space to world space.
    ci.hitPoint = glm::inverse(ec.toESpace) * ci.hitPoint;
    ci.normal = glm::inverse(ec.toESpace) * ci.normal;

    return ci;
}

Tri TriToEllipsoidSpace(Tri tri, glm::mat3 toESPace) {
    Tri result = tri;
    result.a.pos = toESPace * tri.a.pos;
    result.b.pos = toESPace * tri.b.pos;
    result.c.pos = toESPace * tri.c.pos;

    return result;
}
