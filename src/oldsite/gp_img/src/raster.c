#include "raster.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

/* the rasterizer manages the framebuffer.  The frame buffer can 
consist of a few buffers, like the image buffer (32-bit) and the 
z-buffer (32-bit).  Maybe more later, but that's all for now. */

/* creates and initializes a rasterizer and buffers */
p_GPRaster CreateGPRasterizer(gpInt32 nX, gpInt32 nY)
{
	p_GPRaster pRaster = malloc(sizeof(GPRaster));
	pRaster->pRBuffer = malloc(nX * nY * sizeof(gpByte));
	pRaster->pGBuffer = malloc(nX * nY * sizeof(gpByte));
	pRaster->pBBuffer = malloc(nX * nY * sizeof(gpByte));
	pRaster->pZBuffer = malloc(nX * nY * sizeof(gpInt32));

	pRaster->nXSize = nX;
	pRaster->nYSize = nY;

	return pRaster;
}

void ClearAllBuffers(p_GPRaster pRaster)
{
	gpInt32 nArea = pRaster->nXSize*pRaster->nYSize;

	memset(pRaster->pRBuffer,0,nArea * sizeof(gpByte));
	memset(pRaster->pGBuffer,0,nArea * sizeof(gpByte));
	memset(pRaster->pBBuffer,0,nArea * sizeof(gpByte));
	memset(pRaster->pZBuffer,0,nArea * sizeof(gpInt32));
}

#define XMINVERTEX(a,b) (((a)->nX<(b)->nX)?(a):(b))
#define XMAXVERTEX(a,b) (((a)->nX>(b)->nX)?(a):(b))
#define YMINVERTEX(a,b) (((a)->nY<(b)->nY)?(a):(b))
#define YMAXVERTEX(a,b) (((a)->nY>(b)->nY)?(a):(b))

/************** QUADS **********/

/* flat shaded, non-z-buffered quad */
void R_QuadFlat(p_GPRaster pRaster, 
			  gpByte byR, gpByte byG, gpByte byB,
			  p_GPVertex pV0,p_GPVertex pV1,
			  p_GPVertex pV2,p_GPVertex pV3)
{
	R_TriFlat(pRaster,pV0,pV1,pV2,byR,byG,byB);
	R_TriFlat(pRaster,pV0,pV2,pV3,byR,byG,byB);
}

/* flat shaded, z-buffered quad */
void R_QuadFlatZ(p_GPRaster pRaster, 
			   gpByte byR, gpByte byG, gpByte byB,
			   p_GPVertex pV0,p_GPVertex pV1,
			   p_GPVertex pV2,p_GPVertex pV3)
{
	R_TriFlatZ(pRaster,pV0,pV1,pV2,byR,byG,byB);
	R_TriFlatZ(pRaster,pV0,pV2,pV3,byR,byG,byB);
}

/* gouraud shaded, z-buffered quad */
void R_QuadGouraud(p_GPRaster pRaster, 
				 p_GPVertex pV0,p_GPVertex pV1,
				 p_GPVertex pV2,p_GPVertex pV3)
{
	R_TriGouraud(pRaster,pV0,pV1,pV2);
	R_TriGouraud(pRaster,pV0,pV2,pV3);
}

/* gouraud shaded, non-z-buffered quad */
void R_QuadGouraudZ(p_GPRaster pRaster, 
				  p_GPVertex pV0,p_GPVertex pV1,
				  p_GPVertex pV2,p_GPVertex pV3)
{
	R_TriGouraudZ(pRaster,pV0,pV1,pV2);
	R_TriGouraudZ(pRaster,pV0,pV2,pV3);
}

/* wire, flat non-z-buffered quad */
void R_QuadWireFlat(p_GPRaster pRaster,
				  gpByte byR, gpByte byG, gpByte byB,
				  p_GPVertex pV0,p_GPVertex pV1,
				  p_GPVertex pV2,p_GPVertex pV3)
{
	R_LineFlat(pRaster, byR, byG, byB, pV0, pV1);
	R_LineFlat(pRaster, byR, byG, byB, pV1, pV2);
	R_LineFlat(pRaster, byR, byG, byB, pV2, pV3);
	R_LineFlat(pRaster, byR, byG, byB, pV3, pV0);
}

/* wire, flat, z-buffered quad */
void R_QuadWireFlatZ(p_GPRaster pRaster, 
				   gpByte byR, gpByte byG, gpByte byB,
			       p_GPVertex pV0,p_GPVertex pV1,
			       p_GPVertex pV2,p_GPVertex pV3)
{
	R_LineFlatZ(pRaster, byR, byG, byB, pV0, pV1);
	R_LineFlatZ(pRaster, byR, byG, byB, pV1, pV2);
	R_LineFlatZ(pRaster, byR, byG, byB, pV2, pV3);
	R_LineFlatZ(pRaster, byR, byG, byB, pV3, pV0);
}

/* wire, gouraud, z-buffered quad */
void R_QuadWireGouraud(p_GPRaster pRaster, 
				 p_GPVertex pV0,p_GPVertex pV1,
				 p_GPVertex pV2,p_GPVertex pV3)
{
	R_LineGouraud(pRaster, pV0, pV1);
	R_LineGouraud(pRaster, pV1, pV2);
	R_LineGouraud(pRaster, pV2, pV3);
	R_LineGouraud(pRaster, pV3, pV0);
}

/* wire gouraud, non-z-buffered quad */
void R_QuadWireGouraudZ(p_GPRaster pRaster, 
				  p_GPVertex pV0,p_GPVertex pV1,
				  p_GPVertex pV2,p_GPVertex pV3)
{
	R_LineGouraudZ(pRaster, pV0, pV1);
	R_LineGouraudZ(pRaster, pV1, pV2);
	R_LineGouraudZ(pRaster, pV2, pV3);
	R_LineGouraudZ(pRaster, pV3, pV0);
}

/************** TRIANGLES ************/

void R_TriFlat(p_GPRaster pRaster,
                  p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2,
				  gpByte byR,gpByte byG, gpByte byB)
{
    /* this seems to be alot of variables.  Do I need all of these?
	I certainly use them all, but can I eliminate some?*/
    p_GPVertex pVMin,pVMid,pVMax;
    gpInt32 nDY0,nDY1,nDX0,nDX1;
    gpInt32 nXInc0, nXInc1;
    gpInt32 nEXInc0,nEXInc1,nRemInc0,nRemInc1;
    gpInt32 i,nXC0,nXC1,nStart;
    gpByte *pbyR0, *pbyG0, *pbyB0;
    gpByte *pbyR1, *pbyG1, *pbyB1;
    
	/******* DEGENERATE CASES *********/

	/* check for vertical co-linearity */
	if (pV0->nX == pV1->nX && pV1->nX == pV2->nX)
	{
		pVMin = YMINVERTEX(YMINVERTEX(pV0,pV1),pV2);
		pVMax = YMAXVERTEX(YMAXVERTEX(pV0,pV1),pV2);

		R_LineFlat(pRaster,byR,byG,byB,pVMin,pVMax);
		return;
	}
	/* check for horizontal co-linearity */
	else if (pV0->nY == pV1->nY && pV1->nY == pV2->nY)
	{
		pVMin = XMINVERTEX(XMINVERTEX(pV0,pV1),pV2);
		pVMax = XMAXVERTEX(XMAXVERTEX(pV0,pV1),pV2);

		R_LineFlat(pRaster,byR,byG,byB,pVMin,pVMax);
		return;
	}

	/******* NORMAL TRIANGLES *********/

    /* sort the vertices from lowest y (highest point) 
    to highest y (lowest point) */
    if ( pV0->nY >= pV1->nY && pV0->nY >= pV2->nY )
    {
        pVMax = pV0;
        if ( pV1->nY > pV2->nY )
        {
            pVMid = pV1;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV1;
        }
    }
    else if ( pV1->nY >= pV0->nY && pV1->nY >= pV2->nY )
    {
        pVMax = pV1;
        if ( pV0->nY > pV2->nY )
        {
            pVMid = pV0;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV0;
        }
    }
    else
    {
        pVMax = pV2;
        if ( pV0->nY > pV1->nY )
        {
            pVMid = pV0;
            pVMin = pV1;
        }
        else
        {
            pVMid = pV1;
            pVMin = pV0;
        }
    }
    
	/* initialize buffer pointers */
    nStart = pVMin->nY * pRaster->nXSize + pVMin->nX; 
    pbyR0 = pbyR1 = pRaster->pRBuffer + nStart;
    pbyG0 = pbyG1 = pRaster->pGBuffer + nStart;
    pbyB0 = pbyB1 = pRaster->pBBuffer + nStart;

	 /* errors are nothing at first */
    nEXInc0 = nEXInc1 = 0;

	/* calculate relavent variables for the edge that goes from 
	the minimum (highest) vertex to the maximum (lowest) vertex */
	
    nDX0 = pVMax->nX - pVMin->nX;
    nDY0 = pVMax->nY - pVMin->nY;
    nXInc0 = ( nDX0 / nDY0 ) + pRaster->nXSize;
	
	if ( nDX0 >= 0 )
	{
		nXC0 = 1;
	}
	else
	{
		nXC0 = -1;
		nDX0 = -nDX0;
	}

	nRemInc0 = nDX0 % nDY0;
   
	/* deltas for the first active side, besides the min/max 
	edge, which is always active */
    nDY1 = pVMid->nY - pVMin->nY;
	nDX1 = pVMid->nX - pVMin->nX;

	/* upper half of the triangle */
	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0 )
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;
		
		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				memset(pbyR0, byR, pbyR1-pbyR0+1);
				memset(pbyG0, byG, pbyG1-pbyG0+1);
				memset(pbyB0, byB, pbyB1-pbyB0+1);
			}
			else
			{
				memset(pbyR1, byR, pbyR0-pbyR1+1);
				memset(pbyG1, byG, pbyG0-pbyG1+1);
				memset(pbyB1, byB, pbyB0-pbyB1+1);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
        
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
        
			if ( nEXInc0 >= nDY0 )
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
			}
        
			if ( nEXInc1 >= nDY1 )
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
			}
		}
	}
	else
	{
		pbyR1 += nDX1;
		pbyG1 += nDX1;
		pbyB1 += nDX1;
	}
    
