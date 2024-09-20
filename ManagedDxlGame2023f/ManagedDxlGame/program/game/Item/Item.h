#pragma once
///
///�A�C�e�����Ǘ�����N���X
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class GameManager;
class Skill;
class BattleLog;
class CsvManager;

class Item final: public  ItemBase{
public:
	
	Item();
	~Item()=default;

	//�A�C�e���̔z����擾����
	const std::list<ItemBase>&GetItemArray()const {
		return item_status_type;
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
	
	//�\���̂̑���
	int item_status_total_num;

	//�\���̂��i�[����z��
	std::list<ItemBase>item_status_type;

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
	//arg_1 : �A�C�e����ID
	//�A�C�e����ID������Ƃ���ɊY�������A�C�e�����擾����
	ItemBase GetItemById(const int& id) const;

	//�A�C�e�����g�p�������̏���
	//arg_1 : �A�C�e����ID
	//�A�C�e����ID�ɂ���ď����𕪂���
	void ItemUse(const int& itemid);

	//�g�p��Hp�񕜌n
	//arg_1 : �{��
	//arg_2 : �A�C�e����ID
	void ItemHpHeal(const float& percentage, const int& itemid);

	//�o�g�����O�̃|�C���^���Z�b�g����
	//arg_1 : �o�g�����O��Shared�|�C���^
	void SetBattleLog(const Shared<BattleLog>& battle_log) { battle_log_ = battle_log; }

	//�X�L����ǉ�����n�̃A�C�e��
	template <class T>
	void AddSkillItem(const int& itemid, Shared<T> skill);

	//�t���O��؂�ւ���
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

