#pragma once

#include "MainDialog.h"
#include "airMan.h"

//包含库
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   endif
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());   // 设置资源的默认路径（此处设置为和exe在同一目录）
	CMainDialog dia;

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	dia.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	dia.CenterWindow();
	dia.ShowModal();
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}