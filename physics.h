#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "btBulletDynamicsCommon.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>

void		 InitBullet();
btRigidBody* CreateRigidSphereBody(float radius, float mass);
void	     ApplyPhysicsStep(double dt);


#endif
