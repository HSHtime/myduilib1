#include "stdafx.h"
#include "MainDialog.h"
#include "DialogBuilderCBEx.h"
//#include "MenuWnd.h"
//#include "../FlashDemo/flash10a.tlh"

void    CMainDialog::Notify(TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("btn_close")) 
		{
			PostQuitMessage(0);
		}
		else if(msg.pSender->GetName() == _T("btn_min")) 
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
	}
/*	else if(msg.sType==_T("selectchanged"))
	{
	CDuiString name = msg.pSender->GetName();
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_switch")));
	if(name==_T("bt_man"))
	pControl->SelectItem(0);
	else if(name==_T("bt_file"))
	pControl->SelectItem(1);
	else if(name==_T("bt_ms"))
	pControl->SelectItem(2);
	else if(name==_T("bt_phone"))
	pControl->SelectItem(3);
	else if(name==_T("bt_book"))
	pControl->SelectItem(4);
	else if(name==_T("bt_note"))
	pControl->SelectItem(5);
	else if(name==_T("bt_mirror"))
	pControl->SelectItem(6);
	}
	else if( msg.pSender->GetName() == _T("bt_TSet") ) 
	{
	POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
	CMenuWnd *pMenu = new CMenuWnd;

	pMenu->Init(*this, pt);
	pMenu->ShowWindow(TRUE);
	}
	else if( msg.sType == _T("showactivex") ) {
	if( msg.pSender->GetName() != _T("flash") ) return;
	IShockwaveFlash* pFlash = NULL;
	CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
	pActiveX->GetControl(IID_IUnknown, (void**)&pFlash);
	if( pFlash != NULL ) {
	pFlash->put_WMode( _bstr_t(_T("Transparent") ) );
	pFlash->put_Movie( _bstr_t(CPaintManagerUI::GetInstancePath() + _T("flash.swf")) );
	pFlash->DisableLocalSecurity();
	pFlash->put_AllowScriptAccess(L"always");
	BSTR response;
	pFlash->CallFunction(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>", &response);
	pFlash->Release();
	}  
	}*/
}


LRESULT CMainDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		//WM_NCCALCSIZE消息，通知应用程序窗口的位置或者大小变了
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		//WM_NCHITTEST消息用来获取鼠标当前命中的位置,实现拖动窗口
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	//case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	
	// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏,只是隐藏，点击右上角还是有反应
	 if( uMsg == WM_NCACTIVATE ) 
	{
		if( !::IsIconic(m_hWnd) ) 
		{
			return (wParam == 0) ? TRUE : FALSE;
		}
	}
	else if( uMsg == WM_NCCALCSIZE ) 
	{
		return 0;
	}
	else if( uMsg == WM_NCPAINT ) 
	{
		return 0;
	}

	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//屏蔽系统标题栏，有边框。与WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT共用可以去掉边框
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb(&m_PaintManager);//回调创建子窗口
	CControlUI* pRoot = builder.Create(_T("Main.xml"), (UINT)0, &cb, &m_PaintManager);   // duilib.xml需要放到exe目录下
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);//将对话框控件附加到当前的管理器中
	m_PaintManager.AddNotifier(this); 
	return 0;
}
LRESULT CMainDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_PaintManager.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}
LRESULT CMainDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}
LRESULT CMainDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CMainDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}