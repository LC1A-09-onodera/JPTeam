#include "Player.h"
#include "../BaseDirectX/Input.h"
#include "../Enemy/Enemy.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"
#include "../Shader/ShaderManager.h"
#include "../Hole/Hole.h"
#include "../imgui/ImguiControl.h"

Player::Player()
{
	pos = { 0,0,0 };				//プレイヤーの座標
	vec3 = { 0,0,0 };				//向いている方向（正規化済）
	hitEnemypos = { 0,0,0 };		//当たった敵の座標
	hitBombpos = { 0,0,0 };			//爆風が当たった時の爆弾の座標
	//lastVec3 = { 0,0,0 };			//最後に向いていた方向
	firstButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	secondButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	activeCount = 0;				//行動不能カウント
	invincibleCount = 0;			//無敵カウント
	hp = MAX_HP;					//playerのHP
	bombForce = 0.0f;				//ボムの力保存用
	enemyForce = 0.0f;				//敵の力保存用
	isActive = true;				//行動できるかどうか
	isHitBomb = false;				//ボムに当たって飛ばされてるかどうか
	isHitEnemy = false;				//敵に当たって飛ばされてるかどうか
	isInvincible = false;			//無敵かどうか
	isShoot = false;				//射撃中かどうか（弾があるか）
	isDetonating = false;			//起爆したかどうか
}

Player::~Player()
{
}

Player* Player::GetPlayer()
{
	static Player player;
	return &player;
}

//初期化
void Player::Init()
{
	player.CreateModel("Block", ShaderManager::playerShader);
	//water.CreateWater();

	pos = { 0,0,0 };				//プレイヤーの座標
	vec3 = { 0,0,0 };				//向いている方向（正規化済）
	hitEnemypos = { 0,0,0 };		//当たった敵の座標
	hitBombpos = { 0,0,0 };			//爆風が当たった時の爆弾の座標
	//lastVec3 = { 0,0,0 };			//最後に向いていた方向
	firstButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	secondButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	activeCount = 0;				//行動不能カウント
	invincibleCount = 0;			//無敵カウント
	hp = MAX_HP;					//playerのHP
	bombForce = 0.0f;				//ボムの力保存用
	enemyForce = 0.0f;				//敵の力保存用
	isActive = true;				//行動できるかどうか
	isHitBomb = false;				//ボムに当たって飛ばされてるかどうか
	isHitEnemy = false;				//敵に当たって飛ばされてるかどうか
	isInvincible = false;			//無敵かどうか
	isShoot = false;				//射撃中かどうか（弾があるか）
	isDetonating = false;			//起爆したかどうか
}

void Player::Restart()
{
	pos = { 0,0,0 };				//プレイヤーの座標
	vec3 = { 0,0,0 };				//向いている方向（正規化済）
	hitEnemypos = { 0,0,0 };		//当たった敵の座標
	hitBombpos = { 0,0,0 };			//爆風が当たった時の爆弾の座標
	//lastVec3 = { 0,0,0 };			//最後に向いていた方向
	firstButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	secondButtonVec3 = { 0,0,0 };	//ボタン入力保存用
	activeCount = 0;				//行動不能カウント
	invincibleCount = 0;			//無敵カウント
	hp = MAX_HP;					//playerのHP
	bombForce = 0.0f;				//ボムの力保存用
	enemyForce = 0.0f;				//敵の力保存用
	isActive = true;				//行動できるかどうか
	isHitBomb = false;				//ボムに当たって飛ばされてるかどうか
	isHitEnemy = false;				//敵に当たって飛ばされてるかどうか
	isInvincible = false;			//無敵かどうか
	isShoot = false;				//射撃中かどうか（弾があるか）
	isDetonating = false;			//起爆したかどうか
}