/* lower half of the triangle */
    
    nDX1 = pVMax->nX - pVMid->nX;
    nDY1 = pVMax->nY - pVMid->nY;

	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0)
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;
		
		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				memset(pbyR0, byR, pbyR1-pbyR0+1);
				memset(pbyG0, byG, pbyG1-pbyG0+1);
				memset(pbyB0, byB, pbyB1-pbyB0+1);
			}
			else
			{
				memset(pbyR1, byR, pbyR0-pbyR1+1);
				memset(pbyG1, byG, pbyG0-pbyG1+1);
				memset(pbyB1, byB, pbyB0-pbyB1+1);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
        
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
        
			if (nEXInc0 >= nDY0)
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
			}
        
			if (nEXInc1 >= nDY1)
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
			}
		}
	}
	else if ( pbyR0 < pbyR1 )
	{
		memset(pbyR0, byR, pbyR1-pbyR0+1);
		memset(pbyG0, byG, pbyG1-pbyG0+1);
		memset(pbyB0, byB, pbyB1-pbyB0+1);
	}
	else
	{
		memset(pbyR1, byR, pbyR0-pbyR1+1);
		memset(pbyG1, byG, pbyG0-pbyG1+1);
		memset(pbyB1, byB, pbyB0-pbyB1+1);
	}
}

void R_TriFlatZ(p_GPRaster pRaster,
                   p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2,
				   gpByte byR,gpByte byG, gpByte byB)
{
	/* this seems to be alot of variables.  Do I need all of these? */
    p_GPVertex pVMin,pVMid,pVMax;
    gpInt32 nDY0,nDY1,nDX0,nDX1;
    gpInt32 nXInc0, nXInc1;
    gpInt32 nEXInc0,nEXInc1,nRemInc0,nRemInc1;
    gpInt32 i,nXC0,nXC1,nStart;
    gpByte *pbyR0, *pbyG0, *pbyB0;
    gpByte *pbyR1, *pbyG1, *pbyB1;
    
	gpInt32 nZInc0,nZInc1,nDZ0,nDZ1,nZC0,nZC1;
	gpInt32 nEZInc0,nEZInc1,nRemZInc0,nRemZInc1;
	gpInt32 *pnZ0,*pnZ1, nZCur0, nZCur1;

	/******* DEGENERATE CASES *********/

	/* check for vertical co-linearity */
	if (pV0->nX == pV1->nX && pV1->nX == pV2->nX)
	{
		pVMin = YMINVERTEX(YMINVERTEX(pV0,pV1),pV2);
		pVMax = YMAXVERTEX(YMAXVERTEX(pV0,pV1),pV2);

		R_LineFlatZ(pRaster,byR,byG,byB,pVMin,pVMax);
		return;
	}
	/* check for horizontal co-linearity */
	else if (pV0->nY == pV1->nY && pV1->nY == pV2->nY)
	{
		pVMin = XMINVERTEX(XMINVERTEX(pV0,pV1),pV2);
		pVMax = XMAXVERTEX(XMAXVERTEX(pV0,pV1),pV2);

		R_LineFlatZ(pRaster,byR,byG,byB,pVMin,pVMax);
		return;
	}

	/******* NORMAL TRIANGLES *********/

    /* sort the vertices from lowest y (highest point) 
    to highest y (lowest point) */
    if ( pV0->nY >= pV1->nY && pV0->nY >= pV2->nY )
    {
        pVMax = pV0;
        if ( pV1->nY > pV2->nY )
        {
            pVMid = pV1;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV1;
        }
    }
    else if ( pV1->nY >= pV0->nY && pV1->nY >= pV2->nY )
    {
        pVMax = pV1;
        if ( pV0->nY > pV2->nY )
        {
            pVMid = pV0;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV0;
        }
    }
    else
    {
        pVMax = pV2;
        if ( pV0->nY > pV1->nY )
        {
            pVMid = pV0;
            pVMin = pV1;
        }
        else
        {
            pVMid = pV1;
            pVMin = pV0;
        }
    }
    
	/* initialize buffer pointers */
    nStart = pVMin->nY * pRaster->nXSize + pVMin->nX; 
    pbyR0 = pbyR1 = pRaster->pRBuffer + nStart;
    pbyG0 = pbyG1 = pRaster->pGBuffer + nStart;
    pbyB0 = pbyB1 = pRaster->pBBuffer + nStart;
	pnZ0 = pnZ1 = pRaster->pZBuffer + nStart;

	 /* errors are nothing at first */
    nEXInc0 = nEXInc1 = nEZInc0 = nEZInc1 = 0;

	/* calculate relavent variables for the edge that goes from 
	the minimum (highest) vertex to the maximum (lowest) vertex */
	
    nDX0 = pVMax->nX - pVMin->nX;
    nDY0 = pVMax->nY - pVMin->nY;
    nXInc0 = ( nDX0 / nDY0 ) + pRaster->nXSize;

	if ( nDX0 >= 0 )
	{
		nXC0 = 1;
	}
	else
	{
		nXC0 = -1;
		nDX0 = -nDX0;
	}

	nRemInc0 = nDX0 % nDY0;
   
	/* setup stuff so we can do z-buffering */
	nDZ0 = pVMax->nZ - pVMin->nZ;

	nZInc0 = nDZ0 / nDY0;
	if ( nDZ0 >= 0 )
	{
		nZC0 = 1;
	}
	else
	{
		nZC0 = -1;
		nDZ0 = -nDZ0;
	}
	nRemZInc0 = nDZ0 % nDY0;

	/* deltas for the first active side, besides the min/max 
	edge, which is always active */
    nDY1 = pVMid->nY - pVMin->nY;
	nDX1 = pVMid->nX - pVMin->nX;
	nDZ1 = pVMid->nZ - pVMin->nZ;

	/* initialize current z-value */
	nZCur0 = nZCur1 = pVMin->nZ;

	/* upper half of the triangle */
	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0 )
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;

		/* stuff for the z-buffer*/
		nZInc1 = nDZ1 / nDY1;
		if ( nDZ1 >= 0 )
		{
			nZC1 = 1;
		}
		else
		{
			nZC1 = -1;
			nDZ1 = -nDZ1;
		}
		nRemZInc1 = nDZ1 % nDY1;
		
		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				ScanLineZ(
					pbyR0,pbyG0,pbyB0,pnZ0,
					pbyR1,pbyG1,pbyB1,pnZ1,
					nZCur0, nZCur1,
					byR,byG,byB);
			}
			else
			{
				ScanLineZ(
					pbyR1,pbyG1,pbyB1,pnZ1,
					pbyR0,pbyG0,pbyB0,pnZ0,
					nZCur1, nZCur0,
					byR,byG,byB);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
			pnZ0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
       		pnZ1 += nXInc1;

			nZCur0 += nZInc0;
			nZCur1 += nZInc1;

			/* increment error accumulation */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
			
			nEZInc0 += nRemZInc0;
			nEZInc1 += nRemZInc1;
        
			/* make error corrections if necessary */
			if ( nEXInc0 >= nDY0 )
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
				pnZ0 += nXC0;
			}
        
			if ( nEXInc1 >= nDY1 )
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
				pnZ1 += nXC1;
			}

			if ( nEZInc0 >= nDY0 )
			{
				nEZInc0 -= nDY0;
				nZCur0 += nZC0;
			}

			if ( nEZInc1 >= nDY1 )
			{
				nEZInc1 -= nDY1;
				nZCur1 += nZC1;
			}
		}
	}
	else
	{
		pbyR1 += nDX1;
		pbyG1 += nDX1;
		pbyB1 += nDX1;
		pnZ1 += nDX1;

		nZCur1 = pVMid->nZ;
	}
    
/* lower half of the triangle */
    
    nDX1 = pVMax->nX - pVMid->nX;
    nDY1 = pVMax->nY - pVMid->nY;
	nDZ1 = pVMax->nZ - pVMid->nZ;

	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0)
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;
	
		/* stuff for the z-buffer*/
		nZInc1 = nDZ1 / nDY1;
		if ( nDZ1 >= 0 )
		{
			nZC1 = 1;
		}
		else
		{
			nZC1 = -1;
			nDZ1 = -nDZ1;
		}
		nRemZInc1 = nDZ1 % nDY1;

		for ( i = 0; i < nDY1; i++ )
		{
			
			if ( pbyR0 < pbyR1 )
			{
				ScanLineZ(
					pbyR0,pbyG0,pbyB0,pnZ0,
					pbyR1,pbyG1,pbyB1,pnZ1,
					nZCur0, nZCur1,
					byR,byG,byB);
			}
			else
			{
				ScanLineZ(
					pbyR1,pbyG1,pbyB1,pnZ1,
					pbyR0,pbyG0,pbyB0,pnZ0,
					nZCur1, nZCur0,
					byR,byG,byB);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
			pnZ0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
			pnZ1 += nXInc1;
        
			nZCur0 += nZInc0;
			nZCur1 += nZInc1;

			/* accumulate errors */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;

			nEZInc0 += nRemZInc0;
			nEZInc1 += nRemZInc1;
        
			/* purge errors if necessary */
			if (nEXInc0 >= nDY0)
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
				pnZ0 += nXC0;
			}
        
			if (nEXInc1 >= nDY1)
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
				pnZ1 += nXC1;
			}

			if ( nEZInc0 >= nDY0 )
			{
				nEZInc0 -= nDY0;
				nZCur0 += nZC0;
			}

			if ( nEZInc1 >= nDY1 )
			{
				nEZInc1 -= nDY1;
				nZCur1 += nZC1;
			}
		}
	}
	else if ( pbyR0 < pbyR1 )
	{
		ScanLineZ(
			pbyR0,pbyG0,pbyB0,pnZ0,
			pbyR1,pbyG1,pbyB1,pnZ1,
			pVMax->nZ, pVMid->nZ,
			byR,byG,byB);
	}
	else
	{
		ScanLineZ(
			pbyR1,pbyG1,pbyB0,pnZ1,
			pbyR0,pbyG0,pbyB1,pnZ0,
			pVMid->nZ, pVMax->nZ,
			byR,byG,byB);
	}
}

