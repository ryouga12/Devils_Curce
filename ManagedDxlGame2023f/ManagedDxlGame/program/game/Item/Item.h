#pragma once
///
///アイテムを管理するクラス
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class GameManager;
class Skill;
class BattleLog;
class CsvManager;

class Item final: public  ItemBase{
public:
	
	Item();
	~Item()=default;

	//アイテムの配列を取得する
	const std::list<ItemBase>&GetItemArray()const {
		return item_status_type;
	}

	//使用できるアイテム
	enum class UsedItem {
		//ポーション
		PORTION = 3,
		//ハイポーション
		HIPORTION = 8,
		//マジックポーション
		MAGICPORTION = 14,
		//アタックポーション
		ATTACKPORTION = 16,
		//プラントエキス(回復アイテム)
		PLANTEXTRACT = 17,
		//スライムの鈴
		//使用するとスキルを手に入れられる
		SLIMBELL = 20,
		//スネークの鈴
		//使用するとスキルを手に入れられる
		SNAKEBELL = 21,
	};

private:

	//読み込み関数
	void ItemLoadTypeInfo();
	
	//構造体の総数
	int item_status_total_num;

	//構造体を格納する配列
	std::list<ItemBase>item_status_type;

	//バトルシーンのみで使う為のフラグ
	bool BattleFlag = false;

	//バトルログのポインタ
	Shared<BattleLog>battle_log_ = nullptr;

	//回復量
	int healAmount = 0;

	//一時的にアイテムであげる為の攻撃力
	int AttackAmount = 0;

public:

	//IDを指定してアイテムを取得する関数
	//arg_1 : アイテムのID
	//アイテムのIDを入れるとそれに該当したアイテムを取得する
	ItemBase GetItemById(const int& id) const;

	//アイテムを使用した時の処理
	//arg_1 : アイテムのID
	//アイテムのIDによって処理を分ける
	void ItemUse(const int& itemid);

	//使用時Hp回復系
	//arg_1 : 倍率
	//arg_2 : アイテムのID
	void ItemHpHeal(const float& percentage, const int& itemid);

	//バトルログのポインタをセットする
	//arg_1 : バトルログのSharedポインタ
	void SetBattleLog(const Shared<BattleLog>& battle_log) { battle_log_ = battle_log; }

	//スキルを追加する系のアイテム
	template <class T>
	void AddSkillItem(const int& itemid, Shared<T> skill);

	//フラグを切り替える
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

