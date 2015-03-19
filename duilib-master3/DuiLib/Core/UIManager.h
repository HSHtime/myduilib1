#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CControlUI;


/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040



/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
    HFONT hFont;
    CDuiString sFontName;
    int iSize;
    bool bBold;
    bool bUnderline;
    bool bItalic;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
	int delay;        //��ʱʱ��,��λms 
    bool alphaChannel;
    CDuiString sResType;
    DWORD dwMask;
} TImageInfo;

// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

// Listener interface
class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

// MessageFilter interface
class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//
typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);


class UILIB_API CPaintManagerUI
{
public:
    CPaintManagerUI();
    ~CPaintManagerUI();

public:
	//��ͼ�������ĳ�ʼ��(m_hWndPaint,m_hDcPaint��ֵ����Ԥ������Ϣ�м��������)
    void Init(HWND hWnd);
	void NeedUpdate();//��ǰ��Ҫ���½���
	void Invalidate(RECT& rcItem);//ָ������ʧЧ

	HDC GetPaintDC() const;//��ȡ��ͼ�豸DC
	HWND GetPaintWindow() const;//��ȡ��ͼ�Ĵ��ھ��
	HWND GetTooltipWindow() const;//��ȡ��ʾ������

    POINT GetMousePos() const;
    SIZE GetClientSize() const;
    SIZE GetInitSize();
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);//��ȡ����ı߿������С
	RECT& GetCaptionRect();//��ȡ��������λ��
    void SetCaptionRect(RECT& rcCaption);
	SIZE GetRoundCorner() const;//��ȡ�����Ľǵ�Բ�ǻ���
    void SetRoundCorner(int cx, int cy);
	SIZE GetMinInfo() const;//��ȡ������Ե���������С��С
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
	int GetTransparent() const;
	void SetTransparent(int nOpacity);//����Ĳ�͸����(0��ȫ͸��-255��ȫ��͸��)
    void SetBackgroundTransparent(bool bTrans);
    bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);

    static HINSTANCE GetInstance();
    static CDuiString GetInstancePath();
	static CDuiString GetCurrentPath();// ��õ�ǰ�Ĺ���·��
    static HINSTANCE GetResourceDll();
    static const CDuiString& GetResourcePath();
    static const CDuiString& GetResourceZip();
    static bool IsCachedResourceZip();
    static HANDLE GetResourceZipHandle();
    static void SetInstance(HINSTANCE hInst);
    static void SetCurrentPath(LPCTSTR pStrPath);
    static void SetResourceDll(HINSTANCE hInst);
    static void SetResourcePath(LPCTSTR pStrPath);
	static void SetResourceZip(LPVOID pVoid, unsigned int len);
    static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false);
    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
    static void ReloadSkin();
    static bool LoadPlugin(LPCTSTR pstrModuleName);
    static CStdPtrArray* GetPlugins();

    bool UseParentResource(CPaintManagerUI* pm);
    CPaintManagerUI* GetParentResource() const;

    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    TFontInfo* GetDefaultFontInfo();
    void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    DWORD GetCustomFontCount() const;
    HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT GetFont(int index);
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool FindFont(HFONT hFont);
    bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    int GetFontIndex(HFONT hFont);
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool RemoveFont(HFONT hFont);
    bool RemoveFontAt(int index);
    void RemoveAllFonts();
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);

    const TImageInfo* GetImage(LPCTSTR bitmap);
    const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    bool RemoveImage(LPCTSTR bitmap);
    void RemoveAllImages();
    void ReloadAllImages();

    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
    bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
    const CStdStringPtrMap& GetDefaultAttribultes() const;
    void RemoveAllDefaultAttributeList();

    bool AttachDialog(CControlUI* pControl);
    bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    void ReapObjects(CControlUI* pControl);

    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();

    CControlUI* GetFocus() const;
    void SetFocus(CControlUI* pControl);
    void SetFocusNeeded(CControlUI* pControl);

    bool SetNextTabControl(bool bForward = true);

    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    bool KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();

    void SetCapture();
    void ReleaseCapture();
    bool IsCaptured();

	bool AddNotifier(INotifyUI* pControl);// ��ӿؼ���֪ͨ������
    bool RemoveNotifier(INotifyUI* pControl);   
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);

    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    bool RemovePreMessageFilter(IMessageFilterUI* pFilter);

    bool AddMessageFilter(IMessageFilterUI* pFilter);
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

    int GetPostPaintCount() const;
    bool AddPostPaint(CControlUI* pControl);
    bool RemovePostPaint(CControlUI* pControl);
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

    void AddDelayedCleanup(CControlUI* pControl);

	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool TranslateAccelerator(LPMSG pMsg);

    CControlUI* GetRoot() const;
    CControlUI* FindControl(POINT pt) const;
    CControlUI* FindControl(LPCTSTR pstrName) const;
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
    CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
    CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();

    static void MessageLoop();
    static bool TranslateMessage(const LPMSG pMsg);
	static void Term();
	//��Ϣ������(���Ĵ�����)
	//	1.��Ϣ����
	//	2.���Custom��Ϣ������
	//	3.����Ƿ���WM_CLOSE��Ϣ������
	//	4.����WM_ERASEBKGND(��������б�����������ֹ��˸)
	//	5.���ƴ���(����)
	//	5.1���ӳٻ�ͼ�жϣ���ǰ�Ƿ��� �����С�����Ĳ����������Ƿ���Ҫ��ʼ������
	//	5.2���ý���ؼ�
	//	5.3�������˫�����ͼ������˫���淽ʽ��ͼ��������ñ�׼��ͼ��ʽ��ͼ
	//	5.4               
	//	6.����ͻ����Ļ���WM_PRINTCLIENT
	//	7.�ӵ�WM_GETMINMAXINFO��Ϣ����ϵͳ�ύ�ô���ɵ�����С����С���������
	//	8.�����С�ı�ʱ���򽹵�ؼ����͸ı��С��Ϣ�����ô�����Ҫ����
	//	9.����ʱ����Ϣ����ʱ�������й㲥��ʱ��Ϣ
	//	10.���������ͣ
	//	10.1�������ͣ�Ŀؼ����������ͣ��Ϣ
	//	10.2�����ǰ�ؼ�����ʾ��Ϣ��������Ϣ��ʾ����
	//	11.��������뿪�¼����ر���Ϣ��ʾ�򣬷�������뿪��Ϣ��ȡ������׷��
	//	12.����ƶ�ʱ����ʼ׷�����
	//	12.1��������ƶ�ʱ������ڿؼ��Ͻ��룬�ƶ�����ͣ���뿪����Ϣ
	//	13.�������������µ���Ϣ�趨��Ľ���Ŀؼ�
	//	14.���˫���¼���������Ҫ�������˫���¼��Ŀؼ�����˫���¼�
	//	15.������̧��ʱ�����ϴν��յ������Ϣ�Ŀؼ�����������̧�����Ϣ
	//	16.����Ҽ�����ʱ������Ҫ��������Ҽ����µĿؼ������Ҽ�������Ϣ
	//	17.����Ҽ���ݲ˵���Ϣ��������Ϣ֪ͨ���ϴε�����İ�ť
	//	18.������Ϣʱ����������ڵĿؼ����͹�����Ϣ
	//	19.WM_CHAR ��Ϣʱ�����ý���Ŀؼ����͸���Ϣ
	//	20.���̰���ʱ���򽹵�ؼ����͸ü�����Ϣ�����趨����ؼ�Ϊ������Ϣ�ؼ�
	//	21.���̰���̧��ʱ�����¼�����Ϣ�ؼ����͸��¼�
	//	22.�趨�������Ϣʱ����ù�����ڿؼ����ո���Ϣ
	//	23.֪ͨ��Ϣ����ʱ����OCM_BASE����֪ͨ��Ϣ
	//	24.������Ϣ��������OCM_BASE������Ϣ
	//	25.WM_CTLCOLOREDIT��STATIC��Ϣ�����󣬼�OCM_BASE������Ϣ
    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	/*��ϢԤ������
		1.��ϢԤ�������(��ϢԤ������������϶���Ϣ���й��˴���)
		2.����Ƿ���Tab����������һ����ý���Ŀؼ�
		3.����Alt+Shortcut Key���º�Ŀؼ���ý���ͼ��������
		4.����Ƿ���ϵͳ����Ϣ�������ͻ�ý���Ŀؼ����¼�*/
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	void UsedVirtualWnd(bool bUsed);

