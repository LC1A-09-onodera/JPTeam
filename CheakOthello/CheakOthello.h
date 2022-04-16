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


private:
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<vector<SendOthelloData>> othelloDatas;
	std::vector<std::pair<int, int>> startAndEndArray;		//�n�_�ƏI�_��ۑ�����z��
	std::vector<bool> othelloSide;

private:
	int last_x;				//�Ō�ɓ��������I�Z����X���W
	int last_y;				//�Ō�ɓ��������I�Z����Y���W
	//int baseScore;		//��b�_
	int totalScore;			//���v�_
	//int comboCount;		//���݂̃R���{�������̂܂ܔ{���Ɏg�p
	bool side;				//�\���ۑ�

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void Update(const vector<vector<SendOthelloData>>& othelloData);

	const vector<vector<SendOthelloData>>& GetOthelloDatas() { return othelloDatas; }
	const vector<pair<int, int>>& GetStartAndEndArrayDatas() { return startAndEndArray; } //���o�J
	const void ResetStartAndEndArrayDatas() { startAndEndArray.clear(); }

private:
	//�Ֆʃ`�F�b�N
	void CheckLastMove(const vector<vector<SendOthelloData>>& othelloData);
	//���@�̐ݒ�
	bool SetCheckOthello();
	//8��Ă�
	void OthelloCheck(int direction_x, int direction_y);

private:
	//Vector�̒��g�𔻒�
	bool VectorFinder(std::vector<int> vec, int number) {
		auto itr = std::find(vec.begin(), vec.end(), number);
		size_t index = std::distance(vec.begin(), itr);
		if (index != vec.size()) { return true; }
		else { return false; }
	}
};
