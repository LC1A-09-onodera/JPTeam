#pragma once
#include <vector>
#include "../Oserro/Oserro.h"

class CheakOthello
{
private:
	enum Direction_X
	{
		EAST = 1,
		WEST = -1
	};

	enum Direction_Y
	{
		SOUTH = 1,
		NOUTH = -1,
	};

	int NONE_DIRECTION = 0;

private:
	static const int MAX_SIZE_X = 8;
	static const int MAX_SIZE_Y = 8;

private:
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<vector<SendOthelloData>> othelloDatas;

private:
	int last_x;
	int last_y;
	bool side;

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void Update(const vector<vector<SendOthelloData>>& othelloData);

	const vector<vector<SendOthelloData>>& GetOthelloDatas();

private:
	//盤面チェック
	void CheckLastMove(const vector<vector<SendOthelloData>>& othelloData);
	//自機の設定
	bool SetCheckOthello();
	//8回呼ぶ
	void OthelloCheck(int direction_x, int direction_y);

};
