#include "clip.h"
#include "algebra.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

p_GPClipper CreateGPClipper()
{
	GPClipper *pClipper = (GPClipper*)malloc(sizeof(GPClipper));
    pClipper->dfXMin = 0.0;
    pClipper->dfXMax = 1.0; 

    pClipper->dfYMin = 0.0;
    pClipper->dfYMax = 1.0;

    pClipper->dfZMin = 0.0;
    pClipper->dfZMax = 1.0;

    pClipper->byFlags = 0;

	return (p_GPClipper)pClipper;
}

void SetXLimits(p_GPClipper pClipper, double dfXMin, double dfXMax)
{
    pClipper->dfXMin = dfXMin;
    pClipper->dfXMax = dfXMax;
}

void SetYLimits(p_GPClipper pClipper, double dfYMin, double dfYMax)
{
    pClipper->dfYMin = dfYMin;
    pClipper->dfYMax = dfYMax;
}

void SetZLimits(p_GPClipper pClipper, double dfZMin, double dfZMax)
{
    pClipper->dfZMin = dfZMin;
    pClipper->dfZMax = dfZMax;
}

void GetXLimits(p_GPClipper pClipper, double* pdfXMin, double* pdfXMax)
{
    *pdfXMin = pClipper->dfXMin;
    *pdfXMax = pClipper->dfXMax;
}

void GetYLimits(p_GPClipper pClipper, double* pdfYMin, double* pdfYMax)
{
    *pdfYMin = pClipper->dfYMin;
    *pdfYMax = pClipper->dfYMax;
}

void GetZLimits(p_GPClipper pClipper, double* pdfZMin, double* pdfZMax)
{
    *pdfZMin = pClipper->dfZMin;
    *pdfZMax = pClipper->dfZMax;
}

void SetClipFlags(p_GPClipper pClipper, gpByte byFlags)
{
	pClipper->byFlags = byFlags;
}