void R_TriGouraud(p_GPRaster pRaster,
                     p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	/* this seems to be alot of variables.  Do I need all of these? */
    p_GPVertex pVMin,pVMid,pVMax;
    gpInt32 nDY0,nDY1,nDX0,nDX1;
    gpInt32 nXInc0, nXInc1;
    gpInt32 nEXInc0,nEXInc1,nRemInc0,nRemInc1;
    gpInt32 i,nXC0,nXC1,nStart;
    gpByte *pbyR0, *pbyG0, *pbyB0;
    gpByte *pbyR1, *pbyG1, *pbyB1;
    
	gpInt32 nRInc0,nRInc1,nDR0,nDR1,nRC0,nRC1;
	gpInt32 nERInc0,nERInc1,nRemRInc0,nRemRInc1;
	gpInt32 nRCur0, nRCur1;

	gpInt32 nGInc0,nGInc1,nDG0,nDG1,nGC0,nGC1;
	gpInt32 nEGInc0,nEGInc1,nRemGInc0,nRemGInc1;
	gpInt32 nGCur0, nGCur1;

	gpInt32 nBInc0,nBInc1,nDB0,nDB1,nBC0,nBC1;
	gpInt32 nEBInc0,nEBInc1,nRemBInc0,nRemBInc1;
	gpInt32 nBCur0, nBCur1;

	/******* DEGENERATE CASES *********/

	/* check for vertical co-linearity */
	if (pV0->nX == pV1->nX && pV1->nX == pV2->nX)
	{
		pVMin = YMINVERTEX(YMINVERTEX(pV0,pV1),pV2);
		pVMax = YMAXVERTEX(YMAXVERTEX(pV0,pV1),pV2);

		R_LineGouraud(pRaster,pVMin,pVMax);
		return;
	}
	/* check for horizontal co-linearity */
	else if (pV0->nY == pV1->nY && pV1->nY == pV2->nY)
	{
		pVMin = XMINVERTEX(XMINVERTEX(pV0,pV1),pV2);
		pVMax = XMAXVERTEX(XMAXVERTEX(pV0,pV1),pV2);

		R_LineGouraud(pRaster,pVMin,pVMax);
		return;
	}

	/******* NORMAL TRIANGLES *********/

    /* sort the vertices from lowest y (highest point) 
    to highest y (lowest point) */
    if ( pV0->nY >= pV1->nY && pV0->nY >= pV2->nY )
    {
        pVMax = pV0;
        if ( pV1->nY > pV2->nY )
        {
            pVMid = pV1;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV1;
        }
    }
    else if ( pV1->nY >= pV0->nY && pV1->nY >= pV2->nY )
    {
        pVMax = pV1;
        if ( pV0->nY > pV2->nY )
        {
            pVMid = pV0;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV0;
        }
    }
    else
    {
        pVMax = pV2;
        if ( pV0->nY > pV1->nY )
        {
            pVMid = pV0;
            pVMin = pV1;
        }
        else
        {
            pVMid = pV1;
            pVMin = pV0;
        }
    }
    
	/* initialize buffer pointers */
    nStart = pVMin->nY * pRaster->nXSize + pVMin->nX; 
    pbyR0 = pbyR1 = pRaster->pRBuffer + nStart;
    pbyG0 = pbyG1 = pRaster->pGBuffer + nStart;
    pbyB0 = pbyB1 = pRaster->pBBuffer + nStart;

	 /* errors are nothing at first */
    nEXInc0 = nEXInc1 = 0;
	nERInc0 = nERInc1 = 0;
	nEGInc0 = nEGInc1 = 0;
	nEBInc0 = nEBInc1 = 0;

	/* calculate relavent variables for the edge that goes from 
	the minimum (highest) vertex to the maximum (lowest) vertex */
	
    nDX0 = pVMax->nX - pVMin->nX;
    nDY0 = pVMax->nY - pVMin->nY;
    nXInc0 = ( nDX0 / nDY0 ) + pRaster->nXSize;
	if ( nDX0 >= 0 )
	{
		nXC0 = 1;
	}
	else
	{
		nXC0 = -1;
		nDX0 = -nDX0;
	}
	nRemInc0 = nDX0 % nDY0;
   
	/* setup stuff so we can do Gouraud shading */
	nDR0 = pVMax->byR - pVMin->byR;
	nRInc0 = nDR0 / nDY0;
	if ( nDR0 >= 0 )
	{
		nRC0 = 1;
	}
	else
	{
		nRC0 = -1;
		nDR0 = -nDR0;
	}
	nRemRInc0 = nDR0 % nDY0;

	nDG0 = pVMax->byG - pVMin->byG;
	nGInc0 = nDG0 / nDY0;
	if ( nDG0 >= 0 )
	{
		nGC0 = 1;
	}
	else
	{
		nGC0 = -1;
		nDG0 = -nDG0;
	}
	nRemGInc0 = nDG0 % nDY0;

	nDB0 = pVMax->byB - pVMin->byB;
	nBInc0 = nDB0 / nDY0;
	if ( nDB0 >= 0 )
	{
		nBC0 = 1;
	}
	else
	{
		nBC0 = -1;
		nDB0 = -nDB0;
	}
	nRemBInc0 = nDB0 % nDY0;

	/* deltas for the first active side, besides the min/max 
	edge, which is always active */
    nDY1 = pVMid->nY - pVMin->nY;
	nDX1 = pVMid->nX - pVMin->nX;
	nDR1 = pVMid->byR - pVMin->byR;
	nDG1 = pVMid->byG - pVMin->byG;
	nDB1 = pVMid->byB - pVMin->byB;

	/* initialize current colors */
	nRCur0 = nRCur1 = pVMin->byR;
	nGCur0 = nGCur1 = pVMin->byG;
	nBCur0 = nBCur1 = pVMin->byB;

	/* upper half of the triangle */
	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0 )
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;

		/* stuff for the gouraud shading*/
		nRInc1 = nDR1 / nDY1;
		if ( nDR1 >= 0 )
		{
			nRC1 = 1;
		}
		else
		{
			nRC1 = -1;
			nDR1 = -nDR1;
		}
		nRemRInc1 = nDR1 % nDY1;

		nGInc1 = nDG1 / nDY1;
		if ( nDG1 >= 0 )
		{
			nGC1 = 1;
		}
		else
		{
			nGC1 = -1;
			nDG1 = -nDG1;
		}
		nRemGInc1 = nDG1 % nDY1;

		nBInc1 = nDB1 / nDY1;
		if ( nDB1 >= 0 )
		{
			nBC1 = 1;
		}
		else
		{
			nBC1 = -1;
			nDB1 = -nDB1;
		}
		nRemBInc1 = nDB1 % nDY1;
		
		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				ScanLineGouraud(
					pbyR0,pbyG0,pbyB0,
					pbyR1,pbyG1,pbyB1,
					nRCur0, nRCur1,
					nGCur0, nGCur1,
					nBCur0, nBCur1);
			}
			else
			{
				ScanLineGouraud(
					pbyR1,pbyG1,pbyB1,
					pbyR0,pbyG0,pbyB0,
					nRCur1, nRCur0,
					nGCur1, nGCur0,
					nBCur1, nBCur0);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;

			nRCur0 += nRInc0;
			nRCur1 += nRInc1;
			
			nGCur0 += nGInc0;
			nGCur1 += nGInc1;
			
			nBCur0 += nBInc0;
			nBCur1 += nBInc1;

			/* increment error accumulation */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
			
			nERInc0 += nRemRInc0;
			nERInc1 += nRemRInc1;

			nEGInc0 += nRemGInc0;
			nEGInc1 += nRemGInc1;

			nEBInc0 += nRemBInc0;
			nEBInc1 += nRemBInc1;
        
			/* make error corrections if necessary */
			if ( nEXInc0 >= nDY0 )
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
			}
        
			if ( nEXInc1 >= nDY1 )
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
			}

			if ( nERInc0 >= nDY0 )
			{
				nERInc0 -= nDY0;
				nRCur0 += nRC0;
			}

			if ( nERInc1 >= nDY1 )
			{
				nERInc1 -= nDY1;
				nRCur1 += nRC1;
			}

			if ( nEGInc0 >= nDY0 )
			{
				nEGInc0 -= nDY0;
				nGCur0 += nGC0;
			}

			if ( nEGInc1 >= nDY1 )
			{
				nEGInc1 -= nDY1;
				nGCur1 += nGC1;
			}

			if ( nEBInc0 >= nDY0 )
			{
				nEBInc0 -= nDY0;
				nBCur0 += nBC0;
			}

			if ( nEBInc1 >= nDY1 )
			{
				nEBInc1 -= nDY1;
				nBCur1 += nBC1;
			}
		}
	}
	else
	{
		pbyR1 += nDX1;
		pbyG1 += nDX1;
		pbyB1 += nDX1;

		nRCur1 = pVMid->byR; 
		nGCur1 = pVMid->byG;
		nBCur1 = pVMid->byB;
	}
    
