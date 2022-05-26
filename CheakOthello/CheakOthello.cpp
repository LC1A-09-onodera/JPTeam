#include "CheakOthello.h"
#include <string>
#include <iterator>
#include "../BaseDirectX/Input.h"
#include "../imgui/ImguiControl.h"

//SoundData CheakOthello::comboSound[9];

CheakOthello::CheakOthello()
{
	last_x = 0;				//�Ō�ɓ��������I�Z����X���W
	last_y = 0;				//�Ō�ɓ��������I�Z����Y���W
	totalScore = 0;			//���v�_
	totalDeleteOthello = 0;	//���񂾌��̍��v
	collectiveCount = 0;	//�܂Ƃ܂�̐�
	addScore = 0;			//���Z�����X�R�A
	combo = 0;				//�R���{
	totalReverceCount = 0;	//���������v��
	maxCombo = 0;
	side = 0;				//�\���ۑ�
	checkOthello = 0;		//
	isAddScore = 0;			//

	isCombos = false;		//
	isCombosCheck = false;	//

	isSand = false;
}

CheakOthello::~CheakOthello()
{
}

void CheakOthello::Init()
{
	//SoundLoad("Resource/Sound/reverse_4_.wav", comboSound);

	last_x = 0;				//�Ō�ɓ��������I�Z����X���W
	last_y = 0;				//�Ō�ɓ��������I�Z����Y���W
	totalScore = 0;			//���v�_
	totalDeleteOthello = 0;	//���񂾌��̍��v
	collectiveCount = 0;	//�܂Ƃ܂�̐�
	addScore = 0;			//���Z�����X�R�A
	combo = 0;				//�R���{
	//synchroCount = 0;
	totalReverceCount = 0;
	maxCombo = 0;
	side = 0;				//�\���ۑ�
	checkOthello = 0;		//
	isAddScore = 0;			//

	isCombos = false;		//
	isCombosCheck = false;	//
}

void CheakOthello::SoundInit()
{
	SoundLoad("Resource/Sound/reverse_0_.wav", comboSound[0]);
	SoundLoad("Resource/Sound/reverse_1_.wav", comboSound[1]);
	SoundLoad("Resource/Sound/reverse_2_.wav", comboSound[2]);
	SoundLoad("Resource/Sound/reverse_3_.wav", comboSound[3]);
	SoundLoad("Resource/Sound/reverse_4_.wav", comboSound[4]);
	SoundLoad("Resource/Sound/reverse_5_.wav", comboSound[5]);
	SoundLoad("Resource/Sound/reverse_6_.wav", comboSound[6]);
	SoundLoad("Resource/Sound/reverse_7_.wav", comboSound[7]);
	SoundLoad("Resource/Sound/reverse_8_.wav", comboSound[8]);
	SoundLoad("Resource/Sound/reverse_9_.wav", comboSound[9]);
	SoundLoad("Resource/Sound/reverse_10_.wav", comboSound[10]);
	SoundLoad("Resource/Sound/reverse_11_.wav", comboSound[11]);
	SoundLoad("Resource/Sound/reverse_12_.wav", comboSound[12]);
	SoundLoad("Resource/Sound/reverse_13_.wav", comboSound[13]);
	SoundLoad("Resource/Sound/reverse_14_.wav", comboSound[14]);
	//SoundLoad("Resource/Sound/reverse_4_.wav", comboSound);
	SoundLoad("Resource/Sound/miss.wav", missSound);
}

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData, bool isCheck)
{
	//�X�R�A���Z�b�g(debug�p)
	//if (Input::KeyTrigger(DIK_R)) { totalScore = 0; }

	//�Ō�ɓ�������𔻒�A�ۑ�
	CheckLastMove(othelloData);

	while (1) //����while����Ȃ��Ă�����
	{
		//�����������Z�b�g
		pair<int, int> last = GetCheckOthello();
		if (!checkOthello) { break; }

		totalDeleteOthello = 0;
		isAddScore = false;
		isSand = false;

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

		if (totalDeleteOthello > 1) { AddScore(); }
		if (isCombosCheck) { ChangeScoreAndCombo(); }

		if (!isSand)
		{
			SoundStopWave(missSound);
			SoundPlayOnce(missSound);
		}

		//ResetAddScore();
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

	if (!reachData.empty())
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				reachCheck[i][j] = false;
			}
		}

		for (auto itr = reachData.begin(); itr != reachData.end(); ++itr)
		{
			int lastX = itr->first;
			int lastY = itr->second;

			CheckReachOthello(Direction_X::WEST, NONE_DIRECTION, lastX, lastY);
			CheckReachOthello(Direction_X::EAST, NONE_DIRECTION, lastX, lastY);
			CheckReachOthello(NONE_DIRECTION, Direction_Y::SOUTH, lastX, lastY);
			CheckReachOthello(NONE_DIRECTION, Direction_Y::NOUTH, lastX, lastY);
			CheckReachOthello(Direction_X::WEST, Direction_Y::NOUTH, lastX, lastY);
			CheckReachOthello(Direction_X::WEST, Direction_Y::SOUTH, lastX, lastY);
			CheckReachOthello(Direction_X::EAST, Direction_Y::NOUTH, lastX, lastY);
			CheckReachOthello(Direction_X::EAST, Direction_Y::SOUTH, lastX, lastY);
		}
		reachData.clear();
	}
}

