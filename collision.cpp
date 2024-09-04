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
    float D = glm::abs(sD);

    // Project velocity along the plane normal
    float velDotNormal = glm::dot(normal, velocity);

    bool embeddedInPlane = false;
    float t0, t1;
    if (velDotNormal <= HKD_EPSILON) { // Sphere travelling parallel to the plane
        // Distance from unit sphere center to plane is greater than 1 => no intersection!
        if ( D >= 1.0f ) {
            return {false, glm::vec3(0.0f), sD, glm::vec3(0.0f) };
        }
        // else: Sphere is already inside the plane.
        glm::vec3 hitPoint = basePos - sD * normal;
        t0 = 0.0f;
        t1 = 1.0f;
        embeddedInPlane = true;
    }
    else { // N Dot D not 0! There could be an intersection!

        // Calculate t0, that is the time when the unit sphere hits the
        // front face of the plane.
        t0 = (1.0f - sD) / velDotNormal;

        // As above, calculate t1, that is the time when the unit sphere
        // hits the back face of the plane.
        t1 = (-1.0f - sD) / velDotNormal;

        printf("t0: %f, t1: %f\n", t0, t1);

        // t0, t1 marks the intersection interval. Make sure
        // t0 < t1 because depending on what side of the Plane
        // the sphere is, t0 might not be the smallest value.
        // But we need the smallest because it is the maximum we can
        // travel the sphere along the velocity vector before a collision happens.
        if (t0 > t1) {
            float tmp = t0;
            t0 = t1;
            t1 = tmp;
        }

        if (t0 > 1.0f || t1 < 0.0f) { // No collision
            return { false };
        }

        glm::vec3 hitPoint = basePos;
        float t = 0.0f;
        if (t0 <= 1.0f) {
            hitPoint += t0 * velocity - normal;
            t = t0;
        }
        else if (t1 >= -1.0f) {
            hitPoint += t1 * velocity - normal;
            t = t1;
        }
    }

    return { true, glm::vec3(0.0f), 0.0f, normal };
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

    if (ci.didCollide) {
        glm::vec3 newPos = ci.hitPoint;

    }

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
