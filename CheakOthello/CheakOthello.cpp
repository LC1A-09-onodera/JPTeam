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

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData, bool isCheck)
{
	//�X�R�A���Z�b�g
	if (Input::KeyTrigger(DIK_R)) { totalScore = 0; }

	//�Ō�ɓ�������𔻒�A�ۑ�
	CheckLastMove(othelloData);

	while (1)
	{
		//�����������Z�b�g
		pair<int, int> last = SetCheckOthello();
		if (!checkOthello) { break; }

		//�S���ʃ`�F�b�N
		/*-----��-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION, last.first, last.second, false);
		/*-----�E-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION, last.first, last.second, false);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH, last.first, last.second, false);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH, last.first, last.second, false);

		/*-----���΂ߏ�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH, last.first, last.second, false);
		/*-----���΂߉�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH, last.first, last.second, false);
		/*-----�E�΂ߏ�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH, last.first, last.second, false);
		/*-----�E�΂߉�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH, last.first, last.second, false);
	}

	//���������ł�
	if (isCheck)
	{
		//�S���ʃ`�F�b�N
		/*-----��-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION, pPos.first, pPos.second, isCheck);
		/*-----�E-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION, pPos.first, pPos.second, isCheck);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
		/*-----��-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);

		/*-----���΂ߏ�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);
		/*-----���΂߉�-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
		/*-----�E�΂ߏ�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);
		/*-----�E�΂߉�-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
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
			if (othelloDatas[i][j].isOnPlayer) { pPos = make_pair(j, i); }

			//���̏ꏊ����
			if (othelloDatas[i][j].type == NONE) { continue; }
			//�Ō�ɓ������Ă���
			if (!othelloDatas[i][j].isMove) { continue; }
			othelloSide.push_back(othelloDatas[i][j].isFront);
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}
}

pair<int, int> CheakOthello::SetCheckOthello()
{
	if (!comboOthelloDataPos.empty())
	{
		int x, y;
		y = comboOthelloDataPos.front().first;
		x = comboOthelloDataPos.front().second;
		comboOthelloDataPos.erase(comboOthelloDataPos.begin());

		side = othelloSide.front();
		othelloSide.erase(othelloSide.begin());
		checkOthello = true;
		return make_pair(x, y);
	}
	else { checkOthello = false; return make_pair(0, 0); }
}

void CheakOthello::OthelloCheck(int direction_x, int direction_y, int last_x, int last_y, bool isCheck)
{
	int loop = 0;
	int count_x = last_x;
	int count_y = last_y;
	int pair_x = last_x;	//loop��
	int pair_y = last_y;	//loop��

	int max_size;
	if (MAX_SIZE_X > MAX_SIZE_Y) { max_size = MAX_SIZE_X; }
	else { max_size = MAX_SIZE_Y; }

	int side = othelloDatas[count_y][count_x].isFront;

	while (1)
	{
		if (!isCheck)
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
				bool isActiveOthello = false;

				for (int i = 0; i <= loop + 1; i++)
				{
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

				//�܂����܂�ĂȂ��I�Z������������R���{�����Z�ł����ԂɕύX
				for (int i = 1; i <= loop; i++)
				{
					pair_x += direction_x;
					pair_y += direction_y;

					//�R���{�����Z�ł����ԂɕύX
					if (!othelloDatas[pair_y][pair_x].isSandwich) { isActiveOthello = true; }
				}

				//������
				pair_x = last_x;
				pair_y = last_y;

				//isSandwich���`�F�b�N
				for (int i = 0; i <= loop + 1; i++)
				{
					//���ށA���܂��I�Z�������񂾏�ԂɕύX
					othelloDatas[pair_y][pair_x].isSandwich = true;

					pair_x += direction_x;
					pair_y += direction_y;
				}

				//������
				pair_x = last_x;
				pair_y = last_y;

				//�Ђ�����Ԃ���ꍇ
				if (isActiveOthello)
				{
					//maxComboCount�����Z
					maxComboCount++;

					//Score�v�Z
					for (int i = 0; i <= loop + 1; i++)
					{
						//�n�_�ƏI�_��ݒ�
						if (i == 0 || i == loop + 1) { startAndEndArray.push_back(std::make_pair(pair_y, pair_x)); }
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

					//������
					pair_x = last_x;
					pair_y = last_y;

					Imgui::score = totalScore;
				}

				break;
			}
			//���񂵂���i�����ŉ����񂾂�����ł���j
			loop++;
		}

		else
		{
			//�J�E���g�p
			//int count = 1;

			//�}�X����
			//othelloCheckDatas[pair_y][pair_y] = true;
			//if (othelloCheckDatas[pair_y][pair_x]) { return; }

			//���̃}�X
			pair_x += direction_x;
			pair_y += direction_y;

			//�͈͊O
			if (pair_x < 0 || pair_x > OthelloConstData::fieldSize - 1) { break; }
			if (pair_y < 0 || pair_y > OthelloConstData::fieldSize - 1) { break; }

			//���݂��Ȃ�
			if (othelloDatas[pair_y][pair_x].type == NONE) { break; }
			//���݂��Ȃ�
			if (othelloDatas[count_y][count_x].type == NONE) { break; }
			//���F�Ɨא�
			if (othelloDatas[pair_y][pair_x].isFront == side && loop == 0) { break; }

			//�T���\
			bool isActiveOthello = false;
			for (int i = 0; i <= loop; i++)
			{
				count_x += direction_x;
				count_y += direction_y;
				if (othelloDatas[count_y][count_x].isSandwich) { break; }
				if (i == loop) { isActiveOthello = true; }
			}

			//������
			count_x = last_x;
			count_y = last_y;

			//���߂�I�Z��������������
			if (isActiveOthello)
			{
				if (othelloDatas[pair_y][pair_x].isFront == side && loop != 0)
				{
					for (int i = 0; i <= loop + 1; i++)
					{
						othelloDatas[count_y][count_x].isCheckEnd = true;
						count_x += direction_x;
						count_y += direction_y;
					}
					break;
				}
			}

			//���̃}�X
			//pair_x += direction_x;
			//pair_y += direction_y;

			loop++;
		}
	}
}
