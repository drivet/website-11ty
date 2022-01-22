#ifndef _VIEW_H
#define _VIEW_H

#include "gpbase.h"
#include "light.h"

#define GP_PARALLEL    0
#define GP_PERSPECTIVE 1

typedef struct
{
/* parallel or perspective? */
    int nProjType;

/* projection reference point */
    double dfPRPU;
    double dfPRPV;
    double dfPRPN;
    
/* Plane distances, normal to N-axis in VRC*/
    double dfVPD, dfBPD, dfFPD;
    
/* View plane window */    
    double dfUMin, dfUMax; 
    double dfVMin, dfVMax;
}GPViewMap;
typedef GPViewMap *p_GPViewMap;

typedef struct
{
	double adfW2E[16];
	double adfE2N[16];
	double adfN2R[16];
	
	double adfW2R[16];
	double adfE2R[16];
	gpBool bW2RDirty;
	gpBool bE2RDirty;
	GPViewMap sVM;

	/* for the clipper */
	gpInt32 nX1,nY1;
	gpInt32 nX2,nY2;
}GPView;
typedef GPView *p_GPView;

/* create and initlize a viewing module */
p_GPView CreateGPView();

void DefineVRC(p_GPView pView,
			   double dfVRPX, double dfVRPY, double dfVRPZ, 
			   double dfVPNX, double dfVPNY, double dfVPNZ, 
			   double dfVUPX, double dfVUPY, double dfVUPZ);

void DefineViewMapping(p_GPView pView,p_GPViewMap pVM);
void DefineViewport(p_GPView pView, gpInt32 nXOff, gpInt32 nYOff, 
					gpInt32 nXSize, gpInt32 nYSize);

GPViewMap GetViewMapping(p_GPView pView);
void GetViewpoint(p_GPView, double adfViewpoint[]);

void World2Raster(p_GPView pView, p_GPVertex pV);
void World2Eye(p_GPView pView, p_GPVertex pV);
void Eye2Raster(p_GPView pView, p_GPVertex pV);
void Normal2Eye(p_GPView pView, p_GPVertex pV);
void Light2Eye(p_GPView pView,p_GPLight pLight);

#endif