/* lower half of the triangle */
    
    nDX1 = pVMax->nX - pVMid->nX;
    nDY1 = pVMax->nY - pVMid->nY;
	nDR1 = pVMax->byR - pVMid->byR;
	nDG1 = pVMax->byG - pVMid->byG;
	nDB1 = pVMax->byB - pVMid->byB;

	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0)
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;
	
		/* stuff for the Gouraud shading */
		nRInc1 = nDR1 / nDY1;
		if ( nDR1 >= 0 )
		{
			nRC1 = 1;
		}
		else
		{
			nRC1 = -1;
			nDR1 = -nDR1;
		}
		nRemRInc1 = nDR1 % nDY1;

		nGInc1 = nDG1 / nDY1;
		if ( nDG1 >= 0 )
		{
			nGC1 = 1;
		}
		else
		{
			nGC1 = -1;
			nDG1 = -nDG1;
		}
		nRemGInc1 = nDG1 % nDY1;

		nBInc1 = nDB1 / nDY1;
		if ( nDB1 >= 0 )
		{
			nBC1 = 1;
		}
		else
		{
			nBC1 = -1;
			nDB1 = -nDB1;
		}
		nRemBInc1 = nDB1 % nDY1;

		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				ScanLineGouraud(
					pbyR0,pbyG0,pbyB0,
					pbyR1,pbyG1,pbyB1,
					nRCur0, nRCur1,
					nGCur0, nGCur1,
					nBCur0, nBCur1);
			}
			else
			{
				ScanLineGouraud(
					pbyR1,pbyG1,pbyB1,
					pbyR0,pbyG0,pbyB0,
					nRCur1, nRCur0,
					nGCur1, nGCur0,
					nBCur1, nBCur0);
			}
			
		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
			
			nRCur0 += nRInc0;
			nRCur1 += nRInc1;
			
			nGCur0 += nGInc0;
			nGCur1 += nGInc1;
			
			nBCur0 += nBInc0;
			nBCur1 += nBInc1;

			/* accumulate errors */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
			
			nERInc0 += nRemRInc0;
			nERInc1 += nRemRInc1;

			nEGInc0 += nRemGInc0;
			nEGInc1 += nRemGInc1;

			nEBInc0 += nRemBInc0;
			nEBInc1 += nRemBInc1;
        
			/* purge errors if necessary */
			if (nEXInc0 >= nDY0)
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
			}
        
			if (nEXInc1 >= nDY1)
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
			}

			if ( nERInc0 >= nDY0 )
			{
				nERInc0 -= nDY0;
				nRCur0 += nRC0;
			}

			if ( nERInc1 >= nDY1 )
			{
				nERInc1 -= nDY1;
				nRCur1 += nRC1;
			}

			if ( nEGInc0 >= nDY0 )
			{
				nEGInc0 -= nDY0;
				nGCur0 += nGC0;
			}

			if ( nEGInc1 >= nDY1 )
			{
				nEGInc1 -= nDY1;
				nGCur1 += nGC1;
			}

			if ( nEBInc0 >= nDY0 )
			{
				nEBInc0 -= nDY0;
				nBCur0 += nBC0;
			}

			if ( nEBInc1 >= nDY1 )
			{
				nEBInc1 -= nDY1;
				nBCur1 += nBC1;
			}
		}
	}
	else if ( pbyR0 < pbyR1 )
	{
		ScanLineGouraud(
			pbyR0,pbyG0,pbyB0,
			pbyR1,pbyG1,pbyB1,
			pVMax->byR, pVMid->byR,
			pVMax->byG, pVMid->byG,
			pVMax->byB, pVMid->byB);
	}
	else
	{
		ScanLineGouraud(
			pbyR1,pbyG1,pbyB1,
			pbyR0,pbyG0,pbyB0,
			pVMid->byR, pVMax->byR,
			pVMid->byG, pVMax->byG,
			pVMid->byB, pVMax->byB);
	}
}

void R_TriGouraudZ(p_GPRaster pRaster,
                      p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	/* this seems to be alot of variables.  Do I need all of these? */
    p_GPVertex pVMin,pVMid,pVMax;
    gpInt32 nDY0,nDY1,nDX0,nDX1;
    gpInt32 nXInc0, nXInc1;
    gpInt32 nEXInc0,nEXInc1,nRemInc0,nRemInc1;
    gpInt32 i,nXC0,nXC1,nStart;
    gpByte *pbyR0, *pbyG0, *pbyB0;
    gpByte *pbyR1, *pbyG1, *pbyB1;
    
	gpInt32 nRInc0,nRInc1,nDR0,nDR1,nRC0,nRC1;
	gpInt32 nERInc0,nERInc1,nRemRInc0,nRemRInc1;
	gpInt32 nRCur0, nRCur1;

	gpInt32 nGInc0,nGInc1,nDG0,nDG1,nGC0,nGC1;
	gpInt32 nEGInc0,nEGInc1,nRemGInc0,nRemGInc1;
	gpInt32 nGCur0, nGCur1;

	gpInt32 nBInc0,nBInc1,nDB0,nDB1,nBC0,nBC1;
	gpInt32 nEBInc0,nEBInc1,nRemBInc0,nRemBInc1;
	gpInt32 nBCur0, nBCur1;
	
	gpInt32 nZInc0,nZInc1,nDZ0,nDZ1,nZC0,nZC1;
	gpInt32 nEZInc0,nEZInc1,nRemZInc0,nRemZInc1;
	gpInt32 *pnZ0,*pnZ1, nZCur0, nZCur1;

	/******* DEGENERATE CASES *********/

	/* check for vertical co-linearity */
	if (pV0->nX == pV1->nX && pV1->nX == pV2->nX)
	{
		pVMin = YMINVERTEX(YMINVERTEX(pV0,pV1),pV2);
		pVMax = YMAXVERTEX(YMAXVERTEX(pV0,pV1),pV2);

		R_LineGouraudZ(pRaster,pVMin,pVMax);
		return;
	}
	/* check for horizontal co-linearity */
	else if (pV0->nY == pV1->nY && pV1->nY == pV2->nY)
	{
		pVMin = XMINVERTEX(XMINVERTEX(pV0,pV1),pV2);
		pVMax = XMAXVERTEX(XMAXVERTEX(pV0,pV1),pV2);

		R_LineGouraudZ(pRaster,pVMin,pVMax);
		return;
	}

	/******* NORMAL TRIANGLES *********/

    /* sort the vertices from lowest y (highest point) 
    to highest y (lowest point) */
    if ( pV0->nY >= pV1->nY && pV0->nY >= pV2->nY )
    {
        pVMax = pV0;
        if ( pV1->nY > pV2->nY )
        {
            pVMid = pV1;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV1;
        }
    }
    else if ( pV1->nY >= pV0->nY && pV1->nY >= pV2->nY )
    {
        pVMax = pV1;
        if ( pV0->nY > pV2->nY )
        {
            pVMid = pV0;
            pVMin = pV2;
        }
        else
        {
            pVMid = pV2;
            pVMin = pV0;
        }
    }
    else
    {
        pVMax = pV2;
        if ( pV0->nY > pV1->nY )
        {
            pVMid = pV0;
            pVMin = pV1;
        }
        else
        {
            pVMid = pV1;
            pVMin = pV0;
        }
    }
    
	/* initialize buffer pointers */
    nStart = pVMin->nY * pRaster->nXSize + pVMin->nX; 
    pbyR0 = pbyR1 = pRaster->pRBuffer + nStart;
    pbyG0 = pbyG1 = pRaster->pGBuffer + nStart;
    pbyB0 = pbyB1 = pRaster->pBBuffer + nStart;
	pnZ0 = pnZ1 = pRaster->pZBuffer + nStart;

	 /* errors are nothing at first */
    nEXInc0 = nEXInc1 = 0;
	nERInc0 = nERInc1 = 0;
	nEGInc0 = nEGInc1 = 0;
	nEBInc0 = nEBInc1 = 0;
	nEZInc0 = nEZInc1 = 0;

	/* calculate relavent variables for the edge that goes from 
	the minimum (highest) vertex to the maximum (lowest) vertex */
	
    nDX0 = pVMax->nX - pVMin->nX;
    nDY0 = pVMax->nY - pVMin->nY;
    nXInc0 = ( nDX0 / nDY0 ) + pRaster->nXSize;
	if ( nDX0 >= 0 )
	{
		nXC0 = 1;
	}
	else
	{
		nXC0 = -1;
		nDX0 = -nDX0;
	}
	nRemInc0 = nDX0 % nDY0;
   
	/* setup stuff so we can do Gouraud shading */
	nDR0 = pVMax->byR - pVMin->byR;
	nRInc0 = nDR0 / nDY0;
	if ( nDR0 >= 0 )
	{
		nRC0 = 1;
	}
	else
	{
		nRC0 = -1;
		nDR0 = -nDR0;
	}
	nRemRInc0 = nDR0 % nDY0;

	nDG0 = pVMax->byG - pVMin->byG;
	nGInc0 = nDG0 / nDY0;
	if ( nDG0 >= 0 )
	{
		nGC0 = 1;
	}
	else
	{
		nGC0 = -1;
		nDG0 = -nDG0;
	}
	nRemGInc0 = nDG0 % nDY0;

	nDB0 = pVMax->byB - pVMin->byB;
	nBInc0 = nDB0 / nDY0;
	if ( nDB0 >= 0 )
	{
		nBC0 = 1;
	}
	else
	{
		nBC0 = -1;
		nDB0 = -nDB0;
	}
	nRemBInc0 = nDB0 % nDY0;

	nDZ0 = pVMax->nZ - pVMin->nZ;
	nZInc0 = nDZ0 / nDY0;
	if ( nDZ0 >= 0 )
	{
		nZC0 = 1;
	}
	else
	{
		nZC0 = -1;
		nDZ0 = -nDZ0;
	}
	nRemZInc0 = nDZ0 % nDY0;


	/* deltas for the first active side, besides the min/max 
	edge, which is always active */
    nDY1 = pVMid->nY - pVMin->nY;
	nDX1 = pVMid->nX - pVMin->nX;
	nDR1 = pVMid->byR - pVMin->byR;
	nDG1 = pVMid->byG - pVMin->byG;
	nDB1 = pVMid->byB - pVMin->byB;
	nDZ1 = pVMid->nZ - pVMin->nZ;

	/* initialize current colors */
	nRCur0 = nRCur1 = pVMin->byR;
	nGCur0 = nGCur1 = pVMin->byG;
	nBCur0 = nBCur1 = pVMin->byB;
	nZCur0 = nZCur1 = pVMin->nZ;

	/* upper half of the triangle */
	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0 )
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;

		/* stuff for the gouraud shading*/
		nRInc1 = nDR1 / nDY1;
		if ( nDR1 >= 0 )
		{
			nRC1 = 1;
		}
		else
		{
			nRC1 = -1;
			nDR1 = -nDR1;
		}
		nRemRInc1 = nDR1 % nDY1;

		nGInc1 = nDG1 / nDY1;
		if ( nDG1 >= 0 )
		{
			nGC1 = 1;
		}
		else
		{
			nGC1 = -1;
			nDG1 = -nDG1;
		}
		nRemGInc1 = nDG1 % nDY1;

		nBInc1 = nDB1 / nDY1;
		if ( nDB1 >= 0 )
		{
			nBC1 = 1;
		}
		else
		{
			nBC1 = -1;
			nDB1 = -nDB1;
		}
		nRemBInc1 = nDB1 % nDY1;

		nZInc1 = nDZ1 / nDY1;
		if ( nDZ1 >= 0 )
		{
			nZC1 = 1;
		}
		else
		{
			nZC1 = -1;
			nDZ1 = -nDZ1;
		}
		nRemZInc1 = nDZ1 % nDY1;
		
		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				ScanLineGouraudZ(
					pbyR0,pbyG0,pbyB0,pnZ0,
					pbyR1,pbyG1,pbyB1,pnZ1,
					nRCur0, nRCur1,
					nGCur0, nGCur1,
					nBCur0, nBCur1,
					nZCur0, nZCur1);
			}
			else
			{
				ScanLineGouraudZ(
					pbyR1,pbyG1,pbyB1,pnZ1,
					pbyR0,pbyG0,pbyB0,pnZ0,
					nRCur1, nRCur0,
					nGCur1, nGCur0,
					nBCur1, nBCur0,
					nZCur1, nZCur0);
			}

		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
			pnZ0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
			pnZ1 += nXInc1;

			nRCur0 += nRInc0;
			nRCur1 += nRInc1;
			
			nGCur0 += nGInc0;
			nGCur1 += nGInc1;
			
			nBCur0 += nBInc0;
			nBCur1 += nBInc1;

			nZCur0 += nZInc0;
			nZCur1 += nZInc1;

			/* increment error accumulation */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
			
			nERInc0 += nRemRInc0;
			nERInc1 += nRemRInc1;

			nEGInc0 += nRemGInc0;
			nEGInc1 += nRemGInc1;

			nEBInc0 += nRemBInc0;
			nEBInc1 += nRemBInc1;

			nEZInc0 += nRemZInc0;
			nEZInc1 += nRemZInc1;
        
			/* make error corrections if necessary */
			if ( nEXInc0 >= nDY0 )
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
				pnZ0 += nXC0;
			}
        
			if ( nEXInc1 >= nDY1 )
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
				pnZ1 += nXC1;
			}

			if ( nERInc0 >= nDY0 )
			{
				nERInc0 -= nDY0;
				nRCur0 += nRC0;
			}

			if ( nERInc1 >= nDY1 )
			{
				nERInc1 -= nDY1;
				nRCur1 += nRC1;
			}

			if ( nEGInc0 >= nDY0 )
			{
				nEGInc0 -= nDY0;
				nGCur0 += nGC0;
			}

			if ( nEGInc1 >= nDY1 )
			{
				nEGInc1 -= nDY1;
				nGCur1 += nGC1;
			}

			if ( nEBInc0 >= nDY0 )
			{
				nEBInc0 -= nDY0;
				nBCur0 += nBC0;
			}

			if ( nEBInc1 >= nDY1 )
			{
				nEBInc1 -= nDY1;
				nBCur1 += nBC1;
			}
			
			if ( nEZInc0 >= nDY0 )
			{
				nEZInc0 -= nDY0;
				nZCur0 += nZC0;
			}

			if ( nEZInc1 >= nDY1 )
			{
				nEZInc1 -= nDY1;
				nZCur1 += nZC1;
			}
		}
	}
	else
	{
		pbyR1 += nDX1;
		pbyG1 += nDX1;
		pbyB1 += nDX1;
		pnZ1 += nDX1;

		nRCur1 = pVMid->byR; 
		nGCur1 = pVMid->byG;
		nBCur1 = pVMid->byB;
		nZCur1 = pVMid->nZ;
	}
    
