//
// Created by me on 9/2/24.
//

#include "r_common.h"
#include "collision.h"
#include <stdio.h>
#include <glm/gtx/norm.hpp>

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

// Assumes Triangle in CCW order.
glm::vec3 ConstructNormalToTriLineSegment(glm::vec3 a, glm::vec3 b, glm::vec3 planeNormal)
{
    glm::vec3 AB = glm::normalize(b - a);
    glm::vec3 nAB = glm::normalize(glm::cross(planeNormal, AB));

    return nAB;
}

float SignedDistancePointToPlane(glm::vec3 pt, glm::vec3 ptOnPlane, glm::vec3 planeNormal)
{
    glm::vec3 pVec = pt - ptOnPlane;
    float sD = glm::dot(pVec, planeNormal);

    return sD;
}

bool IsPointInTriangle(glm::vec3 point, Tri tri, glm::vec3 triNormal)
{
    glm::vec3 nAB = ConstructNormalToTriLineSegment(tri.a.pos, tri.b.pos, triNormal);
    float sdABplane = SignedDistancePointToPlane(point, tri.a.pos, nAB);
    // printf("sdABplane: %f\n", sdABplane);
    if (sdABplane <= 0.0f) {
        return false;
    }

    glm::vec3 nBC = ConstructNormalToTriLineSegment(tri.b.pos, tri.c.pos, triNormal);
    float sdBCplane = SignedDistancePointToPlane(point, tri.b.pos, nBC);
    if (sdBCplane <= 0.0f) {
        return false;
    }

    glm::vec3 nCA = ConstructNormalToTriLineSegment(tri.c.pos, tri.a.pos, triNormal);
    float sdCAplane = SignedDistancePointToPlane(point, tri.c.pos, nCA);
    if (sdCAplane <= 0.0f) {
        return false;
    }

    return true;
}

