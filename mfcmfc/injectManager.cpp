#include "pch.h"
#include "injectManager.h"

injectManager::injectManager()
{
}

injectManager::~injectManager()
{
}

bool injectManager::process_name_to_pid(__out DWORD& pid,__in const std::wstring& process_name) {
	
	// ã�� ���μ��� �̸��� �̿��� ���������� pid �� ã���� true�� �����Ѵ�.

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

	// ���μ��� id�� �̿��� �ڵ��� ���� �� ���� �޸𸮿� ������ ���ش�.
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
		// Ŀ�� 32 dll �� ����ڵ��� �ҷ��� ���μ��� �ڵ��� �־� �����带 �����Ѵ�.
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

	// dwPID �� �̿��Ͽ� ��� ���μ���(exe)�� HANDLE�� ����
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
		return FALSE;

	// ��� ���μ���(notepad.exe) �޸𸮿� szDllName ũ�⸸ŭ �޸𸮸� �Ҵ�
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// �Ҵ� ���� �޸𸮿� dll ���("c:\\???.dll")�� ��
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)lpDllName, dwBufSize, NULL);

	// LoadLibraryA() API �ּҸ� ����
	hMod = GetModuleHandle(_T("kernel32.dll"));
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA");

	// exe ���μ����� �����带 ����
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	cout << "�Ϸ�" << "\n";
	return TRUE;
}
