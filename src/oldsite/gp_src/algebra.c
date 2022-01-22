#include "algebra.h"
#include <math.h>
#include <memory.h>

void ZeroVec(double adfV[])
{
	adfV[0] = 0;
	adfV[1] = 0;
	adfV[2] = 0;
	adfV[3] = 0;
}

double Mag(double adfV[])
{
	return sqrt(adfV[0]*adfV[0]+adfV[1]*adfV[1]+adfV[2]*adfV[2]+adfV[3]*adfV[3]);
}
    
void Homogenize(double adfV[])
{
	if (adfV[3] != 1 && adfV[3] != 0)
	{
		adfV[0] = adfV[0]/adfV[3];
		adfV[1] = adfV[1]/adfV[3];
		adfV[2] = adfV[2]/adfV[3];
		adfV[3] = 1.0;
	}
}

void Normalize(double adfV[])
{
	double dfMag = Mag(adfV);
	adfV[0] = adfV[0]/dfMag;
	adfV[1] = adfV[1]/dfMag;
	adfV[2] = adfV[2]/dfMag;
	adfV[3] = adfV[3]/dfMag;
}

double *UnitVec(double adfV[], double adfR[])
{
	double dfMag = Mag(adfV);
	adfR[0] = adfV[0]/dfMag;
	adfR[1] = adfV[1]/dfMag;
	adfR[2] = adfV[2]/dfMag;
	adfR[3] = adfV[3]/dfMag;

	return adfR;
}

/* adds the 4-dimensional vector adfV1 and adfV2 and stores the result 
in adfS, and then returns adfS */
double *AddHVector(double adfV1[], double adfV2[], double adfS[])
{
	adfS[0] = adfV1[0] + adfV2[0];
    adfS[1] = adfV1[1] + adfV2[1];
    adfS[2] = adfV1[2] + adfV2[2];
    adfS[3] = adfV1[3] + adfV2[3];

    return adfS;
}

/* suntracts the 4-dimensional vector adfV2 from adfV2 and stores the result 
in adfD, and then returns adfS */
double *SubHVector(double adfV1[], double adfV2[], double adfD[])
{
	adfD[0] = adfV1[0] - adfV2[0];
    adfD[1] = adfV1[1] - adfV2[1];
    adfD[2] = adfV1[2] - adfV2[2];
    adfD[3] = adfV1[3] - adfV2[3];

    return adfD;
}

/* multiplies the 4-D vector adfV by dfC and stores result in adfR. 
Returns adfR */
double *ScalarVecMul(double dfC, double adfV[], double adfR[])
{
	adfR[0] = adfV[0] * dfC;
    adfR[1] = adfV[1] * dfC;
    adfR[2] = adfV[2] * dfC;
    adfR[3] = adfV[3] * dfC;

    return adfR;
}

/* divides the 4-D vector adfV by dfC and stores result in adfR. 
Returns adfR */
double *ScalarVecDiv(double dfC, double adfV[], double adfR[])
{
	adfR[0] = adfV[0] / dfC;
    adfR[1] = adfV[1] / dfC;
    adfR[2] = adfV[2] / dfC;
    adfR[3] = adfV[3] / dfC;

    return adfR;
}
    
/* stores the cross product of adfV1 and adfV2 in adfR and return adfR.
The vectors are 4-D but only the first 3 elements are used */
double *Cross(double adfV1[], double adfV2[], double adfR[])
{
	adfR[0] = adfV1[1] * adfV2[2] - adfV1[2] * adfV2[1];
    adfR[1] = adfV1[2] * adfV2[0] - adfV1[0] * adfV2[2];
    adfR[2] = adfV1[0] * adfV2[1] - adfV1[1] * adfV2[0];
	adfR[3] = 0.0;

    return adfR;
}
    
/* returns dot product of the two 4-D vectors */
double Dot(double adfV1[], double adfV2[])
{
	double dfDotProduct = 0.0;

    dfDotProduct += adfV1[0] * adfV2[0];
    dfDotProduct += adfV1[1] * adfV2[1];
    dfDotProduct += adfV1[2] * adfV2[2];
	dfDotProduct += adfV1[3] * adfV2[3];

    return dfDotProduct;
}

