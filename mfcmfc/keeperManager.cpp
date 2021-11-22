#include "pch.h"
#include "keeperManager.h"

keeperManager::keeperManager()
{
	processMng = make_shared<processManager>();
	websiteMng = make_shared<websiteManager>();
	directoryMng = make_shared<directoryManager>();
	cryptoMng = make_shared<cryptoManager>();
	hookMng = make_shared<hookManager>();
	injectMng = make_shared<injectManager>();
}

keeperManager::~keeperManager()
{
}

