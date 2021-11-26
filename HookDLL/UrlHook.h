#pragma once
#pragma once

class UrlHook
{
private:
	std::vector<std::string> urlBlockList;	// 차단할 웹사이트 name list
public:
	UrlHook();
	~UrlHook();

	void WidgetLoad();	// 웹 위젯 로드 함수
	void FindWindowWidget();	// 현재 웹사이트 탐색
};



