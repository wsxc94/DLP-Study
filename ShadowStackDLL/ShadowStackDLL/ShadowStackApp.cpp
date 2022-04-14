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

        // ���� ���μ��� �ڵ��� ���� �ϵ���� ���� ���� ��ȣ ��å�� �����´�
        GetProcessMitigationPolicy(hProcess, ProcessUserShadowStackPolicy, &userStackPolicy, sizeof(userStackPolicy));

        //--------------------------------------------------------------------------------------------------//
        //	                �ϵ���� ���� ���� ��ȣ�� ������� exe ��� TRUE ��ȯ                           //
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
        //	                    bActive =  TRUE �ϵ���� ���� ���� ��ȣ Ȱ��ȭ                              //                                      
        //                      bActive = FALSE �ϵ���� ���� ���� ��ȣ ��Ȱ��ȭ                            //      
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
