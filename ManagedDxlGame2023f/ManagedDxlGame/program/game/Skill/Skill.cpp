#include "Skill.h"
#include"../Menu/BattleLog.h"
#include <ctime>
#include <random>
#include"../Manager/GameManager.h"

Skill::Skill(const int _id, const std::string& _name, const float _power, const std::string& _description, const int consumeMp, const int skilltype) : BaseSkill(_id, _name, _power, _description, consumeMp, skilltype)
{

}

Skill::~Skill()
{
	tnl::DebugTrace("\n------------------------------------------------------------");
	tnl::DebugTrace("\n %s���������܂���", skill_name.c_str());
	tnl::DebugTrace("\n------------------------------------------------------------");

}

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

//mp����
void Skill::SkillMpConsume(Player::PlayerStatus& playerStatus)
{
	//Mp���擾����
	auto CurentMp = playerStatus.GetCurentMp();

	//���݂�Mp�������Mp�����炷
	auto consumeMp = CurentMp - consume_mp;

	//Mp���������
	playerStatus.SetPlayerCurentMp(consumeMp);
}

//�S�̂̃X�L�����i�[����
void Skill::AddSkillList()
{
	//��̏ꍇ�̂�
	if (skill_list.empty()) {
		skill_list.emplace_back(std::make_shared<FlameSlash>());
		skill_list.emplace_back(std::make_shared<FireStorm>());
		skill_list.emplace_back(std::make_shared<Inferno>());
		skill_list.emplace_back(std::make_shared<IceBlast>());
		skill_list.emplace_back(std::make_shared<WaterBlade>());
		skill_list.emplace_back(std::make_shared<Blizzard>());
		skill_list.emplace_back(std::make_shared<ThunderBolt>());
		skill_list.emplace_back(std::make_shared<LightningSlash>());
		skill_list.emplace_back(std::make_shared<Heal>());
		skill_list.emplace_back(std::make_shared<HighHeels>());
		skill_list.emplace_back(std::make_shared<SlimBell>());
		skill_list.emplace_back(std::make_shared<SnakeBell>());
	}
}

//�����_���t�@�N�^�[�̒l��ݒ肷��
void Skill::initializeRandomFactor()
{
	// ���݂̎������V�[�h�Ƃ��ă����_���G���W����������
	std::default_random_engine generator(static_cast<unsigned int>(std::time(nullptr)));

	// 0.95 ���� 1.05 �͈̔͂ň�l�ɕ��z���郉���_���l�𐶐�
	std::uniform_real_distribution<float> distribution(0.95f, 1.05f); // �}5%�̃����_���ϓ�

	// �����_���t�@�N�^�[�𐶐����A�����o�ϐ� random_factor �Ɋi�[
	random_factor = distribution(generator);
}

void Skill::PlayerReductionRate(float& damage)
{
	// �v���C���[�̃��x�����擾
	int playerLevel = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetLevel();

	// ���x���ɂ��y�������v�Z
	float reductionFactor = playerLevel * BASE_REDUCTION_RATE; // ���x�����Ƃ�3.5%�y��

	// �ő�y������ݒ�i70%�y���j
	reductionFactor = (reductionFactor > koni::Numeric::PERCENT_70) ? koni::Numeric::PERCENT_70 : reductionFactor;

	// �y������K�p
	damage *= (koni::Numeric::FLOAT_VALUE_10 - reductionFactor);
}

void Skill::CheckEpicWeapon(float& damage , Enemy::EnemyConnection& enemyStatus_)
{
	auto weapon_array = GameManager::GetGameManager()->GetInventory()->getEquipArray();

	// Epic�����̃`�F�b�N
	bool isEpic = false;

	//���킪���ꕐ�킩�m�F����
	for (auto& weapon_ : weapon_array) {
		isEpic = weapon_.GetEpic();

		//���킪���ꕐ��ł��A�_���[�W�𑽂��^������G�������ꍇ1.3�{�̕␳��������
		if (isEpic && weapon_.GetEpicCorrection() == enemyStatus_.GetEnemyRace()) {
			damage *= koni::Numeric::SCALE_ONE_THIRTY_F;
		}
	}
}

//---------------------------------------------------------------------------------------------------------

