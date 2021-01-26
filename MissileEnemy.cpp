#include "stdafx.h"
#include "MissileEnemy.h"
#include "Player.h"
#include "EnemyMissile.h"
#include "GameScene.h"

int MissileEnemy::m_missileEnemyNum = 0;

void MissileEnemy::OnDestroy()
{
	DeleteGO(m_enemyPosRender);
	DeleteGO(m_skinModelRender);

	m_missileEnemyNum--;
	if (m_missileEnemyNum < 0) //応急策　ブラックホールフェーズで何故か0以下になることがある
	{
		m_missileEnemyNum = 0;
	}
}

bool MissileEnemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/HFEnemy.cmo");
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });

	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
		});

	m_targetPlayer = FindGO<Player>("player");

	m_missileEnemyNum++;

	m_gameScene = FindGO<GameScene>("gamescene");

	m_isUFO = false;
	return true;
}

void MissileEnemy::Update()
{
	if (m_gameScene->m_isPlayerDead == true)
	{
		DeleteGO(this);
	}

	CVector3 diff;
	//プレイヤーのとの距離
	diff = m_targetPlayer->m_position - m_position;
	m_missileDir = diff;
	m_missileDir.Normalize();

	
	if (diff.Length() < 30000)
		m_moveDir = m_oldMoveDir;
	else
		m_moveDir = diff;

	m_moveDir.Normalize();

	m_oldMoveDir = m_moveDir;

	CVector3 zx = { m_moveDir.x ,0.0f,m_moveDir.z};
	zx.Normalize();

	//X軸の回転
	if (m_moveDir.Dot(zx) > 1)
	{
		degx = 0.0f;
	}
	else
	{
		degx = acosf(m_moveDir.Dot(zx));
	}

	//角度しか手に入らないので向いてる方向が下なら負の数にする
	if (m_moveDir.y > 0)
		degx = -degx;

	m_qRotX.SetRotation(CVector3::AxisX, degx - odegx);

	//y軸の回転
	degy = acosf(CVector3::Front.Dot(zx));
	//角度しか手に入らないので向いてる方向が左なら負の数にする
	if (m_moveDir.x < 0)
		degy = -degy;

	m_qRotY.SetRotation(CVector3::AxisY, degy - odegy);

	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotY);

	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);

	odegx = degx;
	odegy = degy;

	m_rightDir.x = mRot.m[0][0];
	m_rightDir.y = mRot.m[0][1];
	m_rightDir.z = mRot.m[0][2];

	m_upDir.x = mRot.m[1][0];
	m_upDir.y = mRot.m[1][1];
	m_upDir.z = mRot.m[1][2];

	
	
	m_position += m_moveDir * 120;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);


	//プレイヤーとの距離3000以内かつ50度以内かつブラックホールフェーズでないならミサイルを撃つ
	if (diff.Length() < 15000&& m_moveDir.Dot(m_missileDir) > cos(35 * CMath::PI / 180) && m_shotCooldown == 0 && m_gameScene->m_isBlackholePhase == false && m_gameScene->m_isPlayerDead == false)
	{
		
		EnemyMissile* enemymissile = NewGO<EnemyMissile>(0, "enemymissile");
		enemymissile->m_position = m_position;

		m_shotCooldown = 1200;

		//ミッション用
		m_isAttackSuccess = true;
	}

	//ミサイルの再発射間隔
	m_shotCooldown--;
	if (m_shotCooldown < 0)
		m_shotCooldown = 0;

	//自機から125000より離れると削除する
	if (diff.Length() > 125000)
	{
		DeleteGO(this);
	}
}
