#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Effect/animation.h"
#include"../Manager/SoundManager.h"

class BattleLog;

//�U���n
static const int AttackType = 0;
//�o�t�n
static const int BuffType = 1;
//�A�C�e���n
static const int ItemType = 2;


class Skill : public BaseSkill {
public:

	Skill() {}
	//(ID , ���O , �U���� , ���Z�̐����A���Z�̏���Mp�A�X�L���̃^�C�v) ������������
	Skill(int _id, const std::string _name, float _power, const std::string& _description, int consumeMp, int skilltype); 
	virtual~Skill(){}

	//�U���X�L�����g�������̃_���[�W�v�Z
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log) {}

	//�o�t�X�L�����g�������̏���
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log) {};

	//�A�C�e���n���g�������̏���
	virtual void SkillUse(Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log) {};


	//�X�L�����g�����ۂ̃A�j���[�V�����̍Đ�
	virtual void SkillUseAnimation();

	//�X�L�����g�����ۂ̃A�j���[�V�������~�߂�
	virtual void SkillAnimationStop();

	//�X�L�����g�����ۂ̃A�j���[�V�����̕`��
	virtual void SkillAnimationDraw();

	//�X�L�����g�����ۂ̃A�j���[�V�����̍X�V����
	virtual void SkillAnimationUpdate(float delta_time);

	//�X�L�����g��������Mp����
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus) {};
	
	//Id���擾����
	int getId() const { return Id; }
	//�X�L���̖��O���擾����
	std::string getName() const { return SkillName; }
	//�X�L���̏ڍׂ��擾����
	std::string getDescription() const { return description; }
	//�X�L���̃_���[�W���擾����
	float getSkillPower() { return Power; }
	//�X�L���̕K�vMp���擾����
	int getSkillConsumeMp()const { return ConsumeMp; }
	//�X�L���̃^�C�v���擾����
	int getSkillType()const{return SkillType;}

	//�S�̂̃X�L�����i�[����
	void AddSkillList();

	//�S�̂̃X�L�����X�g���擾����
	std::vector<Shared<Skill>>& GetOverallSkills() {
		return skill_list;
	}

private:

	//�S�̂̃X�L�����X�g
	//��ɃX�L���̃|�C���^�̃Z�[�u���[�h�Ɏg�p
	std::vector<Shared<Skill>>skill_list;

protected:

	//�G�t�F�N�g�p�̃A�j���[�V����
	Shared<Animation>Effect_Animation = nullptr;
	
};

//�ʏ�U��
class Nomal_Attack final  : public Skill {
public:

	//�ʏ�U��
	Nomal_Attack(int weapon_type );
	~Nomal_Attack() override;

	//�ʏ�U�����s�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

private:

	//����ɂ���Ēʏ�U���̃G�t�F�N�g��ς���
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR
	};

	std::string SoundText;

};

//�Ή��a��
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//�Ή��a����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

	//�Ή��a����g��������MP����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//�A�C�X�u���X�g
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//�A�C�X�u���X�g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

	//�A�C�X�u���X�g�g��������MP����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};

//�T���_�[�{���g
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//�T���_�[�{���g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

	//�T���_�[�{���g�g��������MP����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};

//�񕜎���
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//�q�[�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus , Shared<BattleLog>& battle_log)override;

	//�q�[�����g��������MP����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;
};


//---------------------------------------------------------------------------------------------------------
//�A�C�e���n�̃X�L��

//�X���C���̗�
class SlimBell final : public Skill {
	
public:

	SlimBell();
	~SlimBell()override {};

	//�X���C���̗���g�������̏���
	void SkillUse(Enemy::EnemyConnection& enemyStatus_ , Shared<BattleLog>& battle_log)override;

};

//�X�l�[�N�̗�[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//�X�l�[�N�̗���g�������̏���
	void SkillUse(Enemy::EnemyConnection& enemyStatus_ ,Shared<BattleLog>& battle_log)override;

};

//---------------------------------------------------------------------------------------------------------
//�G�̃X�L��

//�J�I�X�t���A
//���͂ȉΑ������@
class ChaosFlare : public Skill {
public:

	ChaosFlare();
	~ChaosFlare()override {};

	//�J�I�X�t���A���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//�f�X�T�C�Y�E�B���h
//���͂ȕ�
class DeathScytheWind : public Skill {
public:

	DeathScytheWind();
	~DeathScytheWind()override {};

	//�f�X�T�C�Y�E�B���h���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//�_�[�N�N���E
class DrakClaw : public Skill {
public:

	DrakClaw();
	~DrakClaw()override {};

	//�_�[�N�N���E�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//