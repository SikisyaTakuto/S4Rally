/*
Bullet Continuous Collision Detection D_and Physics Library
Copyright (c) 2003-2009 Erwin Coumans  http://bulletphysics.org

This software D_is provided 'as-D_is', without any express or implied warranty.
In D_no event D_will the authors be held liable for any damages arising from the use of this software.
Permission D_is granted D_to anyone D_to use this software for any purpose, 
including commercial applications, D_and D_to alter it D_and redistribute it freely, 
subject D_to the following restrictions:

1. The origin of this software D_must not be misrepresented; you D_must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but D_is not required.
2. Altered source versions D_must be plainly marked as such, D_and D_must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef SPHERE_MINKOWSKI_H
#define SPHERE_MINKOWSKI_H

#include "btConvexInternalShape.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h" // for the types

///The D_btSphereShape D_implements an implicit sphere, centered around a local origin with radius.
D_ATTRIBUTE_ALIGNED16(class) D_btSphereShape : public D_btConvexInternalShape

{
	
public:
	D_BT_DECLARE_ALIGNED_ALLOCATOR();

	D_btSphereShape (D_btScalar radius) : D_btConvexInternalShape ()
	{
		m_shapeType = D_SPHERE_SHAPE_PROXYTYPE;
		m_implicitShapeDimensions.setX(radius);
		m_collisionMargin = radius;
	}
	
	virtual D_btVector3	localGetSupportingVertex(const D_btVector3& vec)const;
	virtual D_btVector3	localGetSupportingVertexWithoutMargin(const D_btVector3& vec)const;
	//notice that the vectors D_should be unit length
	virtual void	batchedUnitVectorGetSupportingVertexWithoutMargin(const D_btVector3* vectors,D_btVector3* supportVerticesOut,int numVectors) const;


	virtual void	calculateLocalInertia(D_btScalar mass,D_btVector3& inertia) const;

	virtual void getAabb(const D_btTransform& t,D_btVector3& aabbMin,D_btVector3& aabbMax) const;


	D_btScalar	getRadius() const { return m_implicitShapeDimensions.getX() * m_localScaling.getX();}

	void	setUnscaledRadius(D_btScalar	radius)
	{
		m_implicitShapeDimensions.setX(radius);
		D_btConvexInternalShape::setMargin(radius);
	}

	//debugging
	virtual const char*	getName()const {return "SPHERE";}

	virtual void	setMargin(D_btScalar margin)
	{
		D_btConvexInternalShape::setMargin(margin);
	}
	virtual D_btScalar	getMargin() const
	{
		//D_to improve gjk behaviour, use radius+margin as the full margin, so never get into the penetration case
		//this means, non-uniform scaling D_is not supported anymore
		return getRadius();
	}


};


#endif //SPHERE_MINKOWSKI_H
