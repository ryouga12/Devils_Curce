#pragma once
#include"../Item/Item.h"
#include"../Object/Player.h"
#include"../Manager/ResourceManager.h"
#include"../Menu/MenuWindow.h"
#include"../Manager/SoundManager.h"
#include"../Skill/Skill.h"
#include"../../koni_name_space/common/common_value.h"

class ItemBase;

//---------------------------------------------------------------------------------------------------------
//�C���x���g�����Ǘ�����N���X


class Inventory final
{
public:

	Inventory();
	~Inventory() = default;

	//�X�V����
	void Update(const float& delta_time);
	//�`�揈��
	void draw();

	//�A�C�e����ǉ�����֐�
	//���� : arg_1 item_id
	//�����ɃA�C�e����ID�����鎖�ŃC���x���g���ɃA�C�e�����i�[�����
	void AddInventory(const int& id);

	//�A�C�e���𑕔�����֐�
	//���� : arg_1 ����̃C���f�b�N�X
	//�����ɃC���f�b�N�X������ƕ���𑕔��ł���
	void EquipWeapon(const int& weaponIndex);

	//�C���x���g���̍ő吔���擾����
	int GetInventoryMaxSize()const {
		return INVENTORY_MAX_SIZE_;
	}

	//�A�C�e���̏ڍׂ�\������
	void ItemDetail();

	//�C���x���g���̃��X�g�𑼂̃N���X�Ŏg��
	std::vector<ItemBase>& GetInventoryList() {
		return inventory_list;
	}

	//�A�C�e���̃J�[�\���̈ړ������̍ۂ̃C���f�b�N�X�̑���
	void ItemCurourIndex(const int& ItemPerPage);

	//�C���f�b�N�X��0�ɂ���
	void SelectedIndexClear() {
		selected_index = 0;
	}

	//�A�C�e���̃C���f�b�N�X���擾����
	int GetSelectedIndex()const {
		return selected_index;
	}

	//�擾�����A�C�e����id���擾����
	int GetSelectedItemId()const {
		return selected_item_id;
	}

	//�X�L���̃C���f�b�N�X���擾����
	int GetSkillSelectedIndex()const {
		return skill_selected_index;
	}

	//�C���x���g���̐؂�ւ��ׂ̈̃N���X
	enum class MenuWindow_I {
		EMPTY,
		//�ŏ��̃��j���[
		FIRSTMENU,
		//�A�C�e�����j���[
		ITEMMENU,
		//�X�e�[�^�X���j���[
		STATUSMENU,
		//�A�C�e���g�p����ۂ̃��j���[
		ITEMUSEMENU,
		//�A�C�e�������E�B���h�E
		ITEMDETAILMENU,
		//�X�L���\�����j���[
		SKILLMENU,
		//�X�L�������E�B���h�E
		SKILLDATAILMENU
	};

	//�C���x���g���̃��j���[���Z�b�g����
	//���� : arg_1 �ǂ̃E�B���h�E�ɐ؂�ւ���̂�
	//���̏ꏊ�ŃE�B���h�E��؂�ւ���ׂ̊֐�
	void InventoryMenuChange(const MenuWindow_I& new_window) {
		select_menu = new_window;
	}

	//�w�肵��ID�̃A�C�e�����C���x���g������폜����
	void InventoryItemRemove(const int& item_id);

	//�A�C�e�����폜����������𑕔����Ă����ۂ̏���
	void DeleteEquipItemProcess();

	//�A�C�e���̃��j���[�`��
	//arg_1 : �A�C�e���̍��W, 
	//arg_2 : �y�[�W����\��������W, 
	//arg_3 : �J�[�\����X���W , 
	//arg_4 : �y�[�W��
	//�o�g���V�[����}�b�v�V�[���ŃC���x���g���̍��W���Ⴄ�׎g���������
	void ItemMenu(const tnl::Vector2i& itemDrawPos, const tnl::Vector2i& curentPageText, const int& CousourX, const int& itemParPage);

	//�C���x���g���̏������擾����
	MenuWindow_I GetSelectMenuNum() { return select_menu; }

	//�A�C�e���̐������炷
	void DelateItemNum() {
		item_num--;
	}

