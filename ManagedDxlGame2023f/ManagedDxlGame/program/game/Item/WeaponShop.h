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
//�|�C���^
private:

	Shared<Weapon>weapon_ = nullptr;
	Shared<MenuWindow>use_equip = nullptr;
	Weak<Menu>menuWindow;

//------------------------------------------------------------------------------------------------------------------------
//����֌W
private:

	//���킪��������
	enum class Buy_Weapon {
		ENMPTY,
		BUY,
		NO,
	};

	//����̑���
	int weapon_num = 0;

	//�J�[�\���̓���
	int select_cousor = 0;

	//�J�[�\���̍��W
	int cousor_y = 0;

	Buy_Weapon buyWeapon = Buy_Weapon::ENMPTY;

public:

	//�R�����g���N���A����
	void buyComentClear() {
		buyWeapon =  Buy_Weapon::ENMPTY;
	}


};