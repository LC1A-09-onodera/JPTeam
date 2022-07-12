#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"

class MenuUI
{
protected:
	//��������
	Sprite pouseBack;
	//�X�N���[�������l
	float pouseScroll;
	//�����܂ŃX�N���[���ł���悤��
	float maxpouseScroll;
	//��̗v�f�̑傫��
	float eachHeight;
	//������傫���Ȃ�X�N���[���ł���悤�ɂ���
	float showWindowHeight;

	list<pair<string, Sprite>> keySprites;

public:
	void LoadFile(BaseDirectX &baseDirectX);
	void LoadFile(BaseDirectX& baseDirectX, string name);
	void Init(float height = 5.0f);

	void DrawUI(BaseDirectX& baseDirectX);


};