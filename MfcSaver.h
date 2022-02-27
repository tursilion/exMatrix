// MfcSaver.h : main header file for the MFCSAVER application
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
//
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

#ifndef __MFCSAVER_H__
#define __MFCSAVER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////

#include "ScreenSaverWnd.h"

/////////////////////////////////////////////////////////////////////////////
// Matrix stuff
// Character counts in the bitmap (height/Y count = character height)
#define CHAR_YCOUNT 21
#define CHAR_XCOUNT 3

struct _GRID {
	int nChar;
	int brightness;
	bool fDirty;
	bool fSpinner;
};

struct _COLUMN {
	int nCountDown;
	int nSpeed;
	int nSpeedCount;
	int nDirection; // (-1, 0, 1)
	int y;
};

struct _FALLERS {
	int x, y;
};

/////////////////////////////////////////////////////////////////////////////

class CMfcSaver : public CScreenSaverWnd
{
// Construction
public:
	CMfcSaver();
	virtual ~CMfcSaver();

	void load_images();
	void flip_images();
	void init_spinners();
	void init_trace();
	void init_matrix();
	void new_char(int nchar, int x, int y);
	void updateframe();
	void newstate();

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void SaveOptions();
	virtual void RestoreOptions();
	//{{AFX_VIRTUAL(CMfcSaver)
	//}}AFX_VIRTUAL

	int delay;
	char m_TracePhone[1024];
	bool m_Small;
	int m_Density;
	char m_Insert[1024];
	int m_Spinners;
	bool m_Trace;
	int m_Speed;
	HANDLE m_Bitmap, m_OldBitmap;
	HDC m_DC;
	int m_Fading, m_FadeSpeed;
	int m_screenWidth, m_screenHeight;
	int nDensity;

	int imgWidth, imgHeight;	// loaded image size
	int charWidth, charHeight;	// char size in pixels
	int xWidth, yHeight;		// screen size in characters
	bool fIsTrace;
	char szTraceOut[1024];
	struct _GRID *xGrid;
	struct _COLUMN *xColumn;
	struct _FALLERS *xFallers;

protected:
	//{{AFX_MSG(CMfcSaver)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MFCSAVER_H__
