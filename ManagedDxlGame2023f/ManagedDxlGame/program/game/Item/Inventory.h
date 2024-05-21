#pragma once
#include"../Item/Item.h"
#include"../Object/Player.h"
#include"../Manager/ResourceManager.h"
#include"../Menu/MenuWindow.h"
#include"../Manager/SoundManager.h"
#include"../Skill/Skill.h"
#include"../Menu/BattleLog.h"

class ItemBase;

class Inventory
{
public:

	Inventory();
	~Inventory();
	

	void Update(float delta_time);
	void draw();

	//内部にItemを20個持つlist
	std::vector<ItemBase>InventoryList;

	//インベントリの最大数
	int InventoryMaxSize = 20;

	//アイテムを追加する関数
	void AddInventory(int id);

	//アイテムを装備する関数
	void EquipWeapon(int weaponIndex);

	//アイテムの詳細などの処理
	void ItemDetail();

	//アイテムをの番号によって処理をする
	void InventoryItemUse(int itemid );


	//インベントリ内のアイテムの数を取得する関数
	inline int GetItemCount() {
		return InventoryList.size();
	}
	
	//インベントリのリストを他のクラスで使う
	std::vector<ItemBase>& GetInventoryList() {
		return InventoryList;
	}

	//Playerのポインタをセットする
	void SetPlayer(std::shared_ptr<Player>player) { plyer = player; }

	//バトルログのポインタをセットする
	void SetBattleLog(Shared<BattleLog>battle_log) { battleLog = battle_log; }

	//アイテムのカーソルの移動処理の際のインデックスの操作
	void ItemCurourIndex(int ItemPerPage);

	//スキルのカーソルの移動処理の際のインデックスの操作
	void SkillCurourIndex();

	//インデックスを0にする
	void SelectedIndexClear() {
		selectedIndex = 0;
	}

	//アイテムのインデックスを取得する
	int GetSelectedIndex()const {
		return selectedIndex;
	}

	//取得したアイテムのidを取得する
	int GetSelectedItemId()const {
		return selectedItemId;
	}

	//スキルのインデックスを取得する
	int GetSkillSelectedIndex()const {
		return selectedSkillIndex;
	}
	//スキル

private:

	//インベントリの切り替えの為のクラス
	enum class MenuWindow_I {
		Empty,
		First_Menu,
		ItemMenu,
		StatusMenu,
		ItemUseMenu,
		ItemDetailMenu,
		SkillMenu,
		SkillDatailMenu
	};

	MenuWindow_I select_menu = MenuWindow_I::Empty;

	int max_hp = 0;

	//ポーションを使った時の回復量
	/*int recovery = 30;*/
	int healAmount = 0;

	//攻撃力を一時的に保存する
	int Attack = 0;
	//防御力を一時的に保存する
	int Defance = 0;

	//プレイヤーのコントロールを制御する
	int PlyControlTrue = 0; int PlyControlFalse = 1;

public:

	//アイテムのメニュー
	void ItemMenu(const tnl::Vector2i& itemDrawPos, tnl::Vector2i& curentPageText, int CousourX ,int itemParPage);

	//インベントリをセットする
	void SetSelect_num(int select_menu);

	//インベントリをを取得する
	MenuWindow_I getSelect_menu_num() { return select_menu; }

	//recoveryを取得する
	int getRecovery() { return healAmount; }

//------------------------------------------------------------------------------------------------------------------------
//---関数---//
public:

	//カーソルを上下に動かす関数
	void CusorMove();
	//カーソルを一番上に戻す関数
	void CursorReset();
	//ページのリセット
	void CurentPageReset() { currentPage = 0; }
	//使用時Hp回復系
	void ItemHpHeal(float percentage , int itemid);
	
//------------------------------------------------------------------------------------------------------------------------
//---メニュー---//
private:

	//メニューを初期化する
	void InitMenuWinodow();
	//最初のメニュー
	void First_Menu();
	//プレイヤーのステータスを表示するメニュー
	void PlyStatusMenu();
	//武器の中の詳細や使うための描画
	void ItemUseMenu();
	//今どのページにいるか
	int currentPage = 0;
	//1ページあたりのアイテム数
	int item_per_page = 5;
	//ユーザーが選択した要素のインデックス
	//最初は一番上なので0に設定する
	int selectedIndex = 0;
	//取得したアイテムのIDを一時的に保持するための変数
	int selectedItemId = 0;

//アイテムの描画で使うインデックス
//  
	//最初の番号
	int start_index = 0;
	//最後の番号
	int end_index = 0;
	
private:

