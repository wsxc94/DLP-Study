#pragma once
#pragma once

class UrlHook
{
private:
	std::vector<std::string> urlBlockList;	// ������ ������Ʈ name list
public:
	UrlHook();
	~UrlHook();

	void WidgetLoad();	// �� ���� �ε� �Լ�
	void FindWindowWidget();	// ���� ������Ʈ Ž��
};



