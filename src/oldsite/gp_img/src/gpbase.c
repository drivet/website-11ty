/* LIST STUFF */

#include "gpbase.h"
#include <stdlib.h>

void AppendGPVertex(p_GPVPList pList, p_GPVertex pV)
{
	pList->nCount++;
	pList->pArray = (p_GPVertex*)realloc(pList->pArray,
		pList->nCount * sizeof(p_GPVertex));
	pList->pArray[pList->nCount-1] = pV;
}

void InitializeGPVPList(p_GPVPList pList)
{
	pList->nCount = 0;
	pList->pArray = NULL;
}

void EmptyGPVPList(p_GPVPList pList)
{
	pList->nCount = 0;
	
	if(pList->pArray)
	{
		free(pList->pArray);
	}
	
	pList->pArray = NULL;
}