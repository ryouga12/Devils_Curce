#include "BattleLog.h"

void BattleLog::addLog(const std::string& log)
{
	// ログ数が8個を超えている場合、最初のログを削除する
	/*if (logs.size() >= 8) {
		logs.erase(logs.begin());
	}*/
	logs.push_back(log);
}

//ダメージを与えた時のログ
void BattleLog::addDamageLog(const std::string& attacker, const std::string& target, int damage)
{
	std::string log = attacker + "が" + target + "に" + std::to_string(damage) + "のダメージを与えた";
	addLog(log);
}

//ダメージをくらった時のログ
void BattleLog::addDamagePlayerLog(const std::string& attacker, const std::string& target, int damage)
{
	std::string log = target + "が" + attacker + "から" + std::to_string(damage) + "のダメージをくらった";
	addLog(log);
}

//アイテムを使用した時のログ
void BattleLog::addRecoveryLog(const std::string& player, const std::string& Item, int recovery)
{
	std::string log = player + "が" + Item + "を使用しました。" + std::to_string(recovery) + "回復しました。";
	addLog(log);
}

//特技を使った時のログ
void BattleLog::addSkillUseLog(const std::string& name, const std::string& Skillname, const std::string& target ,int damage)
{
	std::string log = name + "が" + Skillname + "を使用しました。" + target + "に" +  std::to_string(damage) + "のダメージを与えた。";
	addLog(log);
}

//特技を覚えた時のログ
void BattleLog::addSkillLog(const std::string& skillName)
{
	std::string log = skillName + "を覚えた!!";
	addLog(log);

}

//ゴールドを手に入れた時のログ
void BattleLog::addMoney(const std::string& Player, int money)
{
	std::string log = Player + "は" + std::to_string(money) + "ゴールドを手に入れた！";
	addLog(log);
}

void BattleLog::addItemDrop(const std::string& Player, const std::string& ItemName)
{
	std::string log = Player + "は" + ItemName + "を手に入れた！";
	addLog(log);
}

void BattleLog::addBuffLog(const std::string& player, const std::string& Item, const std::string& type, int buff)
{
	std::string log = player + "が" + Item + "を使用しました。" + type + "が"+ std::to_string(buff) + "上がった。";
	addLog(log);
}

//描画処理
void BattleLog::drawLogs() const
{
	//ログの描画位置
	int yPos = 50;
	// ログを上から順に描画
	for (const auto& log : logs) {
		DrawStringEx(690, yPos, -1, log.c_str());
		yPos += 20; // 次回ログの位置を調整
	}
}