private:
	//�����еĿؼ���ӵ�m_mNameHash��ϣ����
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
	// ����ؼ�����
    static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
	//���ݵ��Ƿ��������У���ѯ�ؼ�
    static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
	//ͨ��Tab��Ϣ��ѯ�ؼ�
    static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
	//�ӿ����в�ѯ�ؼ�
    static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
	//������Ҫ���µĿؼ�
    static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
	// ͨ�����ƱȽϲ�ѯ�ؼ�
    static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:
	HWND m_hWndPaint;//ҪCPaintManagerUI����Direct��ͼ�����Ĵ�����
	int m_nOpacity;
	//��ͼ�豸
	HDC m_hDcPaint;//ֱ�ӻ��Ƶ������DC(Ϊ�����������������ǿͻ���)
	HDC m_hDcOffscreen;//�ڴ滺������ͼDC
	HDC m_hDcBackground;//��������(֧��AlphaBackgroundʱʹ��)
	//λͼ
	HBITMAP m_hbmpOffscreen;//m_hDcPaint�ĺ�̨��ͼ����
	HBITMAP m_hbmpBackground;//����ͼƬbmp
	HWND m_hwndTooltip;//��ʾ���ھ��
	TOOLINFO m_ToolTip;// ��ʾ��Ϣ,win��
	//�ؼ���Ϣ
	CControlUI* m_pRoot;// xml���ڵ�����ɵĶ���ͨ��Ϊ����Window
	CControlUI* m_pFocus;//���ڻ�ý���״̬�Ŀؼ�
	CControlUI* m_pEventHover;//���������ͣ״̬�Ŀؼ�
	CControlUI* m_pEventClick;//��������Ŀؼ�
	CControlUI* m_pEventKey;//���ռ�������Ŀؼ�
	//λ�ü�¼��Ϣ
	POINT m_ptLastMousePos;//������µ�λ��
	SIZE m_szMinWindow;//���ô�����Ե���������С��С
	SIZE m_szMaxWindow;//������Ե�����������С
	SIZE m_szInitWindowSize;// �����ʼ���ǵĴ�С
	RECT m_rcSizeBox;//������߿�����Ĵ�С
	SIZE m_szRoundCorner;// �����Ľǵ�Բ�ǻ���
	RECT m_rcCaption;//��������������С
	UINT m_uTimerID;//��ǰ��ʱ��ID
	// ��ʶ����Ϣ 
	bool m_bShowUpdateRect;//�Ƿ���ʾ��������
    bool m_bFirstLayout;
    bool m_bUpdateNeeded;
    bool m_bFocusNeeded;
    bool m_bOffscreenPaint;
    bool m_bAlphaBackground;
    bool m_bMouseTracking;
    bool m_bMouseCapture;
	bool m_bUsedVirtualWnd;
	/*o---m_bFirstLayout             �Ƿ����׸����� 
	o---m_bUpdateNeeded            �Ƿ���Ҫ���½���
	o---m_bFocusNeeded             �Ƿ���Ҫ����
	o---m_bOffscreenPaint          �Ƿ���Ҫ��˫�����ͼ
	o---m_bAlphaBackground         ���屳���Ƿ���Ҫ֧��Alphaͨ��(��pngͼƬ�İ�͸��Ч��)
	o---m_bMouseTracking           �Ƿ���Ҫ֧�����׷��
	o---m_bMouseCapture            �Ƿ���Ҫ֧����겶��*/

	//��������Ϣ
    CStdPtrArray m_aNotifiers;
    CStdPtrArray m_aTimers;
    CStdPtrArray m_aPreMessageFilters;
    CStdPtrArray m_aMessageFilters;
    CStdPtrArray m_aPostPaintControls;
    CStdPtrArray m_aDelayedCleanup;
    CStdPtrArray m_aAsyncNotify;
    CStdPtrArray m_aFoundControls;
    CStdStringPtrMap m_mNameHash;
    CStdStringPtrMap m_mOptionGroup;
	/*  o---m_aNotifiers               �ܹ�����֪ͨ�Ķ��󼯺�
		o---m_aTimers                  ��ʱ������
		o---m_aPreMessage              Ԥ������Ϣ����
		o---m_aPreMessageFilters       Ԥ������Ϣ����������
		o---m_aMessageFilters          ��Ϣ����������
		o---m_aPostPaintControls       ���ͻ�������Ŀؼ�����
		o---m_aDelayedCleanup          �ӳ�����Ķ��󼯺�
		o---m_aAsyncNotify             �첽֪ͨ��Ϣ����
		o---m_mNameHash                ����HashMap
		o---m_mOptionGroup             ѡ����Map*/

	//xml��Ӧ��Դ
    CPaintManagerUI* m_pParentResourcePM;
    DWORD m_dwDefaultDisabledColor;
    DWORD m_dwDefaultFontColor;
    DWORD m_dwDefaultLinkFontColor;
    DWORD m_dwDefaultLinkHoverFontColor;
    DWORD m_dwDefaultSelectedBkColor;
    TFontInfo m_DefaultFontInfo;
    CStdPtrArray m_aCustomFonts;
	CStdStringPtrMap m_mImageHash;
	CStdStringPtrMap m_DefaultAttrHash;
	/*o---m_pParentResourcePM        �ϼ�(����)��Դ��PaintManagerUI��ͼ������
	o---m_dwDefaultDisabledColor   Ĭ��ʧЧ״̬��ɫ
	o---m_dwDefaultFontColor       Ĭ��������ɫ
	o---m_dwDefaultLinkFontColor   Ĭ�ϳ�����������ɫ
	o---m_dwDefaultLinkHoverFontColorĬ�ϳ����������ͣ״̬��������ɫ
	o---m_dwDefaultSelectedBkColor Ĭ��ѡ��״̬����ɫ
	o---m_DefaultFontInfo          Ĭ��������Ϣ
	o---m_aCustonFonts             �Զ���������Դ����
	o---m_mImageHash               ͼƬ��ԴHashMap
	o---m_DefaultAttrHash          DefaultAttr��ԴHashMap*/

	//������
	static HINSTANCE m_hInstance;//��ǰ�����Instanceʵ��
	static HINSTANCE m_hResourceInstance;//��ǰ�������ԴDLL Instanceʵ��
	static CDuiString m_pStrResourcePath;//��ǰʹ�õ���Դ·��
	static CDuiString m_pStrResourceZip;//��ǰʹ�õ���Դѹ�����ļ�ȫ��
    static bool m_bCachedResourceZip;
    static HANDLE m_hResourceZip;
    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
    static CStdPtrArray m_aPlugins;

public:
	static CDuiString m_pStrDefaultFontName;
	CStdPtrArray m_aTranslateAccelerator;
};

} // namespace DuiLib

//typedef struct tagTOOLINFOA {
//	UINT cbSize;                //�ýṹ��Ĵ�С sizeof(TOOLINFO)
//	UINT uFlags;                //���ӱ�ʶ����Ϣ
//	HWND hwnd;                  //��Ϣ���ܵĴ���
//	UINT_PTR uId;               //�ؼ�ID
//	RECT rect;                  //��Ϣ����������λ��
//	HINSTANCE hinst;            //��Ϣ���յ�ʵ��
//	LPSTR lpszText;             //��ʾ��Ϣ
//	LPARAM lParam;              //IE3.0���ϵİ汾�и�����
//	void *lpReserved;           //NT5.0���ϵİ汾�и����� ������Ϣ
//} TOOLINFO

//TFontInfo{
//	hFont               ������ľ��
//		sFontName           ��������
//		iSize               �ֺ�
//		bBold               �Ƿ����
//		bUnderline          �Ƿ����»���
//		bItalic             �Ƿ�Ϊб��
//		TEXTMETRIC tm       �������TEXTMETRIC��Ϣ
//}

#endif // __UIMANAGER_H__