//更新
void Player::Update(bool isBombAlive)
{
	/*HPリセット*/
	if (Input::KeyTrigger(DIK_R)) { Init(); }

	/*HP判定*/
	if (!CheakHP()) { return; }

	/*移動*/
	CheakIsInput();
	//行動可能かつ吹っ飛んでなければ
	CheakIsActive();

	/*発射トリガー*/
	CheakShootTrigger(isBombAlive);

	/*起爆トリガー*/
	CheakDetonatingTrigger(isBombAlive);

	/*自機が敵に当たった時の判定*/
	CheakHitEnemy();

	/*自機が穴に当たった時の判定*/
	//CheakHitHole();

	/*自機が周りの線に当たった時の判定*/
	CheakHitDeathLine();

	/*行動不能とか無敵とか*/
	CalcActiveCount();

	/*ボムの力計算*/
	AddBombForce();

	/*敵の力計算*/
	AddEnemyForce();

	player.each.position = ConvertXMFLOAT3toXMVECTOR(pos);
	player.Update();
}

//描画
void Player::Draw()
{
	if (isActive)
	{
		if (!isInvincible) { Draw3DObject(player); }
		else if (invincibleCount % 2 == 0) { Draw3DObject(player); }
	}
	else
	{
		if (activeCount % 3 == 0) { Draw3DObject(player); }
	}
}

void Player::HitBomb(const float& BombForce, XMFLOAT3 bombPos)
{
	if (isActive)
	{
		//ボムの力を代入
		bombForce = BombForce;

		hitBombpos = bombPos;

		bombForce *= 3.0f;

		//移動制限
		isActive = false;

		//吹っ飛び中にする
		isHitBomb = true;

		hp--;
	}
}

void Player::CheakIsInput()
{
	/*移動*/
	//入力なければリセット
	if (DirectInput::leftStickX() == 0.0f &&
		DirectInput::leftStickY() == 0.0f)
	{
		vec3 = { 0,0,0 };
	}
	else
	{
		vec3.x = DirectInput::leftStickX();
		vec3.z = DirectInput::leftStickY();

		//lastVec3 = vec3;
		//VecNormaliz(lastVec3);
	}

	if (!Input::Key(DIK_A) &&
		!Input::Key(DIK_D) &&
		!Input::Key(DIK_W) &&
		!Input::Key(DIK_S))
	{
		if (DirectInput::leftStickX() == 0.0f &&
			DirectInput::leftStickY() == 0.0f)
		{
			vec3 = { 0,0,0 };
		}
	}
	else
	{
		if (DirectInput::leftStickX() == 0.0f &&
			DirectInput::leftStickY() == 0.0f)
		{
			if (Input::Key(DIK_A)) { vec3.x = -1.0f; }
			if (Input::Key(DIK_D)) { vec3.x = 1.0f; }
			if (Input::Key(DIK_W)) { vec3.z = -1.0f; }
			if (Input::Key(DIK_S)) { vec3.z = 1.0f; }

			//lastVec3 = vec3;
			//VecNormaliz(lastVec3);
		}
	}
}

void Player::CheakIsActive()
{
	//行動可能かつ吹っ飛んでなければ
	if (isActive && !isHitBomb)
	{
		pos.x += vec3.x * MAX_SPEED;
		pos.z += -vec3.z * MAX_SPEED;
	}
}

void Player::CheakShootTrigger(bool isBombAlive)
{
	/*発射トリガー(A->B->X->Y)*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_DOWN))
	{
		if (firstButtonVec3.x != 0 && firstButtonVec3.z != 0) { firstButtonVec3 = { 0,0,-1 }; }
	}
	else { isShoot = false; }
}

void Player::CheakDetonatingTrigger(bool isBombAlive)
{
	/*起爆トリガー*/
	if (DirectInput::IsButtonPush(DirectInput::ButtonKind::Button01) ||
		Input::KeyTrigger(DIK_B))
	{
		if (isBombAlive && isActive) { isDetonating = true; }
	}
	else { isDetonating = false; }
}

