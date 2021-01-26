#pragma once
class GameScoreManager : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();


	prefab::CFontRender* m_scoreFontRender = nullptr;
	int m_gameScore = 0;
	std::wstring Score;
	const wchar_t* string;
};