/* fills the 4x4 matrix with identity values */
void Identity(double adfM[])
{
    adfM[0]  = 1.0;
    adfM[1]  = 0.0;
    adfM[2]  = 0.0;
    adfM[3]  = 0.0;

    adfM[4]  = 0.0;
    adfM[5]  = 1.0;
    adfM[6]  = 0.0;
    adfM[7]  = 0.0;

    adfM[8]  = 0.0;
    adfM[9]  = 0.0;
    adfM[10] = 1.0;
    adfM[11] = 0.0;

    adfM[12] = 0.0;
    adfM[13] = 0.0;
    adfM[14] = 0.0;
    adfM[15] = 1.0;
}

/* fills the 4x4 matrix with zero values */
void ZeroMat(double adfM[])
{
	adfM[0]  = 0.0;
    adfM[1]  = 0.0;
    adfM[2]  = 0.0;
    adfM[3]  = 0.0;

    adfM[4]  = 0.0;
    adfM[5]  = 0.0;
    adfM[6]  = 0.0;
    adfM[7]  = 0.0;

    adfM[8]  = 0.0;
    adfM[9]  = 0.0;
    adfM[10] = 0.0;
    adfM[11] = 0.0;

    adfM[12] = 0.0;
    adfM[13] = 0.0;
    adfM[14] = 0.0;
    adfM[15] = 0.0;
}

/* adds two 4x4 matrices.  Stores sum in adfR and returns adfR */
double *AddHMatrix(double adfM1[],double adfM2[],double adfR[])
{
	adfR[0]  = adfM1[0]  + adfM2[0];
    adfR[1]  = adfM1[1]  + adfM2[1];
    adfR[2]  = adfM1[2]  + adfM2[2];
    adfR[3]  = adfM1[3]  + adfM2[3];

    adfR[4]  = adfM1[4]  + adfM2[4];
    adfR[5]  = adfM1[5]  + adfM2[5];
    adfR[6]  = adfM1[6]  + adfM2[6];
    adfR[7]  = adfM1[7]  + adfM2[7];

    adfR[8]  = adfM1[8]  + adfM2[8];
    adfR[9]  = adfM1[9]  + adfM2[9];
    adfR[10] = adfM1[10] + adfM2[10];
    adfR[11] = adfM1[11] + adfM2[11];

    adfR[12] = adfM1[12] + adfM2[12];
    adfR[13] = adfM1[13] + adfM2[13];
    adfR[14] = adfM1[14] + adfM2[14];
    adfR[15] = adfM1[15] + adfM2[15];

    return adfR;
}

/* subtracts two 4x4 matrices.  Stores diff in adfR and returns adfR */
double *SubHMatrix(double adfM1[], double adfM2[], double adfR[])
{
	adfR[0]  = adfM1[0]  - adfM2[0];
    adfR[1]  = adfM1[1]  - adfM2[1];
    adfR[2]  = adfM1[2]  - adfM2[2];
    adfR[3]  = adfM1[3]  - adfM2[3];

    adfR[4]  = adfM1[4]  - adfM2[4];
    adfR[5]  = adfM1[5]  - adfM2[5];
    adfR[6]  = adfM1[6]  - adfM2[6];
    adfR[7]  = adfM1[7]  - adfM2[7];

    adfR[8]  = adfM1[8]  - adfM2[8];
    adfR[9]  = adfM1[9]  - adfM2[9];
    adfR[10] = adfM1[10] - adfM2[10];
    adfR[11] = adfM1[11] - adfM2[11];

    adfR[12] = adfM1[12] - adfM2[12];
    adfR[13] = adfM1[13] - adfM2[13];
    adfR[14] = adfM1[14] - adfM2[14];
    adfR[15] = adfM1[15] - adfM2[15];

    return adfR;
}

