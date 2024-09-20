//------------------------------------------------------------------------------------------------------------
//
//バトルログクラス(バトルログを設定して流す)
//
//-------------------------------------------------------------------------------------------------------------

#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../../koni_name_space/common/common_value.h"

class BattleLog final{
public:

	//ログを追加する
	void AddLog(const std::string& log);

	//アイテムを使用した時のログ
	void AddRecoveryLog(const std::string& player, const std::string& Item, int recovery);

	//アイテムを入手した時の処理
	void AddItemDrop(const std::string& Player, const std::string& ItemName);

	//バフ系のログ
	void AddBuffLog(const std::string& player, const std::string& Item,const std::string& type, int buff);

	//描画処理
	void DrawLogs()const;

	//バトルログのリセット
	void LogReset() {
		logs.clear();
	}

private:

	//バトルログを格納する配列
	std::list<std::string>logs;

};