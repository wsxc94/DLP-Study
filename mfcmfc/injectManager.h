#pragma once

class injectManager // dll injection
{
private:
	DWORD pid;
public:
	injectManager();
	~injectManager();

	bool process_name_to_pid(__out DWORD& pid, __in const std::wstring& process_name); // process name -> pid
	bool dll_injection(__in DWORD pid, __in const std::wstring& dll_name);		       // 프로세스에 dll을 inject 
	BOOL DLLInject(DWORD dwPID, LPCTSTR lpDllName);
	DWORD& Get_Pid() { return pid; }
};


