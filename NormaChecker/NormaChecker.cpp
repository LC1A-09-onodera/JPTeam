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

	//歩数の制限があるなら
	if (normaMoveCount > 0)
	{
		//失敗確認　(歩数を超過している + 条件をクリアしていない)
		isFailed = (normaMoveCount < nowMoveCount && !isClear);

		if (isFailed)
		{//確認用
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
		//コンボ数の最大値をとる
		int tmpComboCount = itr->GetGameData()->comboCount;
		if (nowComboCount <= tmpComboCount)
		{
			nowComboCount = tmpComboCount;
		}

		//生きてるパネルを移動後パネルに追加する
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
	//現在生きているパネルの数を確認する
	nowPanelCount = alivePanels.size();

	//受け取った情報から構造体を作って
	FieldStatus data;
	data.othellos = alivePanels;
	data.playerPos = { x, y };
	//リストに追加
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
		//コンボ数の最大値をとる
		int tmpComboCount = itr->GetGameData()->comboCount;
		if (nowComboCount <= tmpComboCount)
		{
			nowComboCount = tmpComboCount;
		}

		//生きてるパネルを移動後パネルに追加する
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
	//現在生きているパネルの数を確認する
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
	//移動した情報を全て消去
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
	//盤面データをローカル情報に移す
	FieldStatus data;
	//巻き戻す先のデータがなかった場合
	if (preField.size() <= 0)
	{
	//先頭であるという情報をtrueにする
		data.isFirst = true;
	}
	else
	{
		data = *preField.begin();
		//先頭の盤面データを削除
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
