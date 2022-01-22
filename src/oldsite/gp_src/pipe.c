#include "pipe.h"
#include "algebra.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

/*********************************************************/
/* YES!! I'm using GLOBAL variables.  They won't be used 
outside this file and they clean up a bit of the parameter
passing */

/* the current pipe */
p_GPPipe pPipe;

/* these are used for temporarily holding vertices for 
lines, triangles and quads */
p_GPVertex pVertices[4];
GPVertex sV0;
GPVertex sV1;
GPVertex sV2;
GPVertex sV3;

/* used to figure out where we are in vertex specification */
gpInt32 nVIndex;

/**********************************************************/
/* Create a fresh pipe */
p_GPPipe CreateGPPipe(gpInt32 nX, gpInt32 nY)
{
	p_GPPipe pPipe = (p_GPPipe)malloc(sizeof(GPPipe));
	
	pPipe->pRaster = CreateGPRasterizer(nX,nY);

	pPipe->pView = CreateGPView();
	pPipe->pClipper = CreateGPClipper();
	pPipe->pLighting = CreateGPLighting();

	pPipe->nShadingModel = GP_FLAT;
	pPipe->nRenderModel = GP_WIRE;
	pPipe->bDepth = FALSE; 
	pPipe->bLighting = FALSE;

	memset(pPipe->adfCurrentNorm,0,sizeof(double)*4);
	memset(&pPipe->sCurrentMat,0,sizeof(GPMaterial));

	pPipe->pfnVertex = NULL;
	pPipe->pfnEnd = NULL;

	return pPipe;
}

/* picks a current pipe */
void gpSetCurrentPipe(p_GPPipe pNewPipe)
{
	pPipe = pNewPipe;
}

/* clears all the buffers */
void gpClearAllBuffers()
{
	ClearAllBuffers(pPipe->pRaster);
}

/* rendering model - GP_WIRE or GP_FILLED */
void gpSetRendering(int nModel)
{
	pPipe->nRenderModel = nModel;
}

/* shading model - one of GP_FLAT, GP_GOURAUD */
void gpSetShading(int nModel)
{
	pPipe->nShadingModel = nModel;
}

gpByte *gpGetRedBuffer()
{
	return pPipe->pRaster->pRBuffer;
}

gpByte *gpGetGreenBuffer()
{
	return pPipe->pRaster->pGBuffer;
}

gpByte *gpGetBlueBuffer()
{
	return pPipe->pRaster->pBBuffer;
}

/* Enable or disable lighting */
void gpEnableLighting(gpBool bLighting)
{
	pPipe->bLighting = bLighting;
}

/* Enable or disable depth testing */
void gpEnableDepth(gpBool bDepth)
{
	pPipe->bDepth = bDepth;
}

void gpLightEnable(int nLight,gpBool bEnable)
{
	pPipe->pLighting->asLightList[nLight].bEnabled = bEnable;
}

void gpSetLightPos(int nLight,double dfX,double dfY,double dfZ,double dfW)
{
	/* if dfW is 0 then the light is directional;
	non-zero means point source */

	pPipe->pLighting->asLightList[nLight].adfTrans[0] = dfX;
	pPipe->pLighting->asLightList[nLight].adfTrans[1] = dfY;
	pPipe->pLighting->asLightList[nLight].adfTrans[2] = dfZ;
	pPipe->pLighting->asLightList[nLight].adfTrans[3] = dfW;

	/* go from world to eye right now.  This might cause problems if we want
	to querey the light later, but since we can't at the moment, I'll
	think about that problem later */

	Light2Eye(pPipe->pView,&pPipe->pLighting->asLightList[nLight]);
}

void gpSetLightColor(int nLight,double dfR,double dfG,double dfB)
{
	pPipe->pLighting->asLightList[nLight].dfR = dfR;
	pPipe->pLighting->asLightList[nLight].dfG = dfG;
	pPipe->pLighting->asLightList[nLight].dfB = dfB;
}

/* set the ambient light of the environment */
void gpSetAmbientLight( double dfR, double dfG, double dfB)
{
	pPipe->pLighting->dfAR = dfR;
	pPipe->pLighting->dfAG = dfG;
	pPipe->pLighting->dfAB = dfB;
}

