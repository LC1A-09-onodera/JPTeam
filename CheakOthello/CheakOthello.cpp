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
	othelloDatas =  othelloData;
	//���̏�ɃI�Z�������݂��邩�A�Ō�ɓ������Ă����𔻒�i��������ƌ����������j
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if(othelloDatas[i][j].type==NONE) {continue;}
			// �Ō�ɓ������Ă���
			if(!othelloDatas[i][j].isMove) {continue;}
			lastX = j;
			lastY = i;
			side= othelloDatas[i][j].isFront;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}

	//�Ō�̂���ɍ��E�Ǝ΂ߔ���
	while (!comboOthelloDataPos.empty() || !saveOthelloDataPos.empty())
	{
		 /*-----��-----*/
		 int maxLeft=lastX;
		 for(int i=1;i<maxLeft;i++)
		 {
		 
				//���m�F
				if(cheakLeft)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY][lastX-i].type ==NONE)
					{
						cheakLeft=false;
					}
					//������������
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
		 
		 /*-----�E-----*/
		 int maxRight=MAX_SIZE_X-1-lastX;
		 for(int i=1;i<maxRight;i++)
		 {
				//�E�m�F
				if(cheakRight)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY][lastX+i].type ==NONE)
					{
						cheakRight=false;
					}
					//������������
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
		 
		 /*-----��-----*/
		 int maxUp=lastY;
		 for(int i=1;i<maxUp;i++)
		 {
				//��m�F
				if(cheakUp)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY-i][lastX].type ==NONE)
					{
						cheakUp=false;
					}
					//������������
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
		 
		 /*-----��-----*/
		 int maxDown=MAX_SIZE_Y-1-lastY;
		 for(int i=1;i<maxUp;i++)
		 {
				//���m�F
				if(cheakUp)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY+i][lastX].type ==NONE)
					{
						cheakDown=false;
					}
					//������������
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
		 
		 /*-----���΂ߏ�-----*/
		 for(int i=1;i<maxTopLeft;i++)
		 {
				//���΂ߏ�m�F
				if(cheakTopLeft)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY-i][lastX-i].type ==NONE)
					{
						cheakTopLeft=false;
					}
					//������������
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

		 /*-----���΂߉�-----*/
		 for(int i=1;i<maxTopLeft;i++)
		 {
				//���΂߉��m�F
				if(cheakBottomLeft)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY+i][lastX-i].type ==NONE)
					{
						cheakBottomLeft=false;
					}
					//������������
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

		 /*-----�E�΂ߏ�-----*/
		 for(int i=1;i<maxTopRight;i++)
		 {
				//�E�΂ߏ�m�F
				if(cheakTopRight)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY-i][lastX+i].type ==NONE)
					{
						cheakTopRight=false;
					}
					//������������
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

		 /*-----�E�΂߉�-----*/
		 for(int i=1;i<maxBottomRight;i++)
		 {
				//�E�΂ߏ�m�F
				if(cheakBottomRight)
				{
					//�����Ȃ�������
					if(othelloDatas[lastY+i][lastX+i].type ==NONE)
					{
						cheakTopRight=false;
					}
					//������������
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

	//�Ō�ɃR���{��̔Ֆʃf�[�^�𑗂�
}
