#include "Body.h"
#include "physics.h"

void phys_AddBody(Body* body) {
    g_Bodies.push_back(body);
}

void phys_Update(float dt) {

    // Acceleration due to gravity
    for (int i = 0; i < g_Bodies.size(); i++) {
        g_Bodies[i]->m_LinearVelocity += glm::vec3(0.0f, 0.0f, -10.0f) * dt;
    }

    // Positional update based on velocity
    for (int i = 0; i < g_Bodies.size(); i++) {
        Body* body = g_Bodies[i];
        body->m_Position += body->m_LinearVelocity * dt;
    }
}

