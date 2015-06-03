#ifdef WEBKITDLL_EXPORTS
#define WEBKITDLL_API __declspec(dllexport)
#else
#define WEBKITDLL_API __declspec(dllimport)
#endif
#include <ObjIdlbase.h>
#include "callback.h"
//创建webkit窗口所需参数类
class  webkitArg
{

public:
	webkitArg(){ flagsetRect = false; cmdshow = SW_SHOW; }
	//设置webkit显示区域
	void setRect(RECT &r){ rect.right = r.right; rect.left = r.left; rect.bottom = r.bottom; rect.top = r.top; flagsetRect = true; }
	void setRect(LONG left, LONG top, LONG right, LONG bottom){
		rect.right = right; rect.left = left; rect.bottom = bottom; rect.top = top; flagsetRect = true;
	}
	HWND hmainwnd;//主窗口句柄
	int cmdshow;//窗口显示模式

	bool flagsetRect;
	RECT rect;
};
#ifdef __cplusplus
extern "C"
{
#endif
	WEBKITDLL_API int WebKit_BrowserWnd(webkitArg* arg);//创建webkit界面
	WEBKITDLL_API int WebKit_loadHTMLString(const char* sHtml);//加载html字符串
	WEBKITDLL_API int WebKit_loadURL(const char* sHtml);//加载网址或文件路径
	WEBKITDLL_API HRESULT createRequest(const char* method, const char* url);//创建对象默认请求
	WEBKITDLL_API HRESULT setRequestValue(const char* value, const char* filed);//设置默认请求头部域的值
	WEBKITDLL_API HRESULT addRequestValue(const char* value, const char* filed);//添加默认请求额外头部域和值
	WEBKITDLL_API HRESULT setRequestHTTPBody(IStream *pStream);//添加默认请求的http body
	WEBKITDLL_API HRESULT loadRequest();//加载默认请求
	WEBKITDLL_API HRESULT setUserAgent(const char* userAgent);//设置所有请求的userAgent
	WEBKITDLL_API HRESULT setPolicyForNACallBack(callback_t f);
	WEBKITDLL_API HRESULT setResLoadWSRCallBack(callback_t f);
#ifdef __cplusplus
}
#endif
