#ifndef _LIGHT_H
#define _LIGHT_H

#include "gpbase.h"

#define MAXLIGHTS 8

/* structure for a single light */
typedef struct
{
	double adfTrans[4];
    double dfR;
    double dfG;
    double dfB;
	gpBool bEnabled;
}GPLight;
typedef GPLight *p_GPLight;

typedef double (*gpAttFunc)(double);

/* abstract lighting engine */
typedef struct 
{
	GPLight asLightList[MAXLIGHTS];
	double dfAR, dfAG, dfAB;
	gpAttFunc fnAtt;
}GPLighting;
typedef GPLighting *p_GPLighting;

/* INDIVIDUAL LIGHTS */

/* LIGHTING */

/* create an initilized lighting module */
p_GPLighting CreateGPLighting();

/* I suspect this function will be used most often for Gouraud shading
where each vertex has to be lit */
void LightVertexL(p_GPLighting pLighting,double adfViewpoint[],p_GPVPList pVList);

/* I can't be sure, but this will probably be used for flat shading, since 
we likely don't want to light all the vertives willy-nilly, since some 
vertices will not have their color information used */
void LightVertex(p_GPLighting pLighting,double adfViewpoint[],p_GPVertex pVertex);

/* used mostly for phong shading */
void ComputeIntensities(p_GPLighting pLighting, double adfViewpoint[],
                        double adfPosition[],double adfNormal[],
                        GPMaterial* psMaterial, 
				        gpByte *pbyR, gpByte *pbyG, gpByte *pbyB);
#endif
