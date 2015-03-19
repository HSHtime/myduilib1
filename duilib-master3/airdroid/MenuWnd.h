#pragma once
#include "stdafx.h"
class CMenuWnd :
	public WindowImplBase
{
public:
	//必须实现接口
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual CDuiString GetSkinFile()                {   return _T("menu.xml");  }
	virtual CDuiString GetSkinFolder()              {   return _T("");  }

	void Init(HWND hWndParent, POINT ptPos);
	virtual void    OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
protected:
	CDuiString m_strXMLPath;
};

