#pragma once

class CClearRbPage:public CContainerUI, public INotifyUI
{
public:
	CClearRbPage(CPaintManagerUI *pm = NULL);
	virtual void    Notify(TNotifyUI& msg) ;

private:
	CPaintManagerUI *m_PaintManager;
};

