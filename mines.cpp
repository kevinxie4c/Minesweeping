#include "stdafx.h"

#pragma once

#include "resource.h"

#define GAMEUNSTART     0
#define GAMESTART		1
#define GAMEOVER	   -1

#define FACEORIG		4
#define FACEWIN			1
#define FACEFAIL		2
#define FACEORIGDOWN	3	
#define FACEFAILDOWN	4
#define FACEWINDOWN		5

#define IDT_LIGHT		1
#define IDT_TIME		2

#define NUMWIDTH		20

#define NUMHRIGHT		40

#define FACELONG		24

#define RCLONG			25

#define EXCU_W			25
#define EXCU_H			25

#define  ORIG			 0
#define  NUM1			 1
#define  NUM2			 2
#define  NUM3			 3
#define  NUM4			 4
#define  NUM5			 5
#define  NUM6			 6
#define  NUM7			 7
#define  NUM8			 8
#define  MINE			 9
#define  MINE_RED		10
#define  MINE_ERR		11
#define  FLAG			12
#define  QUES			13
#define  BLANK			14
#define  ORIGLIGHT		15


HINSTANCE	hinst;											
HWND		hwndMain;										
HBITMAP		hBitmap;										
HBITMAP		hBitmapFace;									
HBITMAP		hBitmapNum;										
INT			iMines[2][18][32];								
INT			iSound      =  FALSE;							
INT			iGameState  =  GAMEUNSTART;						
INT			iFaceState  =  FACEORIG;						
INT			iMineNum	=   10;								
INT			iTimeNum	=    0;								
INT			iMineFlag   =   10;								
INT			iRow		=    9;							
INT			iCol		=    9;								
INT			iWidth		=   iCol * RCLONG + EXCU_W * 2;		
INT			iHeight		=   iRow * RCLONG + EXCU_H * 5;		

bool BuryMines();
bool GenerateNum();
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

VOID CALLBACK OnTimerLight(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	INT		cxClinent, cyClinent;
	HDC		hdc, hdcMen;
	INT		iX, iY, iMX, iMY, ix, iy;
	POINT	point;
	RECT	rect;

	GetWindowRect(hwndMain, &rect);
	GetCursorPos(&point);
	cxClinent = point.x - rect.left;
	cyClinent = point.y - rect.top - 48;
	hdc = GetDC(hwndMain);
	hdcMen = CreateCompatibleDC(hdc);
	SelectObject(hdcMen, hBitmap);
	if (cxClinent >= EXCU_W && cxClinent <= iWidth - (EXCU_W + 2) &&
		cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
		iGameState != GAMEOVER) {
		cxClinent = cxClinent - cxClinent % 25;
		cyClinent = cyClinent - cyClinent % 25;
		ix = (cxClinent - EXCU_W) / 25 + 1;
		iy = (cyClinent - EXCU_H * 2) / 25 + 1;
		if (ORIG == iMines[0][iy][ix]) {
			BitBlt(hdc, cxClinent, cyClinent, EXCU_W, EXCU_H,
				hdcMen, 0, EXCU_H * ORIGLIGHT, SRCCOPY);
			for (iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += EXCU_H, iMY++) {
				for (iX = EXCU_W, iMX = 1; iMX <= iCol; iX += EXCU_W, iMX++) {
					if ((iMY != iy || iMX != ix) && ORIG == iMines[0][iy][ix]) {
						BitBlt(hdc, iX, iY, EXCU_W, EXCU_H,
							hdcMen, 0, EXCU_H * iMines[0][iMY][iMX], SRCCOPY);
					}
				}
			}
		}
		else {
			InvalidateRect(hwndMain, NULL, FALSE);
		}
	}
	else {
		InvalidateRect(hwndMain, NULL, FALSE);
	}
	DeleteDC(hdcMen);
	ReleaseDC(hwndMain, hdc);
}


bool ModifyDifficulty(INT iX, INT iY, INT iNum)
{
	RECT rect;

	iRow = iX;
	iCol = iY;
	iMineNum = iNum;
	iWidth = iCol * RCLONG + EXCU_W * 2;
	iHeight = iRow * RCLONG + EXCU_H * 5;
	GetWindowRect(hwndMain, &rect);
	if (!MoveWindow(hwndMain, rect.left, rect.top, iWidth, iHeight, TRUE)) {
		return FALSE;
	}

	return TRUE;
}

