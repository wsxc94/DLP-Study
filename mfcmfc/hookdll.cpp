#include "pch.h"

#define DEF_PROCESS_NAME "notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: { // �޸𸮿� DLL �ε��� ȣ��
		MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);
	}
	case DLL_THREAD_ATTACH:	   // DLL ��� ���μ����� ���ο� �����忡�� DLL �ε���
	case DLL_THREAD_DETACH:	   // DLL ��� ���μ����� ���ο� �����忡�� DLL ������
	case DLL_PROCESS_DETACH:   // �޸𸮿��� DLL ������ ȣ��
		break;
	}
	return TRUE;
}
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	char szPath[MAX_PATH] = { 0, };
	char* p = NULL;

	if (nCode >= 0)
	{
		if (!(lParam & 0x800000000))
		{
			GetModuleFileNameA(NULL, szPath, MAX_PATH);
			p = strrchr(szPath, '\\');

			cout << "����" << "\n";
			if (!strcmp(p + 1, DEF_PROCESS_NAME)) return 1;

		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	__declspec(dllexport) void HookStart() {
		g_hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
	}
	__declspec(dllexport) void HookStop() {
		if (g_hHook)
		{
			UnhookWindowsHookEx(g_hHook);
			g_hHook = NULL;
		}
	}
#ifdef __cplusplus
}
#endif

//int _tmain(int argc, TCHAR* argv[]) {
//
//	cout << "����" << "\n";
//	wstring process_name = L"notepad.exe";	
//	wstring dll_name = L"C:\\hookdll.dll";
//	DWORD pid = 0;
//
//		if (KEEPER_MANAGER->Get_InjectMng()->process_name_to_pid(pid, process_name)) {
//			KEEPER_MANAGER->Get_InjectMng()->dll_injection(pid, dll_name);
//		}
//}