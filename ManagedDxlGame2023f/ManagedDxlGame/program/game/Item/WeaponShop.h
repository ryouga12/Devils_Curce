#pragma once
#include"../Object/Player.h"
#include"Inventory.h"
#include"Item.h"
#include"../Scene/InMapScene.h"
#include"../../koni_name_space/common/common_value.h"

class UIManger;
class GameManager;

class WeaponShop final{
public:

	WeaponShop() = default;
	WeaponShop(const InMapScene::InMapState& curent_inmap_scene);
	~WeaponShop();

	void Draw();

	//武器の購入処理
	//引数 : arg_1 プレイヤーのポインタ
	//武器屋で購入した際のインベントリへの追加とお金の処理
	void BuyWeapon(const Shared<Player>& player);

	//武器屋のカーソル処理
	void WeaponShopCursorMove();

	//武器屋の初期化処理
	//引数 : arg_1 InMapの状態
	//引数で指定したマップの武器リストを初期化する
	void WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene);

	//武器の値を他のクラスで使う
	std::vector<ItemBase>& GetWeaponValue()
	{
		return weponList;
	}

	//武器リストの数を取得する
	std::vector<ItemBase>& GetWeaponList() {
		return weponList;
	}
	
//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//ウィンドウ
	Shared<MenuWindow>use_equip = nullptr;
	Weak<Menu>menuWindow;
	Shared<Item>item_ = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//武器関係
private:

	//武器が買えたか
	enum class Buy_Weapon {
		ENMPTY,
		BUY,
		NO,
	};

	//武器の総数
	int weapon_num = 0;

	//カーソルの動き
	int select_cousor = 0;

	//カーソルの座標
	int cousor_y = 0;

	Buy_Weapon buyWeapon = Buy_Weapon::ENMPTY;

	//武器を格納する配列
	std::vector<ItemBase>weponList;

	//カーソルのサイズ
	const float CURSOR_SIZE = 0.3f;

	//インベントリの最大サイズ数
	const int INVENTORY_MAX_SIZE = 20;


	//---座標系---//

	//ウィンドウを表示する座標
	const tnl::Vector2i MENU_WINDOW_POS = { 50 ,50 };

	//ウィンドウの幅
	const int MENU_WINDOW_WIDTH = 350;
	//ウィンドウの高さ
	const int MENU_WINDOW_HEIGHT = 400;

	//武器屋文字座標
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 110 , 70 };

	//武器関連の文字の表示座標
	const tnl::Vector2i WEAPON_CONNECTION_POS = { 85 , 130 };

	//文字の間隔
	const int SPACING = 40;

	//購入時の選択文字の座標
	const tnl::Vector2i PROMPT_POS = { 85 , 380 };

	//武器商人のコメント文字座標
	const tnl::Vector2i MESSAGE_POS = { 150 , 620 };

	//カーソルの座標
	const tnl::Vector2i CURSOR_POS = { 70 ,140 };

public:

	//コメントをクリアする
	void buyComentClear() {
		buyWeapon =  Buy_Weapon::ENMPTY;
	}


};