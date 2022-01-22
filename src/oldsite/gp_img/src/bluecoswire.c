#include <windows.h>
#include <math.h>
#include "pipe.h"
#include "algebra.h"

#define XSIZE 512
#define YSIZE 512

#define PI 3.141592654

p_GPPipe pPipe;
gpByte *pRBuffer,*pGBuffer,*pBBuffer;

//prototype declarations

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR pszCmdLine,
				   int nCmdShow);

LRESULT CALLBACK WindowProc(HWND hWnd,
							UINT uMsgId,
							WPARAM wParam,
							LPARAM lParam);

typedef struct
{
	double dfX;
	double dfY;
	double dfZ;
} Vertex;

double f(double dfX, double dfY)
{
	return 1.0*cos(sqrt(dfX*dfX + dfY*dfY));
}

void DrawSurface(Vertex *pdfMesh,Vertex *pdfNormals,int nXV,int nYV)
{
	int i,j,n0,n1,n2,n3,nXF,nYF;
	GPMaterial sMat;
	sMat.dfDR = 0;
	sMat.dfDG = 0;
	sMat.dfDB = 1;
		
	sMat.dfSR = 1;
	sMat.dfSG = 1;
	sMat.dfSB = 1;
		
	sMat.dfKa = 0.3;
	sMat.dfKd = 0.5;
	sMat.dfKs = 1.0;
		
	sMat.dfSE = 10;

	/* pick the shading model */
	gpSetShading(GP_GOURAUD);

	/* pick the rendering model */
	gpSetRendering(GP_FILLED);

	/* Enable or disable lighting */
	gpEnableLighting(TRUE);

	/* Enable or disable depth testing */
	gpEnableDepth(TRUE);

	/* add ambient light to the scene */
	gpSetAmbientLight(1, 1, 1);

	/* add a diffuse light to scene */
	gpLightEnable(0,TRUE);
	gpSetLightPos(0, 1,0,0, 0);
	gpSetLightColor(0,1,1,1);

	gpBegin(GP_TRIANGLES);

	gpMaterial(&sMat);
	
	nXF = nXV-1;
	nYF = nYV-1;

	for(i = 0; i < nYF; i++)
	{
		for(j = 0; j < nXF; j++)
		{
			n0 = i * nXV + j;
			n1 = (i+1) * nXV + j;
			n2 = (i+1) * nXV + (j+1);
			n3 = i * nXV + (j+1);

			/* first triangle of current facet */
			gpNormal(pdfNormals[n0].dfX,pdfNormals[n0].dfY,pdfNormals[n0].dfZ);
			gpVertex(
				pdfMesh[n0].dfX,
				pdfMesh[n0].dfY,
				pdfMesh[n0].dfZ);

			gpNormal(pdfNormals[n1].dfX,pdfNormals[n1].dfY,pdfNormals[n1].dfZ);
			gpVertex(
				pdfMesh[n1].dfX,
				pdfMesh[n1].dfY,
				pdfMesh[n1].dfZ);

			gpNormal(pdfNormals[n2].dfX,pdfNormals[n2].dfY,pdfNormals[n2].dfZ);
			gpVertex(
				pdfMesh[n2].dfX,
				pdfMesh[n2].dfY,
				pdfMesh[n2].dfZ);

			/* second triangle of current facet */
			gpNormal(pdfNormals[n0].dfX,pdfNormals[n0].dfY,pdfNormals[n0].dfZ);
			gpVertex(
				pdfMesh[n0].dfX,
				pdfMesh[n0].dfY,
				pdfMesh[n0].dfZ);

			gpNormal(pdfNormals[n2].dfX,pdfNormals[n2].dfY,pdfNormals[n2].dfZ);
			gpVertex(
				pdfMesh[n2].dfX,
				pdfMesh[n2].dfY,
				pdfMesh[n2].dfZ);

			gpNormal(pdfNormals[n3].dfX,pdfNormals[n3].dfY,pdfNormals[n3].dfZ);
			gpVertex(
				pdfMesh[n3].dfX,
				pdfMesh[n3].dfY,
				pdfMesh[n3].dfZ);
		}
	}
	gpEnd();

#ifdef notdef
	/* pick the shading model */
	gpSetShading(GP_FLAT);

	/* pick the rendering model */
	gpSetRendering(GP_WIRE);

	/* Enable or disable lighting */
	gpEnableLighting(FALSE);

	gpBegin(GP_TRIANGLES);

	gpColor(0,1,0);
	
	nXF = nXV-1;
	nYF = nYV-1;

	for(i = 0; i < nYF; i++)
	{
		for(j = nXF/2; j < nXF; j++)
		{
			n0 = i * nXV + j;
			n1 = (i+1) * nXV + j;
			n2 = (i+1) * nXV + (j+1);
			n3 = i * nXV + (j+1);

			/* first triangle of current facet */
			gpNormal(pdfNormals[n0].dfX,pdfNormals[n0].dfY,pdfNormals[n0].dfZ);
			gpVertex(
				pdfMesh[n0].dfX,
				pdfMesh[n0].dfY,
				pdfMesh[n0].dfZ);

			gpNormal(pdfNormals[n1].dfX,pdfNormals[n1].dfY,pdfNormals[n1].dfZ);
			gpVertex(
				pdfMesh[n1].dfX,
				pdfMesh[n1].dfY,
				pdfMesh[n1].dfZ);

			gpNormal(pdfNormals[n2].dfX,pdfNormals[n2].dfY,pdfNormals[n2].dfZ);
			gpVertex(
				pdfMesh[n2].dfX,
				pdfMesh[n2].dfY,
				pdfMesh[n2].dfZ);

			/* second triangle of current facet */
			gpNormal(pdfNormals[n0].dfX,pdfNormals[n0].dfY,pdfNormals[n0].dfZ);
			gpVertex(
				pdfMesh[n0].dfX,
				pdfMesh[n0].dfY,
				pdfMesh[n0].dfZ);

			gpNormal(pdfNormals[n2].dfX,pdfNormals[n2].dfY,pdfNormals[n2].dfZ);
			gpVertex(
				pdfMesh[n2].dfX,
				pdfMesh[n2].dfY,
				pdfMesh[n2].dfZ);

			gpNormal(pdfNormals[n3].dfX,pdfNormals[n3].dfY,pdfNormals[n3].dfZ);
			gpVertex(
				pdfMesh[n3].dfX,
				pdfMesh[n3].dfY,
				pdfMesh[n3].dfZ);
		}
	}
	gpEnd();
#endif

	gpSetShading(GP_GOURAUD);
	gpEnableLighting(FALSE);
	gpEnableDepth(TRUE);

	/* draw the axes */
	gpBegin(GP_LINES);

	/* x-axis */
	gpColor(1,0,0);
	gpVertex(0,0,0);
	gpColor(0,0,1);
	gpVertex(10,0,0);

	/* y-axis */
	gpColor(1,1,1);
	gpVertex(0,0,0);
	gpColor(1,0,0);
	gpVertex(0,10,0);

	/* z-axis */
	gpColor(1,1,0);
	gpVertex(0,0,0);
	gpColor(0,1,0);
	gpVertex(0,0,10);

	gpEnd();
}