void ClipLine3d(p_GPClipper pClipper,
				p_GPVertex pV0, p_GPVertex pV1,
			    p_GPVertex pVN0, p_GPVertex pVN1,
				gpBool *pbAccept)
{
	p_GPVertex pVOut;
	double dfScale;

	ComputeOutCode(pClipper,pV0);
	ComputeOutCode(pClipper,pV1);
	
	/* duplicate input vertices into the output ones and then
	can change the output vertices to our heart's content */
	memcpy(pVN0,pV0,sizeof(GPVertex));
	memcpy(pVN1,pV1,sizeof(GPVertex));

	do
	{
		/* trivial accept - could take a few tries */
		if (pVN0->byOutCode == 0 && pVN1->byOutCode == 0)
		{
			*pbAccept = TRUE;
			return;
		}
		/* trivial reject - could take a few tries */
		else if (pVN0->byOutCode & pVN1->byOutCode)
		{
			*pbAccept = FALSE;
			return;
		}
		/* more tests */
		else
		{
			/* at least one endpoint is outside the clip 
			rectangle. Pick it*/
			pVOut = pVN0->byOutCode ? pVN0 : pVN1;

			/* find intersection point */
			if (GP_TOP & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfYMax - pVN0->adfTransform[1]) /
					(pVN1->adfTransform[1] - pVN0->adfTransform[1]);

				pVOut->adfTransform[0] = pVN0->adfTransform[0] + 
					(pVN1->adfTransform[0] - pVN0->adfTransform[0])*dfScale;
				
				pVOut->adfTransform[1] = pClipper->dfYMax;

				pVOut->adfTransform[2] = pVN0->adfTransform[2] + 
					(pVN1->adfTransform[2] - pVN0->adfTransform[2])*dfScale;
			}
			else if (GP_BOTTOM & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfYMin - pVN0->adfTransform[1]) /
					(pVN1->adfTransform[1] - pVN0->adfTransform[1]);

				pVOut->adfTransform[0] = pVN0->adfTransform[0] + 
					(pVN1->adfTransform[0] - pVN0->adfTransform[0])*dfScale;
				
				pVOut->adfTransform[1] = pClipper->dfYMin;

				pVOut->adfTransform[2] = pVN0->adfTransform[2] + 
					(pVN1->adfTransform[2] - pVN0->adfTransform[2])*dfScale;
			}
			else if (GP_RIGHT & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfXMax - pVN0->adfTransform[0]) /
					(pVN1->adfTransform[0] - pVN0->adfTransform[0]);

				pVOut->adfTransform[0] = pClipper->dfXMax;

				pVOut->adfTransform[1] = pVN0->adfTransform[1] + 
					(pVN1->adfTransform[1] - pVN0->adfTransform[1])*dfScale;

				pVOut->adfTransform[2] = pVN0->adfTransform[2] + 
					(pVN1->adfTransform[2] - pVN0->adfTransform[2])*dfScale;
			}
			else if (GP_LEFT & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfXMin - pVN0->adfTransform[0]) /
					(pVN1->adfTransform[0] - pVN0->adfTransform[0]);

				pVOut->adfTransform[0] = pClipper->dfXMin;

				pVOut->adfTransform[1] = pVN0->adfTransform[1] + 
					(pVN1->adfTransform[1] - pVN0->adfTransform[1])*dfScale;

				pVOut->adfTransform[2] = pVN0->adfTransform[2] + 
					(pVN1->adfTransform[2] - pVN0->adfTransform[2])*dfScale;
			}
			else if (GP_FRONT & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfZMax - pVN0->adfTransform[2]) /
					(pVN1->adfTransform[2] - pVN0->adfTransform[2]);

				pVOut->adfTransform[0] = pVN0->adfTransform[0] + 
					(pVN1->adfTransform[0] - pVN0->adfTransform[0])*dfScale;

				pVOut->adfTransform[1] = pVN0->adfTransform[1] + 
					(pVN1->adfTransform[1] - pVN0->adfTransform[1])*dfScale;

				pVOut->adfTransform[2] = pClipper->dfZMax;
			}
			else if (GP_BACK & pVOut->byOutCode)
			{
				dfScale = (pClipper->dfZMin - pVN0->adfTransform[2]) /
					(pVN1->adfTransform[2] - pVN0->adfTransform[2]);

				pVOut->adfTransform[0] = pVN0->adfTransform[0] + 
					(pVN1->adfTransform[0] - pVN0->adfTransform[0])*dfScale;

				pVOut->adfTransform[1] = pVN0->adfTransform[1] + 
					(pVN1->adfTransform[1] - pVN0->adfTransform[1])*dfScale;

				pVOut->adfTransform[2] = pClipper->dfZMin;
			}

			ComputeOutCode(pClipper,pVOut);
		}

	}while(TRUE);
}

void Clip3dQuad(p_GPClipper pClipper,p_GPVertex pV0,p_GPVertex pV1,
				p_GPVertex pV2,p_GPVertex pV3,p_GPVPList pOutList)
{
	GPVPList sInList;
	InitializeGPVPList(&sInList);

	AppendGPVertex(&sInList,pV0);
	AppendGPVertex(&sInList,pV1);
	AppendGPVertex(&sInList,pV2);
	AppendGPVertex(&sInList,pV3);

	Clip3dPolygon(pClipper,&sInList,pOutList);
}

void Clip3dTriangle(p_GPClipper pClipper,p_GPVertex pV0,p_GPVertex pV1,
					p_GPVertex pV2,p_GPVPList pOutList)
{
	GPVPList sInList;
	InitializeGPVPList(&sInList);

	AppendGPVertex(&sInList,pV0);
	AppendGPVertex(&sInList,pV1);
	AppendGPVertex(&sInList,pV2);

	Clip3dPolygon(pClipper,&sInList,pOutList);
}

