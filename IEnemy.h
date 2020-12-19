#pragma once
class Player;

class IEnemy : public IGameObject
{
public:
	virtual void OnDestroy() override = 0;
	virtual bool Start() override = 0;
	virtual void Update() override = 0;

	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position;
	bool m_isMortal = false;
	Player* m_targetPlayer = nullptr;
	CShaderResourceView m_specSRV;
};

