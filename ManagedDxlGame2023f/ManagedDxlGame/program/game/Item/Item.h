#pragma once
///
///�A�C�e�����Ǘ�����N���X
/// 
#include<string>
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class GameManager;
class Skill;
class BattleLog;
class CsvManager;

class Item : public  ItemBase{
public:
	
	Item();
	~Item();

	void update(float delta_time);
	void draw();

	//�A�C�e���̔z����擾����
	const std::list<ItemBase>&getItemArray()const {
		return m_Item_Status_Type;
	}

	//�g�p�ł���A�C�e��
	enum class UsedItem {
		//�|�[�V����
		PORTION = 3,
		//�n�C�|�[�V����
		HIPORTION = 8,
		//�}�W�b�N�|�[�V����
		MAGICPORTION = 14,
		//�A�^�b�N�|�[�V����
		ATTACKPORTION = 16,
		//�v�����g�G�L�X(�񕜃A�C�e��)
		PLANTEXTRACT = 17,
		//�X���C���̗�
		//�g�p����ƃX�L������ɓ������
		SLIMBELL = 20,
		//�X�l�[�N�̗�
		//�g�p����ƃX�L������ɓ������
		SNAKEBELL = 21,
	};

private:

	//�ǂݍ��݊֐�
	void ItemLoadTypeInfo();
	
	//model������ϐ�
	ItemBase item_Status_ary;

	//�\���̂̑���
	int m_item_Status_Total_num;

	//�\���̂��i�[����z��
	std::list<ItemBase>m_Item_Status_Type;

	//�A�C�e���Ȃǂ̐������Ȃǂ�����z��
	std::string ItemDisc = {};

	//item�̍��W
	tnl::Vector3 item_pos = {};

	//�o�g���V�[���݂̂Ŏg���ׂ̃t���O
	bool BattleFlag = false;

	//�o�g�����O�̃|�C���^
	Shared<BattleLog>battle_log_ = nullptr;

	//�񕜗�
	int healAmount = 0;

	//�ꎞ�I�ɃA�C�e���ł�����ׂ̍U����
	int AttackAmount = 0;

public:

	//ID���w�肵�ăA�C�e�����擾����֐�
	//���� : �A�C�e����ID
	//�A�C�e����ID������Ƃ���ɊY�������A�C�e�����擾����
	ItemBase GetItemById(int id) const;

	//�A�C�e�����g�p�������̏���
	void ItemUse(int itemid);

	//�g�p��Hp�񕜌n
	void ItemHpHeal(const float& percentage, int itemid);

	//�o�g�����O�̃|�C���^���Z�b�g����
	void SetBattleLog(Shared<BattleLog>battle_log) { battle_log_ = battle_log; }

	//�A�C�e�����C���x���g�����ɑ��݂�����C���x���g�������������
	void ItemRemove(int itemid);

	//�X�L����ǉ�����n�̃A�C�e��
	template <class T>
	void AddSkillItem(int itemid, Shared<T>skill);

	//�オ����Attack�ʂ̒l���擾����
	int getAttackAmount()const {
		return AttackAmount;
	}

	//recovery���擾����
	int getRecovery() { return healAmount; }

	//�t���O��؂�ւ���
	//0 �� true �� 1 �� false
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

