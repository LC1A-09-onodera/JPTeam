#pragma once
#include "../3DModel/Model.h"
#include "../DX12operator.h"
#include <list>
#include "../Sprite/Sprite.h"
#include "../NormaChecker/NormaChecker.h"
enum OthelloType
{
	NORMAL,
	WALL,
	STOP,
	NONE,
};

//ゲームシーン側から送る合図
enum GameMode
{
	NoneGameMode,
	NormaMode,
	ScoreAttack,
	Dojo,
	Tutorial,
};

//モード選択のシーン
enum ModeSelectType
{
	GameModeSelect,
	DojoSelect,
	PuzzleSelect,
};

//道場の内容
enum DojoType
{
	NoneDojo,
	Conect,
	Waltz,
	Bookend,
};

//Connectの全体フロー
enum ConectFlow
{
	Connect1,//開始
	Connect2,//コンボして壁が立つ
	Connect3,//コンボ３
	Connect4,
	Connect5,
	ConnectEnd,
};
enum WaltzFlow
{
	Waltz1,
	Waltz2,
	Waltz3,
	Waltz4,
	Waltz5,
	Waltz6,
	Waltz7,
	Waltz8,
	WaltzEnd,
};

enum BookendFlow
{
	BookEnd1,//開始
	BookEnd2,
	BookEnd3,
	BookEnd4,
	BookEnd5,
	BookEnd6,
	BookEndEnd,
};

enum ReversibleFlow
{
	Reversible0,//開始
	ReversibleEnd,
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
		OthelloType type = NORMAL;
	};
	struct NormaModeFieldData
	{
		std::list<panelData> panels;
		panelPos playerPos;
		Norma::NormaType type = Norma::Panels;
		int normaStatus = 0;
		bool subNormaFlag = false;
		int subNormaPanels = 0;
		int normaMoveCount = 0;
	};

}

namespace OthelloEase
{
	struct WaveEase
	{
		int timer = 0;
		int MaxTime = 120;
		bool isWaveUp = false;
		WaveEase(int timer = 0, int  Max = 0, bool isWaveUp = true);
		void Set(int timer = 0, int  Max = 0, bool isWaveUp = true);
	};

	float Wave(WaveEase &obj, int powCount);
	float WaveInOut(WaveEase &obj, int powCount);
}
bool operator ==(const panelPos &a, const panelPos &b);
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
	const int minPanelCount = 25;
	const int chainRepairTime = 0;
	const int saveTimerLimit = 180;
	const int tutorialTimerLimit = 180;
	const int spawnDerayTimerMax = 60;

	const float PanelSize = 10.0f;

	const float panelWallRate = 0.0f;
	const int downStepCountMax = 0;
	const int tutorialEndTextTimer = 300;

	const int NormaStageCount = 56;
	//アニメーション
	const int vanishTimerMax = 1200;
	const int vanishAnimation = 180;
	const int vanishWaitTimer = vanishTimerMax - vanishAnimation;
	const int animationTimerMax = 30;
	const int waitTimerMax = 30;
	const int jumpTimerMax = waitTimerMax / 2;
	const int spawnAnimationTimerMax = 120;
	const int downStepTimerMax = 60;

	namespace Breath
	{
		const int breathSpan = 10;
		const int breathLevelTimerMax = 60;
	};
	const panelPos normaMode = { 0, 0 };
	const panelPos ScoreAttackPanel = { 1, 5 };
	const panelPos tutorialPanel = { 1, 2 };
	const panelPos DojoPanel = { 6, 2 };
	const panelPos UndoPanel = { 3, 7 };
	const panelPos ConectPanel = { 0, 4 };
	const panelPos WaltzPanel = { 3, 4 };
	const panelPos BookendPanel = { 6, 4 };
	const panelPos TutorialStartPanel = {3, 3};
	const panelPos TutorialSecondPos = {0, 0};
	//テキストの表示箇所2D
	const XMVECTOR moveTextPos = { 400, 0, 0,0 };
	const XMVECTOR moveAllDeleteCountPos{ 5.0f, 0.0f, 0.0f , 0.0f };
	const XMVECTOR moveSubTextPos = { 105, 50, 0,0 };
	//テキストの表示箇所3D
	const XMVECTOR comboScoreModelPos{ -1.6f, 13.0f, -1.0f ,0 };//コンボとスコアの文字の基準位置
	//const XMVECTOR allDeleteModelPos{ -3.0f, 13.0f, -1.0f ,0 };//ゼンブハサムの文字の基準位置
	const XMVECTOR allDeleteModelPos{ 0.0f, 13.0f, -1.0f ,0 };//ゼンブハサムの文字の基準位置

	const XMVECTOR moveTextModelPos{ 15.0f, 0.0f, 0.0f ,0.0f };//ノルマモードに入る際の移動量
	const XMVECTOR moveSubTextModelPos{ 4.0f, -5.0f, 0.0f ,0.0f };//サブノルマの移動量
	const XMVECTOR scoreAttackTextPos{ 0.0f, 15.0f, -1.0f ,0 };//スコアアタックの文字の基準位置
	const XMVECTOR FloatAnimationDistance{ 0.0f, 1.0f, 0.0f ,0.0f };

	const float textBaseScale = 0.3f;
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

