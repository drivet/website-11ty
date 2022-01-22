#include "raster.h"
#include <stdio.h>

void main()
{
    int c,i,j;
    GPVertex sV0,sV1,sV2;
    p_GPRaster pRaster;
    
    pRaster = CreateGPRasterizer(5, 5);
    ClearAllBuffers(pRaster);
    
    sV0.nX = 0;
    sV0.nY = 0;
    
    sV1.nX = 4;
    sV1.nY = 2;

    sV2.nX = 2;
    sV2.nY = 4;

	printf("Press...\n");
	c = getchar();
    for (i = 0; i < 100000; i++)
	{
		LOW_TriFlat(pRaster,&sV0,&sV1,&sV2);
	}
	printf("Done.\n");
	c = getchar();

/*
	LOW_TriFlat(pRaster,&sV0,&sV1,&sV2);
    for (i = 0; i < pRaster->nYSize; i++)
    {
        for(j = 0; j < pRaster->nXSize; j++)
        {
            c = pRaster->pRBuffer[i*pRaster->nXSize+j];
            if(c)
            {
                printf("*");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
*/
}