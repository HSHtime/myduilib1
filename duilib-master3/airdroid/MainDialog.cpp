#include "StdAfx.h"
#include "MainDialog.h"
#include "airMan.h"
#include "MenuWnd.h"
#include "../FlashDemo/flash10a.tlh"

void    CMainDialog::Notify(TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("bt_TExit")) 
		{
			PostQuitMessage(0);
		}
		else if(msg.pSender->GetName() == _T("bt_TMin")) 
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
	}
	else if(msg.sType==_T("selectchanged"))
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
	}
}


LRESULT CMainDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if( uMsg == WM_CREATE ) 
	{
		//屏蔽系统标题栏，有边框。与WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT共用可以去掉边框
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintManager.Init(m_hWnd);

		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb(&m_PaintManager);//回调创建子窗口
		CControlUI* pRoot = builder.Create(_T("airdroid.xml"), (UINT)0, &cb, &m_PaintManager);   // duilib.xml需要放到exe目录下
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);//将对话框控件附加到当前的管理器中
		m_PaintManager.AddNotifier(this);   // 添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
		return lRes;
	}
	// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏,只是隐藏，点击右上角还是有反应
	else if( uMsg == WM_NCACTIVATE ) 
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

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}


