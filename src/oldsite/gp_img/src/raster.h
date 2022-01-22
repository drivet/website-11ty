#ifndef _RASTER_H
#define _RASTER_H

#include "gpbase.h"
#include "light.h"

/* abstract rasterizing engine */
typedef struct
{
	gpByte *pRBuffer;
	gpByte *pGBuffer;
	gpByte *pBBuffer;
	gpInt32 *pZBuffer;

	gpInt32 nXSize;
	gpInt32 nYSize;
}GPRaster;

typedef GPRaster *p_GPRaster;

/* the rasterizer manages the framebuffer.  The frame buffer can 
consist of a few buffers, like the image buffer (32-bit) and the 
z-buffer (32-bit).  Maybe more later, but that's all for now. */

/* creates and initializes a rasterizer and buffers */
p_GPRaster CreateGPRasterizer(gpInt32 nX, gpInt32 nY);

/* initialize buffer */
void ClearAllBuffers(p_GPRaster pRaster);

/*********** QUADS **************/

/* flat shaded, non-z-buffered quad */
void R_QuadFlat(p_GPRaster pRaster,
			    gpByte byR, gpByte byG, gpByte byB,
			    p_GPVertex pV0,p_GPVertex pV1,
			    p_GPVertex pV2,p_GPVertex pV3);

/* flat shaded, z-buffered quad */
void R_QuadFlatZ(p_GPRaster pRaster, 
			     gpByte byR, gpByte byG, gpByte byB,
			     p_GPVertex pV0,p_GPVertex pV1,
			     p_GPVertex pV2,p_GPVertex pV3);

/* gouraud shaded, z-buffered quad */
void R_QuadGouraud(p_GPRaster pRaster, 
				   p_GPVertex pV0,p_GPVertex pV1,
				   p_GPVertex pV2,p_GPVertex pV3);

/* gouraud shaded, non-z-buffered quad */
void R_QuadGouraudZ(p_GPRaster pRaster, 
				    p_GPVertex pV0,p_GPVertex pV1,
				    p_GPVertex pV2,p_GPVertex pV3);

/* wire, flat non-z-buffered quad */
void R_QuadWireFlat(p_GPRaster pRaster,
				    gpByte byR, gpByte byG, gpByte byB,
			        p_GPVertex pV0,p_GPVertex pV1,
			        p_GPVertex pV2,p_GPVertex pV3);

/* wire, flat, z-buffered quad */
void R_QuadWireFlatZ(p_GPRaster pRaster, 
				     gpByte byR, gpByte byG, gpByte byB,
				     p_GPVertex pV0,p_GPVertex pV1,
				     p_GPVertex pV2,p_GPVertex pV3);

/* wire, gouraud, z-buffered quad */
void R_QuadWireGouraud(p_GPRaster pRaster, 
					   p_GPVertex pV0,p_GPVertex pV1,
					   p_GPVertex pV2,p_GPVertex pV3);

/* wire gouraud, non-z-buffered quad */
void R_QuadWireGouraudZ(p_GPRaster pRaster, 
					    p_GPVertex pV0,p_GPVertex pV1,
					    p_GPVertex pV2,p_GPVertex pV3);

/***************** TRIANGLES ***************/

void R_TriFlat(p_GPRaster pRaster,
			   p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2,
			   gpByte byR,gpByte byG, gpByte byB);

void R_TriFlatZ(p_GPRaster pRaster,
				p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2,
				gpByte byR,gpByte byG, gpByte byB);

void R_TriGouraud(p_GPRaster pRaster,
				  p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

void R_TriGouraudZ(p_GPRaster pRaster,
				   p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

/* wire, flat, non-z-buffered triangle */
void R_TriWireFlat(p_GPRaster pRaster, 
				 gpByte byR, gpByte byG, gpByte byB,
				 p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

/* wire, flat, z-buffered triangle */
void R_TriWireFlatZ(p_GPRaster pRaster, 
				  gpByte byR, gpByte byG, gpByte byB,
				  p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

/* wire, gouraud, z-buffered triangle */
void R_TriWireGouraud(p_GPRaster pRaster, 
					p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

/* wire, gouraud, non-z-buffered triangle */
void R_TriWireGouraudZ(p_GPRaster pRaster, 
					 p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2);

/************* POLYGONS **************/

/* flat shaded, non-z-buffered polygon */
void R_PolygonFlat(p_GPRaster pRaster, 
				 gpByte byR, gpByte byG, gpByte byB,
				 p_GPVPList pList);

/* flat shaded, z-buffered polygon */
void R_PolygonFlatZ(p_GPRaster pRaster, 
				  gpByte byR, gpByte byG, gpByte byB,
				  p_GPVPList pList);

/* gouraud shaded, z-buffered polygon */
void R_PolygonGouraud(p_GPRaster pRaster, p_GPVPList pList);

/* gouraud shaded, non-z-buffered polygon */
void R_PolygonGouraudZ(p_GPRaster pRaster, p_GPVPList pList);

/* wire, flat, non-z-buffered polygon */
void R_PolygonWireFlat(p_GPRaster pRaster, 
					 gpByte byR, gpByte byG, gpByte byB,
					 p_GPVPList pList);

/* wire, flat, z-buffered polygon */
void R_PolygonWireFlatZ(p_GPRaster pRaster, 
					  gpByte byR, gpByte byG, gpByte byB,
					  p_GPVPList pList);

/* wire, gouraud, z-buffered polygon */
void R_PolygonWireGouraud(p_GPRaster pRaster, p_GPVPList pList);

/* wire, gouraud, non-z-buffered polygon */
void R_PolygonWireGouraudZ(p_GPRaster pRaster, p_GPVPList pList);

/************** LINES *************/

void R_LineFlat(p_GPRaster pRaster, 
				gpByte byR, gpByte byG, gpByte byB,
				p_GPVertex pV0,p_GPVertex pV1);

void R_LineFlatZ(p_GPRaster pRaster, 
				 gpByte byR, gpByte byG, gpByte byB,
				 p_GPVertex pV0,p_GPVertex pV1);

/* gouraud line, z-buffered */
void R_LineGouraudZ(p_GPRaster pRaster,p_GPVertex pV0,p_GPVertex pV1); 

/* gouraud line, non-zuffered */
void R_LineGouraud(p_GPRaster pRaster,p_GPVertex pV0,p_GPVertex pV1); 

/*********** POINTS ***************/

/* point, z-buffered */
void R_PointZ(p_GPRaster pRaster,p_GPVertex pVertex);

/* point, non-z-buffered */
void R_Point(p_GPRaster pRaster,p_GPVertex pVertex);


/* STATIC BACK-END FUNCTIONS - NOT PART OF THE PUBLIC API */
static void ScanLineZ(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,gpInt32 *pnZ0,
			   gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,gpInt32 *pnZ1,
			   gpInt32 nZCur0, gpInt32 nZCur1,
			   gpByte byR,gpByte byG,gpByte byB);

static void ScanLineGouraud(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,
					 gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,
					 gpInt32 nRCur0, gpInt32 nRCur1,
					 gpInt32 nGCur0, gpInt32 nGCur1,
					 gpInt32 nBCur0, gpInt32 nBCur1);

static void ScanLineGouraudZ(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,gpInt32 *pnZ0,
					  gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,gpInt32 *pnZ1,
					  gpInt32 nRCur0, gpInt32 nRCur1,
					  gpInt32 nGCur0, gpInt32 nGCur1,
					  gpInt32 nBCur0, gpInt32 nBCur1,
					  gpInt32 nZCur0, gpInt32 nZCur1);
#endif
