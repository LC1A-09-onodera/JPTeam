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
	const int CHAIN_NAME = 100;
	const float OTHELLO_BONUS = 1.2f;

private:
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<vector<SendOthelloData>> othelloDatas;
	std::vector<std::pair<int, int>> startAndEndArray;		//始点と終点を保存する配列
	std::vector<bool> othelloSide;
	std::pair<int, int> pPos;
	std::vector<pair<int, int>> checkScoreData;				//同時消しを考慮
	std::vector<pair<int, int>> sandwichData;
	std::vector<bool> sandwichSide;
	std::vector<pair<int, int>> reachData;
	std::vector<bool> reachSide;

private:
	int last_x;				//最後に動かしたオセロのX座標
	int last_y;				//最後に動かしたオセロのY座標
	int totalScore;			//合計点
	int totalDeleteOthello;	//挟んだ個数の合計
	int collectiveCount;	//まとまりの数
	int addScore;			//加算されるスコア
	bool side;				//表裏保存
	bool checkOthello;

	bool isCombos;
	bool isCombosCheck;

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

	int GetAddScore() { return addScore; }
	const vector<pair<int, int>>& GetReachDatas() { return reachData; }
	const vector<bool>& GetReachSides() { return reachSide; }

private:
	//盤面チェック
	void CheckLastMove(const vector<vector<SendOthelloData>>& othelloData);
	//自機の設定
	pair<int, int> SetCheckOthello();
	//8回呼ぶ
	void OthelloCheck(int direction_x, int direction_y, int last_x, int last_y, bool isCheck);
	//個数の倍率も計算
	void AddScore();
	//複数挟んだ時のスコアとコンボカウントを変更
	void ChangeScoreAndCombo();
	//リーチの目を検索
	void CheckReachOthello(int direction_x, int direction_y);

private:
	//Vectorの中身を判定
	bool VectorFinder(std::vector<int> vec, int number) {
		auto itr = std::find(vec.begin(), vec.end(), number);
		size_t index = std::distance(vec.begin(), itr);
		if (index != vec.size()) { return true; }
		else { return false; }
	}

	void ResetAddScore() { addScore = 0; }
};
