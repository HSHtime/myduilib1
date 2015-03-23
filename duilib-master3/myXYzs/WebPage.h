#pragma once

class CWebPage:public CContainerUI, public INotifyUI
{
public:
	CWebPage(CPaintManagerUI *pm = NULL);
	virtual void    Notify(TNotifyUI& msg) ;
	void showPage(int i);
private:
	CPaintManagerUI *m_PaintManager;
	CActiveXUI* pActiveXUI ;
};

