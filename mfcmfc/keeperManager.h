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
	shared_ptr<processManager> processMng;		// ���μ��� ���� �Ŵ���
	shared_ptr<websiteManager> websiteMng;		// UI automation , ����Ʈ ���� �Ŵ���
	shared_ptr<directoryManager> directoryMng;	// ���丮 �˻� �Ŵ���
	shared_ptr<cryptoManager> cryptoMng;		// ��ȣȭ ��ȣȭ �Ŵ���
	shared_ptr<hookManager> hookMng;			// window ��ũ �Ŵ���
	shared_ptr<injectManager> injectMng;		// window ��ũ �Ŵ���
	
	HMODULE hMod;
	//pGetShadowStackMng getShadowStack;
	ShadowStack* shadowStackMng;			// shadowstack �Ŵ���
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