bool GameStart(VOID)
{
	ZeroMemory(iMines, sizeof(iMines));
	BuryMines();
	GenerateNum();
	iGameState = GAMEUNSTART;
	iFaceState = FACEORIG;
	iTimeNum = 0;
	iMineFlag = iMineNum;

	return TRUE;
}


bool BuryMines(VOID)
{
	INT	iX, iY, iNum = 0;

	srand((UINT)GetCurrentTime());
	while (iNum < iMineNum) {
		iY = rand() % (iCol + 1);
		iX = rand() % (iRow + 1);
		if (iX != 0 && iY != 0 && iMines[1][iX][iY] != MINE) {
			iMines[1][iX][iY] = MINE;
			iNum++;
		}
	}

	return TRUE;
}


bool GenerateNum(VOID)
{
	INT	iX, iY, iNum;

	for (iY = 1; iY <= iCol; iY++) {
		for (iX = 1; iX <= iRow; iX++) {
			iNum = 0;
			if (ORIG == iMines[1][iX][iY])	{
				if (iMines[1][iX - 1][iY] == MINE)iNum++;
				if (iMines[1][iX + 1][iY] == MINE)iNum++;
				if (iMines[1][iX][iY - 1] == MINE)iNum++;
				if (iMines[1][iX][iY + 1] == MINE)iNum++;
				if (iMines[1][iX - 1][iY - 1] == MINE)iNum++;
				if (iMines[1][iX + 1][iY + 1] == MINE)iNum++;
				if (iMines[1][iX - 1][iY + 1] == MINE)iNum++;
				if (iMines[1][iX + 1][iY - 1] == MINE)iNum++;
				iMines[1][iX][iY] = iNum;
			}
		}
	}
	for (iY = 1; iY <= iCol; iY++) {
		for (iX = 1; iX <= iRow; iX++) {
			if (ORIG == iMines[1][iX][iY]) {
				iMines[1][iX][iY] = BLANK;
			}
		}
	}

	return TRUE;
}