//�ʏ�U��
Nomal_Attack::Nomal_Attack(int weapon_type): Skill(0, "�U��", 1, "�ʏ�U��",0 , AttackType)
{
	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 120;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 120;



	//����̎�ނɂ���ăA�j���[�V������ς���
	switch (weapon_type)
	{
	case EMPTY:

		//�G�t�F�N�g�̕��̐�
		effect_width_num = 5;

		//�G�t�F�N�g�̍����̐� 
		effect_height_num = 2;

		//�G�t�F�N�g�̃t���[���̍��v
		effect_frame_all_num = effect_width_num * effect_height_num;

		//�����������Ă��Ȃ����̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack�QEmpty.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Se�T�E���h
		sound_text = "sound/SoundEffect/dageki_3.mp3";

		break;

	case SWOED:

		//���W��ς���
		effect_pos.x = 630;

		//�G�t�F�N�g�̕��̐�
		effect_width_num = 5;

		//�G�t�F�N�g�̍����̐� 
		effect_height_num = 1;

		//�G�t�F�N�g�̃t���[���̍��v
		effect_frame_all_num = effect_width_num * effect_height_num;

		//���𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Sword.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Se�T�E���h
		sound_text = "sound/SoundEffect/zanngeki_2.mp3";

		break;

	case HAMMER:

		//�G�t�F�N�g�̕��̐�
		effect_width_num = 7;

		//�G�t�F�N�g�̍����̐� 
		effect_height_num = 1;

		//�G�t�F�N�g�̃t���[���̍��v
		effect_frame_all_num = effect_width_num * effect_height_num;

		//�n���}�[�𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Hammer.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Se�T�E���h
		sound_text = "sound/SoundEffect/dageki.mp3";

		break;

	case SPEAR:

		//�G�t�F�N�g�̕��̐�
		effect_width_num = 7;

		//�G�t�F�N�g�̍����̐� 
		effect_height_num = 1;

		//�G�t�F�N�g�̃t���[���̍��v
		effect_frame_all_num = effect_width_num * effect_height_num;

		//���𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Spear.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Se�T�E���h
		sound_text = "sound/SoundEffect/spear.mp3";

		break;

	case LONGSWORD:

		//�G�t�F�N�g�̕��̐�
		effect_width_num = 5;

		//�G�t�F�N�g�̍����̐� 
		effect_height_num = 2;

		//�G�t�F�N�g�̕��̃T�C�Y
		effect_width_size = 192;

		//�G�t�F�N�g�̏c�̃T�C�Y
		effect_height_size = 192;

		//�G�t�F�N�g�̃t���[���̍��v
		effect_frame_all_num = effect_width_num * effect_height_num;

		//�����O�\�[�h�𑕔����Ă��鎞�̃A�j���[�V����
		Effect_Animation = std::make_shared<Animation>("graphics/Effect/NomalAttack_Long_Sword.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

		//Se�T�E���h
		sound_text = "sound/SoundEffect/long_sword.mp3";

		break;


	default:

		tnl::DebugTrace("---�A�j���[�V�����𐶐��ł��܂���ł����B---");
		tnl::DebugTrace("---Se�𐶐��ł��܂���ł���---");

		break;
	}

}

Nomal_Attack::~Nomal_Attack()
{
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/dageki_3.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/zanngeki_2.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/dageki.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/spear.mp3");
	SoundManager::GetSoundManager()->DeleteSound("sound/SoundEffect/long_sword.mp3");
}

//�ʏ�U�����s�������̏���(�h���N�G���v�Z��(�C���t���ɂ��Ή��ł���))
void Nomal_Attack::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto player_level = playerStatus.GetLevel();
	auto weapon_array = GameManager::GetGameManager()->GetInventory()->getEquipArray();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���t�@�N�^�[�̒l������������
	//�����_���v�f�������鎖�Ő퓬�Ƀn���n�������o��
	initializeRandomFactor();

	// �_���[�W���v�Z����i�X�P�[�����O�t�@�N�^�[�ƃ����_���t�@�N�^�[��K�p�j
	float damage = static_cast<float>(((PlayerAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) - EnemyDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR)) * power) * scaling_factor * random_factor);

	// �_���[�W��0�����������1�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//���ꕐ�킩���m�F���Ă������ꕐ�킾�����ꍇ�␳��������
	CheckEpicWeapon(damage, enemyStatus_);

	// �G��HP�����炷
	enemyStatus_.SetEnemyHp(enemyStatus_.GetEnemyHp() - static_cast<int>(damage));

	// �퓬���O�Ƀ_���[�W���ʂ��o��
	const std::string log = player_name + "��" + enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	// SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play(sound_text.c_str(), DX_PLAYTYPE_BACK);

	// �{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, sound_text.c_str());

}

