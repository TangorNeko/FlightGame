#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "TitlePlayer.h"
#include "SpaceDock.h"
#include "SceneChange.h"
#include "tkEngine/light/tkDirectionLight.h"

void TitleScene::OnDestroy()
{
	DeleteGO(space);
	DeleteGO(spacedock);
	DeleteGO(player);
	DeleteGOs("DirectionLight");
	DeleteGO(m_titleBGM);
}

bool TitleScene::Start()
{
	auto light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	CVector3 dir = { 1.0f, 0.0f, 0.0f };

	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	//
	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { 1.0f, 1.0f, 0.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { -1.0f, 0.0f, 1.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	light = NewGO<prefab::CDirectionLight>(0, "DirectionLight");
	dir = { -1.0f, -1.0f, 0.0f };
	dir.Normalize();
	light->SetDirection(dir);
	light->SetColor({ 2.0f, 1.0f, 1.0f, 1.0f });

	LightManager().SetAmbientLight({ 0.5f, 0.5f, 0.5f });


	space = NewGO<prefab::CSky>(0, "space");
	space->SetSkyCubeMapFilePath(L"sprite/EarthCube.dds");
	space->SetScale(1000000.0f);
	player = NewGO<TitlePlayer>(0, "player");
	player->m_position = { 0.0f,0.0f,0.0f };

	spacedock = NewGO<SpaceDock>(0,"spacedock");

	MainCamera().SetPosition({ 0.0f,100.0f,-200.0f });
	MainCamera().SetTarget(player->m_position);
	MainCamera().SetNear(10.0f);
	MainCamera().SetFar(1500000.0f);

	m_titleBGM = NewGO<prefab::CSoundSource>(0, "bgm");
	m_titleBGM->Init(L"sound/Science_Mystery.wav");
	m_titleBGM->SetVolume(0.25f);
	//m_titleBGM->Play(true);
	return true;
}

void TitleScene::Update()
{	

	if (Pad(0).IsTrigger(enButtonSelect) || player->m_position.z > 35500.0f)
	{
		NewGO<GameScene>(0, "gamescene");
		DeleteGO(this);
	}

	if (player->m_position.z > 34000.0f && m_sceneChangeFlag == false)
	{
		SceneChange* scenechange = NewGO<SceneChange>(0);
		scenechange->SceneChangeColor = { 0.9f,1.0f,1.0f };

		prefab::CSoundSource* teleportSound = NewGO<prefab::CSoundSource>(0);
		teleportSound->Init(L"sound/Teleport.wav");
		teleportSound->SetVolume(1.5f);
		teleportSound->Play(false);

		m_sceneChangeFlag = true;
	}

	space->SetPosition(player->m_position);

	CVector3 titlecampos = player->m_position;
	titlecampos.y += 50.0f;
	titlecampos.z -= 300.0f;
	CVector3 titletargetpos = player->m_position;
	titletargetpos.y += 50.0f;
	MainCamera().SetPosition(titlecampos);
	MainCamera().SetTarget(titletargetpos);
	MainCamera().SetUp(CVector3::Up);

	MainCamera().Update();
}