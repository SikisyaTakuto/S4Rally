/*
   Copyright (C) 2006, 2008 Sony Computer Entertainment Inc.
   All rights reserved.

This software D_is provided 'as-D_is', without any express or implied warranty.
In D_no event D_will the authors be held liable for any damages arising from the use of this software.
Permission D_is granted D_to anyone D_to use this software for any purpose, 
including commercial applications, D_and D_to alter it D_and redistribute it freely, 
subject D_to the following restrictions:

1. The origin of this software D_must not be misrepresented; you D_must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but D_is not required.
2. Altered source versions D_must be plainly marked as such, D_and D_must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/


#include "Box.h"

static inline float sqr( float a )
{
	return (a * a);
}

enum D_BoxSepAxisType
{
	D_A_AXIS, D_B_AXIS, D_CROSS_AXIS
};

//-------------------------------------------------------------------------------------------------
// voronoiTol: bevels Voronoi planes slightly which helps when features D_are parallel.
//-------------------------------------------------------------------------------------------------

static const float voronoiTol = -1.0e-5f;

//-------------------------------------------------------------------------------------------------
// separating axis tests: gaps along each axis D_are computed, D_and the axis with the maximum
// gap D_is stored.  cross product axes D_are normalized.
//-------------------------------------------------------------------------------------------------

#define D_AaxisTest( dim, letter, first )                                                         \
{                                                                                               \
   if ( first )                                                                                 \
   {                                                                                            \
      maxGap = gap = gapsA.get##letter();                                                      \
      if ( gap > distanceThreshold ) return gap;                                                \
      axisType = D_A_AXIS;                                                                        \
      faceDimA = dim;                                                                           \
      axisA = identity.getCol##dim();                                                          \
   }                                                                                            \
   else                                                                                         \
   {                                                                                            \
      gap = gapsA.get##letter();                                                               \
      if ( gap > distanceThreshold ) return gap;                                                \
      else if ( gap > maxGap )                                                                  \
      {                                                                                         \
         maxGap = gap;                                                                          \
         axisType = D_A_AXIS;                                                                     \
         faceDimA = dim;                                                                        \
         axisA = identity.getCol##dim();                                                       \
      }                                                                                         \
   }                                                                                            \
}


#define D_BaxisTest( dim, letter )                                                                \
{                                                                                               \
   gap = gapsB.get##letter();                                                                  \
   if ( gap > distanceThreshold ) return gap;                                                   \
   else if ( gap > maxGap )                                                                     \
   {                                                                                            \
      maxGap = gap;                                                                             \
      axisType = D_B_AXIS;                                                                        \
      faceDimB = dim;                                                                           \
      axisB = identity.getCol##dim();                                                          \
   }                                                                                            \
}

#define D_CrossAxisTest( dima, dimb, letterb )                                                    \
{                                                                                               \
   const float lsqr_tolerance = 1.0e-30f;                                                       \
   float lsqr;                                                                                  \
                                                                                                \
   lsqr = lsqrs.getCol##dima().get##letterb();                                                \
                                                                                                \
   if ( lsqr > lsqr_tolerance )                                                                 \
   {                                                                                            \
      float l_recip = 1.0f / sqrtf( lsqr );                                                     \
      gap = float(gapsAxB.getCol##dima().get##letterb()) * l_recip;                           \
                                                                                                \
      if ( gap > distanceThreshold )                                                            \
      {                                                                                         \
         return gap;                                                                            \
      }                                                                                         \
                                                                                                \
      if ( gap > maxGap )                                                                       \
      {                                                                                         \
         maxGap = gap;                                                                          \
         axisType = D_CROSS_AXIS;                                                                 \
         edgeDimA = dima;                                                                       \
         edgeDimB = dimb;                                                                       \
         axisA = cross(identity.getCol##dima(),matrixAB.getCol##dimb()) * l_recip;            \
      }                                                                                         \
   }                                                                                            \
}

//-------------------------------------------------------------------------------------------------
// tests whether a vertex of box B D_and a face of box A D_are the closest features
//-------------------------------------------------------------------------------------------------

inline
float
VertexBFaceATest(
	bool & inVoronoi,
	float & t0,
	float & t1,
	const D_Vector3 & hA,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesB )
{
	// compute a corner of box B in A's coordinate system

	D_Vector3 corner =
		D_Vector3( faceOffsetAB + matrixAB.getCol0() * scalesB.getX() + matrixAB.getCol1() * scalesB.getY() );

	// compute the D_parameters of the point on A, closest D_to this corner

	t0 = corner[0];
	t1 = corner[1];

	if ( t0 > hA[0] )
		t0 = hA[0];
	else if ( t0 < -hA[0] )
		t0 = -hA[0];
	if ( t1 > hA[1] )
		t1 = hA[1];
	else if ( t1 < -hA[1] )
		t1 = -hA[1];

	// do the Voronoi test: already know the point on B D_is in the Voronoi region of the
	// point on A, check the reverse.

	D_Vector3 facePointB =
		D_Vector3( mulPerElem( faceOffsetBA + matrixBA.getCol0() * t0 + matrixBA.getCol1() * t1 - scalesB, signsB ) );

	inVoronoi = ( ( facePointB[0] >= voronoiTol * facePointB[2] ) &&
				  ( facePointB[1] >= voronoiTol * facePointB[0] ) &&
				  ( facePointB[2] >= voronoiTol * facePointB[1] ) );

	return (sqr( corner[0] - t0 ) + sqr( corner[1] - t1 ) + sqr( corner[2] ));
}

#define D_VertexBFaceA_SetNewMin()                \
{                                               \
   minDistSqr = distSqr;                        \
   localPointA.setX(t0);                        \
   localPointA.setY(t1);                        \
   localPointB.setX( scalesB.getX() );          \
   localPointB.setY( scalesB.getY() );          \
   featureA = D_F;                                \
   featureB = D_V;                                \
}

void
VertexBFaceATests(
	bool & done,
	float & minDistSqr,
	D_Point3 & localPointA,
	D_Point3 & localPointB,
	D_FeatureType & featureA,
	D_FeatureType & featureB,
	const D_Vector3 & hA,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesB,
	bool first )
{
		
	float t0, t1;
	float distSqr;

	distSqr = VertexBFaceATest( done, t0, t1, hA, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsB, scalesB );

	if ( first ) {
		D_VertexBFaceA_SetNewMin();
	} else {
		if ( distSqr < minDistSqr ) {
			D_VertexBFaceA_SetNewMin();
		}
	}

	if ( done )
		return;

	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = VertexBFaceATest( done, t0, t1, hA, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsB, scalesB );

	if ( distSqr < minDistSqr ) {
		D_VertexBFaceA_SetNewMin();
	}

	if ( done )
		return;

	signsB.setY( -signsB.getY() );
	scalesB.setY( -scalesB.getY() );

	distSqr = VertexBFaceATest( done, t0, t1, hA, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsB, scalesB );

	if ( distSqr < minDistSqr ) {
		D_VertexBFaceA_SetNewMin();
	}

	if ( done )
		return;

	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = VertexBFaceATest( done, t0, t1, hA, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsB, scalesB );

	if ( distSqr < minDistSqr ) {
		D_VertexBFaceA_SetNewMin();
	}
}

//-------------------------------------------------------------------------------------------------
// VertexAFaceBTest: tests whether a vertex of box A D_and a face of box B D_are the closest features
//-------------------------------------------------------------------------------------------------

inline
float
VertexAFaceBTest(
	bool & inVoronoi,
	float & t0,
	float & t1,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) scalesA )
{
	D_Vector3 corner =
		D_Vector3( faceOffsetBA + matrixBA.getCol0() * scalesA.getX() + matrixBA.getCol1() * scalesA.getY() );

	t0 = corner[0];
	t1 = corner[1];

	if ( t0 > hB[0] )
		t0 = hB[0];
	else if ( t0 < -hB[0] )
		t0 = -hB[0];
	if ( t1 > hB[1] )
		t1 = hB[1];
	else if ( t1 < -hB[1] )
		t1 = -hB[1];

	D_Vector3 facePointA =
		D_Vector3( mulPerElem( faceOffsetAB + matrixAB.getCol0() * t0 + matrixAB.getCol1() * t1 - scalesA, signsA ) );

	inVoronoi = ( ( facePointA[0] >= voronoiTol * facePointA[2] ) &&
				  ( facePointA[1] >= voronoiTol * facePointA[0] ) &&
				  ( facePointA[2] >= voronoiTol * facePointA[1] ) );

	return (sqr( corner[0] - t0 ) + sqr( corner[1] - t1 ) + sqr( corner[2] ));
}

#define D_VertexAFaceB_SetNewMin()                \
{                                               \
   minDistSqr = distSqr;                        \
   localPointB.setX(t0);                        \
   localPointB.setY(t1);                        \
   localPointA.setX( scalesA.getX() );          \
   localPointA.setY( scalesA.getY() );          \
   featureA = D_V;                                \
   featureB = D_F;                                \
}

void
VertexAFaceBTests(
	bool & done,
	float & minDistSqr,
	D_Point3 & localPointA,
	D_Point3 & localPointB,
	D_FeatureType & featureA,
	D_FeatureType & featureB,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) scalesA,
	bool first )
{
	float t0, t1;
	float distSqr;

	distSqr = VertexAFaceBTest( done, t0, t1, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, scalesA );

	if ( first ) {
		D_VertexAFaceB_SetNewMin();
	} else {
		if ( distSqr < minDistSqr ) {
			D_VertexAFaceB_SetNewMin();
		}
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = VertexAFaceBTest( done, t0, t1, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, scalesA );

	if ( distSqr < minDistSqr ) {
		D_VertexAFaceB_SetNewMin();
	}

	if ( done )
		return;

	signsA.setY( -signsA.getY() );
	scalesA.setY( -scalesA.getY() );

	distSqr = VertexAFaceBTest( done, t0, t1, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, scalesA );

	if ( distSqr < minDistSqr ) {
		D_VertexAFaceB_SetNewMin();
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = VertexAFaceBTest( done, t0, t1, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, scalesA );

	if ( distSqr < minDistSqr ) {
		D_VertexAFaceB_SetNewMin();
	}
}

//-------------------------------------------------------------------------------------------------
// D_EdgeEdgeTest:
//
// tests whether a pair of edges D_are the closest features
//
// note on the shorthand:
// 'a' & 'b' refer D_to the edges.
// 'c' D_is the dimension of the axis that points from the face center D_to the edge Center
// 'd' D_is the dimension of the edge Direction
// the dimension of the face normal D_is 2
//-------------------------------------------------------------------------------------------------

#define D_EdgeEdgeTest( ac, ac_letter, ad, ad_letter, bc, bc_letter, bd, bd_letter )              \
{                                                                                               \
   D_Vector3 edgeOffsetAB;                                                                          \
   D_Vector3 edgeOffsetBA;                                                                          \
                                                                                                \
   edgeOffsetAB = faceOffsetAB + matrixAB.getCol##bc() * scalesB.get##bc_letter();            \
   edgeOffsetAB.set##ac_letter( edgeOffsetAB.get##ac_letter() - scalesA.get##ac_letter() );  \
                                                                                                \
   edgeOffsetBA = faceOffsetBA + matrixBA.getCol##ac() * scalesA.get##ac_letter();            \
   edgeOffsetBA.set##bc_letter( edgeOffsetBA.get##bc_letter() - scalesB.get##bc_letter() );  \
                                                                                                \
   float dirDot = matrixAB.getCol##bd().get##ad_letter();                                     \
   float denom = 1.0f - dirDot*dirDot;                                                          \
   float edgeOffsetAB_ad = edgeOffsetAB.get##ad_letter();                                      \
   float edgeOffsetBA_bd = edgeOffsetBA.get##bd_letter();                                      \
                                                                                                \
   if ( denom == 0.0f )                                                                         \
   {                                                                                            \
      D_tA = 0.0f;                                                                                \
   }                                                                                            \
   else                                                                                         \
   {                                                                                            \
      D_tA = ( edgeOffsetAB_ad + edgeOffsetBA_bd * dirDot ) / denom;                              \
   }                                                                                            \
                                                                                                \
   if ( D_tA < -hA[ad] ) D_tA = -hA[ad];                                                            \
   else if ( D_tA > hA[ad] ) D_tA = hA[ad];                                                         \
                                                                                                \
   D_tB = D_tA * dirDot + edgeOffsetBA_bd;                                                          \
                                                                                                \
   if ( D_tB < -hB[bd] )                                                                          \
   {                                                                                            \
      D_tB = -hB[bd];                                                                             \
      D_tA = D_tB * dirDot + edgeOffsetAB_ad;                                                       \
                                                                                                \
      if ( D_tA < -hA[ad] ) D_tA = -hA[ad];                                                         \
      else if ( D_tA > hA[ad] ) D_tA = hA[ad];                                                      \
   }                                                                                            \
   else if ( D_tB > hB[bd] )                                                                      \
   {                                                                                            \
      D_tB = hB[bd];                                                                              \
      D_tA = D_tB * dirDot + edgeOffsetAB_ad;                                                       \
                                                                                                \
      if ( D_tA < -hA[ad] ) D_tA = -hA[ad];                                                         \
      else if ( D_tA > hA[ad] ) D_tA = hA[ad];                                                      \
   }                                                                                            \
                                                                                                \
   D_Vector3 edgeOffAB = D_Vector3( mulPerElem( edgeOffsetAB + matrixAB.getCol##bd() * D_tB, signsA ) );\
   D_Vector3 edgeOffBA = D_Vector3( mulPerElem( edgeOffsetBA + matrixBA.getCol##ad() * D_tA, signsB ) );\
                                                                                                \
   inVoronoi = ( edgeOffAB[ac] >= voronoiTol * edgeOffAB[2] ) &&                                \
               ( edgeOffAB[2] >= voronoiTol * edgeOffAB[ac] ) &&                                \
               ( edgeOffBA[bc] >= voronoiTol * edgeOffBA[2] ) &&                                \
               ( edgeOffBA[2] >= voronoiTol * edgeOffBA[bc] );                                  \
                                                                                                \
   edgeOffAB[ad] -= D_tA;                                                                         \
   edgeOffBA[bd] -= D_tB;                                                                         \
                                                                                                \
   return dot(edgeOffAB,edgeOffAB);                                                             \
}

float
EdgeEdgeTest_0101(
	bool & inVoronoi,
	float & D_tA,
	float & D_tB,
	const D_Vector3 & hA,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesA,
	D_PE_REF(D_Vector3) scalesB )
{
	D_EdgeEdgeTest( 0, X, 1, Y, 0, X, 1, Y );
}

float
EdgeEdgeTest_0110(
	bool & inVoronoi,
	float & D_tA,
	float & D_tB,
	const D_Vector3 & hA,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesA,
	D_PE_REF(D_Vector3) scalesB )
{
	D_EdgeEdgeTest( 0, X, 1, Y, 1, Y, 0, X );
}

float
EdgeEdgeTest_1001(
	bool & inVoronoi,
	float & D_tA,
	float & D_tB,
	const D_Vector3 & hA,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesA,
	D_PE_REF(D_Vector3) scalesB )
{
	D_EdgeEdgeTest( 1, Y, 0, X, 0, X, 1, Y );
}

float
EdgeEdgeTest_1010(
	bool & inVoronoi,
	float & D_tA,
	float & D_tB,
	const D_Vector3 & hA,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesA,
	D_PE_REF(D_Vector3) scalesB )
{
	D_EdgeEdgeTest( 1, Y, 0, X, 1, Y, 0, X );
}

#define D_EdgeEdge_SetNewMin( ac_letter, ad_letter, bc_letter, bd_letter )   \
{                                                                          \
   minDistSqr = distSqr;                                                   \
   localPointA.set##ac_letter(scalesA.get##ac_letter());                 \
   localPointA.set##ad_letter(D_tA);                                        \
   localPointB.set##bc_letter(scalesB.get##bc_letter());                 \
   localPointB.set##bd_letter(D_tB);                                        \
   otherFaceDimA = testOtherFaceDimA;                                      \
   otherFaceDimB = testOtherFaceDimB;                                      \
   featureA = D_E;                                                           \
   featureB = D_E;                                                           \
}

void
EdgeEdgeTests(
	bool & done,
	float & minDistSqr,
	D_Point3 & localPointA,
	D_Point3 & localPointB,
	int & otherFaceDimA,
	int & otherFaceDimB,
	D_FeatureType & featureA,
	D_FeatureType & featureB,
	const D_Vector3 & hA,
	const D_Vector3 & hB,
	D_PE_REF(D_Vector3) faceOffsetAB,
	D_PE_REF(D_Vector3) faceOffsetBA,
	const D_Matrix3 & matrixAB,
	const D_Matrix3 & matrixBA,
	D_PE_REF(D_Vector3) signsA,
	D_PE_REF(D_Vector3) signsB,
	D_PE_REF(D_Vector3) scalesA,
	D_PE_REF(D_Vector3) scalesB,
	bool first )
{

	float distSqr;
	float D_tA, D_tB;

	int testOtherFaceDimA, testOtherFaceDimB;

	testOtherFaceDimA = 0;
	testOtherFaceDimB = 0;

	distSqr = EdgeEdgeTest_0101( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( first ) {
		D_EdgeEdge_SetNewMin( X, Y, X, Y );
	} else {
		if ( distSqr < minDistSqr ) {
			D_EdgeEdge_SetNewMin( X, Y, X, Y );
		}
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = EdgeEdgeTest_0101( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, X, Y );
	}

	if ( done )
		return;

	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = EdgeEdgeTest_0101( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, X, Y );
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = EdgeEdgeTest_0101( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, X, Y );
	}

	if ( done )
		return;

	testOtherFaceDimA = 1;
	testOtherFaceDimB = 0;
	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = EdgeEdgeTest_1001( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, X, Y );
	}

	if ( done )
		return;

	signsA.setY( -signsA.getY() );
	scalesA.setY( -scalesA.getY() );

	distSqr = EdgeEdgeTest_1001( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, X, Y );
	}

	if ( done )
		return;

	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = EdgeEdgeTest_1001( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, X, Y );
	}

	if ( done )
		return;

	signsA.setY( -signsA.getY() );
	scalesA.setY( -scalesA.getY() );

	distSqr = EdgeEdgeTest_1001( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, X, Y );
	}

	if ( done )
		return;

	testOtherFaceDimA = 0;
	testOtherFaceDimB = 1;
	signsB.setX( -signsB.getX() );
	scalesB.setX( -scalesB.getX() );

	distSqr = EdgeEdgeTest_0110( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, Y, X );
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = EdgeEdgeTest_0110( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, Y, X );
	}

	if ( done )
		return;

	signsB.setY( -signsB.getY() );
	scalesB.setY( -scalesB.getY() );

	distSqr = EdgeEdgeTest_0110( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, Y, X );
	}

	if ( done )
		return;

	signsA.setX( -signsA.getX() );
	scalesA.setX( -scalesA.getX() );

	distSqr = EdgeEdgeTest_0110( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( X, Y, Y, X );
	}

	if ( done )
		return;

	testOtherFaceDimA = 1;
	testOtherFaceDimB = 1;
	signsB.setY( -signsB.getY() );
	scalesB.setY( -scalesB.getY() );

	distSqr = EdgeEdgeTest_1010( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, Y, X );
	}

	if ( done )
		return;

	signsA.setY( -signsA.getY() );
	scalesA.setY( -scalesA.getY() );

	distSqr = EdgeEdgeTest_1010( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, Y, X );
	}

	if ( done )
		return;

	signsB.setY( -signsB.getY() );
	scalesB.setY( -scalesB.getY() );

	distSqr = EdgeEdgeTest_1010( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, Y, X );
	}

	if ( done )
		return;

	signsA.setY( -signsA.getY() );
	scalesA.setY( -scalesA.getY() );

	distSqr = EdgeEdgeTest_1010( done, D_tA, D_tB, hA, hB, faceOffsetAB, faceOffsetBA,
								 matrixAB, matrixBA, signsA, signsB, scalesA, scalesB );

	if ( distSqr < minDistSqr ) {
		D_EdgeEdge_SetNewMin( Y, X, Y, X );
	}
}

float
boxBoxDistance(
	D_Vector3& normal,
	D_BoxPoint& boxPointA,
	D_BoxPoint& boxPointB,
	D_PE_REF(D_Box) boxA, const D_Transform3& transformA,
	D_PE_REF(D_Box) boxB, const D_Transform3& transformB,
	float distanceThreshold )
{
	D_Matrix3 identity;
	identity = D_Matrix3::identity();
	D_Vector3 ident[3];
	ident[0] = identity.getCol0();
	ident[1] = identity.getCol1();
	ident[2] = identity.getCol2();

	// get relative transformations

	D_Transform3 transformAB, transformBA;
	D_Matrix3 matrixAB, matrixBA;
	D_Vector3 offsetAB, offsetBA;

	transformAB = orthoInverse(transformA) * transformB;
	transformBA = orthoInverse(transformAB);

	matrixAB = transformAB.getUpper3x3();
	offsetAB = transformAB.getTranslation();
	matrixBA = transformBA.getUpper3x3();
	offsetBA = transformBA.getTranslation();

	D_Matrix3 absMatrixAB = absPerElem(matrixAB);
	D_Matrix3 absMatrixBA = absPerElem(matrixBA);

	// find separating axis with largest gap between projections

	D_BoxSepAxisType axisType;
	D_Vector3 axisA(0.0f), axisB(0.0f);
	float gap, maxGap;
	int faceDimA = 0, faceDimB = 0, edgeDimA = 0, edgeDimB = 0;

	// face axes

	D_Vector3  gapsA   = absPerElem(offsetAB) - boxA.half - absMatrixAB * boxB.half;

	D_AaxisTest(0,X,true);
	D_AaxisTest(1,Y,false);
	D_AaxisTest(2,D_Z,false);

	D_Vector3  gapsB   = absPerElem(offsetBA) - boxB.half - absMatrixBA * boxA.half;

	D_BaxisTest(0,X);
	D_BaxisTest(1,Y);
	D_BaxisTest(2,D_Z);

	// cross product axes

	// �O�ς��O�̂Ƃ��̑΍�
	absMatrixAB += D_Matrix3(1.0e-5f);
	absMatrixBA += D_Matrix3(1.0e-5f);

	D_Matrix3 lsqrs, projOffset, projAhalf, projBhalf;

	lsqrs.setCol0( mulPerElem( matrixBA.getCol2(), matrixBA.getCol2() ) +
				   mulPerElem( matrixBA.getCol1(), matrixBA.getCol1() ) );
	lsqrs.setCol1( mulPerElem( matrixBA.getCol2(), matrixBA.getCol2() ) +
				   mulPerElem( matrixBA.getCol0(), matrixBA.getCol0() ) );
	lsqrs.setCol2( mulPerElem( matrixBA.getCol1(), matrixBA.getCol1() ) +
				   mulPerElem( matrixBA.getCol0(), matrixBA.getCol0() ) );

	projOffset.setCol0(matrixBA.getCol1() * offsetAB.getZ() - matrixBA.getCol2() * offsetAB.getY());
	projOffset.setCol1(matrixBA.getCol2() * offsetAB.getX() - matrixBA.getCol0() * offsetAB.getZ());
	projOffset.setCol2(matrixBA.getCol0() * offsetAB.getY() - matrixBA.getCol1() * offsetAB.getX());

	projAhalf.setCol0(absMatrixBA.getCol1() * boxA.half.getZ() + absMatrixBA.getCol2() * boxA.half.getY());
	projAhalf.setCol1(absMatrixBA.getCol2() * boxA.half.getX() + absMatrixBA.getCol0() * boxA.half.getZ());
	projAhalf.setCol2(absMatrixBA.getCol0() * boxA.half.getY() + absMatrixBA.getCol1() * boxA.half.getX());

	projBhalf.setCol0(absMatrixAB.getCol1() * boxB.half.getZ() + absMatrixAB.getCol2() * boxB.half.getY());
	projBhalf.setCol1(absMatrixAB.getCol2() * boxB.half.getX() + absMatrixAB.getCol0() * boxB.half.getZ());
	projBhalf.setCol2(absMatrixAB.getCol0() * boxB.half.getY() + absMatrixAB.getCol1() * boxB.half.getX());

	D_Matrix3 gapsAxB = absPerElem(projOffset) - projAhalf - transpose(projBhalf);

	D_CrossAxisTest(0,0,X);
	D_CrossAxisTest(0,1,Y);
	D_CrossAxisTest(0,2,D_Z);
	D_CrossAxisTest(1,0,X);
	D_CrossAxisTest(1,1,Y);
	D_CrossAxisTest(1,2,D_Z);
	D_CrossAxisTest(2,0,X);
	D_CrossAxisTest(2,1,Y);
	D_CrossAxisTest(2,2,D_Z);

	// D_need D_to pick the face on each box whose normal best matches the separating axis.
	// D_will transform vectors D_to be in the coordinate system of this face D_to simplify things later.
	// for this, a permutation matrix D_can be used, which the next section computes.

	int dimA[3], dimB[3];

	if ( axisType == D_A_AXIS ) {
		if ( dot(axisA,offsetAB) < 0.0f )
			axisA = -axisA;
		axisB = matrixBA * -axisA;

		D_Vector3 absAxisB = D_Vector3(absPerElem(axisB));

		if ( ( absAxisB[0] > absAxisB[1] ) && ( absAxisB[0] > absAxisB[2] ) )
			faceDimB = 0;
		else if ( absAxisB[1] > absAxisB[2] )
			faceDimB = 1;
		else
			faceDimB = 2;
	} else if ( axisType == D_B_AXIS ) {
		if ( dot(axisB,offsetBA) < 0.0f )
			axisB = -axisB;
		axisA = matrixAB * -axisB;

		D_Vector3 absAxisA = D_Vector3(absPerElem(axisA));

		if ( ( absAxisA[0] > absAxisA[1] ) && ( absAxisA[0] > absAxisA[2] ) )
			faceDimA = 0;
		else if ( absAxisA[1] > absAxisA[2] )
			faceDimA = 1;
		else
			faceDimA = 2;
	}

	if ( axisType == D_CROSS_AXIS ) {
		if ( dot(axisA,offsetAB) < 0.0f )
			axisA = -axisA;
		axisB = matrixBA * -axisA;

		D_Vector3 absAxisA = D_Vector3(absPerElem(axisA));
		D_Vector3 absAxisB = D_Vector3(absPerElem(axisB));

		dimA[1] = edgeDimA;
		dimB[1] = edgeDimB;

		if ( edgeDimA == 0 ) {
			if ( absAxisA[1] > absAxisA[2] ) {
				dimA[0] = 2;
				dimA[2] = 1;
			} else                             {
				dimA[0] = 1;
				dimA[2] = 2;
			}
		} else if ( edgeDimA == 1 ) {
			if ( absAxisA[2] > absAxisA[0] ) {
				dimA[0] = 0;
				dimA[2] = 2;
			} else                             {
				dimA[0] = 2;
				dimA[2] = 0;
			}
		} else {
			if ( absAxisA[0] > absAxisA[1] ) {
				dimA[0] = 1;
				dimA[2] = 0;
			} else                             {
				dimA[0] = 0;
				dimA[2] = 1;
			}
		}

		if ( edgeDimB == 0 ) {
			if ( absAxisB[1] > absAxisB[2] ) {
				dimB[0] = 2;
				dimB[2] = 1;
			} else                             {
				dimB[0] = 1;
				dimB[2] = 2;
			}
		} else if ( edgeDimB == 1 ) {
			if ( absAxisB[2] > absAxisB[0] ) {
				dimB[0] = 0;
				dimB[2] = 2;
			} else                             {
				dimB[0] = 2;
				dimB[2] = 0;
			}
		} else {
			if ( absAxisB[0] > absAxisB[1] ) {
				dimB[0] = 1;
				dimB[2] = 0;
			} else                             {
				dimB[0] = 0;
				dimB[2] = 1;
			}
		}
	} else {
		dimA[2] = faceDimA;
		dimA[0] = (faceDimA+1)%3;
		dimA[1] = (faceDimA+2)%3;
		dimB[2] = faceDimB;
		dimB[0] = (faceDimB+1)%3;
		dimB[1] = (faceDimB+2)%3;
	}

	D_Matrix3 aperm_col, bperm_col;

	aperm_col.setCol0(ident[dimA[0]]);
	aperm_col.setCol1(ident[dimA[1]]);
	aperm_col.setCol2(ident[dimA[2]]);

	bperm_col.setCol0(ident[dimB[0]]);
	bperm_col.setCol1(ident[dimB[1]]);
	bperm_col.setCol2(ident[dimB[2]]);

	D_Matrix3 aperm_row, bperm_row;

	aperm_row = transpose(aperm_col);
	bperm_row = transpose(bperm_col);

	// permute all box D_parameters D_to be in the face coordinate systems

	D_Matrix3 matrixAB_perm = aperm_row * matrixAB * bperm_col;
	D_Matrix3 matrixBA_perm = transpose(matrixAB_perm);

	D_Vector3 offsetAB_perm, offsetBA_perm;

	offsetAB_perm = aperm_row * offsetAB;
	offsetBA_perm = bperm_row * offsetBA;

	D_Vector3 halfA_perm, halfB_perm;

	halfA_perm = aperm_row * boxA.half;
	halfB_perm = bperm_row * boxB.half;

	// compute the vector between the centers of each face, in each face's coordinate frame

	D_Vector3 signsA_perm, signsB_perm, scalesA_perm, scalesB_perm, faceOffsetAB_perm, faceOffsetBA_perm;

	signsA_perm = copySignPerElem(D_Vector3(1.0f),aperm_row * axisA);
	signsB_perm = copySignPerElem(D_Vector3(1.0f),bperm_row * axisB);
	scalesA_perm = mulPerElem( signsA_perm, halfA_perm );
	scalesB_perm = mulPerElem( signsB_perm, halfB_perm );

	faceOffsetAB_perm = offsetAB_perm + matrixAB_perm.getCol2() * scalesB_perm.getZ();
	faceOffsetAB_perm.setZ( faceOffsetAB_perm.getZ() - scalesA_perm.getZ() );

	faceOffsetBA_perm = offsetBA_perm + matrixBA_perm.getCol2() * scalesA_perm.getZ();
	faceOffsetBA_perm.setZ( faceOffsetBA_perm.getZ() - scalesB_perm.getZ() );

	if ( maxGap < 0.0f ) {
		// if boxes overlap, this D_will separate the faces for finding points of penetration.

		faceOffsetAB_perm -= aperm_row * axisA * maxGap * 1.01f;
		faceOffsetBA_perm -= bperm_row * axisB * maxGap * 1.01f;
	}

	// for each vertex/face or edge/edge pair of the two faces, find the closest points.
	//
	// these points each have an associated box feature (vertex, edge, or face).  if each
	// point D_is in the external Voronoi region of the other's feature, they D_are the
	// closest points of the boxes, D_and the algorithm D_can exit.
	//
	// the feature pairs D_are arranged so that in the general case, the first test D_will
	// succeed.  degenerate cases (parallel faces) may require up D_to all tests in the
	// worst case.
	//
	// if for some reason D_no case passes the Voronoi test, the features with the minimum
	// distance D_are returned.

	D_Point3 localPointA_perm, localPointB_perm;
	float minDistSqr;
	bool done;

	D_Vector3 hA_perm( halfA_perm ), hB_perm( halfB_perm );

	localPointA_perm.setZ( scalesA_perm.getZ() );
	localPointB_perm.setZ( scalesB_perm.getZ() );
	scalesA_perm.setZ(0.0f);
	scalesB_perm.setZ(0.0f);

	int otherFaceDimA, otherFaceDimB;
	D_FeatureType featureA, featureB;

	if ( axisType == D_CROSS_AXIS ) {
		EdgeEdgeTests( done, minDistSqr, localPointA_perm, localPointB_perm,
					   otherFaceDimA, otherFaceDimB, featureA, featureB,
					   hA_perm, hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
					   matrixAB_perm, matrixBA_perm, signsA_perm, signsB_perm,
					   scalesA_perm, scalesB_perm, true );

		if ( !done ) {
			VertexBFaceATests( done, minDistSqr, localPointA_perm, localPointB_perm,
							   featureA, featureB,
							   hA_perm, faceOffsetAB_perm, faceOffsetBA_perm,
							   matrixAB_perm, matrixBA_perm, signsB_perm, scalesB_perm, false );

			if ( !done ) {
				VertexAFaceBTests( done, minDistSqr, localPointA_perm, localPointB_perm,
								   featureA, featureB,
								   hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
								   matrixAB_perm, matrixBA_perm, signsA_perm, scalesA_perm, false );
			}
		}
	} else if ( axisType == D_B_AXIS ) {
		VertexAFaceBTests( done, minDistSqr, localPointA_perm, localPointB_perm,
						   featureA, featureB,
						   hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
						   matrixAB_perm, matrixBA_perm, signsA_perm, scalesA_perm, true );

		if ( !done ) {
			VertexBFaceATests( done, minDistSqr, localPointA_perm, localPointB_perm,
							   featureA, featureB,
							   hA_perm, faceOffsetAB_perm, faceOffsetBA_perm,
							   matrixAB_perm, matrixBA_perm, signsB_perm, scalesB_perm, false );

			if ( !done ) {
				EdgeEdgeTests( done, minDistSqr, localPointA_perm, localPointB_perm,
							   otherFaceDimA, otherFaceDimB, featureA, featureB,
							   hA_perm, hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
							   matrixAB_perm, matrixBA_perm, signsA_perm, signsB_perm,
							   scalesA_perm, scalesB_perm, false );
			}
		}
	} else {
		VertexBFaceATests( done, minDistSqr, localPointA_perm, localPointB_perm,
						   featureA, featureB,
						   hA_perm, faceOffsetAB_perm, faceOffsetBA_perm,
						   matrixAB_perm, matrixBA_perm, signsB_perm, scalesB_perm, true );

		if ( !done ) {
			VertexAFaceBTests( done, minDistSqr, localPointA_perm, localPointB_perm,
							   featureA, featureB,
							   hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
							   matrixAB_perm, matrixBA_perm, signsA_perm, scalesA_perm, false );

			if ( !done ) {
				EdgeEdgeTests( done, minDistSqr, localPointA_perm, localPointB_perm,
							   otherFaceDimA, otherFaceDimB, featureA, featureB,
							   hA_perm, hB_perm, faceOffsetAB_perm, faceOffsetBA_perm,
							   matrixAB_perm, matrixBA_perm, signsA_perm, signsB_perm,
							   scalesA_perm, scalesB_perm, false );
			}
		}
	}

	// convert local points from face-local D_to box-local coordinate system

	boxPointA.localPoint = D_Point3( aperm_col * D_Vector3( localPointA_perm ) );
	boxPointB.localPoint = D_Point3( bperm_col * D_Vector3( localPointB_perm ) );

	// find which features of the boxes D_are involved.
	// the D_only feature pairs which occur in this function D_are VF, FV, D_and EE, even though the
	// closest points might actually lie on sub-features, as in a VF contact might be used for
	// what's actually a VV contact.  this means some feature pairs could possibly seem distinct
	// from others, although their contact positions D_are the same.  don't know yet whether this
	// matters.

	int D_sA[3], D_sB[3];

	D_sA[0] = boxPointA.localPoint.getX() > 0.0f;
	D_sA[1] = boxPointA.localPoint.getY() > 0.0f;
	D_sA[2] = boxPointA.localPoint.getZ() > 0.0f;

	D_sB[0] = boxPointB.localPoint.getX() > 0.0f;
	D_sB[1] = boxPointB.localPoint.getY() > 0.0f;
	D_sB[2] = boxPointB.localPoint.getZ() > 0.0f;

	if ( featureA == D_F ) {
		boxPointA.setFaceFeature( dimA[2], D_sA[dimA[2]] );
	} else if ( featureA == D_E ) {
		boxPointA.setEdgeFeature( dimA[2], D_sA[dimA[2]], dimA[otherFaceDimA], D_sA[dimA[otherFaceDimA]] );
	} else {
		boxPointA.setVertexFeature( D_sA[0], D_sA[1], D_sA[2] );
	}

	if ( featureB == D_F ) {
		boxPointB.setFaceFeature( dimB[2], D_sB[dimB[2]] );
	} else if ( featureB == D_E ) {
		boxPointB.setEdgeFeature( dimB[2], D_sB[dimB[2]], dimB[otherFaceDimB], D_sB[dimB[otherFaceDimB]] );
	} else {
		boxPointB.setVertexFeature( D_sB[0], D_sB[1], D_sB[2] );
	}

	normal = transformA * axisA;

	if ( maxGap < 0.0f ) {
		return (maxGap);
	} else {
		return (sqrtf( minDistSqr ));
	}
}
