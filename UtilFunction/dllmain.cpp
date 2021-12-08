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

BOOL util::GetProcessPathByPID(DWORD dwPID, CString& strProcessPath)
{
	BOOL bSuccess = FALSE;
	HANDLE hProcess = NULL;

	TCHAR szFileName[MAX_PATH] = { 0 };

	strProcessPath = _T("");

	try
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);

		if (hProcess != NULL)
		{
			//-------------------------------------------------------------------//
			//			종료된 프로세스인 경우 실패로 처리
			//-------------------------------------------------------------------//
			DWORD dwExitCode = 0;

			if (!GetExitCodeProcess(hProcess, &dwExitCode) ||
				dwExitCode != STILL_ACTIVE)
			{
				throw CString("Already Exited Process");
			}

			//-------------------------------------------------------------------------------------------------------//
			//		권한 문제 등으로 이름 얻는 경우가 실패할 수 있으므로 다양한 방법으로 시도
			//-------------------------------------------------------------------------------------------------------//
			if (GetModuleFileNameEx(hProcess, NULL, szFileName, _ARRAYSIZE(szFileName) - 1))
			{
				strProcessPath.Format(_T("%s"), szFileName);

				std::cout << "getmodulefilenameex" << "\n";
			}

			else if (GetProcessImageFileName(hProcess, szFileName, _ARRAYSIZE(szFileName) - 1))
			{
				strProcessPath.Format(_T("%s"), szFileName);

				GetPhysicalPathToLogicalPath(strProcessPath, strProcessPath);

				std::cout << "GetProcessImageFileName" << "\n";
			}
		}

		if (strProcessPath.IsEmpty())
		{
			std::cout << "snapshot" << "\n";
			//-------------------------------------------------------------------//
			//		스냅샵으로 프로세스 경로까지는 구할 수 없으나,
			//		프로세스 이름이라도 구하기 (CPU 점유율 증가 가능성 있음)
			//-------------------------------------------------------------------//
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			if (hSnapshot != INVALID_HANDLE_VALUE)
			{
				PROCESSENTRY32 pe32 = { 0 };
				pe32.dwSize = sizeof(PROCESSENTRY32);

				if (Process32First(hSnapshot, &pe32))
				{
					do
					{
						if (pe32.th32ProcessID == dwPID)
						{
							strProcessPath.Format(_T("%s"), pe32.szExeFile);

							break;
						}

					} while (Process32Next(hSnapshot, &pe32));
				}

				CloseHandle(hSnapshot);
			}
		}

		if (strProcessPath.IsEmpty())
			throw CString("Get Process Path Fail");

		//-------------------------------------------------------------------//
		//짧은 이름일 경우 긴 이름으로 변환
		//-------------------------------------------------------------------//
		if (strProcessPath.Find('~') != -1)
		{
			CString strLongName;

			if (GetShortToLongFileName(strProcessPath, strLongName) > (int)wcslen(szFileName))
				strProcessPath = strLongName;
		}

		bSuccess = TRUE;
	}
	catch (CString strError)
	{
		CString strLog; strLog.Format(_T("[Officekeeper] GetProcessPathByPID Throw %d - [ %s ]\n"), dwPID, strError);
		std::wcout << strLog << "\n";
	}
	catch (...)
	{
		CString strLog; strLog.Format(_T("[OfficeKeeper] GetProcessPathByPID Throw by Unknown reason - %d \n"), GetLastError());
		std::wcout << strLog << "\n";
	}

	//------------------------------------//
	//		리소스 정리
	//------------------------------------//
	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}

	return bSuccess;

}

