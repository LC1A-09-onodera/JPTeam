#pragma once
#include "../3DModel/Model.h"
#include "../Camera/Camera.h"
#include "../Sprite/Sprite.h"
#include "../Player/Player.h"
//#include "../Enemy/Enemy.h"
#include "../Light/Light.h"
#include "../RenderTarget/RenderTarget.h"
#include "../PostEffect/PostEffect.h"
#include "../FBXObject/FBXObject.h"
#include "../FbxLoader/FbxLoader.h"
#include "../King/King.h"
#include "../Bomb/BombManager.h"
#include "../Oserro/Oserro.h"
#include "../CheakOthello/CheakOthello.h"
#include "../water/water.h"
enum GameSceneName{TITLE, SELECT, GAME, END, RESULT};

class GameScene
{
private:
	//�X�V
	void GameUpdate();
	void TitleUpdate();
	void EndUpdate();
	void SelectUpdate();
	void ResultUpdate();
	//�`��
	void GameDraw();
	void TitleDraw();
	void EndDraw();
	void SelectDraw();
	void ResultDraw();
	int SceneNum = TITLE;

public:
	GameScene();
	~GameScene();
	//�V�[���؂�ւ�
	void SceneManageUpdateAndDraw();
	//������
	void Init();
	
	void LightUpdate();

public://�V�[���Ŏg������
	Light *light = nullptr;
	PostEffect post;
	float pointLightPos[3] = {0, 1, 0};
	float pointLightColor[3] = {1, 1, 1};
	float pointLightAtten[3] = {0.3f, 0.1f, 0.1f};

	float spotLightDir[3] = {0, -1, 0};
	float spotLightPos[3] = {0, 1, -100};
	float spotLightColor[3] = {0.9f, 0.9f, 0.9f};
	float spotLightAtten[3] = {0.0f, 0.0f, 0.0f};
	float spotLightAngle[2] = {1.0f, 500.0f};

	float circleShadowDir[3] = {0, -1, 0};
	float circleShadowAtten[3] = {0.5f, 0.6f, 0.0f};
	float circleShadowFactorAngle[2] = {0.0f, 0.0f};

	Tex stageFrameTex;
	Sprite stageFrameSp;

	PostEffect postEffect;
	Model sample;
	OthelloManager othelloManager;
	CheakOthello checkObject;
	Water water;
	Sprite title;
	Sprite titleBack;
	Sprite spaceBack;
	Sprite space;
	//�^�C�g������game�V�[���ւ̈ȍ~�Ɏg��
	Sprite sceneChage;
	bool isSceneChange;
	int SceneWindowSize;
	//�Q�[���J�n���ɃJ�E���g�_�E�����s��
	int countDown;
	bool isCountDownEnd;
	int countDownFrame;
	const int countMax = 239;
	Sprite numbers[20];
	//game�V�[�����烊�U���g����g��
	bool isResultSceneChange;
	int resultSceneWindowSize;

	int gameTime;

	XMFLOAT3 eyeStart;
	XMFLOAT3 eyeEnd;
	float eyeEaseTime;

	int resultForTime;
	const int gameMaxTime = 5999;

	//�X�R�A�p
	Sprite scoreNum[60];
	Sprite scoreSprite;
	Sprite timeUp;
	Sprite startSprite;
	Tex num[10];

	Model sky;
	Model othelloStage;

	SoundData selectSound;
};