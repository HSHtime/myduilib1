#ifdef WEBKITDLL_EXPORTS
#define WEBKITDLL_API __declspec(dllexport)
#else
#define WEBKITDLL_API __declspec(dllimport)
#endif
#include <ObjIdlbase.h>
#include "callback.h"
//����webkit�������������
class  webkitArg
{

public:
	webkitArg(){ flagsetRect = false; cmdshow = SW_SHOW; }
	//����webkit��ʾ����
	void setRect(RECT &r){ rect.right = r.right; rect.left = r.left; rect.bottom = r.bottom; rect.top = r.top; flagsetRect = true; }
	void setRect(LONG left, LONG top, LONG right, LONG bottom){
		rect.right = right; rect.left = left; rect.bottom = bottom; rect.top = top; flagsetRect = true;
	}
	HWND hmainwnd;//�����ھ��
	int cmdshow;//������ʾģʽ

	bool flagsetRect;
	RECT rect;
};
#ifdef __cplusplus
extern "C"
{
#endif
	WEBKITDLL_API int WebKit_BrowserWnd(webkitArg* arg);//����webkit����
	WEBKITDLL_API int WebKit_loadHTMLString(const char* sHtml);//����html�ַ���
	WEBKITDLL_API int WebKit_loadURL(const char* sHtml);//������ַ���ļ�·��
	WEBKITDLL_API HRESULT createRequest(const char* method, const char* url);//��������Ĭ������
	WEBKITDLL_API HRESULT setRequestValue(const char* value, const char* filed);//����Ĭ������ͷ�����ֵ
	WEBKITDLL_API HRESULT addRequestValue(const char* value, const char* filed);//���Ĭ���������ͷ�����ֵ
	WEBKITDLL_API HRESULT setRequestHTTPBody(IStream *pStream);//���Ĭ�������http body
	WEBKITDLL_API HRESULT loadRequest();//����Ĭ������
	WEBKITDLL_API HRESULT setUserAgent(const char* userAgent);//�������������userAgent
	WEBKITDLL_API HRESULT setPolicyForNACallBack(callback_t f);
	WEBKITDLL_API HRESULT setResLoadWSRCallBack(callback_t f);
#ifdef __cplusplus
}
#endif
