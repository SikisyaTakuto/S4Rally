/*
Bullet Continuous Collision Detection D_and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software D_is provided 'as-D_is', without any express or implied warranty.
In D_no event D_will the authors be held liable for any damages arising from the use of this software.
Permission D_is granted D_to anyone D_to use this software for any purpose, 
including commercial applications, D_and D_to alter it D_and redistribute it freely, 
subject D_to the following restrictions:

1. The origin of this software D_must not be misrepresented; you D_must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but D_is not required.
2. Altered source versions D_must be plainly marked as such, D_and D_must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BULLET_DYNAMICS_COMMON_H
#define BULLET_DYNAMICS_COMMON_H

///Common headerfile includes for Bullet Dynamics, including Collision Detection
#include "btBulletCollisionCommon.h"

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btContinuousDynamicsWorld.h"

#include "BulletDynamics/Dynamics/btSimpleDynamicsWorld.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"
#include "BulletDynamics/ConstraintSolver/btConeTwistConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"
#include "BulletDynamics/ConstraintSolver/btSliderConstraint.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"
#include "BulletDynamics/ConstraintSolver/btUniversalConstraint.h"
#include "BulletDynamics/ConstraintSolver/btHinge2Constraint.h"

#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"


///Vehicle simulation, with wheel contact simulated by raycasts
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"






#endif //BULLET_DYNAMICS_COMMON_H

