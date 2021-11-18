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
	GetMessage(&msg, NULL, NULL, NULL);	// ��ũ �޼��� send
	UnHook();
}

void hookManager::SetHook()
{
	HMODULE hInstance = GetModuleHandle(NULL);
	// ���� ����� �ڵ��� ������ ��ũ�� �����Ѵ�.
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);
}

void hookManager::UnHook()
{
	UnhookWindowsHookEx(hHook);	// ������ ��ũ�� ����
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
	// Ű���带 �������� �ٷ� ȣ��
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		// code�� 0���� Ŭ���� ó���ؾ��ϸ� wParam = 256�� Ű���带 ������ �̺�Ʈ�� ���� �̺�Ʈ�� ������ �̺�Ʈ�� ���
		if (code >= 0 && (int)wParam == 256)
		{
			// lParam�� ����Ű�� ������ Ű�ڵ带 �о� ���
			
			cout << (char)pKey->vkCode << " " << pKey->time << "\n";
		}
	}

	CallNextHookEx(hHook, code, wParam, lParam);	//���� ��ũ�� �Ѿ

	return 0;
}
