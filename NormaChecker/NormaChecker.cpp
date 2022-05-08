#include "NormaChecker.h"
#include "../Oserro/Oserro.h"

using namespace Norma;
NormaChecker::NormaChecker()
{
}
NormaChecker::~NormaChecker()
{
}
void NormaChecker::Init()
{
	Reset();
}

void NormaChecker::Update(std::list<Othello> panels)
{
	PanelUpdate(panels);
	switch (nowType)
	{
	case Norma::Combo:
		isClear = nowComboCount >= normaComboCount;
		break;
	case Norma::Score:
		//isClear = nowComboCount >= normaComboCount;
		break;
	case Norma::ComboScore:
		//isClear = nowComboCount >= normaComboCount;
		break;
	case Norma::Panels:
		isClear = nowPanelCount <= normaPanelCount;
		break;
	default:
		break;
	}

	//�����̐���������Ȃ�
	if (normaMoveCount > 0)
	{
		//���s�m�F�@(�����𒴉߂��Ă��� + �������N���A���Ă��Ȃ�)
		isFailed = (normaMoveCount < nowMoveCount && !isClear);

		if (isFailed)
		{//�m�F�p
			int hoge = 0;
		}
	}
}

void NormaChecker::Draw()
{
}

void NormaChecker::Finalize()
{
}

void NormaChecker::SetMove(std::list<Othello> panels, int x, int y)
{
	auto itr = panels.begin();
	std::list<TmpOthelloData> alivePanels;
	nowComboCount = 0;
	for (; itr != panels.end(); itr++)
	{
		//�R���{���̍ő�l���Ƃ�
		int tmpComboCount = itr->GetGameData()->comboCount;
		if (nowComboCount <= tmpComboCount)
		{
			nowComboCount = tmpComboCount;
		}

		//�����Ă�p�l�����ړ���p�l���ɒǉ�����
		bool isPanelAlive = !itr->GetGameData()->isVanish;
		if (isPanelAlive)
		{
			TmpOthelloData PanelData;
			PanelData.x = itr->GetGameData()->widthPos;
			PanelData.y = itr->GetGameData()->heightPos;
			PanelData.isFront = itr->GetGameData()->isFront;
			alivePanels.push_back(PanelData);
		}
	}
	//���ݐ����Ă���p�l���̐����m�F����
	nowPanelCount = alivePanels.size();

	//�󂯎������񂩂�\���̂������
	FieldStatus data;
	data.othellos = alivePanels;
	data.playerPos = { x, y };
	//���X�g�ɒǉ�
	preField.push_front(data);
	nowMoveCount = preField.size();
}

void NormaChecker::PanelUpdate(std::list<Othello> panels)
{
	auto itr = panels.begin();
	std::list<TmpOthelloData> alivePanels;
	nowComboCount = 0;
	for (; itr != panels.end(); itr++)
	{
		//�R���{���̍ő�l���Ƃ�
		int tmpComboCount = itr->GetGameData()->comboCount;
		if (nowComboCount <= tmpComboCount)
		{
			nowComboCount = tmpComboCount;
		}

		//�����Ă�p�l�����ړ���p�l���ɒǉ�����
		bool isPanelAlive = !itr->GetGameData()->isVanish;
		if (isPanelAlive)
		{
			TmpOthelloData PanelData;
			PanelData.x = itr->GetGameData()->widthPos;
			PanelData.y = itr->GetGameData()->heightPos;
			PanelData.isFront = itr->GetGameData()->isFront;
			alivePanels.push_back(PanelData);
		}
	}
	//���ݐ����Ă���p�l���̐����m�F����
	nowPanelCount = alivePanels.size();
}

void NormaChecker::Reset()
{
	isClear = false;
	isFailed = false;
	if (preField.size() <= 0)
	{
		return;
	}
	//�ړ���������S�ď���
	preField.clear();
}

void NormaChecker::SetNorma(Norma::NormaType type, int normaStatus, int normaMoveCount)
{
	nowType = type;
	switch (nowType)
	{
	case Norma::Combo:
		normaComboCount = normaStatus;
		break;
	case Norma::Score:
		normaScore = normaStatus;
		break;
	case Norma::ComboScore:
		normaComboScore = normaStatus;
		break;
	case Norma::Panels:
		normaPanelCount = normaStatus;
		break;
	default:
		break;
	}
	this->normaMoveCount = normaMoveCount;
}

Norma::FieldStatus NormaChecker::Undo()
{
	//�Ֆʃf�[�^�����[�J�����Ɉڂ�
	FieldStatus data;
	//�����߂���̃f�[�^���Ȃ������ꍇ
	if (preField.size() <= 0)
	{
	//�擪�ł���Ƃ�������true�ɂ���
		data.isFirst = true;
	}
	else
	{
		data = *preField.begin();
		//�擪�̔Ֆʃf�[�^���폜
		preField.erase(preField.begin());
	}
	return data;
}

bool NormaChecker::GetClear()
{
	return isClear;
}

bool NormaChecker::GetFailed()
{
	return isFailed;
}
