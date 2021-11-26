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

bool processManager::GetProcessModule(DWORD dwPID, string sProcessName)
{
	HANDLE        hModuleSnap = NULL;	// 프로세스 핸들
	MODULEENTRY32 me32 = { 0 };			// 프로세스 정보
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);	// 스냅샷으로 프로세스 추출

	if (hModuleSnap == (HANDLE)-1) return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);

	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	//true를 리턴한다.

	if (Module32First(hModuleSnap, &me32))	// 프로세스 모듈이 존재하면
	{
		do
		{
			char* tmp = ConvertWCtoC(me32.szModule); // 프로세스 키워드 비교를 위한 모듈값 convert

			if (compareString(tmp, sProcessName)) // 프로세스 키워드가 같다면 true 반환
			{
				CloseHandle(hModuleSnap);
				delete[] tmp;
				return true;
			}
			if (tmp != nullptr) delete[] tmp;
		} while (Module32Next(hModuleSnap, &me32));	// 다음 프로세스 루프
	}
	CloseHandle(hModuleSnap);

	return false;
}

void processManager::ProcessLoad()
{
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
				if (GetProcessModule(pe32.th32ProcessID, processName))
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
