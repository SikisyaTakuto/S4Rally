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

#include "btSoftSoftCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "btSoftBody.h"

#define D_USE_PERSISTENT_CONTACTS 1

D_btSoftSoftCollisionAlgorithm::D_btSoftSoftCollisionAlgorithm(D_btPersistentManifold* /*mf*/,const D_btCollisionAlgorithmConstructionInfo& ci,D_btCollisionObject* /*obj0*/,D_btCollisionObject* /*obj1*/)
: D_btCollisionAlgorithm(ci)
//m_ownManifold(false),
//m_manifoldPtr(mf)
{
}

D_btSoftSoftCollisionAlgorithm::~D_btSoftSoftCollisionAlgorithm()
{
}

void D_btSoftSoftCollisionAlgorithm::processCollision (D_btCollisionObject* body0,D_btCollisionObject* body1,const D_btDispatcherInfo& /*dispatchInfo*/,D_btManifoldResult* /*resultOut*/)
{
	D_btSoftBody* soft0 =	(D_btSoftBody*)body0;
	D_btSoftBody* soft1 =	(D_btSoftBody*)body1;
	soft0->defaultCollisionHandler(soft1);
}

D_btScalar D_btSoftSoftCollisionAlgorithm::calculateTimeOfImpact(D_btCollisionObject* /*body0*/,D_btCollisionObject* /*body1*/,const D_btDispatcherInfo& /*dispatchInfo*/,D_btManifoldResult* /*resultOut*/)
{
	//not yet
	return 1.f;
}
