#ifndef _ALGEBRA_H
#define _ALGEBRA_H

#include "gpbase.h"

void ZeroVec(double adfV[]);
double Mag(double adfV[]);
double *UnitVec(double adfV[]
void Homogenize(double adfV[]);
void Normalize(double adfV[]);
double *UnitVec(double adfV[],double adfR[]);

/* adds the 4-dimensional vector adfV1 and adfV2 and stores the result 
in adfS, and then returns adfS */
double *AddHVector(double adfV1[], double adfV2[], double adfS[]);

/* suntracts the 4-dimensional vector adfV2 from adfV2 and stores the result 
in adfD, and then returns adfS */
double *SubHVector(double adfV1[], double adfV2[], double adfD[]);

/* multiplies the 4-D vector adfV by dfC and stores result in adfR. 
Returns adfR */
double *ScalarVecMul(double dfC, double adfV[], double adfR[]);

/* divides the 4-D vector adfV by dfC and stores result in adfR. 
Returns adfR */

double *ScalarVecDiv(double dfC, double adfV[], double adfR[]);
    
/* stores the cross product of adfV1 and adfV2 in adfR and return adfR.
The vectors are 4-D but only the first 3 elements are used */
double *Cross(double adfV1[], double adfV2[], double adfR[]);
    
/* returns dot product of the two 4-D vectors */
double Dot(double adfV1[], double adfV2[]);

/* fills the 4x4 matrix with identity values */
void Identity(double adfM[]);

/* fills the 4x4 matrix with zero values */
void ZeroMat(double adfM[]);

/* adds two 4x4 matrices.  Stores sum in adfR and returns adfR */
double *AddHMatrix(double adfM1[],double adfM2[],double adfR[]);

/* subtracts two 4x4 matrices.  Stores diff in adfR and returns adfR */
double *SubHMatrix(double adfM1[], double adfM2[], double adfR[]);

/* multiplies two matrices */
double *MatMulMat(double adfM1[], double adfM2[], double adfR[]);

/* multiplies the 4-D vector adfV by the 16-element (4x4)  matrix adfM 
and stores the result in adfR.  Returns adfR */ 
double *MatMulVec(double adfM[], double adfV[], double adfR[]);

/* scalar matrix multiply and div */
double *ScalarMatMul(double dfC, double adfM1[], double adfR[]);
double *ScalarMatDiv(double dfC, double adfM1[], double adfR[]);

/* transformation matrices */
void ComputeTranslationMatrix(double adfM[],double dfX,double dfY,double dfZ);

void ComputeRotationMatrix(double adfM[],double dfA,
				   double dfX, double dfY, double dfZ);

void ComputeScalingMatrix(double adfM[],double dfSX,double dfSY,double dfSZ);

void ComputeRigidMatrixXY(double adfM[],double dfX0,double dfY0,double dfZ0,
    			          double dfX1,double dfX2,double dfX3,
				          double dfY1,double dfY2,double dfY3,
				          gpBool bInverse);

void ComputeRigidMatrixYZ(double adfM[],double dfX0,double dfY0,double dfZ0,
				          double dfY1,double dfY2,double dfY3,
			              double dfZ1,double dfZ2,double dfZ3,
				          gpBool bInverse);

void ComputeRigidMatrixZX(double adfM[],double dfX0,double dfY0,double dfZ0,
				          double dfZ1,double dfZ2,double dfZ3,
				          double dfX1,double dfX2,double dfX3,
			              gpBool bInverse);
    
void ComputeRigidMatrix(double adfM[],double dfX0, double dfY0, double dfZ0,
						double dfXA1,double dfXA2,double dfXA3, 
						double dfYA1,double dfYA2,double dfYA3, 
						double dfZA1,double dfZA2,double dfZA3, 
					    gpBool bInverse);

void MakeBoxMappingMatrix(double adfM[],double dfUMin, double dfUMax,
                          double dfVMin, double dfVMax,
                          double dfNMin, double dfNMax,
                          double dfXMin, double dfXMax,
                          double dfYMin, double dfYMax,
                          double dfZMin, double dfZMax);
#endif
