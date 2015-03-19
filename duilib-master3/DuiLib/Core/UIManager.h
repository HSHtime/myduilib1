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
	int delay;        //延时时间,单位ms 
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
	//绘图管理器的初始化(m_hWndPaint,m_hDcPaint赋值，在预处理消息中加入管理器)
    void Init(HWND hWnd);
	void NeedUpdate();//当前需要更新界面
	void Invalidate(RECT& rcItem);//指定区域失效

	HDC GetPaintDC() const;//获取绘图设备DC
	HWND GetPaintWindow() const;//获取绘图的窗口句柄
	HWND GetTooltipWindow() const;//获取提示窗体句柄

    POINT GetMousePos() const;
    SIZE GetClientSize() const;
    SIZE GetInitSize();
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);//获取窗体的边框区域大小
	RECT& GetCaptionRect();//获取标题区域位置
    void SetCaptionRect(RECT& rcCaption);
	SIZE GetRoundCorner() const;//获取窗体四角的圆角弧度
    void SetRoundCorner(int cx, int cy);
	SIZE GetMinInfo() const;//获取窗体可以调整到的最小大小
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
	int GetTransparent() const;
	void SetTransparent(int nOpacity);//窗体的不透明度(0完全透明-255完全不透明)
    void SetBackgroundTransparent(bool bTrans);
    bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);

    static HINSTANCE GetInstance();
    static CDuiString GetInstancePath();
	static CDuiString GetCurrentPath();// 获得当前的工作路径
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

	bool AddNotifier(INotifyUI* pControl);// 添加控件到通知集合中
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
	//消息处理器(核心处理器)
	//	1.消息过滤
	//	2.检查Custom消息并处理
	//	3.检查是否有WM_CLOSE消息并处理
	//	4.处理WM_ERASEBKGND(不允许进行背景擦除，防止闪烁)
	//	5.绘制处理(核心)
	//	5.1做延迟绘图判断，当前是否有 窗体大小调整的操作，或者是否需要初始化窗体
	//	5.2设置焦点控件
	//	5.3如果开启双缓存绘图，采用双缓存方式绘图，否则采用标准绘图方式绘图
	//	5.4               
	//	6.处理客户区的绘制WM_PRINTCLIENT
	//	7.接到WM_GETMINMAXINFO消息后向系统提交该窗体可调整大小的最小和最大限制
	//	8.窗体大小改变时，向焦点控件发送改变大小消息并设置窗体需要更新
	//	9.处理定时器消息，向定时器集合中广播定时消息
	//	10.处理鼠标悬停
	//	10.1向鼠标悬停的控件发送鼠标悬停消息
	//	10.2如果当前控件有提示消息，创建消息提示窗体
	//	11.处理鼠标离开事件，关闭消息提示框，发送鼠标离开消息，取消鼠标的追踪
	//	12.鼠标移动时，开始追踪鼠标
	//	12.1处理鼠标移动时，鼠标在控件上进入，移动，悬停和离开的消息
	//	13.处理鼠标左键按下的消息设定活动的焦点的控件
	//	14.鼠标双击事件处理，向需要接收鼠标双击事件的控件发送双击事件
	//	15.鼠标左键抬起时，向上次接收到点击消息的控件发送鼠标左键抬起的消息
	//	16.鼠标右键按下时，向需要接收鼠标右键按下的控件发送右键按下消息
	//	17.鼠标右键快捷菜单消息，将该消息通知给上次点击过的按钮
	//	18.滚轮消息时，象鼠标所在的控件发送滚轮消息
	//	19.WM_CHAR 消息时，向获得焦点的控件发送该消息
	//	20.键盘按下时，向焦点控件发送该键盘消息，并设定焦点控件为键盘消息控件
	//	21.键盘按键抬起时，向事键盘消息控件发送该事件
	//	22.设定鼠标光标消息时，获得光标所在控件接收该消息
	//	23.通知消息到来时，加OCM_BASE后发送通知消息
	//	24.命令消息到来，加OCM_BASE后发送消息
	//	25.WM_CTLCOLOREDIT，STATIC消息到来后，加OCM_BASE后发送消息
    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	/*消息预处理器
		1.消息预处理过滤(消息预处理过滤器集合对消息进行过滤处理)
		2.检查是否按下Tab键，设置下一个获得焦点的控件
		3.处理Alt+Shortcut Key按下后的控件获得焦点和激活的设置
		4.检查是否有系统键消息，有则发送获得焦点的控件的事件*/
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	void UsedVirtualWnd(bool bUsed);

