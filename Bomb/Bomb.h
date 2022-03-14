#pragma once
#include <DirectXMath.h>


struct BombData
{
	DirectX::XMVECTOR pos = {};//���e�̍��W
	DirectX::XMVECTOR bombAngle = {0, 0, 0};
	float bombRadius = 0;//���e���̂̑傫��
	float blastRadius = 10;//�����̔��a
	float blastPower = 10;//�����̋���
	int blastTimer = 30;//�����^�C�}�[(�Ƃ肠�����g��Ȃ�)
	bool isAlive = false;
};
class Bomb
{
public:
	Bomb();
	~Bomb();
	void Init();
	void Update();
	void Finailize();
	void Draw();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="angle">����</param>
	/// <param name="pos"></param>
	/// <returns></returns>
	bool Shot(DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 pos);
private:
	BombData data;
};