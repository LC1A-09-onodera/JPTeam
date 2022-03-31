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

void CheakOthello::Update(const vector<vector<SendOthelloData>> &othelloData)
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
	othelloDatas =  othelloData;
	//その場にオセロが存在するかつ、最後に動かしてたかを判定（←ちょっと効率悪そう）
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if(othelloDatas[i][j].type==NONE) {continue;}
			// 最後に動かしてるやつ
			if(!othelloDatas[i][j].isMove) {continue;}
			lastX = j;
			lastY = i;
			side= othelloDatas[i][j].isFront;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}

	//最後のを基準に左右と斜め判定
	while (!comboOthelloDataPos.empty() || !saveOthelloDataPos.empty())
	{
		 /*-----左-----*/
		 int maxLeft=lastX;
		 for(int i=1;i<maxLeft;i++)
		 {
		 
				//左確認
				if(cheakLeft)
				{
					//何もなかったら
					if(othelloDatas[lastY][lastX-i].type ==NONE)
					{
						cheakLeft=false;
					}
					//同じだったら
					else if(othelloDatas[lastY][lastX-i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY][lastX-j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY,lastX-j));
						}
					}
				}
		 }
		 
		 /*-----右-----*/
		 int maxRight=MAX_SIZE_X-1-lastX;
		 for(int i=1;i<maxRight;i++)
		 {
				//右確認
				if(cheakRight)
				{
					//何もなかったら
					if(othelloDatas[lastY][lastX+i].type ==NONE)
					{
						cheakRight=false;
					}
					//同じだったら
					else if(othelloDatas[lastY][lastX+i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY][lastX+j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY,lastX+j));
						}
					}
				}
		 }
		 
		 /*-----上-----*/
		 int maxUp=lastY;
		 for(int i=1;i<maxUp;i++)
		 {
				//上確認
				if(cheakUp)
				{
					//何もなかったら
					if(othelloDatas[lastY-i][lastX].type ==NONE)
					{
						cheakUp=false;
					}
					//同じだったら
					else if(othelloDatas[lastY-i][lastX].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY-j][lastX].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY-j,lastX));
						}
					}
				}
		 }
		 
		 /*-----下-----*/
		 int maxDown=MAX_SIZE_Y-1-lastY;
		 for(int i=1;i<maxUp;i++)
		 {
				//下確認
				if(cheakUp)
				{
					//何もなかったら
					if(othelloDatas[lastY+i][lastX].type ==NONE)
					{
						cheakDown=false;
					}
					//同じだったら
					else if(othelloDatas[lastY+i][lastX].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY+j][lastX].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY+j,lastX));
						}
					}
				}
		 }
		 
		int maxTopLeft;
		if (lastX <= lastY) { maxTopLeft = lastX; }
		else { maxTopLeft = lastY; }
		 
		 /*-----左斜め上-----*/
		 for(int i=1;i<maxTopLeft;i++)
		 {
				//左斜め上確認
				if(cheakTopLeft)
				{
					//何もなかったら
					if(othelloDatas[lastY-i][lastX-i].type ==NONE)
					{
						cheakTopLeft=false;
					}
					//同じだったら
					else if(othelloDatas[lastY-i][lastX-i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY-j][lastX-j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY-j,lastX-j));
						}
					}
				}
		 }
		 
		int maxBottomLeft;
		if (lastX <= lastY) { maxBottomLeft = lastX; }
		else { maxBottomLeft = lastY; }

		 /*-----左斜め下-----*/
		 for(int i=1;i<maxTopLeft;i++)
		 {
				//左斜め下確認
				if(cheakBottomLeft)
				{
					//何もなかったら
					if(othelloDatas[lastY+i][lastX-i].type ==NONE)
					{
						cheakBottomLeft=false;
					}
					//同じだったら
					else if(othelloDatas[lastY+i][lastX-i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY+j][lastX-j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY+j,lastX-j));
						}
					}
				}
		 }

		int maxTopRight;
		if (lastX <= lastY) { maxTopRight = lastX; }
		else { maxTopRight = lastY; }

		 /*-----右斜め上-----*/
		 for(int i=1;i<maxTopRight;i++)
		 {
				//右斜め上確認
				if(cheakTopRight)
				{
					//何もなかったら
					if(othelloDatas[lastY-i][lastX+i].type ==NONE)
					{
						cheakTopRight=false;
					}
					//同じだったら
					else if(othelloDatas[lastY-i][lastX+i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY-j][lastX+j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY-j,lastX+j));
						}
					}
				}
		 }

		int maxBottomRight;
		if (lastX <= lastY) { maxBottomRight = lastX; }
		else { maxBottomRight = lastY; }

		 /*-----右斜め下-----*/
		 for(int i=1;i<maxBottomRight;i++)
		 {
				//右斜め上確認
				if(cheakBottomRight)
				{
					//何もなかったら
					if(othelloDatas[lastY+i][lastX+i].type ==NONE)
					{
						cheakTopRight=false;
					}
					//同じだったら
					else if(othelloDatas[lastY+i][lastX+i].isFront ==side&&i!=1)
					{
						for(int j=1;j<i+1;j++)
						{
							othelloDatas[lastY+j][lastX+j].isFront =side;
							saveOthelloDataPos.push_back(make_pair(lastY+j,lastX+j));
						}
					}
				}
		 }

		if (!comboOthelloDataPos.empty()) { comboOthelloDataPos.erase(comboOthelloDataPos.begin()); }
		else  if (!saveOthelloDataPos.empty()) {
			std::copy(
				saveOthelloDataPos.begin(),
				saveOthelloDataPos.end(),
				std::back_inserter(comboOthelloDataPos));
			combo++;
		}
	}

	//最後にコンボ後の盤面データを送る
}