/* lower half of the triangle */
    
    nDX1 = pVMax->nX - pVMid->nX;
    nDY1 = pVMax->nY - pVMid->nY;
	nDR1 = pVMax->byR - pVMid->byR;
	nDG1 = pVMax->byG - pVMid->byG;
	nDB1 = pVMax->byB - pVMid->byB;
	nDZ1 = pVMax->nZ - pVMid->nZ;

	if ( nDY1 )
	{
		nXInc1 = ( nDX1 / nDY1 ) + pRaster->nXSize;
		if ( nDX1 >= 0)
		{
			nXC1 = 1;
		}
		else
		{
			nXC1 = -1;
			nDX1 = -nDX1;
		}
		nRemInc1 = nDX1 % nDY1;
	
		/* stuff for the Gouraud shading */
		nRInc1 = nDR1 / nDY1;
		if ( nDR1 >= 0 )
		{
			nRC1 = 1;
		}
		else
		{
			nRC1 = -1;
			nDR1 = -nDR1;
		}
		nRemRInc1 = nDR1 % nDY1;

		nGInc1 = nDG1 / nDY1;
		if ( nDG1 >= 0 )
		{
			nGC1 = 1;
		}
		else
		{
			nGC1 = -1;
			nDG1 = -nDG1;
		}
		nRemGInc1 = nDG1 % nDY1;

		nBInc1 = nDB1 / nDY1;
		if ( nDB1 >= 0 )
		{
			nBC1 = 1;
		}
		else
		{
			nBC1 = -1;
			nDB1 = -nDB1;
		}
		nRemBInc1 = nDB1 % nDY1;

		nZInc1 = nDZ1 / nDY1;
		if ( nDZ1 >= 0 )
		{
			nZC1 = 1;
		}
		else
		{
			nZC1 = -1;
			nDZ1 = -nDZ1;
		}
		nRemZInc1 = nDZ1 % nDY1;

		for ( i = 0; i < nDY1; i++ )
		{
			if ( pbyR0 < pbyR1 )
			{
				ScanLineGouraudZ(
					pbyR0,pbyG0,pbyB0,pnZ0,
					pbyR1,pbyG1,pbyB1,pnZ1,
					nRCur0, nRCur1,
					nGCur0, nGCur1,
					nBCur0, nBCur1,
					nZCur0, nZCur1);
			}
			else
			{
				ScanLineGouraudZ(
					pbyR1,pbyG1,pbyB1,pnZ1,
					pbyR0,pbyG0,pbyB0,pnZ0,
					nRCur1, nRCur0,
					nGCur1, nGCur0,
					nBCur1, nBCur0,
					nZCur1, nZCur0);
			}
			
		    pbyR0 += nXInc0;
	        pbyG0 += nXInc0;
		    pbyB0 += nXInc0;
			pnZ0 += nXInc0;
        
			pbyR1 += nXInc1;
	        pbyG1 += nXInc1;
	        pbyB1 += nXInc1;
			pnZ1 += nXInc1;
			
			nRCur0 += nRInc0;
			nRCur1 += nRInc1;
			
			nGCur0 += nGInc0;
			nGCur1 += nGInc1;
			
			nBCur0 += nBInc0;
			nBCur1 += nBInc1;

			nZCur0 += nZInc0;
			nZCur1 += nZInc1;

			/* accumulate errors */
		    nEXInc0 += nRemInc0;
			nEXInc1 += nRemInc1;
			
			nERInc0 += nRemRInc0;
			nERInc1 += nRemRInc1;

			nEGInc0 += nRemGInc0;
			nEGInc1 += nRemGInc1;

			nEBInc0 += nRemBInc0;
			nEBInc1 += nRemBInc1;

			nEZInc0 += nRemZInc0;
			nEZInc1 += nRemZInc1;
        
			/* purge errors if necessary */
			if (nEXInc0 >= nDY0)
		    {
			    nEXInc0 -= nDY0;
				pbyR0 += nXC0;
			    pbyG0 += nXC0;
				pbyB0 += nXC0;
				pnZ0 += nXC0;
			}
        
			if (nEXInc1 >= nDY1)
			{
				nEXInc1 -= nDY1;
				pbyR1 += nXC1;
				pbyG1 += nXC1;
			    pbyB1 += nXC1;
				pnZ1 += nXC1;
			}

			if ( nERInc0 >= nDY0 )
			{
				nERInc0 -= nDY0;
				nRCur0 += nRC0;
			}

			if ( nERInc1 >= nDY1 )
			{
				nERInc1 -= nDY1;
				nRCur1 += nRC1;
			}

			if ( nEGInc0 >= nDY0 )
			{
				nEGInc0 -= nDY0;
				nGCur0 += nGC0;
			}

			if ( nEGInc1 >= nDY1 )
			{
				nEGInc1 -= nDY1;
				nGCur1 += nGC1;
			}

			if ( nEBInc0 >= nDY0 )
			{
				nEBInc0 -= nDY0;
				nBCur0 += nBC0;
			}

			if ( nEBInc1 >= nDY1 )
			{
				nEBInc1 -= nDY1;
				nBCur1 += nBC1;
			}

			if ( nEZInc0 >= nDY0 )
			{
				nEZInc0 -= nDY0;
				nZCur0 += nZC0;
			}

			if ( nEZInc1 >= nDY1 )
			{
				nEZInc1 -= nDY1;
				nZCur1 += nZC1;
			}
		}
	}
	else if ( pbyR0 < pbyR1 )
	{
		ScanLineGouraudZ(
			pbyR0,pbyG0,pbyB0,pnZ0,
			pbyR1,pbyG1,pbyB1,pnZ1,
			pVMax->byR, pVMid->byR,
			pVMax->byG, pVMid->byG,
			pVMax->byB, pVMid->byB,
			pVMax->nZ, pVMid->nZ);
	}
	else
	{
		ScanLineGouraudZ(
			pbyR1,pbyG1,pbyB1,pnZ0,
			pbyR0,pbyG0,pbyB0,pnZ1,
			pVMid->byR, pVMax->byR,
			pVMid->byG, pVMax->byG,
			pVMid->byB, pVMax->byB,
			pVMid->nZ, pVMax->nZ);
	}
}

/* wire, flat, non-z-buffered triangle */
void R_TriWireFlat(p_GPRaster pRaster, 
				 gpByte byR, gpByte byG, gpByte byB,
			     p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	R_LineFlat(pRaster, byR, byG, byB, pV0, pV1);
	R_LineFlat(pRaster, byR, byG, byB, pV1, pV2);
	R_LineFlat(pRaster, byR, byG, byB, pV2, pV0);
}

/* wire, flat, z-buffered triangle */
void R_TriWireFlatZ(p_GPRaster pRaster, 
				  gpByte byR, gpByte byG, gpByte byB,
			      p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	R_LineFlatZ(pRaster, byR, byG, byB, pV0, pV1);
	R_LineFlatZ(pRaster, byR, byG, byB, pV1, pV2);
	R_LineFlatZ(pRaster, byR, byG, byB, pV2, pV0);
}

/* wire, gouraud, z-buffered triangle */
void R_TriWireGouraud(p_GPRaster pRaster, 
				    p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	R_LineGouraud(pRaster, pV0, pV1);
	R_LineGouraud(pRaster, pV1, pV2);
	R_LineGouraud(pRaster, pV2, pV0);
}

/* wire, gouraud, non-z-buffered triangle */
void R_TriWireGouraudZ(p_GPRaster pRaster, 
				     p_GPVertex pV0,p_GPVertex pV1,p_GPVertex pV2)
{
	R_LineGouraudZ(pRaster, pV0, pV1);
	R_LineGouraudZ(pRaster, pV1, pV2);
	R_LineGouraudZ(pRaster, pV2, pV0);
}

/********** POLYGONS ************/