	//�J�[�\�����㉺�ɓ������֐�
	void CusorMove();
	//�J�[�\������ԏ�ɖ߂��֐�
	void CursorReset();
	//�y�[�W�̃��Z�b�g
	void CurentPageReset() { curent_page = 0; }

	//Player������𑕔��������Ɉꎞ�I�ɒl��ۑ����Ă������߂̕ϐ����擾����
	int GetEquipAttack()const {
		return equip_attack;
	}
	int GetEquipDefance()const {
		return equip_defance;
	}


	//����z����擾����
	std::list<ItemBase>& getEquipArray() {
		return equipped_weapon;
	}

	//------------------------------------------------------------------------------------------------------------------------
	//---���j���[---//
private:

	//���j���[������������
	void InitMenuWinodow();

	//�C���x���g������Ă��鎞�v���C���[�������Ă��鎞�̃��j���[�\��
	void Game_Menu();

	//�ŏ��̃��j���[
	void First_Menu();

	//�v���C���[�̃X�e�[�^�X��\�����郁�j���[
	void PlyStatusMenu();

	//����̒��̏ڍׂ�g�����߂̕`��
	void ItemUseMenu();

	//���ǂ̃y�[�W�ɂ��邩
	int curent_page = 0;

	//1�y�[�W������̃A�C�e����
	const int ITEMPERPAGE = 5;

	//���[�U�[���I�������v�f�̃C���f�b�N�X
	//�ŏ��͈�ԏ�Ȃ̂�0�ɐݒ肷��
	int selected_index = 0;

	//�擾�����A�C�e����ID���ꎞ�I�ɕێ����邽�߂̕ϐ�
	int selected_item_id = 0;

	//�A�C�e���̕`��Ŏg���C���f�b�N�X
	  
	//�ŏ��̔ԍ�
	int start_index = 0;

	//�Ō�̔ԍ�
	int end_index = 0;

private:

	//�A�C�e�����̂Ă邪�I�����ꂽ���̏���
	void DisposeItemProcess();

	//�ǂ̃��j���[�ɂ��邩
	MenuWindow_I select_menu = MenuWindow_I::EMPTY;

	//����̎��
	enum EquipItem{
		//�f��
		EMPTY,
		//����
		WEAPON,
		//�h��
		ARMOR
	};

	//�C���x���g�����̃A�C�e����
	int item_num = 0;

	//�I�𒆂̃J�[�\���̈ʒu
	int select_cursor = 0;

	//����𑕔�����
	std::list<ItemBase> equipped_weapon;
	std::list<ItemBase> equipped_armor;

	//����̑������\��
	bool equip_weapon = false;
	bool equip_armor = false;

	//Player������𑕔��������Ɉꎞ�I�ɒl��ۑ����Ă������߂̕ϐ�
	int equip_attack = 0;
	int equip_defance = 0;

	//���ꂼ��̃V�[�P���X�ł̏���
	void swichInventoryUpdate();

	//�ꎞ�I�ɃA�C�e���ł�����ׂ̍U����
	int attack_amount = 0;

	//�g�p�ł���A�C�e�����ł��Ȃ��A�C�e�����̃t���O(��ɃR�����g�Ŏg�p)
	bool can_used_item = true;

	//---�E�B���h�E�̍��W�֘A---//

	//---���j���[�E�B���h�E�̍��W�ƃT�C�Y---//

	//���j���[�E�B���h�E�̍��W
	const tnl::Vector2i MENU_WINDOW_POS = { 50, 50, };
	//���j���[�E�B���h�E�̕�
	const int MENU_WINDOW_WIDTH = 300;
	//���j���[�E�B���h�E�̍���
	const int MENU_WINDOW_HEIGHT = 270;
	//�ŏ��̃E�B���h�E�̕�
	const int FIRST_MENU_WINDOW_WIDTH = 250;
	//�ŏ��̃E�B���h�E�̍���
	const int FIRST_MENU_WINDOW_HEIGHT = 250;

	//�ŏ���ESC���������Ƃ��ɏo��ŏ��̃��j���[�E�B���h�E�̍��W