class TextModel : public Model
{
public:
	void Update(EachInfo *each, XMMATRIX *addRot = nullptr);
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
	bool isCompleteDraw = false;
	bool isAlpha = false;
private:
	OthelloEachInfo each;
	OthelloEachInfo completeEach;
	XMFLOAT3 startPos;
	XMFLOAT3 endPos;
	OthelloModel *model;
	OthelloModel *chainModel;
	OthelloModel *compModel;
	float startAngle;
	float endAngle;
	float time;
	bool isEase;
	bool isRockDraw = true;
	OthelloData data;
	OthelloEase::WaveEase compEase;
public:
	OthelloData *GetGameData() { return &data; }
	bool GetIsEase() { return isEase; }
	XMFLOAT3 GetPosition() { return ConvertXMVECTORtoXMFLOAT3(each.position); }
	void SetIsEase(bool isEase) { this->isEase = isEase; }
	void SetIsRockDraw(bool isRockDraw) { this->isRockDraw = isRockDraw; }
	void SetPosition(XMFLOAT3 &position) { this->each.position = ConvertXMFLOAT3toXMVECTOR(position); }
	void SetScale(XMFLOAT3 &scale) { this->each.scale = scale; }
public:
	void Init(OthelloModel *model, OthelloModel *chainModel, OthelloModel *compModel);
	void Update(int combo, bool isNotErase);
	void Draw();
	void CompDraw();
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
private://アニメーション
	void SinkAnimation();
	void BreathAnimation();
	bool BreathAnimationFlag = true;
	int breathTimer = 5;//初期値
	int breathLevelTimer = 0;
	int breathLevel = 0;
private:
	void ReversUpdate(int combo);
	void LeftRevers();
	void Shake();
	void NormalUpdate();
	void WallUpdate();
	void StopUpdate();
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
	bool isDraw = false;
private:
	ChanceModel *model;
};

class OthelloManager
{
public:
	void Init(Tex num[10], Model numModel[10]);
	void Update(int combo);
	void Draw(bool isChanceDraw = true, bool isCompDraw = true);
	void Finalize();
	void Controll();

	const vector<vector<SendOthelloData>> &Send();
	void Receive(const vector<vector<SendOthelloData>> &data, const vector<pair<int, int>> compPos);

	void MinSpawn(bool inGame);
	static list<Othello> othellos;
	static list<NormaModeFieldData> NormaStartOthellos;
	void SpawnChances(const vector<pair<int, int>> &pos);
	static vector<ChanceObject> chances;
	void DeadPanel();

	void StartSetPos();
	void AllDeadPanel();
	void AllDeletePanel();
	bool GetIsSendDataUpdate();
	void EraseChanceObject();

	bool tutorialEndFlag = false;
public://チュートリアルの関数
	void TutorialUpdate(int combo);


	void TutorialTextDraw();

	bool IsTutorialEnd();

	std::list<XMFLOAT3> &GetPressPanellPos();
	void TutorialStart();
private:
	void whySandwichSpawn();

	void whyChainSpawn();

	void whyStepSpawn();
private:
	bool A();
public://ノルマモード用関数
	void NormaUpdate(int combo);
	void SetNormaMove();
	void Undo();
	void StartNormaMode(int stageNum = 0);
	void EndNormaMode();
	void RestartNorma();
	bool GetIsNormaClear();
	bool GetIsNormaFailed();
	bool isNormaMode = false;
	NormaChecker normaChecker;
	void SetScore(const int score) { nowScore = score; }

