#pragma once
#pragma once

class UrlHook
{
private:
	vector<string> urlBlockList;	// ������ ������Ʈ name list
public:
	UrlHook();
	~UrlHook();

	void WidgetLoad();	// �� ���� �ε� �Լ�
	void FindWindowWidget();	// ���� ������Ʈ Ž��
};



