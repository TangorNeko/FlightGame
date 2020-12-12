#include "stdafx.h"
#include "Laser.h"
#include "Player.h"
#include "Enemy.h"

void Laser::OnDestroy()
{
	DeleteGO(m_effect);
}

bool Laser::Start()
{
	m_targetPlayer = FindGO<Player>("player");
	m_effect = NewGO<prefab::CEffect>(0);
	m_effect->Play(L"effect/Laser.efk");
	m_effect->SetScale({ 25.0f,25.0f,100.0f });
	return true;
}

void Laser::Update()
{

	//レーザーを出す位置はアンテナからにする
	m_position.x = m_owner->m_position.x;
	m_position.y = m_owner->m_position.y + 950.0f;
	m_position.z = m_owner->m_position.z;

	m_effect->SetPosition(m_position);

	//レーザーの向きを取得
	CVector3 laserDir = m_targetPlayer->m_position - m_position;
	laserDir.Normalize();
	

	CVector3 zx = { laserDir.x,0.0f,laserDir.z };
	zx.Normalize();


	//X軸の回転
	//ドット積の結果が1を超える事があったので1より大きければxの角度をacos(1) = 0度にする
	if (laserDir.Dot(zx) > 1)
	{
		degx = 0.0f;
	}
	else
	{
		degx = acos(laserDir.Dot(zx));
	}

	//レーザーの角度しか分からないのでレーザーの向きが上向きなら角度をマイナスにする
	if (laserDir.y > 0)
		degx = -degx;

	//前フレームからの差分の回転をセット
	m_qRotX.SetRotation(CVector3::AxisX, degx - odegx);


	//Y軸の回転
	degy = acos(CVector3::Front.Dot(zx));

	//レーザーの角度しか分からないのでレーザーの向きが左向きなら角度をマイナスにする
	if (laserDir.x < 0)
		degy = -degy;

	//前フレームからの差分の回転をセット
	m_qRotY.SetRotation(CVector3::AxisY, degy - odegy);


	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotY);

	m_effect->SetRotation(m_rotation);
	
	
	//一定フレームごとにプレイヤーにダメージ
	m_damageCount++;
	if (m_damageCount == 50)
	{
		m_targetPlayer->m_hp--;
		m_damageCount = 0;
	}


	//現フレームの角度を記録しておく
	odegx = degx;
	odegy = degy;
}