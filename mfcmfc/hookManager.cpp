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
	GetMessage(&msg, NULL, NULL, NULL);	// ��ũ �޼��� send
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

	thread = AfxBeginThread(ThreadUpdata, param);
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
	// Ű���带 �������� �ٷ� ȣ��
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		// code�� 0���� Ŭ���� ó���ؾ��ϸ� wParam = 256�� Ű���带 ������ �̺�Ʈ�� ���� �̺�Ʈ�� ������ �̺�Ʈ�� ���
		if (code >= 0 && (int)wParam == 256)
		{
			// lParam�� ����Ű�� ������ Ű�ڵ带 �д´�.
			if (pKey->vkCode == ',') return 1; // print screen Ű����
			cout << (char)pKey->vkCode << "\n";
		}
	}

	CallNextHookEx(hHook, code, wParam, lParam);	//���� ��ũ�� �Ѿ

	return 0;
}

void hookManager::Start_Hook()
{
	// ���� ����� �ڵ��� ������ hook�� �����Ѵ�.
	HMODULE hInstance = GetModuleHandle(NULL);

	//Ű���� ��ŷ
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);

}

void hookManager::Stop_Hook()
{
	if (hHook)
	{	
		UnhookWindowsHookEx(hHook);	// ������ ��ũ�� ����
		hHook = NULL;
	}
}

