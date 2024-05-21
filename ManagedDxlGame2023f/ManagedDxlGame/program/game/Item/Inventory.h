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

	//������Item��20����list
	std::vector<ItemBase>InventoryList;

	//�C���x���g���̍ő吔
	int InventoryMaxSize = 20;

	//�A�C�e����ǉ�����֐�
	void AddInventory(int id);

	//�A�C�e���𑕔�����֐�
	void EquipWeapon(int weaponIndex);

	//�A�C�e���̏ڍׂȂǂ̏���
	void ItemDetail();

	//�A�C�e�����̔ԍ��ɂ���ď���������
	void InventoryItemUse(int itemid );


	//�C���x���g�����̃A�C�e���̐����擾����֐�
	inline int GetItemCount() {
		return InventoryList.size();
	}
	
	//�C���x���g���̃��X�g�𑼂̃N���X�Ŏg��
	std::vector<ItemBase>& GetInventoryList() {
		return InventoryList;
	}

	//Player�̃|�C���^���Z�b�g����
	void SetPlayer(std::shared_ptr<Player>player) { plyer = player; }

	//�o�g�����O�̃|�C���^���Z�b�g����
	void SetBattleLog(Shared<BattleLog>battle_log) { battleLog = battle_log; }

	//�A�C�e���̃J�[�\���̈ړ������̍ۂ̃C���f�b�N�X�̑���
	void ItemCurourIndex(int ItemPerPage);

	//�X�L���̃J�[�\���̈ړ������̍ۂ̃C���f�b�N�X�̑���
	void SkillCurourIndex();

	//�C���f�b�N�X��0�ɂ���
	void SelectedIndexClear() {
		selectedIndex = 0;
	}

	//�A�C�e���̃C���f�b�N�X���擾����
	int GetSelectedIndex()const {
		return selectedIndex;
	}

	//�擾�����A�C�e����id���擾����
	int GetSelectedItemId()const {
		return selectedItemId;
	}

	//�X�L���̃C���f�b�N�X���擾����
	int GetSkillSelectedIndex()const {
		return selectedSkillIndex;
	}
	//�X�L��

private:

	//�C���x���g���̐؂�ւ��ׂ̈̃N���X
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

	//�|�[�V�������g�������̉񕜗�
	/*int recovery = 30;*/
	int healAmount = 0;

	//�U���͂��ꎞ�I�ɕۑ�����
	int Attack = 0;
	//�h��͂��ꎞ�I�ɕۑ�����
	int Defance = 0;

	//�v���C���[�̃R���g���[���𐧌䂷��
	int PlyControlTrue = 0; int PlyControlFalse = 1;

public:

	//�A�C�e���̃��j���[
	void ItemMenu(const tnl::Vector2i& itemDrawPos, tnl::Vector2i& curentPageText, int CousourX ,int itemParPage);

	//�C���x���g�����Z�b�g����
	void SetSelect_num(int select_menu);

	//�C���x���g�������擾����
	MenuWindow_I getSelect_menu_num() { return select_menu; }

	//recovery���擾����
	int getRecovery() { return healAmount; }

//------------------------------------------------------------------------------------------------------------------------
//---�֐�---//
public:

	//�J�[�\�����㉺�ɓ������֐�
	void CusorMove();
	//�J�[�\������ԏ�ɖ߂��֐�
	void CursorReset();
	//�y�[�W�̃��Z�b�g
	void CurentPageReset() { currentPage = 0; }
	//�g�p��Hp�񕜌n
	void ItemHpHeal(float percentage , int itemid);
	
//------------------------------------------------------------------------------------------------------------------------
//---���j���[---//
private:

	//���j���[������������
	void InitMenuWinodow();
	//�ŏ��̃��j���[
	void First_Menu();
	//�v���C���[�̃X�e�[�^�X��\�����郁�j���[
	void PlyStatusMenu();
	//����̒��̏ڍׂ�g�����߂̕`��
	void ItemUseMenu();
	//���ǂ̃y�[�W�ɂ��邩
	int currentPage = 0;
	//1�y�[�W������̃A�C�e����
	int item_per_page = 5;
	//���[�U�[���I�������v�f�̃C���f�b�N�X
	//�ŏ��͈�ԏ�Ȃ̂�0�ɐݒ肷��
	int selectedIndex = 0;
	//�擾�����A�C�e����ID���ꎞ�I�ɕێ����邽�߂̕ϐ�
	int selectedItemId = 0;

