#include "pch.h"
#include "injectManager.h"

injectManager::injectManager()
{
}

injectManager::~injectManager()
{
}

bool injectManager::process_name_to_pid(__out DWORD& pid,__in const wstring& process_name) {
	
	// ã�� ���μ��� �̸��� �̿��� ���������� pid �� ã���� true�� �����Ѵ�.

	bool result = false; // ���μ����� ������ ��� true
	HANDLE snapshot = nullptr;
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);

	// ������ ���μ����� �� , ��⿡ ���� ������ ���� �������� ������
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	// ���μ��� ����� Ž���ϸ� ����ڰ� ������ ���μ��� �̸��� �ִ��� Ȯ��
	if (snapshot != INVALID_HANDLE_VALUE) {
		Process32First(snapshot, &entry); // ���������� �߻��� ù��° ���μ��� ���� �˻�
		do {
			if (!_tcsicmp(process_name.c_str(), entry.szExeFile)) { 
				// ������ ���μ��� �̸��� entry�� �̸��� ��ġ�ϸ� ���μ��� id�� true�� ��ȯ�Ѵ�.
				pid = entry.th32ProcessID;
				result = true;
				break;
			}
		} while (Process32Next(snapshot, &entry)); // ���� ���μ����� �Ѿ��
		CloseHandle(snapshot);
	}
	return result;
}

bool injectManager::dll_injection(__in DWORD& pid, __in const wstring& dll_name)
{
    bool result = false;
    HANDLE process_handle = nullptr;
    HANDLE thread_handle = nullptr;
    LPVOID remote_buffer = nullptr;
    HMODULE module = {};
    LPTHREAD_START_ROUTINE thread_start_routine = nullptr;
    do {
        process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        //�ش� PID�� ���� ���μ����� ���� �ڵ��� �����´�
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
        //�ش� ���μ����� ���� �޸� ������ �޸𸮸� �Ҵ���. 
        if (!remote_buffer) {
            break;
        }
        if (!WriteProcessMemory(
            process_handle,
            remote_buffer,
            dll_name.c_str(),
            dll_name.size() * sizeof(wchar_t),
            nullptr)
            ) {
            break;
        }
        // remote_buffer�� ��� �Ҵ�� �޸� ������ �ε�� DLL�� ��� ��Ʈ���� wrtie �Ѵ�. 
        module = GetModuleHandle(L"kernel32.dll");
        //kernel32.dll ����� �ڵ��� ������
        thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");
        //��⿡ ��� Kernel32.dll ���� LoadLibrary�Լ� �����͸� ������ �����Ѵ�. 
        thread_handle = CreateRemoteThread(
            process_handle,
            nullptr,
            0,
            thread_start_routine,
            remote_buffer,
            0,
            nullptr
        );
        //LoadLibrary �Լ��� ������ ���� CreateRemoteThread�Լ��� �̿��Ͽ� Thread�� �����ϴ°� ó�� �ش� �Լ��� ������.
        //�̶� remote_buffer �޸� �������� �����ߴ� dll�� ��� ���ڿ��� ����ִ�.
        //���� LoadLibrary �Լ��� ���� ���� ������ dll�� load�� �� �ְ� �Ǵ°�. 
        WaitForSingleObject(thread_handle, INFINITE);
        result = true;
    } while (false);
    CloseHandle(process_handle);
    CloseHandle(thread_handle);
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
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) return FALSE;

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
