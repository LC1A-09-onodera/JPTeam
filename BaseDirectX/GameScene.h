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

class AddScore
{
public:
	EachInfo scoreEach[8];
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	XMFLOAT3 UpToStartPos;
	XMFLOAT3 UpToEndPos;
	bool isAppeared = false;
	bool isUpToed = false;
	bool isUp = false;
	bool isActive = false;
	int posNum = 0;
	const float easeUpCount = 0.02f;
	float easeTime = 0.0f;
	float easeUpTime = 0.0f;
	int score;
	void Init(int pos, int GetScore);
	void Update();
	void UpInit();
	void UpUpdate();
};

class AddScoreManager
{
public:
	const int MaxNum = 3;
	list<AddScore> scores;
	void Init(int PosNum, int GetScore);
	void Update();
	void Reset();
	int nowDisplayNum;
};

class GameScene
{
private:
	//更新
	void GameUpdate();
	void TitleUpdate();
	void EndUpdate();
	void SelectUpdate();
	void ResultUpdate();
	//描画
	void GameDraw();
	void TitleDraw();
	void EndDraw();
	void SelectDraw();
	void ResultDraw();
	void GetScoreDraw();
	int SceneNum = TITLE;
	void ReStart();
	void ToGame();
	void ToGame2();
	void ToGame3();
	bool isToTutorial = false;
	bool isToDojo = false;
	void ToGame4(bool flags = false, bool isToTutorial = false, bool isToDojo = false);
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
	static const int scoreNum = 7;


public:
	GameScene();
	~GameScene();
	//シーン切り替え
	void SceneManageUpdateAndDraw();
	//初期化
	void Init();
	void LightUpdate();

public://シーンで使うもの
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
	//タイトルからgameシーンへの以降に使う
	Sprite sceneChage;
	bool isSceneChange;
	int SceneWindowSize;
	//ゲーム開始時にカウントダウンを行う
	int countDown;
	bool isCountDownEnd;
	int countDownFrame;
	const int countMax = 239;
	//Sprite numbers[30];
	//gameシーンからリザルトから使う
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
	const float RESULT_EASE_TIMER_COUNT = 0.01f;
	XMFLOAT3 oldPos[3] = { { 0,0,0 } };
	XMFLOAT3 addOutBack = { 0,0,0 };
	float resultEaseTimer = 0.0f;
	bool isCheck[3] = { false };

	//スコア用
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
	//セレクトされているモード
	//falseならスコアアタック
	bool selectMode = false;
	bool selectWindow = false;
	//モードセレクトに入るかどうかをチェック
	bool select = false;

	int selectStageNum = -2;
	bool selectStage = false;

	//動く方向右がfalse;左がture;
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

	//Timer管理用
	const int CHANGE_TIMER_SECOND = 100;	//-1の値から振動開始
	const int MAX_COUNT = 10;				//何フレームかけて拡縮するか（偶数必須）
	const int CHANGE_COLOR_COUNT = 95;		//何カウント目で色を変更するようにするか
	const int DRAW_COUNTDOWN_COUNT = 6;
	const float ADD_SIZE = 0.05f;			//1フレームで拡縮するサイズ
	const float MAX_SIZE_X = 0.3f;			//初期の画像サイズ_X
	const float MAX_SIZE_Y = 0.3f;			//初期の画像サイズ_Y

	OthelloEachInfo countDownInfo;
	bool isChanged = false;
	bool isSetRand = false;
	int timerCount = 0;
	float size_x = MAX_SIZE_X;
	float size_y = MAX_SIZE_Y;
	//ステージ選択の画面に行く
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

	//falseはそのままリザルトへ
	bool isResultOrTitle = false;

	bool isResultStart = false;

	bool isTipsDraw = false;


	//タイトル演出用
	const float EASE_TIMER_COUNT = 0.1f;
	Sprite titleLogo[4];
	XMFLOAT3 logoSize;
	int logoNum;
	float easeTimer;
	bool isDrawLogo;
	bool isDrawLogoEnd = false;

	//tips用
	const int CHANGE_TIMER_FRAME = 90;	//tipsの中身が切り替わるフレーム
	const int FIRST_SS_DELAY = 180;		//最初のスクショのディレイ
	const int USE_TEX_0[7] = {			//tips_0で使うテクスチャの数
		3,
		3,
		3,
		2,
		3,
		5,
		7
	};
	const float EASE_ADD_TIMER = 0.005f;//加算されるタイマー	
	const float SIZE_X = 768;			//tipsの中身の画像サイズ_X
	const float SIZE_Y = 432;			//tipsの中身の画像サイズ_Y
	const float START_X = 50;			//tipsの中身の画像位置_X61
	const float START_Y = 179;			//tipsの中身の画像位置_Y
	//Model pushButton_black;
	//EachInfo pushButton[3];				//ボタン黒
	//Model pushButton_green;				//ボタン緑
	Sprite pushButtonGreen;
	Sprite pushButtonBlack[3];
	Sprite tips[7];						//tips枠
	Sprite tips_ss[3];					//tips_0中身

