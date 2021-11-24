#pragma once

class injectManager // dll injection
{
private:
public:
	injectManager();
	~injectManager();

	bool process_name_to_pid(__out DWORD& pid, __in const std::wstring& process_name); // process name -> pid
	bool dll_injection(__in DWORD pid, __in const std::wstring& dll_name);		       // 프로세스에 dll을 inject 
};


