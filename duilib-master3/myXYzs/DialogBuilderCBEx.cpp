#include "stdafx.h"
#include "DialogBuilderCBEx.h"
#include "ClearRbPage.h"
#include "ToolsPage.h"
#include "WebPage.h"

CDialogBuilderCallbackEx::CDialogBuilderCallbackEx(CPaintManagerUI* ppm)
{
	m_pm = ppm;
}

CControlUI* CDialogBuilderCallbackEx::CreateControl(LPCTSTR pstrClass) 
{
	//MessageBox(NULL,"callback","about",0);
	if( _tcscmp(pstrClass, _T("ClearRubbishPage")) == 0 ) return new CClearRbPage(m_pm);
	if( _tcscmp(pstrClass, _T("ToolsPage")) == 0 ) return new CToolsPage(m_pm);
	//if( _tcscmp(pstrClass, _T("WebPage")) == 0 ) return new CWebPage(m_pm);
	return NULL;
}