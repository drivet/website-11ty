#ifndef _PIPE_H
#define _PIPE_H
/* 
The following API is writtem in C with a C++-like design.
I originally designed it in the latter language, but then thought the 
better of it.  C++ is great for abstract programming, but it 
introduces to many "unknown variables" when you want to do low-level 
programming. Since I may want to do this in the future, I think it's 
best to do this in C.

A gpPipe is an opaque structure holding information about the
current rendering pipeline. The pipe defines the way rendering is
done. It holds lighting and shading information, buffer information,
viewing and projection information, clipping, etc. The word "pipe"
is a metaphor. One end of the pipe accepts abstract objects, and
the other end of the pipe renders them pixel for pixel in the buffer.
Another way to think about this is that the pipe is your ticket
to either

a. Graphics hardware
b. Graphics software algorithms
c. Some combination of the two.
*/

#include "clip.h"
#include "light.h"
#include "gpbase.h"
#include "raster.h"
#include "view.h"

/* Shading models */
#define GP_FLAT		0
#define GP_GOURAUD	1

/* polygon models */
#define GP_FILLED	0
#define GP_WIRE		1

/* primitive identifiers */
#define GP_QUADS      0
#define GP_TRIANGLES  1
#define GP_LINES      2
#define GP_LINE_STRIP 3

/* define the rendering paths; the user never sees these */
#define _GP_WIRE_FLAT			0
#define _GP_WIRE_FLAT_L			1
#define _GP_WIRE_FLAT_Z			2
#define _GP_WIRE_FLAT_LZ		3
#define _GP_WIRE_GOURAUD		4
#define _GP_WIRE_GOURAUD_L		5
#define _GP_WIRE_GOURAUD_Z		6
#define _GP_WIRE_GOURAUD_LZ		7
#define _GP_FILLED_FLAT			8
#define _GP_FILLED_FLAT_L		9	
#define _GP_FILLED_FLAT_Z		10
#define _GP_FILLED_FLAT_LZ		11
#define _GP_FILLED_GOURAUD		12
#define _GP_FILLED_GOURAUD_L	13
#define _GP_FILLED_GOURAUD_Z	14
#define _GP_FILLED_GOURAUD_LZ	15

typedef void (*GPVertexFunc)(double,double,double);
typedef void (*GPEndFunc)();

/***************** FRONT END ***/
/* handle to a gpPipe. Really just a pointer, but we don't
want people messing around with the innards*/

/* structure for am abstract graphics pipe */
typedef struct _GPPipe
{
	p_GPView pView;
	p_GPClipper pClipper;
	p_GPRaster pRaster;
	p_GPLighting pLighting;

	int nShadingModel,nRenderModel;
	gpBool bDepth, bLighting;

	/* pointer to a vertex function*/
	GPVertexFunc pfnVertex;

	/* pointer to an end function*/
	GPEndFunc pfnEnd;

	/* current material structure */
	GPMaterial sCurrentMat;

	/* current normal */
	double adfCurrentNorm[4];
}GPPipe;

typedef GPPipe *p_GPPipe;

/* Create a fresh pipe */
p_GPPipe CreateGPPipe(gpInt32,gpInt32);

/* picks a current pipe */
void gpSetCurrentPipe(p_GPPipe pNewPipe);

/* Clears all the buffers */
void gpClearAllBuffers();

/* rendering model - GP_WIRE or GP_FILLED */
void gpSetRendering(int nModel);

/* shading model - one of GP_FLAT, GP_GOURAUD */
void gpSetShading(int nModel);

/* Enable or disable lighting */
void gpEnableLighting(gpBool bLighting);

/* Enable or disable depth testing */
void gpEnableDepth(gpBool bDepth);

/* Get the buffers */
gpByte *gpGetRedBuffer();
gpByte *gpGetGreenBuffer();
gpByte *gpGetBlueBuffer();

