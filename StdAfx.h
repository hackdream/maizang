// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4FB97E45_8F53_447A_84E7_27A885486F26__INCLUDED_)
#define AFX_STDAFX_H__4FB97E45_8F53_447A_84E7_27A885486F26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers


#define _WIN32_WINNT 0x0501
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
 
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
enum
{
	ONLINELIST_IP=0,//ip的列顺序
ONLINELIST_ADDR,//地址
ONLINELIST_COMPUTER_NAME,//电脑名称/备注
ONLINELIST_OS,//操作系统
ONLINELIST_CPU,//CPU
ONLINELIST_VIDEO,//摄像头
ONLINELIST_PING,//PING
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4FB97E45_8F53_447A_84E7_27A885486F26__INCLUDED_)
