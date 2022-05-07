#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include <list>
#include "../Sprite/Sprite.h"
#include "../NormaChecker/NormaChecker.h"
enum OthelloType
{
	NORMAL,
	HARF,
	NONE,
};
namespace
{
	struct panelPos
	{
		int x;
		int y;
	};
	struct panelData
	{
		panelPos pos;
		bool isFront;
	};
	struct NormaModeFieldData
	{
		std::list<panelData> panels;
		panelPos playerPos;
		Norma::NormaType type = Norma::Panels;
		int normaStatus = 0;
		int normaMoveCount = 0;
	};
}

namespace TutorialSceneFlow
{
	enum TutorialScene
	{
		SandwichSpawn,
		SandwichUpdate,
		ChainSpawn,
		ChainUpdate,
		StepSpawn,
		StepUpdate,
		TutorialEnd
	};

	enum TutorialFlow
	{
		FirstText,
		FirstPlay,
		SecondText,
		SeconDPlay,
		ThirdText,
		ThirdPlay,
		FlowEnd
	};
}
struct OthelloData
{
	//配列にぶち込む
	int widthPos = 0;
	int heightPos = 0;


	//専用のタイプが必要かもしれない
	OthelloType type = NONE;


	//表 = 青
	bool isFront = true;

	//特殊な向きが出てきたら
	bool isUp = true;
	bool isRight = true;

	//大賞の向き
	list<bool> FrontActiveAngle;

	//おれか俺じゃないか
	bool isPlayer = false;


	bool isMove = false;


	list<panelPos> panels;
	OthelloData();

	int comboCount = 0;
	bool isReverce = false;
	bool isSandwich = false;

	int animationTimer = 0;
	int waitTimer = 0;
	int JumpTimer = 0;
	bool isJumpUp = false;

	//反転をおこなったか
	bool isVanish = false;

	//消滅しきったか
	bool isDead = false;

	bool isHarf = false;

	panelPos oldPos = { 0, 0 };
	int vanishTimer = 0;
	vector<int> SandwichLength;

	int score = 0;

	bool isSpawn = false;
	int spawnTimer = 0;
	int spawnDerayTimer = 0;

	bool isShake = false;
	int chainName = 0;
	int maxComboCount = 0;
};
struct SendOthelloData
{
	OthelloType type = NONE;
	bool isFront = true;
	bool isMove = false;
	bool isSandwich = false;
	bool isCheckEnd = false;
	bool isOnPlayer = false;
	bool isChainVanish = false;	//連鎖で再度消えた駒が持つフラグ
	list<bool> FrontActiveAngle;
	vector<int> SandwichLength;
	int comboCount = 0;
	int score = 0;
	int chainName = 0;
	int maxComboCount = 0;

};

namespace OthelloConstData
{//定数
	const int fieldSize = 8;
	const float cellScale = 1.0f;
	const XMFLOAT3 stageLeftTop{ -cellScale * fieldSize, cellScale * fieldSize, 0 };
	const int spawnTimerMAx = 300;
	const int spawnMoveCount = 100;
	const int spawnPanelCount = 2;
	const int minPanelCount = 20;
	const int chainRepairTime = 120;
	const int saveTimerLimit = 180;
	const int tutorialTimerLimit = 180;
	const int SpawnDerayTimerMax = 60;

	const float PanelSize = 10.0f;

	const float panelWallRate = 0.3f;
	const int downStepCountMax = 0;

	//アニメーション
	const int vanishTimerMax = 600;
	const int animationTimerMax = 30;
	const int waitTimerMax = 30;
	const int JumpTimerMax = waitTimerMax / 2;
	const int SpawnAnimationTimerMax = 120;
	const int downStepTimerMax = 60;
}

class OthelloEachInfo : public EachInfo
{
public:
	float alpha;
	void CreateConstBuff0();
};

class OthelloModel : public Model
{
public:
	void Update(OthelloEachInfo *each);
};

class OthelloConstBuffer
{
public:
	XMMATRIX viewproj;//ビュープロジェクション行列
	XMMATRIX world;//ワールド行列
	XMFLOAT3 cameraPos;//カメラ座標
	float flash;
};

class Othello
{
public:

private:
	OthelloEachInfo each;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	OthelloModel *model;
	float startAngle;
	float endAngle;
	float time;
	bool isEase;
	OthelloData data;
public:
	OthelloData *GetGameData() { return &data; }
	bool GetIsEase() { return isEase; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	void SetIsEase(bool isEase) { this->isEase = isEase; }
	void SetPosition(XMFLOAT3 &position) { this->each.position = ConvertXMFLOAT3toXMVECTOR(position); }

public:
	void Init(OthelloModel *model);
	void Update();
	void Draw();
	void Finalize();

	void Spawn(OthelloType type, int x, int y, bool isFront = true);

	void Borne(OthelloType type, int x, int y, bool isFront = true);
	void Controll(const XMFLOAT3 &mousePos, int &moveCount);

	//ひっくり返す
	void Revers();

	void Sandwich();
	void SinkWait();
	void Sink();

	void MakeParticle();

	bool GetIsActive();

	void SpawnUpdate();
private:
	void ReversUpdate();
	void LeftRevers();
	void Shake();
};

class ChanceModel : public OthelloModel
{

};
class ChanceObject
{
public:
	void Init(ChanceModel *model);
	void Update();
	void Draw();
	void Finalize();