void Clip3dPolygon(p_GPClipper pClipper,p_GPVPList pInList,p_GPVPList pOutList)
{
	GPVPList sOutList1, sOutList2, sOutList3;	
	GPVPList sOutList4, sOutList5;
	
	InitializeGPVPList(&sOutList1);
	InitializeGPVPList(&sOutList2);
	InitializeGPVPList(&sOutList3);
	InitializeGPVPList(&sOutList4);
	InitializeGPVPList(&sOutList5);
    
	Clip3dPolygonAgainstXMin(pClipper,pInList,&sOutList1);

	if ( sOutList1.nCount == 0 ) return;

    Clip3dPolygonAgainstXMax(pClipper,&sOutList1,&sOutList2);

	if ( sOutList2.nCount == 0 ) return;

    Clip3dPolygonAgainstYMin(pClipper,&sOutList2,&sOutList3);

	if ( sOutList3.nCount == 0 ) return;

    Clip3dPolygonAgainstYMax(pClipper,&sOutList3,&sOutList4);

	if ( sOutList4.nCount == 0 ) return;

    Clip3dPolygonAgainstZMin(pClipper,&sOutList4,&sOutList5);

	if ( sOutList5.nCount == 0 ) return;

    Clip3dPolygonAgainstZMax(pClipper,&sOutList5,pOutList);
}

/* back end functions */
void ComputeOutCode(p_GPClipper pClipper, p_GPVertex pV)
{
	pV->byOutCode = 0;

	if (pV->adfTransform[0] < pClipper->dfXMin)
	{
		pV->byOutCode |= GP_LEFT;
	}
	else if (pV->adfTransform[0] > pClipper->dfXMax)
	{
		pV->byOutCode |= GP_RIGHT;
	}

	if (pV->adfTransform[1] < pClipper->dfYMin)
	{
		pV->byOutCode |= GP_BOTTOM;
	}
	else if (pV->adfTransform[1] > pClipper->dfYMax)
	{
		pV->byOutCode |= GP_TOP;
	}

	if (pV->adfTransform[2] < pClipper->dfZMin)
	{
		pV->byOutCode |= GP_BACK;
	}
	else if (pV->adfTransform[2] > pClipper->dfZMax)
	{
		pV->byOutCode |= GP_FRONT;
	}
}