//------------------------------------------------------------------------------------------------------------------------------

//�Ή��a��
FlameSlash::FlameSlash() : Skill(1, "�Ή��a��", 1.5f, "���̗͂��܂Ƃ����U��",2 , AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/FlameSlash.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�Ή��a����g�������̏���
void FlameSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_ , const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ł����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance)* power) * scaling_factor * random_factor);

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

	//���ꕐ�킩���m�F���Ă������ꕐ�킾�����ꍇ�␳��������
	CheckEpicWeapon(damage, enemyStatus_);

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͉΂̗͂�Z���U������!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/FlameSlash.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/FlameSlash.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//�t�@�C�A�[�X�g�[��

FireStorm::FireStorm():Skill(19, "�t�@�C�A�[�X�g�[��", 1.8f, "���̉Q�ōU������", 3, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 6;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 3;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/fire_storm.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay,koni::Numeric::SCALE_TRIPLE_F);

}

void FireStorm::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// ��{�_���[�W�̌v�Z
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

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
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͉��̉Q�����o����!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/vortex.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/vortex.mp3");
}

//------------------------------------------------------------------------------------------------------------------------------

//�C���t�F���m

Inferno::Inferno() : Skill(22, "�C���t�F���m", 2.5f, "���̋Ɖ΂ōU������", 8, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/inferno.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_F);
}

void Inferno::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyFireResist = enemyStatus_.GetFireResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// ��{�_���[�W�̌v�Z
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

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
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "���R�����鉊�����������A�S�Ă��Ă��s�������I";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/Hellfire.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/Hellfire.mp3");
}

//------------------------------------------------------------------------------------------------------------------------------

//�A�C�X�u���X�g
IceBlast::IceBlast():Skill(2, "�A�C�X�u���X�g", 1.8f, "�X�����̔������@", 3 , AttackType)
{
	//���W��ς���
	effect_pos.y = 360;

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/IceBlast.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay);
}

//�A�C�X�u���X�g���g�������̌v�Z����
void IceBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const  Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

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
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͏����ȗ�C�̉�����o������������!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/ice_blast.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ice_blast.mp3");
	
}

//------------------------------------------------------------------------------------------------------------------------------

//�E�H�[�^�[�u���[�h

WaterBlade::WaterBlade() : Skill(23, "�E�H�[�^�[�u���[�h", 1.5f, "����Z���U������", 3, AttackType)
{
	//���W��ς���
	effect_pos.y = 380;

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 6;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x���b����ύX����
	effect_delay = 4;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/weter_blade.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_QUINTUPLE_F);
}

void WaterBlade::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ł����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance) * power) * scaling_factor * random_factor);

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

	//���ꕐ�킩���m�F���Ă������ꕐ�킾�����ꍇ�␳��������
	CheckEpicWeapon(damage, enemyStatus_);

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͐��̗͂�Z���U������!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/swingdown.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/swingdown.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//�u���U�[�h

Blizzard::Blizzard() : Skill(24, "�u���U�[�h", 2.5f, "�ƂĂ��Ȃ���C�����o������������", 8, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x���b����ύX����
	effect_delay = 7;

	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Blizzard.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_width_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_DOUBLE_AND_HALF);

}

