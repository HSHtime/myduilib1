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
	// 初始化CActiveXUI控件
	if (!pActiveXUI)
	{
		pActiveXUI = new CActiveXUI;
	}
	
	if( pActiveXUI ) 
	{
		IWebBrowser2* pWebBrowser = NULL;

		pActiveXUI->SetDelayCreate(false);              // 相当于界面设计器里的DelayCreate属性改为FALSE，在duilib自带的FlashDemo里可以看到此属性为TRUE             
		pActiveXUI->CreateControl(CLSID_WebBrowser);    // 相当于界面设计器里的Clsid属性里填入{8856F961-340A-11D0-A96B-00C04FD705A2}，建议用CLSID_WebBrowser，如果想看相应的值，请见<ExDisp.h>
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