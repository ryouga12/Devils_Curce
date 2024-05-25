#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Manager/animation.h"
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
	Skill(int _id , const std::string _name ,float _power , const std::string& _description, int consumeMp , int skilltype) :BaseSkill(_id ,_name, _power,_description ,consumeMp, skilltype) {}
	virtual~Skill(){}

	//�U���X�L�����g�������̃_���[�W�v�Z
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log) {}

	//�o�t�X�L�����g�������̏���
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log) {};

	//�A�C�e���n���g�������̏���
	virtual void SkillUse(Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log) {};


	//�X�L�����g�����ۂ̃A�j���[�V�����̍Đ�
	virtual void SkillUseAnimation();

	//�X�L�����g�����ۂ̃A�j���[�V�������~�߂�
	virtual void SkillAnimationStop();

	//�X�L�����g�����ۂ̃A�j���[�V�����̕`��
	virtual void SkillAnimationDraw();

	//�X�L�����g�����ۂ̃A�j���[�V�����̍X�V����
	virtual void SkillAnimationUpdate(float delta_time);

	//�X�L�����g��������Mp����
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus);
	
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
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)override;

private:

	//����ɂ���Ēʏ�U���̃G�t�F�N�g��ς���
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR
	};

	int Sound = 0;

};

//�Ή��a��
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//�Ή��a����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)override;

};

//�A�C�X�u���X�g
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//�A�C�X�u���X�g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)override;

};

//�T���_�[�{���g
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//�T���_�[�{���g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)override;

};

//�񕜎���
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//�q�[�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus , Shared<BattleLog>& battle_log)override;
};


//---------------------------------------------------------------------------------------------------------
//�A�C�e���n�̃X�L��

//�X���C���̗�
class SlimBell final : public Skill {
	
public:

	SlimBell();
	~SlimBell()override {};

	//�X���C���̗���g�������̏���
	void SkillUse(Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)override;

};

//�X�l�[�N�̗�[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//�X�l�[�N�̗���g�������̏���
	void SkillUse(Enemy::EnemyStatus& enemyStatus_ ,Shared<BattleLog>& battle_log)override;

};