void Blizzard::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyIceResist = enemyStatus_.GetIceResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// ��{�_���[�W�̌v�Z
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

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
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͂ƂĂ��Ȃ���C�����o�������������I";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/Blizzard.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/Blizzard.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//�T���_�[�{���g
ThunderBolt::ThunderBolt():Skill(3, "�T���_�[�{���g", 1.5f, "�������̔������@", 5 , AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/ThunderBolt.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�T���_�[�{���g�g�������̏���
void ThunderBolt::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerMagicPower = playerStatus.GetMagicPower();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();


	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = (PlayerMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * scaling_factor * random_factor;

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
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͏����ȗ��𔭐������A����������!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/ThunderBolt.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//���C�g�j���O�X���b�V��

LightningSlash::LightningSlash() : Skill(25, "���C�g�j���O�X���b�V��", 2.3f, "���̗͂�тт��a���Ń_���[�W��^����", 8, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 640;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 480;

	//�x���b����ύX����
	effect_delay = 3;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightningSlash.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

}

void LightningSlash::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto PlayerAttack = playerStatus.GetAttack();
	auto EnemyDefance = enemyStatus_.GetEnemyDefance();
	auto EnemyThunderResist = enemyStatus_.GetThunderResist();
	auto player_level = playerStatus.GetLevel();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ł����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0) { return; }

	//�X�P�[�����O�t�@�N�^�[�̒l��ݒ肷��
	//�v���C���[�̌��݃��x���ɉ����ĈЗ͂�ݒ肷��
	//�v���C���[�̃��x�����オ�����ۂɂ�����������Ă��炤��
	scaling_factor = 1 + static_cast<float>(player_level) / MAX_LEVEL;

	//�����_���l�����߂�
	initializeRandomFactor();

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float baseDamage = static_cast<float>(((PlayerAttack - EnemyDefance) * power) * scaling_factor * random_factor);

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

	//���ꕐ�킩���m�F���Ă������ꕐ�킾�����ꍇ�␳��������
	CheckEpicWeapon(damage, enemyStatus_);

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "�͗��̗͂�тт��a���𔭐�������!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/sword_2.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/sword_2.mp3");

}

//------------------------------------------------------------------------------------------------------------------------------

//�񕜎���

Heal::Heal(): Skill(4, "�q�[��", 0, "Hp���񕜂���", 2, BuffType)
{
	//���W�����߂�
	effect_pos = { 160 , 160 };

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�G�t�F�N�g�𐶐�����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Heal.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�q�[�����g�����ۂ̏���
void Heal::SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)
{
	auto max_hp =  playerStatus.GetMaxHp();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//Hp��maxhp����������Ă����ꍇ�g�p�ł���
	if (playerStatus.GetcurentHp()< max_hp) {
		// HP���ő�Hp��30���񕜂�����
		float percentage = 0.3f;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hp���񕜂���
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//�񕜉���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");


		//�o�g�����O�𗬂�
		battle_log->AddRecoveryLog(player_name, skill_name, healAmount);
	}

}

//------------------------------------------------------------------------------------------------------------------------------

//�n�C�q�[��

HighHeels::HighHeels() :Skill(26, "�n�C�q�[��", 0, "Hp���񕜂���", 4, BuffType)
{
	//���W�����߂�
	effect_pos = { 160 , 160 };

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�G�t�F�N�g�𐶐�����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/HighHeels.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);

}

void HighHeels::SkillUse(Player::PlayerStatus& playerStatus, const Shared<BattleLog>& battle_log)
{
	auto max_hp = playerStatus.GetMaxHp();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//Hp��maxhp����������Ă����ꍇ�g�p�ł���
	if (playerStatus.GetcurentHp() < max_hp) {
		// HP���ő�Hp��30���񕜂�����
		float percentage = 0.5f;
		int healAmount = static_cast<int>(max_hp * percentage);

		//Hp���񕜂���
		playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() + healAmount);

		// HP��MAX_HP�𒴂���ꍇ��MAX_HP�ɃN���b�v����
		if (playerStatus.GetcurentHp() > max_hp) {
			playerStatus.SetPlayerCurentHp(max_hp);
		}

		//�񕜉���炷
		SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/kaihuku.mp3", DX_PLAYTYPE_BACK);

		//�{�����[����ς���
		SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/ThunderBolt.mp3");


		//�o�g�����O�𗬂�
		battle_log->AddRecoveryLog(player_name, skill_name, healAmount);
	}
}


//---------------------------------------------------------------------------------------------------------
//�A�C�e���n�̃X�L��

