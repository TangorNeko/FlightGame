#include "stdafx.h"
#include "MissileEnemy.h"
#include "Player.h"
#include "EnemyMissile.h"

int MissileEnemy::m_missileEnemyNum = 0;

void MissileEnemy::OnDestroy()
{

	DeleteGO(m_skinModelRender);

	m_missileEnemyNum--;
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
	return true;
}

void MissileEnemy::Update()
{
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

	
	
	m_position += m_moveDir * 100;

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_rotation);


	//プレイヤーとの距離3000以内かつ20度以内ならミサイルを撃つ
	if (diff.Length() < 30000&& m_moveDir.Dot(m_missileDir) > cos(20 * CMath::PI / 180) && m_shotCooldown == 0)
	{
		
		EnemyMissile* enemymissile = NewGO<EnemyMissile>(0, "enemymissile");
		enemymissile->m_position = m_position;

		m_shotCooldown = 3000;
	}

	//ミサイルの再発射間隔
	m_shotCooldown--;
	if (m_shotCooldown < 0)
		m_shotCooldown = 0;
}
