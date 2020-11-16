#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	Vmodel = NewGO<prefab::CSkinModelRender>(0);
	Vmodel->Init(L"modelData/Vehicle.cmo");
	Vmodel->SetScale({ 0.1f,0.1f,0.1f });

	return true;
}

void Player::Update()
{
	fSpeed = 10;
	x = y = z = 0;
	if (Pad(0).IsPress(enButtonDown))
	{
		x--;
		movex--;
	}

	if (Pad(0).IsPress(enButtonUp))
	{
		x++;
		movex++;
	}

	x += Pad(0).GetLStickYF();
	movex += Pad(0).GetLStickYF();
	/*
	if (Pad(0).IsPress(enButtonA))
	{
		y--;
	}

	if (Pad(0).IsPress(enButtonB))
	{
		y++;
	}
	*/

	if (Pad(0).IsPress(enButtonRight))
	{
		z--;
		movez--;
	}

	if (Pad(0).IsPress(enButtonLeft))
	{
		z++;
		movez++;
	}

	qRotX.SetRotationDeg(CVector3::AxisX, x);

	qRotZ.SetRotationDeg(CVector3::AxisZ, z);

	m_rotation.Multiply(qRotX, m_rotation);
	m_rotation.Multiply(qRotZ, m_rotation);

	auto mRot = CMatrix::Identity;
	mRot.MakeRotationFromQuaternion(m_rotation);
	MoveDir.x = mRot.m[2][0];
	MoveDir.y = mRot.m[2][1];
	MoveDir.z = mRot.m[2][2];

	//qRotX.Apply(MoveDir);

	position += MoveDir * 100;

	Vmodel->SetRotation(m_rotation);
	Vmodel->SetPosition(position);
}