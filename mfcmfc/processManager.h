#pragma once
#include "Thread.h"
class processManager : public Thread
{
private:
	vector<CString> sw_block_list;		// ������ ���μ��� name list
public:
	processManager();
	virtual ~processManager();

	BOOL FindProcess(LPCTSTR lpszProcessName, set<DWORD>* stPID = NULL);
	bool GetProcessModule(DWORD pid, string ProcessName); //���μ��� ����� �ҷ����� �Լ�
	DWORD GetProcessByName(CString name);
	void ProcessLoad(); // ���μ����� �ε��ϰ� ����Ʈ�� ���� ���� ��Ű�� �Լ�

	virtual void StartThread();		// ������ ����
	static UINT ThreadUpdata(LPVOID aParam);	// ������ ����
};