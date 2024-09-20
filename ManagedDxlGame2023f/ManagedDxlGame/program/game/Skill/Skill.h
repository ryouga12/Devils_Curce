//------------------------------------------------------------------------------------------------------------
//
//�X�L���N���X
//
//-------------------------------------------------------------------------------------------------------------


#pragma once
#include"BaseSkill.h"
#include "../../dxlib_ext/dxlib_ext.h"
#include<unordered_map>
#include"../../game/Object/Player.h"
#include"../../game/Object/Enemy.h"
#include"../../game/Effect/animation.h"
#include"../Manager/SoundManager.h"
#include"../../koni_name_space/common/common_value.h"

class BattleLog;

//�U���n
static const int AttackType = 0;
//�o�t�n
static const int BuffType = 1;

class Skill : public BaseSkill {
public:

	Skill() = default;

	//�X�L���N���X�̃R���X�g���N�^
	//arg_1 : ID 
	//arg_2 : ���O 
	//arg_3 : �U���� 
	//arg_4 : ���Z�̐���
	//arg_5 : ���Z�̏���Mp
	//arg_6 : �X�L���̃^�C�v
	Skill(const int _id, const std::string& _name, const float _power, const std::string& _description, const int consumeMp, const int skilltype); 
	virtual~Skill();

	//�U���X�L�����g�������̃_���[�W�v�Z
	//arg_1 : �v���C���[�̃X�e�[�^�X
	//arg_2 : �G�l�~�[�̃X�e�[�^�X
	//arg_3  : �o�g�����O�̃X�}�[�g�|�C���^
	virtual void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log) {}

	//�o�t�X�L�����g�������̏���(�v���C���[)
	//arg_1  : �v���C���[�̃X�e�[�^�X
	//arg_2  : �o�g�����O�̃X�}�[�g�|�C���^
	virtual void SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log) {};

	//�G�̃o�t�n�̃X�L��
	//arg_1  : �G�l�~�[�̃X�e�[�^�X
	//arg_2  : �o�g�����O�̃X�}�[�g�|�C���^
	virtual void SkillUse(Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log) {};


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
	int getId() const { return skill_id; }
	//�X�L���̖��O���擾����
	std::string getName() const { return skill_name; }
	//�X�L���̏ڍׂ��擾����
	std::string getDescription() const { return description; }
	//�X�L���̃_���[�W���擾����
	float getSkillPower() { return power; }
	//�X�L���̕K�vMp���擾����
	int getSkillConsumeMp()const { return consume_mp; }
	//�X�L���̃^�C�v���擾����
	int getSkillType()const{return skill_type;}

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

	// �X�P�[�����O�t�@�N�^�[���`
	float scaling_factor = 0;

	// ��{�_���[�W�̔{��
	const float BASE_MULTIPLIER = 0.8f;

	// �����_���t�@�N�^�[
	float random_factor = 0;

	//�����_���t�@�N�^�[�̒l�����߂�
	void initializeRandomFactor();

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	void PlayerReductionRate(float& damage);

	//���킪���ꕐ�킩���m�F����
	void CheckEpicWeapon(float& damage , Enemy::EnemyConnection& enemyStatus_);

	//�}�b�N�X���x��
	const int MAX_LEVEL = 20;

	//�x�[�X�̌y����
	const float BASE_REDUCTION_RATE = 0.035f;

	//---�A�j���[�V����������������ۂ̕ϐ�---//
	//����͂킩��₷���悤�ɃR���X�g���N�^���ŏ���������
	
	//�G�t�F�N�g�̍��W
	//�f�t�H���g�� x_600 , y_350
	//�ς������ꍇ�R���X�g���N�^�Ő؂�ւ���
	tnl::Vector2i effect_pos = { 600 , 350 };
	
	//�G�t�F�N�g�̕��̐�
	int effect_width_num = 0;
	//�G�t�F�N�g�̍����̐� 
	int effect_height_num = 0;
	//�G�t�F�N�g�̕��̃T�C�Y
	int effect_width_size = 0;
	//�G�t�F�N�g�̏c�̃T�C�Y
	int effect_height_size = 0;
	//�G�t�F�N�g�̃t���[���̑���
	int effect_frame_all_num = 0;
	//�G�t�F�N�g�̒x���b��
	//�ʏ��8�b�ɐݒ�(�Z�ɂ���Đ؂�ւ���)
	int effect_delay = 8;

};

//�ʏ�U��
class Nomal_Attack final  : public Skill {
public:

	//�ʏ�U��
	Nomal_Attack(int weapon_type );
	~Nomal_Attack() override;

	//�ʏ�U�����s�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

private:

	//����ɂ���Ēʏ�U���̃G�t�F�N�g��ς���
	enum {
		EMPTY,
		SWOED,
		HAMMER,
		SPEAR,
		LONGSWORD
	};

