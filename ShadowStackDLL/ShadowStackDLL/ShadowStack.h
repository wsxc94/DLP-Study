#pragma once

class ShadowStack //: public singletonBase <CShadowStackManager>
{
public:
	ShadowStack() {}
	virtual ~ShadowStack() {}

	virtual void DestoryObject() = 0;
	virtual BOOL CheckShadowStackByProcessPIDName(DWORD dwPID, CString strProcessName, BOOL& bActive) = 0;
	virtual BOOL SetShadowStackOptionByProcessPIDName(DWORD dwPID, CString strProcessName, BOOL bActive) = 0;
};