#include "pch.h"
#include "processManager.h"

processManager::processManager()
{
	// 소프트웨어 차단 리스트
	sw_block_list.emplace_back("notepad.exe");
	//soft_block_list.emplace_back("msedge.exe");
}

processManager::~processManager()
{
}

bool processManager::GetProcessModule(DWORD pid, string ProcessName)
{
	//HANDLE        hModuleSnap = NULL;	// 프로세스 핸들
	//MODULEENTRY32 me32 = { 0 };			// 프로세스 정보
	//hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);	// 스냅샷으로 프로세스 추출

	//if (hModuleSnap == (HANDLE)-1) return (FALSE);

	///*CString path;
	//util::GetProcessPathByPID(dwPID, path);
	//string s = string(CT2CA(path));
	//cout << s << "\n";*/

	//me32.dwSize = sizeof(MODULEENTRY32);

	////해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	////true를 리턴한다.

	//if (Module32First(hModuleSnap, &me32))	// 프로세스 모듈이 존재하면
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

	//		//if (util::compareString(string(CT2CA(tmp)), sProcessName)) // 프로세스 키워드가 같다면 true 반환
	//		//{
	//		//	CloseHandle(hModuleSnap);
	//		//	return true;
	//		//}

	//	} while (Module32Next(hModuleSnap, &me32));	// 다음 프로세스 루프
	//}
	//CloseHandle(hModuleSnap);

	//return false;

	// 시스템 권한 등으로 인해 open 할수 없는 프로세스는 무조건 snapshot으로 이름을 구하는 수 밖에 없음

	HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);

	if (handle == NULL) return false;

	wchar_t buffer[MAX_PATH] = {};
	DWORD bufferSize = _ARRAYSIZE(buffer) - 1;

	if (QueryFullProcessImageNameW(handle, 0, buffer, &bufferSize)) {
		// 프로세스의 경로 , 이름을 나눈다
		string path = util::ConvertWCtoC(buffer);
		string name = path.substr(path.rfind('\\') + 1, path.size() - 1);

		if (util::compareString(name, ProcessName)) {
			EmptyWorkingSet(handle);
			CloseHandle(handle);
			return true;
		}
	}

	if (handle) {
		EmptyWorkingSet(handle);
		CloseHandle(handle);
	}
	return false;

}

void processManager::ProcessLoad()
{
	/*if (FindProcess(_T("notepad.exe"))) {
		cout << "있음" << "\n";
	}
	else {
		cout << "없음" << "\n";
	}*/

	HANDLE         hProcessSnap = NULL;		// 프로세스를 받아올 핸들
	PROCESSENTRY32 pe32 = { 0 };			// 프로세스 정보 변수

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1) return;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		MODULEENTRY32 me32 = { 0 };
		do
		{
			for (string processName : sw_block_list) {  // 차단 프로세스 리스트 순회
				if (GetProcessModule(pe32.th32ProcessID, processName)) // 프로세스 id로 해당 모듈을 불러온다.
				{
					// 프로세스 ID를 통해 해당 프로세스 핸들을 얻어온다
					HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
					if (hProcess)
					{
						// 프로세스가 실행중이라면
						if (TerminateProcess(hProcess, 0))
						{
							unsigned long nCode; //프로세스 상태 코드
							GetExitCodeProcess(hProcess, &nCode); // 프로세스 종료 함수
							cout << processName << "를 차단했습니다." << "\n";
							AfxMessageBox(_T("소프트웨어 차단"));							
						}
						CloseHandle(hProcess);
					}
				}
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);
}

BOOL processManager::FindProcess(LPCTSTR lpszProcessName, set<DWORD>* stPID)
{
	// 4096
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	set<DWORD> stPIDTemp;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		cout << "목록 못받아옴" << "\n";
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