	void Spawn(int x, int y, bool isFront = true);
	void SetModel(ChanceModel *model) { this->model = model; }
public:
	OthelloEachInfo each;
	bool isFront = false;
private:
	ChanceModel *model;
};

class OthelloManager
{
public:
	void Init();
	void Update();
	void TutorialUpdate();
	void Draw();
	void Finalize();
	void Controll();
	void AddPanel();

	const vector<vector<SendOthelloData>> &Send();
	void Receive(const vector<vector<SendOthelloData>> &data);

	void MinSpawn();
	static list<Othello> othellos;
	static list<NormaModeFieldData> NormaStartOthellos;

	static list<ChanceObject> chances;
	void DeadPanel();

	void StartSetPos();
	void AllDeadPanel();
	bool GetIsSendDataUpdate();
public:
	//挟むチュートリアルでーす
	void whySandwichSpawn();

	//連鎖のチュートリアルだよ～ん
	void whyChainSpawn();

	//落ちてしまった場合の復帰方法だよ
	void whyStepSpawn();

	void TutorialTextDraw();

	bool IsTutorialEnd();

public://ノルマモード用関数
	void NormaUpdate();
	void SetNormaMove();
	void Undo();
	void StartNormaMode(Norma::NormaType normaType = Norma::Combo, int normaStatus = 0, int normaMoveCount = 0);
	void EndNormaMode();
	void RestartNorma();
	bool GetIsNormaClear();
	bool GetIsNormaFailed();
	bool isNormaMode = true;

private://ノルマモード用内部処理関数
	void StartNormaField();
	void TestStage();
private:
	void SetPlayer();

	void KeySetPlayer();
	void Move(const XMFLOAT3 &MousePos);
	void RemovePlayer();
	void SetPanel();

	void RandumSetPanel();

	void SpawnPanel(bool isInGame);

	void MauseControll();
	void PlayerControll();


	void playerMoveEnd();

	void playerNotMove();

	void OthelloDraw();
private://チャンスオブジェクト

	void SetChanceObject(int x, int y, bool Front);
	void ChanceDraw();

private:

	/// <summary>
	/// 自由に動き放題
	/// </summary>
	void TypeA(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// 消えてる途中の駒には乗れない
	/// </summary>
	void TypeB(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// 消えてる途中の駒の大きさが小さくなると通れなくなる
	/// </summary>
	void TypeC(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// TypeCの逆
	/// </summary>
	void TypeD(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	/// <summary>
	/// XI[sai]のように消えかけの駒から登れる
	/// </summary>
	void TypeXI(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	void TypeUp(list<Othello>::iterator playerItr, list<Othello>::iterator nextItr, int x, int y);

	void DownStep(list<Othello>::iterator playerItr);

	void DownStepReset() { downStepCount = 0; downStepTimer = 0; }

	void SaveSpawn();

private:

	//プレイヤーとパネルを特定のマスに配置する処理
	void SetPlayerAndPanel(int x, int y, bool Front);

	void SetSpawnPlayer(int x, int y);

	//特定のマスにパネルを配置する
	void SetSpawnPanel(int x, int y, bool Front);

private:
	panelPos playerPanelPos;
	int spawnTimer = 0;
	int moveCount = 0;

	int downStepTimer = 0;
	int downStepCount = 0;
	int saveTimer = 0;
	XMFLOAT3 mousePoint;

	static OthelloModel oserroModel;
	static ChanceModel chanceModelBlue;
	static ChanceModel chanceModelOrange;
	static vector<vector<SendOthelloData>> sendDatas;

	bool isOnPanel = true;
	bool isOldOnPanel = true;

	bool isPanelMove = false;
	bool isPlayerEnd = true;

	bool isFieldUpdate = false;

	TutorialSceneFlow::TutorialScene scenes = TutorialSceneFlow::TutorialEnd;

	int TutorialTimer = 0;
	bool isTutorialClear = true;

	Sprite TutorialText1;
	Sprite TutorialText2;
	Sprite TutorialText3;
	Sprite TutorialText4;
	Sprite TutorialText5;
	Sprite CongraturationText;
	Sprite TutorialRetryText;
	Sprite back;

	int textChangeTimer = 0;
	bool textChange = false;

	NormaChecker normaChecker;

};