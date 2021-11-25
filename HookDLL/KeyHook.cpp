#include "pch.h"
#include "KeyHook.h"

HHOOK KeyHook::hHook = NULL;

KeyHook::KeyHook() : thread(NULL)
{
}


KeyHook::~KeyHook()
{
}

void KeyHook::keyBoardHookLoad()
{
	Start_Hook();
	MSG msg;
	GetMessage(&msg, NULL, NULL, NULL);	// ��ũ �޼��� send
	Stop_Hook();
}

void KeyHook::checkThread()
{
	StartThread();
}

void KeyHook::StartThread()
{
	thread = AfxBeginThread(ThreadUpdata, NULL);
}

void KeyHook::suspendThread()
{
	if (thread != NULL) thread->SuspendThread();
	Stop_Hook();
}

UINT KeyHook::ThreadUpdata(LPVOID aParam)
{
	KeyHook* pThis = (KeyHook*)aParam;

	pThis->keyBoardHookLoad();

	return 0;
}

LRESULT KeyHook::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	// Ű���带 �������� �ٷ� ȣ��
	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
	{
		PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

		// code�� 0���� Ŭ���� ó���ؾ��ϸ� wParam = 256�� Ű���带 ������ �̺�Ʈ�� ���� �̺�Ʈ�� ������ �̺�Ʈ�� ���
		if (code >= 0 && (int)wParam == 256)
		{
			// lParam�� ����Ű�� ������ Ű�ڵ带 �д´�.
			if (pKey->vkCode == 'a') return 1; // print screen Ű����
			std::cout << (char)pKey->vkCode << "\n";
		}
	}

	CallNextHookEx(hHook, code, wParam, lParam);	//���� ��ũ�� �Ѿ

	return 0;
}

void KeyHook::Start_Hook()
{
	// ���� ����� �ڵ��� ������ hook�� �����Ѵ�.
	HMODULE hInstance = GetModuleHandle(NULL);

	//Ű���� ��ŷ
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, NULL);

}

void KeyHook::Stop_Hook()
{
	if (hHook)
	{
		UnhookWindowsHookEx(hHook);	// ������ ��ũ�� ����
		hHook = NULL;
	}
}

