#include "pch.h"

#define DEF_PROCESS_NAME "notepad.exe"

HINSTANCE g_hInstance = NULL;
HHOOK g_hHook = NULL;
HWND g_hWnd = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: { // 메모리에 DLL 로딩시 호출
		MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);
	}
	case DLL_THREAD_ATTACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 로딩시
	case DLL_THREAD_DETACH:	   // DLL 사용 프로세스의 새로운 스레드에서 DLL 해제시
	case DLL_PROCESS_DETACH:   // 메모리에서 DLL 해제시 호출
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

			cout << "누름" << "\n";
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
//	cout << "실행" << "\n";
//	wstring process_name = L"notepad.exe";	
//	wstring dll_name = L"C:\\hookdll.dll";
//	DWORD pid = 0;
//
//		if (KEEPER_MANAGER->Get_InjectMng()->process_name_to_pid(pid, process_name)) {
//			KEEPER_MANAGER->Get_InjectMng()->dll_injection(pid, dll_name);
//		}
//}