/* flat shaded, non-z-buffered polygon */
void R_PolygonFlat(p_GPRaster pRaster,
				 gpByte byR, gpByte byG, gpByte byB,
				 p_GPVPList pList)
{
	int nLast,nV0,nV1,nV2;
	p_GPVertex pV0,pV1,pV2;
	
	nV0 = 0;
	nLast = pList->nCount - 1;

	nV1 = 0;
	nV2 = 1;
	do
	{
	    nV1++;
		nV2++;

		pV0 = pList->pArray[nV0];
		pV1 = pList->pArray[nV1];
		pV2 = pList->pArray[nV2];

		R_TriFlat(pRaster,pV0,pV1,pV2,byR,byG,byB);		
	}
	while(nV2 < nLast);
}

/* flat shaded, z-buffered polygon */
void R_PolygonFlatZ(p_GPRaster pRaster, 
				  gpByte byR, gpByte byG, gpByte byB,
				  p_GPVPList pList)
{
	int nLast,nV0,nV1,nV2;
	p_GPVertex pV0,pV1,pV2;
	
	nV0 = 0;
	nLast = pList->nCount - 1;

	nV1 = 0;
	nV2 = 1;
	do
	{
		nV1++;
		nV2++;

		pV0 = pList->pArray[nV0];
		pV1 = pList->pArray[nV1];
		pV2 = pList->pArray[nV2];

		R_TriFlatZ(pRaster,pV0,pV1,pV2,byR,byG,byB);		
	}
	while(nV2 < nLast);
}

/* gouraud shaded, z-buffered polygon */
void R_PolygonGouraud(p_GPRaster pRaster, p_GPVPList pList)
{
	int nLast,nV0,nV1,nV2;
	p_GPVertex pV0,pV1,pV2;
	
	nV0 = 0;
	nLast = pList->nCount - 1;

	nV1 = 0;
	nV2 = 1;
	do
	{
		nV1++;
		nV2++;

		pV0 = pList->pArray[nV0];
		pV1 = pList->pArray[nV1];
		pV2 = pList->pArray[nV2];

		R_TriGouraud(pRaster,pV0,pV1,pV2);		
	}
	while(nV2 < nLast);
}

/* gouraud shaded, non-z-buffered polygon */
void R_PolygonGouraudZ(p_GPRaster pRaster, p_GPVPList pList)
{
	int nLast,nV0,nV1,nV2;
	p_GPVertex pV0,pV1,pV2;
	
	nV0 = 0;
	nLast = pList->nCount - 1;

	nV1 = 0;
	nV2 = 1;
	do
	{
		nV1++;
		nV2++;

		pV0 = pList->pArray[nV0];
		pV1 = pList->pArray[nV1];
		pV2 = pList->pArray[nV2];

		R_TriGouraudZ(pRaster,pV0,pV1,pV2);		
	}
	while(nV2 < nLast);
}

/* wire, flat, non-z-buffered polygon */
void R_PolygonWireFlat(p_GPRaster pRaster, 
					 gpByte byR, gpByte byG, gpByte byB,
					 p_GPVPList pList)
{
	p_GPVertex *pArray;
	int i,nCount;

	nCount = pList->nCount - 1;
	pArray = pList->pArray;

	for(i = 0; i < nCount; i++)
	{
		R_LineFlat(pRaster,byR,byG,byB,pArray[i],pArray[i+1]);
	}

	R_LineFlat(pRaster,byR,byG,byB,pArray[i],pArray[0]);
}

/* wire, flat, z-buffered polygon */
void R_PolygonWireFlatZ(p_GPRaster pRaster, 
					  gpByte byR, gpByte byG, gpByte byB,
					  p_GPVPList pList)
{
	p_GPVertex *pArray;
	int i,nCount;

	nCount = pList->nCount - 1;
	pArray = pList->pArray;

	for(i = 0; i < nCount; i++)
	{
		R_LineFlatZ(pRaster,byR,byG,byB,pArray[i],pArray[i+1]);
	}

	R_LineFlatZ(pRaster,byR,byG,byB,pArray[i],pArray[0]);
}

/* wire, gouraud, z-buffered polygon */
void R_PolygonWireGouraud(p_GPRaster pRaster, p_GPVPList pList)
{
	p_GPVertex *pArray;
	int i,nCount;

	nCount = pList->nCount - 1;
	pArray = pList->pArray;

	for(i = 0; i < nCount; i++)
	{
		R_LineGouraud(pRaster,pArray[i],pArray[i+1]);
	}

	R_LineGouraud(pRaster,pArray[i],pArray[0]);
}

/* wire, gouraud, non-z-buffered polygon */
void R_PolygonWireGouraudZ(p_GPRaster pRaster, p_GPVPList pList)
{
	p_GPVertex *pArray;
	int i,nCount;

	nCount = pList->nCount - 1;
	pArray = pList->pArray;

	for(i = 0; i < nCount; i++)
	{
		R_LineGouraudZ(pRaster,pArray[i],pArray[i+1]);
	}

	R_LineGouraudZ(pRaster,pArray[i],pArray[0]);
}

/*********** LINES ************/

/* flat line, non-z-buffered */
void R_LineFlat(p_GPRaster pRaster,gpByte byR,gpByte byG,gpByte byB, 
				  p_GPVertex pV0,p_GPVertex pV1)
{
    gpInt32 nDX, nDY, nXInc, nYInc;
    gpInt32 iIndex, nError = 0;
    gpByte *pbyRB, *pbyGB, *pbyBB;

	/* if we really have a point, not a line */
	if(pV0->nX == pV1->nX && pV0->nY == pV1->nY)
	{
		R_Point(pRaster,pV0);
		return;
	}
    
    pbyRB = pRaster->pRBuffer;
    pbyRB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyGB = pRaster->pGBuffer;
    pbyGB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyBB = pRaster->pBBuffer;
    pbyBB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    nDX = pV1->nX - pV0->nX;
    nDY = pV1->nY - pV0->nY;

    if (nDX >= 0)
    {
        nXInc = 1;
    }
    else
    {
        nXInc = -1;
        nDX = -nDX;
    }
    
    if (nDY >= 0)
    {
        nYInc = pRaster->nXSize;
    }
    else
    {
        nYInc = -pRaster->nXSize;
        nDY = -nDY;
    }
    
    if (nDX >= nDY)
    {
        for(iIndex = 0; iIndex <= nDX; iIndex++)
        {
            *pbyRB = byR;
            *pbyGB = byG;
            *pbyBB = byB;
            
            nError += nDY;
            
            if(nError > nDX)
            {
                nError -= nDX;
                
                pbyRB += nYInc;
                pbyGB += nYInc;
                pbyBB += nYInc;
            }
            
            pbyRB += nXInc;
            pbyGB += nXInc;
            pbyBB += nXInc;
        }
    }
    else
    {
        for(iIndex = 0; iIndex <= nDY; iIndex++)
        {
            *pbyRB = byR;
            *pbyGB = byG;
            *pbyBB = byB;
            
            nError += nDX;
            
            if(nError > nDY)
            {
                nError -= nDY;
                
                pbyRB += nXInc;
                pbyGB += nXInc;
                pbyBB += nXInc;
            }
            
            pbyRB += nYInc;
            pbyGB += nYInc;
            pbyBB += nYInc;
        }
    }
}

/* flat line, z-buffered */
void R_LineFlatZ(p_GPRaster pRaster,gpByte byR,gpByte byG,gpByte byB, 
				   p_GPVertex pV0,p_GPVertex pV1)
{
    gpInt32 nDX, nDY, nADX, nADY, nXInc, nYInc, iIndex, nErrorL;
    gpByte *pbyRB, *pbyGB, *pbyBB;
    gpInt32 *pnZB,nErrorZ,nDZ,nADZ,nZCur,nZInc,nZRem,nZC;

	/* if we really have a point, not a line */
	if(pV0->nX == pV1->nX && pV0->nY == pV1->nY)
	{
		R_PointZ(pRaster,pV0);
		return;
	}
    
    nErrorL = 0;
    nErrorZ = 0;
    
    pbyRB = pRaster->pRBuffer;
    pbyRB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyGB = pRaster->pGBuffer;
    pbyGB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyBB = pRaster->pBBuffer;
    pbyBB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pnZB = pRaster->pZBuffer;
    pnZB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    nDX = pV1->nX - pV0->nX;
    nDY = pV1->nY - pV0->nY;
    nDZ = pV1->nZ - pV0->nZ;
    
    if (nDX >= 0)
    {
        nXInc = 1;
        nADX = nDX;
    }
    else
    {
        nXInc = -1;
        nADX = -nDX; 
    }
    
    if (nDY >= 0)
    {
        nYInc = pRaster->nXSize;
        nADY = nDY;
    }
    else
    {
        nYInc = -pRaster->nXSize;
        nADY = -nDY; 
    }
    
    if (nDZ >= 0)
    {
        nZC = 1;
        nADZ = nDZ;
    }
    else
    {
        nZC = -1;
        nADZ = -nDZ;
    }
    
    nZCur = pV0->nZ;
    
    if (nADX >= nADY)
    {
        nZInc = nDZ / nADX; 
        nZRem = nADZ % nADX;
        
        for(iIndex = 0; iIndex <= nADX; iIndex++)
        {
            if (nZCur > *pnZB)
            {
                *pbyRB = byR;
                *pbyGB = byG;
                *pbyBB = byB;
                *pnZB = nZCur;
            }
            
            nErrorL += nADY;
            
            if(nErrorL > nADX)
            {
                nErrorL -= nADX;
                
                pbyRB += nYInc;
                pbyGB += nYInc;
                pbyBB += nYInc;
                pnZB += nYInc;
            }
            
            pbyRB += nXInc;
            pbyGB += nXInc;
            pbyBB += nXInc;
            pnZB += nXInc;
            
            nZCur += nZInc;
            nErrorZ += nZRem;
            
            if(nErrorZ > nADX)
            {
                nErrorZ -= nADX;
                nZCur += nZC;
            }
        }
    }
    else
    {
        nZInc = nDZ / nADY; 
        nZRem = nADZ % nADY;
        
        for(iIndex = 0; iIndex <= nADY; iIndex++)
        {
            if (nZCur > *pnZB)
            {
                *pbyRB = byR;
                *pbyGB = byG;
                *pbyBB = byB;
                *pnZB = nZCur;
            }
            
            nErrorL += nADX;
            
            if(nErrorL > nADY)
            {
                nErrorL -= nADY;
                
                pbyRB += nXInc;
                pbyGB += nXInc;
                pbyBB += nXInc;
                pnZB += nXInc;
            }
            
            pbyRB += nYInc;
            pbyGB += nYInc;
            pbyBB += nYInc;
            pnZB += nYInc;
            
            nZCur += nZInc;
            nErrorZ += nZRem;
            
            if(nErrorZ > nADY)
            {
                nErrorZ -= nADY;
                nZCur += nZC;
            }
        }
    }
}

