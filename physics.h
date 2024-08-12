#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include <vector>
#include "Body.h"

static std::vector<Body> g_Bodies;

void phys_AddBody(Body body);
void phys_Update(float dt);


#endif