	void NormaTextDraw(int stageNum, bool isDraw);
	void NormaTextModelDraw(int stageNum, bool isDraw);
	int GetNormaStagesCount();
	bool GetIsAct() { return isAct; }
	list<NormaModeFieldData>::iterator GetNormaStage(int num);
private://ノルマモード用内部処理関数
	void StartNormaField(int stageNum = 0);
	void TestStage();
	void LoadNormaStage(std::string stage, list<NormaModeFieldData> &stageList);
	void LoadAllNormaStage();
	void CountTextDraw(int count);
	void CountModelDraw(int count, EachInfo *data = nullptr, EachInfo *dataB = nullptr);
public://モードセレクト用外部関数
	//モード選択開始時
	void ModeSelectStart(int stageNum = -1, ModeSelectType type = GameModeSelect);

	void ModeSelectControll();
	//モード選択中
	void ModeSelectUpdate();
	//モード選択決定した瞬間
	bool InMode();
	//モード選択内容(ゲームモード)
	GameMode GetEnterModeType();
	//モード選択内容(ノルマステージ)
	int GetEnterNormaStage();
	//何かしら描画があるなら
	void ModeSelectDraw(bool isDraw);
	void ModeSelectModelDraw(bool isDraw);
	void SetGameModeSelectPanel(int startPos = -1);
	void SetPuzzleModeSelectPanel(int startPos = -1);
	void SetDojoModeSelectPanel(int startPos = -1);
private://モードセレクト用変数:
	vector<EachInfo> fieldDrawText;
	ModeSelectType nowType = GameModeSelect;
	ModeSelectType oldType = nowType;
private://モードセレクト用内部関数
	void SelectUpdateGameMode();
	void SelectUpdateDojo();
	bool SelectDojoPanel();
	void SelectUpdatePazzle();
private://描画の配置
	void NormaComboTextSetPos(bool isNormaMode);
	void NormaPanelsTextSetPos(bool isNormaMode);
	void NormaScoreTextSetPos(bool isNormaMode);
	void SetCountPos(bool isNormaMode);
	void SubNormaTextPos(bool isNormaMode);
	void SetTextPos(bool isNormaMode, int stageNum = 1);


	void NormaComboModelSetPos(bool isNormaMode);
	void NormaPanelsModelSetPos(bool isNormaMode);
	void NormaScoreModelSetPos(bool isNormaMode);
	void SetCountModelPos(bool isNormaMode, int stageNum);
	void SetScoreAttackTextPos();
	void SubNormaModelPos(bool isNormaMode);
	void SetModelPos(bool isNormaMode);

	void SetModeSelectEachInfo(EachInfo &data, const panelPos &pos);
	void SetPickupModeEachInfo(EachInfo &data);
public:
	void DojoStart();
	void DojoUpdate(int combo);
	void DojoDraw();
	bool DojoEnd();
	bool GetDojoIsStart();
	DojoType dojoType;
private://道場変数
	
	//テキスト読みの時間と実行の時間
	bool isAct = true;
	int TextCount = 0;
private:
	void DojoObjectDraw();

#pragma region connect
	void ConnectStart();
	void ConnectUpdate(int AlivePanel, int ComboCount, bool isOn = true);
	void ConnectDraw();
	ConectFlow	nowConnectFlow;
	vector<Othello> ConnectObject;
	vector<Sprite> ConnectText;
	void ConnectFirstSpwan();
	void ConnectFirstUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void ConnectSecondSpawn();
	void ConnectSecondUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void ConnectThirdSpawn();
	void ConnectThirdUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void ConnectForceSpawn();
	void ConnectForceUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void ConnectFifthSpawn();
	void ConnectFifthUpdate(int AlivePanel, int ComboCount, bool isOn = true);
#pragma endregion

#pragma region Waltz
	void WaltzStart();
	void WaltzUpdate(int AlivePanel, int ComboCount, bool isOn = true);
	void WaltzDrwa();
	WaltzFlow	nowWaltzFlow;
	vector<Othello> WaltzObject;
	vector<Sprite> WaltzText;
	void WaltzFirstSpwan();
	void WaltzFirstUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void WaltzSecondSpawn();
	void WaltzSecondUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void WaltzThirdSpawn();
	void WaltzThirdUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void WaltzForceSpawn();
	void WaltzForceUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void WaltzFifthSpawn();
	void WaltzFifthUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void WaltzSixesSpawn();
	void WaltzSixesUpdate(int AlivePanel, int ComboCount, bool isOn = true);
	void WaltzSevensSpawn();
	void WaltzSevensUpdate(int AlivePanel, int ComboCount, bool isOn = true);

#pragma endregion

#pragma region Bookend
	void BookendStart();
	void BookendUpdate(int AlivePanel, int ComboCount, bool isOn = true);
	void BookendDrwa();
	BookendFlow	nowBookendFlow;
	vector<Othello> BookendObject;
	vector<Sprite> BookendText;
	void BookendFirstSpwan();
	void BookendFirstUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void BookendSecondSpawn();
	void BookendSecondUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void BookendThirdSpawn();
	void BookendThirdUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void BookendForceSpawn();
	void BookendForceUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void BookendFifthSpawn();
	void BookendFifthUpdate(int AlivePanel, int ComboCount, bool isOn = true);

