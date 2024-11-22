#pragma once
///
///�A�C�e�����Ǘ�����N���X
/// 


#include "../../dxlib_ext/dxlib_ext.h"
#include"../Manager/ResourceManager.h"
#include"../Item/ItemBase.h"
#include"../Object/Actor.h"

class Skill;
class BattleLog;
class KonCamera;

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
		//�����̃X�L����i���������`��(��Ή��a��)
		GREATFLAMESLASH = 42,
		//�����̃X�L����i���������`��(�l�v�`���[���X�p�C����)
		WATERFORGE = 43
	};

private:

	//�ǂݍ��݊֐�
	void ItemLoadTypeInfo();
	
	//�\���̂̑���
	int item_status_total_num = 0;

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
	bool ItemUse(const int& itemid);

	//�g�p��Hp�񕜌n
	//arg_1 : �{��
	//arg_2 : �A�C�e����ID
	bool ItemHpHeal(const int& percentage, const int& itemid);

	//�o�g�����O�̃|�C���^���Z�b�g����
	//arg_1 : �o�g�����O��Shared�|�C���^
	void SetBattleLog(const Shared<BattleLog>& battle_log) { battle_log_ = battle_log; }

	//�X�L����ǉ�����n�̃A�C�e��
	bool AddSkillItem(const int& itemid, const Shared<Skill>& skill);

	//�X�L����i��������A�C�e��(��`��)
	bool EvolutionSkillItem(const int& item_id, const Shared<Skill>& skill_);

	//��`���A�C�e���ɉ����ăX�L��ID��Ԃ�
	int EvolutionSkillID(const int& evolution_item_id)const;

	//�t���O��؂�ւ���
	void BattleFlagSwitch() {
		BattleFlag = !BattleFlag;
	}

};

//�����蔻��̂���A�C�e��
class CollisonItem : public Actor {
public:

	//�����蔻��̂���A�C�e��
	//arg_1 : ID
	//arg_2 : X���W(float)
	//arg_3 : Y���W(float)
	//arg_4 : �`�b�v�T�C�Y
	//arg_5 : �{��
	//arg_6 : �摜�n���h��
	//arg_7 : ����ł���A�C�e�����ǂ����̃t���O
	CollisonItem(const int& id, const float& pos_x, const float& pos_y, const int& CHIPSIZE, const float& scale, const int& ghdl, const bool canBeAcquired);

	//�`��
	void Draw(const KonCamera& camera);
	
	//�����蔻��̂���A�C�e����ID���擾����
	int GetCollisonItemID()const {
		return item_id;
	}

	//�A�C�e���̍��W���擾����
	const tnl::Vector3& GetCollisonItemPos()const {
		return item_pos;
	}
	
	//�A�C�e���̃`�b�v�T�C�Y���擾����
	int GetCollsionItemChipSize()const {
		return chip_size;
	}

	//�A�C�e���̃X�P�[�����擾����
	float GetCollisonItemScale()const {
		return item_scale;
	}

	//�A�C�e���̃n���h�����擾����
	int GetCollisonItemHdl()const {
		return item_hdl;
	}

	//����\�̃A�C�e�����̃t���O���擾����
	bool GetCollisonItemisObtainable()const {
		return isObtainable;
	}

	//�A�C�e���̍��W���Z�b�g����
	void SetCollisionItemPos(const tnl::Vector3& new_pos) {
		item_pos = new_pos;
	}

private:


	//�A�C�e����ID
	int item_id = 0;

	//�A�C�e���̍��W
	tnl::Vector3 item_pos = {};

	//�A�C�e���̃`�b�v�T�C�Y
	int chip_size = 0;

	//�A�C�e���̃X�P�[��
	float item_scale = 0;

	//�A�C�e���̉摜�n���h��
	int item_hdl = 0;

	//����\���ǂ����̃t���O
	bool isObtainable = false;

};