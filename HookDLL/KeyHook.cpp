#include "pch.h"
#include "KeyHook.h"

CWinThread* thread = NULL;
HHOOK hHook = NULL;

void keyBoardHookLoad()
{
	Start_Hook();
	MSG msg;
	GetMessage(&msg, NULL, NULL, NULL);	// ��ũ �޼��� send
	Stop_Hook();
}

void checkThread()
{
	StartThread();
}

void StartThread()
{
	thread = AfxBeginThread(ThreadUpdata, NULL);
}

void suspendThread()
{
	if (thread != NULL) thread->SuspendThread();
	Stop_Hook();
}

UINT ThreadUpdata(LPVOID aParam)
{
	
	keyBoardHookLoad();

	return 0;
}

LRESULT KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
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
			std::cout << (char)pKey->vkCode << "\n";
		}
	}

	CallNextHookEx(hHook, code, wParam, lParam);	//���� ��ũ�� �Ѿ

	return 0;
}

void Start_Hook()
{
	// ���� ����� �ڵ��� ������ hook�� �����Ѵ�.
	HMODULE hInstance = GetModuleHandle(NULL);

	//Ű���� ��ŷ
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);

}

void Stop_Hook()
{
	if (hHook)
	{
		UnhookWindowsHookEx(hHook);	// ������ ��ũ�� ����
		hHook = NULL;
	}
}

