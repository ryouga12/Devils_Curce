#include "Enemy.h"
#include"../Scene/battleScene.h"
#include"../Menu/BattleLog.h"
#include"../Manager/GameManager.h"
#include"../Object/Player.h"
#include"../Manager/CsvManager.h"
#include"../Manager/EventManager.h"

Enemy::Enemy()
{
	//csv�̓ǂݍ���	
	enemy_csv_array = CsvManager::GetCsvManager()->GetEnemyStatusCsv();

	//Enemy�̃X�e�[�^�X�����[�h����֐�
	EnemyLoadTypeInfo();
	
	//Enemy�̏�����
	EnemyInit();

	//�A�C�e��
	item = std::make_shared<Item>();

}

//�`�揈��
void Enemy::Draw()
{
	if (!dead_enemy_flag) {
		DrawRotaGraphF(ENEMY_POS.x, ENEMY_POS.y, koni::Numeric::SCALE_TRIPLE_AND_HALF, koni::Color::BLACK, enemy_hdl, true);
	}
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = static_cast<int>(enemy_csv_array.size());

	enemy_status_total_num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csv���疼�O�̎擾
		enemy_status_type.SetEnemyName(enemy_csv_array[y][0].c_str());
		//csv����id�̎擾
		enemy_status_type.SetEnemyId(std::stoi(enemy_csv_array[y][1].c_str()));
		//csv����hp�̎擾
		enemy_status_type.SetEnemyHp(std::stoi(enemy_csv_array[y][2].c_str()));
		//csv����U���͂̎擾
		enemy_status_type.SetEnemyAttack(std::stoi(enemy_csv_array[y][3].c_str()));
		//csv����h��͂̎擾
		enemy_status_type.SetEnemyDefance(std::stoi(enemy_csv_array[y][4].c_str()));
		//csv���炷�΂₳�̎擾
		enemy_status_type.SetEnemySpeed(std::stoi(enemy_csv_array[y][5].c_str()));
		//csv����o���l�̎擾
		enemy_status_type.SetEnemyExpoint(std::stoi(enemy_csv_array[y][6].c_str()));
		//csv���炨���̎擾
		enemy_status_type.SetEnemyMoney(std::stoi(enemy_csv_array[y][7].c_str()));
		//csv����ghpath�̎擾
		enemy_status_type.SetGhdl(enemy_csv_array[y][8].c_str());
		//csv���牊�ϐ��l���擾
		enemy_status_type.SetFireResist((float)std::atof(enemy_csv_array[y][9].c_str()));
		//csv����X�ϐ��l�̎擾
		enemy_status_type.SetIceResist((float)std::atof(enemy_csv_array[y][10].c_str()));
		//csv���痋�ϐ��l�̎擾
		enemy_status_type.SetThunderResist((float)std::atof(enemy_csv_array[y][11].c_str()));
		//csv����ʏ�h���b�v��ID���擾
		enemy_status_type.SetNomalDropID(std::stoi(enemy_csv_array[y][12].c_str()));
		//csv���烌�A�h���b�v��ID���擾
		enemy_status_type.SetRareDoropID(std::stoi(enemy_csv_array[y][13].c_str()));
		//csv����U������SE��path���擾
		enemy_status_type.SetAttackSe(enemy_csv_array[y][14].c_str());
		//csv���疂�@�͂��擾
		enemy_status_type.SetMagicPower(std::stoi(enemy_csv_array[y][15].c_str()));
		//csv����m�[�}���h���b�v�̊m�����擾����
		enemy_status_type.SetNomalDropProbability(std::stoi(enemy_csv_array[y][16].c_str()));
		//csv���烌�A�h���b�v�̊m�����擾����
		enemy_status_type.SetRareDropProbability(std::stoi(enemy_csv_array[y][17].c_str()));
		//csv����G�̎푰���擾����
		enemy_status_type.SetEnemyRace(enemy_csv_array[y][18].c_str());

		enemy_type_array.emplace_back(enemy_status_type);
	}


}

void Enemy::EnemyInit()
{
	//�G���[�p�̔z����N���A���ď���������
	null_array.clear();
}

Enemy::EnemyConnection Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(enemy_type_array.begin(), enemy_type_array.end(), [id]
	(const EnemyConnection& status) {return status.GetEnemyId() == id; });

	if (it != enemy_type_array.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return EnemyConnection{};
	}
}

//�A�C�e���̃h���b�v�����߂�
void Enemy::InitEnemyItemDrop(const int& EnemyID)
{
	enemy_drop_item.clear();

	//�z���ID���r���ĊY������ID���C���x���g���Ɋi�[����
	for (int i = 0; i < enemy_csv_array.size(); i++) {

		//�z���1�s�ڂ͐����̈�+1���Ċm�F���s��
		if (EnemyID == std::stoi(enemy_csv_array[i + 1][1].c_str())) {

			//�C���x���g���ɂ��ꂼ��ɑΉ������A�C�e�����i�[
			enemy_drop_item.emplace_back(item->GetItemById(std::stoi(enemy_csv_array[i + 1][12].c_str())));
 			enemy_drop_item.emplace_back(item->GetItemById(std::stoi(enemy_csv_array[i + 1][13].c_str())));

			break;
		}
	}

}