void Clip3dPolygonAgainstXMin(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList)
{
	int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
        pP = pInList->pArray[i];

        if(pP->adfTransform[0] > pClipper->dfXMin)
        {
            if(pS->adfTransform[0] > pClipper->dfXMin)
            {
                AppendGPVertex(pOutList, pP);
            }
            else
            {
                pI = IntersectX(pClipper,pS,pP,pClipper->dfXMin);
                AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[0] > pClipper->dfXMin)
        {
            pI = IntersectX(pClipper,pS,pP,pClipper->dfXMin);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

void Clip3dPolygonAgainstXMax(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList)
{
	int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
        pP = pInList->pArray[i];

        if(pP->adfTransform[0] < pClipper->dfXMax)
        {
            if(pS->adfTransform[0] < pClipper->dfXMax)
            {
                AppendGPVertex(pOutList, pP);
            }
            else
            {
                pI = IntersectX(pClipper,pS,pP,pClipper->dfXMax);
                AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[0] < pClipper->dfXMax)
        {
            pI = IntersectX(pClipper,pS,pP,pClipper->dfXMax);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

void Clip3dPolygonAgainstYMin(p_GPClipper pClipper,
							  p_GPVPList pInList,
							  p_GPVPList pOutList)
{
    int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
        pP = pInList->pArray[i];

        if(pP->adfTransform[1] > pClipper->dfYMin)
        {
            if(pS->adfTransform[1] > pClipper->dfYMin)
            {
                AppendGPVertex(pOutList, pP);
            }
            else
            {
                pI = IntersectY(pClipper,pS,pP,pClipper->dfYMin);
				AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[1] > pClipper->dfYMin)
        {
            pI = IntersectY(pClipper,pS,pP,pClipper->dfYMin);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

void Clip3dPolygonAgainstYMax(p_GPClipper pClipper,
					          p_GPVPList pInList,
							  p_GPVPList pOutList)
{
    int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
        pP = pInList->pArray[i];

        if(pP->adfTransform[1] < pClipper->dfYMax)
        {
            if(pS->adfTransform[1] < pClipper->dfYMax)
            {
                AppendGPVertex(pOutList, pP);
            }
            else
            {
                pI = IntersectY(pClipper,pS,pP,pClipper->dfYMax);
                AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[1] < pClipper->dfYMax)
        {
            pI = IntersectY(pClipper,pS,pP,pClipper->dfYMax);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

void Clip3dPolygonAgainstZMin(p_GPClipper pClipper,
					          p_GPVPList pInList,
							  p_GPVPList pOutList)
{
	int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
		pP = pInList->pArray[i];

        if(pP->adfTransform[2] > pClipper->dfZMin)
        {
            if(pS->adfTransform[2] > pClipper->dfZMin)
            {
                AppendGPVertex(pOutList, pP);
            }
            else
            {
                pI = IntersectZ(pClipper,pS,pP,pClipper->dfZMin);
				AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[2] > pClipper->dfZMin)
        {
            pI = IntersectZ(pClipper,pS,pP,pClipper->dfZMin);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

void Clip3dPolygonAgainstZMax(p_GPClipper pClipper,
					          p_GPVPList pInList,
							  p_GPVPList pOutList)
{
	int i;
    p_GPVertex pS, pP, pI;

    pS = pInList->pArray[pInList->nCount-1];
    
    for(i = 0; i < pInList->nCount; i++)
    {
        pP = pInList->pArray[i];

        if(pP->adfTransform[2] < pClipper->dfZMax)
        {
            if(pS->adfTransform[2] < pClipper->dfZMax)
            {
				AppendGPVertex(pOutList, pP); 
            }
            else
            {
                pI = IntersectZ(pClipper,pS,pP,pClipper->dfZMax);
                AppendGPVertex(pOutList, pI);
                AppendGPVertex(pOutList, pP);
            }
        }
        else if (pS->adfTransform[2] < pClipper->dfZMax)
        {
            pI = IntersectZ(pClipper,pS,pP,pClipper->dfZMax);
            AppendGPVertex(pOutList, pI);
        }

        pS = pP;
    }
}

p_GPVertex IntersectX(p_GPClipper pClipper,p_GPVertex pV1,p_GPVertex pV2,double dfX)
{
    p_GPVertex pI = (p_GPVertex)malloc(sizeof(GPVertex));
    
    double dfT = ((dfX - pV1->adfTransform[0])/
		(pV2->adfTransform[0] - pV1->adfTransform[0]));

    pI->adfTransform[0] = dfX;
    pI->adfTransform[1] =
        (pV2->adfTransform[1]-pV1->adfTransform[1]) * dfT + pV1->adfTransform[1];
    pI->adfTransform[2] =
        (pV2->adfTransform[2]-pV1->adfTransform[2]) * dfT + pV1->adfTransform[2];
    pI->adfTransform[3] = 1.0;

    ClipAttributes(pClipper,pI,pV1,pV2,dfT);

    return pI;
}

p_GPVertex IntersectY(p_GPClipper pClipper,p_GPVertex pV1,p_GPVertex pV2,double dfY)
{
    p_GPVertex pI = (p_GPVertex)malloc(sizeof(GPVertex));

    double dfT = ((dfY - pV1->adfTransform[1])/
		(pV2->adfTransform[1]-pV1->adfTransform[1]));

    pI->adfTransform[0] =
        (pV2->adfTransform[0]-pV1->adfTransform[0]) * dfT + pV1->adfTransform[0];
    pI->adfTransform[1] = dfY;
    pI->adfTransform[2] =
        (pV2->adfTransform[2]-pV1->adfTransform[2]) * dfT + pV1->adfTransform[2];
    pI->adfTransform[3] = 1.0;

    ClipAttributes(pClipper,pI,pV1,pV2,dfT);

    return pI;
}

p_GPVertex IntersectZ(p_GPClipper pClipper,p_GPVertex pV1,p_GPVertex pV2,double dfZ)
{
    p_GPVertex pI = (p_GPVertex)malloc(sizeof(GPVertex));

    double dfT = ((dfZ-pV1->adfTransform[2])/
		(pV2->adfTransform[2]-pV1->adfTransform[2]));

    pI->adfTransform[0] =
        (pV2->adfTransform[0]-pV1->adfTransform[0]) * dfT + pV1->adfTransform[0];
    pI->adfTransform[1] =
        (pV2->adfTransform[1]-pV1->adfTransform[1]) * dfT + pV1->adfTransform[1] ;
    pI->adfTransform[2] = dfZ;
    pI->adfTransform[3] = 1.0;

    ClipAttributes(pClipper,pI,pV1,pV2,dfT);

    return pI;
}

void ClipAttributes(p_GPClipper pClipper, p_GPVertex pI, 
					p_GPVertex pV1, p_GPVertex pV2, double dfT)
{
    if (pClipper->byFlags & GP_I_NORMALS)
    {
        pI->adfNormTrans[0] =
            (pV2->adfNormTrans[0] - pV1->adfNormTrans[0]) * dfT + pV1->adfNormTrans[0];
        pI->adfNormTrans[1] =
            (pV2->adfNormTrans[1] - pV1->adfNormTrans[1]) * dfT + pV1->adfNormTrans[1];
        pI->adfNormTrans[2] =
            (pV2->adfNormTrans[2] - pV1->adfNormTrans[2]) * dfT + pV1->adfNormTrans[2];
		pI->adfNormTrans[3];
    }

    if (pClipper->byFlags & GP_I_COLORS)
    {
        pI->byR = (gpByte)((pV2->byR - pV1->byR) * dfT + pV1->byR);
        pI->byG = (gpByte)((pV2->byG - pV1->byG) * dfT + pV1->byG);
        pI->byB = (gpByte)((pV2->byB - pV1->byB) * dfT + pV1->byB);
    }

    if (pClipper->byFlags & GP_I_MATERIAL)
    {
		pI->sMaterial.dfDR =
                (pV2->sMaterial.dfDR-pV1->sMaterial.dfDR)*dfT+
                (pV1->sMaterial.dfDR);
        pI->sMaterial.dfDG =
                (pV2->sMaterial.dfDG-pV1->sMaterial.dfDG)*dfT+
                (pV1->sMaterial.dfDG);
        pI->sMaterial.dfDB =
                (pV2->sMaterial.dfDB-pV1->sMaterial.dfDB)*dfT+
                (pV1->sMaterial.dfDB);
		
		pI->sMaterial.dfSR =
                (pV2->sMaterial.dfSR-pV1->sMaterial.dfSR)*dfT+
                (pV1->sMaterial.dfSR);
        pI->sMaterial.dfSG =
                (pV2->sMaterial.dfSG-pV1->sMaterial.dfSG)*dfT+
                (pV1->sMaterial.dfSG);
        pI->sMaterial.dfSB =
                (pV2->sMaterial.dfSB-pV1->sMaterial.dfSB)*dfT+
                (pV1->sMaterial.dfSB);

        pI->sMaterial.dfKa =
                (pV2->sMaterial.dfKa-pV1->sMaterial.dfKa)*dfT+
                (pV1->sMaterial.dfKa);
        pI->sMaterial.dfKd =
                (pV2->sMaterial.dfKd-pV1->sMaterial.dfKd)*dfT+
                (pV1->sMaterial.dfKd);
        pI->sMaterial.dfKs =
                (pV2->sMaterial.dfKs-pV1->sMaterial.dfKs)*dfT+
                (pV1->sMaterial.dfKs);

		pI->sMaterial.dfSE =
                (pV2->sMaterial.dfSE-pV1->sMaterial.dfSE)*dfT+
                (pV1->sMaterial.dfSE);       
    }
}