//�X���C���̍U��
SlimBell::SlimBell() : Skill(20 , "�X���C�����Ă�",5, "�X���C���̗͂��؂�čU������", 0 , AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 240;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 240;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slim.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�X���C���̍U���v�Z
void SlimBell::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�_���[�W���v�Z����
	auto damage = (power  - enemyStatus_.GetEnemyDefance() / static_cast<float>(koni::Numeric::DIVIDER_TWO));

	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "������Ȃ炵���B��������X���C��������Ă���!";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);


	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/dageki_2.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/dageki_2.mp3");

}

//�A�[�X���[�̗���g�������̏���
SnakeBell::SnakeBell(): Skill(21 , "�A�[�X���[�����Ă�" , 10 , "�A�[�X���[���̗͂��؂�čU������", 0 , AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 240;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 240;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Snake.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num , effect_delay);
}

//�X�l�[�N�̗���g�������̏���
void SnakeBell::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	//�G������ŋ����珈�����Ƃ΂�
	if (enemyStatus_.GetEnemyHp() <= 0)return;
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�_���[�W���v�Z����
	auto damage = (power  - enemyStatus_.GetEnemyDefance() / static_cast<float>(koni::Numeric::DIVIDER_TWO));

	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	enemyStatus_.SetEnemyHp(static_cast<int>(enemyStatus_.GetEnemyHp() - damage));

	//�o�g�����O�𗬂�
	std::string log = player_name + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = player_name + "������Ȃ炵���B�A�[�X���[��������Ă��ēG�ɖڊ|���Ĕ�т���";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);


}

//---------------------------------------------------------------------------------------------------------
//�G�̃X�L��

//�J�I�X�t���A
ChaosFlare::ChaosFlare():Skill(5 , "�J�I�X�t���A", 2.3f , "��_�W���̉΂̋ʂ�����A���������G�ɑ�_���[�W��^����",0 , AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 3;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/chaos_flare.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�J�I�X�t���A�g�������̏���
void ChaosFlare::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const  Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	//�����_���l��ݒ肳����
	initializeRandomFactor();

	// ��{�_���[�W�̌v�Z
	float damage =(EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * random_factor;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString()+ "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͖��͂��Ɍ��܂ō��߁A������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_chaos_flare.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_chaos_flare.mp3");
}

//�f�X�E�B���h
DeathScytheWind::DeathScytheWind() :Skill(6, "�f�X�E�B���h", 2.3f, "���͂ȕ������̖��@�����", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DeathScytheWind.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_TRIPLE_F);
}

//�f�X�T�C�Y�E�B���h�g�������̏���
void DeathScytheWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// ��{�_���[�W�̌v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͈Í��̕������o����";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_death_scythe_wind.mp3");
}

//�_�[�N�N���E
DrakClaw::DrakClaw() : Skill(7 , "�_�[�N�N���E" , 3 , "�ł̒܂̍U��",0 ,AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 5;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DrakClaw.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�_�[�N�N���E�g�������̏���
void DrakClaw::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͎����̖��͂Œ܂����o���A�U������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_drak_claw.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_drak_claw.mp3");
}

//�����̕�
LifeWind::LifeWind() : Skill(8, "�����̕�", 1.2f, "������U����Hp�����񕜂���", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/Life_Wind.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

//�X�L���g�p���̏���
void LifeWind::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// ��{�_���[�W�̌v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�^�����_���[�W��3����Hp���񕜂�����
	int heal_amoument = static_cast<int>(damage * koni::Numeric::PERCENT_30);

	//�G��Hp���擾����
	auto enemy_hp = enemyStatus_.GetEnemyHp();

	//�G��Hp�������񕜂���
	enemyStatus_.SetEnemyHp(enemy_hp + heal_amoument);

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͓˕�����肾���U������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/life_wind.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/life_wind.mp3");
}

//�G�i�W�[�u���X�g
EnergyBlast::EnergyBlast() : Skill(9, "�G�i�W�[�u���X�g", 1.5f, "���͂����߂ăr�[�������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�G�t�F�N�g�̃T�C�Y
	const float EFFECT_SIZE = 3.5f;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/energy_blast.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, EFFECT_SIZE);
}

