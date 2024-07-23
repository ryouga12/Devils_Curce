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
	enemy_csv_array = CsvManager::getCsvManager()->GetEnemyStatusCsv();

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
		DrawRotaGraphF(ENEMY_POS.x, ENEMY_POS.y, ENEMY_SIZE, COLOR_BRACK, enemy_hdl, true);
	}
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = static_cast<int>(enemy_csv_array.size());

	EnemyStatus_Total_Num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csv���疼�O�̎擾
		Enemy_Status_Type.SetEnemyName(enemy_csv_array[y][0].c_str());
		//csv����id�̎擾
		Enemy_Status_Type.SetEnemyId(std::stoi(enemy_csv_array[y][1].c_str()));
		//csv����hp�̎擾
		Enemy_Status_Type.SetEnemyHp(std::stoi(enemy_csv_array[y][2].c_str()));
		//csv����U���͂̎擾
		Enemy_Status_Type.SetEnemyAttack(std::stoi(enemy_csv_array[y][3].c_str()));
		//csv����h��͂̎擾
		Enemy_Status_Type.SetEnemyDefance(std::stoi(enemy_csv_array[y][4].c_str()));
		//csv���炷�΂₳�̎擾
		Enemy_Status_Type.SetEnemySpeed(std::stoi(enemy_csv_array[y][5].c_str()));
		//csv����o���l�̎擾
		Enemy_Status_Type.SetEnemyExpoint(std::stoi(enemy_csv_array[y][6].c_str()));
		//csv���炨���̎擾
		Enemy_Status_Type.SetEnemyMoney(std::stoi(enemy_csv_array[y][7].c_str()));
		//csv����ghpath�̎擾
		Enemy_Status_Type.SetGhdl(enemy_csv_array[y][8].c_str());
		//csv���牊�ϐ��l���擾
		Enemy_Status_Type.SetFireResist(static_cast<float>(std::stoi(enemy_csv_array[y][9].c_str())));
		//csv����X�ϐ��l�̎擾
		Enemy_Status_Type.SetIceResist(static_cast<float>(std::stoi(enemy_csv_array[y][10].c_str())));
		//csv���痋�ϐ��l�̎擾
		Enemy_Status_Type.SetThunderResist(static_cast<float>(std::stoi(enemy_csv_array[y][11].c_str())));
		//csv����ʏ�h���b�v��ID���擾
		Enemy_Status_Type.SetNomalDropID(std::stoi(enemy_csv_array[y][12].c_str()));
		//csv���烌�A�h���b�v��ID���擾
		Enemy_Status_Type.SetRareDoropID(std::stoi(enemy_csv_array[y][13].c_str()));
		//csv����U������SE��path���擾
		Enemy_Status_Type.SetAttackSe(enemy_csv_array[y][14].c_str());
		//csv���疂�@�͂��擾
		Enemy_Status_Type.SetMagicPower(std::stoi(enemy_csv_array[y][15].c_str()));
		//csv����m�[�}���h���b�v�̊m�����擾����
		Enemy_Status_Type.SetNomalDropProbability(std::stoi(enemy_csv_array[y][16].c_str()));
		//csv���烌�A�h���b�v�̊m�����擾����
		Enemy_Status_Type.SetRareDropProbability(std::stoi(enemy_csv_array[y][17].c_str()));

		Enemy_Type_Array.emplace_back(Enemy_Status_Type);
	}


}

void Enemy::EnemyInit()
{
	//�G���[�p�̔z����N���A���ď���������
	null_array.clear();
}

Enemy::EnemyConnection Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(Enemy_Type_Array.begin(), Enemy_Type_Array.end(), [id]
	(const EnemyConnection& status) {return status.GetEnemyId() == id; });

	if (it != Enemy_Type_Array.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return EnemyConnection{};
	}
}

//�A�C�e���̃h���b�v�����߂�
void Enemy::InitEnemyItemDrop(int EnemyID)
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
void Enemy::InitEnemyArray(int id)
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
	//�{�X�G���A�ɂ���G(����)
	else if (id == 4) {

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
		battle_scene_->DeadEnemyProcces(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave(), enemy_array[enemy_index]);

		//���x���A�b�v����
		battle_scene_->ChackPlayerLevelUp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave());

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


