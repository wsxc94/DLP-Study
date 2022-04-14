// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "UrlHook.h"
#include <Oleacc.h>
#pragma comment( lib,"Oleacc.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE HookDLLDLL = { false, nullptr };

std::unique_ptr<UrlHook> urlhook;

HWINEVENTHOOK LHook = 0;

void CALLBACK UrlEventHookProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {

	IAccessible* pAcc = NULL;
	VARIANT varChild;
	HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &varChild);

	if ((hr == S_OK) && (pAcc != NULL)) {
		BSTR bstrValue;
		pAcc->get_accValue(varChild, &bstrValue);

		char className[500];
		GetClassName(hwnd, (LPWSTR)className, 500);

		if (event == EVENT_OBJECT_VALUECHANGE) {

			/*

		   Window classe name of each browser =>

			Safari => SafariTaskbarTabWindow
			Chrome => Chrome_WidgetWin_1
			IE => IEFrame
			Firefox => MozillaWindowClass
			Opera => OperaWindowClass

			*/

			if (strcmp(className, "Chrome_WidgetWin_1") != 0) {
				printf("Active URL: %ls\n", bstrValue);
			}
		}
		SysFreeString(bstrValue);
		pAcc->Release();
	}
}

void Hook() {

	if (LHook != 0) return;
	CoInitialize(NULL);
	LHook = SetWinEventHook(EVENT_OBJECT_FOCUS, EVENT_OBJECT_VALUECHANGE, 0, UrlEventHookProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

void Unhook() {

	if (LHook == 0) return;
	UnhookWinEvent(LHook);
	CoUninitialize();
}

DWORD WINAPI ThreadProc(LPVOID IParam) {

	//while (true)
	//{
	//	Sleep(500);
	//	urlhook->WidgetLoad();
	//}
	MessageBox(nullptr, L"thread on", L"ThreadProc", MB_OK);

	MSG msg;
	Hook();

	do
	{
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) break;

	} while (msg.message != WM_QUIT);
	

	Unhook();

	return 0;
}

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	HANDLE hThread = NULL;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: { // 메모리에 DLL 로딩시 호출
		MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);
		urlhook = make_unique<UrlHook>();
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
		CloseHandle(hThread);
		break;
	}
	case DLL_THREAD_ATTACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 로딩시
		break;
	case DLL_THREAD_DETACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 해제시
		break;
	case DLL_PROCESS_DETACH: {  // 메모리에서 DLL 해제시 호출
		MessageBox(nullptr, L"dll exit", L"dll injection", MB_OK);
		SuspendThread(hThread);
		break;
	}
	}
	return TRUE;
}