//WinMain -- this is where execution begins
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR pszCmdLine,
				   int nCmdShow)
{
	/* Windows stuff */
	static char szAppName[] = "Desmond's Window";
	HWND hWnd;
	MSG msg;
	WNDCLASS wndClass;
	int nFrameX,nFrameY,nCaption;
	/* End Windows stuff */

	GPViewMap sVM;
	double dfA1,dfA2,dfScale;
	Vertex *pdfMesh;
	Vertex *pdfNormals;
	int *pnWeights;
	int nXF,nYF,nXV,nYV,nTotalV,i,j;
	int n0,n1,n2,n3,nWeight;
	double dfXRange,dfYRange, dfXInc,dfYInc;
	double dfX1,dfX2,dfY1,dfY2;
	double dfX,dfY,dfZ;
	double adfVec1[4], adfVec2[4];
	double adfNormal1[4],adfNormal2[4];

	pPipe = CreateGPPipe(512,512);
	gpSetCurrentPipe(pPipe);

	gpClearAllBuffers();

	dfA1 = 320;
	dfA2 = 320;

	dfScale = (2 * PI) / 360.0;
	dfA1 = dfScale * dfA1;
	dfA2 = dfScale * dfA2;

	gpDefineVRC( 0, 0, 0, 
		cos(dfA2)*sin(dfA1), sin(dfA2), cos(dfA2)*cos(dfA1), 
	   -sin(dfA1)*sin(dfA1), cos(dfA2),-sin(dfA1)*cos(dfA1));

	/* all of this is in eye coordinates */
    sVM.nProjType = GP_PERSPECTIVE;
    sVM.dfPRPU = 0;
    sVM.dfPRPV = 0;
    sVM.dfPRPN = 30.0;
    
    sVM.dfVPD =  20; 
	sVM.dfBPD = -20; 
	sVM.dfFPD =  20;
        
    sVM.dfUMin = -10;
	sVM.dfUMax =  10; 
    sVM.dfVMin = -10; 
	sVM.dfVMax =  10;
    gpDefineViewMapping(&sVM);

	/* define the viewport - in this case the whole window */
	gpDefineViewport(0,0,512,512);

	nXF = 40;
	nYF = 40;

	nXV = nXF + 1;
	nYV = nYF + 1;

	nTotalV = nXV * nYV;

	dfX1 = -2 * PI;
	dfX2 =  2 * PI;
	dfY1 = -2 * PI;
	dfY2 =  2 * PI;

	dfXRange = dfX2 - dfX1;
	dfYRange = dfY2 - dfY1;

	dfXInc = dfXRange / (double)nXF;
	dfYInc = dfYRange / (double)nYF;

	pdfMesh = (Vertex *)malloc(sizeof(Vertex) * nXV * nYV);
	for(i = 0; i < nYV; i++)
	{
		dfY = dfY1 + i * dfYInc;
	
		for(j = 0; j < nXV; j++)
		{
			dfX = dfX1 + j * dfXInc;
			dfZ = f(dfX, dfY);
		
			pdfMesh[i * nXV + j].dfX = dfX;
			pdfMesh[i * nXV + j].dfY = dfY;
			pdfMesh[i * nXV + j].dfZ = dfZ;
		}
	}

	pdfNormals = (Vertex *)calloc(nXV * nYV, sizeof(Vertex));
	pnWeights = (int *)calloc(nXV * nYV, sizeof(int)); 
	
	for(i = 0; i < nYF; i++)
	{
		for(j = 0; j < nXF; j++)
		{
			n0 = i * nXV + j;
			n1 = (i+1) * nXV + j;
			n2 = (i+1) * nXV + (j+1);
			n3 = i * nXV + (j+1);

			/* calculate the first triangle normal */
			adfVec1[0] = pdfMesh[n2].dfX - pdfMesh[n1].dfX;
			adfVec1[1] = pdfMesh[n2].dfY - pdfMesh[n1].dfY;
			adfVec1[2] = pdfMesh[n2].dfZ - pdfMesh[n1].dfZ;
			adfVec1[3] = 0.0;

			adfVec2[0] = pdfMesh[n1].dfX - pdfMesh[n0].dfX;
			adfVec2[1] = pdfMesh[n1].dfY - pdfMesh[n0].dfY;
			adfVec2[2] = pdfMesh[n1].dfZ - pdfMesh[n0].dfZ;
			adfVec2[3] = 0.0;

			Cross(adfVec1,adfVec2,adfNormal1);
			Normalize(adfNormal1);

			/* calculate the second triangle normal */
			adfVec1[0] = pdfMesh[n2].dfX - pdfMesh[n3].dfX;
			adfVec1[1] = pdfMesh[n2].dfY - pdfMesh[n3].dfY;
			adfVec1[2] = pdfMesh[n2].dfZ - pdfMesh[n3].dfZ;
			adfVec1[3] = 0.0;

			adfVec2[0] = pdfMesh[n3].dfX - pdfMesh[n0].dfX;
			adfVec2[1] = pdfMesh[n3].dfY - pdfMesh[n0].dfY;
			adfVec2[2] = pdfMesh[n3].dfZ - pdfMesh[n0].dfZ;
			adfVec2[3] = 0.0;

			Cross(adfVec2,adfVec1,adfNormal2);
			Normalize(adfNormal2);
		
			/* do the first triangle */

			/* normal for vertex 0 */
			pnWeights[n0]++;
			nWeight = pnWeights[n0];

			pdfNormals[n0].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfX + (1.0/(double)nWeight) * adfNormal1[0];

			pdfNormals[n0].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfY + (1.0/(double)nWeight) * adfNormal1[1];
	
			pdfNormals[n0].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfZ + (1.0/(double)nWeight) * adfNormal1[2];

			/* normal for vertex 1 */
			pnWeights[n1]++;
			nWeight = pnWeights[n1];

			pdfNormals[n1].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n1].dfX + (1.0/(double)nWeight) * adfNormal1[0];

			pdfNormals[n1].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n1].dfY + (1.0/(double)nWeight) * adfNormal1[1];
	
			pdfNormals[n1].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n1].dfZ + (1.0/(double)nWeight) * adfNormal1[2];

			/* normal for vertex 2 */
			pnWeights[n2]++;
			nWeight = pnWeights[n2];

			pdfNormals[n2].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfX + (1.0/(double)nWeight) * adfNormal1[0];

			pdfNormals[n2].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfY + (1.0/(double)nWeight) * adfNormal1[1];
	
			pdfNormals[n2].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfZ + (1.0/(double)nWeight) * adfNormal1[2];
			
			/* do second triangle */

			/* normal for vertex 0 */
			pnWeights[n0]++;
			nWeight = pnWeights[n0];

			pdfNormals[n0].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfX + (1.0/(double)nWeight) * adfNormal2[0];

			pdfNormals[n0].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfY + (1.0/(double)nWeight) * adfNormal2[1];
	
			pdfNormals[n0].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n0].dfZ + (1.0/(double)nWeight) * adfNormal2[2];

			/* normal for vertex 2 */
			pnWeights[n2]++;
			nWeight = pnWeights[n2];

			pdfNormals[n2].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfX + (1.0/(double)nWeight) * adfNormal2[0];

			pdfNormals[n2].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfY + (1.0/(double)nWeight) * adfNormal2[1];
	
			pdfNormals[n2].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n2].dfZ + (1.0/(double)nWeight) * adfNormal2[2];

			/* normal for vertex 3 */
			pnWeights[n3]++;
			nWeight = pnWeights[n3];

			pdfNormals[n3].dfX = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n3].dfX + (1.0/(double)nWeight) * adfNormal2[0];

			pdfNormals[n3].dfY = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n3].dfY + (1.0/(double)nWeight) * adfNormal2[1];
	
			pdfNormals[n3].dfZ = (((double)nWeight-1.0)/((double)nWeight))*
				pdfNormals[n3].dfZ + (1.0/(double)nWeight) * adfNormal2[2];
		}
	}

	for(i = 0; i < nTotalV; i++)
	{
		adfNormal1[0] = pdfNormals[i].dfX;
		adfNormal1[1] = pdfNormals[i].dfY;
		adfNormal1[2] = pdfNormals[i].dfZ;
		adfNormal1[3] = 0.0;
		
		Normalize(adfNormal1);

		pdfNormals[i].dfX = adfNormal1[0];
		pdfNormals[i].dfY = adfNormal1[1];
		pdfNormals[i].dfZ = adfNormal1[2];
	}

	DrawSurface(pdfMesh,pdfNormals,nXV,nYV);

	//free(pdfMesh);
	free(pdfNormals);
	free(pnWeights);

	//first register the window class
	wndClass.style       = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WindowProc;  //callback fn
	wndClass.cbClsExtra  = 0;
	wndClass.cbWndExtra  = 0;
	wndClass.hInstance   = hInstance;
	wndClass.hIcon       = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor     = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName  = NULL;
	wndClass.lpszClassName = szAppName;
	if (RegisterClass(&wndClass) == 0)
	{
		return 0;
	}

	nFrameX = GetSystemMetrics(SM_CXSIZEFRAME);
	nFrameY = GetSystemMetrics(SM_CYSIZEFRAME);
	nCaption = GetSystemMetrics(SM_CYCAPTION);

	//now create the app's one window
	hWnd = CreateWindow(
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		XSIZE + 2 * nFrameX,
		YSIZE + 2 * nFrameY + nCaption,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hWnd == 0)
	{
		return 0;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


//WindowProc -- this function gets called to process Window messages

LRESULT CALLBACK WindowProc(HWND hWnd,		//handle of window
							UINT uMsgId,	//message identifier
							WPARAM wParam,	//first message parameter
							LPARAM lParam)	//second message parameter
{
	static int nCount = 0;
	
	int i,j;
	HDC hDC;
	PAINTSTRUCT paintStruct;

	//decide what to do with each different message type
	switch(uMsgId)
	{
		//handle the window paint message by displaying 
		//the string "Hello World in the upper-left corner (coord 0,0) of the window
	case WM_PAINT:
		pRBuffer = gpGetRedBuffer(pPipe);
		pGBuffer = gpGetGreenBuffer(pPipe);
		pBBuffer = gpGetBlueBuffer(pPipe);

		InvalidateRgn(hWnd,0,TRUE);
		hDC = BeginPaint(hWnd, &paintStruct);

		for (i = 0; i < 512; i++)
		{
			for (j = 0; j < 512; j++)
			{
				SetPixelV(hDC, j, i, RGB(
					pRBuffer[i*512+j],
					pGBuffer[i*512+j],
					pBBuffer[i*512+j])
					); 
			}
		}
		
		EndPaint(hWnd, &paintStruct);
		
		return 0;
		
	//handle the window destroy message by quitiong the application
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	default:
		//let windows handle all other message types
		return DefWindowProc(hWnd,uMsgId,wParam,lParam);
	}
}

