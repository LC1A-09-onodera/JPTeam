#pragma once
#include "../Sprite/Sprite.h"
#include "../BaseDirectX/BaseDirectX.h"

class MenuUI
{
protected:
	//��������
	Sprite pouseBack;
	//�X�N���[�������l
	float pouseScroll = 0.0f;
	//�����܂ŃX�N���[���ł���悤��
	float maxpouseScroll = 0.0f;
	//��̗v�f�̑傫��
	float eachHeight = 0.0f;
	//������傫���Ȃ�X�N���[���ł���悤�ɂ���
	float showWindowHeight = 0.0f;

	list<pair<string, Sprite>> keySprites;

public:
	void LoadFile(BaseDirectX &baseDirectX);
	void LoadFile(BaseDirectX& baseDirectX, string name);
	void Init(float height = 5.0f);

	void DrawUI(BaseDirectX& baseDirectX);


};