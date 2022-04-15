#include "CheakOthello.h"
#include <string>
#include <iterator>
#include "../BaseDirectX/Input.h"

CheakOthello::CheakOthello()
{
	//lastX = 0;
	//lastY = 0;
	//cheakSize = 0;
	//combo = 0;
}

CheakOthello::~CheakOthello()
{
}

void CheakOthello::Init()
{
	//combo = 0;
}

void CheakOthello::Update(const vector<vector<SendOthelloData>>& othelloData)
{
	//最後に動いた駒を判定、保存
	CheckLastMove(othelloData);

	while (1)
	{
		//判定を取る駒をセット
		bool IsSet = SetCheckOthello();
		if (!IsSet) { break; }

		//全方位チェック
		/*-----左-----*/
		OthelloCheck(Direction_X::WEST, NONE_DIRECTION);
		/*-----右-----*/
		OthelloCheck(Direction_X::EAST, NONE_DIRECTION);
		/*-----上-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::SOUTH);
		/*-----下-----*/
		OthelloCheck(NONE_DIRECTION, Direction_Y::NOUTH);

		/*-----左斜め上-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::NOUTH);
		/*-----左斜め下-----*/
		OthelloCheck(Direction_X::WEST, Direction_Y::SOUTH);
		/*-----右斜め上-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::NOUTH);
		/*-----右斜め下-----*/
		OthelloCheck(Direction_X::EAST, Direction_Y::SOUTH);
	}
}

void CheakOthello::CheckLastMove(const vector<vector<SendOthelloData>>& othelloData)
{
	//最新状態を入手
	othelloDatas = othelloData;

	//その場にオセロが存在するかつ、最後に動かしてたかを判定（←ちょっと効率悪そう）
	for (int i = 0; i < MAX_SIZE_Y; i++)
	{
		for (int j = 0; j < MAX_SIZE_X; j++)
		{
			if (!othelloDatas[i][j].isSandwich)
			{
				//その場所が空
				if (othelloDatas[i][j].type == NONE) { continue; }
				//最後に動かしてるやつ
				if (!othelloDatas[i][j].isMove) { continue; }
			}
			side = othelloDatas[i][j].isFront;
			comboOthelloDataPos.push_back(std::make_pair(i, j));
		}
	}
}

bool CheakOthello::SetCheckOthello()
{
	if (!comboOthelloDataPos.empty())
	{
		last_y = comboOthelloDataPos.front().first;
		last_x = comboOthelloDataPos.front().second;
		comboOthelloDataPos.erase(comboOthelloDataPos.begin());
		return true;
	}
	else { return false; }
}

void CheakOthello::OthelloCheck(int direction_x, int direction_y)
{
	int loop = 0;
	int count_x = last_x;
	int count_y = last_y;
	int pair_x = last_x;	//loop内
	int pair_y = last_y;	//loop内

	int max_size;
	if (MAX_SIZE_X > MAX_SIZE_Y) { max_size = MAX_SIZE_X; }
	else { max_size = MAX_SIZE_Y; }

	while (1)
	{
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
			//baseScoreを計算//スコア計算（沈み終わったらそのオセロのスコアを0にしてもらう）


			//全部探索リストに入れる（自分と挟んだ駒まで）←sandwichArrayが同じかも参照する
			//挟んだやつの最大のconboCountを取得→それ+1したのをコンボしたオセロにセット
			int maxComboCount = 0;

			for (int i = 0; i <= loop + 1; i++)
			{
				othelloDatas[pair_y][pair_x].isSandwich = true;
				if (i == 0 || i == loop + 1) { startAndEndArray.push_back(std::make_pair(last_y, last_x)); }

				//そのオセロのコンボ数を取得
				if (maxComboCount < othelloDatas[pair_y][pair_x].comboCount)
				{
					maxComboCount = othelloDatas[pair_y][pair_x].comboCount;
				}

				//オセロ毎にコンボ数保存、違う数で挟んだ場合push_back
				//オセロ毎のコンボカウントを追加しろ
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

			//maxComboCountを加算
			maxComboCount++;

			//comboCountをセット
			for (int i = 0; i <= loop + 1; i++)
			{
				othelloDatas[pair_y][pair_x].comboCount = maxComboCount;
				pair_x += direction_x;
				pair_y += direction_y;
			}

			//初期化
			pair_x = last_x;
			pair_y = last_y;

			//ひっくり返す
			for (int i = 1; i <= loop; i++)
			{
				pair_x += direction_x;
				pair_y += direction_y;
				othelloDatas[pair_y][pair_x].isFront = side;
				othelloDatas[pair_y][pair_x].comboCount++;
			}

			break;
		}
		//周回したよ（こいつで何個挟んだか判定できる）
		loop++;
	}
}
