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
	int lastX, lastY;				//最後の座標を保存する変数
	bool side;					//表か裏か
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
	//その場にオセロが存在するかつ、最後に動かしてたかを判定（←ちょっと効率悪そう）
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if (othelloDatas[i][j].type == NONE) { continue; }
			// 最後に動かしてるやつ
			if (!othelloDatas[i][j].isMove) { continue; }
			lastX = j;
			lastY = i;
			side = othelloDatas[i][j].isFront;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}

	//最後のを基準に左右と斜め判定(jのfor文の範囲、i+1する必要あるか??)
	while (!comboOthelloDataPos.empty() || !saveOthelloDataPos.empty())
	{
		/*-----左-----*/
		int maxLeft = lastX + 1;
		if (maxLeft > 2 && lastX >= 2)
		{
			for (int i = 1; i < maxLeft; i++)
			{

				//左確認
				if (cheakLeft)
				{
					//何もなかったら
					if (othelloDatas[lastY][lastX - i].type == NONE)
					{
						cheakLeft = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY][lastX - i].isFront == side && i == 1)
					{
						cheakLeft = false;
					}
					//同じだったら
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

		/*-----右-----*/
		int maxRight = MAX_SIZE_X - lastX;
		if (maxRight > 2 && lastX <= MAX_SIZE_X - 3)
		{
			for (int i = 1; i < maxRight; i++)
			{
				//右確認
				if (cheakRight)
				{
					//何もなかったら
					if (othelloDatas[lastY][lastX + i].type == NONE)
					{
						cheakRight = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY][lastX + i].isFront == side && i == 1)
					{
						cheakRight = false;
					}
					//同じだったら
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

		/*-----上-----*/
		int maxUp = lastY + 1;
		if (maxUp > 2 && lastY >= 2)
		{
			for (int i = 1; i < maxUp; i++)
			{
				//上確認
				if (cheakUp)
				{
					//何もなかったら
					if (othelloDatas[lastY - i][lastX].type == NONE)
					{
						cheakUp = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY - i][lastX].isFront == side && i == 1)
					{
						cheakUp = false;
					}
					//同じだったら
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

		/*-----下-----*/
		int maxDown = MAX_SIZE_Y - lastY;
		if (maxDown > 2 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxDown; i++)
			{
				//下確認
				if (cheakDown)
				{
					//何もなかったら
					if (othelloDatas[lastY + i][lastX].type == NONE)
					{
						cheakDown = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY + i][lastX].isFront == side && i == 1)
					{
						cheakDown = false;
					}
					//同じだったら
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

		/*-----左斜め上-----*/
		if (maxTopLeft > 2 && lastX >= 2 && lastY >= 2)
		{
			for (int i = 1; i < maxTopLeft; i++)
			{
				//左斜め上確認
				if (cheakTopLeft)
				{
					//何もなかったら
					if (othelloDatas[lastY - i][lastX - i].type == NONE)
					{
						cheakTopLeft = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY - i][lastX - i].isFront == side && i == 1)
					{
						cheakTopLeft = false;
					}
					//同じだったら
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

		/*-----左斜め下-----*/
		if (maxBottomLeft > 2 && lastX >= 2 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxBottomLeft; i++)
			{
				//左斜め下確認
				if (cheakBottomLeft)
				{
					//何もなかったら
					if (othelloDatas[lastY + i][lastX - i].type == NONE)
					{
						cheakBottomLeft = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY + i][lastX - i].isFront == side && i == 1)
					{
						cheakBottomLeft = false;
					}
					//同じだったら
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

		/*-----右斜め上-----*/
		if (maxTopRight > 2 && lastX <= MAX_SIZE_X - 3 && lastY >= 2)
		{
			for (int i = 1; i < maxTopRight; i++)
			{
				//右斜め上確認
				if (cheakTopRight)
				{
					//何もなかったら
					if (othelloDatas[lastY - i][lastX + i].type == NONE)
					{
						cheakTopRight = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY - i][lastX + i].isFront == side && i == 1)
					{
						cheakTopRight = false;
					}
					//同じだったら
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

		/*-----右斜め下-----*/
		if (maxBottomRight > 2 && lastX <= MAX_SIZE_X - 3 && lastY <= MAX_SIZE_Y - 3)
		{
			for (int i = 1; i < maxBottomRight; i++)
			{
				//右斜め上確認
				if (cheakBottomRight)
				{
					//何もなかったら
					if (othelloDatas[lastY + i][lastX + i].type == NONE)
					{
						cheakBottomRight = false;
					}
					//隣接かつ同じ色だったら
					else if (othelloDatas[lastY + i][lastX + i].isFront == side && i == 1)
					{
						cheakBottomRight = false;
					}
					//同じだったら
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

	//最後にコンボ後の盤面データを送る
}

const vector<vector<SendOthelloData>>& CheakOthello::GetOthelloDatas()
{
	return othelloDatas;
}