//�A�C�e���̕`��Ŏg���C���f�b�N�X
//  
	//�ŏ��̔ԍ�
	int start_index = 0;
	//�Ō�̔ԍ�
	int end_index = 0;
	
private:

	enum  {
		EMPTY,
		WEAPON,
		ARMOR
	};

	//�C���x���g�����̃A�C�e����
	int itemNum = 0;

	//�I�𒆂̃J�[�\���̈ʒu
	int selectCursor = 0;

	//����𑕔�����
	std::list<ItemBase> equippedWeapon;
	std::list<ItemBase> equippedArmor;

	//player�̃X�e�[�^�X
	/*Player::PlyerStatus plyerstatus;*/

	//plyerStatus��id�ԍ�
	int PlyerStatus_Id;

	//����̑������\��
	bool equipWeapon = false;
	bool equipArmor = false;

	//����𑕔������ۂ�[E]��\��������W
	tnl::Vector2i equipText = { 80 , 100 };

	//���݂̃y�[�W��\�����W
	tnl::Vector2i CurentPageText = { 100 , 280 };

	//���F
	const int Color_White = -1;

	//�J�[�\���̃T�C�Y
	const float cursorSize = 0.3f;

	//Item��Coment��؂�ւ���ׂ̃N���X
	enum class ItemComent {
		Empty,
		No,
		Use,
	};

	ItemComent item_coment = ItemComent::Empty;

	
	//Player������𑕔��������Ɉꎞ�I�ɒl��ۑ����Ă������߂̕ϐ�
	int equipAttack = 0;
	int equipDefance = 0;

	//���ꂼ��̃V�[�P���X�ł̏���
	void swichInventoryUpdate(float deltatime);

	//�A�C�e����X�L����\��������W
	tnl::Vector2i DrawPos = { 110 , 100 };

	//�J�[�\����x���W
	int cursorX = 70;

	//�A�C�e���̃R�����g�\�����W
	tnl::Vector2i ItemComentPos = { 630 , 100 };

	//�ꎞ�I�ɃA�C�e���ł�����ׂ̍U����
	int AttackAmount = 0;

	//�o�g���V�[���݂̂Ŏg���ׂ̃t���O
	bool BattleFlag = false;

public:

	//Player������𑕔��������Ɉꎞ�I�ɒl��ۑ����Ă������߂̕ϐ����擾����
	int getEquipStatus(int type);

	//����z����擾����
	std::list<ItemBase>& getEquipArray() {
		return equippedWeapon;
	}

	//�オ����Attack�ʂ̒l���擾����
	int getAttackAmount()const {
		return AttackAmount;
	}

	//�t���O��؂�ւ���
	//0 �� true �� 1 �� false
	void BattleFlagSwitch(int id) {
		if (id == 0) {
			BattleFlag = true;
		}
		else {
			BattleFlag = false;
		}
	}

//------------------------------------------------------------------------------------------------------------------------
//���Z
private :

	//���Z�p�̃J�[�\��
	int SkillCousour = 0;

	//���Z�̐�
	int SkillNum = 0;
	
	//�X�L���̃y�[�W�łǂ��̃y�[�W�ɂ��邩
	int SkillCurentPage = 0;

	//�X�L���̗v�f�ԍ�
	int selectedSkillIndex = 0;

	//���Z�p�̃C���x���g��
	std::vector<Shared<Skill>> SkillList;


public:

	//���Z��ǉ�����
	void AddSkill(Shared<Skill>skill);

	//���Z�̕`��
	void InventorySkill();

	//���Z���g�p�������̕`��
	void SkillDetailDraw(int skill_index);

	//���Z�̃J�[�\��
	void SkillCousorMove();

	//�X�L�����Z�b�g����
	void SkillSet();

	//�X�L����ǉ�����n�̃A�C�e��
	template <class T>
	void AddSkillItem(int itemid , Shared<T>skill);

	//�X�L���̔z����擾����
	std::vector<Shared<Skill>>& getSkillList() {
		return SkillList;
	}

	//�X�L���̌����擾����
	int getSkillNum() {
		return SkillNum;
	}

//------------------------------------------------------------------------------------------------------------------------
//�|�C���^
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