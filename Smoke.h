#pragma once
class Player;

class Smoke : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CQuaternion m_rotation;
	Player* m_player = nullptr;

	static int s_smokeNum;
	float m_alpha = 0.0f;	//煙のアルファ
	float m_timer = 0.0f;	//タイマー
};