/* define the view reference coordinate system.  Defines a matrix that
transforms world coordinates into eye coordinates */
void gpDefineVRC(double dfVRPX, double dfVRPY, double dfVRPZ, 
		         double dfVPNX, double dfVPNY, double dfVPNZ, 
		         double dfVUPX, double dfVUPY, double dfVUPZ)
{
	DefineVRC(pPipe->pView,dfVRPX,dfVRPY,dfVRPZ,
		dfVPNX,dfVPNY,dfVPNZ,dfVUPX, dfVUPY, dfVUPZ);
}

/* define the view mapping matrix */
void gpDefineViewMapping( GPViewMap* psMap)
{
	DefineViewMapping(pPipe->pView,psMap);
}

void gpDefineViewport( gpInt32 nXOff, gpInt32 nYOff, 
					  gpInt32 nXSize, gpInt32 nYSize)
{
	DefineViewport(pPipe->pView, nXOff, nYOff, nXSize, nYSize);
	
	/* reset the clipping limits */
	SetXLimits(pPipe->pClipper, nXOff, nXOff + nXSize - 1);
	SetYLimits(pPipe->pClipper, nYOff, nYOff + nYSize - 1);
	SetZLimits(pPipe->pClipper, 0, 1);
}

/**************************************************************/
/* Immediate rendering API */

void gpBegin(int nPrim)
{
	nVIndex = 0;

	/* set up the vertex array */
	pVertices[0] = &sV0;
	pVertices[1] = &sV1;
	pVertices[2] = &sV2;
	pVertices[3] = &sV3;

	/* figure out which specific functions to use */
	ConfigurePipe(nPrim,GetRenderingPath());
}

void gpVertex(double dfX, double dfY, double dfZ)
{
	(*(pPipe->pfnVertex))(dfX,dfY,dfZ);
}

/* these functions change the state of the 
pipe for rendering purposes */

void gpNormal(double dfX, double dfY, double dfZ)
{
	pPipe->adfCurrentNorm[0] = dfX;
	pPipe->adfCurrentNorm[1] = dfY;
	pPipe->adfCurrentNorm[2] = dfZ;
	pPipe->adfCurrentNorm[3] = 0.0;
}

void gpColor(double dfR, double dfG, double dfB)
{
	pPipe->sCurrentMat.dfDR = dfR;
	pPipe->sCurrentMat.dfDG = dfG;
	pPipe->sCurrentMat.dfDB = dfB;
}

void gpMaterial(GPMaterial* pMat)
{
	memcpy(&pPipe->sCurrentMat,pMat,sizeof(GPMaterial));
}

void gpEnd()
{
	if(pPipe->pfnEnd)
	{
		(*(pPipe->pfnEnd))();
	}
}

/**************************************************/
/* PRIVATE RENDERING HELPER FUNCTIONS */

