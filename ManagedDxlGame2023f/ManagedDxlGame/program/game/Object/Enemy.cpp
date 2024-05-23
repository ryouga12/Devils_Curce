#include "Enemy.h"
#include"../Scene/battleScene.h"
#include"../Menu/BattleLog.h"
#include"../Manager/GameManager.h"
#include"../Object/Player.h"

Enemy::Enemy()
{
	//csv�̓ǂݍ���	
	Enemy_Csv_Array = tnl::LoadCsv<std::string>("csv/Enemy.csv");

	//Enemy�̃X�e�[�^�X�����[�h����֐�
	EnemyLoadTypeInfo();
	
	//Enemy�̏�����
	EnemyInit();

	item = std::make_shared<Item>();


	actor_type = Actor::ActorType::ENEMY;
}

//�`�揈��
void Enemy::Draw()
{
	if (!dead_enemy_flag) {
		DrawRotaGraph(600, 320, 0.8f, 0, enemy_hdl, true);
	}
}

void Enemy::EnemyLoadTypeInfo()
{

	int max_num = Enemy_Csv_Array.size();

	EnemyStatus_Total_Num = max_num;

	for (int y = 1; y < max_num; y++) {
		
		//csv���疼�O�̎擾
		Enemy_Status_Type.SetEnemyName(Enemy_Csv_Array[y][0].c_str());
		//csv����id�̎擾
		Enemy_Status_Type.SetEnemyId(std::stoi(Enemy_Csv_Array[y][1].c_str()));
		//csv����hp�̎擾
		Enemy_Status_Type.SetEnemyHp(std::stoi(Enemy_Csv_Array[y][2].c_str()));
		//csv����U���͂̎擾
		Enemy_Status_Type.SetEnemyAttack(std::stoi(Enemy_Csv_Array[y][3].c_str()));
		//csv����h��͂̎擾
		Enemy_Status_Type.SetEnemyDefance(std::stoi(Enemy_Csv_Array[y][4].c_str()));
		//csv���炷�΂₳�̎擾
		Enemy_Status_Type.SetEnemySpeed(std::stoi(Enemy_Csv_Array[y][5].c_str()));
		//csv����o���l�̎擾
		Enemy_Status_Type.SetEnemyExpoint(std::stoi(Enemy_Csv_Array[y][6].c_str()));
		//csv���炨���̎擾
		Enemy_Status_Type.SetEnemyMoney(std::stoi(Enemy_Csv_Array[y][7].c_str()));
		//csv����ghpath�̎擾
		Enemy_Status_Type.SetGhdl(Enemy_Csv_Array[y][8].c_str());
		//csv���牊�ϐ��l���擾
		Enemy_Status_Type.SetFireResist(std::stoi(Enemy_Csv_Array[y][9].c_str()));
		//csv����X�ϐ��l�̎擾
		Enemy_Status_Type.SetIceResist(std::stoi(Enemy_Csv_Array[y][10].c_str()));
		//csv���痋�ϐ��l�̎擾
		Enemy_Status_Type.SetThunderResist(std::stoi( Enemy_Csv_Array[y][11].c_str()));
		//csv����ʏ�h���b�v��ID���擾
		Enemy_Status_Type.SetNomalDropID(std::stoi(Enemy_Csv_Array[y][12].c_str()));
		//csv���烌�A�h���b�v��ID���擾
		Enemy_Status_Type.SetRareDoropID(std::stoi(Enemy_Csv_Array[y][13].c_str()));

		Enemy_Type_Array.emplace_back(Enemy_Status_Type);


	}


}

void Enemy::EnemyInit()
{
	//�G���[�p�̔z����N���A���ď���������
	Null_Array.clear();
}

Enemy::EnemyStatus Enemy::GetEnemyStatus(int id) const
{
	auto it = std::find_if(Enemy_Type_Array.begin(), Enemy_Type_Array.end(), [id]
	(const EnemyStatus& status) {return status.getEnemyId() == id; });

	if (it != Enemy_Type_Array.end()) {
		return *it;
	}
	else {
		// ID�ɑΉ�����A�C�e����������Ȃ��ꍇ�́A�K���ȃf�t�H���g�l��Ԃ����G���[�������s���Ȃ�
		return EnemyStatus{};
	}
}

