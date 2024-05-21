#pragma once
#include"../Object/Player.h"
#include"Weapon.h"
#include"Inventory.h"

class WeaponShop {
public:

	WeaponShop();
	~WeaponShop();

	void Update(float delta_time);

	void Draw();

	void BuyWeapon(const Shared<Player>& player);

	void WeaponShopCursorMove();

	void setInventory(std::shared_ptr<Inventory> i) { inventory = i; }
	
//------------------------------------------------------------------------------------------------------------------------
//ポインタ
private:

	Shared<Weapon>weapon_ = nullptr;
	Shared<Inventory>inventory = nullptr;
	Shared<Player>myplayer = nullptr;
	Shared<MenuWindow>use_equip = nullptr;
	Shared<Menu>menuWindow = nullptr;

//------------------------------------------------------------------------------------------------------------------------
//武器関係
private:

	//武器が買えたか
	enum class B_Weapon {
		empty,
		buy,
		no,
	};

	//武器の総数
	int WeaponNum = 0;

	//カーソルの動き
	int SelectCousor = 0;

	//カーソルの座標
	int coursorY = 0;

	B_Weapon buyWeapon = B_Weapon::empty;

public:

	void buyComentClear() {
		buyWeapon =  B_Weapon::empty;
	}


};