/* multiplies two matrices */
double *MatMulMat(double adfM1[], double adfM2[], double adfR[])
{
	double adfMC1[16];
	double adfMC2[16];

	memcpy(adfMC1,adfM1,16*sizeof(double));
	memcpy(adfMC2,adfM2,16*sizeof(double));

	adfR[0]  =
        adfMC1[0] * adfMC2[0] +
        adfMC1[1] * adfMC2[4] +
        adfMC1[2] * adfMC2[8] +
        adfMC1[3] * adfMC2[12];

    adfR[1]  =
        adfMC1[0] * adfMC2[1] +
        adfMC1[1] * adfMC2[5] +
        adfMC1[2] * adfMC2[9] +
        adfMC1[3] * adfMC2[13];

    adfR[2]  =
        adfMC1[0] * adfMC2[2] +
        adfMC1[1] * adfMC2[6] +
        adfMC1[2] * adfMC2[10] +
        adfMC1[3] * adfMC2[14];

    adfR[3]  =
        adfMC1[0] * adfMC2[3] +
        adfMC1[1] * adfMC2[7] +
        adfMC1[2] * adfMC2[11] +
        adfMC1[3] * adfMC2[15];

    adfR[4]  =
        adfMC1[4] * adfMC2[0] +
        adfMC1[5] * adfMC2[4] +
        adfMC1[6] * adfMC2[8] +
        adfMC1[7] * adfMC2[12];

    adfR[5]  =
        adfMC1[4] * adfMC2[1] +
        adfMC1[5] * adfMC2[5] +
        adfMC1[6] * adfMC2[9] +
        adfMC1[7] * adfMC2[13];

    adfR[6]  =
        adfMC1[4] * adfMC2[2] +
        adfMC1[5] * adfMC2[6] +
        adfMC1[6] * adfMC2[10] +
        adfMC1[7] * adfMC2[14];

    adfR[7]  =
        adfMC1[4] * adfMC2[3] +
        adfMC1[5] * adfMC2[7] +
        adfMC1[6] * adfMC2[11] +
        adfMC1[7] * adfMC2[15];

    adfR[8]  =
        adfMC1[8]  * adfMC2[0] +
        adfMC1[9]  * adfMC2[4] +
        adfMC1[10] * adfMC2[8] +
        adfMC1[11] * adfMC2[12];

    adfR[9]  =
        adfMC1[8]  * adfMC2[1] +
        adfMC1[9]  * adfMC2[5] +
        adfMC1[10] * adfMC2[9] +
        adfMC1[11] * adfMC2[13];

    adfR[10] =
        adfMC1[8]  * adfMC2[2] +
        adfMC1[9]  * adfMC2[6] +
        adfMC1[10] * adfMC2[10] +
        adfMC1[11] * adfMC2[14];

    adfR[11] =
        adfMC1[8]  * adfMC2[3] +
        adfMC1[9]  * adfMC2[7] +
        adfMC1[10] * adfMC2[11] +
        adfMC1[11] * adfMC2[15];

    adfR[12] =
        adfMC1[12] * adfMC2[0] +
        adfMC1[13] * adfMC2[4] +
        adfMC1[14] * adfMC2[8] +
        adfMC1[15] * adfMC2[12];

    adfR[13] =
        adfMC1[12] * adfMC2[1] +
        adfMC1[13] * adfMC2[5] +
        adfMC1[14] * adfMC2[9] +
        adfMC1[15] * adfMC2[13];

    adfR[14] =
        adfMC1[12] * adfMC2[2] +
        adfMC1[13] * adfMC2[6] +
        adfMC1[14] * adfMC2[10] +
        adfMC1[15] * adfMC2[14];

    adfR[15] =
        adfMC1[12] * adfMC2[3] +
        adfMC1[13] * adfMC2[7] +
        adfMC1[14] * adfMC2[11] +
        adfMC1[15] * adfMC2[15];

    return adfR;
}

/* multiplies the 4-D vector adfV by the 16-element (4x4)  matrix adfM 
and stores the result in adfR.  Returns adfR */ 
double *MatMulVec(double adfM[], double adfV[], double adfR[])
{
	double adfVC[4];
	memcpy(adfVC,adfV,4*sizeof(double));

	adfR[0] =
        adfM[0] * adfVC[0] +
        adfM[1] * adfVC[1] +
        adfM[2] * adfVC[2] +
        adfM[3] * adfVC[3];

    adfR[1] =
        adfM[4] * adfVC[0] +
        adfM[5] * adfVC[1] +
        adfM[6] * adfVC[2] +
        adfM[7] * adfVC[3];

    adfR[2] =
        adfM[8]  * adfVC[0] +
        adfM[9]  * adfVC[1] +
        adfM[10] * adfVC[2] +
        adfM[11] * adfVC[3];

    adfR[3] =
        adfM[12] * adfVC[0] +
        adfM[13] * adfVC[1] +
        adfM[14] * adfVC[2] +
        adfM[15] * adfVC[3];

    return adfR;
}

