#include "physics.h"

#include "Body.h"
#include "Intersections.h"

void phys_AddBody(Body* body) {
    g_Bodies.push_back(body);
}

void phys_Update(float dt) {

    // Acceleration due to gravity
    glm::vec3 gravity = glm::vec3(0.0f, 0.0f, -10.0f);
    for (int i = 0; i < g_Bodies.size(); i++) {
        //g_Bodies[i]->m_LinearVelocity += glm::vec3(0.0f, 0.0f, -10.0f) * dt;
        Body* body = g_Bodies[i];
        float mass = 1.0f / body->m_InvMass;
        glm::vec3 gravityForce = mass * gravity;
        glm::vec3 impulse = gravityForce * dt;
        body->ApplyImpulseLinear(impulse);
    }

    // Check for collisions with other bodies
    for (int i = 0; i < g_Bodies.size(); i++) {
        for (int j = i + 1; j < g_Bodies.size(); j++) {
            Body* bodyA = g_Bodies[i];
            Body* bodyB = g_Bodies[j];

            // Bodies with infinite mass don't effect each other.
            if (0.0f == bodyA->m_InvMass && 0.0f == bodyB->m_InvMass) {
                continue;
            }

            if (Intersect(bodyA, bodyB)) {
                bodyA->m_LinearVelocity = glm::vec3(0.0f);
                bodyB->m_LinearVelocity = glm::vec3(0.0f);
            }
        }
    }

    // Positional update based on velocity
    for (int i = 0; i < g_Bodies.size(); i++) {
        Body* body = g_Bodies[i];
        body->m_Position += body->m_LinearVelocity * dt;
    }
}

