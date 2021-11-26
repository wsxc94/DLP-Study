#pragma once

class injectManager // dll injection
{
private:
	DWORD pid;
public:
	injectManager();
	~injectManager();

	bool Process_name_to_pid(const wstring& process_name); // process name -> pid
	bool Dll_injection(const wstring& dll_name);		       // 프로세스에 dll을 inject 
	BOOL DLLInject(DWORD dwPID, LPCTSTR lpDllName);
	DWORD Get_Pid() { return pid; }
};


