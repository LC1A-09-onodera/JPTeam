#include "CheakOthello.h"
#include <string>
#include <iterator>

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

void CheakOthello::Update(const OthelloData(&othelloData)[MAX_SIZE_Y][MAX_SIZE_Y])
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

	memcpy(othelloDatas, othelloData, sizeof(othelloData));

	//���̏�ɃI�Z�������݂��邩�A�Ō�ɓ������Ă����𔻒�i��������ƌ����������j
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			//if(othelloDatas[i][j].Status==NONE) {continue;}
			//if(!othelloDatas[i][j].last) {continue;}
			lastX = j;
			lastY = i;
			//side= othelloDatas[i][j].Side;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			break;
		}
	}

	//�Ō�̂���ɍ��E�Ǝ΂ߔ���
	while (!comboOthelloDataPos.empty() && !saveOthelloDataPos.empty())
	{
		// /*-----��-----*/
		// int maxLeft=lastX+1;(�ŏ����獶�ɉ����Ȃ��ꍇ�������K�v)
		// if(maxLeft>2) {}
		// for(int i=1;i<maxLeft;i++)
		// {
		// 
		//		//���m�F
		//		if(cheakLeft)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY][lastX-i].Status==NONE)
		//			{
		//				cheakLeft=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY][lastX-i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY][lastX-j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY,lastX-j));
		//				}
		//			}
		//		}
		// }
		// 
		// /*-----�E-----*/
		// int maxRight=MAX_SIZE_X-lastX;
		// if(maxRight>2) {}
		// for(int i=1;i<maxRight;i++)
		// {
		//		//�E�m�F
		//		if(cheakRight)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY][lastX+i].Status==NONE)
		//			{
		//				cheakRight=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY][lastX+i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY][lastX+j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY,lastX+j));
		//				}
		//			}
		//		}
		// }
		// 
		// /*-----��-----*/
		// int maxUp=lastY+1;
		// if(maxUp>2) {}
		// for(int i=1;i<maxUp;i++)
		// {
		//		//��m�F
		//		if(cheakUp)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY-i][lastX].Status==NONE)
		//			{
		//				cheakUp=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY-i][lastX].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY-j][lastX].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY-j,lastX));
		//				}
		//			}
		//		}
		// }
		// 
		// /*-----��-----*/
		// int maxDown=MAX_SIZE_Y-lastY;
		// if(maxDown>2) {}
		// for(int i=1;i<maxUp;i++)
		// {
		//		//���m�F
		//		if(cheakUp)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY+i][lastX].Status==NONE)
		//			{
		//				cheakDown=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY+i][lastX].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY+j][lastX].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY+j,lastX));
		//				}
		//			}
		//		}
		// }
		// 
		int maxTopLeft;
		if (lastX <= lastY) { maxTopLeft = lastX; }
		else { maxTopLeft = lastY; }

		if (maxTopLeft > 2) {}
		// /*-----���΂ߏ�-----*/
		// for(int i=1;i<maxTopLeft;i++)
		// {
		//		//���΂ߏ�m�F
		//		if(cheakTopLeft)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY-i][lastX-i].Status==NONE)
		//			{
		//				cheakTopLeft=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY-i][lastX-i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY-j][lastX-j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY-j,lastX-j));
		//				}
		//			}
		//		}
		// }
		// 
		int maxBottomLeft;
		if (lastX <= lastY) { maxBottomLeft = lastX; }
		else { maxBottomLeft = lastY; }

		if (maxBottomLeft > 2) {}
		// /*-----���΂߉�-----*/
		// for(int i=1;i<maxTopLeft;i++)
		// {
		//		//���΂߉��m�F
		//		if(cheakBottomLeft)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY+i][lastX-i].Status==NONE)
		//			{
		//				cheakBottomLeft=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY+i][lastX-i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY+j][lastX-j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY+j,lastX-j));
		//				}
		//			}
		//		}
		// }

		int maxTopRight;
		if (lastX <= lastY) { maxTopRight = lastX; }
		else { maxTopRight = lastY; }

		if (maxTopRight > 2) {}
		// /*-----�E�΂ߏ�-----*/
		// for(int i=1;i<maxTopRight;i++)
		// {
		//		//�E�΂ߏ�m�F
		//		if(cheakTopRight)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY-i][lastX+i].Status==NONE)
		//			{
		//				cheakTopRight=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY-i][lastX+i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY-j][lastX+j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY-j,lastX+j));
		//				}
		//			}
		//		}
		// }

		int maxBottomRight;
		if (lastX <= lastY) { maxBottomRight = lastX; }
		else { maxBottomRight = lastY; }

		if (maxBottomRight > 2) {}
		// /*-----�E�΂߉�-----*/
		// for(int i=1;i<maxBottomRight;i++)
		// {
		//		//�E�΂ߏ�m�F
		//		if(cheakBottomRight)
		//		{
		//			//�����Ȃ�������
		//			if(othelloData[lastY+i][lastX+i].Status==NONE)
		//			{
		//				cheakTopRight=false;
		//			}
		//			//������������
		//			else if(othelloData[lastY+i][lastX+i].Side==side&&i!=1)
		//			{
		//				for(int j=1;j<i+1;j++)
		//				{
		//					othelloData[lastY+j][lastX+j].Side=side;
		//					saveOthelloDataPos.push_back(make_pair(lastY+j,lastX+j));
		//				}
		//			}
		//		}
		// }

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