void ConfigurePipe(int nPrim,int nPath)
{
	/* configure the clipper */
	SetXLimits(pPipe->pClipper, pPipe->pView->nX1, pPipe->pView->nX2);
	SetYLimits(pPipe->pClipper, pPipe->pView->nY1, pPipe->pView->nY2);
	SetZLimits(pPipe->pClipper, 0, 2147483647);

	/* pick the proper vertex functions */
	switch(nPrim)
	{
	case GP_QUADS:
		switch(nPath)
		{
		case _GP_FILLED_FLAT:
			pPipe->pfnVertex = _V_QUADS_FF;
			pPipe->pfnEnd = NULL;

			SetClipFlags(pPipe->pClipper, 0);
			
			break;
		case _GP_FILLED_FLAT_L:
			pPipe->pfnVertex = _V_QUADS_FFL;
			pPipe->pfnEnd = NULL;

			SetClipFlags(pPipe->pClipper, 0);
			
			break;
		case _GP_FILLED_FLAT_Z:
			pPipe->pfnVertex = _V_QUADS_FFZ;
			pPipe->pfnEnd = NULL;
			
			SetClipFlags(pPipe->pClipper, 0);
			
			break;
		case _GP_FILLED_FLAT_LZ:
			pPipe->pfnVertex = _V_QUADS_FFLZ;
			pPipe->pfnEnd = NULL;
			
			SetClipFlags(pPipe->pClipper, 0);
			
			break;
		case _GP_FILLED_GOURAUD:
			pPipe->pfnVertex = _V_QUADS_FG;
			pPipe->pfnEnd = NULL;
			

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);
			
			break;
		case _GP_FILLED_GOURAUD_L:
			pPipe->pfnVertex = _V_QUADS_FGL;
			pPipe->pfnEnd = NULL;

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_FILLED_GOURAUD_Z:
			pPipe->pfnVertex = _V_QUADS_FGZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_FILLED_GOURAUD_LZ:
			pPipe->pfnVertex = _V_QUADS_FGLZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_FLAT:
			pPipe->pfnVertex = _V_QUADS_WF;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_L:
			pPipe->pfnVertex = _V_QUADS_WFL;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_Z:
			pPipe->pfnVertex = _V_QUADS_WFZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_LZ:
			pPipe->pfnVertex = _V_QUADS_WFLZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_GOURAUD:
			pPipe->pfnVertex = _V_QUADS_WG;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_L:
			pPipe->pfnVertex = _V_QUADS_WGL;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_Z:
			pPipe->pfnVertex = _V_QUADS_WGZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_LZ:
			pPipe->pfnVertex = _V_QUADS_WGLZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		}
		break;
	case GP_TRIANGLES:
		switch(nPath)
		{
		case _GP_FILLED_FLAT:
			pPipe->pfnVertex = _V_TRI_FF;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_FILLED_FLAT_L:
			pPipe->pfnVertex = _V_TRI_FFL;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_FILLED_FLAT_Z:
			pPipe->pfnVertex = _V_TRI_FFZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_FILLED_FLAT_LZ:
			pPipe->pfnVertex = _V_TRI_FFLZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_FILLED_GOURAUD:
			pPipe->pfnVertex = _V_TRI_FG;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_FILLED_GOURAUD_L:
			pPipe->pfnVertex = _V_TRI_FGL;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_FILLED_GOURAUD_Z:
			pPipe->pfnVertex = _V_TRI_FGZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_FILLED_GOURAUD_LZ:
			pPipe->pfnVertex = _V_TRI_FGLZ;
			pPipe->pfnEnd = NULL; 

			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_FLAT:
			pPipe->pfnVertex = _V_TRI_WF;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_L:
			pPipe->pfnVertex = _V_TRI_WFL;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_Z:
			pPipe->pfnVertex = _V_TRI_WFZ;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_FLAT_LZ:
			pPipe->pfnVertex = _V_TRI_WFLZ;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, 0);

			break;
		case _GP_WIRE_GOURAUD:
			pPipe->pfnVertex = _V_TRI_WG;
			pPipe->pfnEnd = NULL; 
			
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_L:
			pPipe->pfnVertex = _V_TRI_WGL;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_Z:
			pPipe->pfnVertex = _V_TRI_WGZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		case _GP_WIRE_GOURAUD_LZ:
			pPipe->pfnVertex = _V_TRI_WGLZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		}
		break;

	case GP_LINES:
		switch(nPath)
		{
		case _GP_FILLED_FLAT:
		case _GP_WIRE_FLAT:
			pPipe->pfnVertex = _V_LINES_F;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, 0);

			break;
		
		case _GP_FILLED_FLAT_L:	
		case _GP_WIRE_FLAT_L:
			pPipe->pfnVertex = _V_LINES_FL;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;
		
		case _GP_FILLED_FLAT_Z:
		case _GP_WIRE_FLAT_Z:
			pPipe->pfnVertex = _V_LINES_FZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;
			
		case _GP_FILLED_FLAT_LZ:
		case _GP_WIRE_FLAT_LZ:
			pPipe->pfnVertex = _V_LINES_FLZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;

		case _GP_FILLED_GOURAUD:
		case _GP_WIRE_GOURAUD:
			pPipe->pfnVertex = _V_LINES_G;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_L:
		case _GP_WIRE_GOURAUD_L:
			pPipe->pfnVertex = _V_LINES_GL;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_Z:
		case _GP_WIRE_GOURAUD_Z:
			pPipe->pfnVertex = _V_LINES_GZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_LZ:
		case _GP_WIRE_GOURAUD_LZ:
			pPipe->pfnVertex = _V_LINES_GLZ;
			pPipe->pfnEnd = NULL; 
				
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		}
		break;

	case GP_LINE_STRIP:
		switch(nPath)
		{
		case _GP_FILLED_FLAT:
		case _GP_WIRE_FLAT:
			pPipe->pfnVertex = _V_LINE_STRIP_F;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;
		
		case _GP_FILLED_FLAT_L:	
		case _GP_WIRE_FLAT_L:
			pPipe->pfnVertex = _V_LINE_STRIP_FL;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;
		
		case _GP_FILLED_FLAT_Z:
		case _GP_WIRE_FLAT_Z:
			pPipe->pfnVertex = _V_LINE_STRIP_FZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;
			
		case _GP_FILLED_FLAT_LZ:
		case _GP_WIRE_FLAT_LZ:
			pPipe->pfnVertex = _V_LINE_STRIP_FLZ;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, 0);

			break;

		case _GP_FILLED_GOURAUD:
		case _GP_WIRE_GOURAUD:
			pPipe->pfnVertex = _V_LINE_STRIP_G;
			pPipe->pfnEnd = NULL; 
					
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_L:
		case _GP_WIRE_GOURAUD_L:
			pPipe->pfnVertex = _V_LINE_STRIP_GL;
			pPipe->pfnEnd = NULL; 
						
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_Z:
		case _GP_WIRE_GOURAUD_Z:
			pPipe->pfnVertex = _V_LINE_STRIP_GZ;
			pPipe->pfnEnd = NULL; 
						
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;

		case _GP_FILLED_GOURAUD_LZ:
		case _GP_WIRE_GOURAUD_LZ:
			pPipe->pfnVertex = _V_LINE_STRIP_GLZ;
			pPipe->pfnEnd = NULL; 
						
			SetClipFlags(pPipe->pClipper, GP_I_COLORS);

			break;
		}
		break;

	default:
		assert(FALSE);
		break;
	}
}

