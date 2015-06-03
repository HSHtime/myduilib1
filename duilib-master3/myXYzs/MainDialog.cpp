#include "stdafx.h"
#include "MainDialog.h"
#include "DialogBuilderCBEx.h"
#include "FeedbackWnd.h"
#include "MyWebHandle.h"
#include <Shlwapi.h>
#include <curl/curl.h>
#include <process.h>
#include "Base64Coder.h"
#include "json/json.h"
#include "comutil.h"
#include "atlconv.h"
//#include "../FlashDemo/flash10a.tlh"

#pragma comment(lib,"json_vc71_libmtd.lib")
CProgressUI* gpDownLoadProgress = NULL;
HWND gwnd;
size_t write_data(char *buffer, size_t size, size_t nitems, void *outstream)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)outstream);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}
	str->append(buffer, size * nitems);
	return nitems;
}
size_t write_file(char *buffer, size_t size, size_t nitems, void *outstream)
{

	int written = fwrite(buffer, size, nitems, (FILE*)outstream);
	return written;
}
int wxcurldav_dl_progress_func(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow)  
{                                   
	if(gpDownLoadProgress)
	{
		//设置进度条的值
		char a[50];
		int progress = 100 * (rDlNow/rDlTotal);
		sprintf_s(a,"%d",progress);
		gpDownLoadProgress->SetValue(progress); 
		gpDownLoadProgress->SetText(a);
	}
	return 0; 
} 
string UTF8toANSI(const string& str)
{
	//utf8转unicode
	int unicodeLen = MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,NULL,0);
	wchar_t * pUnicode; 
	pUnicode = new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	::MultiByteToWideChar( CP_UTF8,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen );
	//unicode转ANSI
	int iTextLen = WideCharToMultiByte(CP_ACP,0,pUnicode,-1,NULL,0,NULL,NULL);
	char* pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,0,pUnicode,-1,pElementText,iTextLen,NULL,NULL );
	string ret(pElementText);
	delete [] pElementText;
	delete [] pUnicode;
	return ret;
}
void DownloadCallBackThread(LPVOID pParam)
{
	string* arg = (string*)pParam;
	if (arg->find("getDownLoadaddr") != string::npos)
	{
		CURL* pCurl = curl_easy_init();
		string msg;
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void*)&msg);
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(pCurl, CURLOPT_URL, arg->c_str());
		curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);
		Json::Reader jsReader;
		Json::Value  jsRoot;
		string resurl;
		string title;
		if (jsReader.parse(msg,jsRoot))
		{
			 resurl = jsRoot["resurl"].asString();
			title = jsRoot["title"].asString();
			//const char * fn = resurl.c_str();
			//MessageBox(NULL, fn, NULL, MB_OK);
			 //fn = title.c_str();			
			//MessageBox(NULL, pElementText, NULL, MB_OK);
			//return;
		}
		else
		{
				MessageBox(NULL, "json failed.", NULL, MB_OK);
		}
		
		/*int urlbegin = msg.find("\"resurl\":")+10;
		int urlend = msg.find("\",", urlbegin);
		int length = urlend - urlbegin;
		string tempurl = msg.substr(urlbegin, length);
		*/
		CBase64Coder downUrl;
		const char *url = downUrl.decode(resurl.c_str(), resurl.length());
		string decName(url);
		int fnB = decName.rfind('.');
		title = UTF8toANSI(title);
		string filename = title + decName.substr(fnB, decName.length() - fnB);
		string dir("e:\\downloadtest\\");
		filename = dir + filename; 
		//MessageBox(NULL, decName.c_str(), NULL, MB_OK);
		//return;
		//下载
		CURL* pCurl2 = curl_easy_init();

		FILE* pFile = fopen(filename.c_str(), "wb");
		curl_easy_setopt(pCurl2, CURLOPT_WRITEDATA, (void*)pFile);
		//设置回调函数
		curl_easy_setopt(pCurl2, CURLOPT_WRITEFUNCTION, write_file);
		curl_easy_setopt(pCurl2, CURLOPT_URL, url);
		//curl的进度条声明                   
		curl_easy_setopt(pCurl2, CURLOPT_NOPROGRESS, FALSE);                  
		//回调进度条函数                   
		curl_easy_setopt(pCurl2, CURLOPT_PROGRESSFUNCTION, wxcurldav_dl_progress_func);                  
		//设置进度条名称                   
		curl_easy_setopt(pCurl2, CURLOPT_PROGRESSDATA, gpDownLoadProgress);                    
		//进度条 
		if (gpDownLoadProgress == NULL)
		{
			MessageBox(NULL,_T("progress wrong"), NULL, MB_OK);
			return;
		}
		gpDownLoadProgress->SetValue(0);
		curl_easy_perform(pCurl2);
		fclose(pFile);
		gpDownLoadProgress->SetValue(100);
		curl_easy_cleanup(pCurl2);
		MessageBox(NULL,_T("finshed"), NULL, MB_OK);
		delete arg;
	}

}
void myDownload(void* para)
{
	char *c = ( char*)para;
	string *str = new string(c);
	/*string temp("updatedownload");
	int ret = str->find(temp);*/
	/*BSTR msg = _bstr_t(str->c_str());
	MessageBox(NULL,msg, NULL, MB_OK);*/
	//if (ret != string::npos)
	{
		//string tstr = str.erase(str.find("http://"), 7);

		_beginthread(DownloadCallBackThread, 0, str);
	//	MessageBox(NULL, L"finish", NULL, MB_OK);
	}

}
void    CMainDialog::Notify(TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("btn_close")) 
		{
			PostQuitMessage(0);
		}
		else if(msg.pSender->GetName() == _T("btn_min")) 
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if(msg.pSender->GetName() == _T("btn_main_feedback")) 
		{
			CFeedbackWnd fbwnd;			
			fbwnd.Create(this->m_hWnd, _T("feedbackWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);		
			fbwnd.CenterWindow();
			fbwnd.ShowModal();

			//CFeedbackWnd *fbwnd= new CFeedbackWnd;
			//fbwnd->Create(this->m_hWnd, _T("feedbackWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
			//fbwnd->CenterWindow();
			//fbwnd->ShowModal();

		}
	}
	else if(msg.sType==_T("selectchanged"))
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_page")));
		
		if(name==_T("op_main"))
			pControl->SelectItem(0);
		else if(name==_T("op_app_web"))
		{
		  WebKit_loadURL("http://software.xyzs.com/v3/iphone");
		/*   pActiveXUI->Navigate2(_T("http://software.xyzs.com/v3/iphone"));	
			pControl->SelectItem(0);*/
		 // gpDownLoadProgress->SetValue(10); 
		}
		else if(name==_T("op_game_web"))
		{
			WebKit_loadURL("http://software.xyzs.com/v3/iphone/game");
		   /* pActiveXUI->Navigate2(_T("http://software.xyzs.com/v3/iphone/game"));
			pControl->SelectItem(0);*/
		//	gpDownLoadProgress->SetValue(20); 
		}
		else if(name==_T("op_wallpaper_web"))
		{
			WebKit_loadURL("http://software.xyzs.com/wallpaper");
			/*pActiveXUI->Navigate2(_T("http://software.xyzs.com/wallpaper"));
			pControl->SelectItem(0);*/
		//	gpDownLoadProgress->SetValue(3); 
		}
		else if(name==_T("op_slim"))
			pControl->SelectItem(1);
		//	setRequest();
		else if(name==_T("op_tools"))
			pControl->SelectItem(2);
		}
	
		
	
}


