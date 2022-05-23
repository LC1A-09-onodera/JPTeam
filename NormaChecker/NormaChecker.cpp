#include "NormaChecker.h"
#include "../Oserro/Oserro.h"
#include <algorithm>
using namespace Norma;
NormaChecker::NormaChecker()
{
}
NormaChecker::~NormaChecker()
{
}
void NormaChecker::Init()
{
	clearText.CreateSprite(L"Resource/Img/genius.png", XMFLOAT3(0, 0, 0));
	float changeScale = 0.5f;
	clearText.ChangeSize(318 * changeScale, 101 * changeScale);
	clearText.position = XMVECTOR{ 640 - (318 * changeScale / 2), 360 - (101 * changeScale), 0, 0 };
	Reset();
}

void NormaChecker::Update(std::list<Othello> panels, int score,int combo)
{
	PanelUpdate(panels);
	nowScore = score;
	switch (nowType)
	{
	case Norma::Combo:
		isClear = combo >= normaComboCount;
		break;
	case Norma::Score:
		isClear = nowScore >= normaScore;
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

	if (subNormaFlag)
	{
		bool SubNorma = nowPanelCount <= normaPanelCount;
		isClear = (isClear && SubNorma);
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
	if (isClear)
	{
		clearText.SpriteDraw();
	}
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
			PanelData.type = static_cast<int>(itr->GetGameData()->type);
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
		bool isPanelAlive = !itr->GetGameData()->isVanish && !(itr->GetGameData()->type == OthelloType::WALL);
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

void NormaChecker::SetNorma(Norma::NormaType type, int normaStatus, int normaMoveCount, bool subNorma, int subNormaCount)
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
	if (subNorma)
	{
		subNormaFlag = subNorma;
		normaPanelCount = subNormaCount;
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

int NormaChecker::GetStatus()
{
	switch (nowType)
	{
	case Norma::Combo:
	return nowComboCount;
	case Norma::Score:
		return nowScore;
	case Norma::ComboScore:
		//isClear = nowComboCount >= normaComboCount;
		break;
	case Norma::Panels:
	return nowPanelCount;
	default:
		break;
	}
}

int NormaChecker::GetLestStatus()
{
	switch (nowType)
	{
	case Norma::Combo:
		return normaComboCount - nowComboCount;
	case Norma::Score:
		return normaScore - nowScore;
	case Norma::ComboScore:
		//isClear = nowComboCount >= normaComboCount;
		break;
	case Norma::Panels:
		return nowPanelCount - normaPanelCount ;
	default:
		break;
	}
}


