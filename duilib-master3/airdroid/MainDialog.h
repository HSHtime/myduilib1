#pragma once
#include "stdafx.h"

class CMainDialog :
	public CWindowWnd, public INotifyUI
{
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual void    Notify(TNotifyUI& msg) ;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	CPaintManagerUI m_PaintManager;
};
