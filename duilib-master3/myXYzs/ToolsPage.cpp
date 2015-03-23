#include "stdafx.h"
#include "ToolsPage.h"


CToolsPage::CToolsPage(CPaintManagerUI *pm)
{
	m_PaintManager = pm;
	CDialogBuilder builder;
	CContainerUI* pClRbUI = static_cast<CContainerUI*>(builder.Create(_T("ToolsPage.xml"), (UINT)0));
	if( pClRbUI ) {
		this->Add(pClRbUI);
	}
	else {
		this->RemoveAll();
		return;
	}
	m_PaintManager->AddNotifier(this);	
	
}

void CToolsPage::Notify(TNotifyUI& msg) 
{

}