BOOL util::IsNeedImpersonateAsUser(CString strFilePath)
{
	//------------------------------------//
	//		이미 위장이 된 상태인지 확인
	//------------------------------------//
	HANDLE hToken = NULL;

	if (OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, TRUE, &hToken))
	{
		CloseHandle(hToken);
		hToken = NULL;

		return FALSE;
	}

	//-------------------------------------------------------------------------------------------------------//
	//		파일 열기와 같이 공유위반 충돌이 발생할 수 있는 방법 외의 방법으로 권한 확인
	//-------------------------------------------------------------------------------------------------------//
	DWORD dwAttribute = GetFileAttributes(strFilePath);

	if (dwAttribute != INVALID_FILE_ATTRIBUTES)
	{
		//-------------------------------------------------------------------//
		//				암호화 파일 시스템(EFS)으로 보호된 파일
		//-------------------------------------------------------------------//
		if (dwAttribute & FILE_ATTRIBUTE_ENCRYPTED)
			return TRUE;
	}
	else
	{
		DWORD dwError = GetLastError();

		//-------------------------------------------------------------------//
		//		네트워크 드라이브를 비롯한 네트워크 경로의 파일
		//-------------------------------------------------------------------//
		if (dwError == ERROR_ACCESS_DENIED || dwError == ERROR_PATH_NOT_FOUND || dwError == ERROR_LOGON_FAILURE
			|| dwError == ERROR_BAD_NET_RESP || dwError == ERROR_NO_SUCH_LOGON_SESSION)
			return TRUE;
	}

	return FALSE;
}

BOOL util::ImpersonateAsUser()
{
	DWORD dwUserProcPID = 0;
	DWORD dwPIDList[4096] = { 0 };
	DWORD dwReturn = 0;

	if (!EnumProcesses(dwPIDList, sizeof(dwPIDList), &dwReturn))
		return FALSE;

	DWORD dwTotalCnt = dwReturn / sizeof(DWORD);

	for (UINT nCnt = 0; nCnt < dwTotalCnt; nCnt++)
	{
		DWORD dwPID = dwPIDList[nCnt];
		CString strProcessName;

		if (!GetProcessPathByPID(dwPID, strProcessName))
			continue;

		strProcessName = strProcessName.Mid(strProcessName.ReverseFind(_T('\\')) + 1);
		strProcessName.MakeLower();

		if (strProcessName == _T("explorer.exe"))
		{
			dwUserProcPID = dwPID;

			break;
		}
	}

	if (dwUserProcPID == 0)
		return FALSE;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwUserProcPID);

	if (hProc == NULL)
		return FALSE;

	HANDLE hToken = NULL;
	OpenProcessToken(hProc, TOKEN_ALL_ACCESS, &hToken);

	if (hToken == NULL)
	{
		CloseHandle(hProc);

		return FALSE;
	}

	if (!ImpersonateLoggedOnUser(hToken))
	{
		CloseHandle(hProc);
		CloseHandle(hToken);

		return FALSE;
	}

	CloseHandle(hToken);
	CloseHandle(hProc);

	return TRUE;
}

