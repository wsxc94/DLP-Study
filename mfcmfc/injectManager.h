#pragma once

class injectManager // dll injection
{
private:
	DWORD pid;
public:
	injectManager();
	~injectManager();

	bool process_name_to_pid(__out DWORD& pid, __in const wstring& process_name); // process name -> pid
	bool dll_injection(__in DWORD& pid, __in const wstring& dll_name);		       // ���μ����� dll�� inject 
	BOOL DLLInject(DWORD dwPID, LPCTSTR lpDllName);
	DWORD& Get_Pid() { return pid; }
};


