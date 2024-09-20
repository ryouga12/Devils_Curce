#include "GameManager.h"


//シングルトン
GameManager* GameManager::GetGameManager()
{
	static GameManager* p_instance = nullptr;

	if (!p_instance)p_instance = new GameManager();
	return p_instance;
}

void GameManager::DelateGameManager()
{
	delete GetGameManager();
}

//それぞれのポインターを生成する
void GameManager::PoiterCreate()
{
	//インベントリのポインタ

	if (inventory == nullptr) {
		inventory = std::make_shared<Inventory>();
	}
	else {
		tnl::DebugTrace("ポインタはすでに存在します");
	}
	//プレイヤーのポインタ
	if (player == nullptr) {
		player = std::make_shared<Player>();
	}
	else {
		tnl::DebugTrace("ポインタはすでに存在します");
	}
	//カメラのポインタ
	if (camera == nullptr) {
		camera = std::make_shared<KonCamera>();
	}
	else {
		tnl::DebugTrace("ポインタはすでに存在します");
	}

}

//ポインタをリセットする
void GameManager::Reset()
{
	if (inventory != nullptr) {
		inventory.reset();
	}
	else {
		tnl::DebugTrace("ポインタは存在しません");
	}
	if (player != nullptr) {
		player.reset();
	}
	else {
		tnl::DebugTrace("ポインタは存在しません");
	}
	if (camera != nullptr) {
		camera.reset();
	}
	else {
		tnl::DebugTrace("ポインタは存在しません");
	}
}

int GameManager::GetRandEx(int a, int b)
{

	if (a > b) {
		int diff = static_cast<int>(a - b);
		int random = static_cast<int>(GetRand(diff) + b);

		return random;
	}
	else {
		int diff = static_cast<int>(b - a);
		int random = static_cast<int>(GetRand(diff) + a);

		return random;
	}

	return 0;
}

bool GameManager::TimeCount(float delta_time, float Time)
{
	//指定した時間までプラスする
	count_time += delta_time;

	//指定時間が過ぎたらTrueにする
	if (count_time > Time) {
		count_time = 0;
		return true;
	}
	return false;
}



