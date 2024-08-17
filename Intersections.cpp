//
// Created by me on 8/17/24.
//

#include "Intersections.h"
#include "Body.h"
#include "ShapeSphere.h"

bool Intersect(Body* bodyA, Body* bodyB) {
    float distanceAB = glm::length(bodyB->m_Position - bodyA->m_Position);

    ShapeSphere* shapeA = (ShapeSphere*)bodyA->m_Shape;
    ShapeSphere* shapeB = (ShapeSphere*)bodyB->m_Shape;

    float radiusAB = shapeA->m_Radius + shapeB->m_Radius;
    float distanceSquared = distanceAB * distanceAB;

    if ( distanceSquared < (radiusAB * radiusAB) ) {
        return true;
    }

    return false;
}



