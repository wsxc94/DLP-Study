#include "pch.h"
#include "UrlHook.h"

UrlHook::UrlHook()
{
	// ������Ʈ ���� ����Ʈ
	urlBlockList.emplace_back(".naver.");
	urlBlockList.emplace_back(".bing.");
}

UrlHook::~UrlHook()
{
}

void UrlHook::WidgetLoad()
{
	FindWindowWidget();
}

void UrlHook::FindWindowWidget()
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

		USES_CONVERSION;
		std::string current_site = W2A(bs);

		TRACE(current_site.c_str()); //���� ����Ʈ ����׿�
		
		BSTR url = _T("www.google.com");

		// �� ���� ����Ʈ ��ȸ
		for (std::string s : urlBlockList) {
			if (current_site.find(s) != std::string::npos) {
				value->SetValue(url); // url value �� ����
				//SetForegroundWindow(hwnd);
				PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0); // enter �̺�Ʈ
				AfxMessageBox(_T("������Ʈ ����"));
			}
		}
		break;
	}
	CoUninitialize();
}
