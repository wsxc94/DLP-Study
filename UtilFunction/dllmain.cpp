// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE UtilFunctionDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved를 사용하는 경우 다음을 제거하십시오.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("UtilFunction.DLL을 초기화하고 있습니다.\n");
		// 확장 DLL을 한 번만 초기화합니다.
		if (!AfxInitExtensionModule(UtilFunctionDLL, hInstance))
			return 0;

		// 이 DLL을 리소스 체인에 삽입합니다.
		// 참고: 이 확장 DLL이 MFC 응용 프로그램이
		//  아닌 ActiveX 컨트롤과 같은 MFC 기본 DLL에
		//  의해 명시적으로 링크되어 있는 경우에는
		//  DllMain에서 이 줄을 제거하고, 제거한 줄은 이 확장 DLL에서
		//  내보낸 별도의 함수에 추가합니다.
		//  그런 다음 이 확장 DLL을 사용하는 기본 DLL은
		//  해당 함수를 명시적으로 호출하여 이 확장 DLL을 추가해야 합니다.
		//  그렇지 않으면 CDynLinkLibrary 개체가
		//  기본 DLL의 리소스 체인에 추가되지 않으므로
		//  심각한 문제가 발생합니다.

		new CDynLinkLibrary(UtilFunctionDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("UtilFunction.DLL을 종료하고 있습니다.\n");

		// 소멸자가 호출되기 전에 라이브러리를 종료합니다.
		AfxTermExtensionModule(UtilFunctionDLL);
	}
	return 1;   // 확인
}

#ifndef _DLL_UTIL_
#define _DLL_UTIL_
#endif // !_DLL_UTIL_

#include "UtilFunctionAPI.h"

DLL_UTIL char* util::ConvertWCtoC(wchar_t* str) // wchar_t -> char*
{
    char* pStr; // memory 해제 유의
    int size = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    pStr = new char[size];
    WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, size, 0, 0);
    return pStr;
}

DLL_UTIL std::string util::ConvertTCtoSTR(TCHAR* tc)
{
    int len = wcslen((wchar_t*)tc);
    char* psz = new char[2 * len + 1];

    wcstombs(psz, (wchar_t*)psz, 2 * len + 1);
    std::string str = psz;
    return str;
}

DLL_UTIL wchar_t* util::ConvertCharToWC(char* str)
{
    wchar_t* output = nullptr; {}

    int lenA = lstrlenA(str);
    int lenW = ::MultiByteToWideChar(CP_ACP, 0, str, lenA, NULL, 0);

    if (lenW > 0) {
        output = new wchar_t[lenW];
        ::MultiByteToWideChar(CP_ACP, 0, str, lenA, output, lenW);
    }

    return output;
}

DLL_UTIL bool util::compareString(std::string a, std::string b)
{
    //a , b 대문자 변환
    transform(a.begin(), a.end(), a.begin(), ::toupper);
    transform(b.begin(), b.end(), b.begin(), ::toupper);

    //a , b 비교
    if (a == b) return true;
    return false;
}

DLL_UTIL HWND util::GetHwndFromProcessHandle(HANDLE hProcess)
{
    if (hProcess == NULL) return NULL;

    __int64 lltmp = 0; // 64bit 를 상위 하위 나누어 쓴다
    lltmp = (int)GetProcessIDbyProcessHandle(hProcess); // GetProcessId()

    EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out

    if (((int)(lltmp >> 32)))
        return (HWND)(int)lltmp;

    return NULL;
}

DLL_UTIL HWND util::GetHwndFromProcessID(DWORD dwProcessID)
{
    if (dwProcessID == 0 || dwProcessID == -1)
        return NULL;

    __int64 lltmp = 0; // 64bit 를 상위 하위 나누어 쓴다
    lltmp = (int)dwProcessID;

    EnumWindows(CallbackEnumWindowProc, (LPARAM)&lltmp); // lltmp In/Out

    if (((int)(lltmp >> 32)))
        return (HWND)(int)lltmp;

    return NULL;
}

DLL_UTIL BOOL util::CallbackEnumWindowProc(HWND hWnd, LPARAM lParam)
{
    HANDLE hTarget = NULL;
    DWORD dwID = 0, dwSrcID = (DWORD)(*(int*)lParam);

    GetWindowThreadProcessId(hWnd, &dwID);

    if (dwID == dwSrcID)
    {
        *((int*)lParam) = (int)hWnd;
        *(((int*)lParam) + 1) = 1;
        return FALSE;
    }

    return TRUE;
}

DLL_UTIL DWORD util::GetProcessIDbyProcessHandle(HANDLE hProcess)
{
    // http://www.codeproject.com/Articles/21926/Getting-Process-ID-from-Process-Handle

    // [in]  process handle
    // [out] process ID, or 0xffffffff in case of failure

    if (hProcess == NULL)    return 0xffffffff;
    PTHREAD_START_ROUTINE lpStartAddress = (PTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "GetCurrentProcessId");
    if (lpStartAddress == NULL) return 0xffffffff;
    // We do not know, whether process handle already has required access rights;

    // thus we have to duplicate it
    HANDLE hProcessAccAdj;
    BOOL bRes = DuplicateHandle(GetCurrentProcess(),
        hProcess, GetCurrentProcess(), &hProcessAccAdj,
        PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
        FALSE, 0);
    if (!bRes || hProcessAccAdj == NULL)
    {
        UINT unError = GetLastError();
        return 0xffffffff;
    }
    // Create a remote thread; as its starting address 

    // we specify GetCurrentProcessId() address,
    // which is the same for all processes. Note that GetCurrentProcessId() has no input
    // parameters, and we don't care about our thread stack cleanup,
    // as it will be destroyed right after this call

    DWORD dwThreadID;
    HANDLE hRemoteThread = CreateRemoteThread(hProcessAccAdj, NULL,
        0, lpStartAddress, 0, 0, &dwThreadID);
    CloseHandle(hProcessAccAdj);
    if (hRemoteThread == NULL) return 0xffffffff;
    // Wait until process ID is obtained

    // (replace INFINITE value below to a smaller value to avoid deadlocks);
    // then get the thread exit code, which is a value returned by GetCurrentProcessId()
    // in the context of the remote process
    WaitForSingleObject(hRemoteThread, INFINITE);
    DWORD dwExitCode;
    if (GetExitCodeThread(hRemoteThread, &dwExitCode) == 0)    dwExitCode = 0xffffffff;
    CloseHandle(hRemoteThread);
    return dwExitCode;
}

DLL_UTIL HWND util::GetHwndFormProcessID(ULONG pid)
{
    HWND tempHwnd = FindWindow(NULL, NULL); // 최상위 윈도우 핸들 찾기  

    while (tempHwnd != NULL)
    {
        if (GetParent(tempHwnd) == NULL) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해  
            if (pid == ProcIDFromWnd(tempHwnd))
                return tempHwnd;
        tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기  
    }
    return NULL;
}

DLL_UTIL ULONG util::ProcIDFromWnd(HWND hwnd)
{
    ULONG idProc;
    GetWindowThreadProcessId(hwnd, &idProc);
    return idProc;
}