	//����
	const tnl::Vector2i MENU_TOOL_POS = { 100 , 100 };
	//���Z
	const tnl::Vector2i MENU_SKILL_POS = { 100 , 150 };
	//����
	const tnl::Vector2i MENU_STATUS_POS = { 100 , 200 };
	//����
	const tnl::Vector2i MENU_CLOSE_POS = { 100 , 250 };

	//�A�C�e���E�B���h�E���őI�����郁�j���[

	//�g��
	const tnl::Vector2i MENU_ITEM_USE_POS = { 400 , 90 };
	//�ڍ�
	const tnl::Vector2i MENU_ITEM_DETAIL_POS = { 400, 140 };
	//��������
	const tnl::Vector2i MENU_ITEM_EQUIP_POS = { 400 , 190 };
	//�̂Ă�
	const tnl::Vector2i MENU_ITEM_DISPOSE_POS = { 400 , 240 };
	//����
	const tnl::Vector2i MENU_ITEM_CLOSE_POS = { 400 , 290 };


	//---�A�C�e�������֘A�̍��W�ƃT�C�Y---//

	//�A�C�e���E�B���h�E�̍��W
	const tnl::Vector2i ITEM_DETAIL_WINDOW_POS = { 600, 50, };
	//�A�C�e�������E�B���h�E�̕�
	const int ITEM_DETAIL_WINDOW_WIDTH = 400;
	//�A�C�e�������E�B���h�E�̍���
	const int ITEM_DETAIL_WINDOW_HEIGHT = 250;
	//�A�C�e���̐������������W
	const tnl::Vector2i ITEM_DETAIL_POS = { 630 , 100 };
	//�A�C�e���̃X�e�[�^�X���������W
	const tnl::Vector2i ITEM_STAUS_POS = { 630 , 250 };
	//���炷���W
	const int ITEM_ADD_OFSET_ = 100;
	//�A�C�e���̎g����ڍׂ��J�����肷��E�B���h�E�̍��W�ƕ��ƍ���
	const tnl::Vector2i ITEM_USE_POS = { 350 , 50 };
	//��
	const int ITEM_USE_WINDOW_WIDTH = 250;
	//����
	const int ITEM_USE_WINDOW_HEIGHT = 280;

	//---�S�[���h��\������ׂ̃E�B���h�E�̍��W�ƃT�C�Y---//

	//�S�[���h��\������ׂ̃E�B���h�E�̍��W
	const tnl::Vector2i GOLD_DISPLAY_WINDOW_POS = { 1000 , 50 };
	//�S�[���h��\������ׂ̃E�B���h�E�̕�
	const int GOLD_DISPLAY_WINDOW_WIDTH = 150;
	//�S�[���h��\������ׂ̃E�B���h�E�̍���
	const int GOLD_DISPLAY_WINDOW_HEIGHT = 50;

	//�S�[���h�����̕\�����W
	const tnl::Vector2i GOLD_STRING_POS = { 1030, 65 };

	//---�v���C���[�̃X�e�[�^�X�E�B���h�E�̃T�C�Y---//

	//�S�[���h��\������ׂ̃E�B���h�E�̕�
	const int PLAYER_STATUS_GOLD_WINDOW_WIDTH = 300;
	//�S�[���h��\������ׂ̃E�B���h�E�̍���
	const int PLAYER_STATUS_GOLD_WINDOW_HEIGHT = 400;

	//�v���C���[�̃X�e�[�^�X��\��������W
	const int PLAYER_STATUS_X = 70;
	const int PLAYER_STATUS_Y = 100;

	//�v���C���[�X�e�[�^�X�̃E�B���h�E�̍��W
	const tnl::Vector2i PLAYER_STATUS_WINDOW_POS = { 50, 50 };

	//�v���C���[�X�e�[�^�X�E�B���h�E�̃T�C�Y
	const int PLAYER_STATUS_WINDOW_WIDTH = 250;
	const int PLAYER_STATUS_WINDOW_HEIGHT = 250;

	//---�X�L��������\������ׂ̃E�B���h�E�̍��W�ƃT�C�Y---//

