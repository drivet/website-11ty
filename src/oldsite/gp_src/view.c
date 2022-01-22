#include "view.h"
#include "algebra.h"
#include <stdlib.h>
#include <string.h>

/* create and initialize a viewing module */
p_GPView CreateGPView()
{
	p_GPView pView = (p_GPView)malloc(sizeof(GPView));
	
	Identity(pView->adfW2E);
	Identity(pView->adfE2N);
	Identity(pView->adfN2R);
	Identity(pView->adfW2R);
	
	pView->bW2RDirty = FALSE;
	pView->bE2RDirty = FALSE;
	return pView;
}

void DefineVRC(p_GPView pView,
			   double dfVRPX, double dfVRPY, double dfVRPZ, 
			   double dfVPNX, double dfVPNY, double dfVPNZ, 
			   double dfVUPX, double dfVUPY, double dfVUPZ)
{
	double adfRz[4], adfRy[4], adfRx[4];
	double adfTranslate[16], adfRotate[16];

/* normalize oVPN - in the Foley book, this is now Rz */
	adfRz[0] = dfVPNX;
	adfRz[1] = dfVPNY;
	adfRz[2] = dfVPNZ;
	adfRz[3] = 0;
    Normalize(adfRz);
	
/* I really have to PROJECT VUP before I do anything, but HOW? */	
	adfRy[0] = dfVUPX;
	adfRy[1] = dfVUPY;
	adfRy[2] = dfVUPZ;
	adfRy[3] = 0;
    Normalize(adfRy);

	Cross(adfRy, adfRz, adfRx);
    Normalize(adfRx);

    ComputeTranslationMatrix(adfTranslate,-dfVRPX,-dfVRPY,-dfVRPZ);

	adfRotate[0] = adfRx[0];
	adfRotate[1] = adfRx[1];
	adfRotate[2] = adfRx[2];
	adfRotate[3] = 0;

	adfRotate[4] = adfRy[0];
	adfRotate[5] = adfRy[1];
	adfRotate[6] = adfRy[2];
	adfRotate[7] = 0;

	adfRotate[8] = adfRz[0];
	adfRotate[9] = adfRz[1];
	adfRotate[10] = adfRz[2];
	adfRotate[11] = 0;

	adfRotate[12] = 0;
	adfRotate[13] = 0;
	adfRotate[14] = 0;
	adfRotate[15] = 1;
 
    MatMulMat(adfRotate, adfTranslate, pView->adfW2E);

	pView->bW2RDirty = TRUE;
}

void DefineViewMapping(p_GPView pView, p_GPViewMap pVM)
{
    double dfDOPX = ((pVM->dfUMax + pVM->dfUMin) / 2) - pVM->dfPRPU;
    double dfDOPY = ((pVM->dfVMax + pVM->dfVMin) / 2) - pVM->dfPRPV;
    double dfDOPZ = pVM->dfVPD - pVM->dfPRPN;

    double adfM[16], adfShear[16], adfCube[16];
	double adfScale[16], adfPRPTrans[16];

	double dfNegPRPN, dfXScale, dfYScale, dfZScale, dfZMin;

	memmove(&pView->sVM, pVM, sizeof(GPViewMap));

    adfShear[0] = 1.0;
    adfShear[1] = 0.0;
    adfShear[2] = -(dfDOPX / dfDOPZ);
    adfShear[3] = 0.0;

    adfShear[4] = 0.0;
    adfShear[5] = 1.0;
    adfShear[6] = -(dfDOPY / dfDOPZ);
    adfShear[7] = 0.0;

    adfShear[8] = 0.0;
    adfShear[9] = 0.0;
    adfShear[10] = 1.0;
    adfShear[11] = 0.0;

    adfShear[12] = 0.0;
    adfShear[13] = 0.0;
    adfShear[14] = 0.0;
    adfShear[15] = 1.0;

    if(pVM->nProjType == GP_PARALLEL)
    {
        /* maps to the NPC 3d viewport coordinates */
        MakeBoxMappingMatrix(
            adfCube,
            pVM->dfUMin, pVM->dfUMax,
            pVM->dfVMin, pVM->dfVMax,
            pVM->dfBPD, pVM->dfFPD,
			0, 1, 0, 1, 0, 1);

		MatMulMat(adfCube,adfShear,pView->adfE2N);
    }
    else
    {
        ComputeTranslationMatrix(adfPRPTrans,-pVM->dfPRPU,-pVM->dfPRPV,-pVM->dfPRPN);

        dfNegPRPN = -pVM->dfPRPN;

        dfXScale = (2 * dfNegPRPN) /
            ((pVM->dfUMax - pVM->dfUMin) * (dfNegPRPN + pVM->dfBPD));

        dfYScale = (2 * dfNegPRPN) /
            ((pVM->dfVMax - pVM->dfVMin) * (dfNegPRPN + pVM->dfBPD));

        dfZScale = -1 / (dfNegPRPN + pVM->dfBPD);

        ComputeScalingMatrix(adfScale, dfXScale, dfYScale, dfZScale);

        /* oM is to convert the canonical view volume into  
		the canonical box */
        dfZMin = -(dfNegPRPN + pVM->dfFPD)/(dfNegPRPN + pVM->dfBPD);

        adfM[0] = 1;
        adfM[1] = 0;
        adfM[2] = 0;
        adfM[3] = 0;

        adfM[4] = 0;
        adfM[5] = 1;
        adfM[6] = 0;
        adfM[7] = 0;

        adfM[8] = 0;
        adfM[9] = 0;
        adfM[10] = 1.0 / (1.0 + dfZMin);
        adfM[11] = -dfZMin / (1.0 + dfZMin);

        adfM[12] = 0;
        adfM[13] = 0;
        adfM[14] = -1;
        adfM[15] = 0;
        
        /* make the matrix to transform the canonical box 
		into the 3d NPC cube */

        MakeBoxMappingMatrix(adfCube, -1, 1, -1, 1, -1, 0,
			0, 1, 0, 1, 0, 1);

		MatMulMat(adfShear, adfPRPTrans, pView->adfE2N);
		MatMulMat(adfScale, pView->adfE2N, pView->adfE2N);
		MatMulMat(adfM, pView->adfE2N, pView->adfE2N);
		MatMulMat(adfCube, pView->adfE2N, pView->adfE2N);
    }

 	pView->bW2RDirty = TRUE;
	pView->bE2RDirty = TRUE;
}

