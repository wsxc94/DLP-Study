#include "pch.h"
#include "injectManager.h"

injectManager::injectManager()
{
}

injectManager::~injectManager()
{
}

bool injectManager::process_name_to_pid(__out DWORD& pid,__in const std::wstring& process_name) {
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
	cout << "dll injection on" << "\n";
	bool result = false;
	HANDLE process_handle = nullptr;
	HANDLE thread_handle = nullptr;
	LPVOID remote_buffer = nullptr;
	HMODULE module = {};
	LPTHREAD_START_ROUTINE thread_start_routine = nullptr;

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

	cout << result << "\n";
	return result;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		MessageBox(nullptr, L"injection success", L"dll injection", MB_OK);
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
