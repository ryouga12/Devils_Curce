#pragma once
#include "../../dxlib_ext/dxlib_ext.h"
#include"../Item/Item.h"
#include"Actor.h"

class ItemBase;
class BattleLog;
class GameManager;
class SoundManager;
class SceneManager;
class CsvManager;
class Player;

class Enemy : public Actor{
public:

	Enemy();

	//�`�揈��
	void Draw();

	//Enemy�̃X�e�[�^�X
	struct EnemyStatus
	{
	public:

		//name��Ԃ��Q�b�^�[
		std::string GetEnemyString() const {
			return name;
		}
		//ID��Ԃ��Q�b�^�[
		int GetEnemyId() const {
			return id;
		}
		//Hp��Ԃ��Q�b�^�[
		int GetEnemyHp() const{
			return hp;
		}
		//Attack��Ԃ��Q�b�^�[
		int GetEnemyAttack() const {
			return Attack;
		}
		//Defance��Ԃ��Q�b�^�[
		int GetEnemyDefance() const {
			return Defance;
		}
		//Speed��Ԃ��Q�b�^�[
		int GetEnemySpeed() const {
			return Speed;
		}
		//�o���l��Ԃ��Q�b�^�[
		int GetEnemyExpoint() const {
			return ex_point;
		}
		//money��Ԃ��Q�b�^�[
		int GetEnemyMoney() const {
			return money;
		}
		//ghdl��Ԃ��Q�b�^�[
		const std::string& GetEnemyGhdl()const {
			return gh;
		}
		//���ꂼ��̑ϐ���Ԃ��Q�b�^�[
		float GetFireResist() const {
			return FireResist;
		}
		float GetIceResist() const {
			return IceResist;
		}
		float GetThunderResist() const {
			return ThunderResist;
		}
		//�ʏ�h���b�v�̃A�C�e����ID��Ԃ�
		int GetNomalDrop()const {
			return Nomal_Drop;
		}
		//���A�h���b�v�̃A�C�e��ID��Ԃ�
		int GetRareDrop()const {
			return RareDrop;
		}
		//se�̃T�E���h��Ԃ�
		std::string GetSeAttack()const {
			return attack_se;
		}
		//���@�͂�Ԃ�
		int GetMagicPower()const {
			return magic_power;
		}

		//---�Z�b�^�[---//

		//�G�̖��O���Z�b�g����
		void SetEnemyName(std::string newname) {
			name = newname;
		}
		//�G��ID���Z�b�g����
		void SetEnemyId(int newid){
			id = newid;
		}
		//�G��Hp���Z�b�g����
		void SetEnemyHp(int newhp) {
			hp = newhp;
		}
		//�G��Attack���Z�b�g����
		void SetEnemyAttack(int newattack) {
			Attack = newattack;
		}
		//�G��Defance���Z�b�g����
		void SetEnemyDefance(int newdefance) {
			Defance = newdefance;
		}
		//�G��Speed���Z�b�g����
		void SetEnemySpeed(int newspeed) {
			Speed = newspeed;
		}
		//�G�̌o���l���Z�b�g����
		void SetEnemyExpoint(int newexpoint) {
			ex_point = newexpoint;
		}
		//�G��money���Z�b�g����
		void SetEnemyMoney(int newmoney) {
			money = newmoney;
		}
		//�G�̃n���h�����Z�b�g����
		void SetGhdl(std::string newgh) {
			gh = newgh;
		}
		//�G�̑ϐ����Z�b�g����
		void SetFireResist(float newresist) {
			FireResist = newresist;
		}
		void SetIceResist(float newresist) {
			IceResist = newresist;
		}
		void SetThunderResist(float newresist) {
			ThunderResist = newresist;
		}
		//�G�̒ʏ�h���b�v��ID���Z�b�g����
		void SetNomalDropID(int newID) {
			Nomal_Drop = newID;
		}
		//�G�̃��A�h���b�v��ID���Z�b�g����
		void SetRareDoropID(int newID) {
			RareDrop = newID;
		}
		//�U������SE���Z�b�g����
		void SetAttackSe(const std::string& new_se) {
			attack_se = new_se;
		}
		//���@�͂��Z�b�g����
		void SetMagicPower(int new_magic_power) {
			magic_power = new_magic_power;
		}

	private:

		std::string name;
		int id = 0;
		int hp = 0;
		int Attack = 0;
		int Defance = 0;
		int Speed = 0;
		int ex_point = 0;
		int money = 0;
		std::string gh;
		float FireResist = 0;
		float IceResist = 0;
		float ThunderResist = 0;
		int Nomal_Drop = 0;
		int RareDrop = 0;
		std::string attack_se;
		int magic_power = 0;
	};

