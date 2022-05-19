#pragma once
#include <list>
#include <string>
#include "../Sprite/Sprite.h"
class Othello;

namespace Norma
{
	enum NormaType
	{
		Combo,
		Score,
		ComboScore,
		Panels,
	};

	struct PlayerPos
	{
		int x;
		int y;
	};
	struct TmpOthelloData
	{
		int x = 0;
		int y = 0;
		bool isFront = false;
		int type = 0;
	};
	struct FieldStatus
	{
		bool isFirst = false;//Ç±ÇÍà»è„ñﬂÇÍÇ»Ç¢ÉtÉâÉO
		std::list<TmpOthelloData> othellos;
		PlayerPos playerPos {};
	};
}

class NormaChecker
{
public:
	NormaChecker();
	~NormaChecker();
	void Init();
	void Update(std::list<Othello> panels, int score, int combo);
	void Draw();
	void Finalize();
public:
	void SetMove(std::list<Othello> panels, int x, int y);
	void PanelUpdate(std::list<Othello> panels);
	void Reset();
	void SetNorma(Norma::NormaType type, int normaStatus,int normaMoveCount = 0);
	Norma::FieldStatus Undo();

	bool GetClear();
	bool GetFailed();

private:
	std::list<Norma::FieldStatus> preField;
	Norma::NormaType nowType = Norma::Combo;
	int normaComboCount = 0;
	int nowComboCount = 0;
	int normaPanelCount = 0;
	int nowPanelCount = 0;
	int normaScore = 0;
	int nowScore = 0;
	int normaComboScore = 0;
	int nowComboScore = 0;
	int nowMoveCount = 0;
	int normaMoveCount = 0;
	bool isClear = false;
	bool isFailed = false;
	Sprite clearText;
};

