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
	SetHook();
	MSG msg;
	GetMessage(&msg, NULL, NULL, NULL);	// 후크 메세지 send
	UnHook();
}

void hookManager::SetHook()
{
	HMODULE hInstance = GetModuleHandle(NULL);
	// 현재 모듈의 핸들을 가져와 후크를 셋팅한다.
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);
}

void hookManager::UnHook()
{
	UnhookWindowsHookEx(hHook);	// 윈도우 후크를 종료
}

void hookManager::StartThread()
{
	hookManager* param = new hookManager;
	param = this;

	thread = AfxBeginThread(ThreadUpdata, param);
}

UINT hookManager::ThreadUpdata(LPVOID aParam)
{
	hookManager* pThis = (hookManager*)aParam;

	while (true)
	{
		Sleep(500);
		pThis->keyBoardHookLoad();
	}
	return 0;
}

LRESULT hookManager::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	// 키보드를 눌렀을때 바로 호출
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		// code가 0보다 클때만 처리해야하며 wParam = 256는 키보드를 누르는 이벤트와 떼는 이벤트중 누르는 이벤트만 통과
		if (code >= 0 && (int)wParam == 256)
		{
			// lParam이 가리키는 곳에서 키코드를 읽어 출력
			
			cout << (char)pKey->vkCode << " " << pKey->time << "\n";
		}
	}

	CallNextHookEx(hHook, code, wParam, lParam);	//다음 후크로 넘어감

	return 0;
}
