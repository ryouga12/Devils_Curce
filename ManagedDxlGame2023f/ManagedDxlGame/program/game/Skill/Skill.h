#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Manager/animation.h"
#include"../Manager/SoundManager.h"

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
	virtual int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_) { return 0; }

	//�o�t�X�L�����g�������̏���
	virtual int SkillUse(Player::PlayerStatus& playerStatus) { return 0; };

	//�A�C�e���n���g�������̏���
	virtual int SkillUse(Enemy::EnemyStatus& enemyStatus_) { return 0; };


	//�X�L�����g�����ۂ̃A�j���[�V�����̍Đ�
	virtual void SkillUseAnimation() {}

	//�X�L�����g�����ۂ̃A�j���[�V�������~�߂�
	virtual void SkillAnimationStop() {};

	//�X�L�����g�����ۂ̃A�j���[�V�����̕`��
	virtual void SkillAnimationDraw() {};

	//�X�L�����g�����ۂ̃A�j���[�V�����̍X�V����
	virtual void SkillAnimationUpdate(float delta_time) {}

	//�X�L�����g��������Mp����
	virtual void SkillMpConsume(Player::PlayerStatus& playerStatus) {}
	
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
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//�ʏ�U�����s�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�X�L�����g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�X�L�����g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�X�L�����g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;

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
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//�Ή��a����s�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�Ή��a����g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�Ή��a����g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�Ή��a����g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;

	//�Ή��a����g��������Mp����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//�A�C�X�u���X�g
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//�A�C�X�u���X�g�g�������̏���
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//�A�C�X�u���X�g�s�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�A�C�X�u���X�g�g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�A�C�X�u���X�g�g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�A�C�X�u���X�g�g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;

	//�A�C�X�u���X�g�g��������Mp����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//�T���_�[�{���g
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//�T���_�[�{���g�g�������̏���
	int SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)override;

	//�T���_�[�{���g�s�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�T���_�[�{���g�g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�T���_�[�{���g�g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�T���_�[�{���g�g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;

	//�T���_�[�{���g�g��������Mp����
	void SkillMpConsume(Player::PlayerStatus& playerStatus)override;

};

//�񕜎���
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//�q�[�����g�������̏���
	int SkillUse(Player::PlayerStatus& playerStatus)override;

	//�q�[���g��������Mp����
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
	int SkillUse(Enemy::EnemyStatus& enemyStatus_)override;
	
	//�X���C���̗���g�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�X���C���̎g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�X���C���̎g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�X���C���̎g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;
};

//�X�l�[�N�̗�[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//�X�l�[�N�̗���g�������̏���
	int SkillUse(Enemy::EnemyStatus& enemyStatus_)override;

	//�X�l�[�N�̗���g�������̃A�j���[�V����
	void SkillUseAnimation()override;

	//�X�l�[�N�̎g�����ۂ̃A�j���[�V�������~�߂�
	void SkillAnimationStop()override;

	//�X�l�[�N�̎g�����ۂ̃A�j���[�V�����̕`��
	void SkillAnimationDraw()override;

	//�X�l�[�N�̎g�����ۂ̃A�j���[�V�����̍X�V����
	void SkillAnimationUpdate(float delta_time)override;

};
