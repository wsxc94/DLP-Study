#include "pch.h"
#include "keeperManager.h"
#include <Windows.h>

keeperManager::keeperManager()
{
	processMng = make_shared<processManager>();
	websiteMng = make_shared<websiteManager>();
	directoryMng = make_shared<directoryManager>();
	cryptoMng = make_shared<cryptoManager>();
	hookMng = make_shared<hookManager>();
	injectMng = make_shared<injectManager>();

	//getShadowStack = (pGetShadowStackMng)GetProcAddress(LoadLibrary(_T("C:\\Users\\wsxc94\\Desktop\\DLP-Study\\mfcmfc\\x64\\Release\\OFKShadowStackDLL.dll")), "GetShadowStackManager");
	//..\\x64\\Release\\

	string path = "ShadowStackDLL.dll";
	
	hMod = NULL;

	hMod = LoadLibraryA(path.c_str());

	if (hMod)
	{
		pGetShadowStackMng getShadowStack = reinterpret_cast<pGetShadowStackMng>(GetProcAddress(hMod, "GetShadowStackManager"));
		
		if (getShadowStack)
		{		
			shadowStackMng = NULL;
			shadowStackMng = getShadowStack();

			cout << path << " ·Îµå" << "\n";
		}
	}

	//HWND hwnd;
	//wchar_t className[100];
	//GetClassName(hwnd, className, 100) == 0 || wcscmp(className, L"Chrome_WidgetWin_1");

	//CComPtr<IAccessible> pAccMain;
	//HRESULT hr = ::AccessibleObjectFromWindow(hwndchrome)

}

keeperManager::~keeperManager()
{
	FreeLibrary(hMod);
	shadowStackMng->DestoryObject();
	shadowStackMng = nullptr;
}

