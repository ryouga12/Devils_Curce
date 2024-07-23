#pragma once
#include"../Item/Item.h"
#include"../Object/Player.h"
#include"../Manager/ResourceManager.h"
#include"../Menu/MenuWindow.h"
#include"../Manager/SoundManager.h"
#include"../Skill/Skill.h"


class ItemBase;
class GameManager;
class UIManager;


class Inventory
{
public:

	Inventory();
	~Inventory();

	//更新処理
	void Update(float delta_time);
	//描画処理
	void draw();

	//内部にItemを20個持つlist
	std::vector<ItemBase>inventory_list;

	//インベントリの最大数
	int inventory_max_size_ = 20;

	//アイテムを追加する関数
	//引数 : item_id
	//引数にアイテムのIDを入れる事でインベントリにアイテムが格納される
	void AddInventory(const int& id);

	//アイテムを装備する関数
	//引数 : 武器のインデックス
	//引数にインデックスを入れると武器を装備できる
	void EquipWeapon(const int& weaponIndex);

	//アイテムの詳細を表示する
	void ItemDetail();

	//インベントリのリストを他のクラスで使う
	std::vector<ItemBase>& GetInventoryList() {
		return inventory_list;
	}

	//アイテムのカーソルの移動処理の際のインデックスの操作
	void ItemCurourIndex(int ItemPerPage);

	//スキルのカーソルの移動処理の際のインデックスの操作
	void SkillCurourIndex();

	//インデックスを0にする
	void SelectedIndexClear() {
		selected_index = 0;
	}

	//アイテムのインデックスを取得する
	int GetSelectedIndex()const {
		return selected_index;
	}

	//取得したアイテムのidを取得する
	int GetSelectedItemId()const {
		return selected_item_id;
	}

	//スキルのインデックスを取得する
	int GetSkillSelectedIndex()const {
		return skill_selected_index;
	}
	//スキル

	//インベントリの切り替えの為のクラス
	enum class MenuWindow_I {
		EMPTY,
		FIRSTMENU,
		ITEMMENU,
		STATUSMENU,
		ITEMUSEMENU,
		ITEMDETAILMENU,
		SKILLMENU,
		SKILLDATAILMENU
	};

	//インベントリのメニューをセットする
	//引数 : どのウィンドウに切り替えるのか
	//他の場所でウィンドウを切り替える為の関数
	void InventoryMenuChange(MenuWindow_I new_window) {
		select_menu = new_window;
	}

private:

	//どのメニューにいるか
	MenuWindow_I select_menu = MenuWindow_I::EMPTY;

	//攻撃力を一時的に保存する
	int attack = 0;
	//防御力を一時的に保存する
	int defance = 0;


public:

	//アイテムのメニュー
	//引数 : アイテムの座標, ページ数を表示する座標, カーソルのX座標 , ページ数
	//バトルシーンやマップシーンでインベントリの座標が違う為使い分ける為
	void ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, int CousourX, int itemParPage);

	//インベントリをを取得する
	MenuWindow_I GetSelectMenuNum() { return select_menu; }

	//アイテムの数を減らす
	void DelateItemNum() {
		item_num--;
	}

	//------------------------------------------------------------------------------------------------------------------------
	//---関数---//
public:

	//カーソルを上下に動かす関数
	void CusorMove();
	//カーソルを一番上に戻す関数
	void CursorReset();
	//ページのリセット
	void CurentPageReset() { curent_page = 0; }

	//------------------------------------------------------------------------------------------------------------------------
	//---メニュー---//
private:

	//メニューを初期化する
	void InitMenuWinodow();
	//インベントリを閉じている時プレイヤーが動いている時のメニュー表示
	void Game_Menu();
	//最初のメニュー
	void First_Menu();
	//プレイヤーのステータスを表示するメニュー
	void PlyStatusMenu();
	//武器の中の詳細や使うための描画
	void ItemUseMenu();
	//今どのページにいるか
	int curent_page = 0;
	//1ページあたりのアイテム数
	const int ITEMPERPAGE = 5;
	//ユーザーが選択した要素のインデックス
	//最初は一番上なので0に設定する
	int selected_index = 0;
	//取得したアイテムのIDを一時的に保持するための変数
	int selected_item_id = 0;

	//アイテムの描画で使うインデックス
	  
	//最初の番号
	int start_index = 0;
	//最後の番号
	int end_index = 0;