	//const int USE_TEX_1 = 3;			//tips_1で使うテクスチャの数
	//const int USE_TEX_2 = 3;			//tips_2で使うテクスチャの数
	//const int USE_TEX_3 = 3;			//tips_3で使うテクスチャの数
	//const int USE_TEX_4 = 3;			//tips_4で使うテクスチャの数
	//const int USE_TEX_5 = 3;			//tips_5で使うテクスチャの数
	//const int USE_TEX_6 = 3;			//tips_6で使うテクスチャの数

	Sprite tips_ss_1[3];					//tips_1中身
	Sprite tips_ss_2[3];					//tips_2中身
	Sprite tips_ss_3[2];					//tips_3中身
	Sprite tips_ss_4[3];					//tips_4中身
	Sprite tips_ss_5[5];					//tips_5中身
	Sprite tips_ss_6[7];					//tips_6中身

	float size = 0;
	/*----------新tips用----------*/
	const float EASE_DELAY = 0.2f;
	const float EASE_TIPS_ADD_TIMER = 0.01f;

	const float NAME_X = 50;					//ゴール_X
	const float NAME_Y = 50;					//ゴール_Y
	const float NAME_START_X[8] = {				//スタート_X
		-412,
		-327,
		-243,
		-80,
		-328,
		-499,
		-245,
		-495
	};
	const float NAME_START_Y = 50;				//スタート_Y

	const float SYSTEM_X[2] = {					//ゴール_X
		1110,
		1080
	};
	const float SYSTEM_Y = 178;					//ゴール_Y
	const float SYSTEM_START_X[2] = {			//スタート_X
		118,
		149
	};
	const float SYSTEM_START_Y = 178;			//スタート_Y

	const float BAR_X = 869;					//ゴール_X
	const float BAR_Y = 216;					//ゴール_Y
	const float BAR_START_X = 1280 + 361;		//スタート_X
	const float BAR_START_Y = 216;				//スタート_Y

	//const float TEXT_X = 856;					//ゴール_X
	const float TEXT_Y = 228;					//ゴール_Y
	//const float TEXT_START_X = 1280 + 377;	//スタート_X
	const float TEXT_START_Y = 228;				//スタート_Y

	const float TEXT_X[8] = {
		856,	//コマの移動
		888,	//リバース
		960,	//リーチ
		906,	//壁
		906,	//コネクト
		972,	//ブックエンド
		908,	//ワルツ
		942		//リバーシブル
	};
	const float TEXT_START_X[8] = {
		377,
		342,
		269,
		324,
		323,
		257,
		321,
		287
	};

	Sprite tips_frame;					//tips枠
	Sprite tips_name[7];				//tips名前
	Sprite tips_system[2];				//tipsシステム
	Sprite tips_bar;					//tipsバー
	Sprite tips_text[7];				//tipsテキスト
	bool isInit;
	bool isFirst = true;
	float newTips_easeTimer;
	/*----------新tips用----------*/
	Sprite tips_cont;					//tipsコントローラー用（用修正）
	int tipsCounts;
	int tipsDrawNum;
	int changeTimerFrame;
	float tips_easeTimer;				//タイトルと同じ速さ
	//float green_scale;
	bool isTipsDrawTrigger;
	bool moveTips;

	//シーン切り替えをはじめる
	bool isPouseToTiTle = false;
	float pouseToTitleEaseTime1;
	//背景出す
	bool isBackGroundOthello = false;
	//ノーマルオセロの捜査を始める
	bool isNormalOthello = false;
	float titleScaleEaseTime = 0.0f;

	bool isCameraModed = false;

	//ポーズ用モデル-----------------
	bool isSupport = true;
	Model SupportModel;
	Model BackModel;
	Model TiTleModel;
	Model EndModel;
	Model ChackBoxModel;
	Model CheckModel;
	EachInfo pouseKakko[2];
	EachInfo pouseBackEach;
	XMFLOAT3 pouseMinSizeConst;
	XMFLOAT3 pouseMaxSizeConst;
	XMFLOAT3 pouseNormalSizeConst;
	XMFLOAT3 pouseNowSize;
	float pouseSelectTime1;
	float pouseSelectTime2;
	//-------------------------------
	AddScoreManager addScore;
	Model reverseAddObject;
	bool reverseDisplay = false;
	int reverseDisplayTime = 0;
	const int MaxReverseDisplayTime = 120;
};