/* scalar matrix multiply and div */
double *ScalarMatMul(double dfC, double adfM[], double adfR[])
{
	adfR[0]  = dfC * adfM[0];
    adfR[1]  = dfC * adfM[1];
    adfR[2]  = dfC * adfM[2];
    adfR[3]  = dfC * adfM[3];

    adfR[4]  = dfC * adfM[4];
    adfR[5]  = dfC * adfM[5];
    adfR[6]  = dfC * adfM[6];
    adfR[7]  = dfC * adfM[7];

    adfR[8]  = dfC * adfM[8];
    adfR[9]  = dfC * adfM[9];
    adfR[10] = dfC * adfM[10];
    adfR[11] = dfC * adfM[11];

    adfR[12] = dfC * adfM[12];
    adfR[13] = dfC * adfM[13];
    adfR[14] = dfC * adfM[14];
    adfR[15] = dfC * adfM[15];

    return adfR;
}

double *ScalarMatDiv(double dfC, double adfM[], double adfR[])
{
	adfR[0]  = dfC / adfM[0];
    adfR[1]  = dfC / adfM[1];
    adfR[2]  = dfC / adfM[2];
    adfR[3]  = dfC / adfM[3];

    adfR[4]  = dfC / adfM[4];
    adfR[5]  = dfC / adfM[5];
    adfR[6]  = dfC / adfM[6];
    adfR[7]  = dfC / adfM[7];

    adfR[8]  = dfC / adfM[8];
    adfR[9]  = dfC / adfM[9];
    adfR[10] = dfC / adfM[10];
    adfR[11] = dfC / adfM[11];

    adfR[12] = dfC / adfM[12];
    adfR[13] = dfC / adfM[13];
    adfR[14] = dfC / adfM[14];
    adfR[15] = dfC / adfM[15];

    return adfR;
}

/* transformation matrices */
void ComputeTranslationMatrix(double adfM[],double dfX,double dfY,double dfZ)
{
	adfM[0] = 1.0;
    adfM[1] = 0.0;
    adfM[2] = 0.0;
    adfM[3] = dfX;

    adfM[4] = 0.0;
    adfM[5] = 1.0;
    adfM[6] = 0.0;
    adfM[7] = dfY;

    adfM[8] = 0.0;
    adfM[9] = 0.0;
    adfM[10] = 1.0;
    adfM[11] = dfZ;

    adfM[12] = 0.0;
    adfM[13] = 0.0;
    adfM[14] = 0.0;
    adfM[15] = 1.0;
}

