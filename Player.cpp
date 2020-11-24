#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

void Player::OnDestroy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_spriteRender);
}

bool Player::Start()
{
	//プレイヤーのモデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/Vehicle.cmo");
	m_skinModelRender->SetScale({ 0.1f,0.1f,0.1f });

	//ロックオンマーカーのスプライトを作成
	m_spriteRender = NewGO<prefab::CSpriteRender>(0);
	m_spriteRender->Init(L"sprite/lock.dds", 128, 128);

	return true;
}

void Player::Update()
{
	//スピード
	m_fSpeed = 10;

	//そのフレームごとの移動量を使うので毎フレーム0にする
	m_x = m_y = m_z = 0;

	//入力に応じて角度を変える
	if (Pad(0).IsPress(enButtonDown))
		m_x--;

	if (Pad(0).IsPress(enButtonUp))
		m_x++;

	/*
	if (Pad(0).IsPress(enButtonA))
		m_y--;

	if (Pad(0).IsPress(enButtonB))
		m_y++;
	*/

	if (Pad(0).IsPress(enButtonRight))
		m_z--;

	if (Pad(0).IsPress(enButtonLeft))
		m_z++;


	//入力された角度をモデルに反映する
	m_qRotX.SetRotationDeg(CVector3::AxisX, m_x);

	m_qRotZ.SetRotationDeg(CVector3::AxisZ, m_z);

	m_rotation.Multiply(m_qRotX, m_rotation);
	m_rotation.Multiply(m_qRotZ, m_rotation);


	//入力された角度を進行方向に反映する
	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	m_moveDir.x = mRot.m[2][0];
	m_moveDir.y = mRot.m[2][1];
	m_moveDir.z = mRot.m[2][2];
	m_position += m_moveDir * m_fSpeed;

	//ロックオン用の関数
	Lockon();

	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetPosition(m_position);
}

void Player::Lockon()
{
	//最高ロックオン可能距離を設定する
	CVector3 Lockonpos = m_moveDir * 100000.0f;

	
	//ロックオン可能な角度を設定
	const int lockondegree = 20;


	//最高ロックオン可能距離より敵の位置までの距離が短いかつロックオン可能な角度内ならロックオン距離を更新する処理を繰り返す
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool {
		CVector3 tmp1 = enemy->m_position - this->m_position,
			tmp2 = Lockonpos - this->m_position;
		if (tmp1.Length() < tmp2.Length())
		{
			tmp1.Normalize();
			if(this->m_moveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180))
			Lockonpos = enemy->m_position;
		}
		return true;
		}
	);

	//0ならどの敵もロックオンできていない。
	CVector3 Check = Lockonpos - m_moveDir * 100000.0f;
	
	//ロックオンした敵のワールド座標からスクリーン座標に変換したものを受け取る用
	CVector2 spritepos;

	//ロックオンできているなら敵の位置にロックオンマーカーを表示する
	if (Check.Length())
	{
		dbg::DrawVector(Lockonpos, { 0.0f,0.0f,0.0f }, "locking");
		MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
		CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
		m_spriteRender->SetPosition(screenpos);
	}
	//できていないならプレイヤーのやや上に表示
	else
	{
		m_spriteRender->SetPosition({ 0.0f,150.0f,0.0f });
	}
}