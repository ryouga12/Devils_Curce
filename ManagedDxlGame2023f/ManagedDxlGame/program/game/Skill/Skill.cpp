#include "Skill.h"
#include"../Menu/BattleLog.h"

void Skill::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

void Skill::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

void Skill::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

void Skill::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//---------------------------------------------------------------------------------------------------------

//�ʏ�U��
Nomal_Attack::Nomal_Attack(int weapon_type): Skill(0, "�U��", 1, "�ʏ�U��",0 , AttackType)
{

	//����̎�ނɂ���ăA�j���[�V������ς���
	switch (weapon_type)
	{
	case EMPTY:

		//�����������Ă��Ȃ����̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack�QEmpty.png", 600, 350, 5, 2, 120, 120, 6, 8);

		//Se�T�E���h
		SoundText = "sound/SoundEffect/dageki_3.mp3";

		break;

	case SWOED:

		//���𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", 630, 350, 5, 1, 120, 120, 5, 8);

		//Se�T�E���h
		SoundText = "sound/SoundEffect/zanngeki_2.mp3";

		break;

	case HAMMER:

		//�n���}�[�𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", 600, 350, 7, 1, 120, 120, 7, 8);

		//Se�T�E���h
		SoundText = "sound/SoundEffect/dageki.mp3";

		break;

	case SPEAR:

		//���𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Spear.png", 600, 350, 7, 1, 120, 120, 7, 8);

		break;

	default:

		tnl::DebugTrace("---�A�j���[�V�����𐶐��ł��܂���ł����B---");
		tnl::DebugTrace("---Se�𐶐��ł��܂���ł���---");

		break;
	}

}

Nomal_Attack::~Nomal_Attack()
{
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki_3.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/zanngeki_2.mp3");
	SoundManager::getSoundManager()->daleteSound("sound/SoundEffect/dageki.mp3");
}

//�ʏ�U�����s�������̏���(�h���N�G���v�Z��(�C���t���ɂ��Ή��ł���))
void Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ ,Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();

	//�_���[�W���v�Z����
	int damage = static_cast<int>((PlayerAttack / 2 - EnemyDefance / 4) * Power);

	//�_���[�W��0�����������1�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	// �G��HP�����炷
	enemyStatus_.SetEnemyHp(enemyStatus_.GetEnemyHp() - damage);

	// �퓬���O�Ƀ_���[�W���ʂ��o��
	battle_log->addDamageLog("Player", "Enemy", damage);

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play(SoundText.c_str(), DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, SoundText.c_str());

}

//------------------------------------------------------------------------------------------------------------------------------

//�Ή��a��
FlameSlash::FlameSlash() : Skill(1, "�Ή��a��", 1.5f, "���̗͂��܂Ƃ����U��",2 , AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//�Ή��a����g�������̏���
void FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_ , Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyFireResist = enemyStatus_.GetFireResist();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = (PlayerAttack * Power) - EnemyDefance;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// �����U���͂ɂ��␳��������
	float damage = baseDamage * EnemyFireResist;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/FlameSlash.mp3");

}

//�Ή��a����g��������MP����
void FlameSlash::SkillMpConsume(Player::PlayerStatus& playerStatus) {

	//Mp���擾����
	auto CurentMp = playerStatus.GetCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - ConsumeMp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//------------------------------------------------------------------------------------------------------------------------------

//�A�C�X�u���X�g
IceBlast::IceBlast():Skill(2, "�A�C�X�u���X�g", 2.0f, "�X�����̔������@", 3 , AttackType)
{
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/IceBlast.png", 600, 320, 5, 2, 192, 192, 10, 8);
}

//�A�C�X�u���X�g���g�������̌v�Z����
void IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyIceResist = enemyStatus_.GetIceResist();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = (PlayerMagicPower * Power) - EnemyDefance;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// �����U���͂ɂ��␳��������
	float damage = baseDamage * EnemyIceResist;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ice_blast.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ice_blast.mp3");
	
}

//�A�C�X�u���X�g�g��������MP����
void IceBlast::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.GetCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - ConsumeMp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//------------------------------------------------------------------------------------------------------------------------------

//�T���_�[�{���g
ThunderBolt::ThunderBolt():Skill(3, "�T���_�[�{���g", 2.5f, "�������̔������@", 5 , AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/ThunderBolt.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//�T���_�[�{���g�g�������̏���
void ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = (PlayerMagicPower * Power) - EnemyDefance;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (baseDamage <= 0) {
		baseDamage = 1;
	}

	// �����U���͂ɂ��␳��������
	float damage = baseDamage * EnemyThunderResist;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/ThunderBolt.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ThunderBolt.mp3");

}

