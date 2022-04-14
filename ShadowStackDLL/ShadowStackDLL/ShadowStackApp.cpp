#include "pch.h"
#include "ShadowStackApp.h"


ShadowStackManager::ShadowStackManager()
{
}

ShadowStackManager::~ShadowStackManager()
{
}

void ShadowStackManager::DestoryObject()
{
    delete this;
}

BOOL ShadowStackManager::CheckShadowStackByProcessPIDName(DWORD dwPID, CString strProcessName, BOOL& bActive)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    BOOL Success = FALSE;

    if (hProcess)
    {
        PROCESS_MITIGATION_USER_SHADOW_STACK_POLICY userStackPolicy;

        // 현재 프로세스 핸들을 통해 하드웨어 적용 스택 보호 정책을 가져온다
        GetProcessMitigationPolicy(hProcess, ProcessUserShadowStackPolicy, &userStackPolicy, sizeof(userStackPolicy));

        //--------------------------------------------------------------------------------------------------//
        //	                하드웨어 적용 스택 보호를 사용중인 exe 라면 TRUE 반환                           //
        //--------------------------------------------------------------------------------------------------//
        if (userStackPolicy.EnableUserShadowStack == TRUE)
        {
            bActive = TRUE;
        }
        else
        {
            bActive = FALSE;
        }

        Success = TRUE;
    }

    EmptyWorkingSet(hProcess);
    CloseHandle(hProcess);

    return Success;
}

BOOL ShadowStackManager::SetShadowStackOptionByProcessPIDName(DWORD dwPID, CString strProcessName, BOOL bActive)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

    if (hProcess)
    {
        CString strCommand;

        //--------------------------------------------------------------------------------------------------//
        //	                    bActive =  TRUE 하드웨어 적용 스택 보호 활성화                              //                                      
        //                      bActive = FALSE 하드웨어 적용 스택 보호 비활성화                            //      
        //--------------------------------------------------------------------------------------------------//
        if (bActive == TRUE)
        {
            strCommand.Format(_T(" /C powershell -command \"& {set-processmitigation -name %s -enable usershadowstack}\" "), strProcessName);
        }
        else
        {
            strCommand.Format(_T(" /C powershell -command \"& {set-processmitigation -name %s -disable usershadowstack}\" "), strProcessName);
        }

        HINSTANCE hIns = ShellExecute(NULL, _T("open"), _T("cmd"), strCommand, NULL, SW_HIDE);

        EmptyWorkingSet(hProcess);
        CloseHandle(hProcess);

        return TRUE;
    }

    return FALSE;
}
