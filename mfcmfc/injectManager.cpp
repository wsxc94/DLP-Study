#include "pch.h"
#include "injectManager.h"

injectManager::injectManager()
{
}

injectManager::~injectManager()
{
}

bool injectManager::process_name_to_pid(__out DWORD& pid,__in const std::wstring& process_name) {
	
	// 찾을 프로세스 이름을 이용해 스냅샷으로 pid 를 찾으면 true를 리턴한다.

	bool result = false;
	HANDLE snapshot = nullptr;
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (snapshot != INVALID_HANDLE_VALUE) {
		Process32First(snapshot, &entry);
		do {
			if (!_tcsicmp(process_name.c_str(), entry.szExeFile)) {
				pid = entry.th32ProcessID;
				result = true;
				break;
			}
		} while (Process32Next(snapshot, &entry));
		CloseHandle(snapshot);
	}
	return result;
}

bool injectManager::dll_injection(__in DWORD pid, __in const std::wstring& dll_name)
{

	bool result = false;
	HANDLE process_handle = nullptr;
	HANDLE thread_handle = nullptr;
	LPVOID remote_buffer = nullptr;
	HMODULE module = {};
	LPTHREAD_START_ROUTINE thread_start_routine = nullptr;

	// 프로세스 id를 이용해 핸들을 얻어온 후 가상 메모리에 엑세스 해준다.
	do {
		process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (process_handle == nullptr) {
			break;
		}
		remote_buffer = VirtualAllocEx(
			process_handle,
			nullptr,
			dll_name.size(),
			MEM_COMMIT,
			PAGE_READWRITE
		);
		if (!remote_buffer) {
			break;
		}
		if (!WriteProcessMemory(
			process_handle,
			remote_buffer,
			dll_name.c_str(),
			dll_name.size() * sizeof(wchar_t),
			nullptr
		)) {
			break;
		}
		// 커널 32 dll 의 모듈핸들을 불러와 프로세스 핸들을 넣어 스레드를 실행한다.
		module = GetModuleHandle(L"kernel32.dll");

		thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");
		thread_handle = CreateRemoteThread(
			process_handle,
			nullptr,
			0,
			thread_start_routine,
			remote_buffer,
			0,
			nullptr
		);
		WaitForSingleObject(thread_handle, INFINITE);
		result = true;
	} while (false);

	CloseHandle(process_handle);
	CloseHandle(thread_handle);

	//cout << result << "\n";
	return result;
}

BOOL injectManager::DLLInject(DWORD dwPID, LPCTSTR lpDllName)
{
	HANDLE hProcess, hThread;
	HMODULE hMod;
	LPVOID pRemoteBuf;
	DWORD dwBufSize = lstrlen(lpDllName) + 1;
	LPTHREAD_START_ROUTINE pThreadProc;

	// dwPID 를 이용하여 대상 프로세스(exe)의 HANDLE을 구함
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
		return FALSE;

	// 대상 프로세스(notepad.exe) 메모리에 szDllName 크기만큼 메모리를 할당
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// 할당 받은 메모리에 dll 경로("c:\\???.dll")를 씀
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)lpDllName, dwBufSize, NULL);

	// LoadLibraryA() API 주소를 구함
	hMod = GetModuleHandle(_T("kernel32.dll"));
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");

	// exe 프로세스에 스레드를 실행
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	cout << "완료" << "\n";
	return TRUE;
}
