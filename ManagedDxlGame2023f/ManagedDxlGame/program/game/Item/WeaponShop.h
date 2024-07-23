#pragma once
#include"../Object/Player.h"
#include"Inventory.h"
#include"Item.h"
#include"../Scene/InMapScene.h"

class UIManger;
class GameManager;

class WeaponShop {
public:

	WeaponShop(const InMapScene::InMapState& curent_inmap_scene);
	~WeaponShop();

	void Update(float delta_time);

	void Draw();

	//����̍w������
	//���� : �v���C���[�̃|�C���^
	//���퉮�ōw�������ۂ̃C���x���g���ւ̒ǉ��Ƃ����̏���
	void BuyWeapon(const Shared<Player>& player);

	//���퉮�̃J�[�\������
	void WeaponShopCursorMove();

	//���퉮�̏���������
	//���� : InMap�̏��
	//�����Ŏw�肵���}�b�v�̕��탊�X�g������������
	void WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene);

	//����̒l�𑼂̃N���X�Ŏg��
	std::vector<ItemBase>& GetWeaponValue()
	{
		return weponList;
	}

	//���탊�X�g�̐����擾����
	std::vector<ItemBase>& GetWeaponList() {
		return weponList;
	}
	
//------------------------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�E�B���h�E
	Shared<MenuWindow>use_equip = nullptr;
	Weak<Menu>menuWindow;
	Shared<Item>item_ = nullptr;

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

	//������i�[����z��
	std::vector<ItemBase>weponList;

	//��
	const int COLOR_WHITE = -1;

	//�J�[�\���̃T�C�Y
	const float CURSOR_SIZE = 0.3;

	//�C���x���g���̍ő�T�C�Y��
	const int INVENTORY_MAX_SIZE = 20;


	//---���W�n---//

	//�E�B���h�E��\��������W
	tnl::Vector2i MENU_WINDOW_POS = { 50 ,50 };

	//�E�B���h�E�̕�
	const int MENU_WINDOW_WIDTH = 350;
	//�E�B���h�E�̍���
	const int MENU_WINDOW_HEIGHT = 400;

	//���퉮�������W
	const tnl::Vector2i WEAPON_SHOP_STRING_POS = { 110 , 70 };

	//����֘A�̕����̕\�����W
	const tnl::Vector2i WEAPON_CONNECTION_POS = { 85 , 130 };

	//�����̊Ԋu
	const int SPACING = 40;

	//�w�����̑I�𕶎��̍��W
	const tnl::Vector2i PROMPT_POS = { 85 , 380 };

	//���폤�l�̃R�����g�������W
	const tnl::Vector2i MESSAGE_POS = { 150 , 620 };

	//�J�[�\���̍��W
	const tnl::Vector2i CURSOR_POS = { 70 ,140 };

public:

	//�R�����g���N���A����
	void buyComentClear() {
		buyWeapon =  Buy_Weapon::ENMPTY;
	}


};