	//SE
	std::string sound_text;

};

//�Ή��a��
class FlameSlash final  : public Skill {
public:

	FlameSlash();
	~FlameSlash()override{}

	//�Ή��a����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

	
};

//�t�@�C�A�X�g�[��
class FireStorm : public Skill {
public:

	FireStorm();
	~FireStorm()override {};

	//�t�@�C�A�X�g�[�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�C���t�F���m
class Inferno : public Skill {
public:

	Inferno();
	~Inferno()override {};

	//�C���t�F���m���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�A�C�X�u���X�g
class IceBlast final : public Skill {
public:

	IceBlast();
	~IceBlast()override {}

	//�A�C�X�u���X�g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�E�H�[�^�[�u���[�h
class WaterBlade : public Skill {
public:

	WaterBlade();
	~WaterBlade()override {};

	//�E�H�[�^�[�u���[�h���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;
};

//�v���U�[�h
class Blizzard : public Skill {
public:

	Blizzard();
	~Blizzard()override {};

	//�v���U�[�h���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};


//�T���_�[�{���g
class ThunderBolt final : public Skill {
public:

	ThunderBolt();
	~ThunderBolt()override {}

	//�T���_�[�{���g�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)override;

};

//���C�g�j���O�X���b�V��
class LightningSlash : public Skill {
public:

	LightningSlash();
	~LightningSlash()override {};

	//���C�g�j���O�X���b�V�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�񕜎���

//�q�[��
class Heal final : public Skill {

public:

	Heal();
	~Heal()override {}

	//�q�[�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus , const Shared<BattleLog>& battle_log)override;
};

//�n�C�q�[��
class HighHeels : public Skill {
public:

	HighHeels();
	~HighHeels()override {};

	//�n�C�q�[���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)override;

};


//---------------------------------------------------------------------------------------------------------
//�A�C�e���n�̃X�L��

//�X���C���̗�
class SlimBell final : public Skill {
	
public:

	SlimBell();
	~SlimBell()override {};

	//�X���C���̗���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�X�l�[�N�̗�[
class SnakeBell final : public Skill {
public:

	SnakeBell();
	~SnakeBell()override {}

	//�X�l�[�N�̗���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//---------------------------------------------------------------------------------------------------------
//�G�̃X�L��

//�J�I�X�t���A
//���͂ȉΑ������@
class ChaosFlare final : public Skill {
public:

	ChaosFlare();
	~ChaosFlare()override {};

	//�J�I�X�t���A���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�f�X�T�C�Y�E�B���h
//���͂ȕ�
class DeathScytheWind final : public Skill {
public:

	DeathScytheWind();
	~DeathScytheWind()override {};

	//�f�X�T�C�Y�E�B���h���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�_�[�N�N���E
class DrakClaw final : public Skill {
public:

	DrakClaw();
	~DrakClaw()override {};

	//�_�[�N�N���E�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�ł̔g��
class WaveDarkness : public Skill {
public:

	WaveDarkness();
	~WaveDarkness()override {};
		
	//�ł̔g�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�n���̉��n
class HellFlameBlade : public Skill {
public:
	
	HellFlameBlade();
	~HellFlameBlade() override {};

	//�n���̉��n���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};


//�����̕�
class LifeWind final : public Skill {
public:

	LifeWind();
	~LifeWind()override {};

	//�����̕����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�G�i�W�[�u���X�g
class EnergyBlast final : public Skill {
public:

	EnergyBlast();
	~EnergyBlast()override {};

	//�G�i�W�[�u���X�g���g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�_�[�N�`���[�W
class DarkCharge final : public Skill {
public:

	DarkCharge();
	~DarkCharge()override {};

	//�_�[�N�`���[�W�g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//���̎a��
class SlashLight : public Skill {
public:

	SlashLight();
	~SlashLight()override {};

	//���̎a�����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//�p�Y�̗�
class PowerHeroes : public Skill {
public:

	PowerHeroes();
	~PowerHeroes()override {};

	//�p�Y�̗͂��g�������̏���
	void SkillUse(Enemy::EnemyConnection& enemyStatus, const Shared<BattleLog>& battle_log)override;

};

//���̑�
class LightSpear : public Skill {
public:

	LightSpear();
	~LightSpear()override {};

	//���̑����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//���_�̊�
class GrimReaperSickle : public Skill {
public:

	GrimReaperSickle();
	~GrimReaperSickle()override {};

	//���_�̊����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//���̊�����
class SoulReper : public Skill {
public:

	SoulReper();
	~SoulReper()override {};

	//���̊�������g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};

//���{�̌�
class LightUnderworld : public Skill {
public:

	LightUnderworld();
	~LightUnderworld()override {};


	//���{�̌����g�������̏���
	void SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)override;

};