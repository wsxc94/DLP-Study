#include "pch.h"
#include "injectManager.h"

injectManager::injectManager() : pid(NULL)
{
}

injectManager::~injectManager()
{
}

bool injectManager::Process_name_to_pid(const wstring& process_name) {
	
	// 찾을 프로세스 이름을 이용해 스냅샷으로 pid 를 찾으면 true를 리턴한다.

	bool result = false; // 프로세스가 존재할 경우 true
	HANDLE snapshot = nullptr;
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);

	// 지정된 프로세스의 힙 , 모듈에 대한 스레드 정보 스냅샷을 가져옴
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	// 프로세스 목록을 탐색하며 사용자가 지정한 프로세스 이름이 있는지 확인
	if (snapshot != INVALID_HANDLE_VALUE) {
		Process32First(snapshot, &entry); // 스냅샷에서 발생한 첫번째 프로세스 정보 검색
		do {
			if (!_tcsicmp(process_name.c_str(), entry.szExeFile)) { 
				// 지정한 프로세스 이름과 entry의 이름이 일치하면 프로세스 id와 true를 반환한다.
				pid = entry.th32ProcessID;
				result = true;
				break;
			}
		} while (Process32Next(snapshot, &entry)); // 다음 프로세스로 넘어간다
		CloseHandle(snapshot);
	}
	return result;
}

bool injectManager::Dll_injection(const wstring& dll_name)
{
    bool result = false;
    HANDLE process_handle = nullptr;    // 프로세스 핸들
    HANDLE thread_handle = nullptr;     // 스레드 핸들
    LPVOID remote_buffer = nullptr;     // 가상 버퍼 메모리
    HMODULE module = {};
    LPTHREAD_START_ROUTINE thread_start_routine = nullptr;

    do {
        //해당 PID를 가진 프로세스를 열어 핸들을 가져온다
        process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

        if (process_handle == nullptr) break;
        //해당 프로세스의 가상 메모리 공간에 메모리를 할당함.
		// 프로세스 주소 공간에 대해 사용할 영역을 미리 할당하여 준비작업을 해놓고 
		//실제 사용하기위해 물리적 저장소 할당 및 할당된 영역과 주소에 매핑작업을 해줌
		remote_buffer = VirtualAllocEx(
			process_handle,
			nullptr,
			dll_name.size(),
			MEM_COMMIT, 
			PAGE_READWRITE
		);

        // remote_buffer에 담긴 할당된 메모리 공간에 로드될 DLL의 경로 스트링을 wrtie 한다.
        if (!remote_buffer) break;

        if (!WriteProcessMemory(process_handle, remote_buffer, dll_name.c_str(), dll_name.size() * sizeof(wchar_t), nullptr)) break;

        //kernel32.dll 모듈의 핸들을 가져옴 
        module = GetModuleHandle(L"kernel32.dll");

        //모듈에 담긴 Kernel32.dll 내의 LoadLibrary함수 포인터를 가져와 저장한다.
        thread_start_routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");

        //LoadLibrary 함수와 원형이 같은 CreateRemoteThread함수를 이용하여 Thread를 실행하는것 처럼 해당 함수를 실행함.
        //이때 remote_buffer 메모리 공간에는 지정했던 dll의 경로 문자열이 담겨있다.
        //따라서 LoadLibrary 함수를 통해 내가 지정한 dll을 load할 수 있게 되는것.
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
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) return FALSE;

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
