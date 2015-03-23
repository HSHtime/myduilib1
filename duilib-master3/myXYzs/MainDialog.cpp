#include "stdafx.h"
#include "MainDialog.h"
#include "DialogBuilderCBEx.h"
#include "FeedbackWnd.h"
#include "MyWebHandle.h"
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
		else if(msg.pSender->GetName() == _T("btn_main_feedback")) 
		{
			CFeedbackWnd fbwnd;			
			fbwnd.Create(this->m_hWnd, _T("feedbackWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);		
			fbwnd.CenterWindow();
			fbwnd.ShowModal();

			//CFeedbackWnd *fbwnd= new CFeedbackWnd;
			//fbwnd->Create(this->m_hWnd, _T("feedbackWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
			//fbwnd->CenterWindow();
			//fbwnd->ShowModal();

		}
	}
	else if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_page")));
		if(name==_T("op_main"))
			pControl->SelectItem(0);
		else if(name==_T("op_app_web"))
		{
			if (!m_pWebPage)
			{
				m_pWebPage = static_cast<CWebPage*>(m_PaintManager.FindControl(_T("app_web")));
				ASSERT(m_pWebPage);
			//	m_pWebPage->showPage(1);
			}		
			pControl->SelectItem(0);

		}
			
		else if(name==_T("op_game_web"))
			pControl->SelectItem(0);
		else if(name==_T("op_wallpaper_web"))
			pControl->SelectItem(0);
		else if(name==_T("op_slim"))
			pControl->SelectItem(1);
		else if(name==_T("op_tools"))
			pControl->SelectItem(2);
	}
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
		//WM_NCCALCSIZE��Ϣ��֪ͨӦ�ó��򴰿ڵ�λ�û��ߴ�С����
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		//WM_NCHITTEST��Ϣ������ȡ��굱ǰ���е�λ��,ʵ���϶�����
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	//case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//����ϵͳ���������б߿���WM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT���ÿ���ȥ���߿�
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb(&m_PaintManager);//�ص������Ӵ���
	CControlUI* pRoot = builder.Create(_T("Main.xml"), (UINT)0, &cb, &m_PaintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);//���Ի���ؼ����ӵ���ǰ�Ĺ�������
	m_PaintManager.AddNotifier(this); 
	m_pWebPage = NULL;

	// ��ʼ��CActiveXUI�ؼ�
	//CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("app_web")));
	CWebBrowserUI* pActiveXUI = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("app_web")));
	if( pActiveXUI ) 
	{
		pActiveXUI->SetDelayCreate(false);
		CMyWebHandle *pWebHandle = new CMyWebHandle;
		pActiveXUI->SetWebBrowserEventHandler(pWebHandle);
		pActiveXUI->Navigate2(_T("about:blank"));    //���д��룬���ע�͵����Ͳ���ȥ���߿�IE��bug���ڶ��μ�����ҳ�Ż����¼���������Ч
		pActiveXUI->Navigate2(_T("http://software.xyzs.com/v3/iphone"));
		//IWebBrowser2* pWebBrowser = NULL;

		//pActiveXUI->SetDelayCreate(false);              // �൱�ڽ�����������DelayCreate���Ը�ΪFALSE����duilib�Դ���FlashDemo����Կ���������ΪTRUE             
		//pActiveXUI->CreateControl(CLSID_WebBrowser);    // �൱�ڽ�����������Clsid����������{8856F961-340A-11D0-A96B-00C04FD705A2}��������CLSID_WebBrowser������뿴��Ӧ��ֵ�����<ExDisp.h>
		//pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

		//if( pWebBrowser != NULL ) 
		//{
		//	//pWebhandle = new CMyWebHandle;
		//	//pWebBrowser->SetWebBrowserEventHandler(pWebhandle);
		//	//pWebBrowser->Navigate(L"http://software.xyzs.com/wallpaper",NULL,NULL,NULL,NULL);
		//	////pWebBrowser->Navigate(L"http://software.xyzs.com/v3/iphone",NULL,NULL,NULL,NULL);  // ���ڹȸ�ʱ��ʱ��ǽ�����Ի��ɷ�Ӧ�����վ
		//	//pWebBrowser->Release();

		//}
	}
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