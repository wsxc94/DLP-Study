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
	//HANDLE        hModuleSnap = NULL;	// ���μ��� �ڵ�
	//MODULEENTRY32 me32 = { 0 };			// ���μ��� ����
	//hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);	// ���������� ���μ��� ����

	//if (hModuleSnap == (HANDLE)-1) return (FALSE);

	///*CString path;
	//util::GetProcessPathByPID(dwPID, path);
	//string s = string(CT2CA(path));
	//cout << s << "\n";*/

	//me32.dwSize = sizeof(MODULEENTRY32);

	////�ش� ���μ����� ��⸮��Ʈ�� ������ ������ ���μ����̸��� �����ϸ�
	////true�� �����Ѵ�.

	//if (Module32First(hModuleSnap, &me32))	// ���μ��� ����� �����ϸ�
	//{
	//	do
	//	{
	//		CString tmp = me32.szModule;
	//		CString str(sProcessName.c_str());
	//		LPCTSTR spName = str;

	//		if (tmp.CompareNoCase(spName) == 0) {
	//			CloseHandle(hModuleSnap);
	//			return true;
	//		}

	//		//if (util::compareString(string(CT2CA(tmp)), sProcessName)) // ���μ��� Ű���尡 ���ٸ� true ��ȯ
	//		//{
	//		//	CloseHandle(hModuleSnap);
	//		//	return true;
	//		//}

	//	} while (Module32Next(hModuleSnap, &me32));	// ���� ���μ��� ����
	//}
	//CloseHandle(hModuleSnap);

	//return false;

	HANDLE p_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwPID);

	if (p_handle == NULL) return false;

	wchar_t buffer[MAX_PATH] = {};
	DWORD buffer_size = _ARRAYSIZE(buffer) - 1;

	if (QueryFullProcessImageNameW(p_handle, 0, buffer, &buffer_size)) {
		string path = util::ConvertWCtoC(buffer);
		string p_name = path.substr(path.rfind('\\') + 1, path.size() - 1);

		if (util::compareString(p_name, sProcessName)) {
			CloseHandle(p_handle);
			return true;
		}
		
	}

	if (p_handle) CloseHandle(p_handle);

	return false;
}

void processManager::ProcessLoad()
{
	if (FindProcess(_T("notepad.exe"))) {
		cout << "����" << "\n";
	}
	else {
		cout << "����" << "\n";
	}

	//HANDLE         hProcessSnap = NULL;		// ���μ����� �޾ƿ� �ڵ�
	//PROCESSENTRY32 pe32 = { 0 };			// ���μ��� ���� ����

	//hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//if (hProcessSnap == (HANDLE)-1) return;

	//pe32.dwSize = sizeof(PROCESSENTRY32);

	////���μ����� �޸𸮻� ������ ù��° ���μ����� ��´�
	//if (Process32First(hProcessSnap, &pe32))
	//{
	//	MODULEENTRY32 me32 = { 0 };
	//	do
	//	{
	//		for (string processName : sw_block_list) {  // ���� ���μ��� ����Ʈ ��ȸ
	//			if (GetProcessModule(pe32.th32ProcessID, processName)) // ���μ��� id�� �ش� ����� �ҷ��´�.
	//			{
	//				// ���μ��� ID�� ���� �ش� ���μ��� �ڵ��� ���´�
	//				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
	//				if (hProcess)
	//				{
	//					// ���μ����� �������̶��
	//					if (TerminateProcess(hProcess, 0))
	//					{
	//						unsigned long nCode; //���μ��� ���� �ڵ�
	//						GetExitCodeProcess(hProcess, &nCode); // ���μ��� ���� �Լ�
	//						cout << processName << "�� �����߽��ϴ�." << "\n";
	//						AfxMessageBox(_T("����Ʈ���� ����"));							
	//					}
	//					CloseHandle(hProcess);
	//				}
	//			}
	//		}
	//	} while (Process32Next(hProcessSnap, &pe32)); //���� ���μ����� ������ ���Ͽ� ������ ������ ����.
	//}
	//CloseHandle(hProcessSnap);
}

BOOL processManager::FindProcess(LPCTSTR lpszProcessName, set<DWORD>* stPID)
{
	// 4096
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	set<DWORD> stPIDTemp;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		cout << "��� ���޾ƿ�" << "\n";
		return FALSE;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (UINT nCnt = 0; nCnt < cProcesses; nCnt++)
	{
		DWORD dwPID = aProcesses[nCnt];

		CString strProcessPath;
		util::GetProcessPathByPID(dwPID, strProcessPath);

		if (strProcessPath.IsEmpty())
			continue;

		CString strProcessName = strProcessPath.Mid(strProcessPath.ReverseFind(_T('\\')) + 1);

		if (strProcessName.CompareNoCase(lpszProcessName) == 0)
		{
			if (stPID == NULL)
				return TRUE;

			stPIDTemp.insert(dwPID);
		}
	}

	if (stPID != NULL && stPIDTemp.size() > 0)
	{
		*stPID = stPIDTemp;

		return TRUE;
	}

	return FALSE;
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
	}

	return 0;
}
