#pragma once
#include <vector>
#include "../Oserro/Oserro.h"
//struct OthelloData
//{
//	bool side;
//	//enum Status
//	//bool 表裏
//	//bool 最後に動いたか
//	//bool どっちかの色を基準とした方向[8]
//	//OthelloData othelloData[8][8]
//};

class CheakOthello
{
private:
	enum Status
	{
		NORMAL,
		HALF
	};

	enum Direcion
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

private:
	static const int MAX_SIZE_X = 8;
	static const int MAX_SIZE_Y = 8;

private:
	int combo;
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<std::pair<int, int>> saveOthelloDataPos;
	vector<vector<SendOthelloData>> othelloDatas;

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void Update(const vector<vector<SendOthelloData>> &othelloData);

	const vector<vector<SendOthelloData>> &GetOthelloDatas();

private:

};
