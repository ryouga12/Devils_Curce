#pragma once
#include"../Object/Player.h"
#include"Inventory.h"
#include"Item.h"
#include"../Scene/InMapScene.h"
#include"../../koni_name_space/common/common_value.h"

//---------------------------------------------------------------------------------------------------------
//武器屋を管理するクラス(武器屋での購入や売却などのシステムを担当する)


class WeaponShop final{
public:

	//武器屋の初期化
	//arg_1 : InMapSceneのどこにいるのか
	//どこにいるのかによって武器屋のラインナップを初期化する
	WeaponShop(const InMapScene::InMapState& curent_inmap_scene);
	~WeaponShop() = default;

	//更新処理
	void Update(const float delta_time);

	//描画処理
	void Draw();

	//武器屋の購入か売却なのかを管理する列挙体
	enum class TransactionType {
		EMPTY,
		//購入
		BUY,
		//売却
		SELL
	};

	//売却時の切り替え
	enum class SellAction {
		EMPTY,
		//売価アイテムの選択
		SELLITEMSELECT,
		//売却アイテムの確認
		SELLITEMCHECK
	};


	//武器の購入処理
	//武器屋で購入した際のインベントリへの追加とお金の処理
	void BuyWeapon();

	//武器やアイテムの売却
	void SellItemProcess();

	//武器屋のカーソル処理
	void WeaponShopCursorMove();

	//武器屋の初期化処理
	//引数 : arg_1 InMapの状態
	//引数で指定したマップの武器リストを初期化する
	void WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene);

	//武器屋の状態をセットする
	//プレイヤーが購入か売却のどちらを選んだか
	void SetWeaponShopTransaction(const TransactionType& new_transaction_type) {
		curent_transaction_type = new_transaction_type;
	}

	//武器屋の状態を取得する
	TransactionType GetWeapoShopTransaction()const {
		return curent_transaction_type;
	}

	//シーケンスの状態を待機に戻す
	void WeaponShopChangeIdle() {

		tnl_sequence_.change(&WeaponShop::seqIdle);

		//SellActionが選択画面じゃなければ選択画面に戻す
		if (curent_sell_action != SellAction::SELLITEMSELECT) {
			curent_sell_action = SellAction::SELLITEMSELECT;
		}
	}

//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//シーケンス
	TNL_CO_SEQUENCE(WeaponShop, &WeaponShop::seqIdle);

	//待機状態(主に購入なのか、売却なのかで処理を切り替える)
	bool seqIdle(float delta_time);

	//購入シーケンス
	bool seqBuy(float delta_time);

	//売却シーケンス
	bool seqSell(float delta_time);
	
//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	//ウィンドウ
	Shared<MenuWindow>use_equip = nullptr;
	//アイテム
	Shared<Item>item_ = nullptr;

	//売却コマンド
	Shared<MenuWindow>sell_coment_select = nullptr;
	MenuWindow::MenuElement_t* sell_coment = nullptr;

//------------------------------------------------------------------------------------------------------------------------
private:

	//武器屋の購入か売却なのかを管理する為の変数
	TransactionType curent_transaction_type = TransactionType::EMPTY;

	//今売却時のどのアクションを行っているか
	SellAction curent_sell_action = SellAction::SELLITEMSELECT;

	//武器屋コメント関連
	enum class WeaponshopComent {
		ENMPTY,
		//アイテムが買えた
		BUY,
		//アイテムが買えなかった
		NO,
		//アイテムを売った
		SELL
	};

	//武器の総数
	int weapon_num = 0;

	//カーソルの動き
	int select_cousor = 0;

	WeaponshopComent curent_weapon_shop_coment = WeaponshopComent::ENMPTY;

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

	//1ページあたりのアイテム数
	const int ITEMPERPAGE_ = 5;

	//アイテムを表示する座標
	const tnl::Vector2i ITEM_DRAW_POS = { 145 , 100 };

	//現在のページを表示座標
	const tnl::Vector2i CURENTPAGETEXT = { 100 , 330 };

	//カーソルのx座標
	const int CURSORX = 70;

	//武器商人のコメントの座標
	const tnl::Vector2i ARMSDEALERCOMENT = { 150, 550 };

public:

	//コメントをクリアする
	void BuyComentClear() {
		curent_weapon_shop_coment =  WeaponshopComent::ENMPTY;
	}


};