void DefineViewport(p_GPView pView, gpInt32 nXOff, gpInt32 nYOff, 
					gpInt32 nXSize, gpInt32 nYSize)
{
	/* NPC to raster matrix */
	pView->adfN2R[0] = nXSize;
	pView->adfN2R[1] = 0;
	pView->adfN2R[2] = 0;
	pView->adfN2R[3] = nXOff;

	pView->adfN2R[4] = 0;
	pView->adfN2R[5] = -nYSize;
	pView->adfN2R[6] = 0;
	pView->adfN2R[7] = nYOff + nYSize - 1;

	pView->adfN2R[8] = 0;
	pView->adfN2R[9] = 0;
	pView->adfN2R[10] = 1;
	pView->adfN2R[11] = 0;

	pView->adfN2R[12] = 0;
	pView->adfN2R[13] = 0;
	pView->adfN2R[14] = 0;
	pView->adfN2R[15] = 1;

	pView->bW2RDirty = TRUE;
	pView->bE2RDirty = TRUE;

	/* for the clipper */
	pView->nX1 = nXOff;
	pView->nY1 = nYOff;
	pView->nX2 = nXOff + nXSize;
	pView->nY2 = nYOff + nYSize;
}

GPViewMap GetViewMap(p_GPView pView)
{
	return pView->sVM;
}

void GetViewpoint(p_GPView pView, double adfViewpoint[])
{
	adfViewpoint[0] = pView->sVM.dfPRPU;
	adfViewpoint[1] = pView->sVM.dfPRPV;
	adfViewpoint[2] = pView->sVM.dfPRPN;
	adfViewpoint[3] = 1.0;
}

void World2Raster(p_GPView pView, p_GPVertex pVertex)
{
	if(pView->bW2RDirty)
	{
		MatMulMat(pView->adfE2N,pView->adfW2E,pView->adfW2R);
		MatMulMat(pView->adfN2R,pView->adfW2R,pView->adfW2R);
		pView->bW2RDirty = FALSE;
	}

	MatMulVec(pView->adfW2R,pVertex->adfTransform,pVertex->adfTransform);
}

void World2Eye(p_GPView pView, p_GPVertex pV)
{
	/* I assume here that the world to eye matrix is never dirty */
	MatMulVec(pView->adfW2E,pV->adfTransform,pV->adfTransform);
}

void Eye2Raster(p_GPView pView, p_GPVertex pV)
{
	if(pView->bE2RDirty)
	{
		MatMulMat(pView->adfN2R,pView->adfE2N,pView->adfE2R);
		pView->bE2RDirty = FALSE;
	}
	
	MatMulVec(pView->adfE2R,pV->adfTransform,pV->adfTransform);
}

void Normal2Eye(p_GPView pView, p_GPVertex pVertex)
{
	/* I assume here that the world to eye matrix is never dirty */
	MatMulVec(pView->adfW2E,pVertex->adfNormTrans,pVertex->adfNormTrans);
}

void Light2Eye(p_GPView pView,p_GPLight pLight)
{
	/* I assume here that the world to eye matrix is never dirty */
	MatMulVec(pView->adfW2E,pLight->adfTrans,pLight->adfTrans);
}