#pragma once
class IEnemy;
class SmokeGenerator;
class GameScene;

class Player : public IGameObject
{
public:
	void OnDestroy();
	bool Start();
	void Update();
	void PostUpdate();
	void Lockon();
	void ShootMissile();

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	CVector3 m_moveDir,m_rightDir,m_upDir;
	float m_fSpeed = 50;
	float m_movex = 0, m_movez = 0;
	float m_x=0, m_y=0, m_z=0;
	CQuaternion m_qRotX, m_qRotY, m_qRotZ;
	CQuaternion m_rotation;

	prefab::CSpriteRender* m_sightSpriteRender = nullptr;
	prefab::CSpriteRender* m_lockonSpriteRender = nullptr;
	prefab::CSpriteRender* m_controlSpriteRender = nullptr;
	prefab::CSpriteRender* m_HPSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_HPSpriteFrontRender = nullptr;
	prefab::CSpriteRender* m_FuelSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_FuelSpriteFrontRender = nullptr;
	prefab::CSpriteRender* m_speedSpriteBackRender = nullptr;
	prefab::CSpriteRender* m_speedSpriteFrontRender = nullptr;
	std::wstring m_speedString;
	prefab::CFontRender* m_speedFontRender = nullptr;
	prefab::CSpriteRender* m_missionPanelRender = nullptr;

	prefab::CSpriteRender* m_missileTrackingSpriteRender = nullptr;
	IEnemy* m_lockingEnemy = nullptr;
	IEnemy* m_oldLockingEnemy = nullptr;
	SmokeGenerator* m_smokeGenerator = nullptr;
	float m_maxLockonDistance = 40000.0f;
	int m_shotcooldown = 0;
	prefab::CEffect* m_frictionEffect = nullptr;
	prefab::CEffect* m_sbEffectRight = nullptr,*m_sbEffectLeft = nullptr;
	prefab::CEffect* m_jetEffectRight = nullptr, * m_jetEffectLeft = nullptr;
	GameScene* m_gameScene = nullptr;
	bool m_isBoosting = false;
	bool m_isTurning = false;
	float m_fSpeedBeforeTurn = 0;
	bool m_isMissileRight = false;
	int m_turnCount = 0;
	int m_trackingMissileNum = 0;

	CShaderResourceView m_specSRV;
	int m_hp = 500;
	int m_oldHp = 10000;
	float m_fuel = 1000;
	prefab::CFontRender* m_fontRender = nullptr;

	//音
	prefab::CSoundSource* m_missileShotSound = nullptr;
	prefab::CSoundSource* m_lockonSound = nullptr;
	prefab::CSoundSource* m_engineSound = nullptr;
	prefab::CSoundSource* m_missileEngageSound = nullptr;
	bool m_missileEngageSoundFlag = false;

	//ミッション用
	int m_shootMissileNum = 0;
	int m_damageCount = 0;
	int m_highSpeedCount = 0;
	int m_lowSpeedCount = 0;
};