//�G�̔z�������������
//������ID�͂ǂ̃}�b�v�`�b�v�ɋ��邩�Ŕz��ɓ����G�̏������߂�
void Enemy::InitEnemyArray(const int& id)
{
	//�G�̔z����N���A����
	//�G�̔z�����x�N���A���ē��������擾�ł��Ȃ��悤�ɂ���
	enemy_array.clear();

	//�����̓G(�ア)
	if (id == 1) {
		for (int i = 0; i < ENEMY_NUM; i++) {
			enemy_array.emplace_back(GetEnemyStatus(i));
		}
	}
	//�����̓G(����)
	else if (id == 2) {

		//csv��6�s�ڂ���5�̑I�o����
		for (int i = 6; i < enemy_csv_array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (ENEMY_NUM > enemy_array.size()) {

				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//�G��z��Ɋi�[����
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	//�r���̓G(��������)
	else if (id == 3) {

		//csv��11�Ԗڂ���5�̑I�o����
		for (int i = 11; i < enemy_csv_array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//�G��z��Ɋi�[����
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	//�ጴ(����)
	else if (id == 4) {

		//csv��16�Ԗڂ���5�̑I�o����
		for (int i = 23; i < enemy_csv_array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//�G��z��Ɋi�[����
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	//�{�X�G���A�ɂ���G(����)
	else if (id == 5) {

		//csv��16�Ԗڂ���5�̑I�o����
		for (int i = 16; i < enemy_csv_array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (ENEMY_NUM > enemy_array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(enemy_csv_array[i][1].c_str()));

				//�G��z��Ɋi�[����
				enemy_array.emplace_back(GetEnemyStatus(enmeyid.GetEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
}

//�G�̎��S����
bool Enemy::ChackDeadEnemy()
{
	//�G��Hp��0�ɂȂ������`�F�b�N���s��
	if (enemy_array[enemy_index].GetEnemyHp() <= 0) {

		//�G������ł��玀�S�������Ăяo��
		battle_scene_->DeadEnemyProcces(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave(), enemy_array[enemy_index]);

		//���x���A�b�v����
		battle_scene_->ChackPlayerLevelUp(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave());

		return true;
	}
	else {
		return false;
	}
	
}

//------------------------------------------------------------------------------------------------------------
//
//�G�������X�^�[
//
//-------------------------------------------------------------------------------------------------------------


MobMonster::MobMonster(const int& enemy_id)
{
	//�^�C�v�����u�ɂ���
	enemy_type = Enemytype::MOB;

	//�G��z��Ɋi�[����
	InitEnemyArray(enemy_id);

	//�C���f�b�N�X��1�`4�̃����_���Ȑ����ɐݒ肷��
	enemy_index = rand() % ENEMY_NUM;

	//�V�����z��𐶐�����
	auto& enemy_ = enemy_array[enemy_index];

	//�G�̃O���t�B�b�N�n���h����ǂݍ���
	enemy_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(enemy_.GetEnemyGhdl().c_str());

	//�G�̃A�C�e���h���b�v���i�[����
	InitEnemyItemDrop(enemy_.GetEnemyId());
}

//�G�������X�^�[�̍U������
void MobMonster::EnemyAction(const Shared<BattleLog>& battle_log)
{
	//�v���C���[�̖h��͂��擾����
	auto player_defance = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetDefance();

	//�v���C���[�̖��O���擾����
	auto& player_name = GameManager::GetGameManager()->GetPlayer()->GetPlayerName();

	//�_���[�W���v�Z����u
	int damage = enemy_array[enemy_index].GetEnemyAttack() - player_defance;

	//�_���[�W��0�������Ƃ���1�ɐݒ肷��
	if (damage <= 0) { damage = 1; }

	//�v���C���[��Hp�����炷
	GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().SetPlayerCurentHp(GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave().GetcurentHp() - damage);

	//�o�g�����O�𗬂�
	const std::string  log = player_name + "��" + enemy_array[enemy_index].GetEnemyName() + "����" + std::to_string(damage) + "�_���[�W���������";
	battle_log->AddLog(log);

	// SE�𗬂�
	SoundManager::GetSoundManager()->Sound_Play(enemy_array[enemy_index].GetSeAttack().c_str(), DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::GetSoundManager()->ChangeSoundVolume(80, enemy_array[enemy_index].GetSeAttack().c_str());


}

//------------------------------------------------------------------------------------------------------------
//
//�{�X�����X�^�[
//
//-------------------------------------------------------------------------------------------------------------


BossMonster::BossMonster(const int& enemy_id) {

	//�G�̎�ނ��{�X�ɕύX����
	enemy_type = Enemytype::BOSS;

	//�X�L�����i�[����
	InitEnemySkill(enemy_id);

	//�G�̎��S�����Ȃǔz��ɓ��ꂽ�l���Q�Ƃ��Ă���הz��Ɋi�[����
	enemy_array.emplace_back(GetEnemyStatus(enemy_id));

	//�z��͈�̈�0������
	enemy_index = 0;

	//�G�̔z����i�[����
	auto& enemy = enemy_array[enemy_index];

	//�G�̃O���t�B�b�N�n���h����ǂݍ���
	enemy_hdl = ResourceManager::GetResourceManager()->LoadGraphEX(enemy.GetEnemyGhdl().c_str());

	//�G�̃A�C�e���h���b�v���i�[����
	InitEnemyItemDrop(enemy.GetEnemyId());
}

//�G�̃X�L���z��̏�����
void BossMonster::InitEnemySkill(const int& enemy_id)
{
	//�G�̃X�L���z�������������
	enemy_skill_.clear();

	switch (enemy_id)
	{
	
	//�Ō�̃{�X
	case static_cast<int>(BossType::ZERAHKIEL):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<ChaosFlare>());
		enemy_skill_.emplace_back(std::make_shared<DeathScytheWind>());
		enemy_skill_.emplace_back(std::make_shared<DrakClaw>());
		enemy_skill_.emplace_back(std::make_shared<WaveDarkness>());
		enemy_skill_.emplace_back(std::make_shared<HellFlameBlade>());

		break;

	//���{�X1�̖�
	case static_cast<int>(BossType::GROVEGUARDIAN):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<LifeWind>());
		enemy_skill_.emplace_back(std::make_shared<EnergyBlast>());
		enemy_skill_.emplace_back(std::make_shared<DarkCharge>());

		break;

	//���{�X2�̖�
	case static_cast<int>(BossType::SOULWARRIOR):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<SlashLight>());
		enemy_skill_.emplace_back(std::make_shared<PowerHeroes>());
		enemy_skill_.emplace_back(std::make_shared<LightSpear>());

		break;

	//���{�X3�̖�
	case static_cast<int>(BossType::SHADOWENEMY):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<GrimReaperSickle>());
		enemy_skill_.emplace_back(std::make_shared<SoulReper>());
		enemy_skill_.emplace_back(std::make_shared<LightUnderworld>());

		break;

	//�T�u�{�X
	case static_cast<int>(BossType::CORPORAL):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<FlameSlash>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<WaterBlade>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<ThunderBolt>(Skill::SkillUserType::ENEMY));

		break;


	//���{�X
	case static_cast<int>(BossType::PIRATE):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<WaterForge>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<WaterBlade>(Skill::SkillUserType::ENEMY));
		enemy_skill_.emplace_back(std::make_shared<RainOfHell>());
		enemy_skill_.emplace_back(std::make_shared<ThunderBolt>(Skill::SkillUserType::ENEMY));

		break;

	default:

		tnl::DebugTrace("\n-------------------------------------------------------------");
		tnl::DebugTrace("\n�X�L�����������ł��܂���ł���");
		tnl::DebugTrace("\n-------------------------------------------------------------");

		break;
	}
}

//�{�X�����X�^�[�̍U������
void BossMonster::EnemyAction(const Shared<BattleLog>& battle_log)
{
	//�G�̃X�L���z�񂪋󂶂�Ȃ�������
	if (!enemy_skill_.empty()) {
		
		auto enemy_skill_size = enemy_skill_.size();

		//�����_���ŃC���f�b�N�X���擾����
		enmey_skill_index = rand() % enemy_skill_size;

		//�G�̃X�L�����U���n�������ꍇ�_���[�W�v�Z���s��
		if (enemy_skill_[enmey_skill_index]->GetSkillType() == AttackType) {

			//�v���C���[�̃X�e�[�^�X
			auto& player_status = GameManager::GetGameManager()->GetPlayer()->GetPlayerStatusSave();

			//�v���C���[�ɍU������
			enemy_skill_[enmey_skill_index]->SkillUse(player_status, enemy_array[enemy_index], battle_log);

			//�v���C���[��hp��0�����������0�ɐݒ肷��
			if (player_status.GetcurentHp() <= 0) {
				player_status.SetPlayerCurentHp(0);
			}
		}
		//�o�t�X�L���������ꍇ�A�o�t���ʂ�K�p������
		else if (enemy_skill_[enmey_skill_index]->GetSkillType() == BuffType) {

			//�G�l�~�[�Ƀo�t���ʂ�K�p������
			enemy_skill_[enmey_skill_index]->SkillUse(enemy_array[enemy_index], battle_log);

		}
		
	}
	else {
		tnl::DebugTrace("\n-------------------------------------------------------------");
		tnl::DebugTrace("\n�U���ł��܂���ł���");
		tnl::DebugTrace("\n-------------------------------------------------------------");
		return;

	}
}