void ComputeRotationMatrix(double adfM[],double dfA, double dfX, 
						   double dfY, double dfZ)
{
	double adfU[3], adfS[9], adfUUT[9];
	double dfSinA, dfCosA, dfMag = sqrt(dfX*dfX + dfY*dfY + dfZ*dfZ);

	/* unit vector in direction of axis specified */
	adfU[0] = dfX / dfMag;
	adfU[1] = dfY / dfMag;
	adfU[2] = dfZ / dfMag;

	/* 3x3 S matrix.  No idea what this does.  
	I just copied it from the OpenGL red book */
	adfS[0] = 0;
	adfS[1] = -adfU[2];
	adfS[2] = adfU[1];

	adfS[3] = adfU[2];
	adfS[4] = 0;
	adfS[5] = -adfU[0];

	adfS[6] = -adfU[1];
	adfS[7] = adfU[0];
	adfS[8] = 0;

	/* 3x3 uuT matrix.  Don't ask.  See OpenGL red book */
	adfUUT[0] = adfU[0] * adfU[0];
	adfUUT[1] = adfU[0] * adfU[1];
	adfUUT[2] = adfU[0] * adfU[2];

	adfUUT[3] = adfU[1] * adfU[0];
	adfUUT[4] = adfU[1] * adfU[1];
	adfUUT[5] = adfU[1] * adfU[2];

	adfUUT[6] = adfU[2] * adfU[0];
	adfUUT[7] = adfU[2] * adfU[1];
	adfUUT[8] = adfU[2] * adfU[2];

	dfSinA = sin(dfA);
	dfCosA = cos(dfA);

	adfM[0] = adfUUT[0] + dfCosA * (1 - adfUUT[0]) + dfSinA * adfS[0];
	adfM[1] = adfUUT[1] - dfCosA * adfUUT[1] + dfSinA * adfS[1];
	adfM[2] = adfUUT[2] - dfCosA * adfUUT[2] + dfSinA * adfS[2];
	adfM[3] = 0;

	adfM[4] = adfUUT[3] - dfCosA * adfUUT[3] + dfSinA * adfS[3];
	adfM[5] = adfUUT[4] + dfCosA * (1 - adfUUT[4]) + dfSinA * adfS[4];
	adfM[6] = adfUUT[5] - dfCosA * adfUUT[5] + dfSinA * adfS[5];
	adfM[7] = 0;

	adfM[8] = adfUUT[6] - dfCosA * adfUUT[6] + dfSinA * adfS[6];
	adfM[9] = adfUUT[7] - dfCosA * adfUUT[7] + dfSinA * adfS[7];
	adfM[10] = adfUUT[8] + dfCosA * (1 - adfUUT[8]) + dfSinA * adfS[8];
	adfM[11] = 0;

	adfM[12] = 0;
	adfM[13] = 0;
	adfM[14] = 0;
	adfM[15] = 1;
}

void ComputeScalingMatrix(double adfM[],double dfSX,double dfSY,double dfSZ)
{
	adfM[0] = dfSX;
    adfM[1] = 0.0;
    adfM[2] = 0.0;
    adfM[3] = 0.0;

    adfM[4] = 0.0;
    adfM[5] = dfSY;
    adfM[6] = 0.0;
    adfM[7] = 0.0;

    adfM[8] = 0.0;
    adfM[9] = 0.0;
    adfM[10] = dfSZ;
    adfM[11] = 0.0;

    adfM[12] = 0.0;
    adfM[13] = 0.0;
    adfM[14] = 0.0;
    adfM[15] = 1.0;
}

void ComputeRigidMatrixXY(double adfM[],double dfX0,double dfY0,double dfZ0,
    			          double dfX1,double dfX2,double dfX3,
				          double dfY1,double dfY2,double dfY3,
				          gpBool bInverse)
{
	double adfRz[4], adfRy[4], adfRx[4];

	adfRx[0] = dfX1;
	adfRx[1] = dfX2;
	adfRx[2] = dfX3;
	adfRx[3] = 0;
	Normalize(adfRx);

	adfRy[0] = dfY1;
	adfRy[1] = dfY2;
	adfRy[2] = dfY3;
	adfRy[3] = 0;
	Normalize(adfRy);

	Cross(adfRx, adfRy, adfRz);
    Normalize(adfRz);

	ComputeRigidMatrix(adfM,dfX0,dfY0,dfZ0,
		adfRx[0], adfRx[1], adfRx[2],
		adfRy[0], adfRy[1], adfRy[2],
		adfRz[0], adfRz[1], adfRz[2],bInverse);
}

void ComputeRigidMatrixYZ(double adfM[],double dfX0,double dfY0,double dfZ0,
				          double dfY1,double dfY2,double dfY3,
			              double dfZ1,double dfZ2,double dfZ3,
				          gpBool bInverse)
{
	double adfRz[4], adfRy[4], adfRx[4];

	adfRy[0] = dfY1;
	adfRy[1] = dfY2;
	adfRy[2] = dfY3;
	adfRy[3] = 0;
	Normalize(adfRy);

	adfRz[0] = dfZ1;
	adfRz[1] = dfZ2;
	adfRz[2] = dfZ3;
	adfRz[3] = 0;
	Normalize(adfRz);

	Cross(adfRy, adfRz, adfRx);
    Normalize(adfRx);

	ComputeRigidMatrix(adfM,dfX0,dfY0,dfZ0,
		adfRx[0], adfRx[1], adfRx[2],
		adfRy[0], adfRy[1], adfRy[2],
		adfRz[0], adfRz[1], adfRz[2],bInverse);
}

