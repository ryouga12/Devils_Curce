#include "GameManager.h"


//シングルトン
GameManager* GameManager::getGameManager()
{
	static GameManager* p_instance = nullptr;

	if (!p_instance)p_instance = new GameManager();
	return p_instance;
}



GameManager::GameManager()
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
	//スキルのポインタ
	if (skill == nullptr) {
		skill = std::make_shared<Skill>();
	}
	else {
		tnl::DebugTrace("ポインタはすでに存在します");
	}
}

GameManager::~GameManager()
{
	

}

void GameManager::draw()
{

	
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

void GameManager::displayDialogue()
{
	DrawStringEx(80, 100, -1, "(1 : 話す)");
	DrawStringEx(80, 150, -1, "(0 : メニューを閉じる)");
}

bool GameManager::TimeCount(float delta_time, float Time)
{
	count_Time += delta_time;

	if (count_Time > Time) {
		count_Time = 0;
		return true;
	}

	return false;
}




