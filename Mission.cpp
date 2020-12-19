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

	//画像のかわりにテキストを表示　テスト用
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
		case 0://ミッション1
			teststring = L"ミッション1\n同じ色のリングを連続でくぐる";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 1://ミッション1 裏
			teststring = L"ミッション1 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 2://ミッション2
			teststring = L"ミッション2\nリングを交互にくぐる";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 3://ミッション2 裏
			teststring = L"ミッション2 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 4://ミッション3
			teststring = L"ミッション3\n敵をX体倒す";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 5://ミッション3 裏
			teststring = L"ミッション3 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 6://ミッション4
			teststring = L"ミッション4\nレーザーを食らわずUFOをX体倒す";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 7://ミッション4 裏
			teststring = L"ミッション4 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 8://ミッション5
			teststring = L"ミッション5\n背面から敵をX体倒す";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 9://ミッション5 裏
			teststring = L"ミッション5 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 10://ミッション10
			teststring = L"ミッション6\n最高速度をX秒維持する";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		case 11://ミッション10 裏
			teststring = L"ミッション6 裏";
			string = teststring.c_str();
			m_fontRender->SetText(string);
			break;
		}
	}
	else
	{
		switch (m_receiveMission)
		{
		case 0://ミッション1
			teststring = L"確定されたミッション1\n同じ色のリングを連続でくぐる　\n赤= " + std::to_wstring(Ring::s_redRingStreak) + L"青 =" + std::to_wstring(Ring::s_blueRingStreak);
			m_fontRender->SetText(teststring.c_str());
			break;
		case 1://ミッション1 裏
			teststring = L"確定されたミッション1 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 2://ミッション2
			teststring = L"確定されたミッション2\n違う色のリングを交互にくぐる \n 取得数 = " + std::to_wstring(Ring::s_alternateRingStreak);
			m_fontRender->SetText(teststring.c_str());
			break;
		case 3://ミッション2 裏
			teststring = L"確定されたミッション2 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 4://ミッション3
			teststring = L"確定されたミッション3";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 5://ミッション3 裏
			teststring = L"確定されたミッション3 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 6://ミッション4
			teststring = L"確定されたミッション4";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 7://ミッション4 裏
			teststring = L"確定されたミッション4 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 8://ミッション5
			teststring = L"確定されたミッション5";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 9://ミッション5 裏
			teststring = L"確定されたミッション5 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 10:
			teststring = L"確定されたミッション6";
			m_fontRender->SetText(teststring.c_str());
			break;
		case 11:
			teststring = L"確定されたミッション6 裏";
			m_fontRender->SetText(teststring.c_str());
			break;
		}

		//m_missionTimer--;
	}

	if (m_missionTimer <= 0)
		m_isMissionEnd = true;
}