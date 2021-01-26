#pragma once
class TitleScene;

class TitlePlayer : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CVector3 m_moveDir, m_rightDir, m_upDir;
	float m_fSpeed = 0;
	float m_movex = 0, m_movez = 0;
	float m_x = 0, m_y = 0, m_z = 0;
	CQuaternion m_qRotX, m_qRotY, m_qRotZ;
	CQuaternion m_rotation;

	TitleScene* m_titleScene = nullptr;

	prefab::CEffect* m_frictionEffect = nullptr;
	prefab::CEffect* m_sbEffectRight = nullptr, * m_sbEffectLeft = nullptr;
	prefab::CEffect* m_jetEffectRight = nullptr, * m_jetEffectLeft = nullptr;
	bool m_isBoosting = false;

	prefab::CSpriteRender* m_HPSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_HPSpriteFrontRender = nullptr;
	prefab::CSpriteRender* m_FuelSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_FuelSpriteFrontRender = nullptr;
	prefab::CSpriteRender* m_speedSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_speedSpriteFrontRender = nullptr;
	std::wstring m_speedString;
	prefab::CFontRender* m_speedFontRender = nullptr;
	prefab::CSpriteRender* m_missionPanelRender = nullptr;
	int m_spriteMoveCount = 0;

	prefab::CSpriteRender* m_startSpriteRender = nullptr;
	prefab::CSpriteRender* m_titleSpriteRender = nullptr;
	float m_fontAlpha = 0.0f;

	//‰¹
	prefab::CSoundSource* m_engineSound = nullptr;

	CShaderResourceView m_specSRV;
};