MobMonster::MobMonster(const int enemy_id)
{
	//�^�C�v�����u�ɂ���
	enemy_type = Enemytype::MOB;

	//�G��z��Ɋi�[����
	InitEnemyArray(enemy_id);

	//�C���f�b�N�X��1�`4�̃����_���Ȑ����ɐݒ肷��
	enemy_index = rand() % 5;

	//�V�����z��𐶐�����
	auto& enemy_ = enemy_array[enemy_index];

	//�G�̃O���t�B�b�N�n���h����ǂݍ���
	enemy_hdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy_.GetEnemyGhdl().c_str());

	//�G�̃A�C�e���h���b�v���i�[����
	InitEnemyItemDrop(enemy_.GetEnemyId());
}

//�G�������X�^�[�̍U������
void MobMonster::EnemyAction(Shared<BattleLog>battle_log)
{
	//�v���C���[�̖h��͂��擾����
	auto PlayerDefance = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetDefance();

	//�_���[�W���v�Z����u
	int damage = enemy_array[enemy_index].GetEnemyAttack() - PlayerDefance;

	//�_���[�W��0�������Ƃ���0�ɐݒ肷��
	if (damage <= 0) { damage = 0; }

	//�v���C���[��Hp�����炷
	GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().GetcurentHp() - damage);

	//�J�����V�F�C�N���N����(�_���[�W���󂯂Ă��銴�����o��)
	// ��������\��
	//GameManager::getGameManager()->getCamera()->StartShake(1, 1.5f);

	//�o�g�����O�𗬂�
	battle_log->addDamagePlayerLog("Enemy", "Player", damage);

	// SE�𗬂�
	SoundManager::getSoundManager()->sound_Play(enemy_array[enemy_index].GetSeAttack().c_str(), DX_PLAYTYPE_BACK);

	//�{�����[����ς���
	SoundManager::getSoundManager()->ChangeSoundVolume(80, enemy_array[enemy_index].GetSeAttack().c_str());


}

//------------------------------------------------------------------------------------------------------------
//
//�{�X�����X�^�[
//
//-------------------------------------------------------------------------------------------------------------


BossMonster::BossMonster(int enemy_id) {

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
	enemy_hdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy.GetEnemyGhdl().c_str());

	//�G�̃A�C�e���h���b�v���i�[����
	InitEnemyItemDrop(enemy.GetEnemyId());
}

//�G�̃X�L���z��̏�����
void BossMonster::InitEnemySkill(int enemy_id)
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

		break;

	case static_cast<int>(BossType::GROVEGUARDIAN):

		//���ꂼ��̃X�L�����i�[����
		enemy_skill_.emplace_back(std::make_shared<ChaosFlare>());
		enemy_skill_.emplace_back(std::make_shared<DeathScytheWind>());
		enemy_skill_.emplace_back(std::make_shared<DrakClaw>());

		break;

	default:

		tnl::DebugTrace("-------------------------------------------------------------");
		tnl::DebugTrace("�X�L�����������ł��܂���ł���");
		tnl::DebugTrace("-------------------------------------------------------------");

		break;
	}
}

//�{�X�����X�^�[�̍U������
void BossMonster::EnemyAction(Shared<BattleLog> battle_log)
{
	//�G�̃X�L���z�񂪋󂶂�Ȃ�������
	if (!enemy_skill_.empty()) {
		
		//�����_���ŃC���f�b�N�X���擾����(�X�L���̑�����3�̈� 0 ���� 2 �̃C���f�b�N�X���擾����)
		enmey_skill_index = rand() % 3;

		//�v���C���[�̃X�e�[�^�X
		auto& player_status = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave();

		//�v���C���[�ɍU������
		enemy_skill_[enmey_skill_index]->SkillUse(player_status, enemy_array[enemy_index], battle_log);

		//�v���C���[��hp��0�����������0�ɐݒ肷��
		if (player_status.GetcurentHp() <= 0) {
			player_status.SetPlayerCurentHp(0);
		}
	}
	else {
		tnl::DebugTrace("-------------------------------------------------------------");
		tnl::DebugTrace("�U���ł��܂���ł���");
		tnl::DebugTrace("-------------------------------------------------------------");
		return;

	}
}
