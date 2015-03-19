#pragma once
#include "..\DuiLib\ButtonGifUI.h"
class AirManUI : public CContainerUI, public INotifyUI
{
public:
	AirManUI(CPaintManagerUI *pm = NULL)
	{
		m_PaintManager = pm;
		CDialogBuilder builder;
		CContainerUI* pAirManUI = static_cast<CContainerUI*>(builder.Create(_T("airMan.xml"), (UINT)0));
		if( pAirManUI ) {
			this->Add(pAirManUI);
		}
		else {
			this->RemoveAll();
			return;
		}
		m_PaintManager->AddNotifier(this);	
	}
	virtual void    Notify(TNotifyUI& msg) 
	{
	
	 if( msg.sType == DUI_MSGTYPE_VALUECHANGED )
	 {
		 pProgress = static_cast<CProgressUI*>(m_PaintManager->FindControl(_T("Progress1")));
		 pSlider = static_cast<CSliderUI*>(m_PaintManager->FindControl(_T("Slider1")));
		 int value = pSlider->GetValue();
		 pProgress->SetValue(value);
		//::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
	 }
	}
	
	
private:
	CSliderUI* pSlider;
	CProgressUI* pProgress ;
	CPaintManagerUI *m_PaintManager;
};

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CDialogBuilderCallbackEx(CPaintManagerUI* ppm = NULL)
	{
		m_pm = ppm;
	}
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("airMan")) == 0 ) return new AirManUI(m_pm);

		return NULL;
	}
private:
	// 加一个PAINTMANAGER是为了加载　Default 项
	CPaintManagerUI* m_pm;
};