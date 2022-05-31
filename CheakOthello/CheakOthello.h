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
	SoundData comboSound[15];
	SoundData missSound;

private:
	const int CHAIN_NAME = 100;
	const float OTHELLO_BONUS = 1.05f;

private:
	std::vector<std::pair<int, int>> comboOthelloDataPos;
	std::vector<vector<SendOthelloData>> othelloDatas;
	std::vector<std::pair<int, int>> startAndEndArray;		//�n�_�ƏI�_��ۑ�����z��
	std::vector<bool> othelloSide;
	std::pair<int, int> pPos;
	std::vector<pair<int, int>> checkScoreData;				//�����������l��
	std::vector<pair<int, int>> sandwichData;
	std::vector<bool> sandwichSide;
	std::vector<pair<int, int>> reachData;
	std::vector<bool> reachSide;
	std::vector<pair<int, int>> reachPos;
	std::vector<pair<int, int>> nameAndCombos;				//���O�ƃR���{��
	std::vector<int> chainNames;
	std::vector<pair<int, int>> completePos;

private:
	int last_x;				//�Ō�ɓ��������I�Z����X���W
	int last_y;				//�Ō�ɓ��������I�Z����Y���W
	int totalScore;			//���v�_
	int totalDeleteOthello;	//���񂾌��̍��v
	int collectiveCount;	//�܂Ƃ܂�̐�
	int addScore;			//���Z�����X�R�A
	int combo;
	//int synchroCount;		//�����������l������J�E���g
	int totalReverceCount;	//���������v��
	int maxCombo;
	bool side;				//�\���ۑ�
	bool checkOthello;
	bool isAddScore;

	bool isCombos;
	bool isCombosCheck;

	bool isSand;

	bool reachCheck[8][8] = { false };

	//bool isSideSame = false;

public:
	CheakOthello();
	~CheakOthello();
	void Init();
	void SoundInit();
	void Update(const vector<vector<SendOthelloData>>& othelloData, bool isCheck);

	const vector<vector<SendOthelloData>>& GetOthelloDatas() { return othelloDatas; }
	const vector<pair<int, int>>& GetStartAndEndArrayDatas() { return startAndEndArray; }
	//const vector<pair<int, int>>& GetStartArrayDatas() { return startArray; }
	//const vector<pair<int, int>>& GetEndArrayDatas() { return endArray; }
	const void ResetStartAndEndArrayDatas() { startAndEndArray.clear(); }
	const int GetScore() { return totalScore; }
	void SetScore(int score) { totalScore = score; }

	const int GetAddScore() { return addScore; }
	const vector<pair<int, int>>& GetReachDatas() { return reachPos; }	//���[�`�̏ꏊ
	//const vector<bool>& GetReachSides() { return reachSide; }			//���[�`�̏ꏊ�̕\��
	const int GetCombo() { return combo; }
	const bool IsAddScore() { return isAddScore; }
	const vector<pair<int, int>> GetNameAndCombos() { return nameAndCombos; }
	const bool IsSand() { return isSand; }
	const int GetMaxCombo() { return maxCombo; }
	const int GetTotalReverceCount() { return totalReverceCount; }
	const vector<pair<int, int>> GetCompletePos() { return completePos; }

private:
	//�Ֆʃ`�F�b�N
	void CheckLastMove(const vector<vector<SendOthelloData>>& othelloData);
	//���@�̐ݒ�
	pair<int, int> GetCheckOthello();
	//8��Ă�
	void OthelloCheck(int direction_x, int direction_y, int last_x, int last_y, bool isCheck);
	//���̔{�����v�Z
	void AddScore();
	//�������񂾎��̃X�R�A�ƃR���{�J�E���g��ύX
	void ChangeScoreAndCombo();
	//���[�`�̖ڂ�����
	void CheckReachOthello(int direction_x, int direction_y, int last_x, int last_y);

private:
	//Vector�̒��g�𔻒�
	bool VectorFinder(std::vector<int> vec, int number) {
		auto itr = std::find(vec.begin(), vec.end(), number);
		size_t index = std::distance(vec.begin(), itr);
		if (index != vec.size()) { return true; }
		else { return false; }
	}

	void ResetAddScore() { addScore = 0; }
};
