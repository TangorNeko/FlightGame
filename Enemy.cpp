#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

void Enemy::OnDestroy()
{
	//DeleteGO(m_laser);

	//削除される時レーザーが発射されていた時だけ削除(他の場合はもう削除されているため)
	if (m_isShooting == true)
	 DeleteGO(m_effect);

	DeleteGO(m_skinModelRender);
}

bool Enemy::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/UFOMAPPED.cmo");
	m_skinModelRender->SetScale({ 5.0f,5.0f,5.0f });
	
	m_specSRV.CreateFromDDSTextureFromFile(L"modelData/Vehicle.fbm/vehicle_spec.dds");
	m_skinModelRender->FindMaterial([&](auto material) {
		material->SetSpecularMap(m_specSRV.GetBody());
		});

	m_targetPlayer = FindGO<Player>("player");

	return true;
}

void Enemy::Update()
{
	
	CVector3 diff;
	//プレイヤーのとの距離
	diff = m_targetPlayer->m_position - m_position;


	//プレイヤーとの距離20000以内かつまだレーザーを発射していないなら
	if (diff.Length() <= 20000 && m_isShooting == false)
	{
		/*
		m_laser = NewGO<Laser>(0, "laser");
		m_laser->m_owner = this;
		*/
		m_effect = NewGO<prefab::CEffect>(0);
		m_effect->Play(L"effect/Laser.efk");
		m_effect->SetScale({ 25.0f,25.0f,100.0f });
		
		m_isShooting = true;
		m_isDeleted = false;
	}

	if (m_effect != nullptr)
	{
		//レーザーを出す位置はアンテナからにする
		laserpos.x = m_position.x;
		laserpos.y = m_position.y + 950.0f;
		laserpos.z = m_position.z;

		m_effect->SetPosition(laserpos);

		//レーザーの向きを取得
		CVector3 laserDir = m_targetPlayer->m_position - laserpos;
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

		odegx = degx;
		odegy = degy;
	}

	//プレイヤーとの距離が20000以上離れていて、かつレーザーを発射していたら
	if (diff.Length() > 20000 && m_isShooting == true && m_effect != nullptr)
	{
		DeleteGO(m_effect);
		m_effect = nullptr;
		
		m_isShooting = false;
		m_isDeleted = true;
	}

	//レーザーのダメージを付与
	if (m_isShooting == true)
	{
		m_damageCount++;
		if (m_damageCount == 10)
		{
			m_targetPlayer->m_hp--;
			m_damageCount = 0;
		}
	}

	count++;
	if (count < 100)
	{
		m_position.x += 100;
		m_position.y += 20;
		m_position.z += 50;
	}
	else if (count < 200)
	{
		m_position.x -= 100;
		m_position.y -= 20;
		m_position.z -= 50;
	}
	else
		count = 0;
		
	m_skinModelRender->SetPosition(m_position);
}