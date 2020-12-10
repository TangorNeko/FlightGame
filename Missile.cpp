#include "stdafx.h"
#include "Missile.h"
#include "Enemy.h"

void Missile::OnDestroy()
{
	DeleteGO(m_effect2);
	DeleteGO(m_skinModelRender);
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modeldata/testMissile.cmo");
	m_effect2 = NewGO<prefab::CEffect>(0);
	//m_skinModelRender->SetScale({ 1.0f,1.0f,100.0f });
	return true;
}

void Missile::Update()
{	
	if (m_effect2->IsPlay() == false)
	{
		m_effect2->Play(L"effect/Missile3.efk");
	}

	m_effect2->SetPosition(m_position);
	m_effect2->SetScale({ 5.0f,5.0f,5.0f });

	//“G‚ğ’Ç”ö‚·‚é
	CVector3 diff = m_moveDir = m_trackingEnemy->m_position - m_position;
	m_moveDir.Normalize();
	m_fSpeed += m_fSpeed / 10;
	if (m_fSpeed > 300)
	{
		m_fSpeed = 300;
	}
	m_position += m_moveDir * m_fSpeed;
	m_skinModelRender->SetPosition(m_position);


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
		effect->SetPosition(m_trackingEnemy->m_position);
		effect->SetScale({ 100.0f,100.0f,100.0f });
		effect->Play(L"effect/fire3.efk");
		DeleteGO(m_trackingEnemy);
		DeleteGO(this);
	}
}