	//�X�L��������\������ׂ̃E�B���h�E�̍��W
	const tnl::Vector2i SKILL_DETAIL_WINDOW_POS = { 350 , 50 };
	//�X�L��������\������ׂ̃E�B���h�E�̕�
	const int SKILL_DETAIL_WINDOW_WIDTH = 350;
	//�X�L��������\������ׂ̃E�B���h�E�̍���
	const int SKILL_DETAIL_WINDOW_HEIGHT = 250;
	//�X�L���̐����̕�����\��������W
	const tnl::Vector2i SKILL_DETAIL_POS = { 380 , 100 };
	//�X�L���̕K�v��MP��\��������W
	const int SKILL_CONSME_POS_Y = 250;

	//---����L�[�̐������W---//

	//�߂�ׂ̐����E�B���h�E�̍��W
	const tnl::Vector2i BUTTON_DETAIL_WINDOW_POS = { 50, 0 };
	//�߂�ׂ̐����E�B���h�E�̃T�C�Y
	const int BUTTON_DETAIL_WINDOW_WIDTH = 200;
	const int BUTTON_DETAIL_WINDOW_HEIGHT = 50;
	//�߂�e�L�X�g�̕`����W
	const tnl::Vector2i BUTTON_BACK_STRING_POS = { 70, 15 };


	//---�萔�֘A---//

	//�ŏ��̑I���E�B���h�E
	enum ConstFirstMenu {
		//����
		TOOL,
		//���Z
		SKILL,
		//����
		STATUS,
		//����
		CLOSE
	};

	//�A�C�e���g�p���j���[
	enum ConstItemUseMenu {
		//�g��
		USE,
		//�ڍ�
		DETAILE,
		//����
		EQUIP,
		//�̂Ă�
		DISPOSE,
		//����
		ITEMCLOSE
	};

	//�܂̍ő�y�[�W��
	const int ITEM_MAX_PAGE = 4;

	//�J�[�\����x���W
	const int CURSORX = 70;

	//����𑕔������ۂ�[E]��\��������W
	const tnl::Vector2i EQUIPTEXT = { 80 , 100 };

	//���݂̃y�[�W��\�����W
	const tnl::Vector2i CURENTPAGETEXT = { 100 , 280 };

	//�J�[�\���̃T�C�Y
	const float CURSORSIZE = 0.3f;

	//�A�C�e����X�L����\��������W
	const tnl::Vector2i DRAWPOS = { 110 , 100 };

	//�A�C�e���̃R�����g�\�����W
	const tnl::Vector2i ITEMCOMENTPOS = { 630 , 100 };

	//������Item��20����list
	std::vector<ItemBase>inventory_list;

	//�C���x���g���̍ő吔
	const int INVENTORY_MAX_SIZE_ = 20;

//------------------------------------------------------------------------------------------------------------------------
//���Z
private:

	//���Z�p�̃J�[�\��
	int skill_cousour = 0;

	//�X�L���̃y�[�W�łǂ��̃y�[�W�ɂ��邩
	int skill_curent_page = 0;

	//�X�L���̗v�f�ԍ�
	int skill_selected_index = 0;


public:

	//���Z�̕`��
	//arg_1 : �X�L����`�悷����W
	//arg_2 : �y�[�W����`�悷����W
	//arg_3 : �J�[�\����X���W
	//arg_4 : �A�C�e���̃y�[�W��
	void InventorySkill(const tnl::Vector2i& SKILLDRAWPOS, const tnl::Vector2i& CURENTPAGETEXT, const int& COUSOURX, const int& ITEMPARPAGE);

	//���Z���g�p�������̕`��
	//arg_1 : �X�L���̃C���f�b�N�X
	void SkillDetailDraw(const int& skill_index);

	//���Z�̃J�[�\��
	void SkillCousorMove();

	//�X�L���̃J�[�\���̈ړ������̍ۂ̃C���f�b�N�X�̑���
	void SkillCurourIndex(const int& skil_list_perpage);

//------------------------------------------------------------------------------------------------------------------------
//�|�C���^
private:

	//�A�C�e��
	Shared<Item>item = nullptr;
	//�ŏ��̃E�B���h�E
	Shared<MenuWindow>first_menu = nullptr;
	//�U���I���E�B���h�E
	Shared<MenuWindow>select_action_menu = nullptr;
	//�����E�B���h�E
	Shared<MenuWindow>select_detail_window = nullptr;
	//�o�g�����O
	Shared<BattleLog>battleLog = nullptr;
};