#pragma once
/*
* 테스트 연습용 코드 사용안함
*/
class uiAutomationManager : public singletonBase<uiAutomationManager>
{
private:
	IUIAutomation* automation;
	IUIAutomationElement* rootElement;
	IUIAutomation* pClientUIA;
public:
	uiAutomationManager();
	~uiAutomationManager();

	void setElementHandle(HWND targetWindow);
	HWND FindScrollbarContainerWindow(const long controlType);
};

