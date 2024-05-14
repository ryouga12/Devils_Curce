#include "Skill.h"


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
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/dageki_3.mp3");

		break;

	case SWOED:

		//���𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", 630, 350, 5, 1, 120, 120, 5, 8);

		//Se�T�E���h
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/zanngeki_2.mp3");

		break;

	case HAMMER:

		//�n���}�[�𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", 600, 350, 7, 1, 120, 120, 7, 8);

		//Se�T�E���h
		Sound = SoundManager::getSoundManager()->LoadSoundBGM("sound/SoundEffect/dageki.mp3");

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
int Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();

	//�_���[�W���v�Z����
	int damage = (PlayerAttack / 2 - EnemyDefance / 4) * Power;

	//�_���[�W��0�����������1�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	return damage;
}

//�ʏ�U�����s�������̃A�j���[�V����
void Nomal_Attack::SkillUseAnimation()
{
	//�A�j���[�V�����̗���
	Effect_Animation->play_animation();

	//����炷
	PlaySoundMem(Sound, DX_PLAYTYPE_BACK);

}

//�ʏ�U���̃A�j���[�V�������~�߂�
void Nomal_Attack::SkillAnimationStop()
{
	//�ʏ�U���̃A�j���[�V�������~�߂�
	Effect_Animation->stop_animation();
}

//�ʏ�U���̃A�j���[�V�����̕`��
void Nomal_Attack::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�ʏ�U���������ۂ̃A�j���[�V�����̍X�V����
void Nomal_Attack::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//------------------------------------------------------------------------------------------------------------------------------

//�Ή��a��
FlameSlash::FlameSlash() : Skill(1, "�Ή��a��", 1.5f, "���̗͂��܂Ƃ����U��",2 , AttackType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", 600, 350, 5, 3, 192, 192, 15, 8);
}

//�Ή��a����g�������̏���
int FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerAttack = playerStatus.getAttack();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyFireResist = enemyStatus_.getFireResist();

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

	return static_cast<int>(damage);
}

//�Ή��a����g�����ۂ̃A�j���[�V��������
void FlameSlash::SkillUseAnimation()
{
	Effect_Animation->play_animation();

	//�T�E���h
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);
	//���ʂ�ς���(���� , ghpath)
	SoundManager::getSoundManager()->ChangeSoundVolume(100, "sound/SoundEffect/FlameSlash.mp3");
}

//�Ή��a��̃A�j���[�V�������~�߂�
void FlameSlash::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//�Ή��a��g�����ۂ̃A�j���[�V�����̕`��
void FlameSlash::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�Ή��a����g�����ۂ̃A�j���[�V�����̍X�V����
void FlameSlash::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//�Ή��a����g��������Mp����
void FlameSlash::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.getCurentMp();

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
int IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyIceResist = enemyStatus_.getIceResist();

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

	return static_cast<int>(damage);
	
}

//�A�C�X�u���X�g�g�����ۂ̃A�j���[�V��������
void IceBlast::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

void IceBlast::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//�A�C�X�u���X�g�A�j���[�V�������~�߂�
void IceBlast::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�A�C�X�u���X�g�g�����ۂ̃A�j���[�V�����̍X�V����
void IceBlast::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//�A�C�X�u���X�g�g��������Mp����
void IceBlast::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.getCurentMp();

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
int ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyStatus& enemyStatus_)
{
	auto PlayerMagicPower = playerStatus.getMagicPower();
	auto EnemyDefance = enemyStatus_.getEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.getThunderResist();

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

	return static_cast<int>(damage);
}

//�T���_�[�{���g�s�������̃A�j���[�V����
void ThunderBolt::SkillUseAnimation()
{
	Effect_Animation->play_animation();

	//�T�E���h
	SoundManager::getSoundManager()->sound_Play("sound/SoundEffect/dengeki.mp3", DX_PLAYTYPE_BACK);
	//���ʂ�ς���(���� , ghpath)
	SoundManager::getSoundManager()->ChangeSoundVolume(50, "sound/SoundEffect/dengeki.mp3");
}

//�T���_�[�{���g�g�����ۂ̃A�j���[�V�������~�߂�
void ThunderBolt::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//�T���_�[�{���g�g�����ۂ̃A�j���[�V�����̕`��
void ThunderBolt::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�T���_�[�{���g�g�����ۂ̃A�j���[�V�����̍X�V����
void ThunderBolt::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

//�T���_�[�{���g�g��������Mp����
void ThunderBolt::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.getCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - ConsumeMp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//�񕜎���
Heal::Heal(): Skill(4, "�q�[��", 0, "Hp���񕜂���", 2, BuffType)
{
}

//�q�[�����g�����ۂ̏���
int Heal::SkillUse(Player::PlayerStatus& playerStatus)
{
	auto max_hp =  playerStatus.getMaxHp();

	//Hp��maxhp����������Ă����ꍇ�g�p�ł���
	if (playerStatus.getcurentHp()< max_hp) {
		// HP���ő�Hp��30���񕜂�����
		float percentage = 0.3;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hp���񕜂���
		playerStatus.SetPlayerCurentHp(playerStatus.getcurentHp() + healAmount);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (playerStatus.getcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		return healAmount;
	}
	
	//������ĂȂ�������g�p�ł��Ȃ�
	return 0;
}

//�q�[�����g��������Mp����
void Heal::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.getCurentMp();

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
int SlimBell::SkillUse(Enemy::EnemyStatus& enemyStatus_)
{
	//�_���[�W���v�Z����
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	return static_cast<int>(damage);
}

//�X���C���̗���g�������̃A�j���[�V����
void SlimBell::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

//�X���C���̗�g�����ۂ̃A�j���[�V�������~�߂�
void SlimBell::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//�X���C���̎g�����ۂ̃A�j���[�V�����̕`��
void SlimBell::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�X���C���̎g�����ۂ̃A�j���[�V�����̍X�V����
void SlimBell::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}

SnakeBell::SnakeBell(): Skill(21 , "�X�l�[�N���Ă�" , 10 , "�X�l�[�N�̗͂��؂�čU������", 0 , ItemType)
{
	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", 600, 350, 5, 2, 240, 240, 8, 8);
}

//�X�l�[�N�̗���g�������̏���
int SnakeBell::SkillUse(Enemy::EnemyStatus& enemyStatus_)
{
	//�_���[�W���v�Z����
	auto damage = (Power  - enemyStatus_.getEnemyDefance() / 2);

	if (damage <= 0) {
		damage = 1;
	}

	return static_cast<int>(damage);
}

//�X�l�[�N�̗���g�������̃A�j���[�V����
void SnakeBell::SkillUseAnimation()
{
	Effect_Animation->play_animation();
}

//�X�l�[�N�̎g�����ۂ̃A�j���[�V�������~�߂�
void SnakeBell::SkillAnimationStop()
{
	Effect_Animation->stop_animation();
}

//�X�l�[�N�̎g�����ۂ̃A�j���[�V�����̕`��
void SnakeBell::SkillAnimationDraw()
{
	Effect_Animation->draw();
}

//�X�l�[�N�̎g�����ۂ̃A�j���[�V�����̍X�V����
void SnakeBell::SkillAnimationUpdate(float delta_time)
{
	Effect_Animation->update(delta_time);
}