	enum  {
		EMPTY,
		WEAPON,
		ARMOR
	};

	//インベントリ内のアイテム数
	int itemNum = 0;

	//選択中のカーソルの位置
	int selectCursor = 0;

	//武器を装備する
	std::list<ItemBase> equippedWeapon;
	std::list<ItemBase> equippedArmor;

	//playerのステータス
	/*Player::PlyerStatus plyerstatus;*/

	//plyerStatusのid番号
	int PlyerStatus_Id;

	//武器の装備が可能か
	bool equipWeapon = false;
	bool equipArmor = false;

	//武器を装備した際の[E]を表示する座標
	tnl::Vector2i equipText = { 80 , 100 };

	//現在のページを表示座標
	tnl::Vector2i CurentPageText = { 100 , 280 };

	//白色
	const int Color_White = -1;

	//カーソルのサイズ
	const float cursorSize = 0.3f;

	//ItemのComentを切り替える為のクラス
	enum class ItemComent {
		Empty,
		No,
		Use,
	};

	ItemComent item_coment = ItemComent::Empty;

	
	//Playerが武器を装備した時に一時的に値を保存しておくための変数
	int equipAttack = 0;
	int equipDefance = 0;

	//それぞれのシーケンスでの処理
	void swichInventoryUpdate(float deltatime);

	//アイテムやスキルを表示する座標
	tnl::Vector2i DrawPos = { 110 , 100 };

	//カーソルのx座標
	int cursorX = 70;

	//アイテムのコメント表示座標
	tnl::Vector2i ItemComentPos = { 630 , 100 };

	//一時的にアイテムであげる為の攻撃力
	int AttackAmount = 0;

	//バトルシーンのみで使う為のフラグ
	bool BattleFlag = false;

public:

	//Playerが武器を装備した時に一時的に値を保存しておくための変数を取得する
	int getEquipStatus(int type);

	//武器配列を取得する
	std::list<ItemBase>& getEquipArray() {
		return equippedWeapon;
	}

	//上がったAttack量の値を取得する
	int getAttackAmount()const {
		return AttackAmount;
	}

	//フラグを切り替える
	//0 が true で 1 が false
	void BattleFlagSwitch(int id) {
		if (id == 0) {
			BattleFlag = true;
		}
		else {
			BattleFlag = false;
		}
	}

//------------------------------------------------------------------------------------------------------------------------
//特技
private :

	//特技用のカーソル
	int SkillCousour = 0;

	//特技の数
	int SkillNum = 0;
	
	//スキルのページでどこのページにいるか
	int SkillCurentPage = 0;

	//スキルの要素番号
	int selectedSkillIndex = 0;

	//特技用のインベントリ
	std::vector<Shared<Skill>> SkillList;


public:

	//特技を追加する
	void AddSkill(Shared<Skill>skill);

	//特技の描画
	void InventorySkill();

	//特技を使用した時の描画
	void SkillDetailDraw(int skill_index);

	//特技のカーソル
	void SkillCousorMove();

	//スキルをセットする
	void SkillSet();

	//スキルを追加する系のアイテム
	template <class T>
	void AddSkillItem(int itemid , Shared<T>skill);

	//スキルの配列を取得する
	std::vector<Shared<Skill>>& getSkillList() {
		return SkillList;
	}

	//スキルの個数を取得する
	int getSkillNum() {
		return SkillNum;
	}

//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	Shared<Item>item = nullptr;
	Shared<Player>plyer = nullptr;
	Shared<MenuWindow>first_menu = nullptr;
	Shared<MenuWindow>select_action_menu = nullptr;
	Shared<MenuWindow>select_detail_window = nullptr;
	Shared<Menu>menu_window = nullptr;
	Shared<Menu>item_coment_window = nullptr;
	Shared<Menu>plyer_status_window = nullptr;
	Shared<Menu>SkillInventoryWindow = nullptr;
	Shared<Menu>SkilldetailWinodow = nullptr;
	Shared<Menu>ItemDetailWindow = nullptr;
	Shared<BattleLog>battleLog = nullptr;

};