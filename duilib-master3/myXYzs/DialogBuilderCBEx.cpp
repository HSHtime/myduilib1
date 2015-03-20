#include "stdafx.h"
#include "DialogBuilderCBEx.h"


CDialogBuilderCallbackEx::CDialogBuilderCallbackEx(CPaintManagerUI* ppm)
{
	m_pm = ppm;
}

CControlUI* CDialogBuilderCallbackEx::CreateControl(LPCTSTR pstrClass) 
{
	//if( _tcscmp(pstrClass, _T("airMan")) == 0 ) return new AirManUI(m_pm);

	return NULL;
}