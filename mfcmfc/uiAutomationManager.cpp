#include "pch.h"
#include "uiAutomationManager.h"
/*
 테스트 연습용 코드 사용안함
*/
uiAutomationManager::uiAutomationManager() : automation(NULL) , rootElement(NULL) , pClientUIA(NULL)
{
   
}

uiAutomationManager::~uiAutomationManager()
{
    delete automation;
    delete rootElement;
    delete pClientUIA;
}

void uiAutomationManager::setElementHandle(HWND targetWindow)
{
    if (targetWindow == NULL) {
        cout << "HWND TARGET NULL" << "\n";
        return;
    }
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    if (S_OK != hr)
    {
        printf("CoInitializeEx error: %d\n", hr);
        return;
    }

    hr = CoCreateInstance(CLSID_CUIAutomation, NULL, CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(&pClientUIA));

    if (S_OK != hr)
    {
        printf("CoCreateInstance error: %d\n", hr);
        return;
    }

    hr = pClientUIA->ElementFromHandle(targetWindow, &rootElement);
    if (S_OK != hr)
    {
        printf("ElementFromHandle error: %d\n", hr);
        return;
    }

    HWND foundHwnd = FindScrollbarContainerWindow(UIA_ScrollBarControlTypeId);

    if (NULL == foundHwnd) cout << "found Hwnd NULL" << "\n"; return;
}

HWND uiAutomationManager::FindScrollbarContainerWindow(const long controlType)
{
    HRESULT hr;
    BSTR name;
    IUIAutomationCondition* pCondition;
    VARIANT varProp;
    varProp.vt = VT_I4;
    varProp.uintVal = controlType;

    USES_CONVERSION; //W2A convert

    hr = pClientUIA->CreatePropertyCondition(UIA_ControlTypePropertyId, varProp, &pCondition);
    if (S_OK != hr) printf("CreatePropertyCondition error: %d\n", hr);

    cout << "<parent>" << "\n";
    rootElement->get_CurrentName(&name);
    string str7 = W2A(name);
    cout << str7 << "\n";

    //부모 노드 정보 출력
    rootElement->get_CurrentClassName(&name);
    string str8 = W2A(name);
    cout << str8 << "\n";

    int id2;
    rootElement->get_CurrentProcessId(&id2);
    cout << id2 << "\n";

    IUIAutomationLegacyIAccessiblePattern* legacyPattern;
    hr = rootElement->GetCurrentPatternAs(UIA_LegacyIAccessiblePatternId, IID_PPV_ARGS(&legacyPattern));
    hr = legacyPattern->get_CurrentName(&name);
    string tx1 = W2A(name);
    cout << "current_name : " << tx1 << "\n";

    CComVariant url;
    hr = rootElement->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &url);
    CString s;
    s = url.bstrVal;
    cout << "url : " << s << "\n";


    cout << "\n";
    return NULL;
}