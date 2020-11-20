#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"

void Player::OnDestroy()
{
	DeleteGO(Vmodel);
	DeleteGO(lock);
}

bool Player::Start()
{
	//プレイヤーのモデルを作成
	Vmodel = NewGO<prefab::CSkinModelRender>(0);
	Vmodel->Init(L"modelData/Vehicle.cmo");
	Vmodel->SetScale({ 0.1f,0.1f,0.1f });

	//ロックオンマーカーのスプライトを作成
	lock = NewGO<prefab::CSpriteRender>(0);
	lock->Init(L"sprite/lock.dds", 128, 128);

	return true;
}

void Player::Update()
{
	//スピード
	fSpeed = 10;

	//そのフレームごとの移動量を使うので毎フレーム0にする
	x = y = z = 0;

	//入力に応じて角度を変える
	if (Pad(0).IsPress(enButtonDown))
		x--;

	if (Pad(0).IsPress(enButtonUp))
		x++;

	/*
	if (Pad(0).IsPress(enButtonA))
		y--;

	if (Pad(0).IsPress(enButtonB))
		y++;
	*/

	if (Pad(0).IsPress(enButtonRight))
		z--;

	if (Pad(0).IsPress(enButtonLeft))
		z++;


	//入力された角度をモデルに反映する
	qRotX.SetRotationDeg(CVector3::AxisX, x);

	qRotZ.SetRotationDeg(CVector3::AxisZ, z);

	m_rotation.Multiply(qRotX, m_rotation);
	m_rotation.Multiply(qRotZ, m_rotation);


	//入力された角度を進行方向に反映する
	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	MoveDir.x = mRot.m[2][0];
	MoveDir.y = mRot.m[2][1];
	MoveDir.z = mRot.m[2][2];
	position += MoveDir * fSpeed;

	//ロックオン用の関数
	Lockon();

	Vmodel->SetRotation(m_rotation);
	Vmodel->SetPosition(position);
}

void Player::Lockon()
{
	//最高ロックオン可能距離を設定する
	CVector3 Lockonpos = MoveDir * 100000.0f;

	
	//ロックオン可能な角度を設定
	const int lockondegree = 20;


	//最高ロックオン可能距離より敵の位置までの距離が短いかつロックオン可能な角度内ならロックオン距離を更新する処理を繰り返す
	QueryGOs<Enemy>("enemy", [&Lockonpos,lockondegree,this](Enemy* enemy)->bool {
		CVector3 tmp1 = enemy->position - this->position,
			tmp2 = Lockonpos - this->position;
		if (tmp1.Length() < tmp2.Length())
		{
			tmp1.Normalize();
			if(this->MoveDir.Dot(tmp1) > cos(lockondegree * CMath::PI / 180))
			Lockonpos = enemy->position;
		}
		return true;
		}
	);

	//0ならどの敵もロックオンできていない。
	CVector3 Check = Lockonpos - MoveDir * 100000.0f;
	
	//ロックオンした敵のワールド座標からスクリーン座標に変換したものを受け取る用
	CVector2 spritepos;

	//ロックオンできているなら敵の位置にロックオンマーカーを表示する
	if (Check.Length())
	{
	dbg::DrawVector(Lockonpos, { 0.0f,0.0f,0.0f }, "locking");
	MainCamera().CalcScreenPositionFromWorldPosition(spritepos, Lockonpos);
	CVector3 screenpos = { spritepos.x,spritepos.y,0.0f };
	lock->SetPosition(screenpos);
	}
	//できていないならプレイヤーのやや上に表示
	else
	{
		lock->SetPosition({ 0.0f,150.0f,0.0f });
	}
}