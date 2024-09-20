#include "BattleLog.h"
#include"../Manager/GameManager.h"

void BattleLog::AddLog(const std::string& log)
{
	// ログ数が8個を超えている場合、最初のログを削除する
	if (logs.size() >= 8) {
		auto erase_log = logs.begin();
		//最初のログはターン表記なので2個目の要素を消去する
		logs.erase(erase_log++);
	}
	logs.push_back(log);
}

//アイテムを使用した時のログ
void BattleLog::AddRecoveryLog(const std::string& player, const std::string& item, int recovery)
{
	std::string log = player + "が" + item + "を使用しました。" + std::to_string(recovery) + "回復しました。";
	AddLog(log);
}

void BattleLog::AddItemDrop(const std::string& Player, const std::string& ItemName)
{
	std::string log = Player + "は" + ItemName + "を手に入れた！";
	AddLog(log);
}

void BattleLog::AddBuffLog(const std::string& player, const std::string& Item, const std::string& type, int buff)
{
	std::string log = player + "が" + Item + "を使用しました。" + type + "が"+ std::to_string(buff) + "上がった。";
	AddLog(log);
}

//描画処理
void BattleLog::DrawLogs() const
{
	//ログの描画位置
	int yPos = 50;
	const int LOG_POS_X = 690;

	// ログを上から順に描画
	for (const auto& log : logs) {
		DrawStringEx(LOG_POS_X, yPos, koni::Color::WHITE, log.c_str());
		yPos += 20; // 次回ログの位置を調整
	}
}