bool JugdeWin(VOID)
{
	INT iNum = 0, iFlag = 0, iQues = 0, iY, iX;

	for (iX = 1; iX <= iRow; iX++) {
		for (iY = 1; iY <= iCol; iY++) {
			if (ORIG == iMines[0][iX][iY]) {
				iNum++;
			}
			else if (FLAG == iMines[0][iX][iY]) {
				iFlag++;
			}
			else if (QUES == iMines[0][iX][iY]) {
				iQues++;
			}
		}
	}
	if (iMineNum == iNum + iFlag + iQues) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


bool Recursion(INT iy, INT ix)
{
	INT iNum;

	do  {
		iNum = 0;
		if (iMines[1][iy - 1][ix] == BLANK && iMines[0][iy - 1][ix] != BLANK) {
			iMines[0][iy - 1][ix] = iMines[1][iy - 1][ix];
			iNum++;
			Recursion(iy - 1, ix);
		}
		else if (iMines[1][iy - 1][ix] != MINE) {
			iMines[0][iy - 1][ix] = iMines[1][iy - 1][ix];
		}
		if (iMines[1][iy + 1][ix] == BLANK && iMines[0][iy + 1][ix] != BLANK) {
			iMines[0][iy + 1][ix] = iMines[1][iy + 1][ix];
			iNum++;
			Recursion(iy + 1, ix);
		}
		else if (iMines[1][iy + 1][ix] != MINE) {
			iMines[0][iy + 1][ix] = iMines[1][iy + 1][ix];
			iNum++;
		}
		if (iMines[1][iy][ix - 1] == BLANK && iMines[0][iy][ix - 1] != BLANK) {
			iMines[0][iy][ix - 1] = iMines[1][iy][ix - 1];
			iNum++;
			Recursion(iy, ix - 1);
		}
		else if (iMines[1][iy][ix - 1] != MINE) {
			iMines[0][iy][ix - 1] = iMines[1][iy][ix - 1];
			iNum++;
		}
		if (iMines[1][iy][ix + 1] == BLANK && iMines[0][iy][ix + 1] != BLANK) {
			iMines[0][iy][ix + 1] = iMines[1][iy][ix + 1];
			Recursion(iy, ix + 1);
			iNum++;
		}
		else if (iMines[1][iy][ix + 1] != MINE) {
			iMines[0][iy][ix + 1] = iMines[1][iy][ix + 1];
			iNum++;
		}
		if (iMines[1][iy - 1][ix - 1] == BLANK && iMines[0][iy - 1][ix - 1] != BLANK) {
			iMines[0][iy - 1][ix - 1] = iMines[1][iy - 1][ix - 1];
			iNum++;
			Recursion(iy - 1, ix - 1);
		}
		else if (iMines[1][iy - 1][ix - 1] != MINE) {
			iMines[0][iy - 1][ix - 1] = iMines[1][iy - 1][ix - 1];
			iNum++;
		}
		if (iMines[1][iy + 1][ix + 1] == BLANK && iMines[0][iy + 1][ix + 1] != BLANK) {
			iMines[0][iy + 1][ix + 1] = iMines[1][iy + 1][ix + 1];
			iNum++;
			Recursion(iy + 1, ix + 1);
		}
		else if (iMines[1][iy + 1][ix + 1] != MINE) {
			iMines[0][iy + 1][ix + 1] = iMines[1][iy + 1][ix + 1];
			iNum++;
		}
		if (iMines[1][iy - 1][ix + 1] == BLANK && iMines[0][iy - 1][ix + 1] != BLANK) {
			iMines[0][iy - 1][ix + 1] = iMines[1][iy - 1][ix + 1];
			iNum++;
			Recursion(iy - 1, ix + 1);
		}
		else if (iMines[1][iy - 1][ix + 1] != MINE) {
			iMines[0][iy - 1][ix + 1] = iMines[1][iy - 1][ix + 1];
			iNum++;
		}
		if (iMines[1][iy + 1][ix - 1] == BLANK && iMines[0][iy + 1][ix - 1] != BLANK) {
			iMines[0][iy + 1][ix - 1] = iMines[1][iy + 1][ix - 1];
			iNum++;
			Recursion(iy + 1, ix - 1);
		}
		else if (iMines[1][iy + 1][ix - 1] != MINE) {
			iMines[0][iy + 1][ix - 1] = iMines[1][iy + 1][ix - 1];
			iNum++;
		}
	} while (0 == iNum);

	return TRUE;
}


INT CALLBACK UserDef(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR	cHeight[256], cWidth[256], cMine[256];
	INT		iH, iW, iM;

	ZeroMemory(cHeight, sizeof(cHeight));
	ZeroMemory(cWidth, sizeof(cWidth));
	ZeroMemory(cMine, sizeof(cMine));

	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			GetDlgItemText(hwnd, IDC_HEIGHT, cHeight, sizeof(cHeight) / sizeof(TCHAR));
			GetDlgItemText(hwnd, IDC_WIDTH, cWidth, sizeof(cWidth) / sizeof(TCHAR));
			GetDlgItemText(hwnd, IDC_NUM, cMine, sizeof(cMine) / sizeof(TCHAR));
			iH = _wtoi((const wchar_t*)cHeight);
			iW = _wtoi((const wchar_t*)cWidth);
			iM = _wtoi((const wchar_t*)cMine);
			if (iH >= 9 && iH <= 16 &&
				iW >= 9 && iW <= 30 &&
				iM >= 10 && iM <= 99 &&
				iH * iW >= iM - 10) {
				ModifyDifficulty(iH, iW, iM);
				GameStart();
				InvalidateRect(hwndMain, NULL, FALSE);
				EndDialog(hwnd, LOWORD(wParam));
			}
			else {
				MessageBox(hwnd, TEXT("您输入的数值范围有误请重新输入(雷数不得超过宽度*高度-10)"),
					TEXT("警告"), MB_OK | MB_ICONERROR);
			}

			return TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hwnd, LOWORD(wParam));
		}
		break;
	}

	return FALSE;
}



