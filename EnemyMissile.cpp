#include "stdafx.h"
#include "EnemyMissile.h"
#include "Player.h"
#include <random>

int EnemyMissile::s_missileAvoidNum = 0;

void EnemyMissile::OnDestroy()
{

	DeleteGO(m_effect2);

	DeleteGO(m_skinModelRender);
}

bool EnemyMissile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Missile.cmo");
	m_effect2 = NewGO<prefab::CEffect>(0);
	m_effect2->Play(L"effect/Missile.efk");
	m_skinModelRender->SetScale({1.5f,1.5f,1.5f });

	m_trackingPlayer = FindGO<Player>("player");

	m_trackingPlayer->m_trackingMissileNum++;

	m_oldMoveDir = m_trackingPlayer->m_position - m_position;
	m_oldMoveDir.Normalize();
	return true;
}

void EnemyMissile::Update()
{
	if (m_trackingPlayer == nullptr)
	{
		DeleteGO(this);
	}


	if (m_effect2->IsPlay() == false)
	{
		m_effect2->Play(L"effect/Missile.efk");
	}

	//“G‚ğ’Ç”ö‚·‚é
	CVector3 diff = m_moveDir = m_trackingPlayer->m_position - m_position;
	m_moveDir.Normalize();

	//ƒ^[ƒ“‚µ‚½ê‡’Ç”ö‚µ‚È‚­‚È‚é
	if (m_trackingPlayer->m_isTurning == true && m_isRocking == true)
	{
		m_isRocking = false;
		m_trackingPlayer->m_trackingMissileNum--;

		//ƒ~ƒbƒVƒ‡ƒ“—p
		s_missileAvoidNum++;
	}

	if (m_isRocking == false)
		m_moveDir = m_oldMoveDir;

	m_oldMoveDir = m_moveDir;
	m_fSpeed += m_fSpeed / 20;
	if (m_fSpeed > 300)
	{
		m_fSpeed = 300;
	}
	m_position += m_moveDir * m_fSpeed;
	m_skinModelRender->SetPosition(m_position);

	m_effect2->SetPosition(m_position + m_moveDir * -25.0f);
	m_effect2->SetScale({ 15.0f,15.0f,15.0f });


	//“G‚Ì•û‚ÉŒü‚­
	CVector3 zx = { m_moveDir.x,0.0f,m_moveDir.z };
	zx.Normalize();

	//x²‚Ì‰ñ“]
	float degx = acos(m_moveDir.Dot(zx));
	//Šp“x‚µ‚©è‚É“ü‚ç‚È‚¢‚Ì‚ÅŒü‚¢‚Ä‚é•ûŒü‚ª‰º‚È‚ç•‰‚Ì”‚É‚·‚é
	if (m_moveDir.y > 0)
		degx = -degx;
	CQuaternion qRotx;
	qRotx.SetRotation(CVector3::AxisX, degx - odegx);

	//y²‚Ì‰ñ“]
	float degy = acos(CVector3::Front.Dot(zx));
	//Šp“x‚µ‚©è‚É“ü‚ç‚È‚¢‚Ì‚ÅŒü‚¢‚Ä‚é•ûŒü‚ª¶‚È‚ç•‰‚Ì”‚É‚·‚é
	if (m_moveDir.x < 0)
		degy = -degy;
	CQuaternion qRoty;
	qRoty.SetRotation(CVector3::AxisY, degy - odegy);

	m_qRot.Multiply(qRotx, m_qRot);
	m_qRot.Multiply(qRoty, m_qRot);

	m_skinModelRender->SetRotation(m_qRot);
	m_effect2->SetRotation(m_qRot);

	odegx = degx;
	odegy = degy;


	//“G‚Ì‹ß‚­‚Ü‚Å—ˆ‚½‚ç
	if (diff.Length() < 200)
	{
		prefab::CEffect* effect = NewGO<prefab::CEffect>(0);
		effect->SetPosition(m_trackingPlayer->m_position + m_trackingPlayer->m_moveDir * m_trackingPlayer->m_fSpeed * 20 + m_trackingPlayer->m_upDir * -200);
		effect->Play(L"effect/Explosion.efk");
		effect->SetScale({ 50.0f,50.0f,50.0f });
		m_trackingPlayer->m_hp -= 40.0f;
		m_trackingPlayer->m_trackingMissileNum--;

		m_explosionSound = NewGO<prefab::CSoundSource>(0);
		
		std::random_device seed_gen;
		std::mt19937_64 rnd(seed_gen());
		
		switch (rnd() % 2)
		{
		case 0:
			m_explosionSound->Init(L"sound/Boom0.wav");
			break;
		case 1:
			m_explosionSound->Init(L"sound/Boom1.wav");
			break;
		}

		m_explosionSound->Play(false);
		DeleteGO(this);
	}

	m_lifeSpan--;
	if (m_lifeSpan == 0)
		DeleteGO(this);
}