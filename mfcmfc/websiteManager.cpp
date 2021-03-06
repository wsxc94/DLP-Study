#include "pch.h"
#include "websiteManager.h"

websiteManager::websiteManager()
{
	// 웹사이트 차단 리스트
	wsBlockList.emplace_back(".naver.");
	wsBlockList.emplace_back(".bing.");
}

websiteManager::~websiteManager()
{
	Unhook();
}

void websiteManager::WidgetLoad()
{
	FindWindowWidget();
}

void websiteManager::FindWindowWidget()
{
	CoInitialize(NULL);

	HWND hwnd = NULL;	// 웹사이트의 정보를 받아올 핸들

	while (true)
	{
		//브라우저 위젯 핸들
		hwnd = FindWindowEx(0, hwnd, L"Chrome_WidgetWin_1", NULL);

		if (!hwnd) break; //핸들이 없으면 종료
		if (!IsWindowVisible(hwnd)) continue; // 윈도우가 존재하지않으면

		// UI 오토메이션
		CComQIPtr<IUIAutomation> uia;
		if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia) break;

		// UI 오토메이션 루트노드
		CComPtr<IUIAutomationElement> root;
		if (FAILED(uia->ElementFromHandle(hwnd, &root)) || !root) break;

		// UI 오토메이션 Condition
		CComPtr<IUIAutomationCondition> condition;
		uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(0xC354), &condition);

		// UI 오토메이션 자식노드
		CComPtr<IUIAutomationElement> edit;

		// 자식을 포함한 하위 목록 find
		if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &edit)) || !edit) continue;

		// edit의 value pattern을 받아와 현재의 url 값을 받아온다.
		CComPtr<IUIAutomationValuePattern> value;
		edit->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&value));
		BSTR bs;
		value->get_CurrentValue(&bs);
		string current_site = _com_util::ConvertBSTRToString(bs);
		cout << current_site << "\n"; //현재 사이트 디버그용

		// 웹 차단 리스트 순회
		for (string s : wsBlockList) {
			if (current_site.find(s) != string::npos) {
				value->SetValue(_com_util::ConvertStringToBSTR("www.google.com")); // url value 값 수정
				//SetForegroundWindow(hwnd);
				PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0); // enter 이벤트
				AfxMessageBox(_T("웹사이트 차단"));
			}
		}
		break;
	}
	CoUninitialize();
}

void websiteManager::Hook()
{
	if (LHook != 0) return;
	CoInitialize(NULL);
	LHook = SetWinEventHook(EVENT_OBJECT_FOCUS, EVENT_OBJECT_VALUECHANGE, 0, UrlEventHookProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
}

void websiteManager::Unhook()
{
	if (LHook == 0) return;
	UnhookWinEvent(LHook);
	CoUninitialize();
}

void websiteManager::StartThread()
{
	websiteManager* param = new websiteManager;
	param = this;

	thread = AfxBeginThread(ThreadUpdata, param);
}

UINT websiteManager::ThreadUpdata(LPVOID aParam)
{
	websiteManager* pThis = (websiteManager*)aParam;

	//while (true)
	//{
	//	Sleep(500);
	//	pThis->WidgetLoad();
	//}

	MSG msg;

	pThis->Hook();

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	pThis->Unhook();

	return 0;
}

void websiteManager::UrlEventHookProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	IAccessible* pAcc = NULL;
	VARIANT Child;

	HRESULT hr = AccessibleObjectFromEvent(hwnd, idObject, idChild, &pAcc, &Child);

	if ((hr == S_OK) && (pAcc != NULL)) {
		BSTR urlValue;
		pAcc->get_accValue(Child, &urlValue);

		if (urlValue == NULL) return;

		char className[500];
		GetClassName(hwnd, (LPWSTR)className, 500);

		if (event == EVENT_OBJECT_VALUECHANGE) {
			/*
			Safari => SafariTaskbarTabWindow
			Chrome => Chrome_WidgetWin_1
			IE => IEFrame
			Firefox => MozillaWindowClass
			Opera => OperaWindowClass
			*/
			if (strcmp(className, "Chrome_WidgetWin_1") != 0) {
				//printf("Active URL: %ls\n", urlValue);
				
				string current_site = _com_util::ConvertBSTRToString(urlValue);
				cout << current_site << "\n"; //현재 사이트 디버그용
			
			}
		}
		SysFreeString(urlValue);
		pAcc->Release();
	}
}
