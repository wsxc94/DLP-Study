#pragma once

#ifdef _DLL_UTIL_
#define DLL_UTIL _declspec(dllexport)
#else
#define DLL_UTIL __declspec(dllimport)
#endif // _DLL_UTIL_

namespace util {
	// 사용자 유틸 함수들
	DLL_UTIL BOOL GetProcessPathByPID(DWORD dwPID, CString& strProcessPath);// pid -> path
	DLL_UTIL BOOL IsNeedImpersonateAsUser(CString strFilePath);// 유저 권한으로 토큰 위장이 필요한 파일인지 확인
	DLL_UTIL BOOL ImpersonateAsUser();// 시스템 계정의 권한을 유저 계정으로 임시 변경되도록 현재 스레드의 토큰 위장
	DLL_UTIL int GetShortToLongFileName(CString sShortPath, CString& sLongPath); // 긴 이름 파일명 가져오기
	DLL_UTIL BOOL GetPhysicalPathToLogicalPath(CString strPhysicalPath, CString strLogicalPath);// 물리주소 -> 논리주소
	DLL_UTIL char* ConvertWCtoC(wchar_t* str);								// wchar_t -> char 변수
	DLL_UTIL std::string ConvertTCtoSTR(TCHAR* tc);								// tchar -> string 변수
	DLL_UTIL wchar_t* ConvertCharToWC(char* str);							// char -> wchar_t
	DLL_UTIL bool compareString(std::string a, std::string b);							// string 대문자 변환 및 비교
	DLL_UTIL HWND GetHwndFromProcessHandle(HANDLE hProcess);					// HANDLE -> HWND
	DLL_UTIL HWND GetHwndFromProcessID(DWORD dwProcessID);					// DWORD -> HWND
	DLL_UTIL BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam); // 파라미터와 window 비교
	DLL_UTIL DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess);		// HANDLE -> PROCESS ID
	DLL_UTIL HWND GetHwndFormProcessID(ULONG pid);							// PROCESS ID -> HANDLE
	DLL_UTIL ULONG ProcIDFromWnd(HWND hwnd);									// HWND -> PROC ID
}
