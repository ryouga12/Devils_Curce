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
	struct EnemyConnection
	{
	public:

		//name���擾����
		std::string GetEnemyString() const {
			return name;
		}
		//ID���擾����
		int GetEnemyId() const {
			return id;
		}
		//Hp���擾����
		int GetEnemyHp() const{
			return hp;
		}
		//Attack���擾����
		int GetEnemyAttack() const {
			return Attack;
		}
		//Defance���擾����
		int GetEnemyDefance() const {
			return Defance;
		}
		//Speed���擾����
		int GetEnemySpeed() const {
			return Speed;
		}
		//�o���l���擾����
		int GetEnemyExpoint() const {
			return ex_point;
		}
		//money���擾����
		int GetEnemyMoney() const {
			return money;
		}
		//ghdl���擾����
		const std::string& GetEnemyGhdl()const {
			return gh;
		}
		//���ꂼ��̑ϐ����擾����
		float GetFireResist() const {
			return FireResist;
		}
		float GetIceResist() const {
			return IceResist;
		}
		float GetThunderResist() const {
			return ThunderResist;
		}
		//�ʏ�h���b�v�̃A�C�e����ID���擾����
		int GetNomalDrop()const {
			return Nomal_Drop;
		}
		//���A�h���b�v�̃A�C�e��ID���擾����
		int GetRareDrop()const {
			return RareDrop;
		}
		//se�̃T�E���h���擾����
		std::string GetSeAttack()const {
			return attack_se;
		}
		//���@�͂��擾����
		int GetMagicPower()const {
			return magic_power;
		}
		//�m�[�}���h���b�v�̊m�����擾����
		int GetNomalProbability()const {
			return nomal_drop_probability;
		}
		//���A�h���b�v�̊m�����擾����
		int GetRareProbability()const {
			return rare_drop_probability;
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
		//�m�[�}���h���b�v�A�C�e���̊m�����Z�b�g����
		void SetNomalDropProbability(int new_nomal_probability) {
			nomal_drop_probability = new_nomal_probability;
		}
		//���A�h���b�v�A�C�e���̊m�����Z�b�g����
		void SetRareDropProbability(int new_rare_probability) {
			rare_drop_probability = new_rare_probability;
		}

	private:

		//���O
		std::string name;
		//ID
		int id = 0;
		//�̗�
		int hp = 0;
		//�U����
		int Attack = 0;
		//�h���
		int Defance = 0;
		//�f����
		int Speed = 0;
		//������o���l
		int ex_point = 0;
		//�����邨��
		int money = 0;
		//�摜
		std::string gh;
		//���ϐ�
		float FireResist = 0;
		//�X�ϐ�
		float IceResist = 0;
		//���ϐ�
		float ThunderResist = 0;
		//�m�[�}���h���b�v�A�C�e��
		int Nomal_Drop = 0;
		//���A�h���b�v�A�C�e��
		int RareDrop = 0;
		//�U������SE
		std::string attack_se;
		//���@��
		int magic_power = 0;
		//�m�[�}���h���b�v�A�C�e���̃h���b�v��
		int nomal_drop_probability = 0;
		//���A�h���b�v�A�C�e���̃h���b�v��
		int rare_drop_probability = 0;
	};

	//Enemy�̃X�e�[�^�X��ǂݍ��ވׂ̊֐�
	void EnemyLoadTypeInfo();

	//�G�̏�����
	void EnemyInit();

	//�G�̂̃X�e�[�^�X��id���擾����
	EnemyConnection GetEnemyStatus(int id) const;
	
	//�G�̔z����󂯎��ׂ̃Q�b�^�[
	std::vector<EnemyConnection>& GetEnemyArray(){
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
	EnemyConnection Enemy_Status_Type;

	//�\���̂̑���
	int EnemyStatus_Total_Num;

	//�\���̂��i�[����z��
	std::vector<EnemyConnection>Enemy_Type_Array;

	//csv�����Ă����z��
	std::vector<std::vector<std::string>>enemy_csv_array;

protected:

	//�G���[���ɕԂ��p�̔z��
	std::vector<EnemyConnection>null_array;
	//�G�����Ă����z��
	std::vector<EnemyConnection>enemy_array;

private:

	//�G�̃T�C�Y
	const float ENEMY_SIZE = 0.8f;

	//---���W�n---//

	//�G�̕\�����W
	const tnl::Vector3 ENEMY_POS = { 600 , 320 , 0 };



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
	MobMonster(const int enemy_id);
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

	BossMonster(int enemy_id);
	~BossMonster()override {};

	//�G�̍U������
	void EnemyAction(Shared<BattleLog>battle_log)override;

	//�G�̃X�L���z��̏�����
	void InitEnemySkill(int enemy_id);

	//�G�̃X�L���z����擾����
	std::vector <Shared<Skill>>& GetEnemySkillList() {
		return enemy_skill_;
	}

	//�G�̃X�L���p�̃C���f�b�N�X���擾����
	int GetEnemySkillIndex()const {
		return enmey_skill_index;
	}

private:

	//�G�̃{�X�̎��
	//�ǂ̃{�X���w�肵�Ă邩
	//�萔���͂��ꂼ�ꃂ���X�^�[�̖��O
	enum class BossType {
		NONE,
		//�Ō�̃{�X
		ZERAHKIEL = 20,
		//���{�X1�̖�
		GROVEGUARDIAN = 21
	};

	//�G�̃X�L��
	std::vector<Shared<Skill>>enemy_skill_;

	//�G�̃X�L���̐�
	const int ENEMY_SKILL_MAX_NUM = 3;

	//�G�̃X�L���p�̃C���f�b�N�X
	int enmey_skill_index = 0;

};