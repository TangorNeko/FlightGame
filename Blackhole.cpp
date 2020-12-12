#include "stdafx.h"
#include "Blackhole.h"
#include "Player.h"
#include "RingGenerator.h"

void Blackhole::OnDestroy()
{
	DeleteGO(m_bhEffect);
}

bool Blackhole::Start()
{
	
	DeleteGO(FindGO<RingGenerator>("ringgenerator"));
	m_bhEffect = NewGO<prefab::CEffect>(0);
	m_bhEffect->Play(L"effect/blackhole.efk");

	player = FindGO<Player>("player");

	m_scaling.x = 1000.0f;
	m_scaling.y = 1000.0f;
	m_scaling.z = 1000.0f;

	bhradius = 700.0f;
	return true;
}

void Blackhole::Update()
{
	//ブラックホールエフェクトが再生されていないなら再生(実質的なループ)
	if (m_bhEffect->IsPlay() == false)
	{
		m_bhEffect->Play(L"effect/blackhole.efk");
	}
	m_bhEffect->SetScale(m_scaling);
	m_bhEffect->SetPosition(m_position);

	CVector3 rad = { 0.0f,0.0f,bhradius };
	dbg::DrawVector(rad,m_position,"bhradius");


	//吸い込む用にプレイヤーからブラックホールの中心までの距離を取得
	CVector3 pdiff = player->m_position - m_position;

	//ブラックホールの半径よりプレイヤーとの距離の方が近ければ、毎フレーム100の速さでブラックホールの中心に吸い寄せる
	if (pdiff.Length() < bhradius)
	{
		float a = pdiff.Length();
		pdiff.Normalize();
		player->m_position -= pdiff * 100;
	}
	

	//フラグを使ったブラックホールの拡大処理
	//isExpanding(拡大中)がtrueなら、毎フレームエフェクトの拡大率に150,半径に105足す(拡大率:半径 = 10 : 7)
	if (isExpanding) {
		m_scaling.x += 150.0f;
		m_scaling.y += 150.0f;
		m_scaling.z += 150.0f;

		bhradius += 105.0f;
	}
	else //拡大中でない(縮小中)なら徐々に縮小速度を増しながら縮小する
	{
		m_scaling.x -= expire;
		m_scaling.y -= expire;
		m_scaling.z -= expire;

		bhradius -= 0.7 * expire;

		expire *= 1.01f;
	}

	//縮小の結果ブラックホールの半径が60以下になれば削除
	if (bhradius < 60.0f)
	{
		DeleteGO(this);
	}

	//1800フレームまで拡大しその後縮小する
	if (count == 1800)
	{
		isExpanding = false;
	}

	count++;
	
}