private:
	//将所有的控件添加到m_mNameHash哈希表中
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
	// 计算控件数量
    static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
	//根据点是否在区域中，查询控件
    static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
	//通过Tab信息查询控件
    static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
	//从快照中查询控件
    static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
	//查找需要更新的控件
    static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
	// 通过名称比较查询控件
    static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:
	HWND m_hWndPaint;//要CPaintManagerUI进行Direct绘图操作的窗体句柄
	int m_nOpacity;
	//绘图设备
	HDC m_hDcPaint;//直接绘制到窗体的DC(为窗体的整个区域包括非客户区)
	HDC m_hDcOffscreen;//内存缓冲区绘图DC
	HDC m_hDcBackground;//背景绘制(支持AlphaBackground时使用)
	//位图
	HBITMAP m_hbmpOffscreen;//m_hDcPaint的后台作图画布
	HBITMAP m_hbmpBackground;//背景图片bmp
	HWND m_hwndTooltip;//提示窗口句柄
	TOOLINFO m_ToolTip;// 提示消息,win类
	//控件信息
	CControlUI* m_pRoot;// xml根节点解析成的对象，通常为各种Window
	CControlUI* m_pFocus;//处于获得焦点状态的控件
	CControlUI* m_pEventHover;//处于鼠标悬停状态的控件
	CControlUI* m_pEventClick;//被鼠标点击的控件
	CControlUI* m_pEventKey;//接收键盘输入的控件
	//位置记录信息
	POINT m_ptLastMousePos;//鼠标最新的位置
	SIZE m_szMinWindow;//设置窗体可以调整到的最小大小
	SIZE m_szMaxWindow;//窗体可以调整到的最大大小
	SIZE m_szInitWindowSize;// 窗体初始化是的大小
	RECT m_rcSizeBox;//窗体外边框区域的大小
	SIZE m_szRoundCorner;// 窗体四角的圆角弧度
	RECT m_rcCaption;//窗体标题栏区域大小
	UINT m_uTimerID;//当前定时器ID
	// 标识类信息 
	bool m_bShowUpdateRect;//是否显示更新区域
    bool m_bFirstLayout;
    bool m_bUpdateNeeded;
    bool m_bFocusNeeded;
    bool m_bOffscreenPaint;
    bool m_bAlphaBackground;
    bool m_bMouseTracking;
    bool m_bMouseCapture;
	bool m_bUsedVirtualWnd;
	/*o---m_bFirstLayout             是否是首个布局 
	o---m_bUpdateNeeded            是否需要更新界面
	o---m_bFocusNeeded             是否需要焦点
	o---m_bOffscreenPaint          是否需要开双缓存绘图
	o---m_bAlphaBackground         窗体背景是否需要支持Alpha通道(如png图片的半透明效果)
	o---m_bMouseTracking           是否需要支持鼠标追踪
	o---m_bMouseCapture            是否需要支持鼠标捕获*/

	//集合类信息
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
	/*  o---m_aNotifiers               能够接收通知的对象集合
		o---m_aTimers                  定时器集合
		o---m_aPreMessage              预处理消息集合
		o---m_aPreMessageFilters       预处理消息过滤器集合
		o---m_aMessageFilters          消息过滤器集合
		o---m_aPostPaintControls       发送绘制请求的控件集合
		o---m_aDelayedCleanup          延迟清理的对象集合
		o---m_aAsyncNotify             异步通知消息集合
		o---m_mNameHash                名称HashMap
		o---m_mOptionGroup             选项组Map*/

	//xml对应资源
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
	/*o---m_pParentResourcePM        上级(父类)资源的PaintManagerUI绘图管理器
	o---m_dwDefaultDisabledColor   默认失效状态颜色
	o---m_dwDefaultFontColor       默认字体颜色
	o---m_dwDefaultLinkFontColor   默认超链接字体颜色
	o---m_dwDefaultLinkHoverFontColor默认超链接鼠标悬停状态的字体颜色
	o---m_dwDefaultSelectedBkColor 默认选中状态背景色
	o---m_DefaultFontInfo          默认字体信息
	o---m_aCustonFonts             自定义字体资源集合
	o---m_mImageHash               图片资源HashMap
	o---m_DefaultAttrHash          DefaultAttr资源HashMap*/

	//窗体句柄
	static HINSTANCE m_hInstance;//当前管理的Instance实例
	static HINSTANCE m_hResourceInstance;//当前管理的资源DLL Instance实例
	static CDuiString m_pStrResourcePath;//当前使用的资源路径
	static CDuiString m_pStrResourceZip;//当前使用的资源压缩包文件全称
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
//	UINT cbSize;                //该结构体的大小 sizeof(TOOLINFO)
//	UINT uFlags;                //附加标识类信息
//	HWND hwnd;                  //消息接受的窗体
//	UINT_PTR uId;               //控件ID
//	RECT rect;                  //消息产生的区域位置
//	HINSTANCE hinst;            //消息接收的实例
//	LPSTR lpszText;             //提示消息
//	LPARAM lParam;              //IE3.0以上的版本有该属性
//	void *lpReserved;           //NT5.0以上的版本有该属性 附加信息
//} TOOLINFO

//TFontInfo{
//	hFont               该字体的句柄
//		sFontName           字体名称
//		iSize               字号
//		bBold               是否粗体
//		bUnderline          是否有下划线
//		bItalic             是否为斜体
//		TEXTMETRIC tm       该字体的TEXTMETRIC信息
//}

#endif // __UIMANAGER_H__