INT CALLBACK About(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			EndDialog(hwnd, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}


LRESULT CALLBACK MenuWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	INT				wmId, wmEvent, choice;
	BITMAP			bitmap, bitmapface;
	static HMENU	hMenu;

	wmId = LOWORD(wParam);
	wmEvent = HIWORD(wParam);

	switch (wmId) {
	case IDM_START:
		if (iGameState == GAMESTART && iGameState != GAMEOVER) {
			choice = MessageBox(hwnd, TEXT("您要放弃本局并开始新的游戏? "),
				TEXT("询问"), MB_OKCANCEL | MB_ICONQUESTION);
			if (IDOK == choice) {
				GameStart();
				InvalidateRect(hwndMain, NULL, FALSE);
			}
		}
		else {
			GameStart();
			InvalidateRect(hwndMain, NULL, FALSE);
		}
		break;
	case IDM_PRIMARY:
		ModifyDifficulty(9, 9, 10);
		GameStart();
		InvalidateRect(hwndMain, NULL, FALSE);
		break;
	case IDM_MIDDLE:
		ModifyDifficulty(16, 16, 40);
		GameStart();
		InvalidateRect(hwndMain, NULL, FALSE);
		break;
	case IDM_ADVAN:
		ModifyDifficulty(16, 30, 99);
		GameStart();
		InvalidateRect(hwndMain, NULL, FALSE);
		break;
	case IDM_USERDEF:
		DialogBox(hinst, MAKEINTRESOURCE(IDD_USERDEF), hwnd, UserDef);
		break;
	case IDM_SOUND:
		hMenu = GetMenu(hwnd);
		if (TRUE == iSound) {
			iSound = FALSE;
			CheckMenuItem(hMenu, IDM_SOUND, MF_UNCHECKED);
		}
		else {
			iSound = TRUE;
			CheckMenuItem(hMenu, IDM_SOUND, MF_CHECKED);
		}
		break;
	case IDM_ABOUT:
		DialogBox(hinst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




INT APIENTRY WinMain(	 HINSTANCE	hInstance,
						 HINSTANCE	hPrevInstance, 
						 LPSTR		lpCmdLine, 
						 INT		iCmdShow  )
{
	WNDCLASSEX	wcx;					
	HWND		hwnd;					
	MSG			msg;					
	BOOL		fGotMessage;			

	hinst = hInstance;				


	wcx.cbSize			=   sizeof(wcx);											
	wcx.style			=   CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;					
	wcx.lpfnWndProc		=   MainWndProc;											
	wcx.cbClsExtra		=   0;														
	wcx.cbWndExtra		=   0;														
	wcx.hInstance		=   hInstance;												
	wcx.hIcon			=   LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));
	wcx.hCursor			=   LoadCursor(NULL, IDC_ARROW);							
	wcx.hbrBackground	=   (HBRUSH)GetStockObject((LTGRAY_BRUSH));					
	wcx.lpszMenuName	=   MAKEINTRESOURCE(IDR_MENUMAIN);							
	wcx.lpszClassName	=   TEXT("MainWClass");										
	wcx.hIconSm			=   (HICON)LoadImage(	hInstance,							
												MAKEINTRESOURCE(IDI_ICONMAIN),
												IMAGE_ICON,
												GetSystemMetrics(SM_CXSMICON),
												GetSystemMetrics(SM_CYSMICON),
												LR_DEFAULTCOLOR);
	if (!RegisterClassEx(&wcx)) {
		MessageBox(NULL, TEXT("本程序仅支持在 Windows NT 下运行"), TEXT("警告"), MB_ICONERROR);
		return 1;
	}
	hwnd = CreateWindow(   TEXT("MainWClass"),								
							TEXT("Mine Sweeper"),								
							(WS_OVERLAPPED | WS_CAPTION | \
							  WS_SYSMENU    | WS_MINIMIZEBOX),				
							300,											
							100,											
							iWidth,											
							iHeight,										
							(HWND) NULL,									
							(HMENU) NULL,									
							hInstance,										
							(LPVOID) NULL	);								

	hwndMain = hwnd;                  
	ShowWindow(hwnd, iCmdShow);     
	UpdateWindow(hwnd);			  
	
	while ((fGotMessage = GetMessage(&msg, (HWND) NULL, 0, 0)) != 0 && fGotMessage != -1) {
		TranslateMessage(&msg);    
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		static INT			cxClinent, cyClinent, cxSource, cySource;	
		static HINSTANCE	hInstance;									
		static HDC			hdc, hdcMen;
		BITMAP				bitmap, bitmapface, bitmapnum;
		PAINTSTRUCT			ps;
		INT					iX, iY,iMX, iMY, ix, iy;
		INT					iMineFlag1, iMineFlag10, iTime1, iTime10, iTime100;	 

	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
		hBitmap  = LoadBitmap(hInstance, MAKEINTRESOURCE (IDB_MINES));
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		cxSource = bitmap.bmWidth;						
		cySource = (bitmap.bmHeight) / 16;				

		hBitmapFace  = LoadBitmap(hInstance, MAKEINTRESOURCE (IDB_FACE));	
		GetObject(hBitmapFace, sizeof(BITMAP), &bitmapface);

		hBitmapNum  = LoadBitmap(hInstance, MAKEINTRESOURCE (IDB_NUM));	
		GetObject(hBitmapNum, sizeof(BITMAP), &bitmapnum);

		GameStart();									
		SetTimer(hwnd, IDT_LIGHT, 30, OnTimerLight);  

		return 0;
	case WM_PAINT:				
		hdc = BeginPaint(hwnd, &ps);
		hdcMen = CreateCompatibleDC(hdc);
		SelectObject(hdcMen, hBitmap);
		
		for (iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++) {
			for (iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++) {
				BitBlt(hdc, iX, iY, cxSource, cySource, hdcMen, 0, cySource * iMines[0][iMY][iMX], SRCCOPY);
			}
		}
		SelectObject(hdcMen, hBitmapFace);

		BitBlt(hdc, iWidth / 2-12, 5, FACELONG, FACELONG, hdcMen, 0, FACELONG * iFaceState, SRCCOPY);
		SelectObject(hdcMen, hBitmapNum);

		if (iMineFlag >= 0) {		
			iMineFlag10 = (INT)(iMineFlag / 10);
			iMineFlag1  = iMineFlag % 10;
			BitBlt(hdc, 30, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * 0, SRCCOPY);
		} else {
			iMineFlag10 = (INT)(-iMineFlag / 10);
			iMineFlag1  = -iMineFlag % 10;
			BitBlt(hdc, 30, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * 10, SRCCOPY);
		}
		BitBlt(hdc, 50, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iMineFlag10, SRCCOPY);
		BitBlt(hdc, 70, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iMineFlag1, SRCCOPY);
		iTime100 = iTimeNum / 100;
		iTime10  = iTimeNum / 10 % 10;
		iTime1   = iTimeNum % 10;
		BitBlt(hdc, iWidth - 90, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime100, SRCCOPY);
		BitBlt(hdc, iWidth - 70, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime10, SRCCOPY);
		BitBlt(hdc, iWidth - 50, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime1, SRCCOPY);
		DeleteDC(hdcMen);
		EndPaint(hwnd, &ps);

		return 0;
	case WM_TIMER:
		if (iTimeNum < 999) {
			iTimeNum++;
		} else {
			KillTimer(hwnd, IDT_TIME);
			iGameState = GAMEOVER;
			iFaceState = FACEFAIL;
			for (iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++) {
				for (iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++) {
					if (MINE == iMines[1][iMY][iMX]) {
						BitBlt(hdc, iX, iY, cxSource, cySource, 
								hdcMen, 0, cySource * iMines[1][iMY][iMX], SRCCOPY);
						iMines[0][iMY][iMX] = MINE;
					}
					if (MINE != iMines[1][iMY][iMX] && FLAG == iMines[0][iMY][iMX]) {
						BitBlt(hdc, iX, iY, cxSource, cySource, hdcMen, 0, cySource * MINE_ERR, SRCCOPY);
								iMines[0][iMY][iMX] = MINE_ERR;
					}
				}
			}
			MessageBox(hwnd, TEXT("您超过了游戏最大限时 999 秒 游戏结束"), 
						TEXT("游戏结束"), MB_OK | MB_ICONINFORMATION);
		} 
		
		return 0;
	case WM_COMMAND:		
		MenuWndProc(hwnd, uMsg, wParam, lParam);
		return 0;
	case WM_LBUTTONUP:		
		hdc = GetDC(hwnd);
		hdcMen = CreateCompatibleDC(hdc);
		SelectObject(hdcMen, hBitmap);

		cxClinent = LOWORD(lParam);		
		cyClinent = HIWORD(lParam);	
		if (cxClinent >= iWidth / 2 - 27 && cxClinent <= iWidth / 2 + 17 && 
			 cyClinent >= 6 && cyClinent <= FACELONG + 4 ) {
			if (iGameState == GAMEOVER) {
				GameStart();
				InvalidateRect(hwndMain, NULL, FALSE);
			}
		}
		if (cxClinent >= EXCU_W	 && cxClinent <= iWidth - (EXCU_W + 2)	&& 
			 cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
			 iGameState != GAMEOVER ) {
			cxClinent = cxClinent - cxClinent % EXCU_W;
			cyClinent = cyClinent - cyClinent % EXCU_W;
			ix = (cxClinent - EXCU_W) / EXCU_W + 1;
			iy = (cyClinent - EXCU_H * 2) / EXCU_W + 1;		
			if (iGameState == GAMEUNSTART) {
				iTimeNum = 0;
				SetTimer(hwnd, IDT_TIME, 1000, NULL);
			}
			
	
			while (MINE == iMines[1][iy][ix] && iGameState == GAMEUNSTART) {
				GameStart();
				InvalidateRect(hwndMain, NULL, FALSE);
				mouse_event(MOUSEEVENTF_LEFTUP, LOWORD(lParam), HIWORD(lParam), 0, 0);
			}
			if (MINE == iMines[1][iy][ix]) {		
				KillTimer(hwnd, IDT_TIME);			
				iGameState = GAMEOVER;				
				iFaceState = FACEFAIL;				
				if (TRUE == iSound) {					
					PlaySound(MAKEINTRESOURCE(IDR_EXPLODE), NULL, SND_RESOURCE | SND_ASYNC);
				}
				for (iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++) {
					for (iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++) {
						if (MINE == iMines[1][iMY][iMX]) {
							BitBlt(hdc, iX, iY, cxSource, cySource, 
								    hdcMen, 0, cySource * iMines[1][iMY][iMX], SRCCOPY);
							iMines[0][iMY][iMX] = MINE;
						} 
						if (MINE != iMines[1][iMY][iMX] && FLAG == iMines[0][iMY][iMX]) {
							BitBlt(hdc, iX, iY, cxSource, cySource, 
									hdcMen, 0, cySource * MINE_ERR, SRCCOPY);
							iMines[0][iMY][iMX] = MINE_ERR;
						}
					}
				}
				iMines[0][iy][ix] = MINE_RED;					
				BitBlt(hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * MINE_RED, SRCCOPY);
			}
			if (ORIG == iMines[0][iy][ix]) {					
				iGameState = GAMESTART;							
				if (BLANK == iMines[1][iy][ix]) {			
					Recursion(iy, ix);							
					iMines[0][iy][ix] = BLANK;
					InvalidateRect(hwndMain, NULL, FALSE);		
				} else {										
					BitBlt(hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * iMines[1][iy][ix], SRCCOPY);
					iMines[0][iy][ix] = iMines[1][iy][ix];
				}
			} 
			if (JugdeWin())	{								
				KillTimer(hwnd, IDT_TIME);				
				iGameState = GAMEOVER;							
				iFaceState = FACEWIN;					
				if (TRUE == iSound) {
					PlaySound(MAKEINTRESOURCE(IDR_WIN), NULL, SND_RESOURCE | SND_ASYNC);
				}
				
				for (iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++) {
					for (iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++) {
						if (MINE == iMines[1][iMY][iMX]) {
							BitBlt(hdc, iX, iY, cxSource, cySource, 
								    hdcMen, 0, cySource * FLAG, SRCCOPY);
									iMines[0][iMY][iMX] = FLAG;
						}
						if (BLANK == iMines[1][iMY][iMX]) {
							BitBlt(hdc, iX, iY, cxSource, cySource, 
									hdcMen, 0, cySource * BLANK, SRCCOPY);
									iMines[0][iMY][iMX] = BLANK;
						}
					}
				}
			}
		}
		DeleteDC(hdcMen);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_RBUTTONUP:		
		hdc = GetDC(hwnd);
		hdcMen = CreateCompatibleDC(hdc);
		SelectObject(hdcMen, hBitmap);
		cxClinent = LOWORD(lParam);
		cyClinent = HIWORD(lParam);
		if (cxClinent >= EXCU_W && cxClinent <= iWidth - (EXCU_W + 2) && 
			 cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
			 iGameState != GAMEOVER) {
			cxClinent = cxClinent - cxClinent % EXCU_W;
			cyClinent = cyClinent - cyClinent % EXCU_W;
			ix = (cxClinent - EXCU_W) / EXCU_W + 1;
			iy = (cyClinent - EXCU_H * 2) / EXCU_W + 1;
			if (ORIG == iMines[0][iy][ix]) {  		
				BitBlt(hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * FLAG, SRCCOPY);
				iMines[0][iy][ix] = FLAG;
				iMineFlag--;
			}
			else if (FLAG == iMines[0][iy][ix])	{			
				BitBlt(hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * QUES, SRCCOPY);
				iMines[0][iy][ix] = QUES;
				iMineFlag++;
			} else if (QUES == iMines[0][iy][ix]) {			
				BitBlt(hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * ORIG, SRCCOPY);
				iMines[0][iy][ix] = ORIG;
			}
		}
		DeleteDC(hdcMen);
		ReleaseDC(hwnd, hdc);

		return 0;
	case WM_DESTROY:	
		KillTimer(hwnd, IDT_TIME);	
		KillTimer(hwnd, IDT_LIGHT);		
		DeleteObject(hBitmap);
		PostQuitMessage(0);

		return 0;
	default :
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
