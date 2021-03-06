
//
//  Copyright (C) : Please refer to the COPYRIGHT file distributed 
//   with this source distribution. 
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include "SilhouetteGeomEngine.h"
#include "Silhouette.h"
#include "../geometry/GeomUtils.h"

using namespace std;

Vec3r SilhouetteGeomEngine::_Viewpoint = Vec3r(0,0,0);
real SilhouetteGeomEngine::_translation[3] = {0,0,0};
real SilhouetteGeomEngine::_modelViewMatrix[4][4] = {{1,0,0,0},
						     {0,1,0,0},
						     {0,0,1,0},
						     {0,0,0,1}};
real SilhouetteGeomEngine::_projectionMatrix[4][4] = {{1,0,0,0},
						     {0,1,0,0},
						     {0,0,1,0},
						     {0,0,0,1}};
real SilhouetteGeomEngine::_transform[4][4] = {{1,0,0,0},
					       {0,1,0,0},
					       {0,0,1,0},
					       {0,0,0,1}};
int SilhouetteGeomEngine::_viewport[4] = {1,1,1,1};              // the viewport
real SilhouetteGeomEngine::_Focal = 0.0;
  
real SilhouetteGeomEngine::_glProjectionMatrix[4][4] = {{1,0,0,0},
							{0,1,0,0},
							{0,0,1,0},
							{0,0,0,1}};
real SilhouetteGeomEngine::_glModelViewMatrix[4][4] = {{1,0,0,0},
						       {0,1,0,0},
						       {0,0,1,0},
						       {0,0,0,1}};
real SilhouetteGeomEngine::_znear = 0.0;
real SilhouetteGeomEngine::_zfar = 100.0;
  
SilhouetteGeomEngine * SilhouetteGeomEngine::_pInstance = 0;

void SilhouetteGeomEngine::SetTransform(const real iModelViewMatrix[4][4], const real iProjectionMatrix[4][4], const int iViewport[4], real iFocal) 
{
  unsigned int i,j;
  _translation[0] = iModelViewMatrix[3][0];
  _translation[1] = iModelViewMatrix[3][1];
  _translation[2] = iModelViewMatrix[3][2];
  
  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
    {
      _modelViewMatrix[i][j] = iModelViewMatrix[j][i];
      _glModelViewMatrix[i][j] = iModelViewMatrix[i][j];
    }
  }

  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
    {
      _projectionMatrix[i][j] = iProjectionMatrix[j][i];
      _glProjectionMatrix[i][j] = iProjectionMatrix[i][j];
    }
  }
      
  for(i=0; i<4; i++){
    for(j=0; j<4; j++)
    {
      _transform[i][j] = 0;
      for(unsigned int k=0; k<4; k++)
        _transform[i][j] += _projectionMatrix[i][k] * _modelViewMatrix[k][j];
    }
  }
      
  for(i=0; i<4; i++){
    _viewport[i] = iViewport[i];
  }
  _Focal = iFocal;
}

void SilhouetteGeomEngine::SetFrustum(real iZNear, real iZFar) 
{
  _znear = iZNear;
  _zfar = iZFar;
}

void SilhouetteGeomEngine::retrieveViewport(int viewport[4]){
  memcpy(viewport, _viewport, 4*sizeof(int));
}
//#define HUGE 1e9

void SilhouetteGeomEngine::ProjectSilhouette(vector<SVertex*>& ioVertices)
{
  Vec3r newPoint;
  //  real min=HUGE;
  //  real max=-HUGE;
  vector<SVertex*>::iterator sv, svend;
  
  for(sv=ioVertices.begin(), svend=ioVertices.end();
      sv!=svend;
      sv++)
    {
      GeomUtils::fromWorldToImage((*sv)->point3D(), newPoint, _modelViewMatrix, _projectionMatrix, _viewport);
      //newPoint[2] = (-newPoint[2]-_znear)/(_zfar-_znear); // normalize Z between 0 and 1
      (*sv)->SetPoint2D(newPoint);
      //cerr << (*sv)->point2d().z() << "  ";
      //      real d=(*sv)->point2d()[2];
      //      if (d>max) max =d;
      //      if (d<min) min =d;
    }
      //  for(sv=ioVertices.begin(), svend=ioVertices.end();
      //      sv!=svend;
      //      sv++)
      //    {
      //      Vec3r P((*sv)->point2d());
      //      (*sv)->SetPoint2D(Vec3r(P[0], P[1], 1.0-(P[2]-min)/(max-min)));
      //      //cerr<<(*sv)->point2d()[2]<<"  ";
      //    }
}

void SilhouetteGeomEngine::ProjectSilhouette(SVertex* ioVertex)
{
  Vec3r newPoint;
  //  real min=HUGE;
  //  real max=-HUGE;
  vector<SVertex*>::iterator sv, svend;
  GeomUtils::fromWorldToImage(ioVertex->point3D(), newPoint, _modelViewMatrix, _projectionMatrix, _viewport);
  //newPoint[2] = (-newPoint[2]-_znear)/(_zfar-_znear); // normalize Z between 0 and 1
  ioVertex->SetPoint2D(newPoint);  
}

