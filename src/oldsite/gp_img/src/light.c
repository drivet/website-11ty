#include "light.h"
#include "algebra.h"
#include <stdlib.h>
#include <math.h>
#include <memory.h>

double fAtt(double dfDis)
{
	double dfValue;
	double dfC1,dfC2,dfC3;

	dfC1 = 1.0;
	dfC2 = 0.0;
	dfC3 = 0.0;

	dfValue = 1.0 / (dfC1 + dfC2*dfDis + dfC3*dfDis*dfDis);
	return dfValue < 1 ? dfValue : 1;
}

/* create an initilized lighting module */
p_GPLighting CreateGPLighting()
{
	p_GPLighting pLighting = (p_GPLighting)malloc(sizeof(GPLighting));
	
	memset(pLighting->asLightList,0,sizeof(GPLight)*MAXLIGHTS);

	pLighting->dfAR = 0;
	pLighting->dfAG = 0;
	pLighting->dfAB = 0;

	pLighting->fnAtt = fAtt;

	return pLighting;
}

/* I suspect this function will be used most often for Gouraud shading
where each vertex has to be lit */
void LightVertexL(p_GPLighting pLighting,double adfViewpoint[],p_GPVPList pVList)
{
	int i;
	for(i = 0; i < pVList->nCount; i++)
	{
		LightVertex(pLighting,adfViewpoint,pVList->pArray[i]);
	}
}

void LightVertex(p_GPLighting pLighting,double adfViewpoint[],p_GPVertex pVertex)
{
	ComputeIntensities(pLighting,adfViewpoint,pVertex->adfTransform,
		pVertex->adfNormTrans,&pVertex->sMaterial,
		&pVertex->byR,&pVertex->byG,&pVertex->byB);
}

void ComputeIntensities(p_GPLighting pLighting, double adfViewpoint[],
                        double adfPosition[],double adfNormal[],
                        GPMaterial* pMaterial, 
				        gpByte *pbyR, gpByte *pbyG, gpByte *pbyB)
{
	int i;
	double NdotL, RdotV, dfLightDis, dfR, dfG, dfB;
	double adfV[4], adfL[4], adfN[4], adfTemp[4];
	p_GPLight pLight;
	
	dfR = pLighting->dfAR*pMaterial->dfKa*pMaterial->dfDR;
	dfG = pLighting->dfAG*pMaterial->dfKa*pMaterial->dfDG;
	dfB = pLighting->dfAB*pMaterial->dfKa*pMaterial->dfDB; 

	SubHVector(adfViewpoint,adfPosition,adfV);
	Normalize(adfV);

	memcpy(adfN, adfNormal, sizeof( double ) * 4);
	Normalize(adfN);

	for(i = 0; i < MAXLIGHTS; i++)
	{
		pLight = &pLighting->asLightList[i];

		if(!pLight->bEnabled) continue;

		/* if directional */
		if( pLight->adfTrans[3] == 0 )
		{
			memcpy(adfL, pLight->adfTrans, sizeof( double ) * 4 );
			Normalize(adfL);

			NdotL = fabs( Dot( adfN, adfL ) );

			ScalarVecMul(2 * NdotL, adfN, adfTemp);
			SubHVector(adfTemp, adfL, adfTemp);
			Normalize(adfTemp);
			RdotV = fabs( Dot(adfTemp, adfV) );
					
			/* no attenuation factor.  The light is so far away that
			the attenuation would be about the same for all surfaces
			anyway, I think, so get rid of it. */

			dfR += pLight->dfR * (pMaterial->dfKd * 
				pMaterial->dfDR * NdotL + pMaterial->dfKs * 
				pMaterial->dfSR * pow(RdotV, pMaterial->dfSE));

			dfG += pLight->dfG * (pMaterial->dfKd * 
				pMaterial->dfDG * NdotL + pMaterial->dfKs * 
				pMaterial->dfSG * pow(RdotV, pMaterial->dfSE));

			dfB += pLight->dfB * (pMaterial->dfKd * 
				pMaterial->dfDB * NdotL + pMaterial->dfKs * 
				pMaterial->dfSB * pow(RdotV, pMaterial->dfSE));
		}
		else
		{
			SubHVector( pLight->adfTrans, adfPosition, adfL);
			dfLightDis = Mag(adfL);
			Normalize(adfL);

			NdotL = fabs( Dot( adfN, adfL ) );
			ScalarVecMul(2 * NdotL, adfN, adfTemp);
			SubHVector(adfTemp, adfL, adfTemp);
			RdotV = fabs( Dot( adfTemp, adfV) );

			dfR += (*pLighting->fnAtt)(dfLightDis) * pLight->dfR * 
				(pMaterial->dfKd * pMaterial->dfDR * NdotL + pMaterial->dfKs * 
				pMaterial->dfSR * pow(RdotV, pMaterial->dfSE));
			
			dfG += (*pLighting->fnAtt)(dfLightDis) * pLight->dfG * 
				(pMaterial->dfKd * pMaterial->dfDG * NdotL + pMaterial->dfKs * 
				pMaterial->dfSG * pow(RdotV, pMaterial->dfSE));
			
			dfB += (*pLighting->fnAtt)(dfLightDis) * pLight->dfB * 
				(pMaterial->dfKd * pMaterial->dfDB * NdotL + pMaterial->dfKs * 
				pMaterial->dfSB * pow(RdotV, pMaterial->dfSE));
		}
	}

	if ( dfR > 1.0) dfR = 1.0;
	if ( dfG > 1.0) dfG = 1.0;
	if ( dfB > 1.0) dfB = 1.0;

	*pbyR = (gpByte)(255.0 * dfR);
	*pbyG = (gpByte)(255.0 * dfG);
	*pbyB = (gpByte)(255.0 * dfB);
}