//�T���_�[�{���g�g��������MP����
void ThunderBolt::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.GetCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - ConsumeMp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//�񕜎���
Heal::Heal(): Skill(4, "�q�[��", 0, "Hp���񕜂���", 2, BuffType)
{
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Heal.png", 160, 160, 5, 2, 192, 192, 10, 8);
}

//�q�[�����g�����ۂ̏���
void Heal::SkillUse(Player::PlayerStatus& playerStatus, Shared<BattleLog>& battle_log)
{
	auto max_hp =  playerStatus.GetMaxHp();

	//Hp��maxhp����������Ă����ꍇ�g�p�ł���
	if (playerStatus.GetcurentHp()< max_hp) {
		// HP���ő�Hp��30���񕜂�����
		float percentage = 0.3;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hp���񕜂���
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//�񕜉���炷
		SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/ThunderBolt.mp3");


		//�o�g�����O�𗬂�
		battle_log->addRecoveryLog("Player", SkillName, healAmount);
	}

}

//�q�[�����g��������MP����
void Heal::SkillMpConsume(Player::PlayerStatus& playerStatus) {

	//Mp���擾����
	auto CurentMp = playerStatus.GetCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - ConsumeMp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//---------------------------------------------------------------------------------------------------------
//�A�C�e���n�̃X�L��

//�X���C���̍U��
SlimBell::SlimBell() : Skill(20 , "�X���C�����Ă�",5, "�X���C���̗͂��؂�čU������", 0 , ItemType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slim.png", 600, 350, 5, 2, 240, 240, 6, 8);
}

//�X���C���̍U���v�Z
void SlimBell::SkillUse(Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//�_���[�W���v�Z����
	auto damage = (Power  - enemyStatus_.GetEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/dageki_2.mp3");

}

//�X�l�[�N�̗���g�������̏���
SnakeBell::SnakeBell(): Skill(21 , "�X�l�[�N���Ă�" , 10 , "�X�l�[�N�̗͂��؂�čU������", 0 , ItemType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", 600, 350, 5, 2, 240, 240, 8, 8);
}

//�X�l�[�N�̗���g�������̏���
void SnakeBell::SkillUse(Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//�_���[�W���v�Z����
	auto damage = (Power  - enemyStatus_.GetEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Player", SkillName, "Enemy", static_cast<int>(damage));

}

//---------------------------------------------------------------------------------------------------------
//�G�̃X�L��

//�J�I�X�t���A
ChaosFlare::ChaosFlare():Skill(5 , "�J�I�X�t���A", 3.5f , "��_�W���̉΂̋ʂ�����A���������G�ɑ�_���[�W��^����",0 , AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/chaos_flare.png", 600, 350, 5, 4, 192, 192, 20, 3);
}

//�J�I�X�t���A�g�������̏���
void ChaosFlare::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto PlayerDefance = playerStatus.GetDefance();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float damage = (EnemyMagicPower * Power) - PlayerDefance;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_chaos_flare.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_chaos_flare.mp3");
}

//�f�X�E�B���h
DeathScytheWind::DeathScytheWind() :Skill(6, "�f�X�E�B���h", 3.5f, "���͂ȕ������̖��@�����", 0, AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DeathScytheWind.png", 600, 350, 5, 4, 192, 192, 20, 8);
}

//�f�X�T�C�Y�E�B���h�g�������̏���
void DeathScytheWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto PlayerDefance = playerStatus.GetDefance();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float damage = (EnemyMagicPower * Power) - PlayerDefance;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_death_scythe_wind.mp3");
}

//�_�[�N�N���E
DrakClaw::DrakClaw() : Skill(7 , "�_�[�N�N���E" , 3 , "�ł̒܂̍U��",0 ,AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DrakClaw.png", 600, 350, 5, 5, 192, 192, 25, 8);
}

//�_�[�N�N���E�g�������̏���
void DrakClaw::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_, Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / 2 * Power) - PlayerDefance / 4;

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - damage);

	//�o�g�����O�𗬂�
	battle_log->addSkillUseLog("Enemy", SkillName, "Player", static_cast<int>(damage));

	//SE�𗬂�
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/enemy_drak_claw.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(70, "sound/SoundEffect/enemy_drak_claw.mp3");
}
