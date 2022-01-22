#ifndef _BASE_H
#define _BASE_H

/* some basic types and definitions */
#define TRUE  1
#define FALSE 0

typedef char gpBool;
typedef unsigned char gpByte;
typedef long int gpInt32;

/* structure to hold material properties */
typedef struct
{
	double dfDR, dfDG, dfDB;
    double dfSR, dfSG, dfSB;    
    double dfKa, dfKd, dfKs;
    double dfSE;
}GPMaterial;

/* 
vertex structure and vertex list.  The geometric transformation 
on a vertex always works on the adfTransform variable and 
replaces it with the result.
*/

typedef struct 
{
	double adfTransform[4];
	double adfNormTrans[4];
	GPMaterial sMaterial;
	
	gpInt32 nX,nY,nZ;
	gpByte byR,byG,byB;

	gpByte byOutCode;

}GPVertex;

typedef GPVertex *p_GPVertex;

/* vertex list */
typedef struct
{
	int nCount;
	p_GPVertex *pArray;
}GPVPList;

typedef GPVPList *p_GPVPList;

void AppendGPVertex(p_GPVPList pList, p_GPVertex pV);
void InitializeGPVPList(p_GPVPList pList);
void EmptyGPVPList(p_GPVPList pList);

#endif
