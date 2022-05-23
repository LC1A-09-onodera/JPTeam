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
	int SceneNum = TITLE;
	void ReStart();
	void ToGame();
	void ToGame2();
	void ToGame3();
	void ToGame4(bool flags = false);

	void ToGame1Update();
	void ToGame2Update();
	void ToGame3Update();
	void ToGame4Update();
	void ToModeSelectUpdate();
	void ToModeSelect();

	void ToResult();
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

	PostEffect postEffect;
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
	Sprite numbers[30];
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

	//スコア用
	Sprite scoreNum[60];
	Sprite scoreSprite;
	Sprite timeUp;
	Sprite startSprite;
	Tex num[10];

	Sprite addScoreNum[60];
	Sprite addReverse;
	Sprite addConbo[30];

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

	int selectStageNum = 1;
	bool selectStage = false;

	//動く方向右がfalse;左がture;
	bool selectEaseDirection = false;
	bool selectEase = false;
	float selectEaseTime = 0.0f;
	float selectNumDistance = 170.0f;
	//float selectNumBigDistance = 160.0f;
	Sprite selectStageNumSprite[11];
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

	//Timer管理用
	const int CHANGE_TIMER_SECOND = 100;	//-1の値から振動開始
	const int MAX_COUNT = 10;				//何フレームかけて拡縮するか（偶数必須）
	const float ADD_SIZE = 4.0f;			//1フレームで拡縮するサイズ
	const float MAX_SIZE_X = 48.0f;			//初期の画像サイズ_X
	const float MAX_SIZE_Y = 64.0f;			//初期の画像サイズ_Y

	bool isChanged = false;
	int timerCount = 0;
	float size_x = MAX_SIZE_X;
	float size_y = MAX_SIZE_Y;
	//ステージ選択の画面に行く
	bool isModeSelect = false;

	float flagss;
	Model numbersObject[10];
	Model sNumbersObject[10];
	Model reverseObject;
	Model scoreObject;

	EachInfo reversEach[2];
	EachInfo timerEach[2];
	EachInfo scoreEach[6];
	bool isTipsOk = false;
	
	static const int tipsCount = 5;
	Sprite tipsSprite[tipsCount];

	int tipsNumber;
};