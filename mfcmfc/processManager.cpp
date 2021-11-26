#include "pch.h"
#include "processManager.h"

processManager::processManager()
{
	// ����Ʈ���� ���� ����Ʈ
	sw_block_list.emplace_back("notepad.exe");
	//soft_block_list.emplace_back("msedge.exe");
}

processManager::~processManager()
{
}

bool processManager::GetProcessModule(DWORD dwPID, string sProcessName)
{
	HANDLE        hModuleSnap = NULL;	// ���μ��� �ڵ�
	MODULEENTRY32 me32 = { 0 };			// ���μ��� ����
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);	// ���������� ���μ��� ����

	if (hModuleSnap == (HANDLE)-1) return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);

	//�ش� ���μ����� ��⸮��Ʈ�� ������ ������ ���μ����̸��� �����ϸ�
	//true�� �����Ѵ�.

	if (Module32First(hModuleSnap, &me32))	// ���μ��� ����� �����ϸ�
	{
		do
		{
			char* tmp = ConvertWCtoC(me32.szModule); // ���μ��� Ű���� �񱳸� ���� ��Ⱚ convert

			if (compareString(tmp, sProcessName)) // ���μ��� Ű���尡 ���ٸ� true ��ȯ
			{
				CloseHandle(hModuleSnap);
				delete[] tmp;
				return true;
			}
			if (tmp != nullptr) delete[] tmp;
		} while (Module32Next(hModuleSnap, &me32));	// ���� ���μ��� ����
	}
	CloseHandle(hModuleSnap);

	return false;
}

void processManager::ProcessLoad()
{
	HANDLE         hProcessSnap = NULL;		// ���μ����� �޾ƿ� �ڵ�
	PROCESSENTRY32 pe32 = { 0 };			// ���μ��� ���� ����

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1) return;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	//���μ����� �޸𸮻� ������ ù��° ���μ����� ��´�
	if (Process32First(hProcessSnap, &pe32))
	{
		MODULEENTRY32 me32 = { 0 };
		do
		{
			for (string processName : sw_block_list) {  // ���� ���μ��� ����Ʈ ��ȸ
				if (GetProcessModule(pe32.th32ProcessID, processName))
				{
					// ���μ��� ID�� ���� �ش� ���μ��� �ڵ��� ���´�
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
					if (hProcess)
					{
						// ���μ����� �������̶��
						if (TerminateProcess(hProcess, 0))
						{
							unsigned long nCode; //���μ��� ���� �ڵ�
							GetExitCodeProcess(hProcess, &nCode); // ���μ��� ���� �Լ�
							cout << processName << "�� �����߽��ϴ�." << "\n";
							AfxMessageBox(_T("����Ʈ���� ����"));
							
						}
						CloseHandle(hProcess);
					}
				}
			}
		} while (Process32Next(hProcessSnap, &pe32)); //���� ���μ����� ������ ���Ͽ� ������ ������ ����.
	}
	CloseHandle(hProcessSnap);
}

void processManager::StartThread()
{
	processManager* param = new processManager;
	param = this;

	thread = AfxBeginThread(ThreadUpdata, param);
}

UINT processManager::ThreadUpdata(LPVOID aParam)
{
	processManager* pThis = (processManager*)aParam;

	while (true)
	{
		Sleep(500);
		pThis->ProcessLoad();
		TRACE("process thread call");
	}

	return 0;
}
