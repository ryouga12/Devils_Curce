#pragma once
///
///アイテムを管理するクラス
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class Skill;
class BattleLog;
class KonCamera;

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
		//既存のスキルを進化させる秘伝書(大火炎斬り)
		GREATFLAMESLASH = 42,
		//既存のスキルを進化させる秘伝書(ネプチューンスパイラル)
		WATERFORGE = 43
	};

private:

	//読み込み関数
	void ItemLoadTypeInfo();
	
	//構造体の総数
	int item_status_total_num = 0;

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
	bool ItemUse(const int& itemid);

	//使用時Hp回復系
	//arg_1 : 倍率
	//arg_2 : アイテムのID
	bool ItemHpHeal(const int& percentage, const int& itemid);

	//バトルログのポインタをセットする
	//arg_1 : バトルログのSharedポインタ
	void SetBattleLog(const Shared<BattleLog>& battle_log) { battle_log_ = battle_log; }

	//スキルを追加する系のアイテム
	bool AddSkillItem(const int& itemid, const Shared<Skill>& skill);

	//スキルを進化させるアイテム(秘伝書)
	bool EvolutionSkillItem(const int& item_id, const Shared<Skill>& skill_);

	//秘伝書アイテムに応じてスキルIDを返す
	int EvolutionSkillID(const int& evolution_item_id)const;

	//フラグを切り替える
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

//当たり判定のあるアイテム
class CollisonItem : public Actor {
public:

	//当たり判定のあるアイテム
	//arg_1 : ID
	//arg_2 : X座標(float)
	//arg_3 : Y座標(float)
	//arg_4 : チップサイズ
	//arg_5 : 倍率
	//arg_6 : 画像ハンドル
	//arg_7 : 入手できるアイテムかどうかのフラグ
	CollisonItem(const int& id, const float& pos_x, const float& pos_y, const int& CHIPSIZE, const float& scale, const int& ghdl, const bool canBeAcquired);

	//描画
	void Draw(const KonCamera& camera);
	
	//当たり判定のあるアイテムのIDを取得する
	int GetCollisonItemID()const {
		return item_id;
	}

	//アイテムの座標を取得する
	const tnl::Vector3& GetCollisonItemPos()const {
		return item_pos;
	}
	
	//アイテムのチップサイズを取得する
	int GetCollsionItemChipSize()const {
		return chip_size;
	}

	//アイテムのスケールを取得する
	float GetCollisonItemScale()const {
		return item_scale;
	}

	//アイテムのハンドルを取得する
	int GetCollisonItemHdl()const {
		return item_hdl;
	}

	//入手可能のアイテムかのフラグを取得する
	bool GetCollisonItemisObtainable()const {
		return isObtainable;
	}

	//アイテムの座標をセットする
	void SetCollisionItemPos(const tnl::Vector3& new_pos) {
		item_pos = new_pos;
	}

private:


	//アイテムのID
	int item_id = 0;

	//アイテムの座標
	tnl::Vector3 item_pos = {};

	//アイテムのチップサイズ
	int chip_size = 0;

	//アイテムのスケール
	float item_scale = 0;

	//アイテムの画像ハンドル
	int item_hdl = 0;

	//入手可能かどうかのフラグ
	bool isObtainable = false;

};