/* LIGHT INFO */
void gpLightEnable(int nLight,gpBool bEnable);
void gpSetLightPos(int nLight,double dfX,double dfY,double dfZ,double dfW);
void gpSetLightColor(int nLight,double dfR,double dfG,double dfB);

/* set the ambient light of the environment */
void gpSetAmbientLight(double dfR, double dfG, double dfB);

/* VIEW INFO*/

/* define the view reference coordinate system.  Defines a matrix that
transforms world coordinates into eye coordinates */
void gpDefineVRC(double dfVRPX, double dfVRPY, double dfVRPZ, 
		         double dfVPNX, double dfVPNY, double dfVPNZ, 
		         double dfVUPX, double dfVUPY, double dfVUPZ);

/* define the view mapping matrix */
void gpDefineViewMapping(GPViewMap* psMap);

void gpDefineViewport(gpInt32 nXOff, gpInt32 nYOff, 
					  gpInt32 nXSize, gpInt32 nYSize);

/* Immediate Rendering API */
/* modeled heavily on OpenGL */

void gpBegin(int);
void gpVertex(double,double,double);
void gpNormal(double,double,double);
void gpColor(double,double,double);
void gpMaterial(GPMaterial*);
void gpEnd();

/************ PRIVATE RENDERING PATH FUNCTIONS *************/

static void ConfigurePipe(int nPrim,int nPath);
static int GetRenderingPath();

static void _V_QUADS_FF(double,double,double);
static void _V_QUADS_FFL(double,double,double);
static void _V_QUADS_FFZ(double,double,double);
static void _V_QUADS_FFLZ(double,double,double);
static void _V_QUADS_FG(double,double,double);
static void _V_QUADS_FGL(double,double,double);
static void _V_QUADS_FGZ(double,double,double);
static void _V_QUADS_FGLZ(double,double,double);
static void _V_QUADS_WF(double,double,double);
static void _V_QUADS_WFL(double,double,double);
static void _V_QUADS_WFZ(double,double,double);
static void _V_QUADS_WFLZ(double,double,double);
static void _V_QUADS_WG(double,double,double);
static void _V_QUADS_WGL(double,double,double);
static void _V_QUADS_WGZ(double,double,double);
static void _V_QUADS_WGLZ(double,double,double);

static void _V_TRI_FF(double,double,double);
static void _V_TRI_FFL(double,double,double);
static void _V_TRI_FFZ(double,double,double);
static void _V_TRI_FFLZ(double,double,double);
static void _V_TRI_FG(double,double,double);
static void _V_TRI_FGL(double,double,double);
static void _V_TRI_FGZ(double,double,double);
static void _V_TRI_FGLZ(double,double,double);
static void _V_TRI_WF(double,double,double);
static void _V_TRI_WFL(double,double,double);
static void _V_TRI_WFZ(double,double,double);
static void _V_TRI_WFLZ(double,double,double);
static void _V_TRI_WG(double,double,double);
static void _V_TRI_WGL(double,double,double);
static void _V_TRI_WGZ(double,double,double);
static void _V_TRI_WGLZ(double,double,double);

static void _V_LINES_F(double,double,double);
static void _V_LINES_FL(double,double,double);
static void _V_LINES_FZ(double,double,double);
static void _V_LINES_FLZ(double,double,double);
static void _V_LINES_G(double,double,double);
static void _V_LINES_GL(double,double,double);
static void _V_LINES_GZ(double,double,double);
static void _V_LINES_GLZ(double,double,double);

static void _V_LINE_STRIP_F(double,double,double);
static void _V_LINE_STRIP_FL(double,double,double);
static void _V_LINE_STRIP_FZ(double,double,double);
static void _V_LINE_STRIP_FLZ(double,double,double);
static void _V_LINE_STRIP_G(double,double,double);
static void _V_LINE_STRIP_GL(double,double,double);
static void _V_LINE_STRIP_GZ(double,double,double);
static void _V_LINE_STRIP_GLZ(double,double,double);

static void ConfigureRenderingPath();

#endif
