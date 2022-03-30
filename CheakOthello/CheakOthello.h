#pragma once
#include <vector>

struct OthelloData
{
	bool side;
	//enum Status
	//bool ï\ó†
	//bool ç≈å„Ç…ìÆÇ¢ÇΩÇ©
	//bool Ç«Ç¡ÇøÇ©ÇÃêFÇäÓèÄÇ∆ÇµÇΩï˚å¸[8]
	//OthelloData othelloData[8][8]
};

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
	OthelloData othelloDatas[MAX_SIZE_Y][MAX_SIZE_X];

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void Update(const OthelloData(&othelloData)[MAX_SIZE_Y][MAX_SIZE_Y]);
	OthelloData* GetOthelloData() { return othelloDatas; }

private:

};
