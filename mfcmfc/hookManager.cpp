#include "pch.h"
#include "hookManager.h"

HHOOK hookManager::hHook = NULL;

hookManager::hookManager()
{
}

hookManager::~hookManager()
{
}

void hookManager::keyBoardHookLoad()
{
	Start_Hook();
	MSG msg;
	GetMessage(&msg, NULL, NULL, NULL);	// 후크 메세지 send
	Stop_Hook();
}

void hookManager::checkThread()
{
	StartThread();
}

void hookManager::StartThread()
{
	hookManager* param = new hookManager;
	param = this;

	thread = AfxBeginThread(ThreadUpdata, param);	// 스레드 실행
}

void hookManager::suspendThread()
{
	if (thread != NULL) thread->SuspendThread();
	Stop_Hook();
}

UINT hookManager::ThreadUpdata(LPVOID aParam)
{
	hookManager* pThis = (hookManager*)aParam;
	pThis->keyBoardHookLoad();

	return 0;
}

LRESULT hookManager::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	// 키보드를 눌렀을때 바로 호출
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		// code가 0보다 클때만 처리해야하며 
		// wParam = 256는 키보드를 누르는 이벤트와 떼는 이벤트중 누르는 이벤트만 통과
		if (code >= 0 && (int)wParam == 256)
		{
			// lParam이 가리키는 곳에서 키코드를 읽는다.
			if (pKey->vkCode == ',') return 1; // print screen 키차단
			cout << "현재 누른 키 : " << (char)pKey->vkCode << "\n";	// keyCode 출력 확인용
		}
	}
	CallNextHookEx(hHook, code, wParam, lParam);	//다음 HOOK으로 넘어감

	return 0;
}

void hookManager::Start_Hook()
{
	// 현재 모듈의 핸들을 가져와 hook을 셋팅한다.
	HMODULE hInstance = GetModuleHandle(NULL);

	//키보드 후킹
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);

}

void hookManager::Stop_Hook()
{
	if (hHook)
	{	
		UnhookWindowsHookEx(hHook);	// 윈도우 hooking 종료
		hHook = NULL;
	}
}

