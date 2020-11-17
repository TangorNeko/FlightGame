#pragma once

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();

	prefab::CSkinModelRender* Vmodel = nullptr;
	CVector3 position;
	CVector3 MoveDir = CVector3::AxisZ;
	float fSpeed = 0;
	float movex = 0, movez = 0;
	float x=0, y=0, z=0;
	CQuaternion qRotX, qRotY, qRotZ,qRotCam;
	CQuaternion m_rotation;

	CVector3 cameraup;
	CQuaternion camerarot;

	bool a = false;
};

