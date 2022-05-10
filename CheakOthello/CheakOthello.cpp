#include "CheakOthello.h"
#include <string>
#include <iterator>
#include "../BaseDirectX/Input.h"
#include "../imgui/ImguiControl.h"

SoundData CheakOthello::comboSound;

CheakOthello::CheakOthello()
{
	last_x = 0;				//最後に動かしたオセロのX座標
	last_y = 0;				//最後に動かしたオセロのY座標
	totalScore = 0;			//合計点
	totalDeleteOthello = 0;	//挟んだ個数の合計
	collectiveCount = 0;	//まとまりの数
	addScore = 0;			//加算されるスコア
	combo = 0;				//コンボ
	side = 0;				//表裏保存
	checkOthello = 0;		//
	isAddScore = 0;			//

	isCombos = false;		//
	isCombosCheck = false;	//
}

CheakOthello::~CheakOthello()
{
}

void CheakOthello::Init()
{
	SoundLoad("Resource/Sound/reverse_4_.wav", comboSound);

	last_x = 0;				//最後に動かしたオセロのX座標
	last_y = 0;				//最後に動かしたオセロのY座標
	totalScore = 0;			//合計点
	totalDeleteOthello = 0;	//挟んだ個数の合計
	collectiveCount = 0;	//まとまりの数
	addScore = 0;			//加算されるスコア
	combo = 0;				//コンボ
	side = 0;				//表裏保存
	checkOthello = 0;		//
	isAddScore = 0;			//

	isCombos = false;		//
	isCombosCheck = false;	//
}

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData, bool isCheck)
{
	//スコアリセット(debug用)
	//if (Input::KeyTrigger(DIK_R)) { totalScore = 0; }

	//最後に動いた駒を判定、保存
	CheckLastMove(othelloData);

	while (1) //多分whileじゃなくても動く
	{
		//判定を取る駒をセット
		pair<int, int> last = GetCheckOthello();
		if (!checkOthello) { break; }

		isAddScore = false;

		//全方位チェック
		/*-----左-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION, last.first, last.second, false);
		/*-----右-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION, last.first, last.second, false);
		/*-----上-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH, last.first, last.second, false);
		/*-----下-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH, last.first, last.second, false);

		/*-----左斜め上-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH, last.first, last.second, false);
		/*-----左斜め下-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH, last.first, last.second, false);
		/*-----右斜め上-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH, last.first, last.second, false);
		/*-----右斜め下-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH, last.first, last.second, false);

		if (totalDeleteOthello > 1) { AddScore(); }
		if (isCombosCheck) { ChangeScoreAndCombo(); }

		//ResetAddScore();
	}

	//効率悪いです
	if (isCheck)
	{
		//全方位チェック
		/*-----左-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION, pPos.first, pPos.second, isCheck);
		/*-----右-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION, pPos.first, pPos.second, isCheck);
		/*-----上-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
		/*-----下-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);

		/*-----左斜め上-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);
		/*-----左斜め下-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
		/*-----右斜め上-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH, pPos.first, pPos.second, isCheck);
		/*-----右斜め下-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH, pPos.first, pPos.second, isCheck);
	}

	while (!sandwichData.empty())
	{
		CheckReachOthello(Direction_X::WEST, NONE_DIRECTION);
		CheckReachOthello(Direction_X::EAST, NONE_DIRECTION);
		CheckReachOthello(NONE_DIRECTION, Direction_Y::SOUTH);
		CheckReachOthello(NONE_DIRECTION, Direction_Y::NOUTH);
		CheckReachOthello(Direction_X::WEST, Direction_Y::NOUTH);
		CheckReachOthello(Direction_X::WEST, Direction_Y::SOUTH);
		CheckReachOthello(Direction_X::EAST, Direction_Y::NOUTH);
		CheckReachOthello(Direction_X::EAST, Direction_Y::SOUTH);

		sandwichData.erase(sandwichData.begin());
		sandwichSide.erase(sandwichSide.begin());
	}
}

void CheakOthello::CheckLastMove(const vector<vector<SendOthelloData>>& othelloData)
{
	//最新状態を入手
	othelloDatas = othelloData;

	//ChainName保存用配列
	vector<int> chainName;

	//その場にオセロが存在するかつ、最後に動かしてたかを判定（←ちょっと効率悪そう）
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			//ChainName保存用変数
			int chainNameCount = 0;

			if (othelloDatas[i][j].isSandwich)
			{
				sandwichData.push_back(make_pair(i, j));
				sandwichSide.push_back(othelloDatas[i][j].isFront);
			}

			if (othelloDatas[i][j].isOnPlayer) { pPos = make_pair(j, i); }

			//その場所が空
			if (othelloDatas[i][j].type == NONE) { continue; }

			//ChainNameチェック
			if (othelloDatas[i][j].chainName != 0)
			{
				if (chainName.empty())
				{
					chainName.push_back(othelloDatas[i][j].chainName);
					chainNameCount = othelloDatas[i][j].chainName;
				}
				else
				{
					if (!VectorFinder(chainName, chainNameCount)) { chainName.push_back(chainNameCount); }
				}
			}

			//最後に動かしてるやつ
			if (!othelloDatas[i][j].isMove) { continue; }

			othelloSide.push_back(othelloDatas[i][j].isFront);
			comboOthelloDataPos.push_back(std::make_pair(i, j));
			//break;
		}
	}

	//纏まりのカウントを計算
	collectiveCount = chainName.size();
}

pair<int, int> CheakOthello::GetCheckOthello()
{
	if (!comboOthelloDataPos.empty())
	{
		int x, y;
		y = comboOthelloDataPos.front().first;
		x = comboOthelloDataPos.front().second;
		comboOthelloDataPos.erase(comboOthelloDataPos.begin());

		side = othelloSide.front();
		othelloSide.erase(othelloSide.begin());
		checkOthello = true;
		return make_pair(x, y);
	}
	else { checkOthello = false; return make_pair(0, 0); }
}

void CheakOthello::OthelloCheck(int direction_x, int direction_y, int last_x, int last_y, bool isCheck)
{
	int loop = 0;
	int count_x = last_x;
	int count_y = last_y;
	int pair_x = last_x;	//loop内
	int pair_y = last_y;	//loop内

	int max_size;
	if (MAX_SIZE_X > MAX_SIZE_Y) { max_size = MAX_SIZE_X; }
	else { max_size = MAX_SIZE_Y; }

	int side = othelloDatas[count_y][count_x].isFront;

	while (1)
	{
		if (!isCheck)
		{
			totalDeleteOthello = 0;

			//次のマス
			count_x += direction_x;
			count_y += direction_y;

			//範囲外
			if (count_x < 0 || count_x > OthelloConstData::fieldSize - 1) { break; }
			if (count_y < 0 || count_y > OthelloConstData::fieldSize - 1) { break; }

			//存在しない
			if (othelloDatas[count_y][count_x].type == NONE) { break; }
			//同色と隣接
			else if (othelloDatas[count_y][count_x].isFront == side && loop == 0) { break; }
			//挟んだ場合
			else if (othelloDatas[count_y][count_x].isFront == side && loop != 0)
			{
				if (isCombos) { isCombosCheck = true; }
				isCombos = true;
				SoundStopWave(comboSound);
				SoundPlayOnce(comboSound);
				totalDeleteOthello += loop;
				//全部探索リストに入れる（自分と挟んだ駒まで）←sandwichArrayが同じかも参照する
				//挟んだやつの最大のconboCountを取得→それ+1したのをコンボしたオセロにセット
				int maxComboCount = 0;
				int baseScore = 0;
				int maxChainNameCount = 0;
				vector<int> maxChainName;
				bool isActiveOthello = false;

				for (int i = 0; i <= loop + 1; i++)
				{
					//そのオセロのコンボ数を取得、そのオセロが何コンボ目かを調べる
					if (maxComboCount <= othelloDatas[pair_y][pair_x].comboCount)
					{
						maxComboCount = othelloDatas[pair_y][pair_x].comboCount;
					}

					//そのオセロのスコアを取得、そのオセロのスコアを調べる
					if (baseScore < othelloDatas[pair_y][pair_x].score)
					{
						baseScore += othelloDatas[pair_y][pair_x].score;
					}

					//何個で挟んだかを保存、違う数で挟んだ場合push_back
					if (!othelloDatas[pair_y][pair_x].SandwichLength.empty())
					{
						//数字が見つからなかったら保存
						if (!VectorFinder(othelloDatas[pair_y][pair_x].SandwichLength, loop + 2))
						{
							othelloDatas[pair_y][pair_x].SandwichLength.push_back(loop + 2);
						}
					}
					//空だったらそのまま保存
					else { othelloDatas[pair_y][pair_x].SandwichLength.push_back(loop + 2); }

					pair_x += direction_x;
					pair_y += direction_y;
				}

				//初期化
				pair_x = last_x;
				pair_y = last_y;

				//まだ挟まれてないオセロがあったらコンボを加算できる状態に変更
				for (int i = 1; i <= loop; i++)
				{
					pair_x += direction_x;
					pair_y += direction_y;

					//コンボを加算できる状態に変更
					if (!othelloDatas[pair_y][pair_x].isSandwich) {
						isActiveOthello = true;
					}
				}

				//初期化
				pair_x = last_x;
				pair_y = last_y;

				//isSandwichをチェック
				for (int i = 0; i <= loop + 1; i++)
				{
					//挟む、挟まれるオセロを挟んだ状態に変更
					othelloDatas[pair_y][pair_x].isSandwich = true;

					pair_x += direction_x;
					pair_y += direction_y;
				}

				//初期化
				pair_x = last_x;
				pair_y = last_y;

				//ひっくり返せる場合
				if (isActiveOthello)
				{
					//スコア加算されたフラグ
					isAddScore = true;

					//maxComboCountを加算
					maxComboCount++;

					//使用する乱数を定義
					int random = (int)ShlomonMath::xor64;
					int maxScore = 0;

					//Score計算
					for (int i = 0; i <= loop + 1; i++)
					{
						//そのオセロの纏まりを判別
						if (othelloDatas[pair_y][pair_x].chainName != 0)
						{
							if (!VectorFinder(maxChainName, othelloDatas[pair_y][pair_x].chainName))
							{
								maxChainName.push_back(othelloDatas[pair_y][pair_x].chainName);
								maxChainNameCount = othelloDatas[pair_y][pair_x].chainName;
							}
						}
						else
						{
							othelloDatas[pair_y][pair_x].chainName = random;
						}

						//始点と終点を設定
						if (i == 0 || i == loop + 1) { startAndEndArray.push_back(std::make_pair(pair_y, pair_x)); }
						checkScoreData.push_back(make_pair(pair_y, pair_x));
						othelloDatas[pair_y][pair_x].comboCount = maxComboCount;
						othelloDatas[pair_y][pair_x].maxComboCount = maxComboCount;
						othelloDatas[pair_y][pair_x].score = (baseScore + (loop * (loop + 2) * maxComboCount));
						if (maxScore < othelloDatas[pair_y][pair_x].score)
						{
							maxScore = othelloDatas[pair_y][pair_x].score;
						}
						if (i == 0)
						{
							if (addScore < othelloDatas[pair_y][pair_x].score)
							{
								addScore = othelloDatas[pair_y][pair_x].score;
							}
							totalScore += othelloDatas[pair_y][pair_x].score * maxComboCount;
							//baseScore += othelloDatas[pair_y][pair_x].score;
						}
						pair_x += direction_x;
						pair_y += direction_y;
					}

					//全部空だった場合（1コンボ目）
					//if (maxChainName.empty())
					//{
					//	//初期化
					//	pair_x = last_x;
					//	pair_y = last_y;

					//	for (int i = 0; i <= loop + 1; i++)
					//	{
					//		othelloDatas[pair_y][pair_x].chainName = random;
					//		pair_x += direction_x;
					//		pair_y += direction_y;
					//	}
					//}

					//コンボを繋げた場合
					if (maxChainName.size() == 1)
					{
						int chainName = maxChainName.front();

						for (int i = 0; i < OthelloConstData::fieldSize; i++)
						{
							for (int j = 0; j < OthelloConstData::fieldSize; j++)
							{
								if (othelloDatas[i][j].chainName != chainName) { continue; }
								othelloDatas[i][j].chainName = random;
							}
						}
					}

					//複数挟んだ場合
					else if (maxChainName.size() > 1)
					{
						//コンボが繋がった場合
						for (auto itr = maxChainName.begin(); itr != maxChainName.end(); ++itr)
						{
							int chainName = *itr;

							for (int i = 0; i < OthelloConstData::fieldSize; i++)
							{
								for (int j = 0; j < OthelloConstData::fieldSize; j++)
								{
									if (othelloDatas[i][j].chainName != chainName) { continue; }
									othelloDatas[i][j].chainName = random;
								}
							}
						}
					}

					//ComboCountとScore修正
					if (!maxChainName.empty())
					{
						for (auto itr = maxChainName.begin(); itr != maxChainName.end(); ++itr)
						{
							int chainName = *itr;

							for (int i = 0; i < OthelloConstData::fieldSize; i++)
							{
								for (int j = 0; j < OthelloConstData::fieldSize; j++)
								{
									if (othelloDatas[i][j].chainName == chainName)
									{
										if (othelloDatas[i][j].maxComboCount != maxComboCount)
										{
											othelloDatas[i][j].maxComboCount == maxComboCount;
										}
										if (othelloDatas[i][j].score < maxScore)
										{
											othelloDatas[i][j].score = maxScore;
										}
									}
								}
							}
						}
					}

					//初期化
					pair_x = last_x;
					pair_y = last_y;

					//その場のコンボ数を取得
					combo = othelloDatas[pair_y][pair_x].maxComboCount;

					//ひっくり返す
					for (int i = 1; i <= loop; i++)
					{
						pair_x += direction_x;
						pair_y += direction_y;
						othelloDatas[pair_y][pair_x].isFront = side;
						//othelloDatas[pair_y][pair_x].comboCount++;
					}

					//初期化
					pair_x = last_x;
					pair_y = last_y;

					Imgui::score = totalScore;
				}

				break;
			}
			//周回したよ（こいつで何個挟んだか判定できる）
			loop++;
		}

		else
		{
			//カウント用
			//int count = 1;

			//マス判定
			//othelloCheckDatas[pair_y][pair_y] = true;
			//if (othelloCheckDatas[pair_y][pair_x]) { return; }

			//次のマス
			pair_x += direction_x;
			pair_y += direction_y;

			//範囲外
			if (pair_x < 0 || pair_x > OthelloConstData::fieldSize - 1) { break; }
			if (pair_y < 0 || pair_y > OthelloConstData::fieldSize - 1) { break; }

			//存在しない
			if (othelloDatas[pair_y][pair_x].type == NONE) { break; }
			//存在しない
			if (othelloDatas[count_y][count_x].type == NONE) { break; }
			//同色と隣接
			if (othelloDatas[pair_y][pair_x].isFront == side && loop == 0) { break; }

			//探索可能
			bool isActiveOthello = false;
			for (int i = 0; i < loop; i++)
			{
				count_x += direction_x;
				count_y += direction_y;
				if (!othelloDatas[count_y][count_x].isSandwich) {
					isActiveOthello = true;
				}
				//if (i == loop) { isActiveOthello = true; }
			}

			//初期化
			count_x = last_x;
			count_y = last_y;

			//挟めるオセロが見つかったら
			if (isActiveOthello)
			{
				if (othelloDatas[pair_y][pair_x].isFront == side && loop != 0)
				{
					for (int i = 0; i <= loop + 1; i++)
					{
						othelloDatas[count_y][count_x].isCheckEnd = true;
						count_x += direction_x;
						count_y += direction_y;
					}
					break;
				}
			}

			//次のマス
			//pair_x += direction_x;
			//pair_y += direction_y;

			loop++;
		}
	}
}

void CheakOthello::AddScore()
{
	totalScore = (int)(totalScore * powf(OTHELLO_BONUS, totalDeleteOthello - 1));
	totalDeleteOthello = 0;
}

void CheakOthello::ChangeScoreAndCombo()
{
	isCombos = false;
	isCombosCheck = false;

	//xがfront,yがsecond
	int maxScore = 0;
	int maxConboCount = 0;
	//int maxChainName = 0;
	bool isChangeScore = false;
	//bool isChangeChainName = false;

	for (auto itr = checkScoreData.begin(); itr != checkScoreData.end(); ++itr)
	{
		if (othelloDatas[itr->first][itr->second].score > maxScore)
		{
			maxScore = othelloDatas[itr->first][itr->second].score;
			maxConboCount = othelloDatas[itr->first][itr->second].comboCount;
			isChangeScore = true;
		}
		/*if (othelloDatas[itr->first][itr->second].chainName > maxChainName)
		{
			maxChainName = othelloDatas[itr->first][itr->second].chainName;
			isChangeChainName = true;
		}*/
	}

	for (auto itr = checkScoreData.begin(); itr != checkScoreData.end(); ++itr)
	{
		if (isChangeScore)
		{
			othelloDatas[itr->first][itr->second].score = maxScore;
			othelloDatas[itr->first][itr->second].comboCount = maxConboCount;
		}
		/*if (isChangeChainName)
		{
			othelloDatas[itr->first][itr->second].chainName = maxChainName;
		}*/
	}

	checkScoreData.clear();
}

void CheakOthello::CheckReachOthello(int direction_x, int direction_y)
{
	if (sandwichData.empty()) { return; }

	int lastX = sandwichData.front().second;
	int lastY = sandwichData.front().first;
	bool side = sandwichSide.front();
	bool isReach = false;

	while (1)
	{
		lastX += direction_x;
		lastY += direction_y;

		if (lastX < 0 || lastX > OthelloConstData::fieldSize - 1) { break; }
		if (lastY < 0 || lastY > OthelloConstData::fieldSize - 1) { break; }

		if (isReach)
		{
			if (othelloDatas[lastY][lastX].isFront == side) { continue; }
			else
			{
				reachData.push_back(make_pair(lastY, lastX));
				reachSide.push_back(side);
				break;
			}
		}

		else if (!isReach)
		{
			if (othelloDatas[lastY][lastX].isFront == side) { break; }
			else { isReach = true; }
		}
	}
}