// If a > b then the two will be swapped.
// Otherwise, nothing happens.
void SortFloats(float* a, float* b)
{
    if (*a > *b) {
        float tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

void SortDoubles(double* a, double* b) {
    if (*a > *b) {
        double tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

#define SortValues(a, b, t) do { \
        t(&a, &b); \
    } while(0); \

bool GetSmallestRoot(float a, float b, float c, float maxRoot, float* root)
{
    float D = b*b - 4.0f*a*c;
    if (D < 0.0f) {
        return false;
    }

    float sqrtD = glm::sqrt(D);
    float denom = 1.0f / (2.0f * a);
    float r0 = (-b - sqrtD) * denom;
    float r1 = (-b + sqrtD) * denom;

    // Make sure we have the lowest solution in r0.
    SortValues(r0, r1, SortFloats);

    if (r0 > 0.0f && r0 < maxRoot) {
        *root = r0;
        return true;
    }

    // r0 could be a negative solution. In plain math this is ok,
    // but we are only interested in values that are between
    // 0 and maxRoot (which is 1 at maximum).
    if (r1 > 0.0f && r1 < maxRoot) {
        *root = r1;
        return true;
    }

    return false;
}

bool IsPointOnLineSegment(glm::vec3 p, glm::vec3 a, glm::vec3 b) {
    // Check if the intersection point between sphere and plane we found
    // earlier lies on one of the 3 triangle's edges.
    // Firstly, check if the intersection point is on the line.
    glm::vec3 edge = glm::normalize(b - a);
    glm::vec3 pVec = glm::normalize(p - a);
    float edgeDotPVec = glm::dot(edge, pVec);
    printf("edgeDotPVec = %f\n", edgeDotPVec);
    if ( edgeDotPVec >= HKD_EPSILON ) { // intersection point lies on line
        float lenE0 = glm::length(edge);
        float lenP = glm::length(pVec);
        if ( lenP < lenE0 ) {
            return true;
        }
    }

    return false;
}

bool CheckSweptSphereVsLinesegment(glm::vec3 p0, glm::vec3 p1, glm::vec3 sphereBase, glm::vec3 velocity, float maxT, float *root) {
	// Check sphere against tri's line-segments
	
	glm::vec3 e = p0 - p1;
	float eSquaredLength = glm::length2(e);
	float vSquaredLength = glm::length2(velocity);
	glm::vec3 baseToVertex = p0 - sphereBase;
	float eDotVel = glm::dot(e, velocity);
	float eDotBaseToVertex = glm::dot(e, baseToVertex);

	float a = eSquaredLength * (-vSquaredLength) + eDotVel*eDotVel;
	float b = eSquaredLength * 2.0f*glm::dot(velocity, baseToVertex) - 2.0f*( eDotVel * eDotBaseToVertex );
	float c = eSquaredLength * ( 1.0f - glm::length2(baseToVertex) ) + eDotBaseToVertex*eDotBaseToVertex;

	float newT;
	if (GetSmallestRoot(a, b, c, maxT, &newT)) {
		// Now check if hitpoint is withing the line segment.
		glm::vec3 collisionPt = sphereBase + newT*velocity;
		glm::vec3 ass = p0 - collisionPt;
		float squaredLengthA = glm::length2(ass);
		float ratio = squaredLengthA / eSquaredLength;
		
		if ( ratio < 1.0f && ratio > 0.0f ) {
			if (glm::dot(e, ass) >= 0.0f) {
				*root = newT;
				return true;
			}
		}
	}
	
	return false;
}

CollisionInfo CollideUnitSphereWithPlane(glm::vec3 pos, glm::vec3 velocity, Plane p, Tri tri)
{
    glm::vec3 normal = p.normal;
    glm::vec3 ptOnPlane = p.d * normal;
    glm::vec3 basePos = pos;

    // Signed distance from plane to unit sphere's center
    float sD = glm::dot(normal, basePos - ptOnPlane);
    float D = glm::abs(sD);

    // Project velocity along the plane normal
    float velDotNormal = glm::dot(normal, velocity);

    bool foundCollision = false;
    bool embeddedInPlane = false;
    float t0, t1;
    if ( glm::abs(velDotNormal) <= HKD_EPSILON ) { // Sphere travelling parallel to the plane
        // Distance from unit sphere center to plane is greater than 1 => no intersection!
        if ( D >= 1.0f ) {
            return {false, glm::vec3(0.0f), sD, glm::vec3(0.0f) };
        }
        // else: Sphere is already inside the plane.
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

        // printf("t0: %f, t1: %f\n", t0, t1);

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
            return {false};
        }

        t0 = glm::clamp(t0, 0.0f, 1.0f);
        t1 = glm::clamp(t1, 0.0f, 1.0f);
    }

    // Collision could be at the front side of the plane.
    // This is only possible when the intersection point is not embedded inside
    // the plane.
    float t = 1.0f;
    if (!embeddedInPlane) {
        // Check if the intersection is INSIDE the triangle.
        glm::vec3 intersectionPoint = basePos + t0*velocity - normal;
        if (IsPointInTriangle(intersectionPoint, tri, normal)) { // TODO: Rename function!
            foundCollision = true;
            // printf("Point inside tri side planes.\n");
            t = t0;
			printf("IsPointInTriangle: true\n");
        }
        else {
            // printf("Point outside tri side planes.\n");
        }
    }

    // Check if collision with one of the 3 vertices of the  triangle.
    // Can only happen if we did not collide previously with the 'inside'
    // of the triangle's side planes.
    // The Equation we have to solve is:
    // ( C(t) - p )^2 = 1, that is when the sphere collided with the tri's vertex.
    // where C(t) is the current pos of the sphere on its velocity vector:
    // C(t) = basePos + t * velocity
    // p is one of the vertices of the tri.
    if (!foundCollision) {
        float a = glm::length2(velocity);
        float newT;
        // Check point A
        float b = 2.0f * ( glm::dot(velocity, (basePos - tri.a.pos)) );
        float c = glm::distance2(tri.a.pos, basePos) - 1.0f;
        // Find smallest solution, if available
        if (GetSmallestRoot(a, b, c, t, &newT)) {
            t = newT;
            foundCollision = true;
        }

        // Check point B
        b = 2.0f * ( glm::dot(velocity, (basePos - tri.b.pos)) );
        c = glm::distance2(tri.b.pos, basePos) - 1.0f;
        // Find smallest solution, if available
        if (GetSmallestRoot(a, b, c, t, &newT)) {
            t = newT;
            foundCollision = true;
        }

        // Check point C
        b = 2.0f * ( glm::dot(velocity, (basePos - tri.c.pos)) );
        c = glm::distance2(tri.c.pos, basePos) - 1.0f;
        // Find smallest solution, if available
        if (GetSmallestRoot(a, b, c, t, &newT)) {
            t = newT;
            foundCollision = true;
        }

		// Check sphere against tri's line-segments

		if ( CheckSweptSphereVsLinesegment(tri.a.pos, tri.b.pos, basePos, velocity, t, &newT) ) {
			foundCollision = true;
		}

		if ( CheckSweptSphereVsLinesegment(tri.b.pos, tri.c.pos, basePos, velocity, t, &newT) ) {
			foundCollision = true;
		}

		if ( CheckSweptSphereVsLinesegment(tri.c.pos, tri.a.pos, basePos, velocity, t, &newT) ) {
			foundCollision = true;
		}

    }

    return { foundCollision, glm::vec3(0.0f), 0.0f, normal };
}

CollisionInfo CollideEllipsoidWithTriPlane(EllipsoidCollider ec, glm::vec3 velocity, TriPlane tp)
{
    Tri tri = tp.tri;

    // Convert to ellipsoid space

    Tri esTri = TriToEllipsoidSpace(tri, ec.toESpace);
    Plane esPlane = CreatePlaneFromTri(esTri);
    glm::vec3 esVelocity = ec.toESpace * velocity;
    glm::vec3 esBasePos = ec.toESpace * ec.center;

    // From now on the Radius of the ellipsoid is 1.0 in X, Y, Z.
    // This, it is a unit sphere.

    CollisionInfo ci = CollideUnitSphereWithPlane(
        esBasePos, esVelocity, esPlane, esTri
    );

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
