#pragma once
#include"../Object/Player.h"
#include"Weapon.h"
#include"Inventory.h"

class UIManger;
class GameManager;

class WeaponShop {
public:

	WeaponShop();
	~WeaponShop();

	void Update(float delta_time);

	void Draw();

	void BuyWeapon(const Shared<Player>& player);

	void WeaponShopCursorMove();
	
//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	Shared<Weapon>weapon_ = nullptr;
	Shared<MenuWindow>use_equip = nullptr;
	Weak<Menu>menuWindow;

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

public:

	//コメントをクリアする
	void buyComentClear() {
		buyWeapon =  Buy_Weapon::ENMPTY;
	}


};