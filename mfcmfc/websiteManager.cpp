#include "pch.h"
#include "websiteManager.h"

websiteManager::websiteManager()
{
	// ������Ʈ ���� ����Ʈ
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

	HWND hwnd = NULL;	// ������Ʈ�� ������ �޾ƿ� �ڵ�

	while (true)
	{
		//������ ���� �ڵ�
		hwnd = FindWindowEx(0, hwnd, L"Chrome_WidgetWin_1", NULL);

		if (!hwnd) break; //�ڵ��� ������ ����
		if (!IsWindowVisible(hwnd)) continue; // �����찡 ��������������

		// UI ������̼�
		CComQIPtr<IUIAutomation> uia;
		if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia) break;

		// UI ������̼� ��Ʈ���
		CComPtr<IUIAutomationElement> root;
		if (FAILED(uia->ElementFromHandle(hwnd, &root)) || !root) break;

		// UI ������̼� Condition
		CComPtr<IUIAutomationCondition> condition;
		uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(0xC354), &condition);

		// UI ������̼� �ڽĳ��
		CComPtr<IUIAutomationElement> edit;

		// �ڽ��� ������ ���� ��� find
		if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &edit)) || !edit) continue;

		// edit�� value pattern�� �޾ƿ� ������ url ���� �޾ƿ´�.
		CComPtr<IUIAutomationValuePattern> value;
		edit->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&value));
		BSTR bs;
		value->get_CurrentValue(&bs);
		string current_site = _com_util::ConvertBSTRToString(bs);
		cout << current_site << "\n"; //���� ����Ʈ ����׿�

		// �� ���� ����Ʈ ��ȸ
		for (string s : wsBlockList) {
			if (current_site.find(s) != string::npos) {
				value->SetValue(_com_util::ConvertStringToBSTR("www.google.com")); // url value �� ����
				//SetForegroundWindow(hwnd);
				PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0); // enter �̺�Ʈ
				AfxMessageBox(_T("������Ʈ ����"));
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
				cout << current_site << "\n"; //���� ����Ʈ ����׿�
			
			}
		}
		SysFreeString(urlValue);
		pAcc->Release();
	}
}
