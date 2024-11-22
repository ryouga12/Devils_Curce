#pragma once
#include"../Object/Player.h"
#include"Inventory.h"
#include"Item.h"
#include"../Scene/InMapScene.h"
#include"../../koni_name_space/common/common_value.h"

//---------------------------------------------------------------------------------------------------------
//���퉮���Ǘ�����N���X(���퉮�ł̍w���┄�p�Ȃǂ̃V�X�e����S������)


class WeaponShop final{
public:

	//���퉮�̏�����
	//arg_1 : InMapScene�̂ǂ��ɂ���̂�
	//�ǂ��ɂ���̂��ɂ���ĕ��퉮�̃��C���i�b�v������������
	WeaponShop(const InMapScene::InMapState& curent_inmap_scene);
	~WeaponShop() = default;

	//�X�V����
	void Update(const float delta_time);

	//�`�揈��
	void Draw();

	//���퉮�̍w�������p�Ȃ̂����Ǘ�����񋓑�
	enum class TransactionType {
		EMPTY,
		//�w��
		BUY,
		//���p
		SELL
	};

	//���p���̐؂�ւ�
	enum class SellAction {
		EMPTY,
		//�����A�C�e���̑I��
		SELLITEMSELECT,
		//���p�A�C�e���̊m�F
		SELLITEMCHECK
	};


	//����̍w������
	//���퉮�ōw�������ۂ̃C���x���g���ւ̒ǉ��Ƃ����̏���
	void BuyWeapon();

	//�����A�C�e���̔��p
	void SellItemProcess();

	//���퉮�̃J�[�\������
	void WeaponShopCursorMove();

	//���퉮�̏���������
	//���� : arg_1 InMap�̏��
	//�����Ŏw�肵���}�b�v�̕��탊�X�g������������
	void WeaponShopInit(const InMapScene::InMapState& curent_inmap_scene);

	//���퉮�̏�Ԃ��Z�b�g����
	//�v���C���[���w�������p�̂ǂ����I�񂾂�
	void SetWeaponShopTransaction(const TransactionType& new_transaction_type) {
		curent_transaction_type = new_transaction_type;
	}

	//���퉮�̏�Ԃ��擾����
	TransactionType GetWeapoShopTransaction()const {
		return curent_transaction_type;
	}

	//�V�[�P���X�̏�Ԃ�ҋ@�ɖ߂�
	void WeaponShopChangeIdle() {

		tnl_sequence_.change(&WeaponShop::seqIdle);

		//SellAction���I����ʂ���Ȃ���ΑI����ʂɖ߂�
		if (curent_sell_action != SellAction::SELLITEMSELECT) {
			curent_sell_action = SellAction::SELLITEMSELECT;
		}
	}

//------------------------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�V�[�P���X
	TNL_CO_SEQUENCE(WeaponShop, &WeaponShop::seqIdle);

	//�ҋ@���(��ɍw���Ȃ̂��A���p�Ȃ̂��ŏ�����؂�ւ���)
	bool seqIdle(float delta_time);

	//�w���V�[�P���X
	bool seqBuy(float delta_time);

	//���p�V�[�P���X
	bool seqSell(float delta_time);
	
//------------------------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�E�B���h�E
	Shared<MenuWindow>use_equip = nullptr;
	//�A�C�e��
	Shared<Item>item_ = nullptr;

	//���p�R�}���h
	Shared<MenuWindow>sell_coment_select = nullptr;
	MenuWindow::MenuElement_t* sell_coment = nullptr;

//------------------------------------------------------------------------------------------------------------------------
private:

	//���퉮�̍w�������p�Ȃ̂����Ǘ�����ׂ̕ϐ�
	TransactionType curent_transaction_type = TransactionType::EMPTY;

	//�����p���̂ǂ̃A�N�V�������s���Ă��邩
	SellAction curent_sell_action = SellAction::SELLITEMSELECT;

	//���퉮�R�����g�֘A
	enum class WeaponshopComent {
		ENMPTY,
		//�A�C�e����������
		BUY,
		//�A�C�e���������Ȃ�����
		NO,
		//�A�C�e���𔄂���
		SELL
	};

	//����̑���
	int weapon_num = 0;

	//�J�[�\���̓���
	int select_cousor = 0;

	WeaponshopComent curent_weapon_shop_coment = WeaponshopComent::ENMPTY;

	//������i�[����z��
	std::vector<ItemBase>weponList;

	//�J�[�\���̃T�C�Y
	const float CURSOR_SIZE = 0.3f;

	//�C���x���g���̍ő�T�C�Y��
	const int INVENTORY_MAX_SIZE = 20;


	//---���W�n---//

	//�E�B���h�E��\��������W
	const tnl::Vector2i MENU_WINDOW_POS = { 50 ,50 };

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

	//1�y�[�W������̃A�C�e����
	const int ITEMPERPAGE_ = 5;

	//�A�C�e����\��������W
	const tnl::Vector2i ITEM_DRAW_POS = { 145 , 100 };

	//���݂̃y�[�W��\�����W
	const tnl::Vector2i CURENTPAGETEXT = { 100 , 330 };

	//�J�[�\����x���W
	const int CURSORX = 70;

	//���폤�l�̃R�����g�̍��W
	const tnl::Vector2i ARMSDEALERCOMENT = { 150, 550 };

public:

	//�R�����g���N���A����
	void BuyComentClear() {
		curent_weapon_shop_coment =  WeaponshopComent::ENMPTY;
	}


};