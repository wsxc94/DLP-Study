#pragma once

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