	//Enemy�̃X�e�[�^�X��ǂݍ��ވׂ̊֐�
	void EnemyLoadTypeInfo();

	//�G�̏�����
	void EnemyInit();

	//�G�̂̃X�e�[�^�X��id���擾����
	EnemyStatus GetEnemyStatus(int id) const;
	
	//�G�̔z����󂯎��ׂ̃Q�b�^�[
	std::vector<EnemyStatus>& GetEnemyArray(){
		return enemy_array;

		//�G�̔z����擾�ł��Ȃ�������G���[�p�̔z���Ԃ�
		if (enemy_array.size() == 0) {
			return null_array;
		}
	}

	//�G��ID�ɂ���ăh���b�v�̃A�C�e����؂�ւ���
	void InitEnemyItemDrop(int EnemyID);

	//�G���������Ă���A�C�e���̔z����擾����
	std::vector<ItemBase>& getItemDropArray() {
		return enemy_drop_item;
	}

	//�G�̏���z��Ɋi�[����
	void InitEnemyArray(int id);

	//�G�̍U������(��Ɏq�N���X�Œ�`����)
	virtual void EnemyAction(Shared<BattleLog>battle_log) {};

	//�G�̎��S����
	virtual bool ChackDeadEnemy();

	//�G�̃C���f�b�N�X���擾����
	int GetEnemy_Index()const {
		return enemy_index;
	}

	//�t���O���Ă�(�摜������)
	void DeadEnemyFlag() {
		dead_enemy_flag = true;
	}

	//�G�̎��
	enum class Enemytype {
		NONE,
		//�G���G
		MOB,
		//�{�X�G
		BOSS
	};

	//�G�̃^�C�v���Z�b�g����
	void SetEnemyType(Enemytype enemyType) {
		enemy_type = enemyType;
	}

	//�G�̃^�C�v���擾����
	Enemytype GetEnemyType()const {
		return enemy_type;
	}

private:

	//Enemy�̃X�e�[�^�X�����Ă����ϐ�
	EnemyStatus Enemy_Status_Type;

	//�\���̂̑���
	int EnemyStatus_Total_Num;

	//�\���̂��i�[����z��
	std::vector<EnemyStatus>Enemy_Type_Array;

	//csv�����Ă����z��
	std::vector<std::vector<std::string>>enemy_csv_array;

	//---�G�����Ă����z��---//
protected:

	//�G���[���ɕԂ��p�̔z��
	std::vector<EnemyStatus>null_array;
	//�G�����Ă����z��
	std::vector<EnemyStatus>enemy_array;

private:

	//�G�̃T�C�Y
	const float ENEMY_SIZE = 0.8f;

	//---���W�n---//
	const tnl::Vector2i ENEMY_POS = { 600 , 320 };



	//---�G�̃h���b�v�A�C�e�����i�[�������z��---//

	std::vector<ItemBase>enemy_drop_item;

	//�A�C�e���̃|�C���^
	Shared<Item>item = nullptr;

	//�G�̐�
	const int ENEMY_NUM = 5;

	//���񂾔���̃t���O(���false�̏ꍇ�G�̉摜��\������)
	bool dead_enemy_flag = false;

protected:
	
	//�G�̉摜�n���h��
	int enemy_hdl = 0;

	//�G�̃C���f�b�N�X
	int enemy_index = 0;

	Enemytype enemy_type = Enemytype::NONE;

};

//------------------------------------------------------------------------------------------------------------
//
//�G�������X�^�[
//
//-------------------------------------------------------------------------------------------------------------

class MobMonster : public Enemy {
public:

	MobMonster() {};
	MobMonster(int enemy_id);
	~MobMonster()override {};

	//�G�̍U������
	void EnemyAction(Shared<BattleLog>battle_log)override;

private:

};

//------------------------------------------------------------------------------------------------------------
//
//�{�X�����X�^�[
//
//-------------------------------------------------------------------------------------------------------------

class BossMonster : public Enemy {

public:

	BossMonster();
	~BossMonster()override {};

	//�G�̍U������
	void EnemyAction(Shared<BattleLog>battle_log)override;

	//�G�̃X�L���z����擾����
	std::vector <Shared<Skill>>& GetEnemySkillList() {
		return enemy_skill_;
	}

	//�G�̃X�L���p�̃C���f�b�N�X���擾����
	int GetEnemySkillIndex()const {
		return enmey_skill_index;
	}

private:

	//�G�̃X�L��
	std::vector<Shared<Skill>>enemy_skill_;

	//�G�̃X�L���̐�
	const int ENEMY_SKILL_MAX_NUM = 3;

	//�G�̃X�L���p�̃C���f�b�N�X
	int enmey_skill_index = 0;

};