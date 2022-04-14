#pragma once
#include "singletonBase.h"
#include "processManager.h"
#include "websiteManager.h"
#include "directoryManager.h"
#include "cryptoManager.h"
#include "hookManager.h"
#include "injectManager.h"
#include "ShadowStackApp.h"

typedef ShadowStack* (__cdecl* pGetShadowStackMng)();

class keeperManager : public singletonBase <keeperManager>
{
private:
	shared_ptr<processManager> processMng;		// 프로세스 차단 매니저
	shared_ptr<websiteManager> websiteMng;		// UI automation , 사이트 차단 매니저
	shared_ptr<directoryManager> directoryMng;	// 디렉토리 검사 매니저
	shared_ptr<cryptoManager> cryptoMng;		// 암호화 복호화 매니저
	shared_ptr<hookManager> hookMng;			// window 후크 매니저
	shared_ptr<injectManager> injectMng;		// window 후크 매니저
	
	HMODULE hMod;
	//pGetShadowStackMng getShadowStack;
	ShadowStack* shadowStackMng;			// shadowstack 매니저
public:
	keeperManager();
	~keeperManager(); 

	processManager* Get_ProcessMng() { return processMng.get(); }
	websiteManager* Get_WebsiteMng() { return websiteMng.get(); }
	directoryManager* Get_DirectoryMng() { return directoryMng.get(); }
	cryptoManager* Get_CryptoMng() { return cryptoMng.get(); }
	hookManager* Get_HookMng() { return hookMng.get(); }
	injectManager* Get_InjectMng() { return injectMng.get(); }
	ShadowStack* Get_ShadowStackMng() { return shadowStackMng; }

	//static CShadowStackManager*& Get_ShadowStackMng() { return shadowStackMng; }
};

