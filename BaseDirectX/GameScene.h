#pragma once
#include "../3DModel/Model.h"
#include "../Camera/Camera.h"
#include "../Sprite/Sprite.h"
#include "../Light/Light.h"
#include "../PostEffect/PostEffect.h"
#include "../FBXObject/FBXObject.h"
#include "../FbxLoader/FbxLoader.h"
#include "../Oserro/Oserro.h"
#include "../CheakOthello/CheakOthello.h"
#include "../Sound/Sound.h"
#include "../3DObjectParticle/3DObjectParticle.h"

enum GameSceneName { TITLE, SELECT, GAME, END, RESULT };

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
	void ReStart();
	void ToGame();
	void ToGame2();
	void ToGame3();
	void ToGame4(bool flags = false);
	void ToTutorial();

	void ToGame1Update();
	void ToGame2Update();
	void ToGame3Update();
	void ToGame4Update();
	void ToModeSelectUpdate();
	void ToTutorialUpdate();
	void ToModeSelect();
	void ToTitle();
	void ToTitleUpdate();
	void ToResult();
	void PouseToTitle();
	void PouseToTitleUpdate();
	void NormaToModeSelect();
	void NormaToModeSelectUpdate();
	bool isNormaToSelect;
	void NormaToModeSelect2();
	void NormaToModeSelectUpdate2();
public:
	GameScene();
	~GameScene();
	//�V�[���؂�ւ�
	void SceneManageUpdateAndDraw();
	//������
	void Init();
	void LightUpdate();