int GetRenderingPath()
{
	if(pPipe->nShadingModel == GP_FLAT)
	{
		if(pPipe->nRenderModel == GP_WIRE)
		{
			if(pPipe->bDepth)
			{
				if(pPipe->bLighting)
				{
					return _GP_WIRE_FLAT_LZ;
				}
				else
				{
					return  _GP_WIRE_FLAT_Z;
				}
			}
			else
			{
				if(pPipe->bLighting)
				{
					return _GP_WIRE_FLAT_L;
				}
				else
				{
					return _GP_WIRE_FLAT;
				}
			}
		}
		else
		{
			if(pPipe->bDepth)
			{
				if(pPipe->bLighting)
				{
					return _GP_FILLED_FLAT_LZ;
				}
				else
				{
					return _GP_FILLED_FLAT_Z;
				}
			}
			else
			{
				if(pPipe->bLighting)
				{
					return _GP_FILLED_FLAT_L;
				}
				else
				{
					return _GP_FILLED_FLAT;
				}
			}
		}
	}
	else
	{
		if(pPipe->nRenderModel == GP_WIRE)
		{
			if(pPipe->bDepth)
			{
				if(pPipe->bLighting)
				{
					return _GP_WIRE_GOURAUD_LZ;
				}
				else
				{
					return _GP_WIRE_GOURAUD_Z;
				}
			}
			else
			{
				if(pPipe->bLighting)
				{
					return _GP_WIRE_GOURAUD_L;
				}
				else
				{
					return _GP_WIRE_GOURAUD;
				}
			}
		}
		else
		{
			if(pPipe->bDepth)
			{
				if(pPipe->bLighting)
				{
					return _GP_FILLED_GOURAUD_LZ;
				}
				else
				{
					return _GP_FILLED_GOURAUD_Z;
				}
			}
			else
			{
				if(pPipe->bLighting)
				{
					return _GP_FILLED_GOURAUD_L;
				}
				else
				{
					return _GP_FILLED_GOURAUD;
				}
			}
		}
	}
}

