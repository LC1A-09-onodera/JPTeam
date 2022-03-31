#include "Oserro.h"
#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"
#include "../BaseDirectX/Input.h"

list<Othello> OthelloManager::othellos;
OthelloModel OthelloManager::oserroModel;
vector<vector<SendOthelloData>> OthelloManager::sendDatas;
using namespace OthelloConstData;

OthelloData::OthelloData()
{
	for (int i = 0; i < 8; i++)
	{
		FrontActiveAngle.push_back(true);
	}
}

void Othello::Init(Model *model)
{
	this->model = model;
	each.CreateConstBuff0();
	each.CreateConstBuff1();
}

void Othello::Update()
{
	//�^�C�v��NONE�o�Ȃ������琶���Ă���
	bool isAlive = data.type != NONE;
	if (!isAlive) return;
	float x = (data.widthPos * cellScale * 2);
	float y = -(data.heightPos * cellScale * 2);

	if (!data.isPlayer)
	{
		each.position = XMVECTOR{ x, y ,0, 0 };
		if (data.isFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}
	}
}

void Othello::Draw()
{
	if (data.type != NONE)
	{
		//each.rotation = {90, 0,0 };
		//each.scale = {1, 1, 1};
		//each.rotation.y ++;
		model->Update(&each);
		Draw3DObject(*model);
	}
}

void Othello::Finalize()
{
}

void Othello::RightRevers()
{
	float rate = 0;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;

	if (data.isFront)
	{
		each.rotation.y = 180.0f * easeRate;
	}
	else
	{
		each.rotation.y = 180.0f * easeRate + 180.0f;
	}


	if (rate >= 1.0f)
	{
		//��]�t���Ooff

		//�\���̕ύX
		data.isFront = !data.isFront;
		if (data.isFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}

	}
}
void Othello::LeftRevers()
{
	float rate = 0;
	float easeRate = EaseInOutQuad(XMFLOAT3{}, XMFLOAT3{ 1, 0, 0 }, rate).x;
	if (data.isFront)
	{
		each.rotation.y = -180.0f * easeRate;
	}
	else
	{
		each.rotation.y = -180.0f * easeRate + 180.0f;
	}

	if (rate >= 1.0f)
	{
		//��]�t���Ooff

		//�\���̕ύX
		data.isFront = !data.isFront;

		if (data.isFront)
		{
			each.rotation.y = 0;
		}
		else
		{
			each.rotation.y = 180;
		}
	}

}
void Othello::Controll(const XMFLOAT3 &mousePos)
{
	XMVECTOR playerPos = { data.widthPos * cellScale,data.heightPos * -cellScale , 0, 0 };

	playerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);
	XMVECTOR dist = ConvertXMFLOAT3toXMVECTOR(mousePos - playerPos);
	XMVECTOR angle = XMVector3Normalize(dist);


	bool isOver = false;
	while (true)
	{
		if (Input::KeyTrigger(DIK_1))
		{
			int hoge = 0;
		}


		playerPos = { data.widthPos * cellScale * 2,data.heightPos * -cellScale * 2, 0, 0 };
		playerPos += ConvertXMFLOAT3toXMVECTOR(stageLeftTop);

		dist = ConvertXMFLOAT3toXMVECTOR(mousePos - playerPos);
		angle = XMVector3Normalize(dist);
		float length = XMVector3Length(dist).m128_f32[0];
		//�������}�X�̑傫����蒷�������ꍇ

			//�c���ǂ��炪������
		if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
		{//���̂ق�������������
			float length = fabs(dist.m128_f32[0]);
			if (length >= cellScale)
			{
				//���E�ǂ��炩
				if (angle.m128_f32[0] > 0)
				{//�E�ɍs��
					data.widthPos++;
				}
				else
				{//���ɍs��
					data.widthPos--;
				}
			}
			else
			{
				break;
			}
		}
		else
		{//�c�̂ق�������������
			float length = fabs(dist.m128_f32[1]);
			if (length >= cellScale)
			{
				//�㉺�ǂ��炩
				if (angle.m128_f32[1] < 0)
				{//���ɍs��
					data.heightPos++;
				}
				else
				{//��ɍs��
					data.heightPos--;
				}

			}
			else
			{
				break;
			}
		}

		//�͈͂𒴉߂�����

		isOver = (data.widthPos < 0 ||
			data.widthPos >= fieldSize ||
			data.heightPos < 0 ||
			data.heightPos >= fieldSize);
		if (isOver)
		{
			if (data.widthPos < 0)
			{
				data.widthPos = 0;
			}
			if (data.widthPos >= fieldSize)
			{
				data.widthPos = fieldSize - 1;
			}
			if (data.heightPos < 0)
			{
				data.heightPos = 0;
			}
			if (data.heightPos >= fieldSize)
			{
				data.heightPos = fieldSize - 1;
			}

			break;
		}
		//�Ђ�����Ԃ�
		data.isFront = !data.isFront;

	}


	//�X������
	//�c���ǂ��炪������

	if (!isOver)
	{
		if (data.isFront)
		{
			if (Input::KeyTrigger(DIK_1))
			{
				int hoge = 0;
			}
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//���̂ق�������������
				//���E�ǂ��炩
				playerPos.m128_f32[0] += dist.m128_f32[0];
				each.rotation.y = dist.m128_f32[0] * -90;
				each.rotation.x = 0;
			}
			else
			{//�c�̂ق�������������

				playerPos.m128_f32[1] += dist.m128_f32[1];
				//�㉺�ǂ��炩
				each.rotation.x = dist.m128_f32[1] * 90;
				each.rotation.y = 0;
			}
		}
		else
		{
			if (fabs(angle.m128_f32[0]) > fabs(angle.m128_f32[1]))
			{//���̂ق�������������
				//���E�ǂ��炩
				playerPos.m128_f32[0] += dist.m128_f32[0];
				each.rotation.y = dist.m128_f32[0] * -90 + 180;
				each.rotation.x = 0;
			}
			else
			{//�c�̂ق�������������
				playerPos.m128_f32[1] += dist.m128_f32[1];
				//�㉺�ǂ��炩
				each.rotation.x = dist.m128_f32[1] * 90 + 180;
				each.rotation.y = 0;
			}

		}
	}
	each.position = playerPos;
}

