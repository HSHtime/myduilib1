#include "stdafx.h"
#include "WebPage.h"

CWebPage::CWebPage(CPaintManagerUI *pm)
{
	m_PaintManager = pm;
	pActiveXUI = NULL;
	CDialogBuilder builder;
	CContainerUI* pClRbUI = static_cast<CContainerUI*>(builder.Create(_T("WebPage.xml"), (UINT)0));
	if( pClRbUI ) {
		this->Add(pClRbUI);
	}
	else {
		this->RemoveAll();
		return;
	}
	m_PaintManager->AddNotifier(this);	
}
void CWebPage::showPage(int i)
{
	// ��ʼ��CActiveXUI�ؼ�
	if (!pActiveXUI)
	{
		pActiveXUI = new CActiveXUI;
	}
	
	if( pActiveXUI ) 
	{
		IWebBrowser2* pWebBrowser = NULL;

		pActiveXUI->SetDelayCreate(false);              // �൱�ڽ�����������DelayCreate���Ը�ΪFALSE����duilib�Դ���FlashDemo����Կ���������ΪTRUE             
		pActiveXUI->CreateControl(CLSID_WebBrowser);    // �൱�ڽ�����������Clsid����������{8856F961-340A-11D0-A96B-00C04FD705A2}��������CLSID_WebBrowser������뿴��Ӧ��ֵ�����<ExDisp.h>
		pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

		if( pWebBrowser != NULL ) 
		{
			
			pWebBrowser->Navigate(L"http://software.xyzs.com/v3/iphone",NULL,NULL,NULL,NULL); 
			pWebBrowser->Release();
		}
	}
}
void CWebPage::Notify(TNotifyUI& msg) 
{

}