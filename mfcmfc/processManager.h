#pragma once
class processManager
{
private:
	CWinThread* sw_block_Thread;	// ����Ʈ���� ���� ������ ����
	vector<string> sw_block_list;		// ������ ���μ��� name list
public:
	processManager();
	~processManager();

	bool GetProcessModule(DWORD dwPID, string sProcessName); //���μ��� ����� �ҷ����� �Լ�
	void ProcessLoad(); // ���μ����� �ε��ϰ� ����Ʈ�� ���� ���� ��Ű�� �Լ�
	void StartSwBlock(AFX_THREADPROC thread); // ���μ��� ���� ������ ���� �Լ�

	CWinThread* GetThread() { return sw_block_Thread; } // software block thread getter
};