public://�V�[���Ŏg������
	Light* light = nullptr;
	PostEffect post;
	XMFLOAT3 gameNowEye = { -1.0f, -10.0f, -15.0f };
	XMFLOAT3 gameOpEye = { 0, 0, -15.0f };
	XMFLOAT3 gameNowTarget = { -1.0f, 0, 0 };
	XMFLOAT3 gameOpTarget = { -1.0f, 100.0f, 0 };
	float pointLightPos[3] = { 0, 1, 0 };
	float pointLightColor[3] = { 1, 1, 1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 1, -100 };
	float spotLightColor[3] = { 0.9f, 0.9f, 0.9f };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightAngle[2] = { 1.0f, 500.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.0f };

	bool isTipsButtonDraw;
	//PostEffect postEffect;
	Model sample;
	OthelloManager othelloManager;
	CheakOthello checkObject;
	Sprite title;
	Sprite titleBack;
	Sprite spaceBack;
	Sprite space;
	Sprite pouseBack;
	//�^�C�g������game�V�[���ւ̈ȍ~�Ɏg��
	Sprite sceneChage;
	bool isSceneChange;
	int SceneWindowSize;
	//�Q�[���J�n���ɃJ�E���g�_�E�����s��
	int countDown;
	bool isCountDownEnd;
	int countDownFrame;
	const int countMax = 239;
	//Sprite numbers[30];
	//game�V�[�����烊�U���g����g��
	bool isResultSceneChange;
	int resultSceneWindowSize;

	int gameTime;

	int selectGameType;
	bool selectGameTypeActive;

	XMFLOAT3 eyeStart;
	XMFLOAT3 eyeEnd;
	float eyeEaseTime;

	int resultForTime;
	const int gameMaxTime = 5999;

	EachInfo resultMaxConbo[2];
	EachInfo resultEraseOthello[3];
	Model resultScoreKanaModel;
	Model resultMaxConboModel;
	Model resultEraseOthelloModel;

	//�X�R�A�p
	//Sprite scoreNum[60];
	Sprite scoreSprite;
	Sprite timeUp;
	Sprite startSprite;
	Tex num[10];


	Model sky;
	Model othelloStage;

	SoundData selectSound;
	SoundData timeUpSound;
	SoundData enterSound;
	SoundData BGMSound;
	SoundData countdDownSound;
	SoundData startSound;
	bool isPouse;
	int selectPouse = 0;
	const int selectMaxPouse = 2;
	bool isGameEnd;

	bool isPouseToTitle;

	Sprite moveSprite;
	Sprite reverseSprtie;
	Sprite backSprite;
	Sprite endSprite;
	Sprite titleSprite;
	Sprite kagikakkoStartSprite;
	Sprite kagikakkoEndSprite;
	Sprite kagikakkoSelectStartSprite;
	Sprite kagikakkoSelectEndSprite;
	Sprite tutorialSprite;
	Sprite gameScoreAttackSprite;
	Sprite gameNormaSprite;

	int tornadoTime;

	int displayScore;
	int oldDisplay;
	int nowScore;
	int oldScore;
	bool scoreChange;
	int changeTime;
	int changeCount;

	bool isTutorial = true;

	int titleSelectNum;
	//�Z���N�g����Ă��郂�[�h
	//false�Ȃ�X�R�A�A�^�b�N
	bool selectMode = false;
	bool selectWindow = false;
	//���[�h�Z���N�g�ɓ��邩�ǂ������`�F�b�N
	bool select = false;

	int selectStageNum = -2;
	bool selectStage = false;

	//���������E��false;����ture;
	bool selectEaseDirection = false;
	bool selectEase = false;
	float selectEaseTime = 0.0f;
	float selectNumDistance = 170.0f;
	//float selectNumBigDistance = 160.0f;
	//Sprite selectStageNumSprite[11];
	XMFLOAT3 selectStageFarstStartPos;
	XMFLOAT3 selectStageFarstEndPos;
	bool isScorePuls;

	int sceneChageType = 0;
	Sprite sceneChangeSprite2;
	XMFLOAT3 sceneChangeSpriteStartPos;
	XMFLOAT3 sceneChangeSpriteEndPos;
	int sceneChangeDiray2 = 0;
	float sceneChangeAfterTime = 0.0f;
	bool isSceneChangeRady = false;
	XMFLOAT3 camTargetStart;
	XMFLOAT3 camTargetEnd;

	bool isStageDisplay = false;
	float goToGameTime = 0.0f;

	list<FrameEach> opOthellos;
	list<FrameEach> pouseOthellos;

	//Timer�Ǘ��p
	const int CHANGE_TIMER_SECOND = 100;	//-1�̒l����U���J�n
	const int MAX_COUNT = 10;				//���t���[�������Ċg�k���邩�i�����K�{�j
	const int CHANGE_COLOR_COUNT = 95;		//���J�E���g�ڂŐF��ύX����悤�ɂ��邩
	const int DRAW_COUNTDOWN_COUNT = 6;
	const float ADD_SIZE = 0.05f;			//1�t���[���Ŋg�k����T�C�Y
	const float MAX_SIZE_X = 0.3f;			//�����̉摜�T�C�Y_X
	const float MAX_SIZE_Y = 0.3f;			//�����̉摜�T�C�Y_Y

	OthelloEachInfo countDownInfo;
	bool isChanged = false;
	bool isSetRand = false;
	int timerCount = 0;
	float size_x = MAX_SIZE_X;
	float size_y = MAX_SIZE_Y;
	//�X�e�[�W�I���̉�ʂɍs��
	bool isModeSelect = false;

	float flagss;
	OthelloModel countDownNumber[10];
	Model timerNumber[10];
	Model numbersObject[10];
	Model sNumbersObject[10];
	Model reverseObject;
	Model scoreObject;
	Model startModel;
	Model pushA;
	Model pushSpace;
	Model AButton;
	Model tutorialObject;
	Model titleObject;
	Model backGround;
	Model kakko[2];
	EachInfo backGroundEach[2];

	EachInfo reversEach[2];
	EachInfo timerEach[2];
	EachInfo scoreEach[7];
	bool isTipsOk = false;

	static const int tipsCount = 5;
	Sprite tipsSprite[tipsCount];

	int tipsNumber;
	list<pair<int, int>> tipsNumAndTexCount;

	const int MaxSceneChangeOk = 60;

	//false�͂��̂܂܃��U���g��
	bool isResultOrTitle = false;

	bool isResultStart = false;

	bool isTipsDraw = false;


	//�^�C�g�����o�p
	const float EASE_TIMER_COUNT = 0.1f;
	Sprite titleLogo[4];
	XMFLOAT3 logoSize;
	int logoNum;
	float easeTimer;
	bool isDrawLogo;
	bool isDrawLogoEnd = false;

	//tips�p
	const int CHANGE_TIMER_FRAME = 90;	//tips�̒��g���؂�ւ��t���[��
	const int USE_TEX_0 = 3;			//tips_0�Ŏg���e�N�X�`���̐�
	const float SIZE_X = 768;			//tips�̒��g�̉摜�T�C�Y_X
	const float SIZE_Y = 432;			//tips�̒��g�̉摜�T�C�Y_Y
	const float START_X = 50;			//tips�̒��g�̉摜�ʒu_X61
	const float START_Y = 179;			//tips�̒��g�̉摜�ʒu_Y
	Model pushButton_black;
	EachInfo pushButton[3];				//�{�^����
	Model pushButton_green;				//�{�^����
	Sprite tips[8];						//tips�g
	Sprite tips_ss[3];					//tips_0���g
	/*----------�Vtips�p----------*/
	const float EASE_DELAY = 0.2f;

	const float NAME_X = 50;					//�S�[��_X
	const float NAME_Y = 50;					//�S�[��_Y
	const float NAME_START_X = -412;			//�X�^�[�g_X
	const float NAME_START_Y = 50;				//�X�^�[�g_Y

	const float SYSTEM_X = 1110;				//�S�[��_X
	const float SYSTEM_Y = 178;					//�S�[��_Y
	const float SYSTEM_START_X = 1280 + 118;	//�X�^�[�g_X
	const float SYSTEM_START_Y = 178;			//�X�^�[�g_Y

	const float BAR_X = 869;					//�S�[��_X
	const float BAR_Y = 216;					//�S�[��_Y
	const float BAR_START_X = 1280 + 361;		//�X�^�[�g_X
	const float BAR_START_Y = 216;				//�X�^�[�g_Y

	const float TEXT_X = 856;					//�S�[��_X
	const float TEXT_Y = 278;					//�S�[��_Y
	const float TEXT_START_X = 1280 + 377;		//�X�^�[�g_X
	const float TEXT_START_Y = 278;				//�X�^�[�g_Y

	Sprite tips_frame;					//tips�g
	Sprite tips_name[8];				//tips���O
	Sprite tips_system[2];				//tips�V�X�e��
	Sprite tips_bar;					//tips�o�[
	Sprite tips_text[8];				//tips�e�L�X�g
	bool isInit;
	float newTips_easeTimer;
	/*----------�Vtips�p----------*/
	Sprite tips_cont;					//tips�R���g���[���[�p�i�p�C���j
	int tipsCounts;
	int tipsDrawNum;
	int changeTimerFrame;
	float tips_easeTimer;				//�^�C�g���Ɠ�������
	float green_scale;
	bool isTipsDrawTrigger;
	bool moveTips;

	//�V�[���؂�ւ����͂��߂�
	bool isPouseToTiTle = false;
	float pouseToTitleEaseTime1;
	//�w�i�o��
	bool isBackGroundOthello = false;
	//�m�[�}���I�Z���̑{�����n�߂�
	bool isNormalOthello = false;
	float titleScaleEaseTime = 0.0f;

	bool isCameraModed = false;

	//�|�[�Y�p���f��-----------------
	bool isSupport;
	Model SupportModel;
	Model BackModel;
	Model TiTleModel;
	Model EndModel;
	EachInfo pouseKakko[2];
	//-------------------------------
};