int util::GetShortToLongFileName(CString sShortPath, CString& sLongPath)
{
	int nRet = 0;

	//-------------------------------------------------------------------//
	//		사용자 권한으로 위장이 필요한지 확인과 위장
	//-------------------------------------------------------------------//
	BOOL bImpersonated = (IsNeedImpersonateAsUser(sShortPath) && ImpersonateAsUser());

	try
	{
		// Check whether the input path is valid.
		if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(sShortPath))
		{
			// Special characters.
			CString sep = _T("\\");
			CString colon = _T(":");
			CString sDrive, sCutPath, sTmpShort;
			int npos = sShortPath.GetLength();

			// Copy the short path into the work buffer and convert forward
			// slashes to backslashes.
			CString path = sShortPath;
			path.Replace(_T("/"), sep);

			// We need a couple of markers for stepping through the path.
			int left = 0;
			int right = 0;

			// Parse the first bit of the path.
			if (path.GetLength() >= 2 && isalpha(path[0]) && colon == path[1]) // Drive letter
			{
				if (2 == path.GetLength()) //drive letter
				{
					right = -1; // skip main block
				}
				else if (sep == path[2]) // drive letter + backslash
				{
					// FindFirstFile doesn like "X:\"
					if (3 == path.GetLength())
					{
						right = -1; // skip main block
					}
				}
				else
					throw 0; // parsing failure
			}
			else if (path.GetLength() >= 1 && sep == path[0])
			{
				if (1 == path.GetLength()) // 
				{
					right = -1; // skip main block
				}
				else
				{
					if (sep == path[1]) // is it UNC
					{
						// Find end of machine name
						right = path.Find(sep);

						if (right == -1)
							throw 0;
						// Find end of share name
						right = path.Find(sep);
						if (right == 1)
							throw 0;
					}
					++right;
				}
			}

			// else FindFirstFile will handle relative paths
			// The data block for FindFirstFile.
			WIN32_FIND_DATA fd;

			// extract the Drive-name:
			sDrive = path.Left(3);
			path.Delete(0, 3);
			sTmpShort = sDrive;
			sLongPath = sDrive;

			// Main parse block - step through path.
			while (right != -1)
			{

				// Find next separator.
				right = path.Find(sep);
				if (right == 0)
				{
					path.Delete(0, 1);
					sTmpShort += sep;
					right = path.Find(sep);
				}
				else if (right == -1 && path.GetLength() <= 0)
				{
					nRet = sLongPath.GetLength();

					throw 0;
				}

				// Temporarily replace the separator with a null character so that
				// the path so far can be passed to FindFirstFile.
				if (right == -1)
					sCutPath = path.Left(path.GetLength());
				else
					sCutPath = path.Left(right);

				path.Delete(0, right);

				sTmpShort += sCutPath;

				// See what FindFirstFile makes of the path so far.
				HANDLE hf = FindFirstFile(sTmpShort, &fd);

				if (INVALID_HANDLE_VALUE == hf)
					throw 0;

				FindClose(hf);

				// The file was found add it to the output path:
				if (right != -1)
					sLongPath += fd.cFileName + sep;
				else
					sLongPath += fd.cFileName;
			}

			// Check whether the output path is valid:
			if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(sLongPath))
				throw 0;

			// return the number of characters in the long path:
			nRet = sLongPath.GetLength();
		}
	}
	catch (...)
	{
		CString strLog; strLog.Format(_T("[OfficeKeeper] GetShortToLongFileName Throw - %d \n"), GetLastError());
	}

	//------------------------------------//
	//		사용자 권한으로의 위장 해제
	//------------------------------------//
	if (bImpersonated)
	{
		RevertToSelf();
		bImpersonated = FALSE;
	}

	return nRet;
}



BOOL util::GetPhysicalPathToLogicalPath(CString strPhysicalPath, CString strLogicalPath)
{
	TCHAR szLogicalDriveList[MAX_PATH] = { 0 };

	if (GetLogicalDriveStrings(MAX_PATH - 1, szLogicalDriveList) != 0)
	{
		TCHAR szLogicalDrive[] = _T(" :");
		TCHAR szPhysicalDrive[MAX_PATH];
		TCHAR* cLetter = szLogicalDriveList;

		do
		{
			*szLogicalDrive = *cLetter;

			if (QueryDosDevice(szLogicalDrive, szPhysicalDrive, MAX_PATH) != 0)
			{
				size_t nDriveLen = _tcslen(szPhysicalDrive);

				if (_tcsnicmp(strPhysicalPath, szPhysicalDrive, nDriveLen) == 0 &&
					strPhysicalPath.GetAt(nDriveLen) == _T('\\'))
				{
					strPhysicalPath.Delete(0, nDriveLen);
					strPhysicalPath.Insert(0, szLogicalDrive);
					strLogicalPath = strPhysicalPath;

					return TRUE;
				}
			}

			while (*cLetter++);
		} while (*cLetter);
	}

	return FALSE;
}

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
