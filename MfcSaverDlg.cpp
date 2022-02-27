// MfcSaverDlg.cpp : implementation file
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

#include "StdAfx.h"
#include "MfcSaver.h"
#include "MfcSaverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMfcSaverDlg, CScreenSaverDlg)
	//{{AFX_MSG_MAP(CMfcSaverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnURL)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMfcSaverDlg::CMfcSaverDlg()
{
	//{{AFX_DATA_INIT(CMfcSaverDlg)
	//}}AFX_DATA_INIT
}

BOOL CMfcSaverDlg::OnInitDialog()
{
	CMfcSaver* pSaver = (CMfcSaver*)AfxGetScreenSaverWnd();

	CScreenSaverDlg::OnInitDialog();

	if (pSaver) {
		pSaver->RestoreOptions();
	}

	return TRUE;
}

void CMfcSaverDlg::OnURL() 
{
	CString sURL;
	GetDlgItemText(IDC_BUTTON1, sURL);
	::ShellExecute(m_hWnd, NULL, sURL, NULL, NULL, 0);
}

void CMfcSaverDlg::OnDefault() 
{
	CMfcSaver* pSaver = (CMfcSaver*)AfxGetScreenSaverWnd();
	if (!pSaver)
		return;

	pSaver->delay=10000;
	strcpy(pSaver->m_TracePhone, "(212) 555-0690");
	pSaver->m_Speed=16;
	pSaver->m_Small=false;
	pSaver->m_Density=75;
	strcpy(pSaver->m_Insert, "both");
	pSaver->m_Spinners=5;
	pSaver->m_Trace=false;
	pSaver->m_FadeSpeed=5;

	UpdateData(false);
}

/////////////////////////////////////////////////////////////////////////////

//
// As with most MFC dialog boxes, the DoDataExchange() is where all the work
// happens.  There's no easy DDX_Trackbar thing to set up the control, so
// we do it ourselves.  See how the SaveOptions() and RestoreOptions() are
// called on the CMfcSaver to keep those options persistent.
//

void CMfcSaverDlg::DoDataExchange(CDataExchange* pDX)
{
	CScreenSaverDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMfcSaverDlg)
	//}}AFX_DATA_MAP

	CMfcSaver* pSaver = (CMfcSaver*)AfxGetScreenSaverWnd();
	if (!pSaver)
		return;

	CSliderCtrl* pSlider;
	CButton* pButton;
	CEdit *pEdit;
	if (!pDX->m_bSaveAndValidate)
	{
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_DELAY);
		pSlider->SetRange(500, 50000);
		pSlider->SetPos(pSaver->delay);

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SPINNERS);
		pSlider->SetRange(1, 25);
		pSlider->SetPos(pSaver->m_Spinners);

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_DENSITY);
		pSlider->SetRange(1, 100);
		pSlider->SetPos(pSaver->m_Density);

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SPEED);
		pSlider->SetRange(1, 50);
		pSlider->SetPos(pSaver->m_Speed);

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_FADESPEED);
		pSlider->SetRange(1, 100);
		pSlider->SetPos(pSaver->m_FadeSpeed);

		pButton=(CButton*)GetDlgItem(IDC_SMALL);
		pButton->SetCheck(pSaver->m_Small);

		pButton=(CButton*)GetDlgItem(IDC_TRACE);
		pButton->SetCheck(pSaver->m_Trace);

		pButton=(CButton*)GetDlgItem(IDC_TOP);
		pButton->SetCheck(stricmp(pSaver->m_Insert, "top") ? 0 : 1);

		pButton=(CButton*)GetDlgItem(IDC_BOTTOM);
		pButton->SetCheck(stricmp(pSaver->m_Insert, "bottom") ? 0 : 1);

		pButton=(CButton*)GetDlgItem(IDC_BOTH);
		pButton->SetCheck(stricmp(pSaver->m_Insert, "both") ? 0 : 1);

		pEdit=(CEdit*)GetDlgItem(IDC_TRACEPHONE);
		pEdit->SetWindowText(pSaver->m_TracePhone);
	}
	else
	{
		pSlider = (CSliderCtrl*)GetDlgItem(IDC_DELAY);
		pSaver->delay=pSlider->GetPos();

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SPINNERS);
		pSaver->m_Spinners=pSlider->GetPos();

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_DENSITY);
		pSaver->m_Density=pSlider->GetPos();

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_SPEED);
		pSaver->m_Speed=pSlider->GetPos();

		pSlider = (CSliderCtrl*)GetDlgItem(IDC_FADESPEED);
		pSaver->m_FadeSpeed=pSlider->GetPos();

		pButton=(CButton*)GetDlgItem(IDC_SMALL);
		pSaver->m_Small=(pButton->GetCheck() ? true : false);

		pButton=(CButton*)GetDlgItem(IDC_TRACE);
		pSaver->m_Trace=(pButton->GetCheck() ? true : false);

		pButton=(CButton*)GetDlgItem(IDC_TOP);
		if (pButton->GetCheck()) strcpy(pSaver->m_Insert, "top");

		pButton=(CButton*)GetDlgItem(IDC_BOTTOM);
		if (pButton->GetCheck()) strcpy(pSaver->m_Insert, "bottom");

		pButton=(CButton*)GetDlgItem(IDC_BOTH);
		if (pButton->GetCheck()) strcpy(pSaver->m_Insert, "both");

		pEdit=(CEdit*)GetDlgItem(IDC_TRACEPHONE);
		pEdit->GetWindowText(pSaver->m_TracePhone, 1024);

		pSaver->SaveOptions();
	}
}

