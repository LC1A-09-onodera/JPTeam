#pragma once
#include <vector>
#include "../Oserro/Oserro.h"
#include "../Sound/Sound.h"

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
	static SoundData comboSound;

private:
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<vector<SendOthelloData>> othelloDatas;
	std::vector<std::pair<int, int>> startAndEndArray;		//始点と終点を保存する配列
	std::vector<bool> othelloSide;
	std::pair<int, int> pPos;

private:
	int last_x;				//最後に動かしたオセロのX座標
	int last_y;				//最後に動かしたオセロのY座標
	int totalScore;			//合計点
	int totalDeleteOthello;	//挟んだ個数の合計
	bool side;				//表裏保存
	bool checkOthello;

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void Update(const vector<vector<SendOthelloData>>& othelloData, bool isCheck);

	const vector<vector<SendOthelloData>>& GetOthelloDatas() { return othelloDatas; }
	const vector<pair<int, int>>& GetStartAndEndArrayDatas() { return startAndEndArray; }
	//const vector<pair<int, int>>& GetStartArrayDatas() { return startArray; }
	//const vector<pair<int, int>>& GetEndArrayDatas() { return endArray; }
	const void ResetStartAndEndArrayDatas() { startAndEndArray.clear(); }
	const int GetScore() { return totalScore; }
	void SetScore(int score) { totalScore = score; }
private:
	//盤面チェック
	void CheckLastMove(const vector<vector<SendOthelloData>>& othelloData);
	//自機の設定
	pair<int, int> SetCheckOthello();
	//8回呼ぶ
	void OthelloCheck(int direction_x, int direction_y, int last_x, int last_y, bool isCheck);
	//個数の倍率も計算
	void AddScore();

private:
	//Vectorの中身を判定
	bool VectorFinder(std::vector<int> vec, int number) {
		auto itr = std::find(vec.begin(), vec.end(), number);
		size_t index = std::distance(vec.begin(), itr);
		if (index != vec.size()) { return true; }
		else { return false; }
	}
	//bool CheckOthelloEnd
};