//�G�i�W�[�u���X�g���g�������̏���
void EnergyBlast::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	// �v���C���[������ł����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0) return;

	// ��{�_���[�W�v�Z�Ƀ����_���v�f��ǉ�
	initializeRandomFactor();

	// �G�̖��͂����������_���[�W�ɉ������A���@�͂�10������悹����
	float magic_boost = EnemyMagicPower * koni::Numeric::PERCENT_10; 

	// ��{�_���[�W�v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power + magic_boost) * BASE_MULTIPLIER * random_factor;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	// �v���C���[��HP�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	// �o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͖��͂��W����������������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	// SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/energy_blast.mp3", DX_PLAYTYPE_BACK);

	// �{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_50_PERCENT, "sound/SoundEffect/energy_blast.mp3");
}


//�_�[�N�`���[�W
DarkCharge::DarkCharge() : Skill(10, "�_�[�N�`���[�W", 2.0f, "�ł̋ʂ���G�l���M�[�𒊏o������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 7;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/DarkCharge.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay);
}

void DarkCharge::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// ��{�_���[�W�̌v�Z
	float damage =(EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͋ʂ���ł̗͂𒊏o���A��荞��ŕ��o����";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//�ł̔g��
WaveDarkness::WaveDarkness(): Skill(11, "�ł̔g��", 2.5f, "�ł̗͂𑝕������U������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 4;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/wave_drak.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay,koni::Numeric::SCALE_DOUBLE_AND_HALF);
}

void WaveDarkness::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͈ł̗͂𑝕���������������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//�n���̉��n
HellFlameBlade::HellFlameBlade():Skill(12, "�n���̉��n", 2.6f, "�n���̉���Z���U������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 4;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/hell_fire_slach.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_DOUBLE_AND_HALF);

}

void HellFlameBlade::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͒n���̉���Z���U������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/sword_2.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/sword_2.mp3");

}

//���̎a��
SlashLight::SlashLight(): Skill(13, "���̎a��", 1.3f, "����Z���U������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 2;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/slash_light.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void SlashLight::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �G�̍U���͂������������_���[�W�ɉ������A�U���͂�10������悹����
	float attack_boost = EnemyAttack * koni::Numeric::PERCENT_10;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power * attack_boost) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͌���Z���a���𔭐�������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/slash_flash.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/slash_flash.mp3");

}

//�p�Y�̗�
PowerHeroes::PowerHeroes(): Skill(14, "�p�Y�̗�", 1.2f, "�͂Ɩh��͂𑝕�������", 0, BuffType)
{
	//�G�t�F�N�g�̍��W��ύX����
	effect_pos.y = 500;

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 4;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/PowerHeroes.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay , koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void PowerHeroes::SkillUse(Enemy::EnemyConnection& enemyStatus, const Shared<BattleLog>& battle_log)
{
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G�̍U���͂�1.2�{�㏸������
	enemyStatus.SetEnemyAttack(static_cast<int>(enemyStatus.GetEnemyAttack() * power));

	//�G�̖h��͂�1.2�{�㏸������
	enemyStatus.SetEnemyDefance(static_cast<int>(enemyStatus.GetEnemyDefance() * power));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus.GetEnemyString() + "�͐��_���ꂵ�Ď��g�̍U���͂Ɩh��͂��㏸������";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/status_up.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/status_up.mp3");

}

//���̑�
LightSpear::LightSpear() : Skill(15, "���̑�", 2.0f, "���𑄂ɕω������U������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightSpear.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_DOUBLE_F);

}


void LightSpear::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͌��̑������o���U�����Ă���";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/light_spear.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/light_spear.mp3");
}

//���_�̊�
GrimReaperSickle::GrimReaperSickle() : Skill(16, "���_�̊�", 2.0f, "���_�̊��ł�����������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 4;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/GrimReaperSickle.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}


void GrimReaperSickle::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyAttack = enemyStatus_.GetEnemyAttack();
	auto PlayerDefance = playerStatus.GetDefance();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�G������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������ă_���[�W���v�Z
	float damage = (EnemyAttack / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) - PlayerDefance / static_cast<float>(koni::Numeric::DIVIDER_FOUR);

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�v���C���[��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	// �G�̍U���͂�10���㏸������
	int attack_boost = static_cast<int>(EnemyAttack * koni::Numeric::PERCENT_10);

	//�U���͂��㏸������
	enemyStatus_.SetEnemyAttack(EnemyAttack + attack_boost);

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͎��_�̊��ōU�����Ă����I";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/enemy_death_scythe_wind.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/enemy_death_scythe_wind.mp3");

}

//���̊�����
SoulReper::SoulReper(): Skill(17, "���̊�����", 1.8f, "���������菭��HP���񕜂���", 0, AttackType)
{
	//�G�t�F�N�g�̍��W��ύX����
	effect_pos.y = 500;

	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/SoulReper.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_SIXFOLD_F);

}

