#pragma once
//#include "../../mfcmfc/singletonBase.h"
#include "ShadowStack.h"

#ifdef DLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#pragma once

class ShadowStackManager : public ShadowStack //: public singletonBase <CShadowStackManager>
{
public:
	ShadowStackManager();
	~ShadowStackManager();

	void DestoryObject();
	BOOL CheckShadowStackByProcessPIDName(DWORD dwPID, CString ProcessName, BOOL& bActive);
	BOOL SetShadowStackOptionByProcessPIDName(DWORD dwPID, CString ProcessName, BOOL bActive);
};

extern "C" __declspec(dllexport) ShadowStackManager * __cdecl GetShadowStackManager()
{
	return new ShadowStackManager();
}