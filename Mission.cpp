#include "stdafx.h"
#include "Mission.h"
#include "Ring.h"
#include <random>

void Mission::OnDestroy()
{
	DeleteGO(m_fontRender);
}

bool Mission::Start()
{
	std::random_device seed_gen;
	std::mt19937_64 rnd(seed_gen());
	m_receiveMission = (rnd() % 6) * 2;
	m_receiveMission = 2;

	//�摜�̂����Ƀe�L�X�g��\���@�e�X�g�p
	m_fontRender = NewGO<prefab::CFontRender>(0);
	m_fontRender->SetScale(0.5f);
	m_fontRender->SetPosition({ 300.0f,-250.0f });
	return true;
}

void Mission::Update()
{
	if (Pad(0).IsTrigger(enButtonLB2) && m_isMissionConfirm == false)
	{
		m_isMissionConfirm = true;

		switch (m_receiveMission)
		{
		case 0:
			Ring::s_redRingStreak = 0;
			Ring::s_blueRingStreak = 0;
			break;
		case 1:

		case 2:
			Ring::s_alternateRingStreak = 0;
			Ring::s_lastRingFlag = 0;
			break;
		case 3:

		case 4:

		case 5:

		case 6:

		case 7:

		case 8:

		case 9:

		case 10:

		case 11:
			break;
		}
	}


	if (m_isMissionConfirm == false)
	{
		if (Pad(0).IsTrigger(enButtonLB1))
		{
			if (m_isSelectReverseMission)
			{
				m_receiveMission--;
				m_isSelectReverseMission = false;
			}
			else
			{
				m_receiveMission++;
				m_isSelectReverseMission = true;
			}
		}
		switch (m_receiveMission)
		{
		case 0://�~�b�V����1
			teststring = L"�~�b�V����1\n�����F�̃����O��A���ł�����";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 1://�~�b�V����1 ��
			teststring = L"�~�b�V����1 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 2://�~�b�V����2
			teststring = L"�~�b�V����2\n�����O�����݂ɂ�����";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 3://�~�b�V����2 ��
			teststring = L"�~�b�V����2 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 4://�~�b�V����3
			teststring = L"�~�b�V����3\n�G��X�̓|��";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 5://�~�b�V����3 ��
			teststring = L"�~�b�V����3 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 6://�~�b�V����4
			teststring = L"�~�b�V����4\n���[�U�[��H��킸UFO��X�̓|��";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 7://�~�b�V����4 ��
			teststring = L"�~�b�V����4 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 8://�~�b�V����5
			teststring = L"�~�b�V����5\n�w�ʂ���G��X�̓|��";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 9://�~�b�V����5 ��
			teststring = L"�~�b�V����5 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 10://�~�b�V����10
			teststring = L"�~�b�V����6\n�ō����x��X�b�ێ�����";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 11://�~�b�V����10 ��
			teststring = L"�~�b�V����6 ��";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		}
	}
	else
	{
		switch (m_receiveMission)
		{
		case 0://�~�b�V����1
			teststring = L"�m�肳�ꂽ�~�b�V����1\n�����F�̃����O��A���ł�����@\n��= " + std::to_wstring(Ring::s_redRingStreak) + L"�� =" + std::to_wstring(Ring::s_blueRingStreak);
			m_fontRender->SetText(teststring.c_str());
			break;
		case 1://�~�b�V����1 ��
			teststring = L"�m�肳�ꂽ�~�b�V����1 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 2://�~�b�V����2
			teststring = L"�m�肳�ꂽ�~�b�V����2\n�Ⴄ�F�̃����O�����݂ɂ����� \n �擾�� = " + std::to_wstring(Ring::s_alternateRingStreak);
			m_fontRender->SetText(teststring.c_str());
			break;
		case 3://�~�b�V����2 ��
			teststring = L"�m�肳�ꂽ�~�b�V����2 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 4://�~�b�V����3
			teststring = L"�m�肳�ꂽ�~�b�V����3";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 5://�~�b�V����3 ��
			teststring = L"�m�肳�ꂽ�~�b�V����3 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 6://�~�b�V����4
			teststring = L"�m�肳�ꂽ�~�b�V����4";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 7://�~�b�V����4 ��
			teststring = L"�m�肳�ꂽ�~�b�V����4 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 8://�~�b�V����5
			teststring = L"�m�肳�ꂽ�~�b�V����5";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 9://�~�b�V����5 ��
			teststring = L"�m�肳�ꂽ�~�b�V����5 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 10:
			teststring = L"�m�肳�ꂽ�~�b�V����6";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 11:
			teststring = L"�m�肳�ꂽ�~�b�V����6 ��";
			m_fontRender->SetText(teststring.c_str());
			break;
		}

		//m_missionTimer--;
	}

	if (m_missionTimer <= 0)
		m_isMissionEnd = true;
}