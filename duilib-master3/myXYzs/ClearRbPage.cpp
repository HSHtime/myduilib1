#include "stdafx.h"
#include "ClearRbPage.h"

CClearRbPage::CClearRbPage(CPaintManagerUI *pm)
{
	m_PaintManager = pm;
	CDialogBuilder builder;
	CContainerUI* pClRbUI = static_cast<CContainerUI*>(builder.Create(_T("clear_rubbish_page.xml"), (UINT)0));
	if( pClRbUI ) {
		this->Add(pClRbUI);
	}
	else {
		this->RemoveAll();
		return;
	}
	m_PaintManager->AddNotifier(this);	
}

void CClearRbPage::Notify(TNotifyUI& msg) 
{

}