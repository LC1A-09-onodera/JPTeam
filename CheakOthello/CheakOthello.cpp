#include "CheakOthello.h"
#include <string>
#include <iterator>
#include "../BaseDirectX/Input.h"

CheakOthello::CheakOthello()
{
	//lastX = 0;
	//lastY = 0;
	//cheakSize = 0;
	combo = 0;
}

CheakOthello::~CheakOthello()
{
}

void CheakOthello::Init()
{
	combo = 0;
}

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData)
{
	int lastX, lastY;				//�Ō�̍��W��ۑ�����ϐ�
	bool side;					//�\������
	bool cheakRight = true;
	bool cheakLeft = true;
	bool cheakUp = true;
	bool cheakDown = true;
	bool cheakTopLeft = true;
	bool cheakBottomLeft = true;
	bool cheakTopRight = true;
	bool cheakBottomRight = true;


	if (Input::KeyTrigger(DIK_1))
	{
		int hoge = 0;
	}
	//memcpy(othelloDatas, othelloData, sizeof(othelloData));
	othelloDatas = othelloData;
	//���̏�ɃI�Z�������݂��邩�A�Ō�ɓ������Ă����𔻒�i��������ƌ����������j
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if (othelloDatas[i][j].type == NONE) { continue; }
			// �Ō�ɓ������Ă���
			if (!othelloDatas[i][j].isMove) { continue; }
			lastX = j;
			lastY = i;
			side = othelloDatas[i][j].isFront;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}

	//�Ō�̂���ɍ��E�Ǝ΂ߔ���(j��for���͈̔́Ai+1����K�v���邩??)
	while (!comboOthelloDataPos.empty() || !saveOthelloDataPos.empty())
	{
		/*-----��-----*/
		int maxLeft = lastX + 1;
		if (maxLeft > 2 && lastX >= 2)
		{
			for (int i = 1; i < maxLeft; i++)
			{

				//���m�F
				if (cheakLeft)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY][lastX - i].type == NONE)
					{
						cheakLeft = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY][lastX - i].isFront == side && i == 1)
					{
						cheakLeft = false;
					}
					//������������
					else if (othelloDatas[lastY][lastX - i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY][lastX - j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY, lastX - j));
							cheakLeft = false;
						}
					}
				}
			}
		}

		/*-----�E-----*/
		int maxRight = MAX_SIZE_X - lastX;
		if (maxRight > 2 && lastX <= MAX_SIZE_X - 3)
		{
			for (int i = 1; i < maxRight; i++)
			{
				//�E�m�F
				if (cheakRight)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY][lastX + i].type == NONE)
					{
						cheakRight = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY][lastX + i].isFront == side && i == 1)
					{
						cheakRight = false;
					}
					//������������
					else if (othelloDatas[lastY][lastX + i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY][lastX + j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY, lastX + j));
							cheakRight = false;
						}
					}
				}
			}
		}

		/*-----��-----*/
		int maxUp = lastY + 1;
		if (maxUp > 2 && lastY >= 2)
		{
			for (int i = 1; i < maxUp; i++)
			{
				//��m�F
				if (cheakUp)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY - i][lastX].type == NONE)
					{
						cheakUp = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY - i][lastX].isFront == side && i == 1)
					{
						cheakUp = false;
					}
					//������������
					else if (othelloDatas[lastY - i][lastX].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY - j][lastX].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY - j, lastX));
							cheakUp = false;
						}
					}
				}
			}
		}

		/*-----��-----*/
		int maxDown = MAX_SIZE_Y - lastY;
		if (maxDown > 2 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxDown; i++)
			{
				//���m�F
				if (cheakDown)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY + i][lastX].type == NONE)
					{
						cheakDown = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY + i][lastX].isFront == side && i == 1)
					{
						cheakDown = false;
					}
					//������������
					else if (othelloDatas[lastY + i][lastX].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY + j][lastX].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY + j, lastX));
							cheakDown = false;
						}
					}
				}
			}
		}

		int maxTopLeft;
		if (lastX <= lastY) { maxTopLeft = lastX + 1; }
		else { maxTopLeft = lastY + 1; }

		/*-----���΂ߏ�-----*/
		if (maxTopLeft > 2 && lastX >= 2 && lastY >= 2)
		{
			for (int i = 1; i < maxTopLeft; i++)
			{
				//���΂ߏ�m�F
				if (cheakTopLeft)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY - i][lastX - i].type == NONE)
					{
						cheakTopLeft = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY - i][lastX - i].isFront == side && i == 1)
					{
						cheakTopLeft = false;
					}
					//������������
					else if (othelloDatas[lastY - i][lastX - i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY - j][lastX - j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY - j, lastX - j));
							cheakTopLeft = false;
						}
					}
				}
			}
		}

		int maxBottomLeft;
		if (lastX <= lastY) { maxBottomLeft = lastY + 1; }
		else { maxBottomLeft = lastX + 1; }

		/*-----���΂߉�-----*/
		if (maxBottomLeft > 2 && lastX >= 2 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxBottomLeft; i++)
			{
				//���΂߉��m�F
				if (cheakBottomLeft)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY + i][lastX - i].type == NONE)
					{
						cheakBottomLeft = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY + i][lastX - i].isFront == side && i == 1)
					{
						cheakBottomLeft = false;
					}
					//������������
					else if (othelloDatas[lastY + i][lastX - i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY + j][lastX - j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY + j, lastX - j));
							cheakBottomLeft = false;
						}
					}
				}
			}
		}

		int maxTopRight;
		if (lastX <= lastY) { maxTopRight = lastY + 1; }
		else { maxTopRight = lastX + 1; }

		/*-----�E�΂ߏ�-----*/
		if (maxTopRight > 2 && lastX <= MAX_SIZE_X - 3 && lastY >= 2)
		{
			for (int i = 1; i < maxTopRight; i++)
			{
				//�E�΂ߏ�m�F
				if (cheakTopRight)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY - i][lastX + i].type == NONE)
					{
						cheakTopRight = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY - i][lastX + i].isFront == side && i == 1)
					{
						cheakTopRight = false;
					}
					//������������
					else if (othelloDatas[lastY - i][lastX + i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY - j][lastX + j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY - j, lastX + j));
							cheakTopRight = false;
						}
					}
				}
			}
		}

		int maxBottomRight;
		if (lastX <= lastY) { maxBottomRight = MAX_SIZE_Y - lastY; }
		else { maxBottomRight = MAX_SIZE_X - lastX; }

		/*-----�E�΂߉�-----*/
		if (maxBottomRight > 2 && lastX <= MAX_SIZE_X - 3 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxBottomRight; i++)
			{
				//�E�΂ߏ�m�F
				if (cheakBottomRight)
				{
					//�����Ȃ�������
					if (othelloDatas[lastY + i][lastX + i].type == NONE)
					{
						cheakBottomRight = false;
					}
					//�אڂ������F��������
					else if (othelloDatas[lastY + i][lastX + i].isFront == side && i == 1)
					{
						cheakBottomRight = false;
					}
					//������������
					else if (othelloDatas[lastY + i][lastX + i].isFront == side && i != 1)
					{
						for (int j = 1; j < i; j++)
						{
							othelloDatas[lastY + j][lastX + j].isFront = side;
							saveOthelloDataPos.push_back(make_pair(lastY + j, lastX + j));
							cheakBottomRight = false;
						}
					}
				}
			}
		}

		if (!comboOthelloDataPos.empty())
		{
			comboOthelloDataPos.erase(comboOthelloDataPos.begin());
			if (comboOthelloDataPos.empty())
			{
				if (!saveOthelloDataPos.empty()) {
					std::copy(
						saveOthelloDataPos.begin(),
						saveOthelloDataPos.end(),
						std::back_inserter(comboOthelloDataPos));
					combo++;
					saveOthelloDataPos.clear();
					lastY = comboOthelloDataPos.front().first;
					lastX = comboOthelloDataPos.front().second;
				}
			}
			else
			{
				lastY = comboOthelloDataPos.front().first;
				lastX = comboOthelloDataPos.front().second;
			}
		}
		else  if (!saveOthelloDataPos.empty()) {
			std::copy(
				saveOthelloDataPos.begin(),
				saveOthelloDataPos.end(),
				std::back_inserter(comboOthelloDataPos));
			combo++;
			saveOthelloDataPos.clear();
			lastY = comboOthelloDataPos.front().first;
			lastX = comboOthelloDataPos.front().second;
		}
	}

	//�Ō�ɃR���{��̔Ֆʃf�[�^�𑗂�
}

const vector<vector<SendOthelloData>>& CheakOthello::GetOthelloDatas()
{
	return othelloDatas;
}
