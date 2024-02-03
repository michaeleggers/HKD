#include "physics.h"

#include "btBulletDynamicsCommon.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

static btDefaultCollisionConfiguration* g_CollisionConfig;
static btCollisionDispatcher* g_Dispatcher;
static btBroadphaseInterface* g_Broadphase;
static btSequentialImpulseConstraintSolver* g_Solver;
static btDiscreteDynamicsWorld* g_DynamicsWorld;

void InitBullet() {
    g_Broadphase = new btDbvtBroadphase();
    g_CollisionConfig = new btDefaultCollisionConfiguration();
    g_Dispatcher = new btCollisionDispatcher(g_CollisionConfig);
    g_Solver = new btSequentialImpulseConstraintSolver();
    g_DynamicsWorld = new btDiscreteDynamicsWorld(g_Dispatcher, g_Broadphase, g_Solver, g_CollisionConfig);

    g_DynamicsWorld->setGravity(btVector3(0, 0, -9.8f));
}

btRigidBody* CreateRigidSphereBody(float radius, float mass) {
    btCollisionShape* sphereShape = new btSphereShape(radius);
    btDefaultMotionState* sphereMotionState = new btDefaultMotionState(
        btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))
    );

    btVector3 sphereInertia(0, 0, 0);
    sphereShape->calculateLocalInertia(mass, sphereInertia);

    btRigidBody::btRigidBodyConstructionInfo ci(mass, sphereMotionState, sphereShape, sphereInertia);

    btRigidBody* sphereRigidBody = new btRigidBody(ci);
    sphereRigidBody->setRestitution(1.0);
    sphereRigidBody->setFriction(1.0);

    g_DynamicsWorld->addRigidBody(sphereRigidBody);

    return sphereRigidBody;
}

void ApplyPhysicsStep(double dt)
{
    // TODO: What exactly is the value passed to setpSimulation?
    g_DynamicsWorld->stepSimulation(dt);
}