#pragma once
#include "../../dxlib_ext/dxlib_ext.h"

class BattleLog {
public:

	//ログを追加する
	void addLog(const std::string& log);

	//ダメージを与えた時のログ
	void addDamageLog(const std::string& attacker, const std::string& target, int damage);

	//ダメージをくらった時のログ
	void addDamagePlayerLog(const std::string& attacker, const std::string& target, int damage);

	//アイテムを使用した時のログ
	void addRecoveryLog(const std::string& player, const std::string& Item, int recovery);

	//特技を使った時のログ
	void addSkillUseLog(const std::string& name, const std::string& Skillname, const std::string& target ,int damage);

	//スキルを覚えた時のログ
	void addSkillLog(const std::string& skillName);

	//ゴールドを入手した時のログ
	void addMoney(const std::string& Player, int money);

	//アイテムを入手した時の処理
	void addItemDrop(const std::string& Player, const std::string& ItemName);

	//バフ系のログ
	void addBuffLog(const std::string& player, const std::string& Item,const std::string& type, int buff);

	//描画処理
	void drawLogs()const;

	//バトルログのリセット
	void LogReset() {
		logs.clear();
	}

private:

	//バトルログを格納する配列
	std::list<std::string>logs;

};