/* single quads */
void _V_QUADS_FF(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;	
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	if ( nVIndex == 0 )
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);
		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if ( nVIndex == 3 )
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FFZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);
		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FFL(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		/* this is the only sensitive material that needs to 
		be copied right away. The actual lighting calculation 
		can be deferred until we actually render the quad */

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));
		
		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if(nVIndex == 3)
	{
		/* transform the vertices and normal */

		/* light the first vertex */
		/* the viewpoint doesn't change in between vertex 
		specifications, so we didn't have to make use of it 
		as soon as the first vertex was specified. */

		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[1]);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[2]);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[3]);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FFLZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if ( nVIndex == 3 )
	{
		/* transform all four vertices */
		/* light the first vertex */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[1]);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[2]);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[3]);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FG(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
	
		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		/* colors at each vertex are automatically assigned their 
		proper place in the new vertex list. */

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FGL(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		
		GetViewpoint(pPipe->pView,adfViewpoint);
		
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Eye(pPipe->pView,pVertices[3]);
		Normal2Eye(pPipe->pView,pVertices[3]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[3]);
		Eye2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FGZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_FGLZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		
		GetViewpoint(pPipe->pView,adfViewpoint);
		
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Eye(pPipe->pView,pVertices[3]);
		Normal2Eye(pPipe->pView,pVertices[3]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[3]);
		Eye2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WF(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;	
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	if ( nVIndex == 0 )
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if ( nVIndex == 3 )
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WFL(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		/* this is the only sensitive data that needs to 
		be copied right away (it can change between vertex 
		specifictations. The actual lighting calculation 
		can be deferred until we actually render the quad (since
		the viewpoint does not change between vertex specs)*/

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if(nVIndex == 3)
	{
		/* transform the vertices and normal */

		/* light the first vertex */

		/* the viewpoint doesn't change in between vertex 
		specifications, so we didn't have to make use of it 
		as soon as the first vertex was specified. */

		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[1]);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[2]);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[3]);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WFZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WFLZ(double dfX,double dfY, double dfZ)
{

	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		/* this is the only sensitive data that needs to 
		be copied right away (it can change between vertex 
		specifictations. The actual lighting calculation 
		can be deferred until we actually render the quad (since
		the viewpoint does not change between vertex specs)*/

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished quad to the screen if necessary */
	else if(nVIndex == 3)
	{
		/* transform the vertices and normal */

		/* light the first vertex */

		/* the viewpoint doesn't change in between vertex 
		specifications, so we didn't have to make use of it 
		as soon as the first vertex was specified. */

		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[1]);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[2]);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
		Normal2Eye(pPipe->pView,pVertices[3]);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WG(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);
	
		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		/* colors at each vertex are automatically assigned their 
		proper place in the new vertex list. */

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WGL(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		
		GetViewpoint(pPipe->pView,adfViewpoint);
		
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Eye(pPipe->pView,pVertices[3]);
		Normal2Eye(pPipe->pView,pVertices[3]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[3]);
		Eye2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WGZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_QUADS_WGLZ(double dfX,double dfY, double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished quad to the screen if necessary */
	if(nVIndex == 3)
	{
		/* transform all four vertices */
		
		GetViewpoint(pPipe->pView,adfViewpoint);
		
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		World2Eye(pPipe->pView,pVertices[3]);
		Normal2Eye(pPipe->pView,pVertices[3]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[3]);
		Eye2Raster(pPipe->pView,pVertices[3]);
		Homogenize(pVertices[3]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dQuad(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],pVertices[3],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

/* single triangles */
void _V_TRI_FF(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;	
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	if ( nVIndex == 0 )
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if ( nVIndex == 2 )
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FFL(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if(nVIndex == 2)
	{
		/* transform the vertices and normal */
		/* light the first vertex */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FFZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if(nVIndex == 2)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FFLZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if ( nVIndex == 2 )
	{
		/* transform all three vertices */
		/* light the first vertex */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
		
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],
			pVertices[1],pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FG(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
	
		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FGL(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FGZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all three vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_FGLZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],
			pVertices[1],pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WF(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;	
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	if ( nVIndex == 0 )
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if ( nVIndex == 2 )
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WFL(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if(nVIndex == 2)
	{
		/* transform the vertices and normal */
		/* light the first vertex */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WFZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);
		
		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if(nVIndex == 2)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip quad */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WFLZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	if ( nVIndex == 0 )
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	/* flush the finished triangle to the screen if necessary */
	else if ( nVIndex == 2 )
	{
		/* transform all three vertices */
		/* light the first vertex */
		GetViewpoint(pPipe->pView,adfViewpoint);
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
		
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],
			pVertices[1],pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WG(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);
	
		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WGL(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	
	
	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));
	
	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);
		
		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
			}

			/* rasterize new polygon */
			R_PolygonWireGouraud(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WGZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all three vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],pVertices[1],
			pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_TRI_WGLZ(double dfX,double dfY,double dfZ)
{
	GPVPList sVList;
	p_GPVertex *pArray;
	int i,nCount;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));

	/* flush the finished triangle to the screen if necessary */
	if(nVIndex == 2)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		World2Eye(pPipe->pView,pVertices[2]);
		Normal2Eye(pPipe->pView,pVertices[2]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[2]);
		Eye2Raster(pPipe->pView,pVertices[2]);
		Homogenize(pVertices[2]->adfTransform);

		/* clip triangle */
		InitializeGPVPList(&sVList);
		Clip3dTriangle(pPipe->pClipper,pVertices[0],
			pVertices[1],pVertices[2],&sVList);

		if ( sVList.nCount > 0 )
		{
			nCount = sVList.nCount;
			pArray = sVList.pArray;

			for(i = 0; i < nCount; i++)
			{
				pArray[i]->nX = (gpInt32)pArray[i]->adfTransform[0];
				pArray[i]->nY = (gpInt32)pArray[i]->adfTransform[1];
				pArray[i]->nZ = (gpInt32)(2147483647.0 * pArray[i]->adfTransform[2]);
			}

			/* rasterize new polygon */
			R_PolygonWireGouraudZ(pPipe->pRaster,&sVList);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_F(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	else if(nVIndex == 1)
	{
		/* transform vertices */

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
		
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_FL(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;

	if(nVIndex == 0)
	{
		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	else if(nVIndex == 1)
	{
		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}
		
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_FZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;

	if(nVIndex == 0)
	{
		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		nVIndex++;
	}
	else if(nVIndex == 1)
	{
		/* transform vertices */

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_FLZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;

	if(nVIndex == 0)
	{
		memcpy(pVertices[nVIndex]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[nVIndex]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		nVIndex++;
	}
	else if (nVIndex == 1)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);
	
		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}

}

void _V_LINES_G(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	if(nVIndex == 1)
	{
		/* transform all three vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);
	
		if(bAccept)
		{
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
	
			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineGouraud(pPipe->pRaster,&sV0,&sV1);
		}
	
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_GL(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));

	if(nVIndex == 1)
	{
		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	
		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);
	
		if(bAccept)
		{
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
	
			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineGouraud(pPipe->pRaster,&sV0,&sV1);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_GZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	pVertices[nVIndex]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
	pVertices[nVIndex]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
	pVertices[nVIndex]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

	if(nVIndex == 1)
	{
		/* transform all four vertices */
		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineGouraudZ(pPipe->pRaster,&sV0,&sV1);
		}
	
		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINES_GLZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	pVertices[nVIndex]->adfTransform[0] = dfX;
	pVertices[nVIndex]->adfTransform[1] = dfY;
	pVertices[nVIndex]->adfTransform[2] = dfZ;
	pVertices[nVIndex]->adfTransform[3] = 1.0;	

	memcpy(pVertices[nVIndex]->adfNormTrans, 
		pPipe->adfCurrentNorm, sizeof(double) * 4);

	memcpy(&pVertices[nVIndex]->sMaterial,
		&pPipe->sCurrentMat,sizeof(GPMaterial));

	if(nVIndex == 1)
	{
		/* transform all four vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineGouraudZ(pPipe->pRaster,&sV0,&sV1);
		}

		nVIndex = 0;
	}
	else
	{
		nVIndex++;
	}
}

void _V_LINE_STRIP_F(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		pVertices[1]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[1]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[1]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_FL(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		memcpy(pVertices[1]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[1]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineFlat(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_FZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		pVertices[1]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[1]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[1]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		/* clip line */
		ClipLine3d(pPipe->pClipper,pVertices[0],pVertices[1],&sV0,&sV1,&bAccept);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}
		
		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_FLZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		memcpy(pVertices[1]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[1]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineFlatZ(pPipe->pRaster,
				pVertices[0]->byR,
				pVertices[0]->byG,
				pVertices[0]->byB,
				&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_G(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		pVertices[1]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[1]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[1]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];

			R_LineGouraud(pPipe->pRaster,&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_GL(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		memcpy(pVertices[1]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[1]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
	
			R_LineGouraud(pPipe->pRaster,&sV0,&sV1);
		}
		
		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_GZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		pVertices[0]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[0]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[0]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		pVertices[1]->byR = (gpByte)(255.0*pPipe->sCurrentMat.dfDR);
		pVertices[1]->byG = (gpByte)(255.0*pPipe->sCurrentMat.dfDG);
		pVertices[1]->byB = (gpByte)(255.0*pPipe->sCurrentMat.dfDB);

		World2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineGouraudZ(pPipe->pRaster,&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}

void _V_LINE_STRIP_GLZ(double dfX,double dfY,double dfZ)
{
	GPVertex sV0,sV1;
	gpBool bAccept;
	double adfViewpoint[4];

	if(nVIndex == 0)
	{
		pVertices[0]->adfTransform[0] = dfX;
		pVertices[0]->adfTransform[1] = dfY;
		pVertices[0]->adfTransform[2] = dfZ;
		pVertices[0]->adfTransform[3] = 1.0;

		memcpy(pVertices[0]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[0]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		/* transform vertices */
		GetViewpoint(pPipe->pView,adfViewpoint);

		World2Eye(pPipe->pView,pVertices[0]);
		Normal2Eye(pPipe->pView,pVertices[0]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[0]);
		Eye2Raster(pPipe->pView,pVertices[0]);
		Homogenize(pVertices[0]->adfTransform);
	}
	else
	{
		pVertices[1]->adfTransform[0] = dfX;
		pVertices[1]->adfTransform[1] = dfY;
		pVertices[1]->adfTransform[2] = dfZ;
		pVertices[1]->adfTransform[3] = 1.0;

		memcpy(pVertices[1]->adfNormTrans, 
			pPipe->adfCurrentNorm, sizeof(double) * 4);

		memcpy(&pVertices[1]->sMaterial,
			&pPipe->sCurrentMat,sizeof(GPMaterial));

		World2Eye(pPipe->pView,pVertices[1]);
		Normal2Eye(pPipe->pView,pVertices[1]);
		LightVertex(pPipe->pLighting,adfViewpoint,pVertices[1]);
		Eye2Raster(pPipe->pView,pVertices[1]);
		Homogenize(pVertices[1]->adfTransform);

		if(bAccept)
		{			
			sV0.nX = (gpInt32)sV0.adfTransform[0];
			sV0.nY = (gpInt32)sV0.adfTransform[1];
			sV0.nZ = (gpInt32)(2147483647.0 * sV0.adfTransform[2]);

			sV1.nX = (gpInt32)sV1.adfTransform[0];
			sV1.nY = (gpInt32)sV1.adfTransform[1];
			sV1.nZ = (gpInt32)(2147483647.0 * sV1.adfTransform[2]);

			R_LineGouraudZ(pPipe->pRaster,&sV0,&sV1);
		}

		/* move vertices up the line */
		memcpy(pVertices[0],pVertices[1],sizeof(p_GPVertex));
	}
	
	nVIndex++;
}