real SilhouetteGeomEngine::ImageToWorldParameter(FEdge *fe, real t)
{
  return t;


  // Note: using the Renderman/RIB camera settings makes this code break.  I can't figure out why.
  // am punting by just returning "t".  The way "T" is being computed here is a bit of a hack anyway and
  // a ray intersection with the triangle's plane would be better anyway. -Aaron

  // the fromRetinaToCamera line might a culprit; that function looks wrong.



  // we need to compute for each parameter t the corresponding 
  // parameter T which gives the intersection in 3D.
  //currentEdge = (*fe);
  Vec3r A = (fe)->vertexA()->point3D();
  Vec3r B = (fe)->vertexB()->point3D();
  Vec3r Ai = (fe)->vertexA()->point2D();
  Vec3r Bi = (fe)->vertexB()->point2D();
  Vec3r AB = Vec3r((B-A)); // the edge
  Vec3r ABi(Bi-Ai);
  Vec3r Ac, Bc;
  GeomUtils::fromWorldToCamera(A, Ac, _modelViewMatrix);
  GeomUtils::fromWorldToCamera(B, Bc, _modelViewMatrix);
      
  Vec3r Ii = Vec3r((Ai+t*ABi)); // I image
  // let us compute the 3D point corresponding to the 2D intersection point
  // and lying on the edge:
  Vec3r Ir, Ic;
  GeomUtils::fromImageToRetina(Ii, Ir, _viewport);
  GeomUtils::fromRetinaToCamera(Ir, Ic, -_Focal, _projectionMatrix);
  
  real T;
  T = (Ic[2]*Ac[1] - Ic[1]*Ac[2])/(Ic[1]*(Bc[2]-Ac[2])-Ic[2]*(Bc[1]-Ac[1]));

  printf("========================\n");
  printf("A= [%f %f %f], Ac= [%f %f %f], B= [%f %f %f], Bc= [%f %f %f]\n",
	 A[0], A[1], A[2], Ac[0], Ac[1], Ac[2], B[0], B[1], B[2], Bc[0], Bc[1], Bc[2]);
  printf("Ai= [%f %f %f], Bi= [%f %f %f]\n",
	 Ai[0], Ai[1], Ai[2], Bi[0], Bi[1], Bi[2]);
  printf("Ii= [%f %f %f], Ir= [%f %f %f], Ic= [%f %f %f]\n",
	 Ii[0], Ii[1], Ii[2], Ir[0], Ir[1], Ir[2], Ic[0], Ic[1], Ic[2]);
  printf("t = %f, T = %f\n", t, T);
  printf("viewport = %d %d %d %d\n", _viewport[0], _viewport[1], _viewport[2], _viewport[3]);

  Vec3r Ai_test, Ar_test;

  GeomUtils::fromCameraToRetina(Ac, Ar_test, _projectionMatrix);
  GeomUtils::fromRetinaToImage(Ar_test, Ai_test, _viewport);

  printf("Ar_test= [%f %f %f], Ai_test= [%f %f %f]\n",
	 Ar_test[0], Ar_test[1], Ar_test[2], Ai_test[0], Ai_test[1], Ai_test[2]);

  

  assert( (T >= -.001) && (T < 1.001));

  return T;
}

Vec3r SilhouetteGeomEngine::WorldToImage(const Vec3r& M)

{

  Vec3r newPoint;
  GeomUtils::fromWorldToImage(M, newPoint, _transform, _viewport);

  //  newPoint[2] = (-newPoint[2]-_znear)/(_zfar-_znear); // normalize Z between 0 and 1
  // Aaron: this line is redundant for the RIB camera, which already has this built in

  return newPoint;

}

Vec2r SilhouetteGeomEngine::WorldToImage2(const Vec3r & M)
{
  Vec3r newPoint = WorldToImage(M);
  return Vec2r(newPoint.x(), newPoint.y());
}


bool SilhouetteGeomEngine::IsInClippingPlanes(const Vec3r & pt)
{
  Vec3r newPoint;
  GeomUtils::fromWorldToImage(pt, newPoint, _transform, _viewport);
  bool result= newPoint[2] >= 0 && newPoint[2] <=1;


//  GeomUtils::fromWorldToCamera(pt, newPoint, _modelViewMatrix);
//  bool result = newPoint[2] >= _znear && newPoint[2] <= _zfar;
/*
  if (!result)
    {
      printf("CLIPPED\n");

      printf(" pt3d = [%f %f %f 1]'; pt2d = [%f %f %f]'; znear = %f, zfar = %f\n", 
	     pt[0], pt[1], pt[2], 	     newPoint[0], newPoint[1], newPoint[2], 
	     _znear, _zfar);
    }
*/
  return result;
}
