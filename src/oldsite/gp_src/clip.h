#ifndef _CLIP_H
#define _CLIP_H

#include "gpbase.h"

#define GP_I_NORMALS  0x01
#define GP_I_COLORS   0x02
#define GP_I_MATERIAL 0x04

#define GP_TOP		   0x01
#define GP_BOTTOM      0x02
#define GP_RIGHT       0x04
#define GP_LEFT        0x08
#define GP_FRONT	   0x10	
#define GP_BACK        0x20

/* abstract clipping engine */
typedef struct
{
	double dfXMin, dfXMax;
    double dfYMin, dfYMax;
    double dfZMin, dfZMax;
    gpByte byFlags;
}GPClipper;

typedef GPClipper *p_GPClipper;

/* create an initilized clipping engine */
p_GPClipper CreateGPClipper();

/* get and set methods */
void SetXLimits(p_GPClipper pClipper, double dfXMin, double dfXMax);
void SetYLimits(p_GPClipper pClipper, double dfYMin, double dfYMax);
void SetZLimits(p_GPClipper pClipper, double dfZMin, double dfZMax);
void GetXLimits(p_GPClipper pClipper, double* pdfXMin, double* pdfXMax);
void GetYLimits(p_GPClipper pClipper, double* pdfYMin, double* pdfYMax);
void GetZLimits(p_GPClipper pClipper, double* pdfZMin, double* pdfZMax);
void SetClipFlags(p_GPClipper pClipper, gpByte byFlags);

/* clip line in 3-d */
void ClipLine3d(p_GPClipper pClipper, 
				p_GPVertex pV0, p_GPVertex pV1,
		    	p_GPVertex pVN0, p_GPVertex pVN1,
				gpBool *pbAccept);

/* clips in 3-D, so only one output list is produced */
void Clip3dQuad(p_GPClipper,p_GPVertex,p_GPVertex,
				p_GPVertex,p_GPVertex,p_GPVPList);

void Clip3dTriangle(p_GPClipper,p_GPVertex,
					p_GPVertex,p_GPVertex,p_GPVPList);

void Clip3dPolygon(p_GPClipper,p_GPVPList,p_GPVPList);

/* compute the outcode of a vertex.  Used for line clipping */
static void ComputeOutCode(p_GPClipper pClipper, p_GPVertex pV);

/* clip against a specific plane */
static void Clip3dPolygonAgainstXMin(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static void Clip3dPolygonAgainstXMax(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static void Clip3dPolygonAgainstYMin(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static void Clip3dPolygonAgainstYMax(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static void Clip3dPolygonAgainstZMin(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static void Clip3dPolygonAgainstZMax(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList);

static p_GPVertex IntersectX(p_GPClipper pClipper,p_GPVertex, p_GPVertex, double);
static p_GPVertex IntersectY(p_GPClipper pClipper,p_GPVertex, p_GPVertex, double);
static p_GPVertex IntersectZ(p_GPClipper pClipper,p_GPVertex, p_GPVertex, double);

static void ClipAttributes(p_GPClipper pClipper,p_GPVertex pI,
						   p_GPVertex pV1,p_GPVertex pV2,double dfT);
#endif
