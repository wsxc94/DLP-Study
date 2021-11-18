#pragma once
#include "singletonBase.h"
#include "processManager.h"
#include "websiteManager.h"
#include "directoryManager.h"
#include "cryptoManager.h"
#include "hookManager.h"

class keeperManager : public singletonBase <keeperManager>
{
private:
	shared_ptr<processManager> processMng;		// ���μ��� ���� �Ŵ���
	shared_ptr<websiteManager> websiteMng;		// UI automation , ����Ʈ ���� �Ŵ���
	shared_ptr<directoryManager> directoryMng;	// ���丮 �˻� �Ŵ���
	shared_ptr<cryptoManager> cryptoMng;		// ��ȣȭ ��ȣȭ �Ŵ���
	shared_ptr<hookManager> hookMng;		// window ��ũ �Ŵ���
public:
	keeperManager();
	~keeperManager(); 

	processManager* Get_ProcessMng() { return processMng.get(); }
	websiteManager* Get_WebsiteMng() { return websiteMng.get(); }
	directoryManager* Get_DirectoryMng() { return directoryMng.get(); }
	cryptoManager* Get_CryptoMng() { return cryptoMng.get(); }
	hookManager* Get_HookMng() { return hookMng.get(); }
};

