#pragma once
class Mission : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	int m_receiveMission = 0;
	int m_missionTimer = 300;
	bool m_isMissionConfirm = false;
	bool m_isSelectReverseMission = false;
	bool m_isMissionEnd = false;
	prefab::CFontRender* m_fontRender = nullptr;
	std::wstring teststring;
	const wchar_t* string;
};

