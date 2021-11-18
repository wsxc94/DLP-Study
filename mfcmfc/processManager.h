#pragma once
#include "Thread.h"
class processManager : public Thread
{
private:
	vector<string> sw_block_list;		// ������ ���μ��� name list
public:
	processManager();
	virtual ~processManager();

	bool GetProcessModule(DWORD dwPID, string sProcessName); //���μ��� ����� �ҷ����� �Լ�
	void ProcessLoad(); // ���μ����� �ε��ϰ� ����Ʈ�� ���� ���� ��Ű�� �Լ�

	void StartThread();	// ������ ����
	static UINT ThreadUpdata(LPVOID aParam);	// ������ ����
};