/* gouraud line, non-z-buffered */
void R_LineGouraud(p_GPRaster pRaster,p_GPVertex pV0,p_GPVertex pV1)
{
	gpInt32 nDX, nDY, nADX, nADY, nXInc, nYInc, iIndex, nErrorL;
    gpByte *pbyRB, *pbyGB, *pbyBB;

    gpInt32 nErrorR,nDR,nADR,nRCur,nRInc,nRRem,nRC;
	gpInt32 nErrorG,nDG,nADG,nGCur,nGInc,nGRem,nGC;
	gpInt32 nErrorB,nDB,nADB,nBCur,nBInc,nBRem,nBC;

	/* if we really have a point, not a line */
	if(pV0->nX == pV1->nX && pV0->nY == pV1->nY)
	{
		R_Point(pRaster,pV0);
		return;
	}
    
    pbyRB = pRaster->pRBuffer;
    pbyRB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyGB = pRaster->pGBuffer;
    pbyGB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    pbyBB = pRaster->pBBuffer;
    pbyBB += pV0->nY * pRaster->nXSize + pV0->nX;
    
    nDX = pV1->nX - pV0->nX;
    nDY = pV1->nY - pV0->nY;
	nDR = pV1->byR - pV0->byR;
	nDG = pV1->byG - pV0->byG;
	nDB = pV1->byB - pV0->byB;

	nErrorL = nErrorR = nErrorG = nErrorB = 0;

    if (nDX >= 0)
    {
        nXInc = 1;
		nADX = nDX;
    }
    else
    {
        nXInc = -1;
        nADX = -nDX;
    }
    
    if (nDY >= 0)
    {
        nYInc = pRaster->nXSize;
		nADY = nDY;
    }
    else
    {
        nYInc = -pRaster->nXSize;
        nADY = -nDY;
    }

	/* initialize color information */
	if (nDR >= 0)
    {
        nRC = 1;
        nADR = nDR;
    }
    else
    {
        nRC = -1;
        nADR = -nDR;
    }
    nRCur = pV0->byR;

	if (nDG >= 0)
    {
        nGC = 1;
        nADG = nDG;
    }
    else
    {
        nGC = -1;
        nADG = -nDG;
    }
    nGCur = pV0->byG;

	if (nDB >= 0)
    {
        nBC = 1;
        nADB = nDB;
    }
    else
    {
        nBC = -1;
        nADB = -nDB;
    }
    nBCur = pV0->byB;
    
    if (nADX >= nADY)
    {
		nRInc = nDR / nADX; 
        nRRem = nADR % nADX;

		nGInc = nDG / nADX; 
        nGRem = nADG % nADX;

		nBInc = nDB / nADX; 
        nBRem = nADB % nADX;

        for(iIndex = 0; iIndex <= nADX; iIndex++)
        {
            *pbyRB = (gpByte)nRCur;
            *pbyGB = (gpByte)nGCur;
            *pbyBB = (gpByte)nBCur;
            
            nErrorL += nADY;
            
            if(nErrorL > nADX)
            {
                nErrorL -= nADX;
                
                pbyRB += nYInc;
                pbyGB += nYInc;
                pbyBB += nYInc;
            }
            
            pbyRB += nXInc;
            pbyGB += nXInc;
            pbyBB += nXInc;

			nRCur += nRInc;
			nGCur += nGInc;
			nBCur += nBInc;
            
			nErrorR += nRRem;
			nErrorG += nGRem;
			nErrorB += nBRem;
            
            if(nErrorR > nADX)
            {
                nErrorR -= nADX;
                nRCur += nRC;
            }

			if(nErrorG > nADX)
            {
                nErrorG -= nADX;
                nGCur += nGC;
            }

			if(nErrorB > nADX)
            {
                nErrorB -= nADX;
                nBCur += nBC;
            }
        }
    }
    else
    {
		nRInc = nDR / nADY; 
        nRRem = nADR % nADY;

		nGInc = nDG / nADY; 
        nGRem = nADG % nADY;

		nBInc = nDB / nADY; 
        nBRem = nADB % nADY;

        for(iIndex = 0; iIndex <= nADY; iIndex++)
        {
            *pbyRB = (gpByte)nRCur;
            *pbyGB = (gpByte)nGCur;
            *pbyBB = (gpByte)nBCur;
            
            nErrorL += nADX;
            
            if(nErrorL > nADY)
            {
                nErrorL -= nADY;
                
                pbyRB += nXInc;
                pbyGB += nXInc;
                pbyBB += nXInc;
            }
            
            pbyRB += nYInc;
            pbyGB += nYInc;
            pbyBB += nYInc;

			nRCur += nRInc;
			nGCur += nGInc;
			nBCur += nBInc;
            
			nErrorR += nRRem;
			nErrorG += nGRem;
			nErrorB += nBRem;
            
            if(nErrorR > nADY)
            {
                nErrorR -= nADY;
                nRCur += nRC;
            }

			if(nErrorG > nADY)
            {
                nErrorG -= nADY;
                nGCur += nGC;
            }

			if(nErrorB > nADY)
            {
                nErrorB -= nADY;
                nBCur += nBC;
            }
        }
    }
}

/* gouraud line, z-buffered */
void R_LineGouraudZ(p_GPRaster pRaster,p_GPVertex pV0,p_GPVertex pV1)
{
	gpInt32 nDX, nDY, nADX, nADY, nXInc, nYInc, iIndex, nErrorL;
    gpByte *pbyRB, *pbyGB, *pbyBB;	
	gpInt32 *pnZB, nStart;

    gpInt32 nErrorR,nDR,nADR,nRCur,nRInc,nRRem,nRC;
	gpInt32 nErrorG,nDG,nADG,nGCur,nGInc,nGRem,nGC;
	gpInt32 nErrorB,nDB,nADB,nBCur,nBInc,nBRem,nBC;
	gpInt32 nErrorZ,nDZ,nADZ,nZCur,nZInc,nZRem,nZC;

	/* if we really have a point, not a line */
	if(pV0->nX == pV1->nX && pV0->nY == pV1->nY)
	{
		R_PointZ(pRaster,pV0);
		return;
	}
    
	nStart = pV0->nY * pRaster->nXSize + pV0->nX;
    pbyRB = pRaster->pRBuffer + nStart; 
    pbyGB = pRaster->pGBuffer + nStart;
    pbyBB = pRaster->pBBuffer + nStart;
	pnZB = pRaster->pZBuffer + nStart;
    
    nDX = pV1->nX - pV0->nX;
    nDY = pV1->nY - pV0->nY;
	nDZ = pV1->nZ - pV0->nZ;
	nDR = pV1->byR - pV0->byR;
	nDG = pV1->byG - pV0->byG;
	nDB = pV1->byB - pV0->byB;

	nErrorL = nErrorR = nErrorG = nErrorB = nErrorZ = 0;

    if (nDX >= 0)
    {
        nXInc = 1;
		nADX = nDX;
    }
    else
    {
        nXInc = -1;
        nADX = -nDX;
    }
    
    if (nDY >= 0)
    {
        nYInc = pRaster->nXSize;
		nADY = nDY;
    }
    else
    {
        nYInc = -pRaster->nXSize;
        nADY = -nDY;
    }

	/* initialize color information */
	if (nDR >= 0)
    {
        nRC = 1;
        nADR = nDR;
    }
    else
    {
        nRC = -1;
        nADR = -nDR;
    }
    nRCur = pV0->byR;

	if (nDG >= 0)
    {
        nGC = 1;
        nADG = nDG;
    }
    else
    {
        nGC = -1;
        nADG = -nDG;
    }
    nGCur = pV0->byG;

	if (nDB >= 0)
    {
        nBC = 1;
        nADB = nDB;
    }
    else
    {
        nBC = -1;
        nADB = -nDB;
    }
    nBCur = pV0->byB;

    if (nDZ >= 0)
    {
        nZC = 1;
        nADZ = nDZ;
    }
    else
    {
        nZC = -1;
        nADZ = -nDZ;
    }
    
    nZCur = pV0->nZ;

    if (nADX >= nADY)
    {
		nRInc = nDR / nADX; 
        nRRem = nADR % nADX;

		nGInc = nDG / nADX; 
        nGRem = nADG % nADX;

		nBInc = nDB / nADX; 
        nBRem = nADB % nADX;

		nZInc = nDZ / nADX; 
        nZRem = nADZ % nADX;

        for(iIndex = 0; iIndex <= nADX; iIndex++)
        {
			if (nZCur > *pnZB)
            {
				*pbyRB = (gpByte)nRCur;
				*pbyGB = (gpByte)nGCur;
				*pbyBB = (gpByte)nBCur;
                *pnZB = nZCur;
            }
            
            nErrorL += nADY;
            
            if(nErrorL > nADX)
            {
                nErrorL -= nADX;
                
                pbyRB += nYInc;
                pbyGB += nYInc;
                pbyBB += nYInc;
				pnZB += nYInc;
            }
            
            pbyRB += nXInc;
            pbyGB += nXInc;
            pbyBB += nXInc;
			pnZB += nXInc;

			nRCur += nRInc;
			nGCur += nGInc;
			nBCur += nBInc;
			nZCur += nZInc;
            
			nErrorR += nRRem;
			nErrorG += nGRem;
			nErrorB += nBRem;
			nErrorZ += nZRem;
            
            if(nErrorR > nADX)
            {
                nErrorR -= nADX;
                nRCur += nRC;
            }

			if(nErrorG > nADX)
            {
                nErrorG -= nADX;
                nGCur += nGC;
            }

			if(nErrorB > nADX)
            {
                nErrorB -= nADX;
                nBCur += nBC;
            }
			
			if(nErrorZ > nADX)
            {
                nErrorZ -= nADX;
                nZCur += nZC;
            }
        }
    }
    else
    {
		nRInc = nDR / nADY; 
        nRRem = nADR % nADY;

		nGInc = nDG / nADY; 
        nGRem = nADG % nADY;

		nBInc = nDB / nADY; 
        nBRem = nADB % nADY;

		nZInc = nDZ / nADY; 
        nZRem = nADZ % nADY;

        for(iIndex = 0; iIndex <= nADY; iIndex++)
        {
			if (nZCur > *pnZB)
            {
				*pbyRB = (gpByte)nRCur;
				*pbyGB = (gpByte)nGCur;
				*pbyBB = (gpByte)nBCur;
                *pnZB = nZCur;
            }
            
            nErrorL += nADX;
            
            if(nErrorL > nADY)
            {
                nErrorL -= nADY;
                
                pbyRB += nXInc;
                pbyGB += nXInc;
                pbyBB += nXInc;
				pnZB += nXInc;
            }
            
            pbyRB += nYInc;
            pbyGB += nYInc;
            pbyBB += nYInc;
			pnZB += nYInc;
            
			nRCur += nRInc;
			nGCur += nGInc;
			nBCur += nBInc;
			nZCur += nZInc;
            
			nErrorR += nRRem;
			nErrorG += nGRem;
			nErrorB += nBRem;
			nErrorZ += nZRem;
            
            if(nErrorR > nADY)
            {
                nErrorR -= nADY;
                nRCur += nRC;
            }

			if(nErrorG > nADY)
            {
                nErrorG -= nADY;
                nGCur += nGC;
            }

			if(nErrorB > nADY)
            {
                nErrorB -= nADY;
                nBCur += nBC;
            }
			
			if(nErrorZ > nADY)
            {
                nErrorZ -= nADY;
                nZCur += nZC;
            }
        }
    }
}

