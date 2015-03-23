#pragma once
class CToolsPage:public CContainerUI, public INotifyUI
{
public:
	CToolsPage(CPaintManagerUI *pm = NULL);
	virtual void    Notify(TNotifyUI& msg) ;

private:
	CPaintManagerUI *m_PaintManager;
};