private:

	//武器の種類
	enum {
		EMPTY,
		WEAPON,
		ARMOR
	};

	//インベントリ内のアイテム数
	int item_num = 0;

	//選択中のカーソルの位置
	int select_cursor = 0;

	//武器を装備する
	std::list<ItemBase> equipped_weapon;
	std::list<ItemBase> equipped_armor;

	//武器の装備が可能か
	bool equip_weapon = false;
	bool equip_armor = false;

	//武器を装備した際の[E]を表示する座標
	const tnl::Vector2i EQUIPTEXT = { 80 , 100 };

	//現在のページを表示座標
	const tnl::Vector2i CURENTPAGETEXT = { 100 , 280 };

	//白色
	const int COLORWHITE = -1;

	//カーソルのサイズ
	const float CURSORSIZE = 0.3f;

	//Playerが武器を装備した時に一時的に値を保存しておくための変数
	int equip_attack = 0;
	int equip_defance = 0;

	//それぞれのシーケンスでの処理
	void swichInventoryUpdate(float deltatime);

	//アイテムやスキルを表示する座標
	const tnl::Vector2i DRAWPOS = { 110 , 100 };

	//カーソルのx座標
	const int CURSORX = 70;

	//アイテムのコメント表示座標
	const tnl::Vector2i ITEMCOMENTPOS = { 630 , 100 };

	//一時的にアイテムであげる為の攻撃力
	int attack_amount = 0;

	//使用できるアイテムかできないアイテムかのフラグ(主にコメントで使用)
	bool can_used_item = true;

	//袋の最大ページ数
	const int ITEM_MAX_PAGE = 4;

	//---ウィンドウの座標関連---//

	//---メニューウィンドウの座標とサイズ---//

	//メニューウィンドウの座標
	const tnl::Vector2i MENU_WINDOW_POS = { 50, 50, };
	//メニューウィンドウの幅
	const int MENU_WINDOW_WIDTH = 300;
	//メニューウィンドウの高さ
	const int MENU_WINDOW_HEIGHT = 270;
	//最初のウィンドウの幅
	const int FIRST_MENU_WINDOW_WIDTH = 250;
	//最初のウィンドウの高さ
	const int FIRST_MENU_WINDOW_HEIGHT = 250;


	//最初のESCを押したときに出る最初のメニューウィンドウの座標

	//道具
	const tnl::Vector2i MENU_TOOL_POS = { 100 , 100 };
	//特技
	const tnl::Vector2i MENU_SKILL_POS = { 100 , 150 };
	//強さ
	const tnl::Vector2i MENU_STATUS_POS = { 100 , 200 };
	//閉じる
	const tnl::Vector2i MENU_CLOSE_POS = { 100 , 250 };

	//アイテムウィンドウ内で選択するメニュー

	//使う
	const tnl::Vector2i MENU_ITEM_USE_POS = { 400 , 90 };
	//詳細
	const tnl::Vector2i MENU_ITEM_DETAIL_POS = { 400, 140 };
	//装備する
	const tnl::Vector2i MENU_ITEM_EQUIP_POS = { 400 , 190 };
	//捨てる
	const tnl::Vector2i MENU_ITEM_DISPOSE_POS = { 400 , 240 };
	//閉じる
	const tnl::Vector2i MENU_ITEM_CLOSE_POS = { 400 , 290 };


	//---アイテム説明関連の座標とサイズ---//

	//アイテムウィンドウの座標
	const tnl::Vector2i ITEM_DETAIL_WINDOW_POS = { 600, 50, };
	//アイテム説明ウィンドウの幅
	const int ITEM_DETAIL_WINDOW_WIDTH = 400;
	//アイテム説明ウィンドウの高さ
	const int ITEM_DETAIL_WINDOW_HEIGHT = 250;
	//アイテムの説明を移す座標
	const tnl::Vector2i ITEM_DETAIL_POS = { 630 , 100 };
	//アイテムの使うや詳細を開いたりするウィンドウの座標と幅と高さ
	const tnl::Vector2i ITEM_USE_POS = { 350 , 50 };
	//幅
	const int ITEM_USE_WINDOW_WIDTH = 250;
	//高さ
	const int ITEM_USE_WINDOW_HEIGHT = 280;

	//---ゴールドを表示する為のウィンドウの座標とサイズ---//

	//ゴールドを表示する為のウィンドウの座標
	const tnl::Vector2i GOLD_DISPLAY_WINDOW_POS = { 1000 , 50 };
	//ゴールドを表示する為のウィンドウの幅
	const int GOLD_DISPLAY_WINDOW_WIDTH = 150;
	//ゴールドを表示する為のウィンドウの高さ
	const int GOLD_DISPLAY_WINDOW_HEIGHT = 50;

	//---プレイヤーのステータスウィンドウのサイズ---//

	//ゴールドを表示する為のウィンドウの幅
	const int PLAYER_STATUS_WINDOW_WIDTH = 300;
	//ゴールドを表示する為のウィンドウの高さ
	const int PLAYER_STATUS_WINDOW_HEIGHT = 400;

	//プレイヤーのステータスを表示する座標
	const int PLAYER_STATUS_X = 70;
	const int PLAYER_STATUS_Y = 100;

	//---スキル説明を表示する為のウィンドウの座標とサイズ---//

	//スキル説明を表示する為のウィンドウの座標
	const tnl::Vector2i SKILL_DETAIL_WINDOW_POS = { 350 , 50 };
	//スキル説明を表示する為のウィンドウの幅
	const int SKILL_DETAIL_WINDOW_WIDTH = 300;
	//スキル説明を表示する為のウィンドウの高さ
	const int SKILL_DETAIL_WINDOW_HEIGHT = 250;

	//---選択ウィンドウの定数---//

	//最初の選択ウィンドウ
	enum ConstFirstMenu {
		//道具
		TOOL,
		//特技
		SKILL,
		//強さ
		STATUS,
		//閉じる
		CLOSE,
	};

	//アイテム使用メニュー
	enum ConstItemUseMenu {
		//使う
		USE,
		//詳細
		DETAILE,
		//装備
		EQUIP,
		//捨てる
		DISPOSE,
		//閉じる
		ITEMCLOSE
	};

