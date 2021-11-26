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
	//int cnt = 0;
	while (true)
	{
		//if (cnt > 5) break;
		Sleep(500);
		urlhook->WidgetLoad();
		//cnt++;
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
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
		CloseHandle(hThread);
		break;
	}
	case DLL_THREAD_ATTACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 로딩시
		//MessageBox(nullptr, L"thread attach", L"dll injection", MB_OK);
		break;
	case DLL_THREAD_DETACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 해제시
		//MessageBox(nullptr, L"thread detach", L"dll injection", MB_OK);
		break;
	case DLL_PROCESS_DETACH: {  // 메모리에서 DLL 해제시 호출
		TRACE("DLL 프로세스 종료");
		MessageBox(nullptr, L"dll exit", L"dll injection", MB_OK);
		SuspendThread(hThread);
		break;
	}
	}
	return TRUE;
}