void CheakOthello::CheckLastMove(const vector<vector<SendOthelloData>>& othelloData)
{
	//�ŐV��Ԃ����
	othelloDatas = othelloData;

	//ChainName�ۑ��p�z��
	vector<int> chainName;

	nameAndCombos.clear();

	reachData.clear();
	reachPos.clear();
	completePos.clear();

	//���̏�ɃI�Z�������݂��邩�A�Ō�ɓ������Ă����𔻒�i��������ƌ����������j
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			//ChainName�ۑ��p�ϐ�
			int chainNameCount = 0;

			if (othelloDatas[i][j].isSandwich)
			{
				sandwichData.push_back(make_pair(i, j));
				sandwichSide.push_back(othelloDatas[i][j].isFront);
			}

			if (othelloDatas[i][j].isOnPlayer) { pPos = make_pair(j, i); }

			//���̏ꏊ����
			if (othelloDatas[i][j].type == NONE) { continue; }

			//���[�`�����p
			else if (othelloDatas[i][j].type == NORMAL || othelloDatas[i][j].type == WALL)
			{
				reachData.push_back(make_pair(j, i));
			}

			//ChainName�`�F�b�N
			if (othelloDatas[i][j].chainName != 0)
			{
				if (chainName.empty())
				{
					chainName.push_back(othelloDatas[i][j].chainName);
					chainNameCount = othelloDatas[i][j].chainName;

					nameAndCombos.push_back(make_pair(
						othelloDatas[i][j].chainName,
						othelloDatas[i][j].maxComboCount
					));
				}
				else
				{
					if (!VectorFinder(chainName, othelloDatas[i][j].chainName))
					{
						chainName.push_back(othelloDatas[i][j].chainName);

						nameAndCombos.push_back(make_pair(
							othelloDatas[i][j].chainName,
							othelloDatas[i][j].maxComboCount
						));
					}
				}
			}

			//�Ō�ɓ������Ă���
			if (!othelloDatas[i][j].isMove) { continue; }

			othelloSide.push_back(othelloDatas[i][j].isFront);
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			//break;
		}
	}

	//�Z�܂�̃J�E���g���v�Z
	collectiveCount = chainName.size();

	//if (!chainNames.empty()) { chainNames.clear(); }
	//copy(chainName.begin(), chainName.end(), back_inserter(chainNames));
}

