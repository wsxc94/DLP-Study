#include "pch.h"
#include "util.h"

//char* util::ConvertWCtoC(wchar_t* str) // wchar_t -> char*
//{
//	char* pStr; // memory ���� ����
//	int size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
//	pStr = new char[size];
//	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, size, 0, 0);
//	return pStr;
//}
//
//wchar_t* util::ConvertCharToWC(char* str)
//{
//    wchar_t* output = nullptr; {}
//
//    int lenA = lstrlenA(str);
//    int lenW = ::MultiByteToWideChar(CP_ACP, 0, str, lenA, NULL, 0);
//
//    if (lenW > 0) {
//        output = new wchar_t[lenW];
//        ::MultiByteToWideChar(CP_ACP, 0, str, lenA, output, lenW);
//    }
//
//    return output;
//}
//
//bool util::compareString(string a, string b)
//{
//	//a , b �빮�� ��ȯ
//	transform(a.begin(), a.end(), a.begin(), ::toupper);
//	transform(b.begin(), b.end(), b.begin(), ::toupper);
//
//	//a , b ��
//	if (a == b) return true;
//	return false;
//}
//
//HWND util::GetHwndFromProcessHandle(HANDLE hProcess)
//{
//	if (hProcess == NULL) return NULL;
//
//	__int64 lltmp = 0; // 64bit �� ���� ���� ������ ����
//	lltmp = (int)GetProcessIDbyProcessHandle(hProcess); // GetProcessId()
//
//	EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out
//
//	if (((int)(lltmp >> 32)))
//		return (HWND)(int)lltmp;
//
//	return NULL;
//}
//
//HWND util::GetHwndFromProcessID(DWORD dwProcessID)
//{
//    if (dwProcessID == 0 || dwProcessID == -1)
//        return NULL;
//
//    __int64 lltmp = 0; // 64bit �� ���� ���� ������ ����
//    lltmp = (int)dwProcessID;
//
//    EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out
//
//    if (((int)(lltmp >> 32)))
//        return (HWND)(int)lltmp;
//
//    return NULL;
//}
//
//BOOL util::CallbackEnumWindowProc(HWND hWnd, LPARAM lParam)
//{
//    HANDLE hTarget = NULL;
//    DWORD dwID = 0, dwSrcID = (DWORD)(*(int*)lParam);
//
//    GetWindowThreadProcessId(hWnd, &dwID);
//
//    if (dwID == dwSrcID)
//    {
//        *((int*)lParam) = (int)hWnd;
//        *(((int*)lParam) + 1) = 1;
//        return FALSE;
//    }
//
//    return TRUE;
//}
//
//DWORD __stdcall util::GetProcessIDbyProcessHandle(HANDLE hProcess)
//{
//    // http://www.codeproject.com/Articles/21926/Getting-Process-ID-from-Process-Handle
//
//    // [in]  process handle
//    // [out] process ID, or 0xffffffff in case of failure
//
//    if (hProcess == NULL)    return 0xffffffff;
//    PTHREAD_START_ROUTINE lpStartAddress = (PTHREAD_START_ROUTINE)
//        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "GetCurrentProcessId");
//    if (lpStartAddress == NULL) return 0xffffffff;
//    // We do not know, whether process handle already has required access rights;
//
//    // thus we have to duplicate it
//    HANDLE hProcessAccAdj;
//    BOOL bRes = DuplicateHandle(GetCurrentProcess(),
//        hProcess, GetCurrentProcess(), &hProcessAccAdj,
//        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
//        PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
//        FALSE, 0);
//    if (!bRes || hProcessAccAdj == NULL)
//    {
//        UINT unError = GetLastError();
//        return 0xffffffff;
//    }
//    // Create a remote thread; as its starting address 
//
//    // we specify GetCurrentProcessId() address,
//    // which is the same for all processes. Note that GetCurrentProcessId() has no input
//    // parameters, and we don't care about our thread stack cleanup,
//    // as it will be destroyed right after this call
//
//    DWORD dwThreadID;
//    HANDLE hRemoteThread = CreateRemoteThread(hProcessAccAdj, NULL,
//        0, lpStartAddress, 0, 0, &dwThreadID);
//    CloseHandle(hProcessAccAdj);
//    if (hRemoteThread == NULL) return 0xffffffff;
//    // Wait until process ID is obtained
//
//    // (replace INFINITE value below to a smaller value to avoid deadlocks);
//    // then get the thread exit code, which is a value returned by GetCurrentProcessId()
//    // in the context of the remote process
//    WaitForSingleObject(hRemoteThread, INFINITE);
//    DWORD dwExitCode;
//    if (GetExitCodeThread(hRemoteThread, &dwExitCode) == 0)    dwExitCode = 0xffffffff;
//    CloseHandle(hRemoteThread);
//    return dwExitCode;
//}
//
//HWND util::GetHwndFormProcessID(ULONG pid)
//{
//    HWND tempHwnd = FindWindow(NULL, NULL); // �ֻ��� ������ �ڵ� ã��  
//
//    while (tempHwnd != NULL)
//    {
//        if (GetParent(tempHwnd) == NULL) // �ֻ��� �ڵ����� üũ, ��ư � �ڵ��� ���� �� �����Ƿ� �����ϱ� ����  
//            if (pid == ProcIDFromWnd(tempHwnd))
//                return tempHwnd;
//        tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // ���� ������ �ڵ� ã��  
//    }
//    return NULL;
//}
//
//ULONG util::ProcIDFromWnd(HWND hwnd)
//{
//    ULONG idProc;
//    GetWindowThreadProcessId(hwnd, &idProc);
//    return idProc;
//}