void Player::CheakHitEnemy()
{
	//行動不能時か無敵時か吹っ飛び中は判定無視
	if (!isActive || isInvincible || isHitBomb) { return; }

	bool isHit = false;
	auto itr = Enemys::enemys.begin();
	for (; itr != Enemys::enemys.end(); ++itr)
	{
		XMFLOAT3 enemyPos = itr->GetPosition();
		if (std::isnan(enemyPos.x)) { return; }
		if (std::isnan(enemyPos.z)) { return; }

		//2点間の距離と判定（円）
		isHit = CheakHit(1.2f, 1.2f, pos, enemyPos);

		//当たってなかったらやり直し
		if (!isHit) { continue; }

		//当たったら行動不能にする
		hitEnemypos = itr->GetPosition();
		hp--;
		isActive = false;
		break;
	}

	//誰とも当たらなかった場合、終了
	if (isActive) { return; }

	enemyForce = MAX_ENEMY_FORCE;
}

void Player::CheakHitHole()
{
	//無敵時は判定無視
	if (isInvincible) { return; }

	bool isHit = false;
	auto itr = Holes::holes.begin();
	for (; itr != Holes::holes.end(); ++itr)
	{
		XMFLOAT3 holePos = itr->GetPosition();
		if (std::isnan(holePos.x)) { return; }
		if (std::isnan(holePos.z)) { return; }

		//2点間の距離と判定（円）
		isHit = CheakHit(1.2f, 1.2f, pos, holePos);

		//当たってなかったらやり直し
		if (!isHit) { continue; }

		//当たったら真ん中でリスポーンして行動不能に
		pos = { 0,0,0 };
		isActive = false;
		break;
	}
}

void Player::CheakHitDeathLine()
{
	/*if (pos.x > Imgui::dethLine ||
		pos.x < -Imgui::dethLine ||
		pos.z > Imgui::dethLine ||
		pos.z < -Imgui::dethLine)
	{
		pos = { 0,0,0 };
		isActive = false;
	}*/

	if (pos.x > Imgui::dethLine) { pos.x = Imgui::dethLine; }
	if (pos.x < -Imgui::dethLine) { pos.x = -Imgui::dethLine; }
	if (pos.z > Imgui::dethLine) { pos.z = Imgui::dethLine; }
	if (pos.z < -Imgui::dethLine) { pos.z = -Imgui::dethLine; }
}

void Player::CalcActiveCount()
{
	/*行動不能とか無敵とか*/
	//無敵かつ行動可能時
	if (isInvincible && isActive)
	{
		if (invincibleCount < MAX_INVICIBLE_COUNT) { invincibleCount++; }
		else { invincibleCount = 0; isInvincible = false; }
	}

	//行動不能時
	else if (!isActive)
	{
		if (activeCount < MAX_STAN_COUNT) { activeCount++; }
		else
		{
			activeCount = 0;
			isInvincible = true;
			isActive = true;
		}
	}
}

void Player::AddBombForce()
{
	//吹っ飛び中だけ計算
	if (!isHitBomb) { return; }

	//移動
	//移動
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitBombpos.x;
	move.z = pos.z - hitBombpos.z;
	VecNormaliz(move);
	pos.x += bombForce * move.x;
	pos.z += bombForce * move.z;

	//減算
	bombForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にして移動再開
	if (bombForce < MINIMUM_FORCE)
	{
		bombForce = 0;
		isActive = true;
		isHitBomb = false;
	}
}

void Player::AddEnemyForce()
{
	//行動不能時だけ計算
	if (isActive) { return; }

	//移動
	XMFLOAT3 move = { 0,0,0 };
	move.x = pos.x - hitEnemypos.x;
	move.z = pos.z - hitEnemypos.z;
	VecNormaliz(move);
	pos.x += enemyForce * move.x;
	pos.z += enemyForce * move.z;

	//減算
	enemyForce -= RESISTANCE_VALUE;

	//一定量を下回ったら0にする
	if (enemyForce < MINIMUM_FORCE) { enemyForce = 0; }
}