LRESULT CMainDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		//WM_NCCALCSIZE消息，通知应用程序窗口的位置或者大小变了
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		//WM_NCHITTEST消息用来获取鼠标当前命中的位置,实现拖动窗口
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	//case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainDialog::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//屏蔽系统标题栏，有边框。与WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT共用可以去掉边框
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb(&m_PaintManager);//回调创建子窗口
	CControlUI* pRoot = builder.Create(_T("Main.xml"), (UINT)0, &cb, &m_PaintManager);   // duilib.xml需要放到exe目录下
	ASSERT(pRoot && "Failed to parse XML");

	m_PaintManager.AttachDialog(pRoot);//将对话框控件附加到当前的管理器中
	m_PaintManager.AddNotifier(this); 
	m_pWebPage = NULL;

	// 初始化CActiveXUI控件
	//CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("app_web")));
   // pActiveXUI = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("app_web")));
	//if( pActiveXUI ) 
	{
		webkitArg arg;
		arg.cmdshow = SW_SHOWDEFAULT;
		arg.hmainwnd =this->m_hWnd;
		gwnd = this->m_hWnd;
		RECT clientRect = { 1, 108, 1001, 688};
		//GetClientRect(this->m_hWnd, &clientRect);
		arg.setRect(clientRect);
		WebKit_BrowserWnd(&arg);
		setResLoadWSRCallBack(myDownload);
		gpDownLoadProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("YXDownLoadProgress")));
		gpDownLoadProgress->SetValue(0);
		//pActiveXUI->SetDelayCreate(false);
		//CMyWebHandle *pWebHandle = new CMyWebHandle;
		//pActiveXUI->SetWebBrowserEventHandler(pWebHandle);
		//pActiveXUI->Navigate2(_T("about:blank"));    //这行代码，如果注释掉，就不会去掉边框，IE有bug，第二次加载网页才会让事件处理器有效
		//pActiveXUI->Navigate2(_T("http://software.xyzs.com/v3/iphone"));
		
	}
	return 0;
}
LRESULT CMainDialog::setRequest()
{
	HRESULT hr;
	hr = createRequest("POST","https://p30-buy.itunes.apple.com/WebObjects/MZFinance.woa/wa/signupWizard?Pod=30&PRH=30");
	if (FAILED(hr))
		return hr;
	char* userAgentBStr = "iTunes/12.1.1 (Windows; Microsoft Windows 7 x64 Ultimate Edition Service Pack 1 (Build 7601); x64) AppleWebKit/7600.1017.9000.2";
	hr = setRequestValue(userAgentBStr, "User-Agent");
	if (FAILED(hr))
		return hr;

	char* ctypeBStr="multipart/form-data; boundary=5A501C1E11E391576E0C6F59C2A1A9CB";
	hr = setRequestValue(ctypeBStr, "Content-Type");
	if (FAILED(hr))
		return hr;
	char* xFrontBStr="143465-19,28";
	hr = setRequestValue(xFrontBStr, "X-Apple-Store-Front");

	if (FAILED(hr))
		return hr;
	char* acceptBStr="zh-cn, zh;q=0.75, en-us;q=0.50, en;q=0.25";
	hr = setRequestValue(acceptBStr, "Accept-Language");
	if (FAILED(hr))
		return hr;
	char* xactionBStr="AmK/5rCVkYJ+Wq4U/VirIkMH5T5odiKBBi6uhFXkaRvwAAAB0AMAAAACAAABAKvN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76vN76sAAAAR8gdehvlrHDbJZkIBM1L2a1AAAACjAqIcr/LwXL5y4Qttsq/LW7zYzjBaAAAAigMFhjNyxREUz0ag1fv7v0+fQFUVDAcAAADSl+m4tM0whcOdiOYRIGL9sO5valGlcnS76RTwfX0I2TLrdHoz98iNk8YvKuDROtwXsKEcWItM/grWamxNvUo/AiMKnDS1JzmCCJ5t5OmRv2AriIis3NaS/RP0fNJw6BmhlDn09phzXudMKRdSU7MNrgAAAAAAAAAAAAAAAQQCBzIA";
	hr = setRequestValue(xactionBStr,"X-Apple-ActionSignature");

	if (FAILED(hr))
		return hr;
	char* xTzBStr="28800";
	hr = setRequestValue(xTzBStr, "X-Apple-Tz");

	if (FAILED(hr))
		return hr;
	char* xCookieBStr="ns-mzf-inst=34-233-80-109-82-8291-82444-8-st11; wosid-lite=ROW4Q75jDMBi7zqBtXaZNg";
	hr = setRequestValue(xCookieBStr, "Cookie");
	if (FAILED(hr))
		return hr;

	FILE *fp;
	IStream *pStream;
	CreateStreamOnHGlobal(NULL, TRUE, &pStream);
	HRESULT fhr = SHCreateStreamOnFile("E:\\github\\WebKit2\\WebKitBuild\\Debug\\bin32\\post.txt", STGM_READ, &pStream);
	if (fhr == S_OK)
		setRequestHTTPBody(pStream);
	hr = loadRequest();
	return 0;
}
LRESULT CMainDialog::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_PaintManager.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}
LRESULT CMainDialog::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}
LRESULT CMainDialog::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainDialog::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CMainDialog::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}
LRESULT CMainDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}