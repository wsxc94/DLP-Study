// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "UrlHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE HookDLLDLL = { false, nullptr };

std::unique_ptr<UrlHook> urlhook;
DWORD WINAPI ThreadProc(LPVOID IParam) {
	while (true)
	{
		Sleep(500);
		urlhook->WidgetLoad();
	}
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