/* point, z-buffered */
void R_PointZ(p_GPRaster pRaster,p_GPVertex pV)
{
    gpByte *pbyRB, *pbyGB, *pbyBB;	
	gpInt32 *pnZB, nStart;

	nStart = pV->nY * pRaster->nXSize + pV->nX;
    pbyRB = pRaster->pRBuffer + nStart; 
    pbyGB = pRaster->pGBuffer + nStart;
    pbyBB = pRaster->pBBuffer + nStart;
	pnZB = pRaster->pZBuffer + nStart;

	if(pV->nZ > *pnZB)
	{
		*pbyRB = pV->byR;
		*pbyGB = pV->byG;
		*pbyBB = pV->byB;
        *pnZB = pV->nZ;
	}
}

/* point, non-z-buffered */
void R_Point(p_GPRaster pRaster,p_GPVertex pV)
{
	gpByte *pbyRB, *pbyGB, *pbyBB;	
	gpInt32 nStart;

	nStart = pV->nY * pRaster->nXSize + pV->nX;
    pbyRB = pRaster->pRBuffer + nStart; 
    pbyGB = pRaster->pGBuffer + nStart;
    pbyBB = pRaster->pBBuffer + nStart;

	*pbyRB = pV->byR;
	*pbyGB = pV->byG;
	*pbyBB = pV->byB;    
}

/* BACK_END FUNCTION */

void ScanLineZ(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,gpInt32 *pnZ0,
			   gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,gpInt32 *pnZ1,
			   gpInt32 nZCur0, gpInt32 nZCur1,
			   gpByte byR,gpByte byG,gpByte byB)
{
	gpByte *pbyRCur, *pbyGCur, *pbyBCur;
	gpInt32 nDX, nDZ, nZInc, nZRem, nEZ, nZC;
	gpInt32 *pnZCur, nZ;
	
	nDZ = nZCur1 - nZCur0;
	nDX = pbyR1 - pbyR0 + 1; /* could have used G1-G0 or B1-B0 */
	
	nZInc = nDZ / nDX;
	if(nDZ >=0)
	{
		nZC = 1;
	}
	else
	{
		nZC = -1;
		nDZ = -nDZ;
	}
	nZRem = nDZ % nDX;

	nEZ = 0;
	nZ = nZCur0;

	for (pbyRCur=pbyR0,pbyGCur=pbyG0,pbyBCur=pbyB0,pnZCur=pnZ0;
		 pbyRCur <= pbyR1;
		 pbyRCur++,pbyGCur++,pbyBCur++,pnZCur++) 
	{
		if ( nZ >= *pnZCur )
		{
			*pbyRCur = byR;
			*pbyGCur = byG;
			*pbyBCur = byB;
			*pnZCur = nZ;
		}

		nZ += nZInc;
		nEZ += nZRem;

		if ( nEZ > nDX )
		{
			nEZ -= nDX;
			nZ += nZC;
		}
	}
}

void ScanLineGouraud(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,
					 gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,
					 gpInt32 nRCur0, gpInt32 nRCur1,
					 gpInt32 nGCur0, gpInt32 nGCur1,
					 gpInt32 nBCur0, gpInt32 nBCur1)
{
	gpByte *pbyRCur, *pbyGCur, *pbyBCur;
	gpInt32 nDX; 
	
	gpInt32 nR, nDR, nRInc, nRRem, nER, nRC;
	gpInt32 nG, nDG, nGInc, nGRem, nEG, nGC;
	gpInt32 nB, nDB, nBInc, nBRem, nEB, nBC;
	
	nDX = pbyR1 - pbyR0 + 1; /* could have used G1-G0 or B1-B0 */

	nDR = nRCur1 - nRCur0;
	nDG = nGCur1 - nGCur0;
	nDB = nBCur1 - nBCur0;
	
	nRInc = nDR / nDX;
	if(nDR >=0)
	{
		nRC = 1;
	}
	else
	{
		nRC = -1;
		nDR = -nDR;
	}
	nRRem = nDR % nDX;

	nGInc = nDG / nDX;
	if(nDG >=0)
	{
		nGC = 1;
	}
	else
	{
		nGC = -1;
		nDG = -nDG;
	}
	nGRem = nDG % nDX;

	nBInc = nDB / nDX;
	if(nDB >=0)
	{
		nBC = 1;
	}
	else
	{
		nBC = -1;
		nDB = -nDB;
	}
	nBRem = nDB % nDX;

	nER = nEG = nEB = 0;
	nR = nRCur0;
	nG = nGCur0;
	nB = nBCur0;

	for (pbyRCur=pbyR0,pbyGCur=pbyG0,pbyBCur=pbyB0;
		 pbyRCur <= pbyR1;
		 pbyRCur++,pbyGCur++,pbyBCur++) 
	{
	
		*pbyRCur = (gpByte)nR;
		*pbyGCur = (gpByte)nG;
		*pbyBCur = (gpByte)nB;
	
		nR += nRInc;
		nG += nGInc;
		nB += nBInc;

		nER += nRRem;
		nEG += nGRem;
		nEB += nBRem;

		if ( nER > nDX )
		{
			nER -= nDX;
			nR += nRC;
		}

		if ( nEG > nDX )
		{
			nEG -= nDX;
			nG += nGC;
		}

		if ( nEB > nDX )
		{
			nEB -= nDX;
			nB += nBC;
		}
	}
}

void ScanLineGouraudZ(gpByte *pbyR0,gpByte *pbyG0,gpByte *pbyB0,gpInt32 *pnZ0,
					  gpByte *pbyR1,gpByte *pbyG1,gpByte *pbyB1,gpInt32 *pnZ1,
					  gpInt32 nRCur0, gpInt32 nRCur1,
					  gpInt32 nGCur0, gpInt32 nGCur1,
					  gpInt32 nBCur0, gpInt32 nBCur1,
					  gpInt32 nZCur0, gpInt32 nZCur1)
{
	gpByte *pbyRCur, *pbyGCur, *pbyBCur;
	gpInt32 *pnZCur;
	gpInt32 nDX; 
	
	gpInt32 nR, nDR, nRInc, nRRem, nER, nRC;
	gpInt32 nG, nDG, nGInc, nGRem, nEG, nGC;
	gpInt32 nB, nDB, nBInc, nBRem, nEB, nBC;
	gpInt32 nZ, nDZ, nZInc, nZRem, nEZ, nZC;
	
	nDX = pbyR1 - pbyR0 + 1; /* could have used G1-G0 or B1-B0 */

	nDR = nRCur1 - nRCur0;
	nDG = nGCur1 - nGCur0;
	nDB = nBCur1 - nBCur0;
	nDZ = nZCur1 - nZCur0;
	
	nRInc = nDR / nDX;
	if(nDR >=0)
	{
		nRC = 1;
	}
	else
	{
		nRC = -1;
		nDR = -nDR;
	}
	nRRem = nDR % nDX;

	nGInc = nDG / nDX;
	if(nDG >=0)
	{
		nGC = 1;
	}
	else
	{
		nGC = -1;
		nDG = -nDG;
	}
	nGRem = nDG % nDX;

	nBInc = nDB / nDX;
	if(nDB >=0)
	{
		nBC = 1;
	}
	else
	{
		nBC = -1;
		nDB = -nDB;
	}
	nBRem = nDB % nDX;

	nZInc = nDZ / nDX;
	if(nDZ >=0)
	{
		nZC = 1;
	}
	else
	{
		nZC = -1;
		nDZ = -nDZ;
	}
	nZRem = nDZ % nDX;

	nER = nEG = nEB = nEZ = 0;
	nR = nRCur0;
	nG = nGCur0;
	nB = nBCur0;
	nZ = nZCur0;

	for (pbyRCur=pbyR0,pbyGCur=pbyG0,pbyBCur=pbyB0,pnZCur=pnZ0;
		 pbyRCur <= pbyR1;
		 pbyRCur++,pbyGCur++,pbyBCur++,pnZCur++) 
	{
		if (nZ > *pnZCur) 
		{
			*pbyRCur = (gpByte)nR;	
			*pbyGCur = (gpByte)nG;
			*pbyBCur = (gpByte)nB;
			*pnZCur = nZ;
		}
	
		nR += nRInc;
		nG += nGInc;
		nB += nBInc;
		nZ += nZInc;

		nER += nRRem;
		nEG += nGRem;
		nEB += nBRem;
		nEZ += nZRem;

		if ( nER > nDX )
		{
			nER -= nDX;
			nR += nRC;
		}

		if ( nEG > nDX )
		{
			nEG -= nDX;
			nG += nGC;
		}

		if ( nEB > nDX )
		{
			nEB -= nDX;
			nB += nBC;
		}

		if ( nEZ > nDX )
		{
			nEZ -= nDX;
			nZ += nZC;
		}
	}
}