void Othello::Spawn(OthelloType type, int x, int y, bool isFront)
{
	if (data.type == NONE)
	{
		data.widthPos = x;
		data.heightPos = y;
		data.type = type;
		data.isFront = isFront;
	}
}

void OthelloManager::Init()
{
	oserroModel.CreateModel("newOserro", ShaderManager::playerShader);
	Othello player;
	player.Spawn(NORMAL, 4, 4);
	player.Init(&oserroModel);
	OthelloData *playerData = player.GetGameData();
	playerData->isPlayer = true;
	othellos.push_back(player);

	sendDatas.resize(fieldSize);

	auto itr = sendDatas.begin();
	for (; itr != sendDatas.end(); itr++)
	{
		itr->resize(fieldSize);
	}
}

void OthelloManager::Update()
{
	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Update();
	}
}

void OthelloManager::Draw()
{
	if (othellos.size() == 0) return;

	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Draw();
	}

}

void OthelloManager::Finalize()
{
	auto itr = othellos.begin();
	for (; itr != othellos.end(); ++itr)
	{
		itr->Finalize();
	}

}

void OthelloManager::Controll()
{
	//�}�E�X�̃N���b�N�����ꂽ�u��
	if (false)
	{
		SetPlayer();
	}
	//�}�E�X���h���b�O��
	if (true)
	{
		Move();
	}

	//�}�E�X���h���b�v������
	if (false)
	{
		RemovePlayer();
	}


}

void OthelloManager::SetPlayer()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop;
	//�ǂ̃}�X�Ƀ}�E�X������̂����m�F
	int x = stagePos.x / cellScale;
	int y = stagePos.y / cellScale;
	auto itr = othellos.begin();
	for (; itr != othellos.end(); itr++)
	{
		if (itr->GetGameData()->widthPos == x && itr->GetGameData()->heightPos == y)
		{
			itr->GetGameData()->isPlayer = true;
			break;
		}
	}
}

void OthelloManager::Move()
{
	XMFLOAT3 MousePos = Camera::MousePosition(0.0f);
	XMFLOAT3 stagePos = MousePos - stageLeftTop;
	//�ǂ̃}�X�Ƀ}�E�X������̂����m�F
	int x = stagePos.x / cellScale;
	int y = stagePos.y / cellScale;

	auto itr = othellos.begin();

	//�v���C���[��T��
	for (; itr != othellos.end(); ++itr)
	{
		OthelloData *data = itr->GetGameData();
		if (data->isPlayer)
		{
			break;
		}
	}
	itr->Controll(MousePos);
	//�i�܂����̍��W���v���C���[�̃}�X�̍��W�Ɠ��ꂾ������j�܂��́A
	//�i�ړ���̃}�X�ɋ��������j
}

void OthelloManager::RemovePlayer()
{
	auto itr = othellos.begin();

	//�v���C���[��T��
	for (; itr != othellos.end(); ++itr)
	{
		OthelloData *data = itr->GetGameData();
		if (data->isPlayer)
		{
			break;
		}
	}
	itr->GetGameData()->isPlayer = false;
	int x = itr->GetGameData()->widthPos;
	int y = itr->GetGameData()->heightPos;
}

void OthelloManager::AddPanel()
{
	Othello panelA, panelB, panelC, panelD;
	panelA.Spawn(NORMAL, 2, 0, true);
	panelA.Init(&oserroModel);
	othellos.push_back(panelA);

	panelB.Spawn(NORMAL, 3, 0, false);
	panelB.Init(&oserroModel);
	othellos.push_back(panelB);

	panelC.Spawn(NORMAL, 3, 1, false);
	panelC.Init(&oserroModel);
	othellos.push_back(panelC);
	
	panelD.Spawn(NORMAL, 3, 2, true);
	panelD.Init(&oserroModel);
	othellos.push_back(panelD);
}

const vector<vector<SendOthelloData>> &OthelloManager::Send()
{
	if (Input::KeyTrigger(DIK_1))
	{
		int hoge = 0;
	}
	SendOthelloData empty;
	//������
	empty.isFront = false;
	empty.isMove = false;
	empty.type = NONE;
	for (int i = 0; i < 8; i++)
	{
		empty.FrontActiveAngle.push_back(false);
	}

	//���S�̂���̋�Ŗ��߂�
	for (int i = 0; i < fieldSize; i++)
	{
		for (int j = 0; j < fieldSize; j++)
		{
			sendDatas[i][j] = empty;
		}
	}

	//�����݂�����˂�����ł���
	auto itr = othellos.begin();
	for(;itr != othellos.end();itr++)
	{
		OthelloData gameDatas = *itr->GetGameData();
		SendOthelloData data;

		data.FrontActiveAngle = gameDatas.FrontActiveAngle;
		data.isFront = gameDatas.isFront;
		data.type = gameDatas.type;

		//��Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ�����Ŏ���
		data.isMove = gameDatas.isPlayer;
		sendDatas[gameDatas.heightPos][gameDatas.widthPos] = data;
	}

	return sendDatas;
}