void SoulReper::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// �U���͂���h��͂������Ċ�{�_���[�W���v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(static_cast<int>(playerStatus.GetcurentHp() - static_cast<int>(damage)));

	//�^�����_���[�W��5����Hp���񕜂�����
	int heal_amoument = static_cast<int>(damage * koni::Numeric::PERCENT_50);

	//�G��Hp���擾����
	auto enemy_hp = enemyStatus_.GetEnemyHp();

	//�G��Hp�������񕜂���
	enemyStatus_.SetEnemyHp(enemy_hp + heal_amoument);

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͖����̍��������o���Ԃ��Ă���";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_70_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}

//���{�̌�
LightUnderworld::LightUnderworld() : Skill(18, "���{�̌�", 1.6f, "���{�̌��𗁂т��čU������A����̍U���͂�����������", 0, AttackType)
{
	//�G�t�F�N�g�̕��̐�
	effect_width_num = 5;

	//�G�t�F�N�g�̍����̐� 
	effect_height_num = 3;

	//�G�t�F�N�g�̃t���[���̍��v
	effect_frame_all_num = effect_width_num * effect_height_num;

	//�G�t�F�N�g�̕��̃T�C�Y
	effect_width_size = 192;

	//�G�t�F�N�g�̏c�̃T�C�Y
	effect_height_size = 192;

	//�x�����Ԃ�ύX����
	effect_delay = 5;

	//�A�j���[�V�������쐬����
	Effect_Animation = std::make_shared<Animation>("graphics/Effect/LightUnderworld.png", effect_pos.x, effect_pos.y, effect_width_num, effect_height_num, effect_width_size, effect_height_size, effect_frame_all_num, effect_delay, koni::Numeric::SCALE_TRIPLE_AND_HALF);

}

void LightUnderworld::SkillUse(Player::PlayerStatus& playerStatus, Enemy::EnemyConnection& enemyStatus_, const Shared<BattleLog>& battle_log)
{
	auto EnemyMagicPower = enemyStatus_.GetMagicPower();
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�v���C���[������ŋ����珈�����Ƃ΂�
	if (playerStatus.GetcurentHp() <= 0)return;

	// ��{�_���[�W�̌v�Z
	float damage = (EnemyMagicPower / static_cast<float>(koni::Numeric::DIVIDER_TWO) * power) * BASE_MULTIPLIER;

	//�v���C���[�̃��x���ɂ��y�����̌v�Z
	PlayerReductionRate(damage);

	// �_���[�W�����̒l�ɂȂ�Ȃ��悤�ɂ���
	if (damage <= 0) {
		damage = 1;
	}

	//�G��Hp�����炷
	playerStatus.SetPlayerCurentHp(playerStatus.GetcurentHp() - static_cast<int>(damage));

	//�v���C���[�̍U���͂�10������������
	int player_attack_down_parcentage = static_cast<int>(playerStatus.GetAttack() * koni::Numeric::PERCENT_10);

	//�v���C���[�̍U���͂���������������
	playerStatus.SetPlayerAttack(playerStatus.GetAttack() - player_attack_down_parcentage);

	//�o�g�����O�𗬂�
	std::string log = enemyStatus_.GetEnemyString() + "��" + skill_name + "���g�p���܂����B";
	battle_log->AddLog(log);
	log = enemyStatus_.GetEnemyString() + "�͖��{�̌����Ăяo���U�����Ă���";
	battle_log->AddLog(log);
	log = player_name + "�̍U���͂�������������";
	battle_log->AddLog(log);
	log = player_name + "��" + std::to_string(static_cast<int>(damage)) + "�̃_���[�W��^����";
	battle_log->AddLog(log);

	//SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play("sound/SoundEffect/effect_se_enemy.mp3", DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(koni::Numeric::VOLUME_60_PERCENT, "sound/SoundEffect/effect_se_enemy.mp3");
}
