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
		//����ϵͳ���������б߿���WM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT���ÿ���ȥ���߿�
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_PaintManager.Init(m_hWnd);

		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb(&m_PaintManager);//�ص������Ӵ���
		CControlUI* pRoot = builder.Create(_T("airdroid.xml"), (UINT)0, &cb, &m_PaintManager);   // duilib.xml��Ҫ�ŵ�exeĿ¼��
		ASSERT(pRoot && "Failed to parse XML");

		m_PaintManager.AttachDialog(pRoot);//���Ի���ؼ����ӵ���ǰ�Ĺ�������
		m_PaintManager.AddNotifier(this);   // ��ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����
		return lRes;
	}
	// ����3����ϢWM_NCACTIVATE��WM_NCCALCSIZE��WM_NCPAINT��������ϵͳ������,ֻ�����أ�������Ͻǻ����з�Ӧ
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


