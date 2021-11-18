#pragma once

namespace util {
	char* ConvertWCtoC(wchar_t* str);								// wchar_t -> char 변수
	wchar_t* ConvertCharToWC(char* str);							// char -> wchar_t
	bool compareString(string a, string b);							// string 대문자 변환 및 비교
	HWND GetHwndFromProcessHandle(HANDLE hProcess);					// HANDLE -> HWND
	HWND GetHwndFromProcessID(DWORD dwProcessID);					// DWORD -> HWND
	BOOL CALLBACK CallbackEnumWindowProc(HWND hWnd, LPARAM lParam); 
	DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess);		// HANDLE -> PROCESS ID
	HWND GetHwndFormProcessID(ULONG pid);							// PROCESS ID -> HANDLE
	ULONG ProcIDFromWnd(HWND hwnd);									// HWND -> PROC ID
}

