// maizang.h : main header file for the MAIZANG application
//

#if !defined(AFX_MAIZANG_H__26C5C559_7786_472F_A523_B665ED813E7E__INCLUDED_)
#define AFX_MAIZANG_H__26C5C559_7786_472F_A523_B665ED813E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "Resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMaizangApp:
// See maizang.cpp for the implementation of this class
//

class CMaizangApp : public CWinApp
{
public:
	CMaizangApp();
     void AttachImageList();//��ϵͳ��ͼ���б����Ƕ����ͼ���б���й���
	 void DetachImageList();
   CImageList m_SmallImageList;//����Сͼ��
   CImageList m_LargeImageList;//��ͼ��
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaizangApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMaizangApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAIZANG_H__26C5C559_7786_472F_A523_B665ED813E7E__INCLUDED_)
