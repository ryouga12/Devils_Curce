#pragma once
///
///アイテムを管理するクラス
/// 
#include<string>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class GameManager;
class Skill;
class BattleLog;
class CsvManager;

class Item : public  ItemBase{
public:
	
	Item();
	~Item();

	void update(float delta_time);
	void draw();

	//アイテムの配列を取得する
	const std::list<ItemBase>&getItemArray()const {
		return m_Item_Status_Type;
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
	
	//modelを入れる変数
	ItemBase item_Status_ary;

	//構造体の総数
	int m_item_Status_Total_num;

	//構造体を格納する配列
	std::list<ItemBase>m_Item_Status_Type;

	//アイテムなどの説明文などを入れる配列
	std::string ItemDisc = {};

	//itemの座標
	tnl::Vector3 item_pos = {};

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
	//引数 : アイテムのID
	//アイテムのIDを入れるとそれに該当したアイテムを取得する
	ItemBase GetItemById(int id) const;

	//アイテムを使用した時の処理
	void ItemUse(int itemid);

	//使用時Hp回復系
	void ItemHpHeal(const float& percentage, int itemid);

	//バトルログのポインタをセットする
	void SetBattleLog(Shared<BattleLog>battle_log) { battle_log_ = battle_log; }

	//アイテムがインベントリ内に存在したらインベントリから消去する
	void ItemRemove(int itemid);

	//スキルを追加する系のアイテム
	template <class T>
	void AddSkillItem(int itemid, Shared<T>skill);

	//上がったAttack量の値を取得する
	int getAttackAmount()const {
		return AttackAmount;
	}

	//recoveryを取得する
	int getRecovery() { return healAmount; }

	//フラグを切り替える
	//0 が true で 1 が false
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