//�A�C�e���̃h���b�v�����߂�
void Enemy::InitEnemyItemDrop(int EnemyID)
{
	Enemy_Drop_Item.clear();

	//�z���ID���r���ĊY������ID���C���x���g���Ɋi�[����
	for (int i = 0; i < Enemy_Csv_Array.size(); i++) {

		//�z���1�s�ڂ͐����̈�+1���Ċm�F���s��
		if (EnemyID == std::stoi(Enemy_Csv_Array[i + 1][1].c_str())) {

			//�C���x���g���ɂ��ꂼ��ɑΉ������A�C�e�����i�[
			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][12].c_str())));
 			Enemy_Drop_Item.emplace_back(item->GetItemById(std::stoi(Enemy_Csv_Array[i + 1][13].c_str())));
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
	Enemy_Array.clear();

	//�����̓G(�ア)
	if (id == 1) {
		for (int i = 0; i < Enemy_num; i++) {
			Enemy_Array.emplace_back(GetEnemyStatus(i));
		}
	}
	//�����̓G(����)
	else if (id == 2) {

		for (int i = 6; i < Enemy_Csv_Array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (Enemy_num > Enemy_Array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(Enemy_Csv_Array[i][1].c_str()));

				//�G��z��Ɋi�[����
				Enemy_Array.emplace_back(GetEnemyStatus(enmeyid.getEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	//�r���̓G(��������)
	else if (id == 3) {

		for (int i = 11; i < Enemy_Csv_Array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (Enemy_num > Enemy_Array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(Enemy_Csv_Array[i][1].c_str()));

				//�G��z��Ɋi�[����
				Enemy_Array.emplace_back(GetEnemyStatus(enmeyid.getEnemyId()));
			}
			//�G�̍ő�̐����������珈���𔲂���
			else {
				break;
			}
		}
	}
	//�{�X�G���A�ɂ���G(����)
	else if (id == 4) {

		for (int i = 16; i < Enemy_Csv_Array.size(); i++) {

			//�G�ɂ����ID���擾����
			//�T�C�Y��5�ɂȂ�܂ŉ�
			if (Enemy_num > Enemy_Array.size()) {
				auto enmeyid = GetEnemyStatus(std::stoi(Enemy_Csv_Array[i][1].c_str()));

				//�G��z��Ɋi�[����
				Enemy_Array.emplace_back(GetEnemyStatus(enmeyid.getEnemyId()));
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
	if (Enemy_Array[enemy_index].getEnemyHp() <= 0) {

		//�G������ł��玀�S�������Ăяo��
		battleScene->DeadEnemyProcces(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave(), Enemy_Array[enemy_index]);

		//���x���A�b�v����
		battleScene->ChackPlayerLevelUp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave());

		return true;
	}
	else {
		return false;
	}
	
}

//�G�̃|�C���^������������
void Enemy::InitEnemyPointer(Shared<Enemy>& enemy_pointer,int enemy_id)
{
	switch (enemy_type)
	{
	case Enemy::Enemytype::NONE:
		break;
	
	//���u�����X�^�[�̃|�C���^���i�[����
	case Enemy::Enemytype::MOB:

		enemy_pointer = std::make_shared<MobMonster>(enemy_id);

		break;

	//�{�X�̃|�C���^���i�[����
	case Enemy::Enemytype::BOSS:

		enemy_pointer = std::make_shared<BossMonster>();
		break;

	default:
		
		tnl::DebugTrace("-----------------------------------------------------------------------------------\n");
		tnl::DebugTrace("�G�̃|�C���^�𐶐��ł��܂���ł���");
		tnl::DebugTrace("-----------------------------------------------------------------------------------\n");

		break;
	}
}

//------------------------------------------------------------------------------------------------------------
//
//�G�������X�^�[
//
//-------------------------------------------------------------------------------------------------------------


MobMonster::MobMonster(int enemy_id)
{
	//�G��z��Ɋi�[����
	InitEnemyArray(enemy_id);

	//�C���f�b�N�X��1�`4�̃����_���Ȑ����ɐݒ肷��
	enemy_index = rand() % 5;

	//�V�����z��𐶐�����
	auto& enemy_ = GetEnemyArray()[enemy_index];

	//�G�̃O���t�B�b�N�n���h����ǂݍ���
	enemy_hdl = ResourceManager::getResourceManager()->LoadGraphEX(enemy_.getEnemyGhdl().c_str());

	//�G�̃A�C�e���h���b�v���i�[����
	InitEnemyItemDrop(enemy_.getEnemyId());
}

//�G�������X�^�[�̍U������
void MobMonster::EnemyAction(Shared<BattleLog>battle_log)
{
	//�v���C���[�̖h��͂��擾����
	auto PlayerDefance = GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getDefance();

	//�_���[�W���v�Z����u
	int damage = Enemy_Array[enemy_index].getEnemyAttack() - PlayerDefance;

	//�_���[�W��0�������Ƃ���0�ɐݒ肷��
	if (damage <= 0) { damage = 0; }

	//�v���C���[��Hp�����炷
	GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().SetPlayerCurentHp(GameManager::getGameManager()->getPlayer()->getPlayerStatusSave().getcurentHp() - damage);

	//�o�g�����O�𗬂�
	battle_log->addDamagePlayerLog("Enemy", "Player", damage);

}