public:

	//Playerが武器を装備した時に一時的に値を保存しておくための変数を取得する
	int GetEquipAttack()const {
		return equip_attack;
	}
	int GetEquipDefance()const {
		return equip_defance;
	}


	//武器配列を取得する
	std::list<ItemBase>& getEquipArray() {
		return equipped_weapon;
	}

	//------------------------------------------------------------------------------------------------------------------------
	//特技
private:

	//特技用のカーソル
	int skill_cousour = 0;

	//スキルのページでどこのページにいるか
	int skill_curent_page = 0;

	//スキルの要素番号
	int skill_selected_index = 0;


public:

	//特技の描画
	//引数 : スキルを描画する座標 , ページ数を描画する座標 , カーソルのX座標 , アイテムのページ数
	void InventorySkill(const tnl::Vector2i& SKILLDRAWPOS, const tnl::Vector2i& CURENTPAGETEXT, const int COUSOURX, const int ITEMPARPAGE);

	//特技を使用した時の描画
	void SkillDetailDraw(int skill_index);

	//特技のカーソル
	void SkillCousorMove();

	//------------------------------------------------------------------------------------------------------------------------
	//ポインタ
private:

	Shared<Item>item = nullptr;
	Shared<MenuWindow>first_menu = nullptr;
	Shared<MenuWindow>select_action_menu = nullptr;
	Shared<MenuWindow>select_detail_window = nullptr;
	Weak<Menu>menu_window;
	Shared<BattleLog>battleLog = nullptr;
};