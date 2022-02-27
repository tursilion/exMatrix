// MfcSaver.cpp : Defines the class behaviors for the application.
//
//////////
//
// Copyright (C) 1991-98 Ed Halley.
//   http://www.explorati.com/people/ed/
//   ed@explorati.com
//
// This published source code represents original intellectual
// property, owned and copyrighted by Ed Halley.
//
// The owner has authorized this source code for general public
// use without royalty, under two conditions:
//    * The source code maintains this copyright notice in full.
//    * The source code is only distributed for free or
//      reasonable duplication cost, not for distribution profit.
//
// Unauthorized use, copying or distribution is a violation of
// U.S. and international laws and is strictly prohibited.
//
//////////
// EXMatrix Screen Saver
//
// (C) 2003 Mike Brent aka Tursi aka HarmlessLion.com
// This software is provided AS-IS. No warranty
// express or implied is provided.
//
// You may redistribute this software provided the original
// archive is UNCHANGED and a link back to my web page,
// http://harmlesslion.com, is provided as the author's site.
//
// Source code, if available, is provided for educational purposes
// only. You are welcome to read it, learn from it, mock
// it, and hack it up - for your own use only.
//
// Please contact me before distributing derived works or
// ports so that we may work out terms. I don't mind people
// using my code but it's been outright stolen before.
//
// -COMMERCIAL USE- Contact me first. I didn't make
// any money off it - why should you? ;) If you just learned
// something from this, then go ahead. If you just pinched
// a routine or two, let me know, I'll probably just ask
// for credit. If you want to derive a commercial tool
// or use large portions, we need to talk. ;)
//
// If this, itself, is a derived work from someone else's,
// then their original copyrights and licenses are left intact
// and in full force.
//
// http://harmlesslion.com - hit the web page for contact info
//


#include "StdAfx.h"

#include "MfcSaver.h"
#include "MfcSaverDlg.h"
#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// TODO: the density setting doesn't really do much - eventually the screen
// fills up. We aren't inserting blanks in all cases.


BEGIN_MESSAGE_MAP(CMfcSaver, CScreenSaverWnd)
//{{AFX_MSG_MAP(CMfcSaver)
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// As required of CScreenSaverWnd-based screen savers, these are the two
// global instances of screen saver objects.  One is the saver itself,
// and one is the dialog for configuring the options of the screen saver.
//
// Unlike most MFC applications, there is no instance of any CWinApp object.
//
CMfcSaver theSaver;
CMfcSaverDlg theSaverDialog;

CMfcSaver::CMfcSaver()
{
	srand(time(NULL));
	m_bAutoBlack=false;
	m_Fading=1;
	xGrid=NULL;
	xColumn=NULL;
	xFallers=NULL;
	memset(szTraceOut, 0, 1024);
}

// Not called??
CMfcSaver::~CMfcSaver()
{
	if (m_OldBitmap) SelectObject(m_DC, m_OldBitmap);
	if (m_DC) DeleteDC(m_DC);
	if (m_Bitmap) DeleteObject(m_Bitmap);
	if (xGrid) free(xGrid);
	if (xColumn) free(xColumn);
	if (xFallers) free(xFallers);
}

/////////////////////////////////////////////////////////////////////////////
//
// The CMfcSaverDlg will need to be able to access our customizable
// attributes.  We call OnInitialUpdate() again to reorganize internally if
// any of these change.
//

