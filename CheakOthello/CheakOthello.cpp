#include "CheakOthello.h"
#include <string>
#include <iterator>
#include "../BaseDirectX/Input.h"
#include "../imgui/ImguiControl.h"

CheakOthello::CheakOthello()
{
	//lastX = 0;
	//lastY = 0;
	//cheakSize = 0;
	//combo = 0;
}

CheakOthello::~CheakOthello()
{
}

void CheakOthello::Init()
{
	//combo = 0;
}

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData)
{
	//�X�R�A���Z�b�g
	if (Input::KeyTrigger(DIK_R)) { totalScore = 0; }

	//�Ō�ɓ�������𔻒�A�ۑ�
	CheckLastMove(othelloData);

	while (1)
	{
		//�����������Z�b�g
		bool IsSet = SetCheckOthello();
		if (!IsSet) { break; }

		//�S���ʃ`�F�b�N
		/*-----��-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION);
		/*-----�E-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH);

		/*-----���΂ߏ�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH);
		/*-----���΂߉�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH);
		/*-----�E�΂ߏ�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH);
		/*-----�E�΂߉�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH);

		//baseScore = 0;
	}
}

void CheakOthello::CheckLastMove(const vector<vector<SendOthelloData>>& othelloData)
{
	//�ŐV��Ԃ����
	othelloDatas = othelloData;

	//���̏�ɃI�Z�������݂��邩�A�Ō�ɓ������Ă����𔻒�i��������ƌ����������j
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if (!othelloDatas[i][j].isSandwich)
			{
				//���̏ꏊ����
				if (othelloDatas[i][j].type == NONE) { continue; }
				//�Ō�ɓ������Ă���
				if (!othelloDatas[i][j].isMove) { continue; }
			}
			othelloSide.push_back(othelloDatas[i][j].isFront);
			comboOthelloDataPos.push_back(std::make_pair(i, j));
		}
	}
}

bool CheakOthello::SetCheckOthello()
{
	if (!comboOthelloDataPos.empty())
	{
		last_y = comboOthelloDataPos.front().first;
		last_x = comboOthelloDataPos.front().second;
		comboOthelloDataPos.erase(comboOthelloDataPos.begin());

		side = othelloSide.front();
		othelloSide.erase(othelloSide.begin());
		return true;
	}
	else { return false; }
}

void CheakOthello::OthelloCheck(int direction_x, int direction_y)
{
	int loop = 0;
	int count_x = last_x;
	int count_y = last_y;
	int pair_x = last_x;	//loop��
	int pair_y = last_y;	//loop��

	int max_size;
	if (MAX_SIZE_X > MAX_SIZE_Y) { max_size = MAX_SIZE_X; }
	else { max_size = MAX_SIZE_Y; }

	while (1)
	{
		//���̃}�X
		count_x += direction_x;
		count_y += direction_y;

		//�͈͊O
		if (count_x < 0 || count_x > OthelloConstData::fieldSize - 1) { break; }
		if (count_y < 0 || count_y > OthelloConstData::fieldSize - 1) { break; }

		//���݂��Ȃ�
		if (othelloDatas[count_y][count_x].type == NONE) { break; }
		//���F�Ɨא�
		else if (othelloDatas[count_y][count_x].isFront == side && loop == 0) { break; }
		//���񂾏ꍇ
		else if (othelloDatas[count_y][count_x].isFront == side && loop != 0)
		{
			//�S���T�����X�g�ɓ����i�����Ƌ��񂾋�܂Łj��sandwichArray�����������Q�Ƃ���
			//���񂾂�̍ő��conboCount���擾������+1�����̂��R���{�����I�Z���ɃZ�b�g
			int maxComboCount = 0;
			int baseScore = 0;

			for (int i = 0; i <= loop + 1; i++)
			{
				othelloDatas[pair_y][pair_x].isSandwich = true;
				if (i == 0 || i == loop + 1) { startAndEndArray.push_back(std::make_pair(last_y, last_x)); }

				//���̃I�Z���̃R���{�����擾�A���̃I�Z�������R���{�ڂ��𒲂ׂ�
				if (maxComboCount = othelloDatas[pair_y][pair_x].comboCount)
				{
					maxComboCount = othelloDatas[pair_y][pair_x].comboCount;
				}

				//���̃I�Z���̃X�R�A���擾�A���̃I�Z���̃X�R�A�𒲂ׂ�
				if (baseScore < othelloDatas[pair_y][pair_x].score)
				{
					baseScore += othelloDatas[pair_y][pair_x].score;
				}

				//���ŋ��񂾂���ۑ��A�Ⴄ���ŋ��񂾏ꍇpush_back
				if (!othelloDatas[pair_y][pair_x].SandwichLength.empty())
				{
					//������������Ȃ�������ۑ�
					if (!VectorFinder(othelloDatas[pair_y][pair_x].SandwichLength, loop + 2))
					{
						othelloDatas[pair_y][pair_x].SandwichLength.push_back(loop + 2);
					}
				}
				//�󂾂����炻�̂܂ܕۑ�
				else { othelloDatas[pair_y][pair_x].SandwichLength.push_back(loop + 2); }

				pair_x += direction_x;
				pair_y += direction_y;
			}

			//������
			pair_x = last_x;
			pair_y = last_y;

			//maxComboCount�����Z
			maxComboCount++;

			//comboCount���Z�b�g
			for (int i = 0; i <= loop + 1; i++)
			{
				othelloDatas[pair_y][pair_x].comboCount = maxComboCount;
				othelloDatas[pair_y][pair_x].score += (baseScore + (loop * (loop + 2) * maxComboCount));
				if (i == 0)
				{
					totalScore += othelloDatas[pair_y][pair_x].score * maxComboCount;
					//baseScore += othelloDatas[pair_y][pair_x].score;
				}
				pair_x += direction_x;
				pair_y += direction_y;
			}

			Imgui::score = totalScore;

			//������
			pair_x = last_x;
			pair_y = last_y;

			//�Ђ�����Ԃ�
			for (int i = 1; i <= loop; i++)
			{
				pair_x += direction_x;
				pair_y += direction_y;
				othelloDatas[pair_y][pair_x].isFront = side;
				//othelloDatas[pair_y][pair_x].comboCount++;
			}

			break;
		}
		//���񂵂���i�����ŉ����񂾂�����ł���j
		loop++;
	}
}