	void BookendSixesSpawn();
	void BookendSixesUpdate(int AlivePanel, int ComboCount, bool isOn = true);
#pragma endregion

#pragma region tutorial
	vector<Sprite> tutorialTexts;
	Othello tutorialOthello;
	bool isTutorialDraw = false;
#pragma endregion

private:
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
	void CompDraw();
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
	void SetSpawnPanel(int x, int y, bool Front, OthelloType type = NORMAL, bool isRockDraw = true, bool isSpawn = true);

private:
	panelPos playerPanelPos;
	panelPos nextMovePos = {};
	int spawnTimer = 0;
	int moveCount = 0;

	int downStepTimer = 0;
	int downStepCount = 0;
	int saveTimer = 0;
	int TutorialEndTextCount = 0;
	int nowScore = 0;
	XMFLOAT3 mousePoint;

	static OthelloModel oserroModel;
	static OthelloModel stopOserroModel;
	static OthelloModel wallOserroModel;
	static OthelloModel compModel;
	static ChanceModel chanceModelBlue;
	static ChanceModel chanceModelOrange;

	static TextModel PanelTextModel;
	static TextModel ComboTextModel;
	static TextModel ScoreTextModel;
	static TextModel ScoreAttackTextModel;
	static TextModel NormaModeTextModel;

	static TextModel ConnectModel;
	EachInfo ConnectTextDrawData;

	static TextModel WaltzModel;
	EachInfo WaltzTextDrawData;

	static TextModel BookendModel;
	EachInfo BookendTextDrawData;

	static TextModel TechTrainingModel;
	EachInfo TechTrainingTextDrawData;

	static TextModel UndoModel;
	EachInfo UndoTextDrawData;

	static TextModel TutorialModel;
	EachInfo TutorialTextDrawData;

	static vector<vector<SendOthelloData>> sendDatas;

	bool isOnPanel = true;
	bool isOldOnPanel = true;

	bool isPanelMove = false;
	bool isPlayerEnd = true;

	bool isFieldUpdate = false;

	bool isDojoDraw = false;
	TutorialSceneFlow::TutorialScene scenes = TutorialSceneFlow::TutorialEnd;

	int TutorialTimer = 0;
	bool isTutorialClear = true;

	Sprite TutorialText1;
	Sprite TutorialText2;
	Sprite TutorialText3;
	Sprite TutorialText4;
	Sprite TutorialText5;
	Sprite TutorialText6;
	Sprite CongraturationText;
	Model exselent;
	Sprite back;
	Sprite normaCountTex[50];

	vector<Model *>numberModel;

	vector<EachInfo> CountDrawData;
	EachInfo NormaDrawData;
	EachInfo SubNormaDrawData;
	EachInfo ScoreAttackDrawData;
	EachInfo NormaModeTextDrawData;

	Sprite NormaPanelsText;
	Sprite NormaComboText;
	Sprite NormaScoreText;

	//////////////////////Todo
	OthelloEase::WaveEase textEase;
	float textAnimationRate = 0;
	int textChangeTimer = 0;
	bool textChange = false;


	std::list<XMFLOAT3> pressPos;
};