void CMfcSaver::SaveOptions()
{
	CString myStr;

	myStr.Format("%d", delay);
	SHRegSetUSValue("Software\\WMatrix", "Delay", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	SHRegSetUSValue("Software\\WMatrix", "TracePhone", REG_SZ, m_TracePhone, strlen(m_TracePhone), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_Speed);
	SHRegSetUSValue("Software\\WMatrix", "Speed", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_Small);
	SHRegSetUSValue("Software\\WMatrix", "Small", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_Density);
	SHRegSetUSValue("Software\\WMatrix", "Density", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	SHRegSetUSValue("Software\\WMatrix", "Insert", REG_SZ, m_Insert, strlen(m_Insert), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_Spinners);
	SHRegSetUSValue("Software\\WMatrix", "Spinners", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_Trace);
	SHRegSetUSValue("Software\\WMatrix", "Trace", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);

	myStr.Format("%d", m_FadeSpeed);
	SHRegSetUSValue("Software\\WMatrix", "FadeSpeed", REG_SZ, (void*)(LPCSTR)myStr, myStr.GetLength(), SHREGSET_FORCE_HKLM);
}

void CMfcSaver::RestoreOptions()
{
	char tmp[1025];
	DWORD dwTmp, dwSize;
	int nTmp;

	delay=10000;
	strcpy(m_TracePhone, "(212) 555-0690");
	m_Speed=16;
	m_Small=false;
	m_Density=75;
	strcpy(m_Insert, "both");
	m_Spinners=5;
	m_Trace=false;
	m_FadeSpeed=5;

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Delay", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if ((nTmp>0) && (nTmp<50000)) delay=nTmp;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "TracePhone", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[1024]='\0';
		strcpy(m_TracePhone, tmp);
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Speed", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if ((nTmp>0) && (nTmp<2001)) m_Speed=nTmp;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Small", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if (nTmp) m_Small=true; else m_Small=false;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Density", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if ((nTmp>0) && (nTmp<100)) m_Density=nTmp;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Insert", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[1024]='\0';
		if (stricmp(tmp, "top")==0) strcpy(m_Insert, "top");
		if (stricmp(tmp, "bottom")==0) strcpy(m_Insert, "bottom");
		if (stricmp(tmp, "both")==0) strcpy(m_Insert, "both");
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Spinners", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if ((nTmp>0) && (nTmp<26)) m_Spinners=nTmp;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "Trace", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if (nTmp) m_Trace=true; else m_Trace=false;
	}

	dwSize=1024;
	if (ERROR_SUCCESS == SHRegGetUSValue("Software\\WMatrix", "FadeSpeed", &dwTmp, tmp, &dwSize, true, NULL, NULL)) {
		tmp[dwSize]='\0';
		nTmp=atoi(tmp);
		if ((nTmp>0) && (nTmp<101)) m_FadeSpeed=nTmp;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// While this is just demonstrating how to override the virtual functions of
// CScreenSaverWnd, it also shows something about using CImageList to
// draw from a library of simple images.
//
// When our timer goes off, we just invalidate the window.  The OnDraw()
// override will be called appropriately when the system has some free cycles
// to spend on drawing.  Remember:  a screen saver should share the CPU, so
// the computer can work on other tasks like downloading or numbercrunching.
//

void CMfcSaver::OnInitialUpdate()
{
	if (!m_hWnd)
		return;
	
	init_matrix ();
	
	SetTimer(1, 1, NULL);
}

// Although we really *shouldn't* do this stuff here in the draw method,
// it works well enough since a screensaver won't likely be interrupted
// by other windows or extra paint requests
void CMfcSaver::OnDraw(CDC* pDC)
{
	int idx;
	CDC myCDC;
	CGdiObject *oldPen;

	if (NULL == m_DC) return;

	if (m_Fading>0) {
		// we're fading out...
		for (idx=0; idx<m_FadeSpeed; idx++) {
			oldPen=pDC->SelectStockObject(BLACK_PEN);
			pDC->MoveTo((m_Fading*2)-1, 0);
			pDC->LineTo((m_Fading*2)-1, m_screenHeight);
			pDC->MoveTo(m_screenWidth-(m_Fading*2), 0);
			pDC->LineTo(m_screenWidth-(m_Fading*2), m_screenHeight);
			pDC->SelectObject(oldPen);
			m_Fading--;
			if (m_Fading<1) {
				KillTimer(1);
				SetTimer(1, 2000 / m_Speed, NULL);
				break;
			}
		}
	} else {
		struct _GRID *pGrid;
		int nBright;

		// Normal operation
		myCDC.Attach(m_DC);

		updateframe();
		newstate();

		for (int x=0; x<xWidth; x++) {
			for (int y=0; y<yHeight; y++) {
				
				pGrid=&xGrid[y*xWidth+x];

				if (fIsTrace) {
					// Special processing for the trace sequence
					int tx=x%strlen(m_TracePhone);

					if (y == 0) {
						if (x == tx) {
							if (szTraceOut[tx] == 0) {
								pGrid->nChar=0;
							} else {
								pGrid->nChar=szTraceOut[tx]-'0'+1;
							}
							pGrid->brightness=10;
							pGrid->fSpinner=false;
						} else {
							pGrid->nChar=0;
						}
					} else {
						if (szTraceOut[tx] != 0) {
							pGrid->nChar=0;
							pGrid->brightness=0;
						} else {
							pGrid->nChar=rand()%10+1;	// digits are currently the first 10
							pGrid->brightness=0;
							pGrid->fSpinner=false;
						}
					}
					pGrid->fDirty=true;
				}
				
				nBright=(pGrid->brightness > 0 || pGrid->fSpinner) ? 2 : (pGrid->brightness == 0) ? 1 : 0;
				if (nBright != 2) {
					// no point checking if it's already bright
					// We do this as a separate check so as not to affect grid points already bright
					// Otherwise we'd turn them off as we fell past
					for (int z=0; z<m_Spinners; z++) {
						if ((xFallers[z].x == x) && (xFallers[z].y == y)) {
							nBright=2;
							pGrid->fDirty=true;
							break;
						}
					}
				}

				if (pGrid->fDirty) {
					if (pGrid->nChar == 0) {
						pDC->BitBlt(x*charWidth, y*charHeight, charWidth, charHeight, &myCDC, 0, 0, BLACKNESS);
					} else {
						pDC->BitBlt(x*charWidth, y*charHeight, charWidth-1, charHeight, &myCDC, nBright*charWidth+1, (pGrid->nChar-1) * charHeight, SRCCOPY);
					}

					pGrid->fDirty=false;
				}

				if (pGrid->brightness > 0) {
					pGrid->brightness--;
					if (pGrid->brightness == 0) pGrid->fDirty=true;
				} else {
					if (pGrid->brightness < 0) {
						pGrid->brightness++;
						if (pGrid->brightness == 0) {
							pGrid->nChar=0;
							pGrid->fDirty=true;
						}
					}
				}

				if (pGrid->fSpinner) {
					pGrid->nChar = rand()%CHAR_YCOUNT+1;
					pGrid->fDirty=true;
				}
			}
		}

		// Update the trace
		if (fIsTrace) {
			if (strlen(szTraceOut) == strlen(m_TracePhone)) {
				fIsTrace=false;
				flip_images();
			} else {
				if (rand()%2) {
					int cnt=rand()%strlen(m_TracePhone);
					szTraceOut[cnt]=m_TracePhone[cnt];
				}
			}
		}

		myCDC.Detach();
	}
}

void CMfcSaver::OnTimer(UINT nIDEvent) 
{
	CScreenSaverWnd::OnTimer(nIDEvent);
	
	Invalidate(FALSE);
}

void CMfcSaver::load_images()
{
	int err;
	HINSTANCE inst;
	BITMAP myBmp;

	m_OldBitmap=NULL;
	m_DC=NULL;

	inst=AfxGetInstanceHandle();

	// if m_Small then use matrix2 else use matrix
	// set image_width and image_height to the size of the bitmap
	m_Bitmap=LoadImage(inst, (const char*)(m_Small ? IDB_MATRIX2 : IDB_MATRIX), IMAGE_BITMAP, 0, 0, 0);
	
	if (NULL == m_Bitmap) {
		OutputDebugString("Can't get resources.");
		err=GetLastError();
		// try not to crash
		imgWidth=1;
		imgHeight=1;
	} else {
		m_DC=CreateCompatibleDC(NULL);
		m_OldBitmap=SelectObject(m_DC, m_Bitmap);
		GetObject(m_Bitmap, sizeof(BITMAP), &myBmp);
		imgWidth = myBmp.bmWidth;
		imgHeight = myBmp.bmHeight;
	}

	charWidth=imgWidth/CHAR_XCOUNT;
	charHeight=imgHeight/CHAR_YCOUNT;
}

void CMfcSaver::flip_images()
{	
	CDC myCDC;
	int x;

	myCDC.Attach(m_DC);

	for (x=0; x<CHAR_XCOUNT; x++)
	{
		// flip one whole column
		myCDC.StretchBlt(x*charWidth, 0, charWidth, imgHeight, &myCDC, x*charWidth+charWidth, 0, -charWidth, imgHeight, SRCCOPY);
	}

	myCDC.Detach();
}

void CMfcSaver::init_spinners()
{
	int x, y;

	for (x=0; x<xWidth; x++) {
		for (y=0; y<yHeight; y++) {
			xGrid[y*xWidth+x].fSpinner=false;
		}
	}

	for (x=0; x<m_Spinners; x++) {
		xGrid[(rand()%yHeight)*xWidth+(rand()%xWidth)].fSpinner=true;
		xFallers[x].x=rand()%xWidth;
		xFallers[x].y=rand()%yHeight/3;
	}
}

void CMfcSaver::init_trace()
{
	char *p=m_TracePhone;

	while (*p) {
		if (!isdigit(*p)) {
			memmove(p, p+1, strlen(p+1)+1);
		} else {
			p++;
		}
	}
	/* Make sure there are any digits in the string! */
	if (strlen(m_TracePhone)) {
		memset(szTraceOut, 0, 1024);
		fIsTrace=true;
	} else {
		fIsTrace=false;
	}
}

void CMfcSaver::init_matrix()
{
	load_images();
	
	m_screenWidth=GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_screenHeight=GetSystemMetrics(SM_CYVIRTUALSCREEN);

#ifdef _DEBUG
	m_screenWidth/=3;	// partial screen for debug
#endif

	m_Fading=m_screenWidth/2;

	xWidth  = m_screenWidth  / charWidth + 1;
	yHeight = m_screenHeight / charHeight + 1;

	xGrid=(struct _GRID *)calloc(xWidth*yHeight, sizeof(struct _GRID));
	xColumn=(struct _COLUMN *)calloc(xWidth, sizeof(struct _COLUMN));
	xFallers=(struct _FALLERS *)calloc(m_Spinners, sizeof(struct _FALLERS));

	fIsTrace = m_Trace;
	nDensity=1;
	
	if (fIsTrace) {
		init_trace();
	} else {
		flip_images();
		init_spinners();
	}
}

void CMfcSaver::new_char(int nChar, int x, int y)
{
	struct _GRID *pGrid=&xGrid[y*xWidth+x];

	// If we're inserting a blank, or overwriting a character
	if ((nChar == 0)||(pGrid->nChar)) {
		bool fDown;

		if (xColumn[x].nDirection > 0) {
			fDown=false;
		} else {
			fDown=true;
		}

		if (fDown) {
			// Update density
			if (xGrid[(yHeight-1)*xWidth+x].nChar) {
				if (nDensity > 1) nDensity--;
			}

			// scroll characters from this point down, but don't move spinners
			int yy;
			for (yy=yHeight-1; yy>y; yy--) {	// write this cell from above
				if (xGrid[(yy-1)*xWidth+x].nChar) {
					bool fOld, fNew;
					fNew=xGrid[yy*xWidth+x].fSpinner;
					fOld=xGrid[(yy-1)*xWidth+x].fSpinner;
					memcpy(&xGrid[yy*xWidth+x], &xGrid[(yy-1)*xWidth+x], sizeof(struct _GRID));
					xGrid[(yy-1)*xWidth+x].fSpinner=fOld;
					xGrid[yy*xWidth+x].fSpinner=fNew;
				} else {
					// No character replaces this one - fade it out
					xGrid[yy*xWidth+x].brightness=-1;	// negative # of frames
				}
				xGrid[yy*xWidth+x].fDirty=true;
			}
		} else {
			// Update density
			if (xGrid[x].nChar) {
				if (nDensity > 1) nDensity--;
			}

			// scroll characters from this point up
			int yy;
			for (yy=0; yy<y; yy++) {	// write this cell from below
				if (xGrid[(yy+1)*xWidth+x].nChar) {
					bool fOld, fNew;
					fNew=xGrid[yy*xWidth+x].fSpinner;
					fOld=xGrid[(yy+1)*xWidth+x].fSpinner;
					memcpy(&xGrid[yy*xWidth+x], &xGrid[(yy+1)*xWidth+x], sizeof(struct _GRID));
					xGrid[(yy+1)*xWidth+x].fSpinner=fOld;
					xGrid[yy*xWidth+x].fSpinner=fNew;
				} else {
					// No character replaces this one - fade it out
					xGrid[yy*xWidth+x].brightness=-1;	// negative # of frames
				}
				xGrid[yy*xWidth+x].fDirty=true;
			}
			if (nDensity > 1) nDensity--;
		}
	}

	// Add the new char, randomly make it bright
	if (nChar) nDensity++;
	pGrid->nChar=nChar;
	pGrid->fDirty=true;
	if (rand()%xWidth == 0) {
		pGrid->brightness=5;
	}
}

void CMfcSaver::updateframe()
{
	int x;

	for (x=0; x<xWidth; x++) {
		if (xColumn[x].nSpeedCount>0) {
			xColumn[x].nSpeedCount--;
		} else {
			if (xColumn[x].nCountDown > 0) {
				new_char(rand()%CHAR_YCOUNT, x, xColumn[x].y);
				xColumn[x].nCountDown--;
				xColumn[x].y+=xColumn[x].nDirection;
				if ((xColumn[x].y<0)||(xColumn[x].y>=yHeight)) {
					xColumn[x].nCountDown=0;
					xColumn[x].y=rand()%yHeight;
				}
			} else {
				new_char(0, x, xColumn[x].y);
			}

			if (rand()%xWidth) {
				xColumn[x].nSpeed=rand()%m_Speed/3;
			}
			xColumn[x].nSpeedCount=xColumn[x].nSpeed;
		}
	}

	// and fallers
	for (x=0; x<m_Spinners; x++) {
		if (xFallers[x].y<yHeight) {
			xGrid[xFallers[x].y*xWidth+xFallers[x].x].fDirty=true;
			xFallers[x].y++;
		}
	}

}

void CMfcSaver::newstate()
{
	int x;

	// Set a few characters to glowing, based on width
	for (x=0; x<(rand()%xWidth); x++) {
		int xx, yy;
		xx=(rand()%xWidth);
		yy=(rand()%yHeight);
		xGrid[yy*xWidth+xx].brightness=rand()%10;
		xGrid[yy*xWidth+xx].fDirty=true;
	}

	// Check each column to see if it needs updating
	for (x=0; x<xWidth; x++) {
		if (xColumn[x].nCountDown < 1) {
			if (nDensity < 1) nDensity=1;
			int nRoughPercentage = (nDensity*100)/(xWidth*yHeight)+1;
			if ((nRoughPercentage < m_Density) && ((rand()%nRoughPercentage) < m_Density)) {
				xColumn[x].nCountDown=rand()%(yHeight*m_Density/100)+3;
				xColumn[x].nSpeedCount=rand()%m_Speed;

				// We expect the words 'top, bottom, both'. chars 1 and 4 are important
				if (tolower(m_Insert[0]) == 'b') {
					if (m_Insert[3] == 't') {
						// bottom insert, scroll up
						xColumn[x].nDirection=rand()%2;
					} else {
						// Both - choose a direction randomly
						xColumn[x].nDirection=rand()%3-1;
					}
				} else {
					// top insert by default
					xColumn[x].nDirection=-(rand()%2);
				}

				xColumn[x].y=rand()%yHeight;
			}
		}
	}

	if (!(rand()%xWidth)) {
		init_spinners();
	}
}
