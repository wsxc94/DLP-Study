#pragma once

#ifdef _DLL_UTIL_
#define DLL_UTIL _declspec(dllexport)
#else
#define DLL_UTIL __declspec(dllimport)
#endif // _DLL_UTIL_

// 사용자 유틸 함수들
DLL_UTIL inline char* ConvertWCtoC(wchar_t* str);								// wchar_t -> char 변수
DLL_UTIL wchar_t* ConvertCharToWC(char* str);							// char -> wchar_t
DLL_UTIL bool compareString(std::string a, std::string b);							// string 대문자 변환 및 비교
DLL_UTIL HWND GetHwndFromProcessHandle(HANDLE hProcess);					// HANDLE -> HWND
DLL_UTIL HWND GetHwndFromProcessID(DWORD dwProcessID);					// DWORD -> HWND
DLL_UTIL BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam); // 파라미터와 window 비교
DLL_UTIL DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess);		// HANDLE -> PROCESS ID
DLL_UTIL HWND GetHwndFormProcessID(ULONG pid);							// PROCESS ID -> HANDLE
DLL_UTIL ULONG ProcIDFromWnd(HWND hwnd);									// HWND -> PROC ID