void ComputeRigidMatrixZX(double adfM[],double dfX0,double dfY0,double dfZ0,
				          double dfZ1,double dfZ2,double dfZ3,
				          double dfX1,double dfX2,double dfX3,
			              gpBool bInverse)
{
	double adfRz[4], adfRy[4], adfRx[4];

	adfRz[0] = dfZ1;
	adfRz[1] = dfZ2;
	adfRz[2] = dfZ3;
	adfRz[3] = 0;
	Normalize(adfRz);

	adfRx[0] = dfX1;
	adfRx[1] = dfX2;
	adfRx[2] = dfX3;
	adfRx[3] = 0;
	Normalize(adfRx);

	Cross(adfRz, adfRx, adfRy);
    Normalize(adfRy);

	ComputeRigidMatrix(adfM,dfX0,dfY0,dfZ0,
		adfRx[0], adfRx[1], adfRx[2],
		adfRy[0], adfRy[1], adfRy[2],
		adfRz[0], adfRz[1], adfRz[2],bInverse);
}
    
void ComputeRigidMatrix(double adfM[],double dfX0, double dfY0, double dfZ0,
						double dfXA1,double dfXA2,double dfXA3, 
						double dfYA1,double dfYA2,double dfYA3, 
						double dfZA1,double dfZA2,double dfZA3, 
					    gpBool bInverse)
{
	double adfTranslate[16], adfRotate[16];
	if(bInverse)
	{
		/* Notice the reversal of the usual row column 
        subscripting.  I'm dealing with transposes here ! */
		adfM[0] = dfXA1;
	    adfM[4] = dfXA2;
        adfM[8] = dfXA3;
        adfM[12] = 0;

        adfM[1] = dfYA1;
        adfM[5] = dfYA2;
        adfM[9] = dfYA3;
        adfM[13] = 0;

        adfM[2] = dfZA1;
        adfM[6] = dfZA2;
        adfM[10] = dfZA3;
        adfM[14] = 0;

        adfM[3] = dfX0;
        adfM[7] = dfY0;
        adfM[11] = dfZ0;
        adfM[15] = 1;
	}
	else
	{
		ComputeTranslationMatrix(adfTranslate,-dfX0,-dfY0,-dfZ0);
    
        adfRotate[0] = dfXA1;
        adfRotate[1] = dfXA2;
        adfRotate[2] = dfXA3;
        adfRotate[3] = 0;

        adfRotate[4] = dfYA1;
        adfRotate[5] = dfYA2;
        adfRotate[6] = dfYA3;
        adfRotate[7] = 0;

        adfRotate[8] = dfZA1;
        adfRotate[9] = dfZA2;
        adfRotate[10] = dfZA3;
        adfRotate[11] = 0;

        adfRotate[12] = 0;
        adfRotate[13] = 0;
        adfRotate[14] = 0;
        adfRotate[15] = 1;

        MatMulMat(adfRotate, adfTranslate, adfM);
	}
}

void MakeBoxMappingMatrix(double adfM[],double dfUMin, double dfUMax,
                          double dfVMin, double dfVMax,
                          double dfNMin, double dfNMax,
                          double dfXMin, double dfXMax,
                          double dfYMin, double dfYMax,
                          double dfZMin, double dfZMax)
{
	double dfScaleX = (dfXMax - dfXMin) / (dfUMax - dfUMin);
    double dfScaleY = (dfYMax - dfYMin) / (dfVMax - dfVMin);
    double dfScaleZ = (dfZMax - dfZMin) / (dfNMax - dfNMin);

    adfM[0] = dfScaleX;
    adfM[1] = 0.0;
    adfM[2] = 0.0;
    adfM[3] = -(dfScaleX * dfUMin) + dfXMin;

    adfM[4] = 0.0;
    adfM[5] = dfScaleY;
    adfM[6] = 0.0;
    adfM[7] = -(dfScaleY * dfVMin) + dfYMin;

    adfM[8] = 0.0;
    adfM[9] = 0.0;
    adfM[10] = dfScaleZ;
    adfM[11] = -(dfScaleZ * dfNMin) + dfZMin;

    adfM[12] = 0.0;
    adfM[13] = 0.0;
    adfM[14] = 0.0;
    adfM[15] = 1.0;
}