pair<int, int> CheakOthello::GetCheckOthello()
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
			//totalDeleteOthello = 0;

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
				int comboCount = 0;
				int baseScore = 0;
				int maxChainNameCount = 0;
				int isSandWichCount = 0;
				vector<int> maxChainName;
				bool isActiveOthello = false;

				//�܂����܂�ĂȂ��I�Z������������R���{�����Z�ł����ԂɕύX
				for (int i = 1; i <= loop; i++)
				{
					pair_x += direction_x;
					pair_y += direction_y;

					//�R���{�����Z�ł����ԂɕύX
					if (!othelloDatas[pair_y][pair_x].isSandwich) {
						isActiveOthello = true;
					}
				}

				//������
				pair_x = last_x;
				pair_y = last_y;

				//�Ђ�����Ԃ���ꍇ
				if (isActiveOthello)
				{
					if (isCombos) { isCombosCheck = true; }
					isCombos = true;
					totalDeleteOthello += loop;

					for (int i = 0; i <= loop + 1; i++)
					{
						//���̃I�Z���̃R���{�����擾�A���̃I�Z�������R���{�ڂ��𒲂ׂ�
						if (maxComboCount < othelloDatas[pair_y][pair_x].maxComboCount)
						{
							maxComboCount = othelloDatas[pair_y][pair_x].maxComboCount;
						}

						//���̃I�Z���̃R���{�����擾�A���̃I�Z�������R���{�ڂ��𒲂ׂ�
						if (comboCount <= othelloDatas[pair_y][pair_x].comboCount)
						{
							comboCount = othelloDatas[pair_y][pair_x].comboCount;
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

						if (othelloDatas[pair_y][pair_x].isSandwich) { isSandWichCount++; }

						pair_x += direction_x;
						pair_y += direction_y;
					}

					totalReverceCount += loop + 2 - isSandWichCount;

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

					//���߂���true
					isSand = true;

					//�X�R�A���Z���ꂽ�t���O
					isAddScore = true;

					if (maxComboCount < 15)
					{
						SoundStopWave(comboSound[maxComboCount]);
						SoundPlayOnce(comboSound[maxComboCount]);
					}
					else
					{
						SoundStopWave(comboSound[14]);
						SoundPlayOnce(comboSound[14]);
					}


					//maxComboCount�����Z
					maxComboCount++;
					comboCount++;

					//�ő�R���{���𔻒�
					if (maxCombo < maxComboCount) { maxCombo = maxComboCount; }

					//�g�p���闐�����`
					int random = std::rand();
					int maxScore = 0;

					//Score�v�Z
					for (int i = 0; i <= loop + 1; i++)
					{
						//���̃I�Z���̓Z�܂�𔻕�
						if (othelloDatas[pair_y][pair_x].chainName != 0)
						{
							if (!VectorFinder(maxChainName, othelloDatas[pair_y][pair_x].chainName))
							{
								maxChainName.push_back(othelloDatas[pair_y][pair_x].chainName);
								maxChainNameCount = othelloDatas[pair_y][pair_x].chainName;
							}
						}
						else
						{
							othelloDatas[pair_y][pair_x].chainName = random;
						}

						//�n�_�ƏI�_��ݒ�
						if (i == 0 || i == loop + 1) { startAndEndArray.push_back(std::make_pair(pair_y, pair_x)); }
						checkScoreData.push_back(make_pair(pair_y, pair_x));
						othelloDatas[pair_y][pair_x].comboCount = comboCount;
						othelloDatas[pair_y][pair_x].maxComboCount = maxComboCount;
						if (maxComboCount <= 1)
						{
							othelloDatas[pair_y][pair_x].score = (baseScore + (loop * (loop + 2) * 10));
						}
						else if (maxComboCount <= 10)
						{
							othelloDatas[pair_y][pair_x].score = (baseScore + (loop * (loop + 2) * 10 * powf(1.1f, maxComboCount)));
						}
						else
						{
							othelloDatas[pair_y][pair_x].score = (baseScore + (loop * (loop + 2) * 10 * powf(1.1f, 10)));
						}
						if (maxScore < othelloDatas[pair_y][pair_x].score)
						{
							maxScore = othelloDatas[pair_y][pair_x].score;
						}
						if (i == 0)
						{
							if (addScore < othelloDatas[pair_y][pair_x].score)
							{
								addScore = othelloDatas[pair_y][pair_x].score;
							}
							totalScore += othelloDatas[pair_y][pair_x].score * maxComboCount;
							//baseScore += othelloDatas[pair_y][pair_x].score;
						}
						pair_x += direction_x;
						pair_y += direction_y;
					}

					//�S���󂾂����ꍇ�i1�R���{�ځj
					//if (maxChainName.empty())
					//{
					//	//������
					//	pair_x = last_x;
					//	pair_y = last_y;

					//	for (int i = 0; i <= loop + 1; i++)
					//	{
					//		othelloDatas[pair_y][pair_x].chainName = random;
					//		pair_x += direction_x;
					//		pair_y += direction_y;
					//	}
					//}

					//�R���{���q�����ꍇ
					if (maxChainName.size() == 1)
					{
						//int chainName = maxChainName.front();
						int chainName = maxChainName.front();

						for (int i = 0; i < OthelloConstData::fieldSize; i++)
						{
							for (int j = 0; j < OthelloConstData::fieldSize; j++)
							{
								if (othelloDatas[i][j].chainName != random) { continue; }
								//othelloDatas[i][j].chainName = random;
								othelloDatas[i][j].chainName = chainName;
							}
						}
					}

					//�������񂾏ꍇ
					else if (maxChainName.size() > 1)
					{
						//�R���{���q�������ꍇ
						for (auto itr = maxChainName.begin(); itr != maxChainName.end(); ++itr)
						{
							int chainName = *itr;

							for (int i = 0; i < OthelloConstData::fieldSize; i++)
							{
								for (int j = 0; j < OthelloConstData::fieldSize; j++)
								{
									if (othelloDatas[i][j].chainName != chainName) { continue; }
									//othelloDatas[i][j].chainName = random;
									othelloDatas[i][j].chainName = random;
								}
							}
						}
					}

					//�S�������p
					std::vector<int> chainNames;
					for (int i = 0; i < OthelloConstData::fieldSize; i++)
					{
						for (int j = 0; j < OthelloConstData::fieldSize; j++)
						{
							if (othelloDatas[i][j].chainName != 0)
							{
								if (chainNames.empty())
								{
									chainNames.push_back(othelloDatas[i][j].chainName);
								}
								else
								{
									if (!VectorFinder(chainNames, othelloDatas[i][j].chainName))
									{
										chainNames.push_back(othelloDatas[i][j].chainName);
									}
								}
							}
						}
					}

					//ComboCount��Score�C��
					if (!chainNames.empty())
					{
						for (auto itr = chainNames.begin(); itr != chainNames.end(); ++itr)
						{
							int chainName = *itr;

							for (int i = 0; i < OthelloConstData::fieldSize; i++)
							{
								for (int j = 0; j < OthelloConstData::fieldSize; j++)
								{
									if (othelloDatas[i][j].chainName == chainName)
									{
										if (othelloDatas[i][j].maxComboCount < maxComboCount)
										{
											othelloDatas[i][j].maxComboCount = maxComboCount;
										}
										if (othelloDatas[i][j].score < maxScore)
										{
											othelloDatas[i][j].score = maxScore;
										}
									}
								}
							}

							//������
							pair_x = last_x;
							pair_y = last_y;

							//���̏�̃R���{�����擾
							combo = othelloDatas[pair_y][pair_x].maxComboCount;

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

					for (int i = 0; i < OthelloConstData::fieldSize; i++)
					{
						for (int j = 0; j < OthelloConstData::fieldSize; j++)
						{
							//ChainName�`�F�b�N
							if (othelloDatas[i][j].chainName != 0)
							{
								if (nameAndCombos.empty())
								{
									nameAndCombos.push_back(make_pair(
										othelloDatas[i][j].chainName,
										othelloDatas[i][j].maxComboCount
									));
								}
								else
								{
									if (!VectorFinder(chainNames, othelloDatas[i][j].chainName))
									{
										nameAndCombos.push_back(make_pair(
											othelloDatas[i][j].chainName,
											othelloDatas[i][j].maxComboCount
										));
									}
								}
							}
						}
					}

					//���񂵂���i�����ŉ����񂾂�����ł���j
					//loop++;
				}

				break;
			}

			//���񂵂���i�����ŉ����񂾂�����ł���j
			loop++;
			//synchroCount++;
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
			for (int i = 0; i < loop; i++)
			{
				count_x += direction_x;
				count_y += direction_y;
				if (!othelloDatas[count_y][count_x].isSandwich) {
					isActiveOthello = true;
				}
				//if (i == loop) { isActiveOthello = true; }
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

void CheakOthello::AddScore()
{
	totalScore = (int)(totalScore * powf(OTHELLO_BONUS, totalDeleteOthello - 1));
	totalDeleteOthello = 0;
}

void CheakOthello::ChangeScoreAndCombo()
{
	isCombos = false;
	isCombosCheck = false;

	//x��front,y��second
	int maxScore = 0;
	int maxConboCount = 0;
	//int maxChainName = 0;
	bool isChangeScore = false;
	//bool isChangeChainName = false;

	for (auto itr = checkScoreData.begin(); itr != checkScoreData.end(); ++itr)
	{
		if (othelloDatas[itr->first][itr->second].score > maxScore)
		{
			maxScore = othelloDatas[itr->first][itr->second].score;
			maxConboCount = othelloDatas[itr->first][itr->second].comboCount;
			isChangeScore = true;
		}
		/*if (othelloDatas[itr->first][itr->second].chainName > maxChainName)
		{
			maxChainName = othelloDatas[itr->first][itr->second].chainName;
			isChangeChainName = true;
		}*/
	}

	for (auto itr = checkScoreData.begin(); itr != checkScoreData.end(); ++itr)
	{
		if (isChangeScore)
		{
			othelloDatas[itr->first][itr->second].score = maxScore;
			othelloDatas[itr->first][itr->second].comboCount = maxConboCount;
		}
		/*if (isChangeChainName)
		{
			othelloDatas[itr->first][itr->second].chainName = maxChainName;
		}*/
	}

	checkScoreData.clear();
}

void CheakOthello::CheckReachOthello(int direction_x, int direction_y, int last_x, int last_y)
{
	int lastX = last_x;
	int lastY = last_y;
	bool side = othelloDatas[lastY][lastX].isFront;
	bool isReach = false;
	bool sandCheck = false;

	while (1)
	{
		lastX += direction_x;
		lastY += direction_y;

		if (lastX < 0 || lastX > OthelloConstData::fieldSize - 1) { break; }
		if (lastY < 0 || lastY > OthelloConstData::fieldSize - 1) { break; }

		if (!isReach)
		{
			if (!othelloDatas[lastY][lastX].isSandwich) { sandCheck = true; }
			if (othelloDatas[lastY][lastX].isFront == side) { break; }
			if (othelloDatas[lastY][lastX].type == NONE) { break; }
			else { isReach = true; }
		}

		else if (isReach && sandCheck)
		{
			if (othelloDatas[lastY][lastX].type != NONE)
			{
				if (othelloDatas[lastY][lastX].isFront != side) { continue; }
				else { completePos.push_back(make_pair(lastY, lastX)); break; }
			}
			else if (othelloDatas[lastY][lastX].type == NONE)
			{
				if (!reachCheck[lastY][lastX])
				{
					reachCheck[lastY][lastX] = true;
					reachPos.push_back(make_pair(lastY, lastX));
				}
				break;
			}
		}
	}
}
