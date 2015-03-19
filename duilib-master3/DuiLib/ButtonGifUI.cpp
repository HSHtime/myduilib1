#include "stdafx.h"
#include "ButtonGifUI.h"

namespace DuiLib
{
	CButtonGifUI::CButtonGifUI()
		:m_pGif(NULL)
		,m_nPreUpdateDelay(50)
		,m_isUpdateTime(false)
	{
	}

	CButtonGifUI::~CButtonGifUI()
	{
		m_pManager->KillTimer(this, GIF_TIMER_ID);

		if (m_pGif)
		{
			delete m_pGif;
			m_pGif = NULL;
		}
	}

	void CButtonGifUI::SetVisible( bool bVisible /*= true*/ )
	{
		if(bVisible == false)
			m_pManager->KillTimer(this, GIF_TIMER_ID);

		CButtonUI::SetVisible(bVisible);
	}

	void CButtonGifUI::PaintStatusImage( HDC hDC )
	{
		if(m_pGif)
		{
			TImageInfo* pImageInfo = NULL;
			if (m_isUpdateTime)
			{
				m_isUpdateTime = false;
				pImageInfo = m_pGif->GetNextFrameInfo();
			}
			else
			{
				pImageInfo = m_pGif->GetCurrentFrameInfo();
			}
			if (pImageInfo)
			{
				RECT rcBmpPart = {0};
				RECT rcCorners = {0};
				rcBmpPart.right = pImageInfo->nX;
				rcBmpPart.bottom = pImageInfo->nY;
				CRenderEngine::DrawImage(hDC,pImageInfo->hBitmap,m_rcItem, m_rcPaint,rcBmpPart,rcCorners,pImageInfo->alphaChannel,255);
				if (m_nPreUpdateDelay != pImageInfo->delay)
				{
					m_pManager->KillTimer(this, GIF_TIMER_ID);
					//m_pManager->SetTimer(this, GIF_TIMER_ID, pImageInfo->delay);
					m_pManager->SetTimer(this, GIF_TIMER_ID, 50);
					m_nPreUpdateDelay = pImageInfo->delay;
				}
			}
		}
		//û��gifͼƬ,������ͨ��ťһ��
		else
		{
			if( IsFocused() ) 
				m_uButtonState |= UISTATE_FOCUSED;
			else 
				m_uButtonState &= ~ UISTATE_FOCUSED;
			if( !IsEnabled() ) 
				m_uButtonState |= UISTATE_DISABLED;
			else 
				m_uButtonState &= ~ UISTATE_DISABLED;

			if( (m_uButtonState & UISTATE_DISABLED) != 0 ) 
			{
				if( !m_sDisabledImage.IsEmpty() ) 
				{
					if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) 
						m_sDisabledImage.Empty();
					else 
						return;
				}
			}
			else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
			{
				if( !m_sPushedImage.IsEmpty() ) 
				{
					if( !DrawImage(hDC, (LPCTSTR)m_sPushedImage) ) 
						m_sPushedImage.Empty();
					else 
						return;
				}
			}
			else if( (m_uButtonState & UISTATE_HOT) != 0 ) 
			{
				if( !m_sHotImage.IsEmpty() ) 
				{
					if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) 
						m_sHotImage.Empty();
					else 
						return;
				}
			}
			else if( (m_uButtonState & UISTATE_FOCUSED) != 0 )
			{
				if( !m_sFocusedImage.IsEmpty() ) 
				{
					if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) 
						m_sFocusedImage.Empty();
					else 
						return;
				}
			}

			if( !m_sNormalImage.IsEmpty() ) 
			{
				if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) 
					m_sNormalImage.Empty();
				else 
					return;
			}
		}
	}

	void CButtonGifUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == GIF_TIMER_ID )
		{
			m_isUpdateTime = true;
			Invalidate();
			return;
		}

		CButtonUI::DoEvent(event);
	}

	void CButtonGifUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
	{
		if( _tcscmp(pstrName, _T("NormalGifFile")) == 0 )
		{
			SetNormalGifFile(pstrValue);
		}

		CButtonUI::SetAttribute(pstrName,pstrValue);
	}

	void CButtonGifUI::SetNormalGifFile( LPCTSTR pstrName )
	{
		if(pstrName == NULL) return;

		if (m_pGif)
		{
			m_pManager->KillTimer(this, GIF_TIMER_ID);
			m_nPreUpdateDelay = 50;
			delete m_pGif;
			m_pGif = NULL;
		}

		m_pGif = CRenderEngine::LoadGif(STRINGorID(pstrName),0, 0);

		Invalidate();
	}
}